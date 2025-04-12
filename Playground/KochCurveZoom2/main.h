#ifndef MAIN_H
#define MAIN_H

#include "./utils/Utils.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define DRAW_WIDTH 3 

typedef struct KochCurve {
    Point p[5];
} KochCurve;

int gen_koch_curve(Line line, KochCurve *curve);
int koch_curve(Color color, Line * line, Box draw_boundary, int first);

#endif // MAIN_H
