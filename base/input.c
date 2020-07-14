//
// Input handling routines for chopper3
//
// This module handles SDL keyboard up/down events and translates them into the form
// expected by the game.
//
// This is done by providing a BIOS style keyboard circular queue, that keypresses are
// placed into with appropriate timing by the main thread.
//
// The keybuffer is 16 elements. Keys are repeated at a rate of 20cps after being held down for 500ms.
//
// The game thread is meant to pop keys off the front of the buffer.
// If the buffer is full, new keys are discarded.
//
// In a real PC, this would cause the BIOS to beep the pc speaker.


#include "base/sdl.h"
#include "base/shared.h"

#include <assert.h>
#include <pthread.h>

extern int frames;

// PC BIOS style circular keyboard buffer
// Key presses are pushed to the back by the main thread, and popped from the front
// by the game thread.
// The mutex and cond variable acheive the necessary thread synchronization.
//
// Note: Most of the circular queues in this code use a pattern similar to this one:
//      https://www.snellman.net/blog/archive/2016-12-13-ring-buffers/
struct KeyBuffer
{
    int q[16];
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int back;
    int front;
} keybuffer;

typedef struct KeyBuffer KeyBuffer;

#define NUM_SCAN 512
static int keymap[NUM_SCAN];

// These are the only especially encoded keys the game uses.
#define SPECIAL_KEY_LIST \
    KEY_MAP(SDL_SCANCODE_F1,         59) \
    KEY_MAP(SDL_SCANCODE_UP,         72) \
    KEY_MAP(SDL_SCANCODE_LEFT,       75) \
    KEY_MAP(SDL_SCANCODE_DOWN,       80) \
    KEY_MAP(SDL_SCANCODE_RIGHT,      77) \
    KEY_MAP(SDL_SCANCODE_DELETE,     83) \
    KEY_MAP(SDL_SCANCODE_HOME,       71) \
    KEY_MAP(SDL_SCANCODE_PAGEUP,     73)

#define KEY_MAP(x,y) x,
static const int special_keys[] = { SPECIAL_KEY_LIST };
#undef KEY_MAP
#define KEY_MAP(x,y) y,
static const int special_keycodes[] = { SPECIAL_KEY_LIST };
#undef KEY_MAP

static const int NUM_SPECIAL = sizeof(special_keys) / sizeof(special_keys[0]);

#include "include/base/input_proto.h"

void input_init()
{
    keybuffer_init(&keybuffer);
}

// Called once per frame.
// Read any waiting events (mostly SDL inputs) and process them.
// Afterwards, detect any held down keys and generate presses if necessary
void input()
{
    SDL_Event e;

    while (true)
    {
        int has = SDL_PollEvent(&e);

        if (!has)
            break;

        process_event(&e);
    }

    // pulse any held down keys
    keyscan();
}

// Initialize the keyboard buffer
static void keybuffer_init(KeyBuffer *kb)
{
    int rc = 0;
    memset(kb, 0, sizeof(*kb));

    rc = pthread_cond_init(&kb->cond, 0);
    assert(rc == 0);
    
    rc = pthread_mutex_init(&kb->mutex, 0);
    assert(rc == 0);

    kb->front = 0;
    kb->back = 0;
}

// Teardown the keyboard buffer
static void keybuffer_fini(KeyBuffer *kb)
{
    pthread_mutex_destroy(&kb->mutex);
    pthread_cond_destroy(&kb->cond);
}

// Return the number of keypresses in the buffer (locked)
static size_t keybuffer_size(KeyBuffer *kb)
{
    pthread_mutex_lock(&kb->mutex);
    size_t n = (kb->back - kb->front);
    pthread_mutex_unlock(&kb->mutex);

    return n;
}

// Convert an unwrapped circular q index into a wrapped one
static int kb_mask(int i) { return i % 16; }

// Lock the buffer and push a new key onto the back if there is room.
// Then, unlock and signal the condition variable that there is data.
static void keybuffer_push_back(KeyBuffer *kb, int k)
{
    {
        pthread_mutex_lock(&kb->mutex);

        size_t qs = (kb->back - kb->front);

        // drop new keys if buffer is full
        if (qs < 16)
            kb->q[kb_mask(kb->back++)] = k;

        pthread_mutex_unlock(&kb->mutex);
    }

    pthread_cond_signal(&kb->cond);
}

// The game thread uses this to block until a key is pressed,
// and then pop it from the front of the buffer.
static int keybuffer_pop_front(KeyBuffer *kb)
{
    int k = 0;

    {
        pthread_mutex_lock(&kb->mutex);

        while (kb->front == kb->back)
            pthread_cond_wait(&kb->cond, &kb->mutex);

        k = kb->q[kb_mask(kb->front++)];

        pthread_mutex_unlock(&kb->mutex);
    }

    return k;
}

// external keybuffer API
int   popkey()       { return keybuffer_pop_front(&keybuffer); }
void  pushkey(int k) { keybuffer_push_back(&keybuffer, k); }
bool  haskey()       { return keybuffer_size(&keybuffer) != 0; }

// Check for quit events, or forward key up/down 
// for further processing
static void process_event(const SDL_Event *e)
{
    if (e->type == SDL_QUIT)
        exit(1);

    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
        process_key_event(e);
}

// If a key down event comes in and the key state
// is currently up, then push the key onto the buffer,
// and set the key state to down (i.e. with the current frame number)
// 
// If a key up event comes in, set the key state to up.
//
// The frame count is used later to pulse keypresses
// for held down keys. (see keyscan)
//
// Note, very first frame is 1, not zero.

static void process_key_event(const SDL_Event *e)
{
    int scancode = e->key.keysym.scancode;

    if (e->type == SDL_KEYDOWN)
    {
        if (keymap[scancode] == 0)
        {
            // immediate keypress this frame
            keymap[scancode] = frames;
            pushkey(scancode);
        }
    }
    else if (e->type == SDL_KEYUP)
    {
        keymap[scancode] = 0;
    }
}

// Called by the input function after all key up/dows have been processed,
// to detect if keys are held down and require pulsing.
static void keyscan()
{
    for (size_t i = 0; i < NUM_SCAN; ++i)
    {
        // key is up, so skip it
        if (keymap[i] == 0)
            continue;

        // How long has the held down ?
        int delta = frames - keymap[i];

        // Do a repeat if held down for half a second or more
        // at a rate of 20 presses per second (every 3 frames)
        if ((delta >= 30 && (delta % 3) == 0))
            pushkey(i);
    }
}

// Used by the game code to get the mouse position.
// Note, we don't support the mouse properly, but return a large value
// to get a humourous result when the mouse is turned on when pressing 'T'
void get_mouse(int *x, int *y)
{
    // careful, this is game thread.
    static int r = 0;
    r += 500;

    *x = 0;
    *y = r;
}

// Used by the TP emulation layer to decode a scancode 
int keydecode(int scancode, int *special)
{
    int si = find_special(scancode);

    if (si != -1)
    {
        *special = 1;
        return special_keycodes[si];
    }

    *special = 0;
    return (int) SDL_GetKeyFromScancode((SDL_Scancode) scancode);
}

// Used by keydecode to detect a special key
static int find_special(int code)
{
    for (int i = 0; i < NUM_SPECIAL; ++i)
    {
        if (special_keys[i] == code)
            return i;
    }

    return -1;
}
