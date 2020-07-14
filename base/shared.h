#pragma once

// All the shared structures and entry points between the threads are defined here.

#include <stdint.h>
#include <stdbool.h>

#define BPP 2
#define BYTES_PER_LINE  (320 * BPP / 8)
#define PIXELS_PER_BYTE  (8 / BPP)

// Shared between turbo.c (game thread writer) and render.c (main thread reader)
//
// The game thread updates RenderMem when TP drawing functions are called
// The main thread reads RenderMem once a frame before rendering it.
// There is no synchronization, as the original game has no vsync,
// so using a data race is accurate.

struct FrameBuffer
{
    uint8_t pixels[200 * BYTES_PER_LINE];

    uint16_t palette; // 0 - 3
    uint16_t color;   // 0 - 3
    uint16_t bkcolor; // 0 - 15
};

typedef struct FrameBuffer FrameBuffer;
extern FrameBuffer fb;

// main thread inits fonts with this entry point
void font_init();

// main thread launches the game thread with this entry point
void* gamecode(void *arg);

// Used by the game thread to change the PC speaker frequency
// (pushes onto a synth buffer)
// The audio thread consumes this data and produces sound
void set_freq(int);

// Used by the game thread to consume keypresses from the keyboard buffer
// The main thread produces keypresses into the keyboard buffer
int popkey();
bool haskey();

// used by game thread to decode scancodes it consumes
int keydecode(int scancode, int *special);

#ifndef NDEBUG
void logprintf(const char* fmt, ...);
#else
#define logprintf(fmt, ...)
#endif

// Used by all threads to time things.
int64_t rawclocknanos();
int64_t clocknanos();
int64_t epochnanos();
int64_t threadnanos();
