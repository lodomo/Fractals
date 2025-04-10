#include "main.h"

int main(void) {
    int keydown = 0;
    double depth = U_get_double("Enter the depth of the fractal: ");

    Color draw_color = C_rgb_from_hex(SM32_SEA_FOAM);

    Point start_points[3];
    start_points[0].x = 100;
    start_points[0].y = 100;

    start_points[1].x = 900;
    start_points[1].y = 100;

    start_points[2].x = 500;
    start_points[2].y = 900;


    G_init_graphics(1000, 1000);
    sierpinsky(start_points[0], start_points[1], start_points[2], depth, draw_color);
    G_wait_key();

    return EXIT_SUCCESS;
}

void sierpinsky(Point a, Point b, Point c, double depth, Color color) {
    if (depth == 0) {
        G_rgb(color.r, color.g, color.b);
        G_fill_triangle(a.x, a.y, b.x, b.y, c.x, c.y);
        return;
    }

    Point ab = { (a.x + b.x) / 2, (a.y + b.y) / 2 };
    Point ac = { (a.x + c.x) / 2, (a.y + c.y) / 2 };
    Point bc = { (b.x + c.x) / 2, (b.y + c.y) / 2 };

    sierpinsky(a, ab, ac, depth - 1, color);
    sierpinsky(b, ab, bc, depth - 1, color);
    sierpinsky(c, ac, bc, depth - 1, color);
}
