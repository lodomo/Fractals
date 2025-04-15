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

        U_set_color_hex(SM32_WHITE);
        U_draw_dotted_line(&start_line, 1.0, 10);
        keydown = G_wait_key();

        Point big_right_angle;
        big_right_angle = U_right_angle_reverse(&start_line);
        Line big_rise;
        big_rise.p[0] = big_right_angle;
        big_rise.p[1] = start_line.p[1];

        Line big_run;
        big_run.p[0] = start_line.p[0];
        big_run.p[1] = big_right_angle;

        U_set_color_hex(SM32_SEA_FOAM);
        U_draw_dotted_line(&big_run, 1.0, 10);
        keydown = G_wait_key();

        U_set_color_hex(SM32_ORANGE);
        U_draw_dotted_line(&big_rise, 1.0, 10);
        keydown = G_wait_key();

        U_set_color_hex(SM32_WHITE);
        G_rectangle(big_right_angle.x - 25, big_right_angle.y + 1, 25, 25);
        keydown = G_wait_key();

        Point one_third = U_lerp_line(&start_line, 1.0 / 3.0);
        Point two_thirds = U_lerp_line(&start_line, 2.0 / 3.0);
        U_set_color_hex(SM32_RED);
        G_fill_circle(one_third.x, one_third.y, 5);
        keydown = G_wait_key();
        G_fill_circle(two_thirds.x, two_thirds.y, 5);
        keydown = G_wait_key();


        Point small_right_angle;
        Line base_line = {one_third, two_thirds};
        small_right_angle = U_right_angle_reverse(&base_line);

        Line small_rise;
        small_rise.p[0] = small_right_angle;
        small_rise.p[1] = two_thirds;

        Line small_run;
        small_run.p[0] = one_third;
        small_run.p[1] = small_right_angle;

        U_set_color_hex(SM32_SOFT_GREEN);
        U_draw_dotted_line(&small_run, 1.0, 10);
        keydown = G_wait_key();
        U_set_color_hex(SM32_YELLOW);
        U_draw_dotted_line(&small_rise, 1.0, 10);
        keydown = G_wait_key();

        U_set_color_hex(SM32_WHITE);
        G_rectangle(small_right_angle.x - 25, small_right_angle.y + 1, 25, 25);
        keydown = G_wait_key();

        Point center = U_lerp_line(&start_line, 0.5);
        U_set_color_hex(SM32_BLUE_1);
        G_fill_circle(center.x, center.y, 5);
        keydown = G_wait_key();

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

        U_set_color_hex(SM32_SOFT_GREEN);
        U_draw_dotted_line(&perp_rise, 1.0, 10);
        keydown = G_wait_key();

        U_set_color_hex(SM32_YELLOW);
        U_draw_dotted_line(&perp_run, 1.0, 10);
        keydown = G_wait_key();

        U_set_color_hex(SM32_WHITE);
        G_rectangle(perp_right_angle.x - 25, perp_right_angle.y - 24, 25, 25);
        keydown = G_wait_key();

        U_set_color_hex(SM32_WHITE);
        Line perp_line;
        perp_line.p[0] = center;
        perp_line.p[1] = perp_run.p[1];
        U_draw_dotted_line(&perp_line, 1.0, 10);
        keydown = G_wait_key();

        U_set_color_hex(SM32_BLUE_1);
        G_fill_circle(perp_line.p[1].x, perp_line.p[1].y, 5);
        keydown = G_wait_key();

        U_set_color_hex(SM32_RED);
        Point final_point;
        final_point = U_lerp(&perp_line.p[0], &perp_line.p[1], SQRT_3_OVER_2);
        G_fill_circle(final_point.x, final_point.y, 5);

        U_set_color_hex(SM32_WHITE);
        Line one_third_line;
        one_third_line.p[0] = one_third;
        one_third_line.p[1] = final_point;
        U_draw_dotted_line(&one_third_line, 1.0, 10);
        keydown = G_wait_key();
        Line two_thirds_line;
        two_thirds_line.p[0] = two_thirds;
        two_thirds_line.p[1] = final_point;
        U_draw_dotted_line(&two_thirds_line, 1.0, 10);
        keydown = G_wait_key();


        draw_curve(&first_curve);
        keydown = G_wait_key();

        U_set_color_hex(SM32_BLACK);
        G_clear();

        U_set_color_hex(SM32_WHITE);
        draw_curve(&first_curve);
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
    for (int i = 0; i < 4; i++) {
        G_line(curve->p[i].x, curve->p[i].y, curve->p[i + 1].x,
               curve->p[i + 1].y);
    }
    return 1;
}
