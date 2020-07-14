//
// The entrypoint and main loop of the translated game code.
//
// note: This is run in the 'game' thread.
//
// Translated from:
//     pascal/chopper2.pas
//

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "base/turbo.h"

// (* Compile under Turbo Pascal version 5.0  *)

// (* Good Luck understanding this code.  Even though I wrote it,
// after three years since looking at it, I don't understand it.   (Moral :
// /always put lots of comments in you source code)
// *)

#define MaxBomb ((int16_t) 100)
#define MaxSpeed ((int16_t) 12)
#define MaxSoldiers ((int16_t) 25)
#define MaxHoles ((int16_t) 200)

// from missions
#define NumofBuildings ((int16_t) 12)

struct bombtype {
    double x;
    double y;
    double xv;
    double yv;
    double power;
    int16_t kind; //   kind : integer; { 0 bomb, 1 missile }
    double dir; //   dir  : real; { - left  + right  ( only for kind 1 ) }
};

struct planetype
{
   double x;
   double y;
   double xv;
   double yv;
   double fall;
   bool alive;
   int16_t status;
   int16_t engine;
};

typedef struct planetype planetype;

typedef bool Deadhelitype[1+10]; // fine
typedef bool Buildingtype[1+NumofBuildings];

struct helitype {
    double x;
    double y;
    double xv;
    double yv;
    bool alive;
    bool fall;
}; 

typedef struct helitype helitype;

struct persontype {
    double x;
    double y;
    double xv;
    double yv;
    int16_t screen; // { screen person is on (for sold. only) }
    int16_t cond;   // { 3 to 0 }
    int16_t time;   // { timing parachute }
    int16_t eject;  // { 2 in air no chute, 1 in air chute, 0 on ground ** not used by man. He has his own global }
};

struct holetype {
   void* image[MaxHoles+1];
   int16_t total;
   int16_t hx[MaxHoles+1];
   int16_t hy[MaxHoles+1];
   int16_t screen[MaxHoles+1];
   int16_t size[MaxHoles+1];
};

typedef struct holetype holetype;

struct cruisetype {
    int16_t x;
    int16_t y;
    int16_t xv;
    int16_t yv;
    bool alive;

    int16_t pic1[60+1];
    int16_t pic2[60+1];

    int16_t blank[60+1];
   // { for tank }
    int16_t screen;
};

typedef char stattype[1+10+1];

struct pilottype {

    char name[1+20+1];
    int16_t totalmissions;
    int16_t totalscore;
    double dmg;
    stattype status;
    char rank[1+10+1];
};

typedef struct pilottype pilottype;

int16_t sounds; // { 0 no sound, 1 all but engine, 2 all }
bool aeject;
int16_t del;
bool mousee;
void *plane;
void *bplane;
bool quit;

double x,y; // { position of heli }
double xv,yv; //  { velocity }
double mfall; //  { for mouse }

bool eheli[1+10]; // { Which enemy helicopters have taken off?   true = taken of  }
Deadhelitype deheli; // { which enemy helis are dead? }
Buildingtype building; // { which buildings are not destroyed? true = not destroyed }
int16_t mis; // { mission # }
int16_t curreheli; // { which enemy heli is currently on alive? }
int16_t counter2; // counter2   : integer;
bool splash; // splash     : boolean;
int32_t dx;
int32_t dy;

uint16_t dx2;
uint16_t dy2;

planetype pl;
int16_t area;

bool free_; // was free free:boolean; { true means practice mission }
holetype hole; // { hole from explosions }

int16_t landed; // { 2 = landed, 1 = gear down, 0 = up}
int16_t eject; // { 0 = not ejected , 1 = in air, 2 on ground ** only for man not soldier }

int16_t time_;
int16_t time2; // { one time2 = 60 * time }

int16_t noise1; // { hitting ground, 2 to 0 }
int16_t noise2;
int16_t noise3;
int16_t onscreen; // { screen your on }

int16_t step;  // { step in mission, only for some }
double bpower;  // { bombpower for man }

int16_t ammo;       // { bombs }
int16_t ammo2;      // { missiles }
int16_t ammo3;      // { big bombs }

struct helitype enemyheli;
int16_t bombtotal;
int16_t totalmen; // { total of onscreen soldiers }

double damage; // { dmg. in $ }

double fall;
int16_t HitTotal;

int16_t deadheli; // { counts dead enemy helicopters }
int16_t side; // { 1 blade right, 0 blade left }
int16_t status; // dunno - was status
struct persontype man;
int16_t totalejects;

struct persontype sold[1+MaxSoldiers];
bool auto_; // { auto pilot }

char level;

int16_t pilnum; // { pilot number }
struct pilottype pilot;

int16_t enemy[40+1];
struct cruisetype tank;

int16_t heli[40+1];
int16_t tHeli[40+1];
int16_t enemyhelipic[40+1];
int16_t blank[40+1];

struct cruisetype cruise;
int16_t heli2[40+1];

int16_t tHeli2[40+1];
int16_t enemyheli2pic[40+1];

int16_t pict1[1+12]; // { man }
int16_t pict2[1+12];
int16_t pict3[1+45]; // { parachute }
int16_t pblank[1+12];
bool mainquit;

// { starting }
       
uint16_t time1;
uint16_t time22;
uint16_t time3;
uint16_t time4;

// { after mission }

uint16_t time5;
uint16_t time6;       
uint16_t time7;
uint16_t time8;

struct bombtype bomb[1+MaxBomb];

char lvl; // { level }
const char *scene; // { day, midday, night }

int16_t loop;
int16_t bkcolor; // { 0 (black) for night, 1 (blue) for day }

double CalcDif;

char keyhit;

int16_t counter;

void BlowUp(double xx, double yy, int16_t ssize);

bool under; // ( is the spot 5 pixels under heli land? (for autoeject) )

