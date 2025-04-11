#include "main.h"

int main(void) {
    int keydown = 0;

    double draw_padding = 200.0;
    Box draw_boundary;
    draw_boundary.p[0] = (Point){-draw_padding, -draw_padding};
    draw_boundary.p[1] =
        (Point){SCREEN_WIDTH + draw_padding, SCREEN_HEIGHT + draw_padding};

    int depth = 7;
    double step = 1.01;

    Color draw_color = U_init_color_hex(SM32_BLUE_2);
    printf("Instructions:\n"
           "Click where you want to zoom in.\n");

    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    U_set_draw_color_hex(SM32_BLACK);
    G_clear();

    Line first_line = {0, (int)(SCREEN_HEIGHT / 3), SCREEN_WIDTH,
                       (int)(SCREEN_HEIGHT / 3)};
    Point main_points[5];

    U_set_draw_color_hex(SM32_BLACK);
    G_clear();
    koch_curve(draw_color, &first_line, depth, draw_boundary);

    double click[2];
    U_set_draw_color_hex(SM32_RED);
    G_wait_click(click);
    Point click_point = {click[0], click[1]};

    while (keydown != 'q') {
        U_set_draw_color_hex(SM32_BLACK);
        G_clear();
        koch_curve(draw_color, &first_line, depth, draw_boundary);
        keydown = G_wait_key();

        // Scale from the click point
        first_line.p[0] =
            U_point_pos_by_percent(click_point, first_line.p[0], step);
        first_line.p[1] =
            U_point_pos_by_percent(click_point, first_line.p[1], step);
    }

    return EXIT_SUCCESS;
}

int koch_curve(Color color, Line *line, int depth, Box draw_boundary) {
    // NOTES
    //
    // Line segment, rotate 60 degrees, rotate -120 degrees, line segment.
    // _/\_
    // Then depth is treating each sub-line as a new line
    //              p2
    //             /  \
    //    ______  /    \ ____
    //   p[0]     p[1]     p3   p4

    Point p[5];

    p[0] = line->p[0];
    p[1] = U_line_pos_by_percent(*line, 0.33);
    p[2] = U_line_pos_by_percent(*line, 0.5);
    p[3] = U_line_pos_by_percent(*line, 0.66);
    p[4] = line->p[1];

    double line_angle = U_get_angle_rad(*line);
    double line_perp = U_perp_angle(line_angle);
    double line_len = U_line_length(*line);
    double height = U_eq_triangle_height(line_len / 3); // 3 segments

    U_shift_point(&p[2], line_perp, height);

    if (depth == 0) {
        U_set_draw_color(color);
        for (int i = 0; i < 4; i++) {
            G_line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
        }
        return 0;
    }

    Line lines[4];
    int lines_drawn = 0;
    int index_drawn = -1;
    lines[0] = (Line){p[0], p[1]};
    lines[1] = (Line){p[1], p[2]};
    lines[2] = (Line){p[2], p[3]};
    lines[3] = (Line){p[3], p[4]};

    for (int i = 0; i < 4; i++) {
        ClipCode code = U_line_intersect_box(lines[i], draw_boundary);

        if (code == CLIP_INSIDE) {
            lines_drawn++;
            index_drawn = i;
            koch_curve(color, &lines[i], depth - 1, draw_boundary);
        }
    }

    if (lines_drawn == 1 && index_drawn != -1) {
        line->p[0] = lines[index_drawn].p[0];
        line->p[1] = lines[index_drawn].p[1];
    }

    if (depth == 7) {
        printf("Lines drawn: %d\n", lines_drawn);
    }
    return 0;
}
