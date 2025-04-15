#include "main.h"

int main(void)
{
    int keydown = 0;
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    U_set_color_hex(SM32_BLACK);

    while (keydown != 'q')
    {
        G_clear();

        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}
