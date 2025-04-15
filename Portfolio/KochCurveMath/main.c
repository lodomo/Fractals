#include "main.h"

int main(void) {
    int keydown = 0;
    Color color = U_color_hex(SM32_GRAY);
    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    U_set_color_hex(SM32_BLACK);

    Line start_line;
    start_line.p[0].x = 200;
    start_line.p[0].y = 300;
    start_line.p[1].x = 800;
    start_line.p[1].y = 700;

    while (keydown != 'q') {
        U_set_color_hex(SM32_BLACK);
        G_clear();
        KochCurve first_curve = init_curve(&start_line);
        U_draw_dotted_line(&color, &start_line);
        keydown = G_wait_key();

        // Draw the big triangle
        Point big_right_angle;
        big_right_angle.x = start_line.p[1].x;
        big_right_angle.y = start_line.p[0].y;
        Line big_run = {start_line.p[0], big_right_angle};
        Line big_rise = {big_right_angle, start_line.p[1]};
        U_draw_dotted_line(&color, &big_run);
        keydown = G_wait_key();
        U_draw_dotted_line(&color, &big_rise);
        keydown = G_wait_key();
        G_rectangle(big_right_angle.x - 25, big_right_angle.y + 1, 25, 25);
        keydown = G_wait_key();

        Point small_right_angle;
        Point one_third = U_lerp_line(&start_line, 1.0 / 3.0);
        Point two_thirds = U_lerp_line(&start_line, 2.0 / 3.0);
        U_set_color_hex(SM32_RED);
        G_fill_circle(one_third.x, one_third.y, 3);
        keydown = G_wait_key();
        G_fill_circle(two_thirds.x, two_thirds.y, 3);
        keydown = G_wait_key();
        small_right_angle.x = two_thirds.x;
        small_right_angle.y = one_third.y;
        Line run = {one_third, small_right_angle};
        Line rise = {small_right_angle, two_thirds};
        U_draw_dotted_line(&color, &run);
        keydown = G_wait_key();
        U_draw_dotted_line(&color, &rise);
        keydown = G_wait_key();
        G_rectangle(small_right_angle.x - 25, small_right_angle.y + 1, 25, 25);
        keydown = G_wait_key();

        Point perp_right_angle;
        Point center = U_lerp(&start_line.p[0], &start_line.p[1], 0.5);
        U_set_color_hex(SM32_BLUE_1);
        G_fill_circle(center.x, center.y, 3);
        keydown = G_wait_key();
        perp_right_angle.x = center.x;
        perp_right_angle.y = center.y + U_length(&run);
        Line perp_rise;
        Line perp_run;
        perp_rise.p[0] = center;
        perp_rise.p[1] = perp_right_angle;

        perp_run.p[0] = perp_right_angle;
        perp_run.p[1].x = perp_right_angle.x - U_length(&rise);
        perp_run.p[1].y = perp_right_angle.y;

        U_draw_dotted_line(&color, &perp_rise);
        keydown = G_wait_key();
        U_draw_dotted_line(&color, &perp_run);
        keydown = G_wait_key();

        G_rectangle(perp_right_angle.x - 25, perp_right_angle.y - 24, 25, 25);
        keydown = G_wait_key();

        Line perp_line;
        perp_line.p[0] = center;
        perp_line.p[1] = perp_run.p[1];
        U_draw_dotted_line(&color, &perp_line);
        keydown = G_wait_key();

        Point final_point;
        final_point = U_lerp(&center, &perp_line.p[1], SQRT_3_OVER_2);
        U_set_color_hex(SM32_BLUE_2);
        G_fill_circle(final_point.x, final_point.y, 3);
        keydown = G_wait_key();



        draw_curve(&first_curve);
        keydown = G_wait_key();
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
    U_set_color_hex(SM32_WHITE);
    for (int i = 0; i < 4; i++) {
        G_line(curve->p[i].x, curve->p[i].y, curve->p[i + 1].x,
               curve->p[i + 1].y);
    }
}
