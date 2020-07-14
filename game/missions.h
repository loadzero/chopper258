
void ExtraPoints (int16_t *score,
        double *damage,
        bool tank,
        Buildingtype building,
        Deadhelitype deheli)
{
    int16_t loop;

    for (loop = 1; loop <= NumofBuildings; ++loop)
    {
        if (building[loop] == false)
        {
            switch (loop) {
                case 1 : {
                             dec_by(score,25);
                             break;
                         }
                case 2 : {
                             inc_by(score,5); // ( tower )
                             *damage = *damage + 1;
                             break;
                         }
                case 3 : {
                             inc_by(score,7);   // ( e i building )
                             *damage = *damage + 2.2;
                             break;
                         }
                case 4 : {
                             inc_by(score,9); // ( radar )
                             *damage = *damage + 2;
                             break;
                         }
                case 5 : {
                             inc_by(score,7);
                             *damage = *damage + 2.5;
                             break;
                         }
                case 6 : {
                             inc_by(score,13); // ( radar building )
                             *damage = *damage + 6;
                             break;
                         }
                case 7 : {
                             inc_by(score,10);  // ( ICBM building )
                             *damage = *damage + 7;
                             break;
                         }
                case 8 : {
                             inc_by(score,7);
                             *damage = *damage + 2;
                             break;
                         }
                case 9 : { // ( tower )
                             inc_by(score,5);
                             *damage = *damage + 1;
                             break;
                         }
                case 10 : dec_by(score,25); break; // ( base )
                case 11 : dec_by(score,15); break; // ( good gun )
                case 12 : {    // ( gun )
                              inc_by(score,5);
                              *damage = *damage + 0.5;
                          }

                          break;
            } // ( of case )
        }
    }

    for (loop = 1; loop <= 10; ++loop) 
    {
        if (deheli[loop]) 
        {
            inc_by(score,5);
            *damage = *damage + 1.9;
        }
    }

    if (tank == false) 
    {
        *damage = *damage + 2;
        inc_by(score,3);
    }

}

void General (int16_t hate, int16_t mis, stattype status)
{
    int16_t time_;
    cleardevice();
    setcolor(0);
    setfillstyle(1,3);
    bar (50,20,285,114);
    settextstyle(2,0,5);
    outtextxy (60,20,"IN THE OFFICE :");
    setcolor(2);
    settextstyle (2,0,4);
    randomize();

    time_ = random_int(65);

    switch (hate) {
        case -3 : outtextxy (65,37,"  . . ."); break;
        case -2 : {  // ( base blown up )
                      outtextxy (65,35," Back in Washington they are");
                      outtextxy (65,43,"investigating the incident.");
                      delay (1500);
                      outtextxy (65,51,"The conclusion has been reached:");

                      if (mis == 1 || mis == 3)
                      {
                          outtextxy (65,59," They believe the enemy heli-");
                          outtextxy (65,67," copter destroyed the base. Your");
                          outtextxy (65,75," name is cleared.");
                      }
                      if (mis >= 4 || mis <= 13)
                      {
                          outtextxy (65,59," They believe that you intention-");
                          outtextxy (65,67," ally destroyed the base and killed");
                          outtextxy (65,75," 32 people. You will be put in the");
                          outtextxy (65,83," chair.  Good bye.");
                          pstrcpy(status, "dead");
                      }
                      if (mis == 2) 
                      {
                          outtextxy (65,59," They believe the cruise missile");
                          outtextxy (65,67," destroyed the base. Your name is");
                          outtextxy (65,75," cleared. Washington is disapointed");
                          outtextxy (65,83," in your performance.");
                      }
                      if (mis > 13) 
                      {
                          outtextxy (65,59," They believe the enemy plane");
                          outtextxy (65,67," bombed the base. Your name is");
                          outtextxy (65,75," cleared. Washington is disapointed");
                          outtextxy (65,83," in your performance.");
                      }

                  }  // ( base blown up )

                  break;
        case -1 : {  // ( dead )
                      if ((time_ < 25) && (time_ >= 0 )) 
                      {
                          outtextxy (65,35,"\"Ah darn, we lost another one.\"");
                          outtextxy (65,43,"\"It's okay, General. Life goes");
                          outtextxy (65,51,"on. For most of us anyways.\"");
                      }
                      if ((time_ < 30) && (time_ >= 25 )) 
                      {
                          outtextxy (65,35,"\"I am sorry to say that your");
                          outtextxy (65,43,"husband has been killed in action.\"");
                          outtextxy (65,51,"\"It's okay, General. He wasn't that");
                          outtextxy (65,59,"good of a husband either. Are you");
                          outtextxy (65,67,"doing anything tonight?\"");
                      }
                      if ((time_ >= 30) && (time_ < 35)) 
                      {
                          outtextxy (65,35,"\"I can't believe that he crashed!\"");
                          outtextxy (65,43,"\"You can't predict things like this");
                          outtextxy (65,51,"General.\"");
                          outtextxy (65,59,"\"I know, but that helicopter cost");
                          outtextxy (65,67,"4 million dollars!\"");
                      }
                      if ((time_ >= 35) && (time_ < 40)) 
                      {
                          outtextxy (65,35,"\"I can't believe that he crashed!\"");
                          outtextxy (65,43,"\"Well General you remember our bet.\"");
                          outtextxy (65,51,"\"I know, here's your 5 bucks.\"");
                      }
                      if ((time_ >= 40) && (time_ < 55)) 
                      {
                          outtextxy (65,35,"\"Sure gonna miss old top gun.\"");
                          outtextxy (65,43,"\"Yeah, you can say that again.\"");
                      }
                      if ((time_ >= 55) && (time_ < 60)) 
                      {
                          outtextxy (65,35,"\"I kind of liked that guy.\"");
                          outtextxy (65,43,"\"Yeh.\"");
                      }
                      if ((time_ >= 60) && (time_ < 65)) 
                      {
                          outtextxy (65,35,"\"Man, that guy owed my $10.\"");
                          outtextxy (65,43,"\"Guess you'll never see");
                          outtextxy (65,51,"that again.\"");
                      }
                      break;
                  }
        case 0 : {  // (* mission complete *)
                     if ((time_ >= 0) && (time_ < 9)) 
                     {
                         outtextxy (65,35,"\"Good work, son!\"");
                         outtextxy (65,43,"\"Why thank you, General.\"");
                         outtextxy (65,51,"\"Tell me son, do you like Cuban");
                         outtextxy (65,59,"cigars?\"");
                     }
                     if ((time_ >= 9) && (time_ < 18)) 
                     {
                         outtextxy (65,35,"\"Good work son!\"");
                         outtextxy (65,43,"\"Thank you, General.\"");
                         outtextxy (65,51,"\"You know, I used to be a pilot");
                         outtextxy (65,59,"like you. Back in 1943 I was...\"");
                     }
                     if ((time_ >= 18) && (time_ < 27)) 
                     {
                         outtextxy (65,35,"\"Good work, pilot!\"");
                         outtextxy (65,43,"\"Thank you, General.\"");
                         outtextxy (65,51,"\"I actually didn't think you");
                         outtextxy (65,59,"could pull it off.\"");
                     }
                     if ((time_ >= 27) && (time_ < 40)) 
                     {
                         outtextxy (65,35,"\"Good work son.");
                         outtextxy (65,43,"You're a regular Top Gun.\"");
                         outtextxy (65,51,"\"Thank you, General.\"");
                     }
                     if ((time_ >= 40) && (time_ < 48)) 
                     {
                         outtextxy (65,35,"\"Well, well, well.");
                         outtextxy (65,43,"You are a great pilot!\"");
                         outtextxy (65,51,"\"Thank you, General.\"");
                     }
                     if ((time_ >= 48) && (time_ < 56)) 
                     {
                         outtextxy (65,35,"\"I never had a doubt that");
                         outtextxy (65,43,"he would come through.\"");
                         outtextxy (65,51,"\"I know, General.\"");
                     }
                     if ((time_ >= 56) && (time_ < 65)) 
                     {
                         outtextxy (65,35,"\"Well you sure lucked");
                         outtextxy (65,43,"out this time_.\"");
                         outtextxy (65,51,"\"No General, I'm good.\"");
                     }
                     break;
                 }
        case 1 : {
                     if (time_ < 30) 
                     {
                         outtextxy (65,35,"\"I expected more from you. Next");
                         outtextxy (65,43,"time DO BETTER!\"");
                         outtextxy (65,51,"\"Sorry, sir.\"");
                     }
                     if (time_ > 29) 
                     {
                         outtextxy (65,35,"\"We both know you can");
                         outtextxy (65,43,"do better than that.\"");
                         outtextxy (65,51,"\"Sorry, sir.\"");
                     }
                 }
                 break;
        case 2 : {
                     outtextxy (65,35,"\"I CAN'T BELIEVE WHAT YOU'VE");
                     outtextxy (65,43,"DONE!! You call yourself an Air");
                     outtextxy (65,51,"Force pilot? Get out of my office!\"");
                 }
                 break;
    } // ( of case )
    delay (1000);
    Wait (115,101);
} // ( of general )

