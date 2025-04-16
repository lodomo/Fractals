#include "Utils.h"

// ########## COLORS AND DRAWING ##########

/**
 *  Initializes a color from a hex value in the range of 0x000000 to 0xFFFFFF.
 */
Color U_color_hex(int hex) {
    hex = U_clamp_int(hex, 0x000000, 0xFFFFFF);

    Color color;
    color.r = ((hex >> 16) & 0xFF) / 255.0;
    color.g = ((hex >> 8) & 0xFF) / 255.0;
    color.b = (hex & 0xFF) / 255.0;
    C_update_hsv(&color);
    return color;
}

/**
 *  Shifts a color in the HSV space.
 *  Will wrap around the hue value, will clamp the saturation and value values.
 */
int U_shift_hsv(Color *io_color, double dh, double ds, double dv) {
    if (io_color == NULL) {
        return 0;
    }

    io_color->h = U_wrap_double(io_color->h + dh, 0.0, 1.0);
    io_color->s = U_clamp_double(io_color->s + ds, 0.0, 1.0);
    io_color->v = U_clamp_double(io_color->v + dv, 0.0, 1.0);
    C_update_rgb(io_color);
    return 1;
}

/**
 *  Shifts a color in the RGB space.
 *  Will clamp the RGB values between [0.0, 1.0].
 */
int U_shift_rgb(Color *io_color, double dr, double dg, double db) {
    if (io_color == NULL) {
        return 0;
    }

    io_color->r = U_clamp_double(io_color->r + dr, 0.0, 1.0);
    io_color->g = U_clamp_double(io_color->g + dg, 0.0, 1.0);
    io_color->b = U_clamp_double(io_color->b + db, 0.0, 1.0);
    C_update_hsv(io_color);
    return 1;
}

/**
 *  Convert the RGB values to HSV values.
 *  This function was adapted from:
 *  https://en.wikipedia.org/wiki/HSL_and_HSV
 */
int C_update_hsv(Color *io_color) {
    if (io_color == NULL) {
        return 0;
    }

    if (io_color->r < 0 || io_color->r > 1 || io_color->g < 0 ||
        io_color->g > 1 || io_color->b < 0 || io_color->b > 1) {
        return 0;
    }

    double max = fmax(io_color->r, fmax(io_color->g, io_color->b));
    double min = fmin(io_color->r, fmin(io_color->g, io_color->b));
    double delta = max - min;
    double hue, saturation, value;

    // Value
    value = max;

    // Saturation
    if (max == 0) {
        saturation = 0;
    } else {
        saturation = delta / max;
    }

    // Hue
    if (delta == 0) {
        hue = 0;
    } else if (max == io_color->r) {
        hue = fmod((io_color->g - io_color->b) / delta, 6);
    } else if (max == io_color->g) {
        hue = (io_color->b - io_color->r) / delta + 2;
    } else {
        hue = (io_color->r - io_color->g) / delta + 4;
    }
    hue *= 60;

    if (hue < 0) {
        hue += 360;
    }

    io_color->h = hue / 360.0;
    io_color->s = saturation;
    io_color->v = value;
    return 1;
}

/**
 *  Update the RGB values from the HSV values.
 *  This function was adapted from:
 *  https://en.wikipedia.org/wiki/HSL_and_HSV
 */
int C_update_rgb(Color *color) {
    if (color == NULL) {
        return 0;
    }

    if (color->h < 0 || color->h > 1 || color->s < 0 || color->s > 1 ||
        color->v < 0 || color->v > 1) {
        return 0;
    }

    double position = color->h * 6;
    double sector = floor(position);
    double sector_position = position - sector;

    double brightness_low = color->v * (1 - color->s);
    double brightness_medium = color->v * (1 - (color->s * sector_position));
    double brightness_high =
        color->v * (1 - (color->s * (1 - sector_position)));

    switch ((int)sector) {
    case 0:
        color->r = color->v;
        color->g = brightness_high;
        color->b = brightness_low;
        break;
    case 1:
        color->r = brightness_medium;
        color->g = color->v;
        color->b = brightness_low;
        break;
    case 2:
        color->r = brightness_low;
        color->g = color->v;
        color->b = brightness_high;
        break;
    case 3:
        color->r = brightness_low;
        color->g = brightness_medium;
        color->b = color->v;
        break;
    case 4:
        color->r = brightness_high;
        color->g = brightness_low;
        color->b = color->v;
        break;
    case 5:
        color->r = color->v;
        color->g = brightness_low;
        color->b = brightness_medium;
        break;
    }

    return 1;
}

