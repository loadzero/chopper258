
void DrawGoodGun(int16_t x, int16_t y)
{
    setcolor (3);
    setfillstyle (1,3);
    moveto (x,y);
    lineto (x+5,y-10);
    lineto (x+10,y);
    lineto (x,y);
    floodfill (x + 2,y - 2,3);
    plot (x+5,y-10,2);
    setfillstyle (1,1);
}

void DrawBase()
{
    setfillstyle (1,3);
    setcolor (3);
    rectangle (20,82,27,85);
    moveto (20,80);
    lineto (27,80);
    lineto (30,85);
    lineto (27,90);
    lineto (27,105);
    lineto (32,110);
    lineto (32,125);
    lineto (15,125);
    lineto (15,110);
    lineto (20,105);
    lineto (20,90);
    lineto (17,85);
    lineto (20,80);
    floodfill (25,100,3);
    setfillstyle (1,1);
}

void DrawTower()
{
    setcolor (3);
    setfillstyle (1,3);
    moveto (240,155);
    lineto (250,125);
    line (240,155,260,155);
    line (250,125,260,155);
    floodfill (250,150,3);
    draw (250,125,250,155,0);
    plot (250,125,2);
    setfillstyle (1,1);
}

void DrawBuilding(int16_t x, int16_t y)
{
    setfillstyle (1,3);
    bar (x,y-40,x+25,y);
    setfillstyle (1,2);
    for (loop = 1; loop <= 4; loop++)
        bar (x+5,y-47+loop*10,x+10,y-47+loop*10 + 3);
    for (loop = 1; loop <= 4; loop++)
        bar (x+15,y-47+loop*10,x+20,y-47+loop*10 + 3);
    setfillstyle (1,1);
}

void DrawRadar(int16_t x, int16_t y)
{
    setcolor (3);
    ellipse (x,y,270,0,10,10);
    ellipse (x,y,270,0,11,11);
    line (x,y,x+6,y+8);
    line (x,y,x+8,y+6);
    line (x+8,y+6,x+15,y+11);
    line (x+3,y+11,x+15,y+11);
}
