#include "main.h"

int main(void)
{
    int keydown = 0;
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    double clicks[2][2];


    while (keydown != 'q')
    {
        U_set_draw_color_hex(SM32_BLACK);
        G_clear();
        G_wait_click(clicks[0]);

        U_set_draw_color_hex(SM32_RED);
        G_circle(clicks[0][0], clicks[0][1], 10);

        G_wait_click(clicks[1]);
        U_set_draw_color_hex(SM32_RED);
        G_circle(clicks[1][0], clicks[1][1], 10);


        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}
