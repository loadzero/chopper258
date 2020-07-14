
typedef struct pilottype arraypilot[1+6];

int16_t t_loop;
arraypilot t_pilot;
char t_keyhit;

struct arraypilotfile {
    const char *filename;
};

typedef struct arraypilotfile arraypilotfile;
arraypilotfile infile;

int16_t firstgo; // { this equals zero at first title screen, after "S" is pressed this equals 13.  }

char Upcase(char s)
{
    return toupper(s);
}

void Shutdown();

void EasyWait()
{
   char key;
   outtextxy (100,185,"press any key");

   key = readkey();
   if (key == 27) Shutdown();
}

// Print out the game instructions.
// instruct
void Instruct()
{
    cleardevice();
    settextstyle(2,0,4);
   setcolor (2);
   setbkcolor (0);
   outtextxy (5,10, "   Keys:       (nothing new from 2.0) ");
   outtextxy (5,20, "                 Up - up arrow");
   outtextxy (5,30, " Left - left arrow      Right - right arrow");
   outtextxy (5,40, "               Down - down arrow");
   outtextxy (5,57, "  F1 || \"G\"        -  toggle landing gear ");
   outtextxy (5,67, "  F2 || \"E\"        -  eject/exit craft");
   outtextxy (5,77, "  \"B\" || ins key   -  drop bomb");
   outtextxy (5,87, "  space key        -  launch bomb");
   outtextxy (5,97,"  \"N\"              -  drop mega bomb");
   outtextxy (5,107,"  \"M\"              -  launch missile");
   outtextxy (5,117,"  \"T\"              -  toggle mouse");
   outtextxy (5,127,"  \"P\"              -  pause");
   outtextxy (5,137,"  \"S\"              -  toggle game speed");
   outtextxy (5,147,"  \"X\"              -  toggle sound");
   outtextxy (5,157,"  Esc              -  quit game");
   outtextxy (5,167,"  \"A\"              -  toggle auto-eject");
   outtextxy (5,177,"  home , PgUp keys -  throws grenades (when ejected)");
   EasyWait();
   cleardevice();
   settextstyle (2,0,4);
   setcolor (2);
   setbkcolor (0);
   outtextxy (5,10, " Objectives :");
   outtextxy (5,20, "  - complete missions");
   outtextxy (5,30, "  - destroy the enemy");
   outtextxy (5,40, "  - protect the good guys");
   outtextxy (5,50, "  - stay alive");
   outtextxy (5,70, "  Once ejected wait for a backup helicopter.");
   outtextxy (5,80, " To land : make sure gear is down, lower");
   outtextxy (5,90, " slowly, make contact with flat ground. ");
   outtextxy (5,100,"  Once you complete your mission return");
   outtextxy (5,110," back to base. Try to stay alive to build");
   outtextxy (5,120," up your pilot""s stats.");
   outtextxy (5,138,"  Scoring :");
   outtextxy (5,148,"   - completing missions");
   outtextxy (5,158,"   - destroying the enemy");
   outtextxy (5,168,"   - high difficulty");
   outtextxy (5,178,"   - staying alive");
   EasyWait();
   cleardevice();
   settextstyle (2,0,4);
   setcolor (2);
   setbkcolor (0);
   outtextxy (5,10, "        Tips...");
   outtextxy (5,20, "  This is not an easy game.  Beginners should");
   outtextxy (5,30, "start off on an easy difficultly. Adjust game");
   outtextxy (5,40, "speed to skill, slower the easier.");
   outtextxy (5,50, " Using the mouse makes landings a little harder");
   outtextxy (5,60, "but make air-to-air combat much easier. You");
   outtextxy (5,70, "can always toggle your mouse on/off during the");
   outtextxy (5,80, "game. (press \"T\")");
   outtextxy (5,90,"  The unavoidable bug that temporarily froze");
   outtextxy (5,100,"the game when explosions off screen occurred");
   outtextxy (5,110,"was corrected.");
   outtextxy (5,130," To exit helicopter without ejecting : land");
   outtextxy (5,140,"then press \"E\". To enter the helicopter : walk");
   outtextxy (5,150,"into it.");
   outtextxy (5,170," Caps-Lock should be off.  Good Luck!");
   outtextxy (5,160," Gear must be UP for auto-eject to work.");
   EasyWait();
}

