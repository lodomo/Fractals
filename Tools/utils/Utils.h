#ifndef UTILS_H
#define UTILS_H

#include "FPToolkit.h"
#include <stdio.h>
#include <stdlib.h>

// These only have #defines
#include "Palette.h"        // Colors
#include "X11_keycodes.h"   // Return signals from X11 keypresses

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
Color U_init_color_rgb_int(int r, int g, int b);
Color U_init_color_rgb_percent(double r, double g, double b);
Color U_init_color_hex(int hex);
Color U_init_color_hsv_int(int h, int s, int v);
Color U_init_color_hsv_percent(double h, double s, double v);
void U_shift_hsv(Color *color, double dh, double ds, double dv);

// INTERNAL COLOR FUNCTIONS
int C_hsv_from_rgb(Color *color);
int C_rgb_from_hsv(Color *color);
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

// ######## MISC FUNCTIONS ##########
int U_clamp_int(int value, int min, int max);
double U_clamp_double(double value, double min, double max);
int U_wrap_int(int value, int min, int max);
double U_wrap_double(double value, double min, double max);

#endif // UTILS_H
