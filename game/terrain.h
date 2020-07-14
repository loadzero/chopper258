
void DrawGround0(int16_t *ammo, int16_t* ammo2, int16_t* ammo3, int16_t *status, double *fall)
{
    setcolor (2);
    settextstyle (2,0,5);
    outtextxy (10,12,"You have crossed into another dimension.");
    setcolor (1);
    outtextxy (10,50,"The Mark Currie Zone.");
    setcolor (3);
    // {  outtextxy (10,100,"Rearmed.  Going up?"); }
    outtextxy (10,100,"Rearmed.");
    if (*status > 10)
    {
        outtextxy (10,130,"So you know both secrets.");
        outtextxy (10,150, "Go back right for some para-action!");
        *ammo  =  999;
    }
    else *ammo  =  150;
    *ammo2  =  10;
    *ammo3  =  10;
    // {  fall  =  -0.2; }
    if (*status < 4) *status  =  4;
}

void DrawGround1()
{
    setcolor(1);
    moveto (0,0);
    lineto (50,100);
    lineto (100,160);
    lineto (200,175);
    lineto (250,150);
    lineto (320,50);
    floodfill (1,160,1);
}

void DrawGround2()
{
    setcolor(1);
    moveto (0,50);
    lineto (50,150);
    lineto (150,175);
    lineto (200,150);
    lineto (250,135);
    lineto (320,125);
    floodfill (1,199,1);
}

void DrawGround3()
{
    setcolor(1);
    line (0,125,55,125);
    moveto (55,125);
    lineto (70,140);
    lineto (60,150);
    lineto (55,140);
    lineto (20,135);
    lineto (15,165);
    lineto (60,165);
    lineto (70,175);
    lineto (100,175);
    lineto (125,190);
    lineto (180,190);
    lineto (180,190);
    lineto (200,170);
    lineto (215,175);
    lineto (240,150);
    lineto (260,160);
    lineto (280,175);
    lineto (320,140);
    setfillstyle (1,1);
    floodfill (2,199,1);
    setlinestyle (1,1,1);
    setcolor (2);
    line (15,166,60,166);
    setlinestyle (0,1,1);
}

void DrawGround4()
{
    setcolor(1);
    moveto (0,140);
    lineto (100,160);
    lineto (175,155);
    lineto (260,130);
    lineto (320,140);
    floodfill (1,160,1);
}

void DrawGround5()
{
    setcolor(1);
    moveto (0,140);
    lineto (50,150);
    lineto (75,175);
    lineto (125,125);
    lineto (150,150);
    lineto (172,125);
    lineto (200,175);
    lineto (235,150);
    lineto (250,125);
    lineto (275,170);
    lineto (320,170);
    floodfill (1,199,1);
}

void DrawGround6()
{
    setcolor(1);
    moveto (0,170);
    lineto (10,160);
    lineto (10,175);
    lineto (30,175);
    lineto (50,160);
    lineto (60,170);
    lineto (90,135);
    lineto (110,150);
    lineto (110,160);
    lineto (120,160);
    lineto (130,173);
    lineto (160,160);
    lineto (175,170);
    lineto (180,180);
    lineto (200,180);
    lineto (220,155);
    lineto (280,155);
    lineto (320,170);
    floodfill (1,199,1);
}

void DrawGround7()
{
    setcolor(1);
    moveto (0,170);
    lineto (50,150);
    lineto (75,155);
    lineto (225,155);
    lineto (275,120);
    lineto (300,140);
    lineto (320,130);
    floodfill (1,199,1);
}

void DrawGround8()
{
    setcolor(1);
    moveto (0,130);
    lineto (75,125);
    lineto (100,150);
    lineto (125,125);
    lineto (175,140);
    lineto (300,140);
    lineto (320,150);
    floodfill (1,199,1);
}

