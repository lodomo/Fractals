#include "main.h"

int main(void) {
    char title[50];
    sprintf(title, "KochCurve");
    int frame = 0;

    int keydown = 0;
    Color color = U_color_hex(SM32_GRAY);
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    U_set_color_hex(SM32_BLACK);

    Box draw_boundary;
    draw_boundary.p[0].x = -500;
    draw_boundary.p[0].y = -500;
    draw_boundary.p[1].x = SCREEN_WIDTH + 500;
    draw_boundary.p[1].y = SCREEN_HEIGHT + 500;

    Line start_line;
    start_line.p[0].x = 200;
    start_line.p[0].y = 300;
    start_line.p[1].x = SCREEN_WIDTH - 200;
    start_line.p[1].y = SCREEN_HEIGHT - 300;

    KochCurve base_curve = init_curve(&start_line);


    U_set_color_hex(SM32_BLACK);
    G_clear();

    paint_math(&start_line, &frame);

    for (int i = 2; i < 7; i++) {
        U_scale_line(&start_line, &base_curve.p[2], 1.05);
        U_set_color_hex(SM32_BLACK);
        G_clear();
        U_set_color_hex(SM32_BLUE_1);
        koch_by_depth(&start_line, i);
        char text[100];
        sprintf(text, "Koch Curve Depth %d", i);
        G_draw_string(text, start_line.p[0].x, start_line.p[0].y - 20);
        keydown = G_wait_key();
    }

    KochCurve current_curve = init_curve(&start_line);
    Line base_lines[4];
    base_lines[0].p[0] = current_curve.p[0];
    base_lines[0].p[1] = current_curve.p[1];
    base_lines[1].p[0] = current_curve.p[1];
    base_lines[1].p[1] = current_curve.p[2];
    base_lines[2].p[0] = current_curve.p[2];
    base_lines[2].p[1] = current_curve.p[3];
    base_lines[3].p[0] = current_curve.p[3];
    base_lines[3].p[1] = current_curve.p[4];
    int depth_count = 7;
    char text[100];
    
    while (1) {
        U_set_color_hex(SM32_BLACK);
        G_clear();
        U_set_color_hex(SM32_BLUE_1);
        int extra_scale = 0;
        for (int i = 0; i < 4; i++) {
            extra_scale = koch_by_length(&base_lines[i], &draw_boundary, TRUE);
            if (extra_scale){
                U_scale_line(&base_lines[i], &base_curve.p[2], 1.02);
            }
            U_scale_line(&base_lines[i], &base_curve.p[2], 1.01);
        }


        if (depth_count < 50) {
            sprintf(text, "Koch Curve Depth %d", depth_count);
            G_draw_string(text, base_lines[0].p[0].x, base_lines[0].p[0].y - 20);
            depth_count++;
        }

        U_draw_buffer();
        usleep(10000);
    }

    return EXIT_SUCCESS;
}

KochCurve init_curve(Line *line) {
    KochCurve curve;
    curve.p[0] = line->p[0];
    curve.p[1] = U_lerp_line(line, 1.0 / 3.0);
    curve.p[2] = U_lerp_line(line, 1.0 / 2.0);
    curve.p[3] = U_lerp_line(line, 2.0 / 3.0);
    curve.p[4] = line->p[1];
    U_lerp_perp(&curve.p[2], &curve.p[1], &curve.p[3], SQRT_3_OVER_2);
    return curve;
}

int draw_curve(KochCurve *curve) {
    for (int i = 0; i < 4; i++) {
        G_line(curve->p[i].x, curve->p[i].y, curve->p[i + 1].x,
               curve->p[i + 1].y);
    }
    return 1;
}

