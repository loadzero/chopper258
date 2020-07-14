
bool LeftKeyPressed() { return false; }
bool RightKeyPressed() { return false; }
bool BothKeyPressed() { return false; }

void get_mouse(int *x, int *y);

int lx = -1;
int ly = -1;

void ReadMotion(uint16_t *x, uint16_t *y)
{
    int ix = 0;
    int iy = 0;

    get_mouse(&ix, &iy);

    if (lx == -1)
    {
        lx = ix;
        ly = iy;
    }

    *x = (ix - lx);
    *y = (iy - ly);
    lx = ix;
    ly = iy;
}

void SetMousePos(uint16_t x, uint16_t y)
{
} 

void HideMouse()
{
}

bool MousePresent()
{
    return true;
}
