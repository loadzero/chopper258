#pragma once

struct Glyph
{
    int w;
    int h;
    char data[2048];
};

typedef struct Glyph Glyph;

void set_font_size(int s);
void font_init();

Glyph* getglyph(int c);
void print(Glyph *g);