int paint_math(Line *line, int *frame) {
    int keydown = 0;
    double line_weight = 2.0;
    KochCurve first_curve = init_curve(line);

    // Draw the Original Line
    U_set_color_hex(SM32_WHITE);
    U_draw_dotted_line(line, line_weight, 10);
    keydown = G_wait_key();

    // Find the point to make it a right angle triangle
    Point big_right_angle;
    big_right_angle = U_right_angle_reverse(line);
    Line big_rise;
    big_rise.p[0] = big_right_angle;
    big_rise.p[1] = line->p[1];

    Line big_run;
    big_run.p[0] = line->p[0];
    big_run.p[1] = big_right_angle;

    // Draw the Big Run
    U_set_color_hex(SM32_SEA_FOAM);
    U_draw_dotted_line(&big_run, line_weight, 10);
    keydown = G_wait_key();
    G_draw_string("Delta X", big_run.p[0].x, big_run.p[0].y - 20);

    // Draw the Big Rise
    U_set_color_hex(SM32_ORANGE);
    U_draw_dotted_line(&big_rise, line_weight, 10);
    keydown = G_wait_key();
    G_draw_string("Delta Y", big_rise.p[0].x + 20, big_rise.p[0].y);

    // Draw the Right Angle
    U_set_color_hex(SM32_WHITE);
    U_draw_box_on_line(&big_rise, line_weight, 25, 25);
    keydown = G_wait_key();

    // Draw the 1/3 and 2/3 points
    Point one_third = U_lerp_line(line, 1.0 / 3.0);
    Point two_thirds = U_lerp_line(line, 2.0 / 3.0);

    U_set_color_hex(SM32_RED);
    G_fill_circle(one_third.x, one_third.y, 5);
    keydown = G_wait_key();
    G_draw_string("1/3", one_third.x + 20, one_third.y);

    G_fill_circle(two_thirds.x, two_thirds.y, 5);
    keydown = G_wait_key();
    G_draw_string("2/3", two_thirds.x + 20, two_thirds.y);

    // Find the small triangle
    Point small_right_angle;
    Line base_line = {one_third, two_thirds};
    small_right_angle = U_right_angle_reverse(&base_line);

    Line small_rise;
    small_rise.p[0] = small_right_angle;
    small_rise.p[1] = two_thirds;

    Line small_run;
    small_run.p[0] = one_third;
    small_run.p[1] = small_right_angle;

    // Draw the small run
    U_set_color_hex(SM32_SOFT_GREEN);
    U_draw_dotted_line(&small_run, line_weight, 10);
    keydown = G_wait_key();
    G_draw_string("X'", small_run.p[0].x, small_run.p[0].y - 20);

    // Draw the small rise
    U_set_color_hex(SM32_YELLOW);
    U_draw_dotted_line(&small_rise, line_weight, 10);
    keydown = G_wait_key();
    G_draw_string("Y'", small_rise.p[0].x + 20, small_rise.p[0].y);

    // Draw the small right angle
    U_set_color_hex(SM32_WHITE);
    U_draw_box_on_line(&small_rise, line_weight, 25, 25);
    keydown = G_wait_key();

    // Draw the center point of hte line
    Point center = U_lerp_line(line, 0.5);
    U_set_color_hex(SM32_BLUE_1);
    G_fill_circle(center.x, center.y, 5);
    keydown = G_wait_key();
    G_draw_string("Center", center.x + 20, center.y);

    // Calculate everything slowly for debugging purposes
    // Find  the perpendicular line
    Point perp_right_angle;
    perp_right_angle.x = center.x;
    perp_right_angle.y = center.y + U_length(&small_run);

    Line perp_rise;
    perp_rise.p[0] = center;
    perp_rise.p[1] = perp_right_angle;

    Line perp_run;
    perp_run.p[0] = perp_right_angle;
    perp_run.p[1].x = perp_right_angle.x - U_length(&small_rise);
    perp_run.p[1].y = perp_right_angle.y;

    // Draw perp rise
    U_set_color_hex(SM32_SOFT_GREEN);
    U_draw_dotted_line(&perp_rise, line_weight, 10);
    keydown = G_wait_key();
    G_draw_string("X'", perp_rise.p[0].x, perp_rise.p[0].y - 20);

    // Draw perp run
    U_set_color_hex(SM32_YELLOW);
    U_draw_dotted_line(&perp_run, line_weight, 10);
    keydown = G_wait_key();
    G_draw_string("Y'", perp_run.p[0].x + 20, perp_run.p[0].y);

    // Draw the square
    U_set_color_hex(SM32_WHITE);
    U_draw_box_on_line(&perp_run, line_weight, 25, 25);
    keydown = G_wait_key();

    // Get the perpendicular point
    Line perp_line;
    perp_line.p[0] = center;
    perp_line.p[1] = perp_run.p[1];

    // Draw the perpendicular point
    U_set_color_hex(SM32_BLUE_1);
    G_fill_circle(perp_line.p[1].x, perp_line.p[1].y, 5);
    keydown = G_wait_key();

    // Draw the perpendicular line
    U_set_color_hex(SM32_WHITE);
    U_draw_dotted_line(&perp_line, line_weight, 10);
    keydown = G_wait_key();

    // Draw a square from the center point to show the line is truely
    // perpendicular
    Line half_line;
    half_line.p[0] = center;
    half_line.p[1] = line->p[1];
    U_set_color_hex(SM32_WHITE);
    U_draw_box_on_line(&half_line, line_weight, 25, 25);
    keydown = G_wait_key();

    U_set_color_hex(SM32_RED);
    Point final_point;
    final_point = U_lerp(&perp_line.p[0], &perp_line.p[1], SQRT_3_OVER_2);
    G_fill_circle(final_point.x, final_point.y, 5);
    keydown = G_wait_key();

    U_set_color_hex(SM32_WHITE);
    Line one_third_line;
    one_third_line.p[0] = one_third;
    one_third_line.p[1] = final_point;
    U_draw_dotted_line(&one_third_line, line_weight, 10);
    keydown = G_wait_key();

    Line two_thirds_line;
    two_thirds_line.p[0] = two_thirds;
    two_thirds_line.p[1] = final_point;
    U_draw_dotted_line(&two_thirds_line, line_weight, 10);
    keydown = G_wait_key();

    draw_curve(&first_curve);
    keydown = G_wait_key();

    U_set_color_hex(SM32_BLACK);
    G_clear();

    U_set_color_hex(SM32_WHITE);
    draw_curve(&first_curve);
    G_draw_string("Base Koch Curve", first_curve.p[0].x,
                  first_curve.p[0].y - 20);
    keydown = G_wait_key();

    U_set_color_hex(SM32_BLUE_1);
    KochCurve depth1[4];
    for (int i = 0; i < 4; i++) {
        Line line;
        line.p[0] = first_curve.p[i];
        line.p[1] = first_curve.p[i + 1];
        depth1[i] = init_curve(&line);
    }

    for (int i = 0; i < 4; i++) {
        draw_curve(&depth1[i]);
        keydown = G_wait_key();
    }

    U_set_color_hex(SM32_BLACK);
    G_clear();

    U_set_color_hex(SM32_BLUE_1);
    for (int i = 0; i < 4; i++) {
        draw_curve(&depth1[i]);
    }
    G_draw_string("Koch Curve Depth 1", depth1[0].p[0].x,
                  depth1[0].p[0].y - 20);
    keydown = G_wait_key();
    return keydown;
}

