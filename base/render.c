// 
// Main thread CGA luts and rendering routines.
//
// On initialization, some luts are computed for the various CGA combinations of
// background color and palette, so that rendering can transform four pixels at a time.

#include "render.h"
#include "shared.h"
#include "sdl.h"

#include <assert.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static int64_t elapsed;

// The 16 possible CGA colors
uint32_t cga_color_triplets[] = {
    0x000000,  //  Black         0
    0x0000AA,  //  Blue          1
    0x00AA00,  //  Green         2
    0x00AAAA,  //  Cyan          3
    0xAA0000,  //  Red           4
    0xAA00AA,  //  Magenta       5
    0xAA5500,  //  Brown         6
    0xAAAAAA,  //  LightGray     7
    0x555555,  //  DarkGray      8
    0x5555FF,  //  LightBlue.    9
    0x55FF55,  //  LightGreen    10
    0x55FFFF,  //  LightCyan     11
    0xFF5555,  //  LightRed      12
    0xFF55FF,  //  LightMagenta  13
    0xFFFF55,  //  Yellow        14
    0xFFFFFF,  //  White         15
};

// The four CGA palettes.
//
// The first element is the bgcolor and can be changed to any of the 16 colors, meaning
// there are 64 palette combinations in total.
//
uint32_t cga_palettes[4][4] = { {0,10,12,14},      // Black, LightGreen, LightRed, Yellow
                                {0,11,13,15},      // Black, LightCyan, LightMagenta, White
                                {0,2,4,6},         // Black, Green, Red, Brown
                                {0,3,5,7} };       // Black, Cyan, Magenta, LightGray

#define USETRUECOLOR

#ifdef USETRUECOLOR
typedef uint32_t PixelType;
#else
typedef uint16_t PixelType;
#endif

// Storage for luts for the 64 possible combinations
//
// lookup key    = bg_color, palette, input byte value
// output value  = 4 pixels (16 bytes if truecolor, 8 bytes if not)
static PixelType pixlut[16][4][256][4];

#include "include/base/render_proto.h"

// Initialize Rendering
//     Init SDL and create a window of the right dimensions
//     Create a streaming texture to blit pixels onto
//     Compute the lookup tables to speedup render
void render_init(int renderscale)
{
    int rc = SDL_Init(SDL_INIT_EVERYTHING);
    assert(rc == 0);

    window = SDL_CreateWindow("chopper3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            320 * renderscale, 200 * renderscale, 0);
    assert(window);

    SDL_CaptureMouse(SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(renderer);

    texture = SDL_CreateTexture(
            renderer,
            sizeof(PixelType) == 4 ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_ARGB4444,
            SDL_TEXTUREACCESS_STREAMING,
            320, 200);

    assert(texture);

    for (int bg = 0; bg < 16; ++bg)         // for each possible bg color index
    for (int pal = 0; pal < 4; ++pal)       // for each possible palette index
    for (int k = 0; k < 256; ++k)           // for each possible input byte value
    {
        // note - each input byte contains 4 pixels at 2bpp and will produce 4 output pixels
        for (int i = 0; i < 4; ++i)         // for each possible pixel index
        {
            uint8_t byte = k;
            int pi = (byte >> (6-i*2)) & 0x3;                  // 2-bit pixel value at this index
            int ci = (pi == 0) ? bg : cga_palettes[pal][pi];   // determine color index based on pixel value
                                                               // (either bg or from palette)
            uint32_t big_color = 0xff000000 | cga_color_triplets[ci]; // now color index is determined, lookup
                                                                      // the appropriate CGA color

            // truncate color to 16 bit if thats all we need.
            uint16_t sml_color = 
                (((big_color >> 28) & 0xf) << 12) |
                (((big_color >> 20) & 0xf) << 8) |
                (((big_color >> 12) & 0xf) << 4) |
                ((big_color >> 4) & 0xf);

            // Store the pixel in the right place in the lut

            if (sizeof(PixelType) == 4)
                pixlut[bg][pal][byte][i] = big_color;
            else
                pixlut[bg][pal][byte][i] = sml_color;
        }
    }
}

// API for the TP emu layer (game thread) to write to the framebuffer.

// Render the framebuffer.
//
//     Uses the pixlut to copy the framebuffer to the streaming texture four pixels at a time.
//     Does an SDL_RenderCopy to get it to the screen (scaling by renderscale happens here)
//     Finally, do a flip by using SDL_RenderPresent
void render()
{
    PixelType *out;
    int pitch;
    SDL_LockTexture(texture, NULL, (void**) &out, &pitch);

    int64_t tick = threadnanos();

    for (int i = 0; i < 200 * BYTES_PER_LINE; ++i)
    {
        memcpy(out, &pixlut[fb.bkcolor][fb.palette][fb.pixels[i]], sizeof(PixelType) * 4);
        out += 4; // 4 pixels (not bytes)
    }

    int64_t tock = threadnanos();

    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    elapsed += (tock - tick);
}