// other - print the game credits
void Other()
{
   cleardevice();
   settextstyle (2,0,4);
   setcolor (2);
   setbkcolor (0);
   outtextxy (5,10, "  Chopper Commando version 2.56 was written by Mark");
   outtextxy (5,20, "Currie in Turbo Pascal 5. I hope you enjoy the game.");
   outtextxy (5,30, "                                                   ");
   outtextxy (5,40, "  Chopper Commando version 2.57 was written by Harvey");
   outtextxy (5,50, "Patterson in Turbo Pascal 5.5 using the source code");
   outtextxy (5,60, "from 2.56, with Mark Currie's permission.          ");
   outtextxy (5,70, "                                                ");
   outtextxy (5,80, "  Chopper Commando version 2.58 was written by Jason");
   outtextxy (5,90, "McSweeney by translating the code from Pascal to C");
   outtextxy (5,100,"and published with Mark Currie's permission.");
   EasyWait();
}

// Shutdown
void Shutdown()
{
    exit(1);
}

// This is a nop in this impl
void reset(arraypilotfile *f) {}

void init_pilot(pilottype *p,
                const char *name,
                int16_t totalmissions,
                int16_t totalscore,
                double dmg,
                const char *status,
                const char *rank)
{
    memset(p, 0, sizeof(*p));

    pstrcpy(p->name, name);
    p->totalmissions = totalmissions;
    p->totalscore = totalscore;
    p->dmg = dmg;
    pstrcpy(p->status, status);
    pstrcpy(p->rank, rank);
}

void save_pilots();
static void p_unescape(char *p_into, const char *from);

void do_load_pilots()
{
    // Simple unix friendly text file format.
    // Space separated. Columns are escaped with percent encoding if need be.

    FILE *f = fopen("pilots.txt", "rb");

    if (!f)
        return;

    char *lptr = 0;
    size_t ln = 0;

    for (int i = 1; i <= 6; ++i)
    {
        ssize_t rn = getline(&lptr, &ln, f);

        if (rn <= 0)
            break;

        lptr[rn-1] = '\0';
        rn--;

        char *saveptr;

        char *e_name = strtok_r(lptr, " ", &saveptr); lptr = NULL;
        if (!e_name) break;
        p_unescape(t_pilot[i].name, e_name);

        char *s_totalmissions = strtok_r(lptr, " ", &saveptr); lptr = NULL;
        if (!s_totalmissions) break;
        t_pilot[i].totalmissions = atoi(s_totalmissions);

        char *s_totalscore = strtok_r(lptr, " ", &saveptr); lptr = NULL;
        if (!s_totalscore) break;
        t_pilot[i].totalscore = atoi(s_totalscore);

        char *s_dmg = strtok_r(lptr, " ", &saveptr); lptr = NULL;
        if (!s_dmg) break;
        t_pilot[i].dmg = atoi(s_dmg);

        char *e_status = strtok_r(lptr, " ", &saveptr); lptr = NULL;
        if (!e_status) break;
        p_unescape(t_pilot[i].status, e_status);

        char *e_rank = strtok_r(lptr, " ", &saveptr); lptr = NULL;
        if (!e_rank) break;
        p_unescape(t_pilot[i].rank, e_rank);
    }

    free(lptr);
    fclose(f);
}

void load_pilots()
{
    struct pilottype *p = t_pilot;
    memset(p, 0, sizeof(arraypilot));

    // Default pilot set
    init_pilot(&p[1], "Mark Currie"     ,8  ,1874  ,90.0  ,"retired"  ,"General");
    init_pilot(&p[2], "Gef"             ,3  ,312   ,24.3  ,"dead"     ,"2nd Liut.");
    init_pilot(&p[3], "The Rookie"      ,1  ,1     ,0.0   ,"alive"    ,"Private");
    init_pilot(&p[4], "Use arrow keys"  ,1  ,0     ,0.0   ,"missing"  ,"Private");
    init_pilot(&p[5], "Mark Currie"     ,8  ,2075  ,235.0 ,"retired"  ,"General");
    init_pilot(&p[6], "F1 = Make new"   ,1  ,97    ,23.1  ,"alive"    ,"Private");

    do_load_pilots();
}

static bool allowed(int c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '.') ||
           (c == '_') ||
           (c == '-');
}

