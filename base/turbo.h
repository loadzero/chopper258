#pragma once

#include <stdint.h>
#include <stdbool.h>

void turbo_init();

void cleardevice();
void setgraphmode(uint16_t m);
void setbkcolor(uint16_t c);
void setcolor(uint16_t c);
void clrscr();
void graphcolormode();
void plot(int16_t x, int16_t y, uint16_t color);
void gettime(uint16_t *Hour, uint16_t *Minute, uint16_t *Second, uint16_t *Sec100);
void draw(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillshape(int16_t x, int16_t y, uint16_t fillcolor, uint16_t bordercolor);
void floodfill(int16_t x, int16_t y, uint16_t bordercolor);
void setlinestyle(uint16_t linestyle, uint16_t pattern, uint16_t thickness);
void getpic(void *ptr, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void putpic(void *ptr, int16_t x1, int16_t y1);
void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void getimage(int16_t x1, int16_t y1, int16_t x2, int16_t y2, void *ptr);
void putimage(int16_t x1, int16_t y1, void *ptr, uint16_t bitblt);
void* getmem(uint16_t n);
void freemem(void* addr, uint16_t n);
uint16_t imagesize(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void plot_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c);
void udelay(unsigned us);
void delay(uint16_t ms);
char readkey();
bool keypressed();
void dec_by(int16_t *v, int16_t amt);
void inc_by(int16_t *v, int16_t amt);
void dec(int16_t *v);
void inc(int16_t *v);
void settextstyle(uint16_t font, uint16_t direction, uint16_t charsize);
void outtextxy(int16_t x, int16_t y, const char *s);
void outtextxyp(int16_t x, int16_t y, const char *s);
void outtextxyc(int16_t x, int16_t y, char c);
void rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void bar(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void setfillstyle(uint16_t pattern, uint16_t color);
void gotoxy(int16_t x, int16_t y);
void moveto(int16_t x, int16_t y);
void lineto(int16_t x, int16_t y);
void writeln_str_i16(const char *text, int16_t var);
void writeln_str(const char *v1);
void writeln_u16_str(uint16_t v1, const char *v2);
void writeln_str_i32_str(const char *v1, int v2, const char *v3);
void write_str_i16(const char *v1, int16_t v2);
void write_str(const char *text);
void halt();
bool odd(int16_t c);
void randomize();
uint16_t random_int(uint16_t range);
void initgraph(int16_t gd, int16_t gm, const char *drv);
void nosound();
void sound(uint16_t hz);
uint16_t getdotcolor(int16_t x, int16_t y);
int32_t round_(double x);
double abs_f64(double x);
int16_t abs_i16(int16_t x);
void ellipse(int16_t x, int16_t y, uint16_t start_angle, uint16_t end_angle, uint16_t xrad, uint16_t yrad);
void circle(int16_t x, int16_t y, uint16_t radius, uint16_t color);
void palette(uint16_t p);
uint16_t getpixel(uint16_t x, uint16_t y);
int memavail();
void putpixel(int16_t x, int16_t y, uint16_t c);
bool in(int v, int lo, int hi);
void pstrcpy(char *pasc, const char *cstr);
int pstrcmp(char *pasc, const char *cstr);
bool pstreq(const char *pasc, const char *cstr);
void int2str( int16_t num , char *ps );
void real2str( double num , int width, int dec, char *ps );
void nextpalette();
void nextbkcolor();
