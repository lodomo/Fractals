#include "main.h"

int main(void)
{
    int keydown = 0;
    double screen_size = 800;
    G_init_graphics(screen_size, screen_size);

    double r = 350;
    double offset = screen_size / 2;
    double step = 10;

    while (keydown != 'q')
    {
        G_rgb(0, 0, 0);
        G_clear();


        for(double x = -r; x <= r; x+= r/step)
        {
            double uy = sqrt(r*r - x*x);
            double ly = -uy;

            G_rgb(1, 1, 1);
            G_fill_circle(x + offset, uy + offset, 2);
            G_fill_circle(x + offset, ly + offset, 2);

            G_rgb(0, 0, 1);
            G_line(x + offset, uy + offset, x + offset, ly + offset);
            printf("%f %f\n", x, uy);
            printf("%f %f\n", x, ly);
        }

        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}