static void p_unescape(char *p_into, const char *str)
{
    int n = strlen(str);

    *p_into++ = 0;
    char *out = p_into;

    for (int i = 0; i < n; ++i)
    {
        if (str[i] == '+')
        {
            *out++ = ' ';
        }
        else if (str[i] == '%')
        {
            if ((n - i) < 3) break;
            i++;

            unsigned c = 0;
            sscanf(str + i, "%02X", &c);

            *out++ = c;
            i += 2;
        }
        else
        {
            *out++ = str[i];
        }
    }

    *out++ = '\0';
}

static const char *p_escape(const char *p_str)
{
    static char buffer[4096];

    const char *str = p_str + 1;
    int n = strlen(str);

    char *out = buffer;

    for (int i = 0; i < n; ++i)
    {
        if (allowed(str[i]))
        {
            *out++ = str[i];
        }
        else if (str[i] == ' ')
        {
            *out++ = '+';
        }
        else
        {
            *out++ = '%';
            out += sprintf(out, "%02X", (uint8_t) (str[i]));
        }
    }

    *out++ = '\0';

    return buffer;
}

void save_pilots()
{
    // unix format.

    FILE *f = fopen("pilots.txt", "wb");

    for (int i = 1; i <= 6; ++i)
    {
        fprintf(f, "%s ", p_escape(t_pilot[i].name));
        fprintf(f, "%d ", t_pilot[i].totalmissions);
        fprintf(f, "%d ", t_pilot[i].totalscore);
        fprintf(f, "%3.2f ", t_pilot[i].dmg);
        fprintf(f, "%s ", p_escape(t_pilot[i].status));
        fprintf(f, "%s\n", p_escape(t_pilot[i].rank));
    }

    fclose(f);
}

void DoNames()
{
    char dummy[1+64+1];
    int16_t clr;
    (void)(clr);
    double x;

    int16_t loop2;

    for (t_loop = 1; t_loop <= 6; ++t_loop)
    {
        settextstyle (2,0,4);
        outtextxy (20,30,"1)");
        outtextxy (20,55,"2)");
        outtextxy (20,80,"3)");
        outtextxy (20,105,"4)");
        outtextxy (20,130,"5)");
        outtextxy (20,155,"6)");
        setcolor (1);

        for (loop2 = 1; loop2 <= 15; ++loop2)
            outtextxyc(25+(loop2*6),5+t_loop*25,t_pilot[t_loop].name[loop2]);

        if (pstreq(t_pilot[t_loop].status, "dead")) outtextxyc (140,5+t_loop*25,'D');
        if (pstreq(t_pilot[t_loop].status, "missing")) outtextxyc (140,5+t_loop*25,'M');
        if (pstreq(t_pilot[t_loop].status, "retired")) outtextxyc (140,5+t_loop*25,'R');
        if (pstreq(t_pilot[t_loop].status, "C")) outtextxyc (140,5+t_loop*25,'C');

        outtextxyp(160, 5+t_loop*25, t_pilot[t_loop].rank);

        int2str(t_pilot[t_loop].totalmissions, dummy);
        outtextxyp(220,5+t_loop*25,dummy);

        if (t_pilot[t_loop].totalmissions == 0)
            x = 0.01;
        else
            x = 0;

        real2str(t_pilot[t_loop].totalscore / (t_pilot[t_loop].totalmissions-x), 4, 2, dummy);

        outtextxyp(255,5+t_loop*25,dummy);
        setfillstyle (1,0);
    }

    bar (14,178,298,192); // { erase old junk }
    outtextxy(20,180,"press ENTER to confirm, F1 to create a new pilot");
}

void Save(const struct pilottype *pi, int16_t pilnum)
{
    t_pilot[pilnum] = *pi;
    save_pilots();
}

