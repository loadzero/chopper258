
void MissionSet()
{
    int16_t rand2;

    step = 0;
    randomize();
    rand2 = random_int (10);
    cruise.alive = false;
    cruise.xv = 0;

    if (mis == 3) {
        eheli[1] = true;
        eheli[2] = true;
        enemyheli.alive = true;
        enemyheli.x = 1500;
        enemyheli.y = 50;
        enemyheli.xv = 0;
        enemyheli.yv = 0;
        curreheli = 1;
    }

  if (mis == 9 || mis == 10 || mis == 12 || mis == 13)
      step = 5;

  if (mis == 7)
  {
     eheli[1] = true;
     eheli[3] = true;
  }
  if (mis == 1)
    eheli[2] = true;
  if (mis == 4)
      if (rand2 < 5) scene = "night";
  if (mis == 2)
  {
      if (rand2 < 4) scene = "night";
      cruise.alive = true;
      cruise.x = 3200;
      cruise.y = 50;
      cruise.xv = -4;
      cruise.yv = -1;
  }

  if (mis == 6 || mis == 8)
    if (rand2 < 6) scene = "night";

  if ((mis == 11) && (rand2 < 4)) step = 5;

  if (mis == 14)
  {
      if (rand2 == 4) scene = "night";
      pl.alive = true;
      pl.x = 3000;
  }

  if (mis == 15)
  {
      step = 5;
      if (rand2 < 4) scene = "night";
      pl.alive = true;
  }

  if (mis == 16)
  {
      eheli[1] = true;
      eheli[2] = true;
      enemyheli.alive = true;
      enemyheli.x = -600;
      enemyheli.y = 50;
      enemyheli.xv = 0;
      enemyheli.yv = 0;
      curreheli = 1;
      pl.alive = true;
      pl.x = 3000;
      if (rand2 < 4) scene = "night";
  }

  if (mis == 17)
  {
      if (rand2 < 4) scene = "night";
      cruise.alive = true;
      cruise.x = -1700;
      cruise.y = 50;
      cruise.xv = -4;
      cruise.yv = -1;
      step = 5;
  }

  if (mis == 18)
  {
      cruise.alive = true;
      cruise.x = 2000;
      cruise.y = 50;
      cruise.xv = -3;
      cruise.yv = -1;
  }

  if (mis == 19)
  {
      pl.alive = true;
      pl.x = 2800;
      step = 4;
  }

  if (mis == 20)
  {
      eheli[1] = true;
      enemyheli.alive = true;
      enemyheli.x = 600;
      enemyheli.y = 50;
      enemyheli.xv = 0;
      enemyheli.yv = 0;
      cruise.alive = true;
      cruise.x = 2000;
      cruise.y = 50;
      cruise.xv = -5;
      cruise.yv = -1;
      curreheli = 1;
      pl.alive = true;
      pl.x = 5000;
      if (rand2 < 4) scene = "night";
  }


}

void Pause()
{
    // not implemented
}

void DrawEnemy()
{
    graphcolormode();

    draw (1,3,9,3,3);
    draw (1,3,1,2,3);
    draw (9,3,9,2,3);
    draw (3,1,1,3,3);
    draw (3,1,4,1,3);
    draw (9,2,7,1,3);
    draw (7,1,6,1,3);
    draw (6,1,6,0,3);
    draw (4,1,4,0,3);
    draw (6,0,4,0,3);

    fillshape (4,2,3,3);
    getpic (enemy,1,0,9,3);
    setcolor (3);  // draw tank
    line (55,51,51,55);
    line (56,51,60,55);
    line (55,53,56,53);
    line (52,54,59,54);
    line (52,56,59,56);
    plot (53,55,3);
    plot (55,55,3);
    plot (57,55,3);
    plot (59,55,3);
    getpic (tank.pic1,51,51,60,56);
    draw (53,55,59,55,0);
    plot (52,55,3);
    plot (54,55,3);
    plot (56,55,3);
    plot (58,55,3);
    getpic (tank.pic2,51,51,60,56);
    getpic (tank.blank,151,51,160,56);
} 

void DrawHeli()
{
    graphcolormode();
    draw (11,10,19,10,3);
    draw (15,11,13,12,3);
    draw (15,11,19,13,3);
    draw (16,14,19,13,3);
    draw (16,14,13,13,3);
    draw (13,13,10,13,3);
    plot (10,12,3);
    draw (17,10,19,10,0);

    getpic (heli,10,10,19,15);
    draw (11,10,19,10,0);
    draw (15,10,19,10,3);
    getpic (tHeli,10,10,19,15);
    draw (11,10,19,10,2);
    getpic (enemyhelipic,10,10,19,15);
    getpic (blank,30,30,39,35);
 
    plot (108,100,3);
    draw (102,101,108,101,3);
    draw (100,102,108,102,3);
    draw (102,103,108,103,3);
    plot (109,102,2);
    getpic (cruise.pic1,100,100,109,103);
    draw (109,101,109,103,2);
    plot (110,102,2);
    plot (111,102,2);
    getpic (cruise.pic2,100,100,111,103);
    getpic (cruise.blank,150,150,161,153);
}

void DrawHeli2()
{
    graphcolormode();
    draw (19,10,11,10,3);
    draw (15,11,17,12,3);
    draw (15,11,11,13,3);
    draw (14,14,11,13,3);
    draw (14,14,17,13,3);
    draw (17,13,20,13,3);
    plot (20,12,3);
    draw (17,10,19,10,0);
    getpic (heli2,11,10,20,15);


    draw (11,10,19,10,0);
    draw (15,10,19,10,3);
    getpic (tHeli2,11,10,20,15);
    draw (11,10,19,10,2);
    getpic (enemyheli2pic,11,10,20,15);
    setcolor (3);
    line (112,101,115,101);
    line (110,102,115,102);
    plot (19,2,3);
    line (103,104,119,104);
    line (101,105,119,105);
    line (100,106,119,106);
    line (101,107,118,107);
    line (107,108,115,108);
    line (110,109,115,109);
    line (112,110,115,110);
    line (114,111,115,110);
    line (118,103,119,103);

    getimage (101,100,119,110,plane);
    getimage (121,120,139,130,bplane);
}