void CheckRank(pilottype *pilot)
{
    double aver;
    int16_t miss;
    bool new_;

    new_ = false;
    aver = pilot->totalscore / (double) pilot->totalmissions;
    miss = pilot->totalmissions;

    // with pilot do
    {
        if (pstrcmp(pilot->rank, "Private") == 0) 
        {
            if ((aver > 100) && (miss > 1))
                pstrcpy(pilot->rank, "2nd Liut.");
            if (miss > 3) 
            {
                if (aver > 55) pstrcpy(pilot->rank, "2nd Liut.");
                if ((miss > 6) && (aver > 30)) pstrcpy(pilot->rank, "2nd Liut.");
            }
            if (pstrcmp(pilot->rank, "2nd Liut.") == 0) new_ = true;
        }
        if (pstrcmp(pilot->rank, "2nd Liut.") == 0) 
        {
            if ((aver > 120) && (miss > 2)) pstrcpy(pilot->rank, "1st Liut.");
            if (miss > 4) 
            {
                if (aver > 70) pstrcpy(pilot->rank, "1st Liut.");
                if ((miss > 9) && (aver > 50)) pstrcpy(pilot->rank, "1st Liut.");
            }
            if (pstrcmp(pilot->rank, "1st Liut.") == 0) new_ = true;
        }
        if (pstrcmp(pilot->rank, "1st Liut.") == 0) 
        {
            if ((aver > 130) && (miss > 3)) pstrcpy(pilot->rank, "Captain");
            if (miss > 6) 
                if (aver > 80) pstrcpy(pilot->rank, "Captain");
            if (miss > 10) 
                if (aver > 60) pstrcpy(pilot->rank, "Captain");
            if (pstrcmp(pilot->rank, "Captain") == 0) new_ = true;
        }
        if (pstrcmp(pilot->rank, "Captain") == 0) 
        {
            if ((aver > 140) && (miss > 4)) pstrcpy(pilot->rank, "Major");
            if (miss > 7) 
                if (aver > 95) pstrcpy(pilot->rank, "Major");
            if (miss > 11) 
                if (aver > 65) pstrcpy(pilot->rank, "Major");
            if (pstrcmp(pilot->rank, "Major") == 0) new_ = true;
        }
        if (pstrcmp(pilot->rank, "Major") == 0) 
        {
            if ((aver > 150) && (miss > 5)) pstrcpy(pilot->rank, "Colonel");
            if (miss > 8) 
                if (aver > 105) pstrcpy(pilot->rank, "Colonel");
            if (miss > 12) 
                if (aver > 75) pstrcpy(pilot->rank, "Colonel");
            if (pstrcmp(pilot->rank, "Colonel") == 0) new_ = true;
        }
        if (pstrcmp(pilot->rank, "Colonel") == 0) 
        {
            if ((aver > 150) && (miss > 6)) pstrcpy(pilot->rank, "Commando");
            if (miss > 9) 
                if (aver > 110) pstrcpy(pilot->rank, "Commando");
            if (miss > 13) 
                if (aver > 80) pstrcpy(pilot->rank, "Commando");
            if (pstrcmp(pilot->rank, "Commando") == 0) new_ = true;
        }
        if (pstrcmp(pilot->rank, "Commando") == 0) 
        {
            if ((aver > 250) && (miss > 7)) pstrcpy(pilot->rank, "General");
            if (miss > 10) 
                if (aver > 180) pstrcpy(pilot->rank, "General");
            if (miss > 15) 
                if (aver > 100) pstrcpy(pilot->rank, "General");
            if (pstrcmp(pilot->rank, "General") == 0) 
            {
                new_ = true;
                if (pstrcmp(pilot->status, "alive") == 0)
                        pstrcpy(pilot->status, "retired");
            }
        }
    } // { of with }
    if (new_) 
    {
        cleardevice();
        setcolor(0);
        setfillstyle(1,3);
        bar (50,20,300,100);
        settextstyle(2,0,5);
        outtextxy (60,20,"CONGRATULATIONS :");
        setcolor(2);
        settextstyle (2,0,4);
        outtextxy (65,35," You have been promoted to the rank of:");
        if ((aver > 140) && (miss < 4)) 
        {
                char buf[256]; snprintf(buf, 256, "%s%s", pilot->rank + 1, ", for high mission scores.");
                outtextxy (66,44,buf);
        }
        else
            if ((miss > 5) && (aver < 100))
            {
                char buf[256]; snprintf(buf, 256, "%s%s", pilot->rank + 1, ", for completing many missions.");
                outtextxy (66,44,buf);
            }
        else
        {
            char buf[256]; snprintf(buf, 256, "%s%s", pilot->rank + 1, ", for overall performance.");
            outtextxy (70,44,buf);
        }

        if ((pstrcmp(pilot->status, "alive") == 0) && (pstrcmp(pilot->rank, "General") != 0)) outtextxy (65,53,"  Keep up the good work!");
        if (pstrcmp(pilot->rank, "General") == 0)
            outtextxy (65,62,"PS,go past the mountains! (east of NorthCape)");
        Wait (115,80);
    }
}

