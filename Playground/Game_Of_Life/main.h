#ifndef MAIN_H
#define MAIN_H

#include "./srcs/LDM_Utils.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define RUN_BUTTON_WIDTH 100

typedef struct Colors {
    Color lines;
    Color run;
    Color plant;
    Color dead;
    Color dying;
    Color born;
    Color alive;
} Colors;

void print_grid(double **grid, double grid_n);
void draw_init_blocks(Colors colors, double **grid, double grid_n,
                      double grid_size);
void draw_lines(Color color, double grid_n, double grid_size);
int add_life(double mouse_pos[2], double **grid, double grid_n,
             double grid_size);
void copy_grid(double **src, double **dest, double grid_n);

double sum_neighbors(double **grid, int x, int y);
void set_neighbors(double **grid, double **neighbors, double grid_n,
                   double grid_size);
void update_grid(double **grid, double **neighbors, double grid_n);
void draw_sim_blocks(Colors colors, double **grid, double **old_grid,
                     double grid_n, double grid_size);
#endif // MAIN_H
