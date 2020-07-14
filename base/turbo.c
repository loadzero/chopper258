//
// Contains an implementation of the Turbo Pascal routines used in Chopper
// Commando. The game thread uses these routines to handle input, drawing,
// timing and sound.
//
// This is not a complete implementation, just what the game needs. Lots of
// functions are missing, and lots of options inside functions are not
// supported either.
//
// In keeping with the original chopper code, lowercase names are used for TP
// routines.

#include <stdint.h>
#include <stdbool.h>

#include "shared.h"
#include "turbo.h"
#include "font.h"

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <poll.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Ownership in this module because it manipulates it the most.
// render.c reads from fb to output frames to SDL
// See framebuffer.h for details
FrameBuffer fb;

// Circular Queue used for flood filling
struct FillPoint
{
    int16_t x;
    int16_t y;
};
typedef struct FillPoint FillPoint;

static FillPoint fillq[1024];
int q_front;
int q_back;

// Bitmap used for flood filling
static uint8_t seen[(320*200)/8];

// TP graphics state variables
static uint16_t fillstyle_pattern;
static uint16_t fillstyle_color;

static uint16_t textstyle_font;
static uint16_t textstyle_direction;
static uint16_t textstyle_charsize;

static int16_t pen_x;
static int16_t pen_y;

static int16_t cursor_x;
static int16_t cursor_y;

// Extra state to handle special keys
static int bufkey;
static int hasbuf;

#include "include/base/turbo_proto.h"

#define SWAP_INT(x,y) { x = x ^ y; y = y ^ x; x = x ^ y; }

static void clear_framebuffer() { memset(fb.pixels, 0, sizeof(fb.pixels)); }

// Resets most of the graphics state
static void reset_graphics(uint16_t c)
{
    clear_framebuffer();
    fb.palette = c;
    fb.bkcolor = 0;

    fillstyle_pattern = 1;
}

// Sets the system to graphics mode and clears the screen.
void setgraphmode(uint16_t c)
{
    reset_graphics(c);
}

// Color state utilities
void setbkcolor(uint16_t c)   { fb.bkcolor = c; }
void setcolor(uint16_t color) { fb.color = color; }
static uint16_t getcolor()    { return fb.color; }

// Another graphics resetting routine.
void cleardevice()
{
    set_font_size(0);
    clear_framebuffer();
}

// Another graphics resetting routine.
void clrscr()
{
    clear_framebuffer();
}

// Another graphics resetting routine.
void graphcolormode()
{
    reset_graphics(0);
}

// Draws a filled rectangle using the given color
static void solid_rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c)
{
    if (x1 > x2)
        SWAP_INT(x1,x2);

    if (y1 > y2)
        SWAP_INT(y1,y2);

    for (int y = y1; y <= y2; ++y)
    for (int x = x1; x <= x2; ++x)
        plot(x,y,c);
}

// Draw a rectangle outline using the given color
static void rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c)
{
    plot_line(x1, y1, x2, y1, c);
    plot_line(x2, y1, x2, y2, c);
    plot_line(x2, y2, x1, y2, c);
    plot_line(x1, y2, x1, y1, c);
}

// Draws a rectangle using the current line style and color.
void rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    rect(x1, y1, x2, y2, fb.color);
}

// Draw a solid rectangle using the current fillstyle
void bar(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    solid_rect(x1,y1,x2,y2,fillstyle_color);
}

// Sleep for approximately ms millis.
void delay(uint16_t ms)
{
    udelay(ms * 1000);
}

// Sleep for approximately ns nanos.
static void nsleep(int64_t ns)
{
    struct timespec req;
    struct timespec rem;

    req.tv_sec = ns / 1000000000;
    req.tv_nsec = ns % 1000000000;

    nanosleep(&req, &rem);
}

// Sleep for approximately us micros.
void udelay(unsigned us)
{
    int64_t ns = us * 1000LL;
    nsleep(ns);
}

// Reads a character from the keyboard buffer. (blocking)
//
// There is some special handling around special keys.
// A special key comes from the keyboard buffer as an int, but must be returned to game
// in two separate calls to readkey, a zero and then the key.
//
// hasbuf and bufkey handle this buffering for the special case.
char readkey()
{
    if (hasbuf)
    {
        hasbuf = 0;
        return bufkey;
    }

    int sc = popkey();

    int special = 0;
    int k = keydecode(sc, &special);

    if (special)
    {
        hasbuf = 1;
        bufkey = k;

        return 0;
    }

    return k;
}

// Is there a key ready to be read (non blocking)
bool keypressed() { return hasbuf || haskey(); }

// dec/int helpers