void Roster(int16_t *pilnum, struct pilottype *pil)
{
   int16_t counter;
   int16_t clr = 1;

   load_pilots();
   
   setgraphmode (3);
   cleardevice();
   setcolor (3);

   rectangle (8,8,300,195);
   setcolor (2);
   outtextxy (10,10,"ROSTER name,rank,missions,aver.score");

   DoNames();
   *pilnum = 1;

   do {

    t_keyhit = 'w';  // { dummy letter }

   if (keypressed())
   {
       t_keyhit = readkey();
       setcolor(0);
       rectangle (15,*pilnum*25,291,25+*pilnum*25); // { erase old one }
   }

   if (t_keyhit == 72) dec(pilnum);
   else if (t_keyhit == 80) inc(pilnum);
   else if (t_keyhit == 27) Shutdown();
   else if (t_keyhit == 59)           // { enter name }
   {
       struct pilottype *with = &t_pilot[*pilnum];
       {
           setcolor (3);
           setfillstyle (1,0);
           bar (15,*pilnum*25,291,25+*pilnum*25);
           rectangle (15,*pilnum*25,291,25+*pilnum*25);
           pstrcpy(with->name, "                 ");
           pstrcpy(with->rank, "Private");
           with->totalmissions = 0;
           with->totalscore = 0;
           with->dmg = 0;
           pstrcpy(with->status, "alive");
           DoNames();
           bar (14,178,298,192);
           outtextxy(20,180,"Type name of pilot, then enter.(max 15 letters)");
           counter = 1;
           do {
               t_keyhit = readkey();
               if ((t_keyhit == 83) || (t_keyhit == 75) || (t_keyhit == 27) || (t_keyhit == 8)) {
                   if (counter > 1)
                   {
                       setcolor (0);
                       counter = counter - 1;
                       outtextxyc(25+counter*6, *pilnum * 25 + 5, with->name[counter]);
                       with->name[counter] = ' ';
                       setcolor(1);
                   }
               }
               if (!((t_keyhit == 83) || (t_keyhit == 75) || (t_keyhit == 27) || (t_keyhit == 8))) 
               {
                   with->name[counter] = t_keyhit;
                   outtextxyc(25+counter*6,*pilnum*25+5,t_keyhit);
                   inc(&counter);
               }
           }
           while (!(t_keyhit == 13) || (counter == 15));
           t_keyhit = 'w';
       }; // endwith

      setfillstyle (1,0);
      bar (14,178,298,192); // { erase old junk }
      outtextxy (20,180,"press ENTER to confirm, F1 to create a new pilot");
   }  // { enter name }
   else if (in(t_keyhit, '1', '6')) *pilnum = t_keyhit - '0';
    if (*pilnum < 1) *pilnum = 1;
    if (*pilnum > 6) *pilnum = 6;

    if (clr == 1)
        clr = 2;
    else
        clr = 1;

    setcolor (clr);
    rectangle (15,*pilnum*25,291,25+*pilnum*25);
    if (t_keyhit == 13)
    {
      bar (14,178,298,192);
      if (pstreq(t_pilot[*pilnum].status,"dead")) outtextxy (20,180," Sorry he's dead. Select a living pilot");
      if (pstreq(t_pilot[*pilnum].status,"missing")) outtextxy (20,180," Sorry he's missing in action. Select another");
      if (pstreq(t_pilot[*pilnum].status,"retired")) outtextxy (20,180," Sorry he's retired. Select a different pilot");
      if (pstreq(t_pilot[*pilnum].status, "C")) outtextxy (20,180," Sorry he's contaminated. Select another.");
    }

   } while (!((t_keyhit == 13) && (pstreq(t_pilot[*pilnum].status, "alive")))); // { #13 is enter }

   setgraphmode (1);
   t_pilot[*pilnum].totalmissions = t_pilot[*pilnum].totalmissions + 1;
   *pil = t_pilot[*pilnum];
   pstrcpy(pil->status, "missing");
 
   Save (pil,*pilnum);

   pstrcpy(pil->status, "alive");
}

void SelectDifficulty(char *level)
{

    do {
        setgraphmode (2);
        cleardevice();
        setcolor (2);
        settextstyle(2,0,6);
        setbkcolor (8);
        gotoxy(1,8);
        outtextxy (50,70," Select Game Difficulty:");
        outtextxy (50,85,"  1) WIMP (150 bombs)");
        outtextxy (50,100,"  2) AVERAGE (75 bombs)");
        outtextxy (50,115,"  3) COMMANDO (50 bombs)");
        *level = readkey();
        if (*level == 27) Shutdown();
    } while (!(in(*level,'1','3')));

}

