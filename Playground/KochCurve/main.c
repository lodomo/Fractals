#include "main.h"

int main(void) {
    int keydown = 0;
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    Line line;
    line.start.x = 100;
    line.start.y = 300;
    line.end.x = 900;
    line.end.y = 300;

    G_rgb(0.0, 0.0, 0.0);
    G_clear();
    koch_curve(line, 0);

    while (keydown != 'q') {
        keydown = G_wait_key();
    }

    return EXIT_SUCCESS;
}

void koch_curve(Line line, int depth) {
    // NOTES
    //
    // Line segment, rotate 60 degrees, rotate -120 degrees, line segment.
    // _/\_
    // Then depth is treating each sub-line as a new line
    //              p2
    //             /  \
    //    ______  /    \ ____
    //   p0     p1     p3   p4

    Point p[5];

    p[0] = line.start;
    p[1] = U_get_pos_by_percent(line, 0.33);
    p[2] = U_get_pos_by_percent(line, 0.5);
    p[3] = U_get_pos_by_percent(line, 0.66);
    p[4] = line.end;

    double line_angle = U_get_angle_rad(line);
    double line_perp = U_perp_angle(line_angle);
    double line_len = U_line_length(line);
    double height = U_eq_triangle_height(line_len / 3); // 3 segments

    p[2] = U_shift_point(p[2], line_perp, height);

    // p2 needs to shift perpendicular to the line at the height of the
    // triangle.

    // Draw all the lines
    Color green = C_rgb_from_hex(SM32_GREEN_3);
    G_rgb(green.r, green.g, green.b);
    for (int i = 0; i < 4; i++) {
        G_line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
    }
}

double line_angle(Line line) {
    return atan2(line.end.y - line.start.y, line.end.x - line.start.x);
}

Point midpoint(Point p1, Point p2) {
    Point mid;
    mid.x = (p1.x + p2.x) / 2;
    mid.y = (p1.y + p2.y) / 2;
    return mid;
}