void dec_by(int16_t *v, int16_t amt) { (*v) -= amt; }
void inc_by(int16_t *v, int16_t amt) { (*v) += amt; }
void dec(int16_t *v) { (*v)--; }
void inc(int16_t *v) { (*v)++; }

// Used by gamecode to allocate memory for remembering terrain holes.
void* getmem(uint16_t n) { return malloc(n); }
void freemem(void *addr, uint16_t n) { free(addr); }

// Used by gamecode to return the number of bytes needed to store an image.
uint16_t imagesize(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    int rows = (y2 - y1) + 1;
    int cols = (((x2 - x1) + 1) + 7) / 8;
    int bpp = 2;
    int hdr = 6;

    return hdr + rows * cols * bpp;
}

// Plot a pixel at x,y with the given color
void plot(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || x >= 320) return;
    if (y < 0 || y >= 200) return;

    int16_t col = x / 4;
    int16_t rem = x % 4;

    uint16_t byte_idx = y * BYTES_PER_LINE + col;
    uint8_t byte = fb.pixels[byte_idx];

    uint8_t shift = (3-rem) * 2;
    uint8_t mask = 3 << shift;
    uint8_t bits = color << shift;

    byte = bits | (byte & ~mask);
    fb.pixels[byte_idx] = byte;
}

// Draw a line from x0,y0 to x1,y1 with the given color
// Uses the bresenham algorithm from wikipedia.
void plot_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c)
{
    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
            plot_line_low(x1, y1, x0, y0, c);
        else
            plot_line_low(x0, y0, x1, y1, c);
    }
    else
    {
        if (y0 > y1)
            plot_line_high(x1, y1, x0, y0, c);
        else
            plot_line_high(x0, y0, x1, y1, c);
    }
}

// Synonymous with plot_line
void draw(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    plot_line(x1,y1,x2,y2,color);
}

void setlinestyle(uint16_t linestyle, uint16_t pattern, uint16_t thickness) {}

// Plot line using current color
void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    plot_line(x1,y1,x2,y2,getcolor());
}

void moveto(int16_t x, int16_t y)
{
    pen_x = x;
    pen_y = y;
}

void lineto(int16_t x, int16_t y)
{
    plot_line(pen_x, pen_y, x, y, fb.color);
    pen_x = x;
    pen_y = y;
}

// Bresenham line drawing - adapted from the wikipedia algorithm
static void plot_line_low(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t yi = 1;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    int16_t D = 2*dy - dx;
    int16_t y = y0;

    for (int16_t x = x0; x <= x1; ++x)
    {
        plot(x, y, c);

        if (D > 0)
        {
            y = y + yi;
            D = D - 2*dx;
        }

        D = D + 2*dy;
    }
}

static void plot_line_high(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t xi = 1;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int16_t D = 2*dx - dy;
    int16_t x = x0;

    for (int16_t y = y0; y <= y1; ++y)
    {
        plot(x, y, c);
        if (D > 0)
        {
            x = x + xi;
            D = D - 2*dy;
        }
        D = D + 2*dx;
    }
}

// bresenham circle drawing routine
static void raster_circle(
        int x0,
        int y0,
        int radius,
        uint16_t color)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    plot(x0, y0 + radius, color);
    plot(x0, y0 - radius, color);
    plot(x0 + radius, y0, color);
    plot(x0 - radius, y0, color);
 
    while(x < y) 
    {
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;    
        plot(x0 + x, y0 + y, color);
        plot(x0 - x, y0 + y, color);
        plot(x0 + x, y0 - y, color);
        plot(x0 - x, y0 - y, color);
        plot(x0 + y, y0 + x, color);
        plot(x0 - y, y0 + x, color);
        plot(x0 + y, y0 - x, color);
        plot(x0 - y, y0 - x, color);
    }
}

void circle(int16_t x, int16_t y, uint16_t radius, uint16_t color)
{
    raster_circle(x, y, radius, color);
}

// Not a true ellipse drawint algorithm. Draws a quarter circle only, for the radar dish.
void ellipse(int16_t x0, int16_t y0, uint16_t start_angle, uint16_t end_angle, uint16_t xrad, uint16_t yrad)
{
    assert(xrad == yrad);
    assert(start_angle == 270);
    assert(end_angle == 0);

    int color = fb.color;
    int radius = xrad;

    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    plot(x0, y0 + radius, color);
    plot(x0 + radius, y0, color);
 
    while(x < y) 
    {
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        plot(x0 + y, y0 + x, color);
        plot(x0 + x, y0 + y, color);
    }
}

