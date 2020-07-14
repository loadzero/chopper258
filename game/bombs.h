
void TextDie(int16_t n)
{
    gotoxy (1,1);
    switch (n) {
        case 1 : write_str ("Good Base destroyed.    "); break;
        case 2 : write_str ("Tower destroyed.        "); break;
        case 3 : write_str ("Building destroyed.     "); break;
        case 4 : write_str ("Radar destroyed.        "); break;
        case 5 : write_str ("Good Gun destroyed.     "); break;
        case 6 : write_str ("Gun destroyed.          "); break;
    } // ( of case )
}

void DeleteBomb()
{
    int16_t loop;

    if ((bomb[counter].x > -10) && (bomb[counter].x < 330)) 
    {
        if (bomb[counter].kind == 1) // ( 1 is missile )
            draw (round_ (bomb[counter].x),round_ (bomb[counter].y),round_ (bomb[counter].x-7),round_ (bomb[counter].y),0);
        else
            plot (round_ (bomb[counter].x),round_ (bomb[counter].y),0);
    }

    loop = 0;

    do {
        inc(&loop);
        bomb[counter+loop-1]=bomb[counter+loop];
    } while (!((counter + loop) >= bombtotal));

    dec(&bombtotal);

    if (bombtotal < 0) bombtotal = 0;
}

void PlCrash()
{
    pl.alive = false;
    BlowUp (pl.x,pl.y,15);
    BlowUp (pl.x+20,pl.y,10);
    gotoxy (1,1);
    writeln_str("Plane Destroyed.         ");
}

void hurtpl()
{
    pl.status = pl.status - 1;
    if (pl.status < 5) pl.engine = -2;
    if (pl.status < 3) pl.engine = -1;
    if (pl.status < 1) PlCrash();
    pl.fall = pl.fall + 0.25;
}

void soldeject (double xx, double yy)
{
    if (totalmen < MaxSoldiers) // ( enemyheli eject )
    {
        inc(&totalmen);
        sold[totalmen].x = xx + 7;
        sold[totalmen].y = yy - 7;
        sold[totalmen].time = 0;
        sold[totalmen].yv = -1;
        sold[totalmen].xv = 1;
        sold[totalmen].cond = 3;
        sold[totalmen].eject = 2;
        sold[totalmen].screen = onscreen;
    }
}

void BombBlowUp(double xxc, double yyc, double power);
            void dostatus();
void dostatus2();