void StatsScreen(pilottype *pilot,
                 int16_t score, int16_t hittotal,
                 double damage,
                 double time_)
{
    char ftime[256];
    char damge[256];
    char x[256];
    char dummy[256];

    setgraphmode (3);
    setbkcolor (0);
    settextstyle (2,0,6);
    setcolor (2);
    outtextxy (10,10,"Mission Stats");
    setcolor (3);
    rectangle (5,5,280,195);
    settextstyle (2,0,4);
    outtextxy (30,25,"mission score ");
    int2str (score,dummy);
    outtextxyp(200,25,dummy);
    outtextxy (30,35,"time in flight (sec.)");
    real2str (time_,4,2,ftime);
    outtextxyp(200,35,ftime);
    outtextxy (30,45,"damage on enemy (mil.)");

    sprintf(damge, " $%4.1f", damage);
    outtextxyp(200,45,damge);
    outtextxy (30,55,"status ");
    outtextxyp(200,55,pilot->status);
    outtextxy (30,65,"hits taken");
    int2str(hittotal,dummy);
    outtextxyp(200,65,dummy);
    setcolor (2);
    settextstyle (2,0,6);

    // with pilot do
    {
        pilot->dmg = pilot->dmg + damage;
        outtextxy (10,100,"Life Time Stats");
        setcolor (3);
        settextstyle (2,0,4);
        outtextxy(30,115,"total missions");
        int2str (pilot->totalmissions,x);
        outtextxyp(200,115,x);
        outtextxy (30,125,"total score ");
        int2str(pilot->totalscore,x);
        outtextxyp(200,125,x);
        outtextxy (30,135,"average score ");
        real2str((double) pilot->totalscore / pilot->totalmissions, 4, 2, x);
        outtextxyp(200,135,x);
        outtextxy (30,145,"rank ");
        outtextxyp(200,145,pilot->rank);
        outtextxy (30,155,"total damage in (mil.)");

        sprintf(damge, " $%4.1f", damage);
        outtextxyp(200,155,damge);
    }

    Wait (95,180);
}

