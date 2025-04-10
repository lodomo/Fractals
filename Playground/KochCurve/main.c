#include "main.h"

int main(void)
{
    int keydown = 0;
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    Line line;
    line.start.x = 100;
    line.start.y = 100;
    line.end.x = 900;
    line.end.y = 100;

    koch_curve(line, 0);

    while (keydown != 'q')
    {
        G_rgb(1, 1, 1);
        G_clear();

        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}

void koch_curve(Line line, int depth)
{
    double angle = line_angle(line);
    printf("angle: %f\n", angle);
    // NOTES
    //
    // Line segment, rotate 60 degrees, rotate -120 degrees, line segment.
    // _/\_
    // Then depth is treating each sub-line as a new line
}

double line_angle(Line line)
{
    return atan2(line.end.y - line.start.y, line.end.x - line.start.x);
}
