#include "main.h"

int main(void)
{
    int keydown = 0;
    int screen_size = 1000;
    int draw_click_radius = 5;
    int line_resolution = 1000;
    int line_thickness = 1;
    double vector[2] = {0, 0};
    double normalized[2] = {0, 0};
    double length = 0.0;
    G_init_graphics(screen_size, screen_size);

    while (keydown != 'q')
    {
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

        vector[0] = click_b[0] - click_a[0];
        vector[1] = click_b[1] - click_a[1];

        length = sqrt(vector[0] * vector[0] + vector[1] * vector[1]);

        normalized[0] = vector[0] / length;
        normalized[1] = vector[1] / length;

        for (int i = 0; i < length; i += 1)
        {
            double x = click_a[0] + normalized[0] * i;
            double y = click_a[1] + normalized[1] * i;
            G_rgb(0, 1, 0);
            G_point(x, y);
        }

        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}
