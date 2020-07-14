
void DrawMan()
{
    graphcolormode();
    draw (2,6,4,6,2);   // draw parachute
    draw (1,7,5,7,2);
    draw (1,8,2,8,2);
    draw (4,8,5,8,2);
    draw (1,9,2,10,2);
    draw (5,9,4,10,2);
    draw (2,12,4,12,3);
    plot (3,11,3);    // draw man
    plot (4,12,3);
    plot (2,14,3);
    plot (4,14,3);
    plot (3,13,3);
    getpic (pict1,2,11,4,14);
    getpic (pict3,1,5,5,14);
    draw (2,14,4,14,0);
    plot (3,14,3);
    getpic (pict2,2,11,4,14);
    getpic (pblank,100,100,102,103);
}

