#ifndef LDM_UTILS_H
#define LDM_UTILS_H

#include "FPToolkit.h"
#include <stdio.h>
#include <stdlib.h>

// X11 key codes
#define LEFT 65361
#define RIGHT 65363
#define UP 65362
#define DOWN 65364
#define KEY_Q 113

// Color Codes
#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

// My favorite color palette SoftMilk32
#define SM32_HOT_PINK 0xd95b9a
#define SM32_PURPLE 0x9e4491
#define SM32_DARK_PURPLE 0x633662
#define SM32_DARK_RED 0x903d62
#define SM32_RED 0xbd515a
#define SM32_ORANGE 0xd69a4e
#define SM32_YELLOW 0xf3d040
#define SM32_LIGHT_YELLOW 0xf3d040
#define SM32_SOFT_GREEN 0x94e092
#define SM32_SEA_FOAM 0x1f9983
#define SM32_NIGHT_FOAM 0x22636b
#define SM32_PINK 0xc56876
#define SM32_BROWN 0x5c3841
#define SM32_LIGHT_BROWN 0x945848
#define SM32_TAN 0xd17f6b
#define SM32_LIGHT_TAN 0xeb9f7f
#define SM32_PEACH 0xf1c28f
#define SM32_LIGHT_GRAY 0xb9b5c3
#define SM32_GRAY 0x76747d
#define SM32_DARK_GRAY 0x57546f
#define SM32_BLUE_5 0x454194
#define SM32_BLUE_4 0x425bbd
#define SM32_BLUE_3 0x4884d4
#define SM32_BLUE_2 0x45a1de
#define SM32_BLUE_1 0x7cd8eb
#define SM32_GREEN_5 0x376129
#define SM32_GREEN_4 0x597f1e
#define SM32_GREEN_3 0x82aa28
#define SM32_GREEN_2 0xc3d442
#define SM32_GREEN_1 0xe2f266

#define SM32_WHITE 0xf2f2f0
#define SM32_BLACK 0x23213d

// Useful Constants

#define SQRT_2 1.4142135623730951
#define SQRT_3 1.7320508075688772
#define HALF_SQRT_3 0.8660254037844387


/*
 * Stores colors as RGB and HSV values.
 * All values are in the range of [0.0 to 1.0]
 */
typedef struct Color {
    double r;
    double g;
    double b;
    double h;
    double s;
    double v;
} Color;

// COLOR FUNCTIONS
Color C_rgb_from_ints(int r, int g, int b);
Color C_rgb_from_percent(double r, double g, double b);
Color C_rgb_from_hex(int hex);
int C_update_hsv(Color *color);
int C_update_rgb(Color *color);
int C_print_color(Color *color);

int U_set_color(Color color);
// int U_set_color(char * hex);
// int U_set_color_rgb(int r, int g, int b);
// int U_set_color_hsv(int h, int s, int v);

// MATRIX FUNCTIONS
double ** U_create_matrix2d(double n, double m);
double ** U_create_sq_matrix(double n);
void U_free_matrix2d(double **matrix, double n, double m);
void U_free_sq_matrix(double **matrix, double n);

// User Input
double U_get_double(char *prompt);

// Line Tools
typedef struct Point {
    double x;
    double y;
} Point;

typedef struct Line {
    Point start;
    Point end;
} Line;

Point U_get_pos_by_percent(Line line, double percentage);
double U_line_length(Line line);
double U_get_angle_rad(Line line);
double U_perp_angle(double rads);
Point U_shift_point(Point p, double angle, double distance);

// Shape Tools
double U_eq_triangle_height(double base);


#endif // LDM_UTILS_H
