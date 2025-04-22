#include "main.h"

int main(void) {
    int keydown = 0;
    int depth = 0;
    Color draw_color = U_init_color_hex(SM32_BLUE_2);
    printf("Instructions:\n"
           "Click 3 points to define a triangle.\n"
           "Press 'q' to quit.\n"
           "Press any other key to increase the depth of the Koch curve.\n");
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    U_set_draw_color_hex(SM32_BLACK);
    G_clear();

    double clicks[3][2];
    for (int i = 0; i < 3; i++) {
        U_set_draw_color_hex(SM32_RED);
        G_wait_click(clicks[i]);
        G_fill_circle(clicks[i][0], clicks[i][1], 2);
    }

    Line lines[3];
    for (int i = 0; i < 3; i++) {
        lines[i].p0.x = clicks[i][0];
        lines[i].p0.y = clicks[i][1];
        lines[i].p1.x = clicks[(i + 1) % 3][0];
        lines[i].p1.y = clicks[(i + 1) % 3][1];
    }

    while (keydown != 'q') {
        U_set_draw_color_hex(SM32_BLACK);
        G_clear();
        for (int i = 0; i < 3; i++) {
            koch_curve(draw_color, lines[i], depth);
        }
        keydown = G_wait_key();
        depth++;
    }

    return EXIT_SUCCESS;
}

void koch_curve(Color color, Line line, int depth) {
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

    p[0] = line.p0;
    p[1] = U_line_pos_by_percent(line, 0.33);
    p[2] = U_line_pos_by_percent(line, 0.5);
    p[3] = U_line_pos_by_percent(line, 0.66);
    p[4] = line.p1;

    double line_angle = U_get_angle_rad(line);
    double line_perp = U_perp_angle(line_angle);
    double line_len = U_line_length(line);
    double height = U_eq_triangle_height(line_len / 3); // 3 segments

    U_shift_point(&p[2], line_perp, height);

    if (depth == 0) {
        U_set_draw_color(color);
        for (int i = 0; i < 4; i++) {
            G_line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
        }
        return;
    }

    koch_curve(color, (Line){p[0], p[1]}, depth - 1);
    koch_curve(color, (Line){p[1], p[2]}, depth - 1);
    koch_curve(color, (Line){p[2], p[3]}, depth - 1);
    koch_curve(color, (Line){p[3], p[4]}, depth - 1);
}