void BomCrash()
{
    if (sounds > 0) 
        for (loop = 450; loop >= 50; loop--)
            sound (loop);
    nosound();
    if (bomb[counter].power > 14) noise2 = 0;

    if (
        ((area == 1) && (onscreen == 9) && (bomb[counter].x > 175) && (bomb[counter].x<300) && (bomb[counter].y > 145)) ||
        (onscreen == 13) ||
        ((onscreen == 12) && (bomb[counter].x > 277)) ||
        ((onscreen == 14) && (bomb[counter].x < 150)))

        // nothing ?
        ;
    else
        BombBlowUp(bomb[counter].x,bomb[counter].y,bomb[counter].power);


    if ((x > bomb[counter].x - 10) && (x < bomb[counter].x + 20) && (y < bomb[counter].y + 10) && (y > bomb[counter].y-15)) {
            dec(&status);
            inc(&HitTotal);
            dostatus();
            if (fall > 0.25)
                Crash();
            else
                fall = fall + 0.05;
            if (man.cond < 1) Crash();
        }
    if (eject > 0) 
        if ((man.x>bomb[counter].x-5) && (man.x<bomb[counter].x+8) && (man.y<bomb[counter].y+10) && (man.y>bomb[counter].y-10)) 
            {
                man.cond = man.cond -3;
                dostatus2(); // { do status for para man }
                if (man.time > 20) 
                {
                    man.cond = man.cond + 2;
                    man.time = -120;
                    putpic (blank,round_(man.x-2),round_(man.y-4));
                    putpic (blank,round_(man.x),round_(man.y-4));
                }
            }
    for (loop = 1; loop <= totalmen; loop++) 
        if ((sold[loop].x>bomb[counter].x-8) && (sold[loop].x<bomb[counter].x+11) &&
            (sold[loop].y<bomb[counter].y+8) && (sold[loop].y>bomb[counter].y-20)) 
            {
                sold[loop].cond = sold[loop].cond -3;
                if (sold[loop].time > 19) 
                {
                    sold[loop].cond = sold[loop].cond + 2;
                    sold[loop].time = -120;
                    sold[loop].eject = 1;
                    putpic (blank,round_(sold[loop].x-1),round_(sold[loop].y-4));
                    putpic (blank,round_(man.x),round_(man.y-4));
                }
            }
    if ((pl.alive)) 
        if ((pl.x > bomb[counter].x - 21) && (pl.x < bomb[counter].x + 35) && (pl.y < bomb[counter].y + 4)
            && (pl.y > bomb[counter].y -17)) hurtpl();
    if ((enemyheli.alive) && (enemyheli.fall == false)) 
        if ((enemyheli.x > bomb[counter].x - 10) && (enemyheli.x < bomb[counter].x + 20) && (enemyheli.y < bomb[counter].y + 20) && (enemyheli.y > bomb[counter].y -7)) 
            {
                enemyheli.fall = true;
                deheli[curreheli] = true;
                soldeject (enemyheli.x,enemyheli.y);
            }
    if ((tank.alive) && (onscreen == tank.screen)) 
        if ((tank.x > bomb[counter].x - 3) && (tank.x < bomb[counter].x + 10) && (tank.y < bomb[counter].y + 10)
            && (tank.y > bomb[counter].y - 10)) 
            {
                tank.alive = false;
                tank.screen = 16;
                BlowUp (tank.x+5,tank.y-2,10);
                if (totalmen < MaxSoldiers) // ( eject )
                soldeject (tank.x+2,tank.y-8);
                if (totalmen < MaxSoldiers) // ( eject )
                soldeject (tank.x-1,tank.y-9);
            }
    if ((cruise.alive)) 
        if ((cruise.x-15 < bomb[counter].x) && (cruise.x+15 > bomb[counter].x)) 
            if ((cruise.y-15 < bomb[counter].y) && (cruise.y+15 > bomb[counter].y)) 
                CruiseCrash();
    if ((onscreen == 3) && (building[1])) 
        if ((bomb[counter].x > 13) && (bomb[counter].x < 34) && (bomb[counter].y > 76) && (bomb[counter].y < 135)) 
        {
            TextDie (1);
            building[1] = false;
            BlowUp (24,85,10);
            BlowUp (23,105,20);
        }
    if ((onscreen == 6) && (area == 1) && (building[2])) 
        if ((bomb[counter].x > 238) && (bomb[counter].x < 282) && (bomb[counter].y < 165) && (bomb[counter].y > 120)) 
        {
            TextDie (2);
            building[2] = false;
            BlowUp (250,135,10);
            BlowUp (250,145,10);
        }
    if ((onscreen == 7) && (area == 1) && (building[3])) 
        if ((bomb[counter].x > 195) && (bomb[counter].x < 230) && (bomb[counter].y < 190) && (bomb[counter].y > 100)) 
        {
            TextDie (3);
            building[3] = false;
            BlowUp (210,125,20);
            BlowUp (210,145,20);
        }
    if ((onscreen == 8) && (area == 1)) 
    {
        if ((bomb[counter].x > 195) && (bomb[counter].x < 220) && (bomb[counter].y < 180) && (bomb[counter].y > 110) &&
            (building[4])) 
            {
                TextDie (4); // ( radar )
                building[4] = false;
                BlowUp (210,135,15);
            }
        if ((bomb[counter].x > 219) && (bomb[counter].x < 242) && (bomb[counter].y < 180) && (bomb[counter].y > 90) &&
            (building[5])) 
            {
                TextDie (3); // ( building)
                building[5] = false;
                BlowUp (230,130,20);
                BlowUp (230,140,20);
                BlowUp (230,110,20);
            }
    }
    if ((onscreen == 10) && (area == 1)) 
    {
        if ((bomb[counter].x > 47) && (bomb[counter].x < 73) && (bomb[counter].y < 150) && (bomb[counter].y > 60) &&
            (building[6])) 
            {
                TextDie (4); // ( radar )
                building[6] = false;
                BlowUp (60,100,15);
                BlowUp (60,120,20);
                BlowUp (60,133,20);
            }
        if ((bomb[counter].x > 247) && (bomb[counter].x < 273) && (bomb[counter].y < 180) && (bomb[counter].y > 135) &&
            (building[7])) 
            {
                TextDie (3);
                building[7] = false;
                BlowUp (260,150,20);
                BlowUp (260,165,15);
            }
    }
    if ((onscreen == 11) && (area == 1) && (building[8])) 
        if ((bomb[counter].x > 195) && (bomb[counter].x < 230) && (bomb[counter].y < 160) && (bomb[counter].y > 100)) 
        {
            TextDie (3);
            building[8] = false;
            BlowUp (210,130,20);
            BlowUp (210,145,15);
        }
    if ((onscreen == 12) && (area == 1) && (building[9])) 
        if ((bomb[counter].x > 238) && (bomb[counter].x < 282) && (bomb[counter].y < 165) && (bomb[counter].y > 120)) 
        {
            TextDie (2);
            building[9] = false;
            BlowUp (250,135,10);
            BlowUp (250,145,10);
        }
    if ((onscreen == 15) && (building[10])) 
        if ((bomb[counter].x > 13) && (bomb[counter].x < 34) && (bomb[counter].y > 76) && (bomb[counter].y < 135)) 
        {
            TextDie (1);
            building[10] = false;
            BlowUp (24,85,10);
            BlowUp (23,105,20);
        }
    DeleteBomb();
} // (of bombcrash)