// Get the pixel value at x,y
static uint16_t get_pixel(int16_t x, int16_t y)
{
    if (x < 0 || x >= 320)
        return 0;

    if (y < 0 || y >= 200)
        return 0;

    int16_t col = x / 4;
    int16_t rem = x % 4;

    uint16_t byte_idx = y * BYTES_PER_LINE + col;
    uint8_t byte = fb.pixels[byte_idx];
    uint8_t shift = (3-rem) * 2;

    return (byte >> shift) & 0x3;
}

// Floodfill the region bounded by border color starting at x,y with the current fill pattern.
void floodfill(int16_t x, int16_t y, uint16_t bordercolor)
{
    fillshape(x,y,fillstyle_color,bordercolor);
}

// Floodfill the region bounded by border color starting at x,y with the current fill pattern.
void fillshape(int16_t x, int16_t y, uint16_t fillcolor, uint16_t bordercolor)
{
    // Fill patterns are 8x8 - a 64 bit bitmap
    // E.g., This is the human readable slashed fill, used
    // for drawing water.
    
    //    "..#....."   0x40
    //    "...#...."   0x80
    //    "....#..."   0x01
    //    ".....#.."   0x02
    //    "......#."   0x04
    //    ".......#"   0x08
    //    "#......."   0x10
    //    ".#......"   0x20
    
    static const uint64_t patterns[] = {
        0,
        0xffffffffffffffffull,    // solid fill
        0,
        0x4080010204081020ull     // slashed fill, used for drawing water
    };

    do_fillshape(x,y,fillcolor, bordercolor, patterns[fillstyle_pattern]);
}

void setfillstyle(uint16_t pattern, uint16_t color)
{
    fillstyle_pattern = pattern;
    fillstyle_color = color;
}

// Floodfill the region bounded by border color starting at x,y with the given fill color and fill pattern
// Uses a breadth first search to do the fill.
//
// The queue is implemented as a circular buffer.
// The discovered vertices are tracked using the bitmap 'seen'
static void do_fillshape(int16_t x, int16_t y, uint16_t fillcolor, uint16_t bordercolor, uint64_t pattern)
{
    memset(seen, 0, sizeof(seen));
 
    q_clear();
    q_push(make_point(x,y));

    int dirs[][2] = { {0, -1}, {1,0}, {0,1}, {-1, 0} };
    plot(x, y, fillcolor);
    mark_seen(seen,x,y);

    while (!q_empty())
    {
        FillPoint p = q_pop();

        for (int i = 0; i < 4; ++i)
        {
            FillPoint n = make_point(p.x + dirs[i][0], p.y + dirs[i][1]);

            if (n.x < 0 || n.x >= 320)
                continue;

            if (n.y < 0 || n.y >= 200)
                continue;

            if (has_seen(seen, n.x, n.y))
                continue;

            if (get_pixel(n.x, n.y) == bordercolor)
                continue;

            int bit = 63 - ((n.y % 8) * 8 + (n.x % 8));
            int on = (pattern >> bit) & 0x1;
            mark_seen(seen, n.x, n.y);

            plot(n.x, n.y, fillcolor & -on);
            q_push(n);
        }
    }
}

// Flood filling utility functions
static FillPoint make_point(int16_t x, int16_t y)
{
    FillPoint p;
    p.x = x;
    p.y = y;

    return p;
}

static bool has_seen(uint8_t *seen, int16_t x, int16_t y)
{
    int bit = y * 320 + x;
    int byte = bit / 8;
    int modbit = bit % 8;

    return seen[byte] & (1 << (7-modbit));
}

static void mark_seen(uint8_t *seen, int16_t x, int16_t y)
{
    int bit = y * 320 + x;
    int byte = bit / 8;
    int modbit = bit % 8;

    seen[byte] |= (1 << (7-modbit));
}

// Flood filling circular queue functions 
static void q_clear() { q_front = 0; q_back = 0; }
static bool q_empty() { return q_front == q_back; }
static int q_mask(int i) { return i % 1024; }

static void q_push(FillPoint p)
{
    fillq[q_mask(q_back++)] = p;
}

static FillPoint q_pop()
{
    FillPoint p = fillq[q_mask(q_front++)];
    return p;
}

// Utility function for getimage
// Get the pixel value at x,y unless x >= the rhs limit 'xc',
// in which case return 0.
static uint16_t get_pixel_clip(int16_t x, int16_t y, int16_t xc)
{
    if (x >= xc)
        return 0;

    return get_pixel(x, y);
}

// Utility function for putimage
// Set the pixel value at x,y to c unless x >= the rhs limit 'xc'
static void set_pixel_clip(int x, int y, int c, int xc)
{
    if (x >= xc)
        return;

    plot(x,y,c);
}

