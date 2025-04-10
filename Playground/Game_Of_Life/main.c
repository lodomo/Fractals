#include "main.h"

int main(void) {
    int keydown = 0;
    double mouse_pos[2] = {0, 0};
    double grid_n;
    double grid_size;
    double **last_grid;
    double **grid;
    double **neighbors;

    Colors colors;
    colors.lines = C_rgb_from_hex(SM32_GRAY);
    colors.run = C_rgb_from_hex(SM32_DARK_GRAY);
    colors.plant = C_rgb_from_hex(SM32_WHITE);
    colors.dead = C_rgb_from_hex(SM32_BLACK);
    colors.dying = C_rgb_from_hex(SM32_RED);
    colors.born = C_rgb_from_hex(SM32_SOFT_GREEN);
    colors.alive = C_rgb_from_hex(SM32_BLUE_2);

    printf("Welcome to the Game of Life!\n");
    grid_n = U_get_double("Please enter the grid size: ");

    last_grid = U_create_sq_matrix(grid_n + 2); // +2 for padding
    grid = U_create_sq_matrix(grid_n + 2);      // +2 for padding
    neighbors = U_create_sq_matrix(grid_n + 2); // +2 for padding

    grid_size = SCREEN_WIDTH / grid_n;

    G_init_graphics(SCREEN_WIDTH + RUN_BUTTON_WIDTH, SCREEN_HEIGHT);
    G_rgb(0, 0, 0);
    G_clear();

    // Draw the run button
    G_rgb(colors.run.r, colors.run.g, colors.run.b);
    G_fill_rectangle(SCREEN_WIDTH, 0, RUN_BUTTON_WIDTH, SCREEN_HEIGHT);

    printf("Click on the grid to toggle cells.\n");
    printf("Click on the right edge to run the simulation.\n");
    while (1) {
        draw_init_blocks(colors, grid, grid_n, grid_size);
        draw_lines(colors.lines, grid_n, grid_size);

        // If the mouse is clicked on the edge run the simulation
        G_wait_click(mouse_pos);
        if (!add_life(mouse_pos, grid, grid_n, grid_size)) {
            break;
        }
    }

    printf("Running the simulation...\n");
    printf("Hold any key -except- 'q' to step through generations.\n");
    printf("Press 'q' to quit.\n");

    while (keydown != 'q') {
        copy_grid(grid, last_grid, grid_n); // Make a copy of the last state
        set_neighbors(grid, neighbors, grid_n, grid_size);
        update_grid(grid, neighbors, grid_n);
        draw_sim_blocks(colors, grid, last_grid, grid_n, grid_size);
        keydown = G_wait_key();
    }

    // Free the grid
    U_free_sq_matrix(grid, grid_n + 2);
    U_free_sq_matrix(neighbors, grid_n + 2);
    return EXIT_SUCCESS;
}

// Print the grid, skipping the padding
void print_grid(double **grid, double grid_n) {
    for (int i = grid_n - 1; i > -1; --i) {
        for (int j = 0; j < grid_n; ++j) {
            printf("%d ", (int)grid[i + 1][j + 1]);
        }
        printf("\n");
    }
}

// Draw the grid blocks
void draw_init_blocks(Colors colors, double **grid, double grid_n,
                      double grid_size) {
    for (int i = 0; i < grid_n; i++) {
        for (int j = 0; j < grid_n; j++) {
            if (grid[i + 1][j + 1] == 1) {
                U_set_color(colors.plant);
            } else {
                U_set_color(colors.dead);
            }
            G_fill_rectangle(i * grid_size, j * grid_size, grid_size,
                             grid_size);
        }
    }
}

// Draw the grid lines
void draw_lines(Color color, double grid_count, double grid_size) {
    U_set_color(color);
    for (int i = 0; i < grid_count; i++) {
        G_line(i * grid_size, 0, i * grid_size, SCREEN_HEIGHT);
        G_line(0, i * grid_size, SCREEN_WIDTH, i * grid_size);
    }
}

// Put a cell in the grid
int add_life(double mouse_pos[2], double **grid, double grid_n,
             double grid_size) {
    if (mouse_pos[0] > SCREEN_WIDTH || mouse_pos[1] > SCREEN_HEIGHT) {
        return 0;
    }

    int x = (int)(mouse_pos[0] / grid_size);
    int y = (int)(mouse_pos[1] / grid_size);
    grid[x + 1][y + 1] = !grid[x + 1][y + 1];
    return 1;
}

void copy_grid(double **src, double **dest, double grid_n) {
    for (int i = 0; i < grid_n; ++i) {
        for (int j = 0; j < grid_n; ++j) {
            dest[i + 1][j + 1] = src[i + 1][j + 1];
        }
    }
}

// Sum the neighbors of a cell
double sum_neighbors(double **grid, int x, int y) {
    double sum = 0.0;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            sum += grid[x + i][y + j];
        }
    }

    sum -= grid[x][y];
    return sum;
}

// Set the neighbors of a cell in parallel grid
void set_neighbors(double **grid, double **neighbors, double grid_n,
                   double grid_size) {
    for (int i = 0; i < grid_n; ++i) {
        for (int j = 0; j < grid_n; ++j) {
            neighbors[i + 1][j + 1] = sum_neighbors(grid, i + 1, j + 1);
        }
    }
}

void update_grid(double **grid, double **neighbors, double grid_n) {
    for (int i = 0; i < grid_n; ++i) {
        for (int j = 0; j < grid_n; ++j) {
            if (grid[i + 1][j + 1] == 1) {
                if (neighbors[i + 1][j + 1] < 2 ||
                    neighbors[i + 1][j + 1] > 3) {
                    grid[i + 1][j + 1] = 0;
                }
            } else {
                if (neighbors[i + 1][j + 1] == 3) {
                    grid[i + 1][j + 1] = 1;
                }
            }
        }
    }
}

// Draw the blocks of the simulation
void draw_sim_blocks(Colors colors, double **grid, double **old_grid,
                     double grid_n, double grid_size) {
    for (int i = 0; i < grid_n; i++) {
        for (int j = 0; j < grid_n; j++) {
            if (grid[i + 1][j + 1] == 1) {
                // If this cell is alive, was it alive last time?
                if (old_grid[i + 1][j + 1] == 1) {
                    U_set_color(colors.alive);
                } else {
                    U_set_color(colors.born);
                }
            } else {
                // If this cell is dead, was it alive last time?
                if (old_grid[i + 1][j + 1] == 1) {
                    U_set_color(colors.dying);
                } else {
                    U_set_color(colors.dead);
                }
            }
            G_fill_rectangle(i * grid_size, j * grid_size, grid_size,
                             grid_size);
        }
    }
}
