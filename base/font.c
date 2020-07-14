//
// Font handling and font data to render the turbo pascal fonts used in chopper.
// The font data is stored as bitmaps in the fontdata.h header file, in a human readable format, similar
// to PCF
//
// Font 0 is the normal 'fat' PC BIOS font
// Fots 4,5,6,10 are the TP stroked fonts of that size

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "font.h"
#include "fontdata.h"
#include "shared.h"

static int fontsize;
#define NUMFONTS (sizeof(fontinfo) / sizeof(fontinfo[0]))
static Glyph fontlut[NUMFONTS][256];

static void read_glyph(Glyph *g, int size, int c);

void set_font_size(int s)
{
    fontsize = s;
}

void font_init()
{
    for (int i = 0; i < NUMFONTS; ++i)
    {
        if (!fontinfo[i].ptr)
            continue;

        for (int c = 0; c < 256; ++c)
            read_glyph(&fontlut[i][c], i, c);
    }
}

// Read a Glyph out of the fontdata
static void read_glyph(Glyph *g, int size, int c)
{
    assert(size >= 0 && size <= 10);
    assert(fontinfo[size].ptr);

    int FW = fontinfo[size].w;
    int FH = fontinfo[size].h;

    int i = c * FW * FH;
    const char *iter = fontinfo[size].ptr + i;

    char hdr[FW+1];
    memcpy(hdr, iter, FW); hdr[FW] = '\0';

    iter += FW;
    char *saveptr;

    const char *sord = strtok_r(hdr, " ", &saveptr);
    const char *sw = strtok_r(NULL, " ", &saveptr);
    const char *sh = strtok_r(NULL, " ", &saveptr);

    int w = atoi(sw);
    int h = atoi(sh);

    memset(g, 0, sizeof(*g));
    g->w = w;
    g->h = h;

    char *out = g->data;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
            *out++ = iter[x];

        iter += FW;
    }

    *out++ = '\0';
//    print(g);
}

// Return a glyph using the fontsize and ord to look it up
Glyph* getglyph(int c)
{
    return &fontlut[fontsize][c];
}

// Debugging routine to print a human readable glyph to stdout
void print(Glyph *g)
{
    char *iter = g->data;

    for (int y = 0; y < g->h; ++y)
    {
        for (int x = 0; x < g->w; ++x)
            logprintf("%c", *iter++);

        logprintf("\n");
    }
}
