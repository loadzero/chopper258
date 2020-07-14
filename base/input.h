#pragma once

// Header file for input handling routines.

#include <stdbool.h>

void input_init();

int popkey();
void pushkey(int k);
bool haskey();

void input();