/**
 *  Prints the color data in RGB, HSV, and hex format.
 */
int C_print_color(Color *color) {
    if (color == NULL) {
        return 0;
    }

    printf("RGB: (%f, %f, %f)\n", color->r, color->g, color->b);
    printf("RGB (int): (%d, %d, %d)\n", (int)(color->r * 255),
           (int)(color->g * 255), (int)(color->b * 255));
    printf("HSV: (%f, %f, %f)\n", color->h, color->s, color->v);
    printf("Hex: #%02X%02X%02X\n", (int)(color->r * 255), (int)(color->g * 255),
           (int)(color->b * 255));
    return 1;
}

/**
 *  Sets the color for the next drawing operation(s)
 */
int U_set_color(Color *color) { return G_rgb(color->r, color->g, color->b); }

/**
 *  Sets the color for the next drawing operation(s) using a hex value.
 *  Useful when a color is being used from the palette and not a color that
 *  changes
 */
int U_set_color_hex(int hex) {
    Color color = U_color_hex(hex);
    return G_rgb(color.r, color.g, color.b);
}

/**
 * The same exact function as G_display_image except it yells when I forget to
 * initialize the graphics system.
 */
int U_draw_buffer() {
    if (G_display_image == NULL) {
        printf("G_display_image is NULL\n");
        printf("You didn't initialize the graphics system.\n");
        printf("Run G_init_graphics() first.\n");
        exit(80085);
    }

    G_display_image();
    return 1;
}

int U_draw_line(Line *line, double weight) {
    int pixel_length = (int)U_length(line);
    double pixels_per_percent = pixel_length / 100.0;
    weight /= 2.0;

    for (int i = 0; i < pixel_length; ++i) {
        Point point = U_lerp_line(line, (double)i / (double)pixel_length);
        G_fill_circle(point.x, point.y, weight);
    }
    return 1;
}

int U_draw_dotted_line(Line *line, double weight, int dot_length) {
    int pixel_length = (int)U_length(line);
    double pixels_per_percent = pixel_length / 100.0;
    weight /= 2.0;
    int draw = TRUE;

    for (int i = 0; i < pixel_length; ++i) {
        if (draw) {
            Point point = U_lerp_line(line, (double)i / (double)pixel_length);
            G_fill_circle(point.x, point.y, weight);
        }

        if (i % dot_length == 0) {
            draw = !draw;
        }
    }

    return 1;
}

/**
 * Draws a box on a line.
 * The p[0] of the line that is the start of the box
 */
int U_draw_box_on_line(Line *line, double weight, double width,
                          double height) {
    int pixel_length = (int)U_length(line);
    double pixels_per_percent = pixel_length / 100.0;

    // Make a line along the line that is the length of the width.
    Line base;
    base.p[0] = line->p[0];
    base.p[1] = U_lerp_line(line, width / pixel_length);
    U_draw_line(&base, weight);

    Line right;
    right.p[0] = base.p[1];
    right.p[1] = base.p[1];
    U_lerp_perp(&right.p[1], &base.p[0], &base.p[1], height / width);
    U_draw_line(&right, weight);

    Line left;
    left.p[0] = base.p[0];
    left.p[1] = base.p[0];
    U_lerp_perp(&left.p[1], &base.p[0], &base.p[1], height / width);
    U_draw_line(&left, weight);

    Line top;
    top.p[0] = left.p[1];
    top.p[1] = right.p[1];
    U_draw_line(&top, weight);

    return 1;
}

// ########## INPUT ##########

/**
 * The same exact function as G_wait_key except it doesn't update the screen.
 */
int U_wait_key() {
    int p[2];
    int sig;

    do {
        sig = Gi_events(p);
    } while (sig < 0);

    return sig;
}

/**
 * The same exact function as G_wait_click except it doesn't update the screen.
 */
int U_wait_click(double p[2]) {
    int sig;
    int pi[2];

    do {
        sig = Gi_events(pi);
    } while (sig != -3);

    p[0] = pi[0];
    p[1] = pi[1];

    return sig;
}

