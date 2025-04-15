#ifndef MAIN_H
#define MAIN_H

#include "./utils/Utils.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

typedef struct KochCurve {
    Point p[5];
} KochCurve;

KochCurve init_curve(Line * line);
int draw_curve(KochCurve * curve);



#endif // MAIN_H
