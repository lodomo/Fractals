#include "main.h"

int main(void) {
    int keydown = 0;
    double step = 1.01;

    Box draw_boundary =
        U_gen_box_by_size((Point){SCREEN_WIDTH / -2.0, SCREEN_HEIGHT / -2.0}, SCREEN_WIDTH + SCREEN_WIDTH/2.0, SCREEN_HEIGHT + SCREEN_HEIGHT/2.0);

    Color draw_color = U_init_color_hex(SM32_BLUE_2);
    Color background_color = U_init_color_hex(SM32_BLACK);

    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    U_set_draw_color_hex(SM32_BLACK);
    G_clear();

    Line first_line = {0, (int)(SCREEN_HEIGHT / 3), SCREEN_WIDTH,
                       (int)(SCREEN_HEIGHT / 3)};

    KochCurve seed_curve;
    gen_koch_curve(first_line, &seed_curve);
    Line lines[4];
    lines[0] = (Line){seed_curve.p[0], seed_curve.p[1]};
    lines[1] = (Line){seed_curve.p[1], seed_curve.p[2]};
    lines[2] = (Line){seed_curve.p[2], seed_curve.p[3]};
    lines[3] = (Line){seed_curve.p[3], seed_curve.p[4]};

    Point scale_point = seed_curve.p[2];

    while (keydown != 'q') {
        U_set_draw_color(background_color);
        U_shift_hsv(&background_color, 0.001, 0, 0);
        G_clear();
        U_set_draw_color_hex(SM32_RED);
        U_shift_hsv(&draw_color, 0.01, 0, 0);
        for (int i = 0; i < 4; i++){
            koch_curve(draw_color, &lines[i], draw_boundary, TRUE);
        }

        for (int i = 0; i < 4; i++) {
            U_scale_line(scale_point, &lines[i], step);
        }

        usleep(1000000 / 60);
        U_draw_buffer();
    }

    return EXIT_SUCCESS;
}

int gen_koch_curve(Line line, KochCurve *curve) {
    //             p2
    //            /  \
    //    ______ /    \ ____
    //   p0     p1     p3   p4
    //   Offset p2 as if an equilateral triangle
    if (curve == NULL) {
        return 0;
    }
    curve->p[0] = line.p[0];
    curve->p[1] = U_line_pos_by_percent(line, (1.0/3.0));
    curve->p[2] = U_line_pos_by_percent(line, 0.5);
    curve->p[3] = U_line_pos_by_percent(line, (2.0/3.0));
    curve->p[4] = line.p[1];

    // All this to shift p2
    double line_angle = U_line_angle(line);
    double line_perp = U_perp_angle(line_angle);
    double line_len = U_line_length(line);
    double height = U_eq_triangle_height(line_len / 3); // 3 segments
    U_shift_point(&curve->p[2], line_perp, height);


    return 1;
}

int koch_curve(Color color, Line *line, Box draw_boundary, int first) {
    if (U_line_intersect_box(*line, draw_boundary) != CLIP_INSIDE) {
        // Make sure this is blasted into space and near instant check
        line->p[0].x = INFINITY;
        line->p[0].y = INFINITY;
        line->p[1].x = INFINITY;
        line->p[1].y = INFINITY;
        return 0;
    }

    static const int draw_width = DRAW_WIDTH;
    KochCurve curve;
    gen_koch_curve(*line, &curve);

    // Draw the deepest level of recursion I think I can do this better.
    if (U_line_length(*line) < draw_width) {
        U_set_draw_color(color);
        for (int i = 0; i < 4; i++) {
            G_line(curve.p[i].x, curve.p[i].y, curve.p[i + 1].x, curve.p[i + 1].y);
        }
        return 0;
    }

    Line lines[4];
    lines[0] = (Line){curve.p[0], curve.p[1]};
    lines[1] = (Line){curve.p[1], curve.p[2]};
    lines[2] = (Line){curve.p[2], curve.p[3]};
    lines[3] = (Line){curve.p[3], curve.p[4]};

    int inside_count = 0;
    int inside_index = -1;
    for (int i = 0; i < 4; i++) {
        if (U_line_intersect_box(lines[i], draw_boundary) == CLIP_INSIDE) {
            koch_curve(color, &lines[i], draw_boundary, FALSE);
            inside_count++;
            inside_index = i;
        }
    }

    if (!first)
    {
        return 0;
    }

    // printf("Inside count: %d\n", inside_count);

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