// The routines getimage, putimage, getpic, putpic are sprite creation and drawing routines.
// They are used in the game code to create and draw the sprites for the moving game
// objects such as helicopters, tanks, planes and missiles.

// Copy the screen contents in the rectangle (x1,y2,x2,y2) into ptr.
void getimage(int16_t x1, int16_t y1, int16_t x2, int16_t y2, void *ptr)
{
    uint16_t *hdr = (uint16_t*)(ptr);

    int w = (x2 - x1) + 1;
    int h = (y2 - y1) + 1;

    *hdr++ = 2;
    *hdr++ = w;
    *hdr++ = h;

    uint8_t *body = (uint8_t*)(hdr);

    int rb = (w + 3) / 4;
    int xc = x2 + 1;

    for (int y = y1; y <= y2; ++y)
    {
        int x = x1;

        for (int b = 0; b < rb; ++b)
        {
            int byte = 0;

            byte = get_pixel_clip(x, y, xc) << 6 |
                   get_pixel_clip(x + 1, y, xc) << 4 |
                   get_pixel_clip(x + 2, y, xc) << 2 |
                   get_pixel_clip(x + 3, y, xc);

            *body++ = byte;

            x += 4;
        }
    }
}

// Inverse of getimage
// Copy the sprite in ptr to the screen at x1,y1
void putimage(int16_t x1, int16_t y1, void *ptr, uint16_t bitblt)
{
    assert(bitblt == 0);

    uint16_t *hdr = (uint16_t*)(ptr);
    uint16_t bits = *hdr++;
    int w = *hdr++;
    int h = *hdr++;

    (void)(bits);

    uint8_t *body = (uint8_t*)(hdr);

    int rb = (w + 3) / 4;

    int xc = x1 + w;
    int y2 = y1 + h - 1;

    for (int y = y1; y <= y2; ++y)
    {
        int x = x1;

        for (int b = 0; b < rb; ++b)
        {
            int byte = *body++;

            set_pixel_clip(x + 0, y, (byte >> 6) & 3, xc);
            set_pixel_clip(x + 1, y, (byte >> 4) & 3, xc);
            set_pixel_clip(x + 2, y, (byte >> 2) & 3, xc);
            set_pixel_clip(x + 3, y, (byte >> 0) & 3, xc);

            x += 4;
        }
    }
}

// Synonymous with getimage in this implementation
// IIRC, its flipped vertically in the original TP
void getpic(void *ptr, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    getimage(x1, y1, x2, y2, ptr);
}

// Almost synonymous with putimage.
// Adjust the args of putpic, so we can reuse putimage.
void putpic(void *ptr, int16_t x1, int16_t y1)
{
    uint16_t *hdr = (uint16_t*)(ptr);
    uint16_t bits = *hdr++;
    int w = *hdr++;
    int h = *hdr++;

    (void) (bits);
    (void) (h);
    (void) (w);

    putimage(x1, y1 - (h - 1), ptr, 0);
}

// Returns the current time set in the operating system.
void gettime(uint16_t *Hour, uint16_t *Minute, uint16_t *Second, uint16_t *Sec100)
{
    struct timeval tv;
    int rc = gettimeofday(&tv, NULL);
    (void)(rc);

    int t = tv.tv_sec % 86400;

    int hours = t / (3600);
    int rem = t % 3600;

    int mins = rem / 60;
    rem = rem % 60;

    int secs = rem;
    int sec100 = tv.tv_usec / 100000;

    *Hour = hours;
    *Minute = mins;
    *Second = secs;
    *Sec100 = sec100;
}

// Sets the current text font, style, and character magnification factor.
void settextstyle(uint16_t font, uint16_t direction, uint16_t charsize)
{
    textstyle_font = font;
    textstyle_direction  = direction;
    textstyle_charsize = charsize;

    set_font_size(charsize);
}

// Draws the given glyph at x,y with color.
// Only pixels set to one are drawn.
static void draw_glyph(int x, int y, int color, Glyph *g)
{
    const char *iter = g->data;

    for (int gy = 0; gy < g->h; ++gy)
    {
        for (int gx = 0; gx < g->w; ++gx)
        {
            int on = (*iter++ == '#');

            if (on)
                plot(x + gx, y + gy, color);
        }
    }
}

// Sends a string to the output device.
static void outtextxy_color(int16_t x, int16_t y, const char *s, int color)
{
    int n = strlen(s);

    for (int i = 0; i < n; ++i)
    {
        Glyph* g = getglyph(s[i]);
        draw_glyph(x, y, color, g);
        x += g->w;
    }
}

