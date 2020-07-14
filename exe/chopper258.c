//
// The entrypoint and main loop of the chopper3 project.
// note: This is the main thread.
//
// The main thread handles interfacing with SDL, and currently communicates with the game thread
// by reading the framebuffer, and writing input down a pipe.
//
// For game code see: base/chopper2.cpp

#include "base/shared.h"
#include "base/sdl.h"
#include "base/input.h"
#include "base/sound.h"
#include "base/render.h"

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <execinfo.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

int64_t basenanos; // initted at main
int frames;

pthread_t game_thread;

int64_t clocknanos();
static void run_game_thread();

int main(int argc, char **argv)
{
    int renderscale = 3;

    if (argc > 1)
        renderscale = atoi(argv[1]);

    basenanos = clocknanos();

    input_init();
    font_init();
    render_init(renderscale);
    sound_init();

    run_game_thread();

    while (true)
    {
        frames++;

        input();
        render();
    }
}

static void run_game_thread()
{
    int rc = pthread_create(&game_thread, NULL, gamecode, NULL);
    assert(rc == 0);
}

int64_t rawclocknanos()
{
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC_RAW, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

int64_t clocknanos()
{
    return rawclocknanos() - basenanos;
}

int64_t epochnanos()
{
    struct timeval tv;
    int rc = gettimeofday(&tv, NULL);
    (void)(rc);

    return tv.tv_sec * 1000000000LL + tv.tv_usec * 1000;
}

int64_t threadnanos()
{
    struct timespec ts;
    clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

#ifndef NDEBUG
void logprintf(const char* fmt, ...)
{
    char buf[4096];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, 4096, fmt, ap);
    va_end(ap);

    int64_t nanos = clocknanos();
    printf("%ld.%09ld %s", nanos / 1000000000, nanos % 1000000000, buf);
}
#endif
