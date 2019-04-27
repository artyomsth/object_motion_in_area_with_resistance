/*
Modeling of object motion
in area with resistance force and without it.
Comparing curve of motion in both cases
*/

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define g 9.80665
#define pi 3.141592
#define N 100000

using namespace std;

uint32_t i_max;
float x[N], y[N], v[1], vx[N], vy[N];
float limit = 0, range = 0, h_max = 0;

float compute_xy(float &m, float &dt, float &k);
void draw_graph(uint16_t ymax, float &mp);
void setup_graphics();

int main(void)
{
    uint16_t gcolor[2], accuracy;
    float angle = 0, m = 0, mp = 1, t, dt = 0.01, k = 0;
    int16_t x1 = 540, y1 = 430, x2 = 540, y2 = 50;

    x[0] = 0;
    y[0] = 0;

    system( "mode 60, 32" ); // set window size

    cout << endl;
    // scan data and check if values are valid
    cout << "   Initial data:" << endl;
    cout << "   m (kg) = ";
    cin >> m;
    while(m < 1)
    {
        cout << "   m should be >= 1\n";
        cout << "   m (kg) = ";
        cin >> m;
    }

    cout << "   angle (degrees) = ";
    cin >> angle;
    angle = abs(angle);
    while(angle > 90)
    {
        cout << "   angle should be in [-90 ; 90]\n";
        cout << "   angle (degrees) = ";
        cin >> angle;
        angle = abs(angle);
    }

    cout << "   v[0] (m/s) = ";
    cin >> v[0];
    while(v[0] <= 0 | v[0] > 1000)
    {
        cout << "   v[0] should be in(0;1000]\n";
        cout << "   v[0] (m/s) = ";
        cin >> v[0];
    }

    cout << "\n   1 Purple\n   2 Green\n   3 Cyan";
    cout << "\n   4 Coral\n   5 Pink\n   6 Yellow" << endl;
    cout << "   Color (index) = ";
    cin >> gcolor[0];
    while (gcolor[0] < 1 | gcolor[0] > 6)
    {
        cout << "   Color index should be in [1 ; 6]" << endl;
        cout << "   Color (index) = ";
        cin >> gcolor[0];
    }
    gcolor[0]+=8;

    cout << endl;
    cout.setf(ios::fixed);

    // compute initial velocity projections on the x, y axis
    vx[0] = v[0] * cosf(angle * pi / 180);
    vy[0] = v[0] * sinf(angle * pi / 180);

    setup_graphics();
    settextstyle(BOLD_FONT, HORIZ_DIR, 1);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    // draw x, y axis
    for (int i = 0; i < 4; i++)
    {
        line(50, getmaxy() - 50 + i, 590, getmaxy() - 50 + i);
        line(x1 - 490 + i, y1, x2 - 490 + i, y2);
    }

    // reduce compute accuracy if graph is gonna be large
    if (v[0] > 499)
        dt = 0.1;

    t = compute_xy(m, dt, k);

    // compute graph size multiplier value
    if (range > h_max)
    {
        mp = 540 / range;
    }
    else
    {
        mp = 380 / h_max;
    }

    setcolor(gcolor[0]);
    outtextxy(x1, y2 * 2, "k = 0");
    draw_graph(getmaxy() - 50, mp);

    cout << "   Accuracy (number of digits after point) = ";
    cin >> accuracy;
    while (accuracy < 0)
    {
        cout << "   Invalid value" << endl;
        cout << "   Accuracy (number of digits after point) = ";
        cin >> accuracy;
    }

    cout << endl;
    cout << setprecision(accuracy)
         << "   1. Without resistance force:" << endl
         << "   t = " << t << " s" << endl
         << "   X max = " << range << " m" << endl
         << "   Y max = " << h_max << " m" << endl << endl;

    cout << "   k (coefficient of resistance) = ";
    cin >> k;
    while (k <= 0 | k > 1)
    {
        cout << "   k should be in (0;1]" << endl;
        cout << "k = ";
        cin >> k;
        cout << endl;
    }

    cout << "   Color (index) = ";
    cin >> gcolor[1];
    while (gcolor[1] < 1 | gcolor[1] > 6)
    {
        cout << "   Color (index) = ";
        cout << "   Color index should be in [1 ; 6]" << endl;
        cin >> gcolor[1];
    }
    gcolor[1]+=8;

    t = compute_xy(m, dt, k);
    setcolor(gcolor[1]);
    outtextxy(x1, y2 * 1.5, "k > 0");
    draw_graph(getmaxy() - 50, mp);

    cout << endl;
    cout << setprecision(accuracy)
         << "   2. With resistance force:" << endl
         << "   t = " << t << " s" << endl
         << "   X max = " << range << " m" << endl
         << "   Y max = " << h_max << " m" << endl << endl << "   ";

    system("pause");
    restorecrtmode();
    return 0;
}

// THE END. FUNCTIONS USED IN PROGRAM:

float compute_xy(float &m, float &dt, float &k)
{
    float t = 0;
    h_max = 0;
    for (int i = 1; i <= N; i++) // compute coordinates
    {
        vx[i] = vx[i - 1] - k * vx[i - 1] * dt / m;
        vy[i] = vy[i - 1] - (g + k * vy[i - 1]/ m) * dt;
        x[i] = x[i - 1] + vx[i - 1] * dt;
        y[i] = y[i - 1] + vy[i - 1] * dt;

        if (y[i] > h_max)
            h_max = y[i];

        if (y[i] <= 0 | i == N) // exit from loop in case of object fallen
        {
            limit = y[i - 1];
            range = x[i];
            i_max = i;
            t = i * dt;
            break;
        }
    }

    return(t);
}

void draw_graph(uint16_t ymax, float &mp)
{
    for (int i = 0; i <= N - 1; i++) // draw graph
    {
        for (int j = 0; j < 4; j++)
        {
            line(50 + x[i] * mp, ymax - y[i] * mp + j, 50 + x[i + 1] * mp, ymax - y[i + 1] * mp + j);
            line(50 + x[i] * mp + j, ymax - y[i] * mp, 50 + x[i + 1] * mp + j, ymax - y[i + 1] * mp);
        }
        if (i > i_max / 2 && y[i] == limit) // end drawing
            break;
    }
}

void setup_graphics()
{
    int gdriver = DETECT, gmode, errorcode;
    initgraph(&gdriver, &gmode, "");
    errorcode = graphresult();
    if (errorcode != grOk) // in case of error
    {
        cout << "ERROR" << grapherrormsg(errorcode);
        cout << "Press any key to halt:";
        cin.get(); // wait for button press
        exit(1);
    }
}
