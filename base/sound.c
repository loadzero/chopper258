//
// Sound handling, synthesis and timing routines for chopper3.
//
// Emulates a PC speaker by emitting square waves at the appropriate frequencies.
//
// The game thread produces sounds by calling 'set_freq(hz)' at particular intervals.
// When set_freq is called, a datapoint '(timestamp, freq)' is pushed onto the sound buffer.
//
// The audio thread consumes these points at regular intervals, and uses them to control the
// frequency of the square waves it is producing.
//
// A stable least squares regressor is used to estimate clock skew between the audio thread and
// game thread, and controls the synthesis rate.
//
// This prevents the audio thread from either underrunning or overrunning, which sounds glitchy.
//
// See regressor.h for details about the least square regressor.
//
// Underrun = audio thread runs out of data to play, results in dropouts
// Overrun  = audio thread lags behind synthesis, audio will be delayed, and old data
//            will get dropped, results in skips.

#include "sound.h"
#include "sdl.h"
#include "shared.h"
#include "regressor.h"

// The SynthBuffer hold SynthPoints in a circular queue of 2048 elements
// The game thread pushes (time, freq) points onto the back of the synth buffer.
// The audio thread consume (time, freq) points from the front of the synth buffer.
struct SynthPoint
{
    int id;
    int64_t sample;
    int freq;
};

typedef struct SynthPoint SynthPoint;

#define QSIZE 2048
struct SynthBuffer
{
    SynthPoint q[QSIZE];
    int front;
    int back;

} synth_buffer;

// Stats to help debug sound sync issues from the pov of the audio thread.
struct SoundDebug
{
    int id;
    int64_t sample;
    int qsize;
    int64_t last;
    const char *rstate;
};

typedef struct SoundDebug SoundDebug;

// Least Square Regressor used to account for clock skew and jitter.
// See regressor.h
StableRegressor reg;
SDL_AudioDeviceID dev;

int64_t audio_time;
int64_t audio_pos;

int point_id_ = 1;
int64_t nanos_per_samp = 1000000000 / 48000;

double slope = 48000.0;
double intercept;

#include "include/base/sound_proto.h"

// Initialize the stable regressor
// Init SDL Audio for 48Khz 16-bit with our callback
// Open sound device
// Launch audio thread
void sound_init()
{
    reg_init(&reg, 400);

    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
        logprintf("Audio driver %d: %s\n", i, SDL_GetAudioDriver(i));
    }

    logprintf("[SDL] Audio driver: %s\n", SDL_GetCurrentAudioDriver());

    SDL_AudioSpec want, have;

    memset(&want, 0, sizeof(want));
    memset(&have, 0, sizeof(have));

    want.freq = 48000;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 1024;
    want.callback = audio_callback;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);

    logprintf("have freq %d channels %d samples %d\n", have.freq, have.channels, have.samples);
    SDL_PauseAudioDevice(dev, 0); /* play! */
}

// Generate `len` bytes of PC speaker sound into stream
//
// This callback is called from the SDL audio thread when it is ready to receive
// more data. This will usually occur when the sound card has finished playing
// some sound and has freed up some space in its buffer for more.
//
// To produce the PC speaker sound, square waves are generated with the
// frequency and timing provided by synth datapoints consumed from the
// synth_buffer.
//
// These datapoints are placed into the synth_buffer by the game thread when it
// wants to play a sound in response to some game event.
//
// After generating the sound, the current time and audio position are pushed
// into the stable regressor.
//
// This allows the game thread to predict the audio position and control the
// synthesis rate by placing datapoints 50ms in front of the estimated audio
// position.
//
void audio_callback(void* userdata, uint8_t* stream, int len)
{
    len = len/2;

    short *buf = (short*)(stream);
    SoundDebug dbg;

    for (int i = 0; i < len; ++i)
    {
        int wavesamp = synth_buffer_get_wavesamp(audio_pos, &dbg);
        short samp = 0;

        if (wavesamp != 0)
        {
            int t = audio_pos % wavesamp;
            double frac = 1.0 / wavesamp;
            int h = wavesamp/2;

            (void)(h);
            (void)(frac);
            int v = -1 + 2 * (int) (t < h);

            samp = v * 16000;
        }

        static int maxq = 0;

        if (dbg.qsize > maxq)
        {
            maxq = dbg.qsize;
            logprintf("sound maxq %d\n", maxq);
        }

        buf[i] = samp;
        audio_pos++;
    }

    audio_time = clocknanos();
    reg_push(&reg, audio_time / 1000000000.0, audio_pos / 48000.0);
}

// set_freq is how sound information gets from the game thread to the audio thread.
//
// The game code will call set_freq with various values at certain time intervals, to produce
// different kinds of sounds.
//
// For example, the nuke drop sound is a frequency sweep from 600hz down to 500hz, produced by 
// the game sending a series of decreasing tones every 30ms or so.
//
// When the game sends a tone, the current time is obtained, and used with the regressor to produce an
// estimate of the audio threads' current reading position.
//
// The SynthPoint is placed onto the back of the SynthBuffer with a playback time intended to be
// 50ms in front the the current audio thread position.
//
// The intent is for any clock skew or jitter to be smoothed over, and prevent underruns and
// overruns.

void set_freq(int freq) // locked
{
    SDL_LockAudioDevice(dev);

    // use the stable regressor to estimate the current audio_pos
    // and push the synth point 50ms into the audio thread's future.

    if (reg_ready(&reg))
    {
        slope = reg_slope(&reg);
        intercept = reg_intercept(&reg);
    }

    int64_t now = clocknanos();

    double x = now / 1000000000.0;
    double y = slope * x + intercept;
    int64_t sample = llround((y + 0.05) * 48000);

    SynthPoint p;
    p.id = point_id_++;
    p.sample = sample;
    p.freq = freq;

    synth_buffer_push_back(p);

    SDL_UnlockAudioDevice(dev);
}

// locked by game thread (in set_freq)
static void synth_buffer_push_back(SynthPoint p) 
{
    // drop old samples if full
    if (qsize() == QSIZE)
        synth_buffer.front++;

    synth_buffer.q[mask(synth_buffer.back++)] = p;
}

// locked by audio thread (audio_callback is called by SDL in a locked dev context)
static int synth_buffer_get_wavesamp(int64_t sample, SoundDebug *dbg) // loc
{
    memset(dbg, 0, sizeof(*dbg));

    if (synth_buffer.front == synth_buffer.back)
    {
        dbg->rstate = "q.empty";
        return 0;
    }

    int i = 0;

    for (i = 0; i < qsize(); ++i)
    {
        SynthPoint p = synth_buffer.q[mask(synth_buffer.front + i)];

        if (p.sample > sample)
            break;
    }

    // too early. there is a synth in the buffer, but its not until later.
    if (i == 0)
    {
        dbg->rstate = "early";
        return 0;
    }

    // its the previous element.
    i--;

    SynthPoint p = synth_buffer.q[mask(synth_buffer.front + i)];

    dbg->id = p.id;
    dbg->sample = p.sample;
    dbg->qsize = qsize();
    dbg->rstate = "found";
    dbg->last = qback()->sample;

    trim(i);

    if (p.freq)
        return llround(48000 * 1.0 / p.freq);

    return 0;
}

static int mask(int i) { return i % QSIZE; }
SynthPoint* qback() { return synth_buffer.q + mask(synth_buffer.back-1); }
void trim(int pi) { synth_buffer.front += pi; }
size_t qsize() { return synth_buffer.back - synth_buffer.front; }
