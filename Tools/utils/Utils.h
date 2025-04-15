#ifndef UTILS_H
#define UTILS_H

#include "FPToolkit.h"
#include <stdio.h>
#include <stdlib.h>

// These only have #defines
#include "Palette.h"      // Colors
#include "X11_keycodes.h" // Return signals from X11 keypresses

#define TRUE 1
#define FALSE 0

// Clipping Codes
#define CLIP_INSIDE 0b0000
#define CLIP_LEFT 0b0001
#define CLIP_RIGHT 0b0010
#define CLIP_BOTTOM 0b0100
#define CLIP_TOP 0b1000
#define CLIP_ALL 0b1111
#define CLIP_FAIL 0b1111

typedef int ClipCode;

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
void U_shift_rgb(Color *color, double dr, double dg, double db);

// INTERNAL COLOR FUNCTIONS
int C_update_hsv(Color *color);
int C_update_rgb(Color *color);
int C_print_color(Color *color);

int U_set_draw_color(Color color);
int U_set_draw_color_hex(int hex);

// DRAWING AND INPUT
int U_wait_key();
int U_wait_click(double p[2]);
int U_draw_buffer();

// MATRIX FUNCTIONS
double **U_create_matrix2d(double n, double m);
double **U_create_sq_matrix(double n);
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
    Point p[2];
} Line;

// Inherently, does not rotate.
// p[0] is the origin
// p[1] is the destination
typedef struct Box {
    Point p[2];
} Box;

typedef struct Triangle {
    Point p[3];
} Triangle;

Point U_point_pos_by_percent(Point p0, Point p1, double percentage);
Point U_line_pos_by_percent(Line line, double percentage);

double U_distance_two_points(Point p0, Point p1);
double U_line_length(Line line);

double U_point_angle(Point p0, Point p1);
double U_line_angle(Line line);

double U_perp_angle(double rads);

int U_shift_point(Point *point, double angle, double distance);
int U_scale_point(Point origin, Point *point, double scale);
int U_scale_line(Point origin, Line *line, double scale);

// Shape Tools
double U_eq_triangle_height(double base);
Box U_gen_box_by_size(Point origin, double width, double height);
void U_print_box(Box box);

// Collisions
ClipCode U_point_intersect_box(Point p, Box box);
ClipCode U_line_intersect_box(Line line, Box box);

// ######## MISC FUNCTIONS ##########
int U_clamp_int(int value, int min, int max);
double U_clamp_double(double value, double min, double max);
int U_wrap_int(int value, int min, int max);
double U_wrap_double(double value, double min, double max);

#endif // UTILS_H