int koch_by_depth(Line *line, int depth) {
    KochCurve curve = init_curve(line);

    if (depth == 0) {
        draw_curve(&curve);
        return 1;
    }

    Line new_lines[4];
    for (int i = 0; i < 4; i++) {
        new_lines[i].p[0] = curve.p[i];
        new_lines[i].p[1] = curve.p[i + 1];
        koch_by_depth(&new_lines[i], depth - 1);
    }
    return 1;
}

int koch_by_length(Line *line, Box *draw_boundary, int first) {
    if (U_line_intersect_box(line, draw_boundary) != CLIP_INSIDE) {
        // Make sure this is blasted into space and near instant check
        line->p[0].x = INFINITY;
        line->p[0].y = INFINITY;
        line->p[1].x = INFINITY;
        line->p[1].y = INFINITY;
        return 0;
    }

    KochCurve curve = init_curve(line);
    

    // Draw the deepest level of recursion I think I can do this better.
    if (U_length(line) < DRAW_WIDTH) {
        for (int i = 0; i < 4; i++) {
            G_line(curve.p[i].x, curve.p[i].y, curve.p[i + 1].x,
                   curve.p[i + 1].y);
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
        if (U_line_intersect_box(&lines[i], draw_boundary) == CLIP_INSIDE) {
            koch_by_length(&lines[i], draw_boundary, FALSE);
            inside_count++;
            inside_index = i;
        }
    }

    if (!first) {
        return 0;
    }

    // printf("Inside count: %d\n", inside_count);

    if (inside_count == 1) {
        // This is the first call, and there's only one line inside the box.
        // Change it to the whole line.
        line->p[0] = lines[inside_index].p[0];
        line->p[1] = lines[inside_index].p[1];
        return 1;
    }

    // Figure out if the line is inside the draw boundary and if it should
    // recurse further down.
    return 0;
}
