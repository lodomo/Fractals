#include "main.h"

int main(void)
{
    int keydown = 0;
    G_init_graphics(1000, 1000);

    while (keydown != 'q')
    {
        G_rgb(1, 1, 1);
        G_clear();

        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}
