#include "main.h"

#define KEY_REDUX 'q'

int main(void)
{
    int keydown = 0;
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    double negative_number = -3.3;
    printf("Negative number: %f\n", negative_number);
    // Make a percentage (-0.3) is positive 0.7
    negative_number = fmod(negative_number, 1.0);
    printf("Negative number after fmod: %f\n", negative_number);


    while (keydown != KEY_REDUX)
    {
        keydown = G_wait_key();
        printf("Key pressed: %d\n", keydown);
    }

    return EXIT_SUCCESS;
}
