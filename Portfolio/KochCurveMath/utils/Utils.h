#ifndef UTILS_H
#define UTILS_H

#include "FPToolkit.h"
#include <stdio.h>
#include <stdlib.h>

// These only have #defines
#include "Palette.h"      // Colors
#include "X11_keycodes.h" // Return signals from X11 keypresses

// Useful Constants
#define TRUE 1
#define FALSE 0

// Clipping Codes
typedef int ClipCode;
#define CLIP_INSIDE 0b0000
#define CLIP_LEFT 0b0001
#define CLIP_RIGHT 0b0010
#define CLIP_BOTTOM 0b0100
#define CLIP_TOP 0b1000
#define CLIP_ALL 0b1111
#define CLIP_FAIL 0b1111

// Useful Constants
#define SQRT_3_OVER_2 0.8660254037844386

// ########## STRUCTS ##########
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

// ########## COLORS AND DRAWING ##########
Color U_color_hex(int hex);
int U_shift_hsv(Color *io_color, double dh, double ds, double dv);
int U_shift_rgb(Color *io_color, double dr, double dg, double db);

// These shouldn't really be used outside of this file.
int C_update_hsv(Color *io_color);
int C_update_rgb(Color *io_color);

int U_print_color(Color *io_color);
int U_set_color(Color *color);
int U_set_color_hex(int hex);
int U_draw_buffer();

int U_draw_line(Line *line, double weight);
int U_draw_dotted_line(Line *line, double weight, int dot_length);
int U_draw_box_on_line(Line *line, double weight, double width, double height);

// ########### INPUT ##########
int U_wait_key();
int U_wait_click(double p[2]);
int U_wait_click_point(Point *p);
double U_get_double(char *prompt);

// ########## MATRIX TOOLS ##########
double **U_create_matrix2d(double n, double m);
double **U_create_sq_matrix(double n);
void U_free_matrix2d(double **matrix, double n, double m);
void U_free_sq_matrix(double **matrix, double n);

// ########## LINE FUNCTIONS ##########
Point U_lerp(Point *p0, Point *p1, double percentage);
Point U_lerp_line(Line *line, double percentage);
double U_distance(Point *p0, Point *p1);
double U_length(Line *line);
double U_angle(Point *p0, Point *p1);
double U_angle_line(Line *line);
int U_lerp_perp(Point *output, Point *p0, Point *p1, double ratio);
double U_perp_angle(double rads);

int U_shift_point(Point *io_point, double angle, double distance);
int U_scale_point(Point *io_point, Point *origin, double scale);
int U_scale_line(Line *io_line, Point *origin, double scale);

// ########## LINE FUNCTIONS ##########
double U_eq_triangle_height(double base);
Box U_gen_box_by_size(Point *origin, double width, double height);
void U_print_box(Box box);
Point U_right_angle(Line *hypotenuse);
Point U_right_angle_reverse(Line *hypotenuse);

// Collisions
ClipCode U_point_intersect_box(Point *point, Box *box);
ClipCode U_line_intersect_box(Line *line, Box *box);

// ######## MISC FUNCTIONS ##########
int U_clamp_int(int value, int min, int max);
double U_clamp_double(double value, double min, double max);
int U_wrap_int(int value, int min, int max);
double U_wrap_double(double value, double min, double max);

#endif // UTILS_H
