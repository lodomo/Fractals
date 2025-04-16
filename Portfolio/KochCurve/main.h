#ifndef MAIN_H
#define MAIN_H

#include "./utils/Utils.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define DRAW_WIDTH 2

typedef struct KochCurve {
    Point p[5];
} KochCurve;

KochCurve init_curve(Line * line);
int draw_curve(KochCurve * curve);
int paint_math(Line * line, int*frame);

int koch_by_depth(Line * line, int depth);
int koch_by_length(Line *line, Box * draw_boundary, int first);

#endif // MAIN_H