void RememberHole (int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void CruiseCrash();
void DeleteMan (int16_t loop);
void DoHoles();
void Crash();
void Shutdown();
void DoMouse();

int16_t looop;
void ToggleMouse();

#include "mouse.h"
#include "vehicles.h"
#include "people.h"
#include "bombs.h"
#include "building.h"
#include "explode.h"
#include "missiona.h"
#include "title.h"
#include "terrain.h"

void GraphicStartUp()
{
    int16_t gm;
    int16_t gd;

    int CGA = 1;

    gd = CGA;
    gm = 1;

    initgraph(gd,gm,"");
}

void ResetVars()
{
    int16_t loop;

    for (loop = 1; loop <= MaxBomb; ++loop)
    {
        bomb[loop].x = 0;
        bomb[loop].y = 0;
        bomb[loop].xv = 0;
        bomb[loop].yv = 0;
        bomb[loop].power = 5;
        bomb[loop].kind = 0;
        bomb[loop].dir = 0;
    }

  quit = false;
  x = 25;
  mfall = 0;

  for (loop = 1; loop <= 10; ++loop)
  {
      eheli[loop] = false;
      deheli[loop] = false;
  }

  for (loop = 1; loop <= NumofBuildings; ++loop)
      building[loop] = true;

  if (mis != 3) curreheli = 0;
  y = 163;
  counter2 = 0;
  splash = false;
  dx = 100;
  dy = 100;

  // eww.. pl

  // with pl do 
  {
      pl.alive = false;
      pl.status = 7;
      pl.engine = -3;
      pl.x = -900;
      pl.y = 20;
      pl.fall = 0;
      pl.xv = 0;
      pl.yv = 0;
  }

  xv = 0;
  area = 1;
  free_ = false;
  hole.total = 0;

  for (loop = 1; loop <= MaxHoles; ++loop)
  {
      // 1 byte ?
      hole.image[loop] = getmem(1);
      hole.size[loop] = 1;
      hole.screen[loop] = 0;
  }

  yv = 0;
  landed = 1;
  eject = 0;
  time_ = 0;
  noise1 = 0;
  noise2 = 0;
  noise3 = 0;
  time2 = 0;
  onscreen = 3;
  step = 0;
  bpower = 5;
  ammo2 = 5;
  ammo3 = 5;
  enemyheli.fall = false;
  bombtotal = 0;
  enemyheli.alive = false;
  totalmen = 0;
  damage = 0;
  fall = 0;
  HitTotal = 0;
  deadheli = 0;
  side = 1;
  status = 4;
  man.cond = 3;
  totalejects = 0;
  tank.alive = true;
  tank.screen = 11;
  tank.x = 150;
  tank.y = 149;
  tank.xv = 0;
  tank.yv = 0;

  for (loop = 1; loop <= 10; loop++)
      sold[loop].cond = 3;

  auto_ = false;
  totalmen = 0;

}

void ResetVars2()
{
    if (level == '1') ammo = 150;
    if (level == '2') ammo = 75;
    if (level == '3') ammo = 50;
    if (level == '1') lvl = 1;
    if (level == '2') lvl = 2;
    if (level == '3') lvl = 3;
}

void CheckBuildings() // ( Check to see if Buildings are blown up )
{
    switch (onscreen) {
        case 3 : 
            if (building[1]) 
                if ((getdotcolor (25,88) == 0 ) || (getdotcolor (25,122) == 0 ) ||
                    (getdotcolor (25,104) == 0 )) 
                    {
                        TextDie(1); // ( 1 == good base, write it on screen )
                        building[1] = false;
                        BlowUp (25,87,20);
                        BlowUp (25,111,15);
                    }
            if (building[11]) 
                if ((getdotcolor (239,157) == 0 ) || (getdotcolor (242,150) == 0 ) ||
                    (getdotcolor (247,157) == 0 )) 
                    {
                        TextDie(5);
                        building[11] = false;
                        BlowUp (239,155,15);
                    }

            break;

        case 6 : if (building[2]) 
                     if ((getdotcolor (251,132) == 0 ) || (getdotcolor (245,147) == 0 ) ||
                         (getdotcolor (255,147) == 0 )) 
                         {
                             building[2] = false;
                             BlowUp (250,135,15);
                             BlowUp (250,145,20);
                             TextDie(2); // ( write on screen, 2 == tower )
                         }

                 break;

        case 7 : 
                 if (building[3]) 
                     if ((getdotcolor (210,132) == 0 ) || (getdotcolor (220,132) == 0 ) ||
                         (getdotcolor (215,147) == 0 )) 
                         {
                             TextDie(3); // ( write on screen, 3 == building )
                             building[3] = false;
                             BlowUp (215,135,20);
                             BlowUp (215,145,20);
                         }
                 if (building[12]) 
                     if ((getdotcolor (100,153) == 0 ) || (getdotcolor (103,151) == 0 ) ||
                         (getdotcolor (105,153) == 0 ))
                         {
                             TextDie(6);
                             building[12] = false;
                             BlowUp (103,152,10);
                         }
                 break;

        case 8 : 
                 if (building[4]) // ( radar )
                 if (getdotcolor (200,128) == 0) 
                 {
                     TextDie(4);  // ( 4 == radar )
                     building[4] = false;
                     BlowUp (205,135,20);
                 }
                 if (building[5]) 
                     if ((getdotcolor (230,122) == 0 ) || (getdotcolor (235,124) == 0 ) ||
                         (getdotcolor (232,147) == 0 )) 
                         {
                             TextDie(3);
                             building[5] = false;
                             BlowUp (235,120,20);
                             BlowUp (235,145,20);
                         }
                 break;

        case 10 : 
                 if (building[6]) // ( radar )
                 if ((getdotcolor (52,87) == 0) && (getdotcolor (60,105) == 0) && (getdotcolor (65,135) == 0) &&
                     (getdotcolor (52,133) == 0)) 
                     {
                         TextDie(4);
                         building[6] = false;
                         BlowUp (52,87,20);
                         BlowUp (60,110,20);
                         BlowUp (60,130,20);
                     }
                 if (building[7]) 
                     if ((getdotcolor (253,150) == 0 ) || (getdotcolor (267,155) == 0 ) ||
                         (getdotcolor (260,160) == 0 )) 
                         {
                             TextDie (3);
                             building[7] = false;
                             BlowUp (260,150,15);
                             BlowUp (260,165,20);
                         }
                 break;
        case 11 : if (building[8]) 
                      if ((getdotcolor (205,125) == 0 ) || (getdotcolor (215,130) == 0 ) ||
                          (getdotcolor (210,142) == 0 )) 
                          {
                              TextDie(3);
                              building[8] = false;
                              BlowUp (210,125,20);
                              BlowUp (210,137,15);
                          }
                  break;
        case 15 : if (building[10]) 
                      if ((getdotcolor (25,88) == 0 ) || (getdotcolor (25,122) == 0 ) ||
                          (getdotcolor (25,104) == 0 )) 
                          {
                              TextDie(1);
                              building[10] = false;
                              BlowUp (25,87,20);
                              BlowUp (25,111,15);
                          }

                  break;
        case 12 : if (building[9]) 
                      if ((getdotcolor (251,132) == 0 ) || (getdotcolor (245,147) == 0 ) ||
                          (getdotcolor (255,147) == 0 )) 
                          {
                              TextDie (2); // ( tower )
                              building[9] = false;
                              BlowUp (250,135,15);
                              BlowUp (250,145,20);
                          }

                  break;
    }  // ( of onscreen )
}

void dostatus2()
{
    gotoxy (29,25);
    write_str ("MAN:");
    gotoxy (33,25);
    if (man.cond < 0) man.cond = 0;
    if (man.cond > 4) man.cond = 4;
    switch (man.cond) {
        case 4: write_str ("HE-MAN "); break;
        case 3: write_str ("PERFECT"); break;
        case 2: write_str ("HURT   "); break;
        case 1: write_str ("INJURED"); break;
        case 0: write_str ("DEAD   "); break;
    }
    if ((man.cond == 0) && (eject == 0) && (status > -7)) Crash();
}

void dostatus()
{
    gotoxy (25,24);
    write_str ("CHOPPER:");
    gotoxy (33,24);
    if ((status > 4) && (status < 100)) write_str ("SUPER   ");
    else
        if ((status >= 100) && (status < 200)) write_str ("MEGA!   ");
        else
            if ((status >= 200)) write_str ("MEGA!!!!");

    if ((status > 20)) 
    {
        mfall = 0; // ( mfall is mouse-fall )
        fall = 0;
    }

    switch (status) {
        case 4: write_str ("PERFECT "); break;
        case 3: write_str ("POOR   "); break;
        case 2: write_str ("INJURED"); break;
        case 1: write_str ("CRITICAL"); break;
        case -7: case 0: write_str ("TRASHED "); break;
    } // ( of case )

    if (eject > 0) dostatus2();
    if ((status < 1) && (status != -7)) 
    {
        status = -7;
        Crash();
    }
}

void Speeds()
{
}

void CheckSound()
{
}

void Input()
{
    if ((getpixel (round_(x+5),round_(y+5+yv+abs_f64(xv))) > 0) && (getpixel (round_(x+4),round_(y+6+yv+abs_f64(xv))) > 0)) 
        under = true;
    else under = false;
    keyhit = 'd';
    delay (del-round_(bombtotal/3)+10);
    if (keypressed()) keyhit = readkey();
    if (mousee) DoMouse();
    if ((aeject) && (eject == 0) && (landed == 0) && (under))
        switch (status) {
            case 4: if ((yv > 0.5) && (yv+abs_f64(xv) > 1)) keyhit = 'e'; break;
            case 3: if (yv > 0.4) keyhit = 'e'; break;
            case 1: case 2: keyhit = 'e'; break;
        }

  switch (keyhit)
  {
    case '8': case 72 :
              yv = yv - 0.5;
              if (landed == 2) 
              {
                  landed = 1;
                  yv = yv - 0.5;
                  gotoxy(1,1);
                  writeln_str("TAKE OFF              ");
              }
              break;

    case '4':case 75 :  xv = xv - 0.5; break;
    case '6':case 77 :  xv = xv + 0.5; break;
    case '2':case 80 :  yv = yv + 0.5; break;

     case 'a':case 'A':
             gotoxy (1,1);
             aeject = aeject ^ true;

             if (aeject) writeln_str("Auto Eject is ON          ");
             else writeln_str("Auto Eject is OFF          ");

              break;
             
     case 'p': nextpalette(); break;
     case 'c': nextbkcolor(); break;
     case 's':case 'S' : Speeds(); break;
     case 'x':case 'X' : CheckSound(); break;
     case 't':case 'T' : ToggleMouse(); break;
     case 'b':case 'B': case 82: case '0': 
         if ((bombtotal < MaxBomb) && (ammo > 0)) 
         {
             dec(&ammo);
             inc(&bombtotal);
             bomb[bombtotal].x = x + 5;
             bomb[bombtotal].y = y + 5;
             bomb[bombtotal].xv = xv;
             bomb[bombtotal].yv = yv + 3;
             bomb[bombtotal].power = bpower;
             bomb[bombtotal].kind = 0;
             gotoxy(34,23);
             writeln_u16_str(ammo," ");
         }
            break;
     case 'n':case 'N':case 13:   // ( #13 is return )
         if ((bombtotal < MaxBomb) && (ammo3 > 0)) 
         {
             dec(&ammo3);
             inc(&bombtotal);
             bomb[bombtotal].x = x + 5;
             bomb[bombtotal].y = y + 5;
             bomb[bombtotal].xv = xv;
             bomb[bombtotal].yv = yv + 3.5;
             bomb[bombtotal].power = 15;
             bomb[bombtotal].kind = 0;
             noise2 = 20;
         }
     break;
     case 'm':
         if ((bombtotal < MaxBomb) && (ammo2 > 0)) 
         {
             dec(&ammo2);
             inc(&bombtotal);
             bomb[bombtotal].x = x + 5;
             bomb[bombtotal].y = y + 5;
             bomb[bombtotal].xv = xv;
             bomb[bombtotal].yv = yv + 2;
             bomb[bombtotal].power = 8;
             bomb[bombtotal].kind = 1;
             noise3 = 10;
             if (xv >= 0) 
                 bomb[bombtotal].dir = 0.7;
             if (xv < 0) 
                 bomb[bombtotal].dir = -0.7;
             gotoxy(34,23);
             writeln_u16_str(ammo," ");
         }
              break; 
     case 27:  Shutdown(); break;
     case 'e': case 'E': case 60:
          dostatus();
          ammo = 10;
          man.time = 0;

          if ((eject == 0) && (status > 0) && (under == false)) 
          {
              inc(&totalejects);
              man.x= x +2;
              man.y= y -10;
              man.xv = xv;
              if (man.xv > 0) man.xv = man.xv - 3;
              else man.xv = man.xv + 3;
              man.yv = yv-2.5;
              if (y < 25) { man.y = y+10; man.yv = yv; }
              gotoxy (1,1);
              writeln_str("EJECTED                 ");
              gotoxy(34,23);
              writeln_u16_str(ammo," ");
              eject = 1;
          }
          if (((eject == 0) && (status > 0)) || ((under) && (eject == 0) && (status > 0))) 
          {
              gotoxy (1,1);
              man.x= x - (xv*4);
              man.y= y - 3 - (yv*4);
              if (landed < 2) 
              {
                  man.xv = -xv * 3;
                  man.yv = -yv -3;
                  inc(&totalejects);
                  writeln_str("Auto-Ejected             ");
              }
              else {
                  man.xv = 2;
                  man.yv = -0.9;
                  man.y = y-11;
                  writeln_str("Exited                   ");
              }
              gotoxy(34,23);
              writeln_u16_str(ammo," ");
          }
         eject = 1;
        break;
     case ' ': 
        if ((bombtotal < MaxBomb) && (xv >= 0) && (ammo > 0)) 
        {
            inc(&bombtotal);
            bomb[bombtotal].x = x + 10;
            bomb[bombtotal].y = y - 1;
            bomb[bombtotal].xv = xv + 16;
            bomb[bombtotal].yv = yv - 1.5;
            bomb[bombtotal].power = bpower;
            bomb[bombtotal].kind = 0;
            gotoxy(34,23);
            dec(&ammo);
            writeln_u16_str(ammo," ");
        }
        if ((bombtotal < MaxBomb) && (xv < 0) && (ammo > 0)) 
        {
            dec(&ammo);
            inc(&bombtotal);
            bomb[bombtotal].x = x - 1;
            bomb[bombtotal].y = y - 1;
            bomb[bombtotal].xv = xv - 16;
            bomb[bombtotal].yv = yv - 1.5;
            bomb[bombtotal].power = bpower;
            bomb[bombtotal].kind = 0;
            gotoxy(34,23);
            writeln_u16_str(ammo," ");
        }
               break;
     case 'f': gotoxy (1,1); writeln_str_i32_str("Memory Avail. : ",memavail(),"   "); break;
     case 59: case 'g':case 'G':
     switch (landed) {
                  case 0 : landed = 1; gotoxy (29,1); writeln_str("gear DOWN"); break;
                  case 1: case 2: landed = 0; gotoxy (29,1); writeln_str("gear UP  "); break;
           }
     break;

   } // ( of case KeyHit )
  if (landed == 0) // (* if gear up *)
  {
      if (xv > MaxSpeed) xv = MaxSpeed;
      if (xv < -MaxSpeed) xv = -MaxSpeed;
  }
   else
   {
       if (xv > MaxSpeed) xv = MaxSpeed*0.75;
       if (xv < -MaxSpeed) xv = -MaxSpeed*0.75;
   }
 }


void BombBlowUp(double xxc, double yyc, double power)
{
    int16_t loop;
    (void) loop;


    if ((power < 6) && (power > 1.6)) 
    {
        circle (round_(xxc),round_(yyc),round_(power),2);
        fillshape (round_ (xxc),round_ (yyc+1),0,2);
        circle (round_(xxc),round_(yyc),round_(power),0);
        putpixel (round_(xxc),round_(yyc),0);
        if (getpixel (round_(xxc),round_(yyc)+6) == 1) 
            RememberHole (round_ (xxc-power), round_ (yyc-power), round_ (xxc+power), round_ (yyc+power));
    }

    if (power >= 6) BlowUp(xxc,yyc,round_(power));
    if ((power >= 6) && (bomb[counter].kind == 1)) BlowUp(xxc-9,yyc,round_(power));
    CheckBuildings();
}

void GearDown() //  ( landing gear )
{
    if ((getdotcolor(round_(x),round_(y+2))>0) && (getdotcolor(round_(x+9),round_(y+2))>0)) 
    {
        if ((getdotcolor(round_(x),round_(y+1))==0) && (getdotcolor(round_(x+9),round_(y+1))==0)) 
        {
            if ((abs_f64(xv)+abs_f64(yv)) < 1) 
            {
                dx=100;dy=100;mfall=0;
                landed = 2;
                xv = 0; yv = 0;
                gotoxy (1,1);
                putpic (blank,round_(x),round_(y));
                y = y + 1;
                writeln_str("LANDED              ");
            }
            else if (yv > 0) 
            {
                status = status - round_ (yv);
                dostatus();
                fall = fall + abs_f64(yv/10)+0.005;
                yv = yv -yv - yv -0.5;
                noise1 = 4;
            }
        }
    }

}

void MoveHeli()
{
    if ((status == 4) && (eject > 0) && (landed < 2)) yv = yv + 0.1;
    if ((landed < 2)) yv = yv + fall;
    if (y> 205) Crash();
    putpic (blank,round_(x),round_(y));
    x = x + xv;
    y = y + yv;
    if (eject == 0) 
    {
        man.x = x+7;
        man.y = y-2;
    }
    if (y < 15) 
    {
        y =  15;
        yv = 0;
    };
    if (landed == 1) 
    {
        GearDown();
        if (side == 1) x = x + (0.5 * round_(xv));
        if (side == 0) x = x - (0.5 * round_(xv));
    };
    if (landed == 2) yv = 0;

    if ((xv >= 0) && (side == 1)) putpic (heli,round_(x),round_(y));
    if ((xv >= 0) && (side == 0)) putpic (tHeli,round_(x),round_(y));
    if ((xv < 0) && (side == 1)) putpic (heli2,round_(x),round_(y));
    if ((xv < 0) && (side == 0)) putpic (tHeli2,round_(x),round_(y));
}

void MoveEnemyHeli()
{
    struct helitype *eh = &enemyheli;

    // with enemyheli (eh)
    {
        if (eh->y < 10) eh->yv = eh->yv + 3;
        if (eh->y > 210) eh->alive = false;
        if (eh->fall) eh->yv = eh->yv + 0.75;
        if (eh->y < man.y-25) eh->yv = eh->yv + 0.5;
        if (eh->y > man.y-20) eh->yv = eh->yv - 0.5;
        if ((eh->y < man.y) && (eh->x > man.x)) eh->xv = eh->xv - 0.25;
        if ((eh->y < man.y) && (eh->x < man.x)) eh->xv = eh->xv + 0.25;
        if (eh->fall == false) // ( max speed for enemyheli )
        {
            if ((eh->yv > 2)) eh->yv = 2;
            if ((eh->yv < -2)) eh->yv = -2;
            if ((eh->xv < -2)) eh->xv = -2;
            if ((eh->xv > 2)) eh->xv = 2;
        }
        if (getdotcolor(round_ (eh->x+5),round_ (eh->y-25)) > 0) eh->yv = eh->yv + 0.55;
        if (getdotcolor(round_ (eh->x+5),round_ (eh->y+25)) > 0) eh->yv = eh->yv - 0.65;
        putpic (blank,round_(eh->x),round_(eh->y));
        eh->x = eh->x + eh->xv;
        eh->y = eh->y + eh->yv;
        if (eh->y < 15) enemyheli.y = 15;
        if (eh->xv > 0) putpic (enemyhelipic,round_ (eh->x),round_ (eh->y));
        if (eh->xv <= 0) putpic (enemyheli2pic,round_ (eh->x),round_ (eh->y));
        if ((getdotcolor (round_ (eh->x + 4),round_ (eh->y +1)) > 0) ||
            (getdotcolor (round_ (eh->x + 4),round_ (eh->y -6)) > 0)) 
            {
                eh->alive = false;
                inc(&deadheli);
                deheli[curreheli]= true;
                curreheli = 0;
                gotoxy (1,1);
                writeln_str("Helicopter destroyed.    ");
                BlowUp(eh->x,eh->y,round_(10+(eh->xv*2)+eh->yv));
            }
    } // { with }
} // ( of MoveEnemyHeli )

void Crash()
{
    int16_t loop,ssize,xx,yy;
    ssize = 0;
    if (eject == 0) status = -7;
    xx = round_(x);
    yy = round_(y);
    if (!(yv == -1.63738)) 
    {
        ssize = round_(5+abs_f64(yv)*2.5+abs_f64(xv));
        for (loop = 1; loop <= ssize; loop++)
        {
            if (sounds > 0) sound (60-loop);
            circle (round_ (x),round_ (y),loop,2);
            circle (round_ (x+5),round_ (y+3),loop,3);
            circle (round_ (x-5),round_ (y-3),loop,3);
            circle (round_ (x+6),round_ (y-3),loop,2);
            circle (round_ (x-8),round_ (y+2),loop,2);
            circle (round_ (x-2),round_ (y+8),loop,2);
            circle (round_ (x+8),round_ (y-3),loop,2);
            circle (round_ (x),round_ (y-8),loop,2);
        }
        nosound();
        if (eject == 0) yv = 0;
        for (loop = 1; loop <= round_(5+abs_f64(yv)*2.5+abs_f64(xv)); loop++)
        {
            if (sounds > 0) sound (45-loop);
            circle (round_ (x),round_ (y),loop,0);
            circle (round_ (x+5),round_ (y+3),loop,0);
            circle (round_ (x-5),round_ (y-3),loop,0);
            circle (round_ (x+6),round_ (y-3),loop,0);
            circle (round_ (x-8),round_ (y+2),loop,0);
            circle (round_ (x-2),round_ (y+8),loop,0);
            circle (round_ (x+8),round_ (y-3),loop,0);
            circle (round_ (x),round_ (y-8),loop,0);
        }
    }
    nosound();
    if ((cruise.x-15 < xx+ssize) && (cruise.x+30 > xx-ssize) && (cruise.y-15<yy+ssize)&&(cruise.y+30>yy-ssize) &&
            (cruise.alive)) CruiseCrash();
    if ((pl.x-15 < xx+ssize) && (pl.x+30 > xx-ssize) && (pl.y-15<yy+ssize)&&(pl.y+30>yy-ssize) &&
            (pl.alive)) PlCrash();
    if ((tank.x-15 < xx+ssize) && (tank.x + 30 > xx-ssize) && (tank.y-15<yy+ssize)&&(tank.y+30>yy-ssize) &&
            (tank.alive) && (tank.screen == onscreen)) 
    {
        tank.alive = false;
        BlowUp (tank.x,tank.y,15);
    }
    if ((enemyheli.x-15 < xx+ssize) && (enemyheli.x+30 > xx-ssize) && (enemyheli.y-15<yy+ssize)&&(enemyheli.y+30>yy-ssize) && (enemyheli.alive)) 
    {
        enemyheli.alive=false;
        inc(&deadheli);
        deheli[curreheli] = true;
        curreheli = 0;
        gotoxy (1,1); writeln_str("Helicopter destroyed.    ");
        BlowUp(enemyheli.x,enemyheli.y,round_(10+(enemyheli.xv*2)+enemyheli.yv));
    }
    if ((x < 320) && (x > 0) && (eject > 0)) 
        RememberHole (round_ (x-ssize-10), round_ (y-ssize-10), round_ (x+10+ssize), round_(y+ssize+10));
    gotoxy (1,1);writeln_str("Crash!!               ");
    if (eject == 0) 
    {
        man.cond = 0;
        quit = true;
    }
    if ((man.x < xx+ssize) && (man.x > xx-ssize) && (man.y < yy+ssize) && (man.y > yy-ssize)) man.cond = 0;
    if ((man.x-5 < xx+ssize) && (man.x+10 > xx-ssize) && (man.y-5<yy+ssize) && (man.y+10>yy-ssize)) 
        man.cond=man.cond-1;
    if ((man.x-10 < xx+ssize) && (man.x+20 > xx-ssize) && (man.y-10 < yy+ssize) && (man.y+15 > yy-ssize)) 
        man.cond = man.cond -1;
    for (loop = 1; loop <= totalmen; ++loop)
        if ((sold[loop].x-10 < xx+ssize) && (sold[loop].x+20 > xx-ssize) && (sold[loop].y-10 < yy+ssize)
                && (sold[loop].y+15 > yy-ssize) && (sold[loop].screen == onscreen)) 
        {
            sold[loop].cond = 0;
            setfillstyle (1,0);
            bar (round_(sold[loop].x-5),round_(sold[loop].y-10),round_(sold[loop].x+5),round_(sold[loop].y+1));
            counter = loop; // ( for delete men )
            setfillstyle (1,1);
            DeleteMan(loop);
        }
    if (eject > 0) 
    {
        x = (320*(3-onscreen));
        y = 50;
        xv = 0;
        yv = 0;
        landed = 0;
        status = 4;
        fall = 0;
        ammo = 20;
        if (level == '1') ammo = 50;
        if (level == '2') ammo = 40;
        auto_ = true;
        fall = 0;
    }
    gotoxy (1,1);
    if (splash) writeln_str("Splash!!");
    dostatus();
    CheckBuildings();
}


void Doscreen()
{
    int16_t loop;
    nosound();
    setfillstyle(1,1);

    for (loop = 1; loop <= bombtotal; loop++)
        DeleteBomb();

    cleardevice();

    if (onscreen == 0) setbkcolor(0);
    else setbkcolor(bkcolor);
  
    switch (onscreen) {
   case 0: DrawGround0(&ammo,&ammo2,&ammo3,&status,&fall); break;
   case 1: DrawGround1(); break;
   case 2:
     DrawGround2();
     if (ammo > 500) // (* if chopper went to MARK CURRIE ZONE then *)
      for (loop = 1; loop <= MaxSoldiers; ++loop)
          soldeject (loop * 5 + 40, -4);
   
     break;
   case 3 :
     DrawGround3();
     setcolor (3);
     if (cruise.xv == -32)
     {
       setfillstyle (1,0);
       bar (0,75,80,150);
       setfillstyle (1,1);
     }
     if (building[11]) DrawGoodGun (237,153);
     if (building[1]) DrawBase();
     gotoxy (29,1);
     if (landed == 0) writeln_str("gear UP  ");
     else writeln_str("gear DOWN");
     break;

   case 4 : DrawGround4(); break;
   case 5 :
     DrawGround5();
     if (eheli[1] == false)
     {
       enemyheli.alive = true;
       enemyheli.xv = 0;
       enemyheli.yv = 0;
       enemyheli.y = 168;
       enemyheli.x = 285;
       enemyheli.fall = false;
       eheli[1] = true;
       curreheli = 1;
     }
   break;
  case 6 :
    DrawGround6();
    if (building[2]) DrawTower(); // { enemy boundary marker }
    if (eheli[2] == false)
    {
      enemyheli.alive = true;
      enemyheli.xv = 0;
      enemyheli.yv = 0;
      enemyheli.y = 149;
      enemyheli.x = 270;
      enemyheli.fall = false;
      eheli[2] = true;
      curreheli = 2;
    }
   break;
  case 7 :
    DrawGround7();
    if (building[3]) DrawBuilding (200,154);
    if (building[12]) putpic(enemy,100,154);
   break;
  case 8 :
    DrawGround8();
    if (building[4]) DrawRadar (200,128);
    if (building[5]) DrawBuilding (220,139);
   break;
  case 9 :
    DrawGround9();
   break;
  case 10 :
    DrawGround10();
    if (building[6])
    {
      DrawBuilding (50,139);
      DrawRadar (52,87);
    }
    if (building[7]) DrawBuilding (250,174);
    if (eheli[3] == false)
    {
      enemyheli.alive = true;
      enemyheli.xv = 0;
      enemyheli.yv = 0;
      enemyheli.y = 138;
      enemyheli.x = 80;
      enemyheli.fall = false;
      eheli[3] = true;
      curreheli = 2;
      inc(&totalmen);

      // with sold[totalmen] do
      {
          sold[totalmen].x = 245;
          sold[totalmen].y = 170;
          sold[totalmen].xv = 0;
          sold[totalmen].yv = 0;
          sold[totalmen].eject = 0;
          sold[totalmen].cond = 4;
          sold[totalmen].screen = 10;
      }
    }  // { if eheli[3] }
   break;
  case 11 :
   
    DrawGround11();
    if (building[8]) DrawBuilding (200,149);
    if (mis == 8)
        if (step == 0) draw (50,174,51,174,2);
   break;
  case 12 :
    DrawGround12();
    if (building[9]) DrawTower();
   break;
  case 13 :
    DrawGround13();
   break;
  case 14 :
    DrawGround14();
   break;
  case 15 :
    DrawGround15();
    if (building[10]) DrawBase();
    gotoxy (29,1);
    if (landed == 0) writeln_str("gear UP  ");
    else writeln_str("gear DOWN");
   break;
  case 16 :
    DrawGround16();
    gotoxy (1,1);
    writeln_str("press \"E\" to exit craft");
   break;
  case 17 :
   DrawGround17();
   if ((eject > 0) && (man.y > 138))
   {
      man.cond = 4;
      status = 300;
      fall = 0;
      ammo = 250;
      ammo2 = 10;
      ammo3 = 10;
      bpower = 8;
   }
   break;
    } 

  gotoxy (29,23);
  write_str_i16("AMMO:",ammo);
  dostatus();
  DoHoles();
}

void Autopilot()
{

    yv = yv - 0.2;
    if (x < man.x + 5) xv = xv + 0.25;
    if (x > man.x - 8) xv = xv - 0.25;
    if ((x > man.x - 5) && (x < man.x +5)) 
    {
        xv = xv / 1.4;
        if (y < man.y - 7) yv = yv + 0.5;
    }
    if (y > man.y ) yv = yv - 0.5;
    if (getdotcolor (round_(x+ 20),round_(y)) > 0) xv = xv - 0.75;
    if (getdotcolor (round_(x- 20),round_(y)) > 0) xv = xv + 0.75;
    if (xv > 2) xv = 2;          // ( max speeds )
    if (xv < -2) xv = -2;
    if (y > man.y - 30) 
        if (yv > 0.5) yv = 0.5;
    if (yv > 1) yv = 1;
    if (yv < -2) yv = -2;

}

void Enemyshoot()
{

    if ((tank.alive) && (tank.screen == onscreen))
        Fire(tank.x,tank.y,man.x,man.y);

    for (loop = 1; loop <= totalmen; loop++)
    {
        if (sold[loop].screen == onscreen) 
            if ((sold[loop].x > 0) && (sold[loop].x < 320)) 
                if ((sold[loop].cond > 0) && (sold[loop].yv == 0) && (abs_f64(sold[loop].xv) < 0.2)) 
                {
                    if ((onscreen != 3)) 
                        Fire (sold[loop].x+1,sold[loop].y-4,man.x,man.y*1.5); // ( *1.5 so it shoots a little low )
                    else if (building[1])
                        Fire (sold[loop].x+1,sold[loop].y-4,10,110); // ( Fire at base )
                    else Fire (sold[loop].x+1,sold[loop].y-4,man.x,man.y*1.5);
                }
    }

    if (eject == 0)
    {
        if ((enemyheli.alive) && (enemyheli.x < 400) && (enemyheli.y < man.y + 14)) 
            Fire (enemyheli.x+3,enemyheli.y+4,man.x,man.y+5);
    }
    else if ((enemyheli.alive) && (enemyheli.x < 400) && (enemyheli.y < man.y + 20)) 
        Fire (enemyheli.x+3,enemyheli.y-20,man.x,man.y+5);
    if (mis == 3) 
        if ((enemyheli.alive) && (enemyheli.x < 250) && (onscreen == 3)) 
            Fire (enemyheli.x+3,enemyheli.y+8,33,140);
    
    switch (onscreen)
    {
        case 3 : 
            if ((getdotcolor (240,151) == 3) && (enemyheli.alive) && (enemyheli.x < 325) && (enemyheli.x > 20)) 
                Fire(240,148,enemyheli.x,enemyheli.y);
            break;
        case 6 : 
            if (building[2]) 
            {
                if (time_ == 20) plot (250,125,2);
                if (time_ == 35) 
                {
                    plot (250,125,0);
                    if ((x > 0) && (x < 320)) Fire (250,125,x,y);
                }
            }
            break;
        case 7 :
            if (building[12])
                Fire (104,150,man.x,man.y);
            break;
        case 12 :
            if (building[9]) 
            {
                if (time_ == 20) plot (250,125,2);
                if (time_ == 35) 
                {
                    plot (250,125,0);
                    if ((x > 0) && (x < 320))
                        Fire (250,125,x,y);
                }
            }
            break;
    }
} 

void CheckGround()
{
    splash = false;
    if (status > -7) 
    {
        if ((getdotcolor (round_ (x+5),round_ (y-6)) > 0) ||
            (getdotcolor (round_ (x), round_  (y+1)) > 0) ||
                (getdotcolor (round_ (x+9),round_ (y+1)) > 0)) 
                {
                    status = status - round_ (abs_f64(xv)/2) - round_ (abs_f64(yv));
                    fall = fall + abs_f64(yv/10) + 0.005;
                    yv = yv -yv -yv -0.5;
                    noise1 = 4;
                    if ((area == 1) && (onscreen == 9)) 
                        if ((x > 170) && (y < 300) && (y > 120)) splash = true;
                    if ((area == 1) && (onscreen == 12)) 
                        if ((x > 245) && (y > 160)) splash = true;
                    if ((area == 1) && (onscreen == 13)) 
                        if ((y > 160)) splash = true;
                    if ((area == 1) && (onscreen == 14)) 
                        if ((x < 150) && (y > 160)) splash = true;
                    if (splash) 
                    {
                        gotoxy (1,1);
                        write_str ("Splash             ");
                        noise1 = 0;
                        fall = fall - 0.005;
                        if (status < 1) yv = -1.63738; // ( no crash )
                    }
                    dostatus();
                }
    }
    if ((man.cond < 1) && (eject == 0)) Crash();
} // ( CheckGround )

void EngineSounds()
{
    if ((x > 0) && (x < 320)) 
    {
        if (side == 0) sound (round_ (100-(y/3.0) + abs_f64(xv)*8 - yv*7));
        if (landed == 2) sound (35);
        if (side == 1) nosound();
    }
    else nosound();
}

void ManCheck()  // ( man in air (not soldier) )
{
    delay (del-round_(bombtotal/3)+10);
    if ((getdotcolor(round_ (man.x+1),round_ (man.y-4))>0)) // ( head )
    {
        gotoxy (1,1);
        writeln_str("Ahh my head!            ");
        man.xv = 0;
        man.cond = 0;
        dostatus2();
    }
    if ((getdotcolor(round_(man.x-1),round_ (man.y-2)) > 0)) 
        if (man.xv < 0) man.xv = 0;
    if ((getdotcolor(round_(man.x+3),round_ (man.y-2)) > 0)) 
        if (man.xv > 0) man.xv = 0;
    putpic (pblank,round_ (man.x),round_ (man.y));
    man.xv = man.xv /1.1;
    man.time = man.time + 1;
    man.yv = man.yv + 0.25;
    if (man.time > 20) man.xv = 0;
    if ((man.time > 20) && (man.yv > 1)) man.yv = 1;
    man.x = man.x + man.xv;
    man.y = man.y + man.yv;
    if (man.y < 15) 
    {
        man.y = 15;
        man.yv = 0;
    }
    if (man.time < 21) 
    {
        if (abs_f64(man.xv) > 0.1) 
        {
            if (side == 0) putpic (pict1,round_ (man.x),round_ (man.y));
            if (side == 1) putpic (pict2,round_ (man.x),round_ (man.y));
        }
        else putpic (pict1,round_ (man.x),round_ (man.y));
    }
    if ((man.time > 20))
        putpic (pict3,round_ (man.x-1),round_ (man.y));
    if (man.y >200) 
    {
        man.cond= 0;
        eject =2;
        man.yv = 0;
        dostatus2();
    }
    if ((getdotcolor(round_(man.x),round_(man.y+2))>0) || (getdotcolor(round_(man.x+2),round_(man.y+1))>0)) 
    {
        if (man.time < 21) 
        {                                         // ( impact on ground )
            man.yv = -man.yv;
            man.yv = man.yv/2;
            if (abs_f64(man.yv) < 1.5) 
            {
                man.yv = 0;
                eject = 2;
            }
            man.time = -100;
            if ((abs_f64(man.yv) > 1) || (abs_f64(man.xv) > 2)) 
            {
                gotoxy (1,1);
                man.cond = man.cond - round_(abs_f64(man.yv/1.5) - abs_f64(man.xv/8));
                if (round_(abs_f64(man.yv/1.5) + abs_f64(man.xv/8)) > 0) writeln_str("Ouch!                  ");
            }
            dostatus2();
        }
        else
        {
            man.yv = 0;
            eject =2;
            dostatus2();
            putpic (pblank,round_(man.x-1),round_ (man.y-4)); // ( erase parachute )
            putpic (pblank,round_(man.x+1),round_ (man.y-4));
            putpic (pblank,round_(man.x),round_(man.y-5));
            man.time = 0;
            gotoxy (1,1);
            writeln_str("Wait for backup.         ");
            dx = 100;
            dy = 100;
        }
    }
    if (mousee) 
    {
        if (side == 1) 
        {
            if (LeftKeyPressed()) keyhit = '7';
            if (RightKeyPressed()) keyhit = '9';
        }

        if (BothKeyPressed()) 
        {
            if (side == 1) keyhit = '7';
            if (side == 0) keyhit = '9';
        }
    }
    if ((keypressed() || mousee) && (man.cond > 0)) 
    {
        if (keypressed()) keyhit = readkey();

        switch (keyhit)
        {
            case '7':case 71:
                if ((bombtotal < MaxBomb) && (ammo > 0)) 
                {
                    dec(&ammo);
                    inc(&bombtotal);
                    bomb[bombtotal].x = man.x - 1;
                    bomb[bombtotal].y = man.y - 2;
                    bomb[bombtotal].xv = man.xv - 9;
                    bomb[bombtotal].yv = man.yv - 2;
                    bomb[bombtotal].kind = 0;
                    bomb[bombtotal].power = 4;
                    gotoxy(34,23);
                    writeln_u16_str(ammo," ");
                }
                break;
            case '9': case 73:
                if ((bombtotal < MaxBomb) && (ammo > 0)) 
                {
                    dec(&ammo);
                    inc(&bombtotal);
                    bomb[bombtotal].x = man.x + 2;
                    bomb[bombtotal].y = man.y - 2;
                    bomb[bombtotal].xv = man.xv + 9;
                    bomb[bombtotal].yv = man.yv - 2;
                    bomb[bombtotal].kind = 0;
                    bomb[bombtotal].power = 4;
                    gotoxy(34,23);
                    writeln_u16_str(ammo," ");
                }
                break;
            case 27 : Shutdown(); break;
            case 'p':case 'P' : Pause(); break;
            case 's':case 'S' : Speeds(); break;
            case 't':case 'T' : ToggleMouse(); break;
            case 'x':case 'X' : CheckSound(); break;
        } // { case }
    keyhit = '~';
    }
    if ((man.cond < 1) && (time_ == 30)) quit = true;
}

void ToggleMouse()
{
    gotoxy (1,1);
    if (MousePresent())
    {
        mousee = mousee ^ true;
        dx = 100;
        dy = 100;
        mfall = 0;
        if (mousee == false) writeln_str("Mouse is OFF               ");
        else writeln_str("Mouse is ON                ");
    }
    else
        writeln_str("Mouse not present.      ");
}

void Input2() // ( after eject )
{
    keyhit = 'd';
    delay (del-round_(bombtotal/3)+10);
    if (keypressed()) keyhit = readkey();
    if (mousee) DoMouse();
    switch (keyhit)
    {
        case '8': case 72 :
            eject = 1;
            man.time = -120;
            man.yv = man.yv-2;
            break;
        case '4': case 75 :
            man.xv = man.xv - 1;
            if (man.xv < -3) man.xv = -3;
            if (getdotcolor (round_(man.x-1),round_(man.y)) > 0) 
            {
                man.yv = man.yv - 1.5;
                eject = 1;
                man.time = -120;
            }
            break;
        case '7': case 71:
            if ((bombtotal < MaxBomb) && (ammo > 0)) 
            {
                dec(&ammo);
                inc(&bombtotal);
                bomb[bombtotal].x = man.x - 1;
                bomb[bombtotal].y = man.y - 2;
                bomb[bombtotal].xv = man.xv - 9;
                bomb[bombtotal].yv = man.yv - 2;
                bomb[bombtotal].kind = 0;
                bomb[bombtotal].power = 4;
                gotoxy(34,23);
                writeln_u16_str(ammo," ");
            }
            break;
        case '9': case 73  :

                  if ((bombtotal < MaxBomb) && (ammo > 0)) 
                  {
                      dec(&ammo);
                      inc(&bombtotal);
                      bomb[bombtotal].x = man.x + 2;
                      bomb[bombtotal].y = man.y - 2;
                      bomb[bombtotal].xv = man.xv + 9;
                      bomb[bombtotal].yv = man.yv - 2;
                      bomb[bombtotal].kind = 0;
                      bomb[bombtotal].power = 4;
                      gotoxy(34,23);
                      writeln_u16_str(ammo," ");
                  }

                  break;
        case '6': case 77:
                  man.xv = man.xv + 1;
                  if (man.xv > 3) man.xv = 3;
                  if (getdotcolor (round_(man.x+3),round_(man.y)) > 0) 
                  {
                      man.yv = man.yv - 1.5;
                      eject = 1;
                      man.time = -120;
                  }
                  break;
        case 27 : Shutdown(); break;
        case 'p':case 'P' : Pause(); break;
        case 's':case 'S' : Speeds(); break;
        case 't':case 'T' : ToggleMouse(); break;
        case 'x':case 'X' : CheckSound(); break;
    }
}

// ( going east )

void CheckScreen()
{
    if (enemyheli.alive) enemyheli.x = enemyheli.x - 320;
    if (cruise.alive) cruise.x = cruise.x-320;
    if (pl.alive) pl.x = pl.x-320;
    inc(&onscreen);
    x = x - 295;
    man.x = 5;
    Doscreen();
} 

void CheckScreen2() // ( going west )
{
    if (enemyheli.alive) enemyheli.x = enemyheli.x + 320;
    if (cruise.alive) cruise.x = cruise.x+320;
    if (pl.alive) pl.x = pl.x+320;
    dec(&onscreen);
    x = x + 300;
    man.x = 302;
    Doscreen();
}

void ManCheck2() // ( man on ground )
{
    if ((getdotcolor(round_(man.x),round_(man.y+1))==0) && (getdotcolor(round_(man.x+2),round_(man.y+1))==0)) 
    {
        eject =1;
        dostatus2();
    }
    if ((getdotcolor(round_ (man.x+1),round_ (man.y-4))>0)) // ( head )
    {
        gotoxy (1,1);
        writeln_str("Ahh my head!            ");
        man.xv =0;
        man.cond = 0;
        dostatus2();
    }
    if (abs_f64(man.yv) < 0.3) man.yv = 0;
    if ((getdotcolor(round_(man.x-1),round_ (man.y)) > 0)) 
        if (man.xv < 0) man.xv = 0;
    if ((getdotcolor(round_(man.x+3),round_ (man.y)) > 0)) 
        if (man.xv > 0) man.xv = 0;
    putpic (pblank,round_ (man.x),round_ (man.y));
    man.yv = 0;
    man.xv = man.xv / 1.2;
    man.x = man.x + man.xv;
    man.y = man.y + man.yv;
    if ((man.cond < 1) && (time_ == 30)) quit = true;
    if (abs_f64(man.xv) > 0.1) 
    {
        if (side == 0) putpic (pict1,round_ (man.x),round_ (man.y));
        if (side == 1) putpic (pict2,round_ (man.x),round_ (man.y));
    }
    else putpic (pict1,round_ (man.x),round_ (man.y));
}

void DeleteMan (int16_t loop)
{
    int16_t counter;
    counter = 0;
 
    do {
        inc(&counter);
        sold[loop+counter-1]=sold[loop+counter];
    } while (!((counter + loop) >= totalmen));
    dec(&totalmen);
    if (totalmen < 0) totalmen = 0;
}

void CalcTime()
{
    int16_t t1,t2,t3,t4,t5,t6,t7,t8;

    gettime(&time5,&time6,&time7,&time8);
    t1 = time1; t2 = time22; t3 = time3; t4 = time4; t5 = time5; t6 = time6; t7 = time7; t8 = time8;
    CalcDif = (t5-t1) * 3600.0;
    CalcDif = CalcDif + (t6- t2) * 60.0;
    CalcDif = CalcDif + (t7- t3);
    CalcDif = CalcDif + (t8 - t4) / 100.0;
}

void Rearm()
{
    nosound();
    gotoxy (1,1);
    writeln_str("Safe landing. Rearmed. ");
    delay (2000);
    if (landed == 2) 
    {
        if (level == '1') ammo = 150;
        if (level == '2') ammo = 75;
        if (level == '3') ammo = 50;
        ammo2 = 5;
        ammo3 = 5;
    }
    if (eject == 2) ammo = 10;
    ammo2 = 5;
    ammo3 = 5;
    gotoxy(34,23);
    writeln_u16_str(ammo," ");
    gotoxy (1,2);
    if (eject == 0) 
    {
        fall = 0;
        status = 4;
        man.cond = 3;
        dostatus();
    }
    if (!(onscreen == 15) || !(mis == 6)) 
    {
        gotoxy (1,2);
        writeln_str("Are you returning? (Y/N) ");
        keyhit = readkey();
        if ((keyhit == 'Y') || (keyhit == 'y')) 
        {
            quit = true;
            gotoxy (1,1);
            writeln_str("Welcome home pilot.         ");
        }
        else {
            gotoxy (1,2);
            writeln_str("                               ");
        }
    }
    else if (step == 0)
    {
        gotoxy (1,1);
        step = 1;
        CalcTime();
        if (CalcDif < 120) step = 2;
        writeln_str("Device transferred.    ");
        delay (1500);
    }
}

void DoPlane()
{

    // with pl do
    {
        putimage (round_(pl.x),round_(pl.y),bplane,0);
        if (pl.engine < 0)
            if (pl.xv > pl.engine) pl.xv = pl.xv - 1;
        pl.x = pl.x + pl.xv;
        pl.xv = pl.xv / 1.5;
        pl.y = pl.y + pl.yv+pl.fall;
        if ((getpixel (round_(pl.x+3),round_(pl.y+12)) > 0) && (pl.x < 315) && (pl.x > 0)) 
        {
            pl.y = pl.y - 4;
            pl.yv = -pl.yv;
            pl.fall = -pl.fall;
            noise1 = 4;
            pl.status = pl.status - round_ (pl.fall + pl.yv);
            hurtpl();
        }
        if ((getpixel (round_(pl.x-3),round_(pl.y)) > 0) && (pl.x < 320) && (pl.x > 4)) 
        {
            pl.x = pl.x + 4;
            pl.xv = -pl.xv-4;
            noise1 = 4;
            pl.status = pl.status - round_ (pl.fall + pl.yv + 1);
            hurtpl();
        }
        if (((onscreen == 3) && (pl.x > -10) && (pl.x < 330)) || (pl.fall > 0)) 
            if ((time_ == 10) || (time_ == 25) || (time_ == 40) || (time_ == 55)) soldeject (pl.x,pl.y);
        if ((pl.x < 150+(3-onscreen)*320) && (pl.y < 100)) 
            if ((onscreen > 3) && (building[1])) 
            {
                building[1] = false;
                TextDie (1);
            }
        if ((pl.x < 150+(1-onscreen)*320) && (onscreen > 1)) 
        {
            pl.yv = -32;
            pl.alive = false;
        }
        if (pl.fall > 1) pl.fall = 1;
        if (pl.fall != 0) pl.fall = pl.fall + 0.02;
        putimage (round_(pl.x),round_(pl.y),plane,0);
        if ((pl.status < 1) || (pl.y > 199)) PlCrash();
    } // { with }
}

void mis19()
{
    tank.x = round_(pl.x + 9);
    tank.y = round_(pl.y + 16);
    if ((pl.x < 320) && (pl.x > 0)) 
    {
        if ((pl.alive)) tank.screen = onscreen;
        if ((pl.fall != 0)) // (* release tank *)
        {
            step = 1;
            tank.x = tank.x + 5;
        }
    }
}

void PlotDeadSold (double x, double y)
{
    plot (round_ (x-1), round_ (y), 3);
    plot (round_ (x), round_ (y), 3);
    plot (round_ (x+1), round_ (y), 3);
    plot (round_ (x), round_ (y-1), 3);
}

void DoMen() //  ( enemy soldiers )
{

    for (loop = 1; loop <= totalmen; ++loop) {
        if (sold[loop].screen == onscreen) 
        {
            sold[loop].time = sold[loop].time + 1;
            if (sold[loop].x < man.x - 40) sold[loop].xv = 0.25 - sold[loop].xv;
            if (sold[loop].x > man.x + 40) sold[loop].xv = - (0.25 - sold[loop].xv);
            if (sold[loop].eject < 2) 
            {
                if ((getdotcolor(round_(sold[loop].x),round_(sold[loop].y+1)) == 0) &&
                        (getdotcolor(round_(sold[loop].x+2),round_(sold[loop].y+1)) == 0)) 
                    sold[loop].eject =1;       //          { check under sold }
                else sold[loop].eject = 0;
            }
            if (sold[loop].eject == 0) 
            {
                sold[loop].time = -120;
                if (sold[loop].yv > 0) 
                {
                    if (sold[loop].yv > 1) sold[loop].yv = -sold[loop].yv/4;
                    else sold[loop].yv = 0;
                    sold[loop].cond = sold[loop].cond + round_(sold[loop].yv); // ( damage taken on impact )
                }
            }
            if (sold[loop].eject == 1) sold[loop].yv = sold[loop].yv + 0.4;
            if ((getdotcolor(round_ (sold[loop].x+1),round_ (sold[loop].y-4))>0)) // ( check head )
            {
                sold[loop].xv = 0;
                sold[loop].cond = 0;
            }
            if ((getdotcolor(round_(sold[loop].x-1),round_ (sold[loop].y)) > 0)) 
                if ((sold[loop].xv < 0) && (sold[loop].cond > 0)) 
                {
                    sold[loop].xv = 0;  // ( check left side )
                    sold[loop].yv = -1.2;
                }
            if ((getdotcolor(round_(sold[loop].x+3),round_ (sold[loop].y)) > 0)) 
                if ((sold[loop].xv > 0) && (sold[loop].cond > 0)) 
                {
                    sold[loop].xv = 0;  // ( check right side )
                    sold[loop].yv = -1.2;
                }
            putpic (pblank,round_ (sold[loop].x),round_ (sold[loop].y));
            if (abs_f64(sold[loop].yv) < 0.3) sold[loop].yv = 0;
            sold[loop].xv = sold[loop].xv /1.2;
            if (sold[loop].time > 19) sold[loop].yv = 1;
            sold[loop].x = sold[loop].x + sold[loop].xv;
            sold[loop].y = sold[loop].y + sold[loop].yv;
            if (sold[loop].y < 15) sold[loop].y = 15;
            if (sold[loop].time < 20) 
                if (abs_f64(sold[loop].xv) > 0.1) 
                {
                    if (side == 0) putpic (pict1,round_ (sold[loop].x),round_ (sold[loop].y));
                    if (side == 1) putpic (pict2,round_ (sold[loop].x),round_ (sold[loop].y));
                }
                else putpic (pict1,round_ (sold[loop].x),round_ (sold[loop].y));
            else
            {
                sold[loop].eject = 2;
                sold[loop].xv = 0;
                sold[loop].cond = 3;
                putpic (pict3,round_ (sold[loop].x),round_ (sold[loop].y));
                if (getdotcolor (round_ (sold[loop].x+1),round_ (sold[loop].y+2)) > 0) 
                {
                    sold[loop].time = -120;
                    sold[loop].eject = 0;
                    setfillstyle (1,0);
                    bar (round_ (sold[loop].x-1),round_ (sold[loop].y-10),round_ (sold[loop].x+4),round_ (sold[loop].y));
                    // ( erase parachute )
                }
            }
            if (sold[loop].cond < 1) 
                if ((abs_f64(yv) + abs_f64(xv) < 0.8)) 
                    if ((getpixel (round_(sold[loop].x+1),round_(sold[loop].y+1)) > 0) ||
                            (getpixel (round_(sold[loop].x),round_(sold[loop].y+1)) > 0) ||
                            (getpixel (round_(sold[loop].x+3),round_(sold[loop].y+1)) > 0)) 
                    {
                        gotoxy (1,1);
                        writeln_str("Soldier killed.        ");
                        putpic (pblank,round_ (sold[loop].x),round_ (sold[loop].y));
                        PlotDeadSold (sold[loop].x+1,sold[loop].y);
                        DeleteMan (loop);
                    }
            if (sold[loop].y > 215) DeleteMan (loop);
        }
    }
    // ( of loop )
} // ( of doMan )

// practice missions
void DoFree()
{
    if (time_ == 5) 
    {
        gotoxy (1,1);
        switch (time2) {
            case 0 : writeln_str("Go up to take off.       "); break;
            case 1: case 2: case 3:
                     if (landed < 2) writeln_str("Proceed East.          ");
                     else writeln_str("Press \"8\" to go up.       ");
                     break;
            case 5  :
                     if ((onscreen < 5) && (deheli[1] == false)) write_str ("Go Right                 ");
                     break;
            case 6  :
                     if (deheli[1]) write_str ("Return home              ");
                     else write_str ("Destroy the helicopter     ");
                     break;
            case 7  :
                     if ((enemyheli.alive) && (enemyheli.x > 320))
                         write_str ("Go east              ");
                     break;
            case 8  :
                     if ((enemyheli.alive) && (enemyheli.x < 0))
                         write_str ("Go west              ");
                     break;
        }
        gotoxy (1,1);
        if ((onscreen == 3) && (deheli[1])) writeln_str("Land on landing pad.     ");
        if (time2 > 7) time2 = 4;
    }
}

void CruiseCrash()
{
    gotoxy (1,1);
    writeln_str("Missile destroyed.     ");
    cruise.alive = false;
    if (mis == 18) 
    {
        setbkcolor (15);
        setbkcolor (bkcolor);
        palette (3);
        palette (2);
        if ((cruise.x < x + 70) && (cruise.x > x - 70)) step = 1;
        if ((cruise.x < 700+(3-onscreen)*320)) step = 2;
    }
    BlowUp(cruise.x+3,cruise.y,20);
    BlowUp(cruise.x+3,cruise.y+5,20);
    if ((cruise.x < 100+(3-onscreen)*320) && (cruise.y > 100)) 
    {
        cruise.xv = -32; // ( -32 means base BlowUp )
        building[1] = false;
        TextDie (1);
        if (onscreen == 3) 
        {
            BlowUp (40,125,30);
            BlowUp (50,125,30);
            BlowUp (50,130,30);
        }
    }
} 


void RememberHole (int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if ((x1 < 0)) x1 = 0;  // (* prevent freeze *)
    if ((y1 < 0)) y1 = 0;
    if ((x2 > 320)) x2 = 319;
    if ((y2 > 200)) y2 = 199;

    if (hole.size[hole.total+1] == 1)
    {
        // with hole do
        {
            inc(&hole.total);
            hole.size[hole.total] = imagesize(x1,y1,x2,y2);
            hole.image[hole.total] = getmem(hole.size[hole.total]);
            getimage(x1,y1,x2,y2, hole.image[hole.total]);
            hole.screen[hole.total] = onscreen;
            hole.hx[hole.total] = x1;
            hole.hy[hole.total] = y1;
        }
    }
}

void DoHoles()
{

    // with hole
    {
        for (loop = 1; loop <= hole.total; loop++)
            if (hole.screen[loop] == onscreen) 
                putimage(hole.hx[loop], hole.hy[loop], hole.image[loop], 0);

    }
}

void DoTank()
{
    if (x > tank.x) tank.xv = 1;
    if (x < tank.x) tank.xv = -1;

    {
        putpic (tank.blank,tank.x,tank.y);
        if (tank.xv < 0) 
            if ((getdotcolor (tank.x-2,tank.y-2) > 0) || (getdotcolor (tank.x-2,tank.y+1) == 0)) 
                tank.xv = 0;
        if (tank.xv > 0) 
            if ((getdotcolor (tank.x+12,tank.y+1) == 0) || (getdotcolor (tank.x+12,tank.y-2) > 0)) 
                tank.xv = 0;
        if (getdotcolor (tank.x + 5,tank.y+1) > 0) tank.yv = 0;
        else tank.yv = 1;
        if (tank.y > 200) tank.alive = false;
        tank.x = tank.x + tank.xv;
        tank.y = tank.y + tank.yv;
        if (step == 4) mis19();

        if (abs_i16(tank.x) > 1) 
        {
            if (side == 1) putpic (tank.pic1,tank.x,tank.y);
            if (side == 0) putpic (tank.pic2,tank.x,tank.y);
        }
        else putpic (tank.pic1,tank.x,tank.y);

    } // ( of with )
} // ( of DoTank )


void Noises() // ( all but crash )
{
    if (noise2 == 0) nosound();
    if (noise2 > 0) sound (noise2*5+500);
    if (noise1 == 0) nosound();
    if (noise3 == 0) nosound();
    if (noise3 > 0) sound (noise3*100+2000);
    if (noise1 > 1) sound (50);
    if (noise1 == 1) sound (35);
    dec(&noise1);
    dec(&noise2);
    dec(&noise3);
}

void MoveHeli();

void Main()
{

    while (quit == false)
    {
        if ((tank.screen == onscreen) && (tank.alive == true)) DoTank();
        else if (step == 4) DoTank();

        // ( if side is 1 its now 0, and vice-versa )

        if (sounds > 0) Noises();

        side = side ^ 1; 
        if (landed < 2) CheckGround(); // ( checks to see if hits the ground )

        if (eject == 0) Input();
        if (landed == 2) 
        {
            xv = 0;
            mfall = 0;
            if (yv > 0) yv = 0;
        }
        if ((eject == 2) && (man.cond > 0)) Input2();
        if (status > -7) MoveHeli();
        if (time_ == 33)  //       (*  on landing pad? *)
            if ((eject == 2) || (landed == 2))
            {
                if (onscreen == 3) 
                    if ((man.x > 15) && (man.x < 60) && (man.y >150) && (time2 > 4)) Rearm();
                if (onscreen == 15) 
                    if ((man.x > 50) && (man.x < 105) && (time2 > 4)) Rearm();
            }
        if (totalmen > 0) DoMen();
        if (eject == 1) ManCheck();
        if (eject == 2) ManCheck2();
        if (cruise.alive) 
        {
            putpic (cruise.blank,cruise.x,cruise.y);
            cruise.x = cruise.x + cruise.xv;
            cruise.y = cruise.y + cruise.yv;
            if (side == 1) putpic (cruise.pic1,cruise.x,cruise.y);
            if (side == 0) putpic (cruise.pic2,cruise.x,cruise.y);
            if (cruise.y < 10) cruise.yv = 1;
            if (cruise.y > 55) cruise.yv = -1;
            if (cruise.x < 100+(3-onscreen)*320) cruise.yv = 6;
            if ((getdotcolor(cruise.x-1,cruise.y+2) > 0) || (cruise.y > 195)) CruiseCrash();
        }
        if (pl.alive) DoPlane();
        if ((eject > 0) && (landed == 2)) 
            if ((y > man.y -8) && (y < man.y + 8) && (x < man.x + 10) && (x > man.x -6) && (man.cond > 0)) 
            {
                eject = 0;
                dx = 100;
                dy = 100;
                gotoxy (1,1);
                writeln_str("Entered                ");
                gotoxy(34,23);
                writeln_u16_str(ammo," ");
                putpic (blank,round_(man.x),round_(man.y));
            }
        if (auto_) 
        {
            Autopilot();
            MoveHeli();
            if ((y > man.y -8) && (y < man.y + 8) && (x < man.x + 10) && (x > man.x -6) && (man.cond > 0)) 
            {
                eject = 0;
                auto_ = false;
                yv = 0;
                xv = 0;
                switch(level) {
                    case '1' : ammo = 40; break;
                    case '2' : ammo = 30; break;
                    case '3' : ammo = 20; break;
                }

                gotoxy (1,1);
                writeln_str("Picked Up!             ");
                gotoxy(34,23);
                writeln_u16_str(ammo," ");
                gotoxy (29,1);
                writeln_str("gear UP  ");
                dostatus();
                putpic (blank,round_(man.x),round_(man.y));
                putpic (blank,round_(man.x),round_(man.y-4));
                dx = 100;
                dy = 100;
            }
        }
        if (free_) DoFree();
        if (sounds == 2) EngineSounds();
        if ((man.x > 313)) CheckScreen();
        if ((man.x < 1)) CheckScreen2();
        if (enemyheli.alive) MoveEnemyHeli();
        inc(&time_);
        if (time_ == 20) Enemyshoot();
        if ((lvl == 1) && (time_ == 35) && (x > 0) && (x < 320)) 
        {
            plot (250,125,0);
            if ((onscreen == 6) && (building[2])) Fire(250,125,x,y);
            if ((onscreen == 12) && (building[9])) Fire(250,125,x,y);
        }
        if ((time_ == 35) && (lvl > 1)) Enemyshoot();
        if ((time_ == 50) && (lvl == 3)) Enemyshoot();
        if (time_ > 59) 
        {
            time_ = 0;
            inc(&time2);
        }
        if (bombtotal > 0) CheckBomb();
        if ((time2 == 1) && (time_ == 20)) 
            if (strcmp(scene, "night") == 0)
            {
                gotoxy (1,1);
                writeln_str("Night Mission.  ");
            }
        if ((time_ == 5) && (time_ == 2)) 
            if ((mis == 1) || (mis == 6)) 
            {
                gotoxy (1,1);
                writeln_str("Hurry Up!            ");
            }
        if ((onscreen == 11) && (mis == 8)) 
        {
            if ((eject > 0) && (man.x > 49) && (man.x < 54) && (man.y > 170)) 
                if (step == 0)
                {
                    draw (50,174,51,174,0);
                    gotoxy (1,1);
                    step = 1;
                    writeln_str("Capsule Retrieved.       ");
                }
            if ((eject == 0) && (y > 150) && (x > 40) && (x < 60) && (step == 0)) 
                draw (50,174,51,174,2);
        }
        if ((enemyheli.alive) && (onscreen > 3) && ((onscreen-4) * 320+enemyheli.x < 0) && (time_ == 30) &&
                ((onscreen-4) * 320+enemyheli.x > -320) && (building[1])) 
        {
            // (* if enemyheli is near base and pilot isn't then enemyheli kills base *)
            TextDie (1);  // (* display good base dead *)
            building[1] = false;
        }
        if (onscreen == 17)
        {
            if ((eject > 0) && (man.y > 145) && (man.x > 20) && (man.x < 40)) 
            {
                gotoxy (1,1);
                write_str ("I have the POWER! ");
                delay (2400);
                man.cond = 99;
                status = 200;
                fall = 0;
                if ((ammo < 250)) ammo = 250;
                ammo2 = 10;
                ammo3 = 10;
                x = x + 320;
                bpower = 8;
                onscreen = 16;
                man.x = x-3;
                man.y = 100;
                Doscreen();
            }
        }
    } // ( of While quit )
}

void InitMouse()
{
    if (mousee) 
    {
        HideMouse();
        SetMousePos (100,100);
    }
}

void DoMouse()
{
    ReadMotion(&dx2, &dy2);
    if (dx2 < 20000) dx = dx + dx2;
    else dx = dx - (65535 - dx2);
    if (dy2 < 20000) dy = dy + dy2;
    else dy = dy - (65535 - dy2);
    if (eject == 0) 
    {
        if (landed < 2) 
        {
            yv = (dy-100)/50.0;
            xv = (dx-100)/50.0;
        }
        if (side == 1) 
        {
            if (LeftKeyPressed()) keyhit = ' ';
            if (RightKeyPressed()) keyhit = 'b';
        }
        if (BothKeyPressed()) 
        {
            if (side == 1) keyhit = ' ';
            if (side == 0) keyhit = 'b';
        }
        if (landed == 2)
            if (dy < 50) keyhit = '8';
        if (fall == 0) mfall = 0;
        mfall = mfall + fall;
        yv = yv + mfall;
        if (y == 15)
            if (yv < 0) yv = 0;
    }
    else
    {
        man.xv = (dx-100)/100.0;
        if (man.xv < -3) man.xv = -3;
        if (man.xv > 3) man.xv = 3;
        if (dy < 95) keyhit = '8';
        if (side == 1) 
        {
            if (LeftKeyPressed()) keyhit = '7';
            if (RightKeyPressed()) keyhit = '9';
        }
        if (BothKeyPressed()) 
        {
            if (side == 1) keyhit = '7';
            if (side == 0) keyhit = '9';
        }
    }
}

void Start()
{
    ResetVars();
    TitleScreen(&level,&pilnum,&pilot,&free_);

    InitMouse();
    ResetVars2();

    mis = 0;

    if (!free_)
    {
        Mission(&scene, &mis);
    }
    else
    {
        Practice();
        scene = "day";
        ammo = 150;
        level = '1';

        for (loop = 2; loop <= 10; ++loop) {
            eheli[loop] = true;
        }
    }

    MissionSet();

    if (step == 5) // { start at NorthCape Base }
    {
        onscreen = 15;
        x = 75;
        y = 148;
    }

    if (strcmp(scene, "day") == 0) {
        setgraphmode (2);
        bkcolor = 1;
    }
    else
    {
        setgraphmode (3);
        bkcolor = 0;
    }
    Doscreen();
    gotoxy (29,23);
    writeln_str_i16("AMMO:",ammo);
}

#include "missions.h"

void save_pilots();

void* gamecode(void *arg)
{
    clrscr();

    sounds = 2;
    aeject = true;
    del = 25;
    mousee = false;
    OpenUpfile();
    GraphicStartUp();

    plane = getmem(imagesize (101,100,119,110));
    bplane = getmem(imagesize (101,100,119,110));
    DrawEnemy();
    DrawHeli();
    DrawHeli2();
    DrawMan();

    cleardevice();

    do {
        Start();
        gettime(&time1,&time22,&time3,&time4);
        Main();
        CalcTime();
        nosound();
        for (loop = 1; loop <= MaxHoles; loop++) // (* free memory of screen save *)
            freemem(hole.image[loop],hole.size[loop]);
        delay (2000);
        if (pl.yv == -32) pl.alive = true;
        if (free_ == false) MissionResults (&pilot,onscreen,CalcDif,man.cond,mis,time2,time_,building,deheli,
                cruise.alive,tank.alive,pl.alive,cruise.xv,step,damage,totalejects,level,HitTotal);
        else
            if ((man.cond < 1) && (onscreen == 3) && (x < 50) && (y > 133) && (time2 < 5)) 
                Practice3();
            else
                if ((deheli[1]) && (man.cond > 0)) Practice1();
                else Practice2();
        if (free_ == false)
        {
            Save(&pilot, pilnum);
        }

        keyhit = '@';

    } while (!mainquit);

    return NULL;
}
