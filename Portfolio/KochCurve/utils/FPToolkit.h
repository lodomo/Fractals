#ifndef FPToolkit_H
#define FPToolkit_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int G_init_graphics(double w, double h);
int Gi_wait_click(int p[2]);
int G_wait_click(double p[2]);
int G_wait_key();
int G_no_wait_key();
int G_sector(double xcenter, double ycenter, double radius,
             double start_radians, double end_radians);
int G_fill_sector(double xcenter, double ycenter, double radius,
                  double start_radians, double end_radians);
int G_wait_mouse();
double G_x_mouse();
double G_y_mouse();
int G_get_time(double *hms);
int G_save_to_bmp_file(const char *fname);
int G_display_bmp_file(const char *fname, double xoffset, double yoffset);

extern int (*G_close)();
extern int (*G_display_image)();
extern int (*Gi_events)(int *d);
extern int (*G_events)(double *d);
extern int (*G_change_pen_dimensions)(double w, double h);
extern int (*Gi_get_current_window_dimensions)(int *dimentsions);
extern int (*G_get_current_window_dimensions)(double *dimentsions);
extern int (*Gi_rgb)(int r, int g, int b);
extern int (*G_rgb)(double r, double g, double b);
extern int (*G_pixel)(double x, double y);
extern int (*G_point)(double x, double y);
extern int (*G_circle)(double a, double b, double r);
extern int (*G_unclipped_line)(double ixs, double iys, double ixe, double iye);
extern int (*G_line)(double ixs, double iys, double ixe, double iye);
extern int (*Gi_polygon)(int *x, int *y, int numpts);
extern int (*G_polygon)(double *x, double *y, double numpts);
extern int (*G_triangle)(double x0, double y0, double x1, double y1, double x2,
                         double y2);
extern int (*G_rectangle)(double xleft, double yleft, double width,
                          double height);
extern int (*G_single_pixel_horizontal_line)(double x0, double x1, double y);
extern int (*G_clear)();
extern int (*G_fill_circle)(double a, double b, double r);
extern int (*G_unclipped_fill_polygon)(double *xx, double *yy, double n);
extern int (*Gi_fill_polygon)(int *xx, int *yy, int n);
extern int (*G_fill_polygon)(double *xx, double *yy, double n);
extern int (*G_fill_triangle)(double x0, double y0, double x1, double y1,
                              double x2, double y2);
extern int (*G_fill_rectangle)(double xleft, double yleft, double width,
                               double height);
extern int (*G_font_pixel_height)();
extern int (*G_string_pixel_width)(const void *s);
extern int (*G_draw_string)(const void *one_line_of_text, double LLx,
                            double LLy);
extern int (*G_draw_text)(int num_lines_of_text,
                          const void *lines_of_text, // an array of pointers
                          double startx, double starty, double height,
                          double x_over_y_ratio,
                          double extra_space_between_letters_fraction,
                          double extra_space_between_lines_fraction);
extern int (*G_save_image_to_file)(const void *filename);
extern int (*G_get_image_from_file)(const void *filename, double x, double y);
extern int (*G_get_pixel)(double x, double y);
extern int (*G_get_pixel_SAFE)(double x, double y, int pixel[1]);
extern int (*G_convert_pixel_to_rgbI)(int pixel, int rgbI[3]);
extern int (*G_convert_rgbI_to_rgb)(int rgbI[3], double rgb[3]);

#endif // FPToolkit_H
