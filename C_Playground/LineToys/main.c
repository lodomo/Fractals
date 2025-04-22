#include "main.h"

int main(void) {
    int keydown = 0;
    int screen_size = 1000;
    int draw_click_radius = 5;
    int line_resolution = 1000;
    int line_thickness = 1;
    double vector[2] = {0, 0};
    double normalized[2] = {0, 0};
    double length = 0.0;
    G_init_graphics(screen_size, screen_size);

    while (keydown != 'q') {
        G_rgb(0, 0, 0);
        G_clear();

        double click_a[2] = {0, 0};
        double click_b[2] = {0, 0};

        G_rgb(1, 0, 0);
        G_wait_click(click_a);
        G_fill_circle(click_a[0], click_a[1], draw_click_radius);
        G_rgb(1, 0, 0);
        G_wait_click(click_b);
        G_fill_circle(click_b[0], click_b[1], draw_click_radius);

        G_rgb(0, 0, 1);
        Point twenty_five =
            U_get_pos_by_percent((Line){.start = {click_a[0], click_a[1]},
                                        .end = {click_b[0], click_b[1]}},
                                 0.25);
        Point fifty =
            U_get_pos_by_percent((Line){.start = {click_a[0], click_a[1]},
                                        .end = {click_b[0], click_b[1]}},
                                 0.5);

        Point seventy_five =
            U_get_pos_by_percent((Line){.start = {click_a[0], click_a[1]},
                                        .end = {click_b[0], click_b[1]}},
                                 0.75);

        G_fill_circle(twenty_five.x, twenty_five.y, draw_click_radius);
        G_fill_circle(fifty.x, fifty.y, draw_click_radius);
        G_fill_circle(seventy_five.x, seventy_five.y, draw_click_radius);

        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}