void TitleScreen(char *level, int16_t *pilnum, struct pilottype *pil, bool *isfree)
{
    if (firstgo != 13) {
        do {
            firstgo = 13;
            cleardevice();
            setgraphmode(2);
            setbkcolor(8);
            setcolor(3);

            settextstyle (2,0,5);
            outtextxy (110,73,"v 2.57");
            outtextxy (100,86,"by Mark Currie");
            outtextxy (90,108,"S)tart");
            outtextxy (90,123,"I)nstructions");
            outtextxy (90,139,"O)ther Info");
            outtextxy (90,153,"P)ractice Mission");
            outtextxy (90,167,"Q)uit");
            
            settextstyle (2,0,4);

            if (MousePresent()) {
                outtextxy(90,190,"Mouse Detected.");
                HideMouse();
            }
            else
                outtextxy(90,190,"No Mouse Detected.");

            setcolor (2);
            settextstyle (2,0,10);
            outtextxy (75,5,"CHOPPER");
            outtextxy (67,40,"COMMANDO"); delay (100);
            outtextxy (76,5,"CHOPPER");
            setcolor (3);
            outtextxy (68,40,"COMMANDO"); delay (100);
            outtextxy (77,5,"CHOPPER");
            setcolor (2);
            outtextxy (69,40,"COMMANDO"); delay (100);
            outtextxy (78,5,"CHOPPER");
            outtextxy (70,40,"COMMANDO"); delay (100);

            t_keyhit = readkey();
            *isfree = false;

            switch (t_keyhit) {

                case 'i': case 'I': Instruct(); break;
                case 'o': case 'O': Other(); break;
                case 's': case 'S': break; // { Start() } 
                case 'q': case 'Q': case 27: Shutdown(); break;
                case 'p': case 'P': *isfree = true; break;
                default: t_keyhit = '~';
            }

        } while (! ((Upcase(t_keyhit) == 'S') || (Upcase(t_keyhit) == 'P')));
    }

    if (*isfree == false)
    {
        Roster(pilnum, pil);
        SelectDifficulty(level);
    }
}

void Mission(const char** scene, int16_t *mis)
{
    char dif;
    int16_t myrand;

    myrand = random_int(5);
    setbkcolor(8);
    cleardevice();
    setcolor (2);
    settextstyle(2,0,6);
    outtextxy (50,55,"SELECT MISSION:");
    outtextxy (60,70," 1) easy");
    outtextxy (60,85," 2) regular");
    outtextxy (60,100," 3) hard");
    outtextxy (60,115," 4) very hard");
    *scene = "day";

    do {
        dif = readkey();
    } while (!in(dif, '1', '4'));

    MissionSetUp();  // { draw box on screen }

    if (dif == '1') {

        switch (myrand) {
            case 0: *mis = 3; break;
            case 1: *mis = 12; break;
            case 2: *mis = 9; break;
            case 3: *mis = 10; break;
            case 4: *mis = 7; break;
        }
    }
    else if (dif == '2') {
        switch (myrand) {
            case 0: *mis = 1; break;
            case 1: *mis = 4; break;
            case 2: *mis = 5; break;
            case 3: *mis = 13; break;
            case 4: *mis = 14; break;
        }
    }
    else if (dif == '3') {
        switch (myrand) {
            case 0: *mis = 2; break;
            case 1: *mis = 6; break;
            case 2: *mis = 8; break;
            case 3: *mis = 11; break;
            case 4: *mis = 15; break;
        }
    }
    else if (dif == '4') {
        switch (myrand) {
            case 0: *mis = 16; break;
            case 1: *mis = 17; break;
            case 2: *mis = 18; break;
            case 3: *mis = 19; break;
            case 4: *mis = 20; break;
        }
    }

    switch (*mis) {
        case 1: Mission1(); break;
        case 2: Mission2(); break;
        case 3: Mission3(); break;
        case 4: Mission4(); break;
        case 5: Mission5(); break;
        case 6: Mission6(); break;
        case 7: Mission7(); break;
        case 8: Mission8(); break;
        case 9: Mission9(); break;
        case 10: Mission10(); break;
        case 11: Mission11(); break;
        case 12: Mission12(); break;
        case 13: Mission13(); break;
        case 14: Mission14(); break;
        case 15: Mission15(); break;
        case 16: Mission16(); break;
        case 17: Mission17(); break;
        case 18: Mission18(); break;
        case 19: Mission19(); break;
        case 20: Mission20(); break;
    }

    Wait(115,135);
    setgraphmode(0);
}

// Nop on this impl
void OpenUpfile() {}