void CheckBomb()
{
    for (counter = 1; counter <= bombtotal; ++counter)
    {
        if (bomb[counter].kind == 0) // ( 0 is dot bomb )
        {
            plot (round_ (bomb[counter].x),round_ (bomb[counter].y),0);
            bomb[counter].xv = bomb[counter].xv /1.1;
            bomb[counter].yv = bomb[counter].yv +0.3;
            bomb[counter].y = bomb[counter].y + bomb[counter].yv;
            bomb[counter].x = bomb[counter].x + bomb[counter].xv;
            plot (round_ (bomb[counter].x),round_ (bomb[counter].y),2);
            if ((getdotcolor(round_ (bomb[counter].x),round_ (bomb[counter].y+1)) > 0) ||
                (getdotcolor(round_ (bomb[counter].x),round_ (bomb[counter].y-1)) > 0) ||
                    (getdotcolor(round_ (bomb[counter].x+1),round_ (bomb[counter].y)) > 0)) BomCrash();

            if (cruise.alive) // ?
            {
            }
        }
        if (bomb[counter].kind == 1) // ( 1 is missile )
        {
            draw (round_ (bomb[counter].x),round_ (bomb[counter].y),round_ (bomb[counter].x-7),round_ (bomb[counter].y),0);
            if (abs_f64(bomb[counter].xv) < 12) bomb[counter].xv = bomb[counter].xv + bomb[counter].dir; // ( dir is direction )
            bomb[counter].yv = bomb[counter].yv /1.2;
            bomb[counter].y = bomb[counter].y + bomb[counter].yv;
            bomb[counter].x = bomb[counter].x + bomb[counter].xv;
            draw (round_ (bomb[counter].x),round_ (bomb[counter].y),round_ (bomb[counter].x-7),round_ (bomb[counter].y),3);
            if ((getdotcolor(round_ (bomb[counter].x-4),round_ (bomb[counter].y+3)) > 0) ||
                (getdotcolor(round_ (bomb[counter].x+1),round_ (bomb[counter].y)) > 0) ||
                    (getdotcolor(round_ (bomb[counter].x-8),round_ (bomb[counter].y)) > 0)) BomCrash();

            if (cruise.alive) // ?
            {
            }
        }
        if (bomb[counter].y> 200) DeleteBomb();
        if (bomb[counter].x > 360) DeleteBomb();
        if (bomb[counter].x < -40) DeleteBomb();
    }
} // ( of checkbomb )

