#ifndef MAIN_H
#define MAIN_H

#include "./utils/Utils.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

typedef struct Production {
    char var;
    char axiom[100];
    char rule[100];
} Production;

int find_rule(char s);
void string_builder();
void string_interpreter();
void autoplacer(int swidth, int sheight);

void square_wave();
void sierpinski();

#endif // MAIN_H
