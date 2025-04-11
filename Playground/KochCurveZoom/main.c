#include "main.h"

int main(void) {
    int keydown = 0;
    int depth = 8;
    double step = 1.01;
    Box draw_boundary;
    draw_boundary.p[0] = (Point){0, 0};
    draw_boundary.p[1] = (Point){SCREEN_WIDTH, SCREEN_HEIGHT};

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
    koch_curve(draw_color, &first_line, draw_boundary);

    /*
    double click[2];
    U_set_draw_color_hex(SM32_RED);
    G_wait_click(click);
    Point click_point = {click[0], click[1]};
    */

    // All this to shift p2
    Point apex = U_line_pos_by_percent(first_line, 0.5);
    double line_angle = U_line_angle(first_line);
    double line_perp = U_perp_angle(line_angle);
    double line_len = U_line_length(first_line);
    double height = U_eq_triangle_height(line_len / 3); // 3 segments
    U_shift_point(&apex, line_perp, height);
    Point click_point;
    click_point.x = apex.x;
    click_point.y = apex.y;

    while (keydown != 'q') {
        U_set_draw_color_hex(SM32_BLACK);
        G_clear();

        /*
        U_set_draw_color_hex(SM32_RED);
        G_rectangle(draw_boundary.p[0].x, draw_boundary.p[0].y,
                     draw_boundary.p[1].x - draw_boundary.p[0].x,
                     draw_boundary.p[1].y - draw_boundary.p[0].y);
        */

        koch_curve(draw_color, &first_line, draw_boundary);
        //keydown = G_wait_key();

        // Scale from the click point
        first_line.p[0] =
            U_point_pos_by_percent(click_point, first_line.p[0], step);
        first_line.p[1] =
            U_point_pos_by_percent(click_point, first_line.p[1], step);

        usleep(10000);
        U_draw_buffer();
    }

    return EXIT_SUCCESS;
}

int koch_curve(Color color, Line *line, Box draw_boundary) {
    // NOTES
    //             p2
    //            /  \
    //    ______ /    \ ____
    //   p0     p1     p3   p4
    //   Offset p2 as if an equilateral triangle
 
    // This is wasteful of memory, I'll optimize if it needs it.
    static const int draw_width = 5;
    Point p[5];

    p[0] = line->p[0];
    p[1] = U_line_pos_by_percent(*line, 0.33);
    p[2] = U_line_pos_by_percent(*line, 0.5);
    p[3] = U_line_pos_by_percent(*line, 0.66);
    p[4] = line->p[1];

    // All this to shift p2
    double line_angle = U_line_angle(*line);
    double line_perp = U_perp_angle(line_angle);
    double line_len = U_line_length(*line);
    double height = U_eq_triangle_height(line_len / 3); // 3 segments
    U_shift_point(&p[2], line_perp, height);


    // Draw the deepest level of recursion I think I can do this better.
    if (line_len < draw_width) {
        U_set_draw_color(color);
        for (int i = 0; i < 4; i++) {
            G_line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
        }
        return 0;
    }

    Line lines[4];
    lines[0] = (Line){p[0], p[1]};
    lines[1] = (Line){p[1], p[2]};
    lines[2] = (Line){p[2], p[3]};
    lines[3] = (Line){p[3], p[4]};

    int inside_count = 0;
    int inside_index = -1;
    for (int i = 0; i < 4; i++) {
        if (U_line_intersect_box(lines[i], draw_boundary) == CLIP_INSIDE) {
            koch_curve(color, &lines[i], draw_boundary);
            inside_count++;
            inside_index = i;
        }
    }

    if (inside_count == 1) {
        // This is the first call, and there's only one line inside the box.
        // Change it to the whole line.
        line->p[0] = lines[inside_index].p[0];
        line->p[1] = lines[inside_index].p[1];
    }

    // Figure out if the line is inside the draw boundary and if it should
    // recurse further down.
    return 0;
}