// Sends a string to the output device.
void outtextxy(int16_t x, int16_t y, const char *s)
{
    outtextxy_color(x,y,s,fb.color);
}

// Sends a 'pascal' string to the output device.
// These will be the non literal strings.
void outtextxyp(int16_t x, int16_t y, const char *s)
{
    outtextxy(x,y,s+1);
}

// send a single char to the output device
void outtextxyc(int16_t x, int16_t y, char c)
{
    char s[2] = { c, 0 };
    outtextxy(x,y,s);
}

// send a single char to the output device
static void outtextxyc_color(int16_t x, int16_t y, char c, int color)
{
    char s[2] = { c, 0 };
    outtextxy_color(x,y,s,color);
}

// Positions the cursor - note, also related to 'Window'
void gotoxy(int16_t x, int16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

static void writefmt(const char *fmt, ...)
{
    set_font_size(0);
    char buf[512];

    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, 512, fmt, ap);
    va_end(ap);

    const char *iter = buf;
    int orig_x = cursor_x;
    char c = 0;

    while ((c = *iter++))
    {
        if (c == '\n')
        {
            // newline
            cursor_x = orig_x;
            cursor_y += 1;
        }
        else
        {
            // text written with write is blacked out first.
            solid_rect((cursor_x - 1) * 8,
                (cursor_y-1) * 8,
                (cursor_x) * 8 - 1,
                (cursor_y) * 8 - 1,
                0);

            outtextxyc_color((cursor_x - 1) * 8, (cursor_y-1) * 8, c, 3);
            cursor_x++;
        }
    }
}

void write_str_i16(const char *v1, int16_t v2)
{
    writefmt("%s %d", v1, v2);
}

void write_str(const char *v1)
{
    writefmt("%s", v1);
}

void writeln_str_i16(const char *v1, int16_t v2)
{
    writefmt("%s %d\n", v1, v2);
}

void writeln_str(const char *v1)
{
    writefmt("%s\n", v1);
}

void writeln_u16_str(uint16_t v1, const char *v2)
{
    writefmt("%d %s\n", v1, v2);
}

void writeln_str_i32_str(const char *v1, int v2, const char *v3)
{
    writefmt("%s %d %s\n", v1, v2, v3);
}

void halt()
{
    exit(0);
}

bool odd(int16_t c)
{
    return (c % 2) == 1;
}

void randomize()
{
    srand(time(NULL));
}

uint16_t random_int(uint16_t range)
{
    return rand() % range;
}

// nop on this impl
void initgraph(int16_t gd, int16_t gm, const char *drv) {}

void nosound()
{
    set_freq(0);
}

void sound(uint16_t hz)
{
    set_freq(hz);
}

int32_t round_(double x)
{
    return round(x);
}

double abs_f64(double x)
{
    return fabs(x);
}

int16_t abs_i16(int16_t x)
{
    return abs(x);
}

void palette(uint16_t p)
{
    fb.palette = p;
}

uint16_t getdotcolor(int16_t x, int16_t y)
{
    if (x < 0 || x >= 320) return 0;
    if (y < 0 || y >= 200) return 0;

    // find the byte affected.
    // each byte has 4 pixels in it.

    int16_t col = x / 4;
    int16_t rem = x % 4;

    uint16_t byte_idx = y * BYTES_PER_LINE + col;
    uint8_t byte = fb.pixels[byte_idx];

    uint8_t shift = (3-rem) * 2;
    return (byte >> shift) & 3;
}

uint16_t getpixel(uint16_t x, uint16_t y)
{
    return getdotcolor(x,y);
}

int memavail() { return 0xfffff; }

void putpixel(int16_t x, int16_t y, uint16_t c)
{
    plot(x,y,c);
}

bool in(int v, int lo, int hi)
{
    return (v >= lo && v <= hi);
}

void pstrcpy(char *pasc, const char *cstr)
{
    strcpy(pasc+1, cstr);
}

int pstrcmp(char *pasc, const char *cstr)
{
    return strcmp(pasc+1, cstr);
}

bool pstreq(const char *pasc, const char *cstr)
{
    return strcmp(pasc+1, cstr) == 0;
}

void int2str( int16_t num , char *ps )
{
    sprintf(ps + 1, "%d", num);
}

void real2str( double num , int width, int dec, char *ps )
{
    char fmt[32]; snprintf(fmt, 32, "%%%d.%df", width, dec);
    sprintf(ps + 1, fmt, num);
}

void nextpalette()
{
    fb.palette = (fb.palette + 1) % 4;
}

void nextbkcolor()
{
    fb.bkcolor = (fb.bkcolor + 1) % 16;
}

