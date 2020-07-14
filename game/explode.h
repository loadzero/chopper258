
void dostatus();
void dostatus2();
void CheckBuildings();

void BlowUp(double xx, double yy, int16_t ssize)
{
    for (looop = 1; looop <= ssize; ++looop)
    {
        if (sounds > 0) sound (60-looop);
        circle (round_ (xx),round_ (yy),looop,2);
        circle (round_ (xx+5),round_ (yy+4),looop,3);
        circle (round_ (xx+5),round_ (yy-4),looop,2);

        udelay(looop * 500);
    }
    nosound();
    for (looop = 1; looop <= ssize; ++looop)
    {
        if (sounds > 0) sound (65-looop);
        circle (round_ (xx),round_ (yy),looop,0);
        circle (round_ (xx+5),round_ (yy+4),looop,0);
        circle (round_ (xx+5),round_ (yy-4),looop,0);

        udelay(looop * 250);
    }
    nosound();
    RememberHole (round_ (xx-ssize), round_ (yy-ssize-4), round_ (xx+ssize+5), round_ (yy+ssize+4));
    if ((x-5 < xx+ssize) && (x+15 > xx-ssize) && (y-5 < yy+ssize) && (y+10 > yy-ssize)) status = 0;
    if ((x-10 < xx+ssize) && (x+20 > xx-ssize) && (y-10 < yy+ssize) && (y+15 > yy-ssize)) 
    {
        status = status -2;
        fall = fall + 0.15;
        dostatus();
    }
    if ((cruise.x-15 < xx+ssize) && (cruise.x+30 > xx-ssize) && (cruise.y-15<yy+ssize)&&(cruise.y+30>yy-ssize) &&
            (cruise.alive)) CruiseCrash();
    if ((pl.x-15 < xx+ssize) && (pl.x+30 > xx-ssize) && (pl.y-15<yy+ssize)&&(pl.y+30>yy-ssize) &&
            (pl.alive)) 
    {
        pl.status = pl.status - 2;
        pl.fall = pl.fall + 0.75;
        pl.engine = pl.engine / 3.0;
        if ((pl.status < 1)) PlCrash();
    }
    if ((enemyheli.x-15 < xx+ssize) && (enemyheli.x+30 > xx-ssize) && (enemyheli.y-15<yy+ssize)&&(enemyheli.y+30>yy-ssize) &&
            (enemyheli.alive)) 
    {
        enemyheli.alive=false;
        inc(&deadheli);
        deheli[curreheli]= true;
        curreheli = 0;
        gotoxy (1,1); writeln_str("Helicopter destroyed.    ");
        BlowUp(enemyheli.x,enemyheli.y,round_(10+(enemyheli.xv*2)+enemyheli.yv));
    }
    if ((man.x-5 < xx+ssize) && (man.x+15 > xx-ssize) && (man.y-5 < yy+ssize) && (man.y+10 > yy-ssize)) man.cond = 0;
    if ((man.x-15 < xx+ssize) && (man.x+30 > xx-ssize) && (man.y-15<yy+ssize) && (man.y+30>yy-ssize)) 
        man.cond=man.cond-1;
    if ((man.x-10 < xx+ssize) && (man.x+20 > xx-ssize) && (man.y-10 < yy+ssize) && (man.y+15 > yy-ssize)) 
    {
        man.cond = man.cond -1;
        fall = fall + 0.25;
        dostatus2();
    }
    for (loop = 1; loop <= totalmen; loop++)
        if ((sold[loop].x-10 < xx+ssize) && (sold[loop].x+20 > xx-ssize) && (sold[loop].y-10 < yy+ssize)
                && (sold[loop].y+15>yy-ssize)) 
            if ((sold[loop].screen == onscreen) && (sold[loop].cond > 0)) 
            {
                sold[loop].cond = 0;
                setfillstyle (1,0);
                bar (round_(sold[loop].x-5),round_(sold[loop].y-10),round_(sold[loop].x+5),round_(sold[loop].y+1));
                setfillstyle (1,1);
                DeleteMan (loop);
            }
    CheckBuildings();
}

void Fire(double Xdes,double Ydes,double Xtarget,double Ytarget)
{
    if (bombtotal < MaxBomb) 
    {
        inc(&bombtotal);
        bomb[bombtotal].x = Xdes;
        bomb[bombtotal].y = Ydes;
        bomb[bombtotal].xv = (Xtarget-Xdes) / 7.5;
        bomb[bombtotal].yv = (Ytarget-Ydes) / 10 -3;
        bomb[bombtotal].power = 5;
        bomb[bombtotal].kind = 0;
    }
} // ( of fire )