/**
 * Same as U_wait_click but updates the Point struct instead of a double array.
 */
int U_wait_click_point(Point *p) {
    double temp[2];
    U_wait_click(temp);
    p->x = temp[0];
    p->y = temp[1];
    return 1;
}

/**
 * Gets a double from the user.
 * Prompt is the string to display to the user.
 */
double U_get_double(char *prompt) {
    double value;
    printf("%s: ", prompt);
    scanf("%lf", &value);
    return value;
}

// ########## MATRIX ##########

/**
 * Mallocs a 2D matrix of doubles. Returns a pointer to the matrix.
 * The matrix is initialized to all 0.
 * This matrix must be freed with U_free_matrix2d.
 */
double **U_create_matrix2d(double n, double m) {
    double **matrix = malloc(n * sizeof(double *));
    for (int i = 0; i < m; ++i) {
        matrix[i] = malloc(m * sizeof(double));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = 0.0;
        }
    }

    return matrix;
}

/**
 * Mallocs a square 2D matrix of doubles. Returns a pointer to the matrix.
 * The matrix is initialized to all 0.
 * This matrix must be freed with U_free_sq_matrix.
 */
double **U_create_sq_matrix(double n) { return U_create_matrix2d(n, n); }

/**
 * Frees a 2D matrix of doubles.
 */
