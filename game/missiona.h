
void Wait(int16_t x, int16_t y) 
{
    int16_t c; // color
    char keyhit = 'q';

    do {
        outtextxy (x-2,y,"press space key");
        inc(&c);
        delay (50);

        if (odd (c)) c = 1;
        else c = 2;

        setcolor(c);
        delay (5);
        if (keypressed()) keyhit = readkey();
        if (keyhit == 27) halt();
    } while (keyhit != ' '); // (* 27 = esc *)
}

void MissionSetUp()
{
    // draw rect around mission text

    setgraphmode(1);
    cleardevice();
    setcolor(0);
    setfillstyle(1,3);
    bar(50,20,285,150);
    settextstyle(2,0,5);
}

void Mission1()
{
    outtextxy (60,20,"MISSION  1A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," A large attack is planned against");
    outtextxy (65,43,"enemy territory. An enemy heli-");
    outtextxy (65,51,"copter \"P-77\" is currently guard-");
    outtextxy (65,59,"ing the west side of the enemy");
    outtextxy (65,67,"territory. Your mission is to");
    outtextxy (65,75,"destroy the \"P-77\" located 2 miles");
    outtextxy (65,83,"(2 screens) east of Ocale Base,");
    outtextxy (65,91,"then return to Ocale Base to con-");
    outtextxy (65,99,"firm the kill. You have 1 minute.");
}

void Mission2()
{
    outtextxy (60,20,"MISSION  2A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," A cruise missile has been spotted");
    outtextxy (65,43,"on our radar. It""s headed for our");
    outtextxy (65,51,"base (Ocale Base). It""s located 10");
    outtextxy (65,59,"miles east going 250 MPH west. It");
    outtextxy (65,67,"must be destroyed before it reaches");
    outtextxy (65,75,"our base. Return home after mission");
    outtextxy (65,83,"complete.");
}

void Mission3()
{
    outtextxy (60,20,"MISSION  3A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," A \"P-79\" helicopter has been");
    outtextxy (65,43,"sighted on radar. It""s currently");
    outtextxy (65,51,"5 miles east of Ocale Base (our");
    outtextxy (65,59,"base) going 60 to 70 MPH west in");
    outtextxy (65,67,"our direction. Fire upon sight of");
    outtextxy (65,75,"aircraft. We are very confident");
    outtextxy (65,83,"that his intentions are negative.");
    outtextxy (65,91,"Then return home to Ocale Base.");
    outtextxy (65,99,"WARNING - the \"P-79\" has double");
    outtextxy (65,107,"firepower.");
}

void Mission4()
{
    outtextxy (60,20,"MISSION  4A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," Kuremee-Gabaree-Ushaad has been");
    outtextxy (65,43,"sighted at the Enemy Intelligence");
    outtextxy (65,51,"Building located 4 miles(4 screens)");
    outtextxy (65,59,"east of Ocale Base. Kuremee is a");
    outtextxy (65,67,"known terrorist and must be killed.");
    outtextxy (65,75,"Your mission is to destroy the");
    outtextxy (65,83,"E. I. Building located 4 miles east");
    outtextxy (65,91,"of Ocale Base (our base).");
    outtextxy (65,99,"Note - Enemy boundaries are 2 miles");
    outtextxy (65,107,"east of Ocale Base. Good Luck.");
}

void Mission5()
{
    outtextxy (60,20,"MISSION  5A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," The Enemy Radar has made it");
    outtextxy (65,43,"impossible for suprise attacks");
    outtextxy (65,51,"against them. An upcoming attack");
    outtextxy (65,59,"will require the radar to be");
    outtextxy (65,67,"destroyed. Your mission is to");
    outtextxy (65,75,"destroy the enemy radar located");
    outtextxy (65,83,"5 miles east of Ocale Base (our");
    outtextxy (65,91,"base) then return home safely.");
    outtextxy (65,99,"Good luck.");
}

void Mission6()
{
    outtextxy (60,20,"MISSION  6A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," A special military device must");
    outtextxy (65,43,"be sent to NorthCape base");
    outtextxy (65,51,"located 12 miles east of Ocale");
    outtextxy (65,59,"Base. It must be sent in less");
    outtextxy (65,67,"than 2 minutes. Return home to");
    outtextxy (65,75,"Ocale Base when mission");
    outtextxy (65,83,"complete. Good luck.");
}

void Mission7()
{
    outtextxy (60,20,"MISSION  7A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," Top secret papers must be sent");
    outtextxy (65,43,"to NorthCape base located 12 miles");
    outtextxy (65,51,"east (12 screens to the right) of");
    outtextxy (65,59,"Ocale Base. You will be flying");
    outtextxy (65,67,"over enemy territory so be careful.");
    outtextxy (65,75,"Hint - fly as fast as possible.");
}

void Mission8()
{
    bar (50,20,290,165);
    outtextxy (60,20,"MISSION  8A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," Thirty minutes ago a transport");
    outtextxy (65,43,"helicopter was shot down. It was");
    outtextxy (65,51,"carrying a top secret capsule.");
    outtextxy (65,59,"Luckily the capsule was ejected");
    outtextxy (65,67,"before the plane went down.");
    outtextxy (65,75," Your mission is to recover the");
    outtextxy (65,83,"capsule and return it safely to");
    outtextxy (65,91,"NorthCape Base which is located");
    outtextxy (65,99,"12 miles east of Ocale Base. The");
    outtextxy (65,107,"RED capsule is located approx. 8");
    outtextxy (65,115,"miles east of Ocale Base. You");
    outtextxy (65,123,"must exit your aircraft and per-");
    outtextxy (65,131,"sonally retrieve the capsule.");
}

void Mission9()
{
    outtextxy (60,20,"MISSION  9A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," We are planning an attack on");
    outtextxy (65,43,"enemy territory. First the");
    outtextxy (65,51,"tower located 3 miles west of");
    outtextxy (65,59,"NorthCape Base (3 screens to");
    outtextxy (65,67,"the left) must be destroyed so");
    outtextxy (65,75,"we are sure to get by it. Your");
    outtextxy (65,83,"mission is to destroy the");
    outtextxy (65,91,"tower. Return home after miss-");
    outtextxy (65,99,"ion complete.  Good Luck.");
}

void Mission10()
{
    outtextxy (60,20,"MISSION  10A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," We received top secret inform-");
    outtextxy (65,43,"ation on a special weapon being");
    outtextxy (65,51,"developed at an enemy lab. The");
    outtextxy (65,59,"lab must be destroyed. Your");
    outtextxy (65,67,"mission is to destroy the lab");
    outtextxy (65,75,"located 4 miles west of North-");
    outtextxy (65,83,"Cape Base (4 screens to the");
    outtextxy (65,91,"left). Return home after miss-");
    outtextxy (65,99,"ion complete. Good luck.");
}

void Mission11()
{
    outtextxy (60,20,"MISSION  11A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," A full state of war now");
    outtextxy (65,43,"exists. The General thinks");
    outtextxy (65,51,"you""re the only one who can");
    outtextxy (65,59,"do this seemingly impossible");
    outtextxy (65,67,"mission. Your mission is to");
    outtextxy (65,75,"destroy everything. If it""s");
    outtextxy (65,83,"a part of the enemy, destroy");
    outtextxy (65,91,"it. Good luck.");
}

void Mission12()
{
    outtextxy (60,20,"MISSION  12A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," An attack on enemy territory");
    outtextxy (65,43,"is in the planning. First the");
    outtextxy (65,51,"enemy guard tower must be de-");
    outtextxy (65,59,"stroyed. It is located 3 miles");
    outtextxy (65,67,"(3 screens) west of North Cape");
    outtextxy (65,75,"Base. Fly directly over it and");
    outtextxy (65,83,"release as many bombs as it");
    outtextxy (65,91,"takes.  Return home to North");
    outtextxy (65,99,"Cape Base when mission com-");
    outtextxy (65,108,"plete. Good luck, pilot.");
}

void Mission13()
{
    outtextxy (60,20,"MISSION  13A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"   Countless aircraft of ours");
    outtextxy (65,43,"have been shot down by an X-10");
    outtextxy (65,51,"tank. We are fed up with it.");
    outtextxy (65,59,"Your mission is to destroy the");
    outtextxy (65,67,"X-10 tank located 4 miles west");
    outtextxy (65,75,"of NorthCape Base. Return home");
    outtextxy (65,83,"when mission complete.");
    outtextxy (65,91,"  Good luck!");
}

void Mission14()
{
    outtextxy (60,20,"MISSION  14A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted an enemy transport");
    outtextxy (65,43,"plane flying west at 200 mph. It is");
    outtextxy (65,51,"located 10 miles east of Ocale Base.");
    outtextxy (65,59,"We suspect it is planning to bomb us.");
    outtextxy (65,67,"Your mission is to intercept and de-");
    outtextxy (65,75,"stroy the transport plane before it");
    outtextxy (65,83,"reaches Ocale Base.");
    outtextxy (65,91,"  Good luck!");
}

void Mission15()
{
    outtextxy (60,20,"MISSION  15A :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted an enemy transport");
    outtextxy (65,43,"plane flying west at 200 mph. It is");
    outtextxy (65,51,"located 3 miles west of North Cape");
    outtextxy (65,59,"Base. We suspect it is planning to");
    outtextxy (65,67,"bomb Ocale Base. Your mission is to");
    outtextxy (65,75,"destroy the transport plane before");
    outtextxy (65,83,"it reaches Ocale Base.");
    outtextxy (65,91,"  Good luck!");
}


void Mission16()
{
    outtextxy (60,20,"MISSION  1B :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted an enemy transport");
    outtextxy (65,43,"plane flying west at 200 mph. It is");
    outtextxy (65,51,"located 4 miles east of Ocale Base.");
    outtextxy (65,59,"We""ve also spotted an enemy helicopter");
    outtextxy (65,67,"3 miles west of Ocale Base. We suspect");
    outtextxy (65,75,"a dual attack. Your mission is to");
    outtextxy (65,83,"destroy these suspects before they");
    outtextxy (65,91,"destroy us. Good luck!");
}

void Mission17()
{
    outtextxy (60,20,"MISSION  2B :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted an enemy cruise");
    outtextxy (65,43,"missile on our radar. It""s headed");
    outtextxy (65,51,"for Ocale base. It""s located 6");
    outtextxy (65,59,"miles east of Ocale Base going 250");
    outtextxy (65,67,"MPH west. You are at NorthCape Base.");
    outtextxy (65,75,"Fly west until you catch up with the");
    outtextxy (65,83,"cruise missile and destroy it.");
    outtextxy (65,91,"Return home when mission complete.");
}

void Mission18()
{
    outtextxy (60,20,"MISSION  3B :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted an enemy cruise");
    outtextxy (65,43,"missile on our radar. It""s headed");
    outtextxy (65,51,"for Ocale base. It""s located 7");
    outtextxy (65,59,"miles east of Ocale Base going 200");
    outtextxy (65,67,"MPH west. We believe that it has");
    outtextxy (65,75,"a nuclear warhead. It must be");
    outtextxy (65,83,"destroyed at least 2 miles away from");
    outtextxy (65,91,"our base to avoid contamination.");
    outtextxy (65,99,"Return home after mission complete.");
}

void Mission19()
{
    outtextxy (60,20,"MISSION  4B :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted an enemy transport");
    outtextxy (65,43,"plane on our radar. It""s headed for");
    outtextxy (65,51,"Ocale base. It""s located 9 miles east");
    outtextxy (65,59,"of Ocale Base. It seems that it is");
    outtextxy (65,67,"carrying some large object. We");
    outtextxy (65,75,"believe that this \"object\" is a bomb.");
    outtextxy (65,83,"It might not be. The plane and object");
    outtextxy (65,91,"must be destroyed. Return home after");
    outtextxy (65,99,"mission complete.");
}

void Mission20()
{
    outtextxy (60,20,"MISSION  5B :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"  We""ve spotted a cruise missile");
    outtextxy (65,43,"on our radar. It""s headed for Ocale");
    outtextxy (65,51,"Base. It""s located 9 miles east");
    outtextxy (65,59,"of Ocale Base going 300 MPH west.");
    outtextxy (65,67,"There is also a helicopter approach-");
    outtextxy (65,75,"ing Ocale Base. Your mission is to");
    outtextxy (65,83,"destroy them both.");
    outtextxy (65,91,"Return home after mission complete.");
}

void Practice()
{
    MissionSetUp();
    outtextxy (60,20," Beginner Practice :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," Your mission is to fly 3");
    outtextxy (65,43,"screens to the right (3 miles),");
    outtextxy (65,51,"then shoot down the helicopter.");
    outtextxy (65,59,"After that return back to the");
    outtextxy (65,67,"base. Remember to press E to eject");
    outtextxy (65,75,"if things look rough. Good luck!");
    Wait(115,135);
    setgraphmode(0);
}

void Practice1()
{
    MissionSetUp();
    outtextxy (60,20," Mission Results :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," You completed your beginner");
    outtextxy (65,43,"mission. The General thinks you""re");
    outtextxy (65,51,"ready to do a real one next time.");
    outtextxy (65,59,"Good job.");
    Wait(115,135);
    setgraphmode(0);
}

void Practice2()
{
    MissionSetUp();
    outtextxy (60,20," Good Try :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35,"   You unfortunately failed to");
    outtextxy (65,43,"complete your mission. The General");
    outtextxy (65,51,"recommends trying this mission");
    outtextxy (65,59,"again until you can complete it.");
    Wait(115,135);
    setgraphmode(0);
}

void Practice3()
{
    MissionSetUp();
    outtextxy (60,20," Bad take off :");
    setcolor(2);
    settextstyle (2,0,4);
    outtextxy (65,35," You need to work on your take-");
    outtextxy (65,43,"offs. Hint- press up once and");
    outtextxy (65,51,"immediately press down twice.");
    outtextxy (65,59,"Then fly out of the hole.");
    Wait(115,135);
    setgraphmode(0);
}

