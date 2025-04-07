#include "LDM_Utils.h"

typedef struct Color {
    double r;
    double g;
    double b;
} Color;

int main() {
    int keydown;
    double screen_width, screen_height, run_box_width;
    double cell_count;

    keydown = 0;
    run_box_width = 50;
    screen_width = 1000;
    screen_height = 1000;

    printf("How many cells wide?");
    scanf("%lf", &cell_count);
    printf("Cell count: %lf\n", cell_count);

    G_init_graphics(screen_width + run_box_width, screen_height);
    G_rgb(0, 0, 0);
    G_clear();

    while (keydown != 'q') {
        // Draw a rectangle at the right side of the screen, width of run_box
        G_rgb(0.5, 0.5, 0.5);
        G_fill_rectangle(screen_width, 0, run_box_width, screen_height);
        G_rgb(1, 1, 1);
        keydown = G_wait_key();
    }

    return 0;
}
