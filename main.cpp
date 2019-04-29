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
#define N 65535

using namespace std;

float x[N], y[N], v[1], vx[N], vy[N];

struct motion_description
{
    float t = 0, range = 0, h_max = 0, y_min = 0;
    uint16_t num_of_dots = 0;
} object;

void compute_xy(float &m, float &dt, float &k);
void draw_graph(uint16_t ymax, float &multiplier);
void setup_graphics();

int main(void)
{
    uint16_t graph_color[2], accuracy;
    float angle = 0, m = 0, multiplier = 1, dt = 0.01, k = 0;
    int16_t x1 = 540, y1 = 430, x2 = 540, y2 = 50;

    x[0] = 0;
    y[0] = 0;

    system( "mode 60, 32" ); // set window size

    cout << endl;
    // scan data and check if values are valid
    cout << "   Initial data:" << endl;
    cout << "   m (kg) = ";
    cin >> m;
    while(m < 1 | m > 10000)
    {
        cout << "   m should be in [1 ; 10000]\n";
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
    while(v[0] <= 0 | v[0] > 10000)
    {
        cout << "   v[0] should be in(0;10000]\n";
        cout << "   v[0] (m/s) = ";
        cin >> v[0];
    }

    cout << "\n   1 Purple\n   2 Green\n   3 Cyan";
    cout << "\n   4 Coral\n   5 Pink\n   6 Yellow" << endl;
    cout << "   Color (index) = ";
    cin >> graph_color[0];
    while (graph_color[0] < 1 | graph_color[0] > 6)
    {
        cout << "   Color index should be in [1 ; 6]" << endl;
        cout << "   Color (index) = ";
        cin >> graph_color[0];
    }
    graph_color[0]+=8;

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
    if (v[0] > 2999)
        dt = 0.1;
    else if (v[0] > 499)
        dt = 0.05;

    compute_xy(m, dt, k);

    // compute graph size multiplier value
    if (object.range > object.h_max)
        multiplier = 540 / object.range;
    else
        multiplier = 380 / object.h_max;

    setcolor(graph_color[0]);
    outtextxy(x1, y2 * 2, "k = 0");
    draw_graph(getmaxy() - 50, multiplier);

    cout << "   Accuracy (number of digits after point) = ";
    cin >> accuracy;
    while (accuracy < 0 | accuracy > 7)
    {
        cout << "   Accuracy number shoud be in [0 ; 7]" << endl;
        cout << "   Accuracy (number of digits after point) = ";
        cin >> accuracy;
    }

    cout << endl;
    cout << setprecision(accuracy)
         << "   1. Without resistance force:" << endl
         << "   t = " << object.t << " s" << endl
         << "   X max = " << object.range << " m" << endl
         << "   Y max = " << object.h_max << " m" << endl << endl;

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
    cin >> graph_color[1];
    while (graph_color[1] < 1 | graph_color[1] > 6)
    {
        cout << "   Color (index) = ";
        cout << "   Color index should be in [1 ; 6]" << endl;
        cin >> graph_color[1];
    }
    graph_color[1]+=8;

    compute_xy(m, dt, k);
    setcolor(graph_color[1]);
    outtextxy(x1, y2 * 1.5, "k > 0");
    draw_graph(getmaxy() - 50, multiplier);

    cout << endl;
    cout << setprecision(accuracy)
         << "   2. With resistance force:" << endl
         << "   t = " << object.t << " s" << endl
         << "   X max = " << object.range << " m" << endl
         << "   Y max = " << object.h_max << " m" << endl << endl << "   ";

    system("pause");
    restorecrtmode();
    return 0;
}

// THE END. FUNCTIONS USED IN PROGRAM:

void compute_xy(float &m, float &dt, float &k)
{
    object.h_max = 0;
    for (int i = 1; i <= N; i++) // compute coordinates
    {
        vx[i] = vx[i - 1] - k * vx[i - 1] * dt / m;
        vy[i] = vy[i - 1] - (g + k * vy[i - 1]/ m) * dt;
        x[i] = x[i - 1] + vx[i - 1] * dt;
        y[i] = y[i - 1] + vy[i - 1] * dt;

        if (y[i] > object.h_max)
            object.h_max = y[i];

        if (y[i] <= 0 | i == N) // exit from loop in case of object fallen
        {
            object.y_min = y[i - 1];
            object.range = x[i];
            object.num_of_dots = i;
            object.t = i * dt;
            break;
        }
    }
}

void draw_graph(uint16_t ymax, float &multiplier)
{
    for (int i = 0; i <= N - 1; i++) // draw graph
    {
        for (int j = 0; j < 4; j++)
        {
            line(50 + x[i] * multiplier, ymax - y[i] * multiplier + j, 50 + x[i + 1] * multiplier, ymax - y[i + 1] * multiplier + j);
            line(50 + x[i] * multiplier + j, ymax - y[i] * multiplier, 50 + x[i + 1] * multiplier + j, ymax - y[i + 1] * multiplier);
        }
        if (i > object.num_of_dots / 2 && y[i] == object.y_min) // end drawing
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