void DrawGround9()
{
    setcolor (1);
    setfillstyle (3,1);
    moveto (175,150);
    lineto (250,175);
    lineto (300,150);
    lineto (175,150);
    floodfill (250,170,1);
    setcolor(1);
    setfillstyle (1,1);
    moveto (0,150);
    lineto (75,125);
    lineto (125,75);
    lineto (175,150);
    lineto (250,175);
    lineto (300,150);
    lineto (320,140);
    floodfill (1,199,1);
}

void DrawGround10()
{
    setcolor(1);
    moveto (0,140);
    lineto (110,140);
    lineto (125,155);
    lineto (150,175);
    lineto (320,175);
    floodfill (1,199,1);
}

void DrawGround11()
{
    setcolor(1);
    moveto (0,175);
    lineto (75,175);
    lineto (125,150);
    lineto (225,150);
    lineto (250,125);
    lineto (275,135);
    lineto (320,100);
    floodfill (1,199,1);
}

void DrawGround12()
{
    setcolor (1);
    setfillstyle (1,1);
    moveto (0,100);
    lineto (50,150);
    lineto (75,125);
    lineto (125,175);
    lineto (175,125);
    lineto (200,150);
    lineto (240,156);
    lineto (267,156);
    lineto (275,175);
    lineto (310,200);
    floodfill (250,170,1);
    setfillstyle (3,1);
    moveto (275,175);
    lineto (320,175);
    floodfill (319,199,1);
}

void DrawGround13()
{
    setcolor (1);
    moveto (0,175);
    lineto (320,175);
    setfillstyle (3,1);
    floodfill (319,199,1);
}


void DrawGround14()
{
    setcolor (1);
    moveto (150,175);
    lineto (175,125);
    lineto (200,165);
    lineto (225,135);
    lineto (250,150);
    lineto (300,75);
    lineto (320,100);
    setfillstyle (3,1);
    moveto (0,175);
    lineto (150,175);
    lineto (0,200);
    floodfill (1,199,1);
    setfillstyle (1,1);
    floodfill (250,170,1);
}

void DrawGround15()
{
    setcolor (1);
    setfillstyle (1,1);
    moveto (0,100);
    lineto (10,125);
    lineto (55,125);
    lineto (65,150);
    lineto (100,150);
    lineto (150,175);
    lineto (175,150);
    lineto (225,125);
    lineto (250,75);
    lineto (275,100);
    lineto (320,0);
    floodfill (1,199,1);
    setlinestyle (1,1,1);
    setcolor (2);
    line (65,151,100,151);
    setlinestyle (0,1,1);
}

void DrawGround16()
{
    setcolor (1);

    // solid fill, color 1
    setfillstyle (1,1);
    moveto (0,0);
    lineto (10,75);
    lineto (80,100);
    lineto (110,180);
    lineto (125,185);
    lineto (135,175);
    lineto (250,175);
    lineto (320,100);

    // This screen doesn't fill properly with this seed point,
    // workaround by moving it right a bit.
    // floodfill (1,10,1);
    floodfill (10,150,1);
    setcolor (1);
    moveto (187,0);
    lineto (250,70);
    lineto (270,65);
    lineto (320,94);
    setfillstyle (1,1);
    floodfill (319,1,1);
    floodfill (316,137,1);
    setcolor (2);
    line (170,176,185,176);
    setlinestyle (0,1,1);
}

void DrawGround17()
{
    setcolor (1);
    setfillstyle (1,1);
    moveto (0,94);
    lineto (50,94);
    lineto (135,85);
    lineto (160,115);
    lineto (160,115);
    lineto (140,120);
    lineto (140,125);
    lineto (160,130);
    lineto (160,145);
    lineto (145,160);
    lineto (20,160);
    lineto (20,145);
    lineto (100,145);
    lineto (125,130);
    lineto (85,130);
    lineto (85,115);
    lineto (125,115);
    lineto (125,100);
    lineto (0,106);
    floodfill (1,1,1);
}