void U_free_matrix2d(double **matrix, double n, double m) {
    for (int i = 0; i < n; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

/**
 * Frees a square 2D matrix of doubles.
 */
void U_free_sq_matrix(double **matrix, double n) {
    U_free_matrix2d(matrix, n, n);
}

// ########## LINE FUNCTIONS ##########

/**
 *  Returns a point that is a percentage of the way between two points.
 *  The percentage can be negative or greater than 1 and used as a scalar.
 */
Point U_lerp(Point *p0, Point *p1, double percentage) {
    Point point;
    point.x = p0->x + (p1->x - p0->x) * percentage;
    point.y = p0->y + (p1->y - p0->y) * percentage;
    return point;
}

/**
 *  Returns a point that is a percentage of the way between two points.
 *  The percentage can be negative or greater than 1 to move beyond the line
 */
Point U_lerp_line(Line *line, double percentage) {
    return U_lerp(&line->p[0], &line->p[1], percentage);
}

/**
 *  Returns the distance between two points.
 */
double U_distance(Point *p0, Point *p1) {
    return sqrt(pow(p1->x - p0->x, 2) + pow(p1->y - p0->y, 2));
}

/**
 * Returns the length of a line (distance between the two points).
 */
double U_length(Line *line) { return U_distance(&line->p[0], &line->p[1]); }

/**
 *  Returns the angle between two points in radians.
 *  The angle is measured from the x-axis, going counter-clockwise.
 */
double U_angle(Point *p0, Point *p1) {
    return atan2(p1->y - p0->y, p1->x - p0->x);
}

/**
 *  Returns the angle of a line in radians.
 *  The angle is measured from the x-axis, going counter-clockwise.
 */
double U_angle_line(Line *line) { return U_angle(&line->p[0], &line->p[1]); }

/**
 * Moves a point perpendicular to the line, by a ratio of the original line.
 */
int U_lerp_perp(Point *output, Point *p0, Point *p1, double ratio) {
    double dx = p1->x - p0->x;
    double dy = p1->y - p0->y;

    output->x += -dy * ratio;
    output->y += dx * ratio;

    return 1;
}

/**
 *  Returns the perpendicular angle of a line in radians.
 *  The angle is measured from the x-axis, going counter-clockwise.
 */
double U_perp_angle(double rads) { return fmod(rads + M_PI / 2, 2 * M_PI); }

/**
 *  Returns a point that is a percentage of the way between two points.
 *  The percentage can be negative or greater than 1 and used as a scalar.
 */
int U_shift_point(Point *io_point, double angle, double distance) {
    io_point->x += cos(angle) * distance;
    io_point->y += sin(angle) * distance;
    return 1;
}

int U_scale_point(Point *io_point, Point *origin, double scale) {
    Point new_point;
    new_point = U_lerp(origin, io_point, scale);
    io_point->x = new_point.x;
    io_point->y = new_point.y;
    return 1;
}

int U_scale_line(Line *io_line, Point *origin, double scale) {
    U_scale_point(&io_line->p[0], origin, scale);
    U_scale_point(&io_line->p[1], origin, scale);
    return 1;
}

// ########## SHAPE TOOLS ##########
Box U_gen_box_by_size(Point *origin, double width, double height) {
    Box box;
    box.p[0].x = origin->x;
    box.p[0].y = origin->y;
    box.p[1].x = origin->x + width;
    box.p[1].y = origin->y + height;
    return box;
}

void U_print_box(Box box) {
    printf("Box: (%f, %f) - (%f, %f)\n", box.p[0].x, box.p[0].y, box.p[1].x,
           box.p[1].y);
}

/**
 * Treat a line as a triangle and find the right angle point.
 */
Point U_right_angle(Line *hypotenuse) {
    Point point;
    point.x = hypotenuse->p[0].x;
    point.y = hypotenuse->p[1].y;
    return point;
}

/**
 * Treat a line as a triangle and find the right angle point.
 * Follow the line in reverse
 */
Point U_right_angle_reverse(Line *hypotenuse) {
    Line reverse;
    reverse.p[0].x = hypotenuse->p[1].x;
    reverse.p[0].y = hypotenuse->p[1].y;
    reverse.p[1].x = hypotenuse->p[0].x;
    reverse.p[1].y = hypotenuse->p[0].y;
    return U_right_angle(&reverse);
}

// ########## COLLISIONS ##########

/**
 * Find if there's a point inside a box.
 * Uses the Cohen-Sutherland algorithm
 * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 */
ClipCode U_point_intersect_box(Point *point, Box *box) {
    ClipCode code = CLIP_INSIDE;
    double left = fmin(box->p[0].x, box->p[1].x);
    double right = fmax(box->p[0].x, box->p[1].x);
    double bottom = fmin(box->p[0].y, box->p[1].y);
    double top = fmax(box->p[0].y, box->p[1].y);

    if (point->x < left) {
        code |= CLIP_LEFT;
    } else if (point->x > right) {
        code |= CLIP_RIGHT;
    }

    if (point->y < bottom) {
        code |= CLIP_BOTTOM;
    } else if (point->y > top) {
        code |= CLIP_TOP;
    }

    return code;
}

/**
 * Find if a line intersects a box.
 * Uses the Cohen-Sutherland algorithm
 * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 */
ClipCode U_line_intersect_box(Line *line, Box *box) {
    ClipCode code[2];
    code[0] = U_point_intersect_box(&line->p[0], box);
    code[1] = U_point_intersect_box(&line->p[1], box);

    //  Easy Outs:
    //  If both points are opposite sides, SUCCESS.
    //  Now we need linear interpolation to see if the line intersects the box.

    // If either point is inside the box, SUCCESS
    if (code[0] == CLIP_INSIDE || code[1] == CLIP_INSIDE) {
        return CLIP_INSIDE;
    }

    // If both points are either left or right, or top or bottom, FAIL.
    if (code[0] & code[1]) {
        return code[0] & code[1];
    }

    // If points are on opposite sides, SUCCESS.
    if ((code[0] | code[1]) == CLIP_ALL) {
        return CLIP_INSIDE;
    }

    double line_length = U_length(line);
    double check_step = 1.0 / line_length;
    for (double i = 0; i < line_length; i += check_step) {
        Point check_point = U_lerp_line(line, i);
        ClipCode check_code = U_point_intersect_box(&check_point, box);
        if (check_code == CLIP_INSIDE) {
            return CLIP_INSIDE;
        }
    }

    return CLIP_FAIL;
}

// ########## MISC FUNCTIONS ##########
int U_clamp_int(int value, int min, int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

double U_clamp_double(double value, double min, double max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

int U_wrap_int(int value, int min, int max) {
    int range = max - min;
    if (value < min) {
        return max - ((min - value) % range);
    } else if (value > max) {
        return min + ((value - max) % range);
    }
    return value;
}

double U_wrap_double(double value, double min, double max) {
    double range = max - min;
    if (value < min) {
        return max - fmod(min - value, range);
    } else if (value > max) {
        return min + fmod(value - max, range);
    }
    return value;
}