void MissionResults(struct pilottype *pilot,
                     int16_t onscreen,
                     double timedif,
                     int16_t cond,
                     int16_t mis,
                     int16_t time2,
                     int16_t time_,
                     Buildingtype building,
                     Deadhelitype deheli,
                     bool cruisealive,
                     bool tank,
                     bool pl,
                     int16_t cruisexv,
                     int16_t step,
                     double damage,
                     int16_t totalejects,
                     char level,
                     int16_t hittotal)
{
    int16_t hate; // ( 0 to 3 (how mad the general is) )
    int16_t score;
    char dummy[64];
    int16_t loop;
    int16_t counter;

    setgraphmode(1);
    cleardevice();
    setcolor(0);
    setfillstyle(1,3);
    bar (50,20,285,180);
    settextstyle(2,0,5);
    outtextxy (60,20,"MISSION  RESULTS :");
    setcolor(2);
    settextstyle (2,0,4);
    if (((building[1]) && (onscreen < 9)) || ((building[10]) && (onscreen > 9))) 
        switch (mis) {
            case 1 : {
                         if ((cond > 0) && (timedif <= 60) && (deheli[1])) 
                         {
                             outtextxy (65,35," You destroyed the \"P-77\" helicopter");
                             outtextxy (65,43,"in less than 1 minute. Good job!");
                             outtextxy (65,51,"The General would like to congratulate");
                             outtextxy (65,59,"you.");
                             score = 75;
                             hate = 0;
                         }
                         if ((cond > 0) && (timedif > 60) && (deheli[1])) 
                         {
                             outtextxy (65,35," You destroyed the \"P-77\" helicopter.");
                             outtextxy (65,43,"But not in 1 minute! Our forces were");
                             outtextxy (65,51,"massively delayed! The General wants ");
                             outtextxy (65,59,"to see you in his office!");
                             hate = 1;
                             score = 45;
                         }
                         if ((deheli[1] == false) && (cond > 0)) 
                         {
                             outtextxy (65,35,"   The \"P-77\" helicopter was not");
                             outtextxy (65,43,"destroyed! Next time do your mission");
                             outtextxy (65,51,"before returning home! The General");
                             outtextxy (65,59,"wants to see you in his office!");
                             hate = 2;
                             score = 0;
                         }
                         if ((deheli[1] == false) && (cond < 1)) 
                         {
                             outtextxy (65,35,"   The \"P-77\" helicopter was not");
                             outtextxy (65,43,"destroyed and you were tragically");
                             outtextxy (65,51,"kill in action. You will be");
                             outtextxy (65,69,"remembered.");
                             hate = -1;
                             score = onscreen;
                         }
                         if ((deheli[1]) && (cond < 1)) 
                         {
                             outtextxy (65,35," The \"P-77\" helicopter was destroyed");
                             outtextxy (65,43,"but you were tragically killed in");
                             outtextxy (65,51,"action. You will be remembered.");
                             hate = -1;
                             score = 40;
                         }
                     }  // ( mis 1 )
                     break;
            case 2 : {
                         if ((building[1] == false) || (cond < 1) || (cruisealive) || (cruisexv == -32)) 
                         {
                             outtextxy (65,35," You failed in completing your");
                             outtextxy (65,43,"mission.");
                             if ((cruisealive)) 
                                 outtextxy (65,51,"The missile wasn't destroyed.");
                             else if ((cond < 1)) outtextxy (65,51, "You & the missile were destroyed.");
                             hate = 2;
                             score = 2;
                             if (building[1] == false) hate = -2;
                             if (cond < 1) hate = -3;
                         }
                         else
                         {
                             outtextxy (65,35," You destroyed the cruise missile");
                             outtextxy (65,43,"and returned safely. Good Job.");
                             hate = 0;
                             score = 100;
                         }
                     }  // ( mis 2 )
                     break;
            case 3 : {
                         if ((cond > 0) && (deheli[1])) 
                         {
                             outtextxy (65,35," You destroyed the \"P-79\" helicopter.");
                             outtextxy (65,43,"Good job!  You are ready for more.");
                             outtextxy (65,51,"The General would like to congratulate");
                             outtextxy (65,59,"you.");
                             score = 50;
                             hate = 0;
                         }
                         if ((deheli[1] == false) && (cond > 0)) 
                         {
                             outtextxy (65,35,"   The \"P-79\" helicopter was not");
                             outtextxy (65,43,"destroyed! Next time do your mission");
                             outtextxy (65,51,"before returning home! The General");
                             outtextxy (65,59,"wants to see you in his office!");
                             hate = 2;
                             score = 1;
                         }
                         if ((deheli[1] == false) && (cond < 1)) 
                         {
                             outtextxy (65,35,"   The \"P-79\" helicopter was not");
                             outtextxy (65,43,"destroyed and you were tragically");
                             outtextxy (65,51,"killed in action. You will be");
                             outtextxy (65,59,"remembered.");
                             hate = -1;
                             score = 2;
                         }
                         if ((deheli[1]) && (cond < 1)) 
                         {
                             outtextxy (65,35," The \"P-79\" helicopter was destroyed");
                             outtextxy (65,43,"but you were tragically killed in");
                             outtextxy (65,51,"action. You will be remembered.");
                             hate = -1;
                             score = 35;
                         }
                     }  // ( mis 3 )
                     break;
            case 4 : {
                         if ((cond > 0) && (building[3] == false)) 
                         {
                             outtextxy (65,35," You destroyed the E. I. building.");
                             outtextxy (65,43,"Kuremee has been killed. Good job.");
                             outtextxy (65,51,"The General would like to congratulate");
                             outtextxy (65,59,"you.");
                             score = 60;
                             hate = 0;
                         }
                         if ((building[3]) && (cond > 0)) 
                         {
                             outtextxy (65,35,"   The Kuremee was not killed!");
                             outtextxy (65,43,"Next time do your mission before");
                             outtextxy (65,51,"returning home! The General wants");
                             outtextxy (65,59,"to see you in his office! Now!");
                             hate = 2;
                             score = 1;
                         }
                         if ((building[3] == false) && (cond < 1)) 
                         {
                             outtextxy (65,35,"   The E. I. Building was destroyed.");
                             outtextxy (65,43,"Kuremee was creamed, but so were you.");
                             outtextxy (65,51,"You will be remembered.");
                             hate = -1;
                             score = 45;
                         }
                         if ((building[3]) && (cond < 1)) 
                         {
                             outtextxy (65,35,"The E.I. Building was not destroyed,");
                             outtextxy (65,43,"and you were tragically killed in");
                             outtextxy (65,51,"action. You will be remembered.");
                             hate = -1;
                             score = 2;
                         }
                     }  //  ( mis 4 )
                     break;
            case 5 : {
                         if ((cond > 0) && (building[4] == false)) 
                         {
                             outtextxy (65,35," You destroyed the radar. It will");
                             outtextxy (65,43,"take at least 7 days for them to");
                             outtextxy (65,51,"replace it. Now we can initiate");
                             outtextxy (65,59,"our surprise attack. Good work!");
                             score = 70;
                             hate = 0;
                         }
                         if ((building[4]) && (cond > 0)) 
                         {
                             outtextxy (65,35,"   The radar wasn't destroyed!");
                             outtextxy (65,43,"Always do your mission before");
                             outtextxy (65,51,"returning home. I guess we'll have");
                             outtextxy (65,59,"to get a real pilot to do the job.");
                             outtextxy (65,67,"The General would like to say some-");
                             outtextxy (65,75,"thing to you. He's in his office.");
                             hate = 2;
                             score = 1;
                         }
                         if ((building[4] == false) && (cond < 1)) 
                         {
                             outtextxy (65,35,"   The radar was destroyed but you");
                             outtextxy (65,43,"were tragically killed. You will be");
                             outtextxy (65,51,"remembered.");
                             hate = -1;
                             score = 45;
                         }
                         if ((building[4]) && (cond < 1)) 
                         {
                             outtextxy (65,35," The radar was not destroyed,");
                             outtextxy (65,43,"and you were tragically killed in");
                             outtextxy (65,51,"action. You will be remembered.");
                             hate = -1;
                             score = 2;
                         }
                     }  //  ( mis 5 )
                     break;
            case 6 : {
                         if ((cond > 0) && (step == 2)) 
                         {
                             outtextxy (65,35," You transferred the top secret");
                             outtextxy (65,43,"device within two minutes and");
                             outtextxy (65,51,"returned safely. Good work!");
                             score = 105;
                             hate = 0;
                         }
                         if ((cond > 0) && (step == 1)) 
                         {
                             outtextxy (65,35," You transferred the top secret");
                             outtextxy (65,43,"device. But not in two minutes!");
                             outtextxy (65,51,"The timing was critical, and");
                             outtextxy (65,59,"you messed up!");
                             score = 65;
                             hate = 1;
                         }
                         if ((step == 0) && (cond > 0)) 
                         {
                             outtextxy (65,35,"   The device wasn't transferred!");
                             outtextxy (65,43,"Next time do your mission before");
                             outtextxy (65,51,"returning home. I guess we'll have");
                             outtextxy (65,59,"to get a real pilot to do the job.");
                             outtextxy (65,67,"The General wants you in his");
                             outtextxy (65,75,"office, pronto!");
                             hate = 2;
                             score = 0;
                         }
                         if ((step > 0) && (cond < 1)) 
                         {
                             outtextxy (65,35,"   The device was sent, but you");
                             outtextxy (65,43,"were tragically killed. You will");
                             outtextxy (65,51,"be remembered for your valor.");
                             hate = -1;
                             score = 55;
                         }
                         if ((step == 0) && (cond < 1)) 
                         {
                             outtextxy (65,35," You were killed before getting");
                             outtextxy (65,43,"a chance to transfer the secret");
                             outtextxy (65,51,"device. You will be remembered.");
                             hate = -1;
                             score = 2;
                         }
                     }  //  ( mis 6 )
                     break;
            case 7 : {
                         if ((cond > 0) && (onscreen == 15)) 
                         {
                             outtextxy (65,35,"You transferred the top secret");
                             outtextxy (65,43,"papers successfully. Good job.");
                             score = 65;
                             hate = 0;
                         }
                         if ((onscreen == 3) && (cond > 0)) 
                         {
                             outtextxy (65,35,"  The papers weren't transferred!");
                             outtextxy (65,43,"Next time do your mission before");
                             outtextxy (65,51,"returning home. I guess we'll have");
                             outtextxy (65,59,"to get a REAL pilot to do the job.");
                             outtextxy (65,67,"The General wants you in his");
                             outtextxy (65,75,"office right now!");
                             hate = 2;
                             score = 0;
                         }
                         if ((cond < 1)) 
                         {
                             outtextxy (65,35,"   The papers weren't sent and you");
                             outtextxy (65,43,"were tragically killed. You will be");
                             outtextxy (65,51,"remembered.");
                             hate = -1;
                             score = 1;
                         }
                     }  //  ( mis 7 )
                     break;
            case 8 : {
                         if ((cond > 0) && (step == 1) && (onscreen == 15)) 
                         {
                             outtextxy (65,35," You picked up the top secret");
                             outtextxy (65,43,"capsule and returned it to");
                             outtextxy (65,51,"NorthCape Base. The General");
                             outtextxy (65,59,"is very pleased.");
                             if ((pstreq(pilot->rank, "Private")) || (pstreq(pilot->rank,"2nd Liut.")) || (pstreq(pilot->rank,  "1st Liut."))) 
                                 outtextxy (65,67,"This could mean a promotion.");
                             score = 130;
                             hate = 0;
                         }
                         if ((cond > 0) && (step == 1) && (onscreen == 3)) 
                         {
                             outtextxy (65,35," You picked up the top secret");
                             outtextxy (65,43,"capsule but, you returned it");
                             outtextxy (65,51,"to the wrong base! You were");
                             outtextxy (65,59,"supposed to return it to");
                             outtextxy (65,67,"NorthCape Base.");
                             score = 133;
                             hate = 1;
                         }
                         if ((step == 0) && (cond > 0)) 
                         {
                             outtextxy (65,35,"   The capsule wasn't retrieved!");
                             outtextxy (65,43,"Next time do your mission before");
                             outtextxy (65,51,"returning home. I guess will have");
                             outtextxy (65,59,"to get some other pilot to do the");
                             outtextxy (65,67,"job. The General wants you in his");
                             outtextxy (65,75,"office.");
                             hate = 2;
                             score = 0;
                         }
                         if ((step == 1) && (cond < 1)) 
                         {
                             outtextxy (65,35," The capsule was retrieved but you");
                             outtextxy (65,43,"were tragically killed. You will");
                             outtextxy (65,51,"be remembered for your courage.");
                             hate = -1;
                             score = 75;
                         }
                         if ((step == 0) && (cond < 1)) 
                         {
                             outtextxy (65,35," You were killed before getting");
                             outtextxy (65,43,"a chance to retrieve the secret");
                             outtextxy (65,51,"capsule. You will be remebered.");
                             hate = -1;
                             score = 3;
                         }
                     }  //  ( mis 8 )
                     break;
            case 9 : {
                         if ((cond > 0) && (building[9] == false)) 
                         {
                             outtextxy (65,35," You destroyed the tower.");
                             outtextxy (65,43,"Now we can initiate our");
                             outtextxy (65,51,"attack on the enemy terr-");
                             outtextxy (65,59,"itory. Good job.");
                             score = 35;
                             hate = 0;
                         }
                         if ((cond > 0) && (building[9])) 
                             {
                              outtextxy (65,35," You didn't do your mission!");
                              outtextxy (65,43,"You were supposed to destroy");
                              outtextxy (65,51,"the tower!  Now we can't");
                              outtextxy (65,59,"attack the enemy territory");
                              outtextxy (65,67,"because the tower is guarding");
                              outtextxy (65,75,"it.");
                              score = 1;
                              hate = 1;
                              }
                              if ((cond < 1) && (building[9]))
                             {
                                 outtextxy (65,35,"   The tower wasn't destroyed.");
                                 outtextxy (65,43,"You were killed in action. You");
                                 outtextxy (65,51,"will be remembered.");
                                 hate = -1;
                                 score = 1;
                             }
                         if ((cond < 1) && (building[9] == false)) 
                         {
                             outtextxy (65,35," The tower was destroyed but so");
                             outtextxy (65,43,"were you. You will be remembered.");
                             hate = -1;
                             score = 20;
                         }
                     }  //  ( mis 9 )
                     break;
            case 10 : {
                          if ((cond > 0) && (building[8] == false)) 
                          {
                              outtextxy (65,35," You destroyed the lab.");
                              outtextxy (65,43,"No more worry of that");
                              outtextxy (65,51,"weapon now.  Good job.");
                              score = 40;
                              hate = 0;
                          }
                          if ((cond > 0) && (building[8]))
                                  {
                                  outtextxy (65,35," You didn't do your mission!");
                                  outtextxy (65,43,"You were supposed to destroy");
                                  outtextxy (65,51,"the lab!  They might you the");
                                  outtextxy (65,59,"the weapon on us now!  Hope-");
                                  outtextxy (65,67,"fully you.");
                                  score = 1;
                                  hate = 1;
                                  }
                                  if ((cond < 1) && (building[8])) 
                                  {
                                  outtextxy (65,35,"   The lab wasn't destroyed.");
                                  outtextxy (65,43,"You were killed in action. You");
                                  outtextxy (65,51,"will be remembered.");
                                  hate = -1;
                                  score = 1;
                                  }
                                  if ((cond < 1) && (building[8] == false))
                          {
                              outtextxy (65,35," The lab was destroyed but so");
                              outtextxy (65,43,"were you. You will be remembered.");
                              hate = -1;
                              score = 25;
                          }
                      }  //  ( mis 10 )
                      break;
            case 11 : {
                          counter = 0;
                          for (loop = 2; loop <= NumofBuildings; loop++)
                              if (building[loop] == false) inc(&counter);
                          if (building[11] == false) dec(&counter);
                          if ((cond > 0) && (counter == 9)) 
                          {
                              outtextxy (65,35," All of the enemy buildings");
                              outtextxy (65,43,"have been destroyed. It will");
                              outtextxy (65,51,"take them at least a year for");
                              outtextxy (65,59,"them to recover. You are a");
                              outtextxy (65,67,"true Commando. Great work.");
                              score = 160;
                              hate = 0;
                          }
                          if ((cond > 0) && (in(counter,6,8))) 
                          {
                              outtextxy (65,35," You destroyed most of the");
                              outtextxy (65,43,"buildings, but not all of");
                              outtextxy (65,51,"them. The enemy is working");
                              outtextxy (65,59,"on repair. We might get some");
                              outtextxy (65,67,"attacks because the enemy");
                              outtextxy (65,75,"wasn't totally destroyed.");
                              score = 90;
                              hate = 1;
                          }
                          if ((cond > 0) && (in(counter,1,5))) 
                          {
                              outtextxy (65,35," You did not complete your");
                              outtextxy (65,43,"mission. The General isn't");
                              outtextxy (65,51,"pleased.");
                              score = 30;
                              hate = 1;
                          }
                          if ((cond < 1) && (in(counter,1,5)))
                          {
                              outtextxy (65,35,"   You destroyed some buildings,");
                              outtextxy (65,43,"but you are unfortunately apart");
                              outtextxy (65,51,"of the rubble. You will be remem-");
                              outtextxy (65,59,"bered for your effort.");
                              hate = -1;
                              score = 50;
                          }
                          if ((cond < 1) && (counter < 3)) 
                          {
                              outtextxy (65,35,"   You were killed before");
                              outtextxy (65,43,"completing your mission.");
                              outtextxy (65,51,"You will be remembered for");
                              outtextxy (65,59,"your bravery.");
                              hate = -1;
                              score = 5;
                          }
                          if ((cond < 1) && (in(counter,6,8))) 
                          {
                              outtextxy (65,35,"   You destroyed most of the build-");
                              outtextxy (65,43,"ings, but you were killed in action.");
                              outtextxy (65,51,"You will be remembered for your skill.");
                              hate = -1;
                              score = 80;
                          }
                          if ((cond < 1) && (counter == 9)) 
                          {
                              outtextxy (65,35,"   You destroyed all of the buildings,");
                              outtextxy (65,43,"but you were tragically killed in");
                              outtextxy (65,51,"action. You will be remembered for your");
                              outtextxy (65,59,"great skill.");
                              hate = -1;
                              score = 140;
                          }
                          if ((cond > 0) && (counter == 0)) 
                          {
                              outtextxy (65,35," You failed to destroy any buildings.");
                              outtextxy (65,43,"Do your mission before returning home!");
                              hate = 2;
                              score = 1;
                          }
                      }  //  ( mis 11 )
                      break;
            case 12 : {
                          if ((cond > 0) && (building[9] == false)) 
                          {
                              outtextxy (65,35," You destroyed the enemy guard tower");
                              outtextxy (65,43,"and returned home alive. Good Job!");
                              outtextxy (65,51,"The General would like to congratulate");
                              outtextxy (65,59,"you.");
                              score = 45;
                              hate = 0;
                          }
                          if ((building[9]) && (cond > 0)) 
                          {
                              outtextxy (65,35,"   The tower wasn't destroyed!");
                              outtextxy (65,43,"Next time do your mission before");
                              outtextxy (65,51,"returning home! I guess we'll have");
                              outtextxy (65,59,"to get REAL pilot to do the job.");
                              hate = 2;
                              score = 1;
                          }
                          if ((building[9] == false) && (cond < 1)) 
                              {
                               outtextxy (65,35,"   The tower was destroyed, but you");
                               outtextxy (65,43,"were taken out too. Good try. You");
                               outtextxy (65,51,"will be remembered for trying.");
                               hate = -1;
                               score = 25;
                               }
                              if ((building[9]) && (cond < 1)) 
                              {
                                  outtextxy (65,35," The tower was not destroyed,");
                                  outtextxy (65,43,"and you were tragically killed in");
                                  outtextxy (65,51,"action. You will be remembered.");
                                  hate = -1;
                                  score = 2;
                              }
                      }  //  ( mis 12 )
                      break;
            case 13 : {
                          if ((cond > 0) && (tank == false)) 
                          {
                              outtextxy (65,35," You destroyed the enemy tank and");
                              outtextxy (65,43,"returned home alive. Good Job! The");
                              outtextxy (65,51,"General would like to congratulate");
                              outtextxy (65,59,"you.");
                              score = 65;
                              hate = 0;
                          }
                          if ((tank) && (cond > 0)) 
                          {
                              outtextxy (65,35,"   The Tank wasn't destroyed!");
                              outtextxy (65,43,"Next time do your mission before");
                              outtextxy (65,51,"returning home! I guess we'll have");
                              outtextxy (65,59,"to get REAL pilot to do the job.");
                              hate = 2;
                              score = 0;
                          }
                          if ((tank == false) && (cond < 1)) 
                          {
                              outtextxy (65,35,"   The tank was destroyed, but so");
                              outtextxy (65,43,"were you.  You will be remembered.");
                              hate = -1;
                              score = 30;
                          }
                          if ((tank) && (cond < 1)) 
                          {
                              outtextxy (65,35," The tank was not destroyed,");
                              outtextxy (65,43,"and you were tragically killed in");
                              outtextxy (65,51,"action.");
                              hate = -1;
                              score = 1;
                          }
                      }  //  ( mis 13 )
                      break;
            case 15 : {
                          if ((cond > 0) && (pl == false)) 
                          {
                              if (building[1]) 
                              {
                                  outtextxy (65,35," You destroyed the enemy transport");
                                  outtextxy (65,43,"plane returned home alive. The");
                                  outtextxy (65,51,"General is very happy about it.");
                                  if ((pstreq(pilot->rank, "Private")) || (pstreq(pilot->rank, "2nd Liut.")) || (pstreq(pilot->rank, "1st Liut."))) 
                                      outtextxy (65,67,"This could mean a promotion.");
                                  score = 115;
                                  hate = 0;
                              }
                              else
                              {
                                  outtextxy (65,35," You destroyed the enemy transport");
                                  outtextxy (65,43,"plane and returned home alive. But");
                                  outtextxy (65,51,"Ocale Base was destroyed.");
                                  score = 35;
                                  hate = 1;
                              }
                          }
                          if ((pl) && (cond > 0)) 
                          {
                              outtextxy (65,35,"   The plane wasn't destroyed.");
                              outtextxy (65,43,"Next time do your mission before");
                              outtextxy (65,51,"returning home. I guess we'll have");
                              outtextxy (65,59,"to get a better pilot next time.");
                              hate = 2;
                              score = 5;
                          }
                          if ((pl == false) && (cond < 1)) 
                          {
                              outtextxy (65,35,"   The plane was destroyed, but so");
                              outtextxy (65,43,"were you. You will be remembered.");
                              hate = -1;
                              score = 50;
                          }
                          if ((pl) && (cond < 1)) 
                          {
                              outtextxy (65,35," The plane was not destroyed,");
                              outtextxy (65,43,"and you were tragically killed in");
                              outtextxy (65,51,"action.");
                              hate = -1;
                              score = 5;
                          }
                      }  //  ( mis 15 )
                      break;
            case 14 : {
                          if ((cond > 0) && (pl == false)) 
                          {
                              if (building[1]) 
                              {
                                  outtextxy (65,35," You destroyed the enemy transport");
                                  outtextxy (65,43,"plane and returned home alive. The");
                                  outtextxy (65,51,"General is very happy about it.");
                                  score = 90;
                                  hate = 0;
                              }
                              else
                              {
                                  outtextxy (65,35," You destroyed the enemy transport");
                                  outtextxy (65,43,"plane and returned home alive. But");
                                  outtextxy (65,51,"Ocale Base was destroyed.");
                                  score = 35;
                                  hate = 1;
                              }
                          }
                          if ((pl) && (cond > 0)) 
                          {
                              outtextxy (65,35,"   The plane wasn't destroyed.");
                              outtextxy (65,43,"Next time do your mission before");
                              outtextxy (65,51,"returning home. I guess we'll have");
                              outtextxy (65,59,"to get a better pilot next time.");
                              hate = 2;
                              score = 5;
                          }
                          if ((pl == false) && (cond < 1)) 
                          {
                              outtextxy (65,35,"   The plane was destroyed, but so");
                              outtextxy (65,43,"were you. You will be remembered.");
                              hate = -1;
                              score = 45;
                          }
                          if ((pl) && (cond < 1)) 
                          {
                              outtextxy (65,35," The plane was not destroyed,");
                              outtextxy (65,43,"and you were tragically killed in");
                              outtextxy (65,51,"action.");
                              hate = -1;
                              score = 5;
                          }
                      }  //  ( mis 14 )
                      break;
            case 16 : {
                          if (building[1] == false)
                          {
                              outtextxy (65,35,"  Ocale base was destroyed.");
                              outtextxy (65,43,"  You have failed.");
                              hate = 2;
                              score = 2;
                          }
                          else // ( building[1] == true )
                          {
                              hate = 1;
                              if (pl == false)
                              {
                                  outtextxy (65,35,"The enemy plane was destroyed,");
                                  if ((deheli[1])) 
                                      outtextxy (65,43,"and so was the enemy helicopter.");
                                  else
                                      outtextxy (65,43,"but not the enemy helicopter.");
                              }
                              else // ( if pl is false )
                              {
                                  outtextxy (65,35,"The enemy plane was not destroyed,");
                                  if ((deheli[1])) 
                                      outtextxy (65,43,"but the enemy helicopter was.");
                                  else
                                      outtextxy (65,43,"and neither was the enemy helicopter.");
                              }
                              if (pl == false) score = 65;
                              else score = 0;
                              if ((deheli[1])) inc_by(&score,25);
                              if ((cond > 0) && (score > 25))
                                  inc_by(&score,40);
                                      else 
                                      if (cond < 1) hate = -1;
                                      else
                                      {
                                      hate = 2;
                                      outtextxy (65,51,"Do the mission before returning home!");
                                      }
                                      if (cond < 1) outtextxy (65,51,"  You were killed in action.");
                                      if (score == 130) 
                                      {
                                      outtextxy (65,51,"  Good work.");
                                      hate = 0;
                                      }
                                      } // ( else )
                                      }  //  ( mis 16 )
                                      break;
                                      case 17 : {
                                      if (building[1] == false)
                                      {
                                      outtextxy (65,35,"  Ocale base was destroyed.");
                                      outtextxy (65,43,"  You have failed.");
                                      hate = 2;
                                      score = 2;
                                      }
                                      else // ( building[1] == true )
                                      {
                                          if (cruisealive == false) 
                                          {
                                              outtextxy (65,35," You destroyed the cruise missile");
                                              score = 150;
                                              hate = 0;
                                              if (cond > 0) outtextxy (65,43," and returned safely. Good Job.");
                                              else
                                              {
                                                  outtextxy (65,43," but were killed in action.");
                                                  score = 40;
                                                  hate = -1;
                                              }
                                          }
                                          else // ( cruisealive == true )
                                          {
                                              if (cond > 0) 
                                              {
                                                  outtextxy (65,35," You didn't finish your mission!!");
                                                  outtextxy (65,43," You were supposed to destroy the");
                                                  outtextxy (65,51," cruise missile, it will be here");
                                                  outtextxy (65,59," any time now.");
                                                  hate = 2;
                                                  score = 0;
                                              }
                                              else
                                              {
                                                  outtextxy (65,35," You didn't destroy the cruise");
                                                  outtextxy (65,43,"missile and were killed. You will");
                                                  outtextxy (65,51,"be remembered.");
                                                  score = 2;
                                                  hate = -1;
                                              }
                                          }
                                      }
                                                }//  ( mis 17 )
                                                break;
                                      case 18 : {
                                                    if (building[1] == false)
                                                    {
                                                        outtextxy (65,35,"  Ocale Base was destroyed.");
                                                        outtextxy (65,43,"  You have failed.");
                                                        hate = 2;
                                                        score = 2;
                                                    }
                                                    else // ( building[1] == true )
                                                    {
                                                        if (cruisealive == false) 
                                                        {
                                                            outtextxy (65,35," You destroyed the cruise missile");
                                                            score = 150;
                                                            hate = 0;
                                                            if (cond > 0) 
                                                            {
                                                                outtextxy (65,43," and returned home safely.");
                                                                if (step == 0) outtextxy (65,51," Great Job!");
                                                                if (step == 1) 
                                                                {
                                                                    outtextxy (65,51," Though the explosion of the");
                                                                    outtextxy (65,59," missile was far enough away");
                                                                    outtextxy (65,67," from our base, your chopper");
                                                                    outtextxy (65,75," wasn't. You are contaminated.");
                                                                    score = score - 35;
                                                                    pstrcpy(pilot->status, "C");
                                                                }
                                                                if (step == 2) 
                                                                {
                                                                    outtextxy (65,51," The explosion of the missile");
                                                                    outtextxy (65,59," wasn't far enough away from");
                                                                    outtextxy (65,67," our base. You and the rest of");
                                                                    outtextxy (65,75," us are contaminated.");
                                                                    pstrcpy(pilot->status, "C");
                                                                    hate = -3;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                outtextxy (65,43," but were killed in action.");
                                                                score = 40;
                                                                hate = -1;
                                                            }
                                                        }
                                                        else // ( cruisealive == true )
                                                        {
                                                            if (cond > 0) 
                                                            {
                                                                outtextxy (65,35," You didn't finish your mission!!");
                                                                outtextxy (65,43," You were supposed to destroy the");
                                                                outtextxy (65,51," cruise missile, it will be here");
                                                                outtextxy (65,59," any time now.");
                                                                hate = 2;
                                                                score = 0;
                                                            }
                                                            else
                                                            {
                                                                outtextxy (65,35," You didn't destroy the cruise");
                                                                outtextxy (65,43,"missle and were killed. You will");
                                                                outtextxy (65,51,"be remembered.");
                                                                score = 2;
                                                                hate = -1;
                                                            }
                                                        }
                                                    }
                                                }//  ( mis 18 )
                                                break;
                                      case 19 : {
                                                    if (building[1] == false)
                                                    {
                                                        outtextxy (65,35,"  Ocale Base was destroyed.");
                                                        outtextxy (65,43,"  You have failed.");
                                                        hate = 2;
                                                        score = 2;
                                                    }
                                                    else // ( building[1] == true )
                                                    {
                                                        if (pl == false)
                                                        {
                                                            outtextxy (65,35," You destroyed the enemy plane,");
                                                            score = 135;
                                                            hate = 0;
                                                            if (cond > 0) 
                                                            {
                                                                outtextxy (65,43," and returned home safely.");
                                                                if (tank == false) 
                                                                {
                                                                    outtextxy (65,51," The 'object' turned out to be");
                                                                    outtextxy (65,59," a tank. It was also destroyed.");
                                                                    outtextxy (65,67," Great Job!");
                                                                    score = score + 15;
                                                                }
                                                                else
                                                                {
                                                                    outtextxy (65,51," The 'object' turned out to be");
                                                                    outtextxy (65,59," a tank. It wasn't destroyed.");
                                                                }
                                                            }
                                                            else // ( cond =< 0 )
                                                            {
                                                                outtextxy (65,43," but were killed in action.");
                                                                score = 40;
                                                                hate = -1;
                                                            }
                                                        }
                                                        else // ( pl == true )
                                                        {
                                                            if (cond > 0) 
                                                            {
                                                                outtextxy (65,35," You didn't finish your mission!!");
                                                                outtextxy (65,43," You were supposed to destroy the");
                                                                outtextxy (65,51," plane! Maybe we can find someone");
                                                                outtextxy (65,59," more qualified.");
                                                                hate = 2;
                                                                score = 1;
                                                            }
                                                            else
                                                            {
                                                                outtextxy (65,35," You didn't destroy the plane");
                                                                outtextxy (65,43," and were killed. You might be");
                                                                outtextxy (65,51," remembered.");
                                                                score = 2;
                                                                hate = -1;
                                                            }
                                                        }
                                                    }
                                                }//  ( mis 19 )
                                                break;
                                      case 20 : {
                                                    if (building[1] == false)
                                                    {
                                                        outtextxy (65,35,"  Ocale Base was destroyed.");
                                                        outtextxy (65,43,"  You have failed.");
                                                        hate = 2;
                                                        score = 2;
                                                    }
                                                    else // ( building[1] == true )
                                                    {
                                                        hate = 1;
                                                        if (cruisealive == false)
                                                        {
                                                            outtextxy (65,35," The cruise missile was destroyed,");
                                                            if ((deheli[1])) 
                                                                outtextxy (65,43," and so was the enemy helicopter.");
                                                            else
                                                                outtextxy (65,43," but not the enemy helicopter.");
                                                        }
                                                        else // ( if pl is false )
                                                        {
                                                            outtextxy (65,35," The cruise missile was not destroyed,");
                                                            if ((deheli[1])) 
                                                                outtextxy (65,43," but the enemy helicopter was.");
                                                            else
                                                                outtextxy (65,43," and neither was the enemy helicopter.");
                                                        }
                                                        if (cruisealive == false) score = 85;
                                                        else score = 0;
                                                        if ((deheli[1])) inc_by(&score,25);
                                                        if ((cond > 0) && (score > 25)) inc_by(&score,20);
                                                        else
                                                            if (cond < 1) hate = -1;
                                                            else
                                                            {
                                                                hate = 2;
                                                                outtextxy (65,51,"Do the mission before returning home!");
                                                            }
                                                        if (cond < 1) outtextxy (65,51," You were killed in action.");
                                                        if (score == 130) 
                                                        {
                                                            outtextxy (65,51," Good work.");
                                                            hate = 0;
                                                        }
                                                    } // ( else )
                                                } // ( mis 20 )
                                                break;
        } // ( of case )
    else {
        score = 0;
        outtextxy (65,35," The base has been destroyed.");
        outtextxy (65,43,"Nobody is left from the explosion.");
        if (cond > 0) 
        {
            outtextxy (65,51,"Everyone is dead except you.");
            hate = -2;
        }
        if (cond <= 0) hate = -3;
    }

    ExtraPoints (&score,&damage,tank,building,deheli);

    score = score - totalejects * (5 * (((int)level)-48));  // (* subtract score for wasting helicopters through ejecting *)
    switch(level) {
        case '1' : score = round_ (score * 0.8); break;
        case '2' : score = round_ (score * 1.1); break;
        case '3' : score = round_ (score * 1.3); break;
    }

    (void)(hate);
    if ((score < 0)) score = score / 3;

    outtextxy (75,130,"MISSION SCORE :");
    sprintf(dummy, " %d", score);
    outtextxyp(175,130,dummy);
    pilot->totalscore = pilot->totalscore + score;
    delay (500);
    Wait (115,157);
    General (hate,mis,pilot->status); // ( conversation in his office )

    if ((cond < 1)) 
    {
        if ((onscreen < 7) || (onscreen > 8)) pstrcpy(pilot->status, "dead");
        else pstrcpy(pilot->status, "missing");
    }
    CheckRank (pilot);
    StatsScreen (pilot,score,hittotal,damage,timedif);
    setgraphmode (0);
} //    { of mission results }


