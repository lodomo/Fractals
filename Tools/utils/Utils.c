#include "Utils.h"

// ########## COLORS ##########
Color U_init_color_rgb_int(int r, int g, int b) {
    r = U_clamp_int(r, 0, 255);
    g = U_clamp_int(g, 0, 255);
    b = U_clamp_int(b, 0, 255);

    Color color;
    color.r = r / 255.0;
    color.g = g / 255.0;
    color.b = b / 255.0;
    C_update_hsv(&color);
    return color;
}

Color U_init_color_percent(double r, double g, double b) {
    r = U_clamp_double(r, 0.0, 1.0);
    g = U_clamp_double(g, 0.0, 1.0);
    b = U_clamp_double(b, 0.0, 1.0);

    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    C_update_hsv(&color);
    return color;
}

Color U_init_color_hex(int hex) {
    hex = U_clamp_int(hex, 0x000000, 0xFFFFFF);

    Color color;
    color.r = ((hex >> 16) & 0xFF) / 255.0;
    color.g = ((hex >> 8) & 0xFF) / 255.0;
    color.b = (hex & 0xFF) / 255.0;
    C_update_hsv(&color);
    return color;
}

Color U_init_color_hsv_int(int h, int s, int v) {
    h = U_clamp_int(h, 0, 360);
    s = U_clamp_int(s, 0, 100);
    v = U_clamp_int(v, 0, 100);

    Color color;
    color.h = h / 360.0;
    color.s = s / 100.0;
    color.v = v / 100.0;
    C_update_rgb(&color);
    return color;
}

Color U_init_color_hsv_percent(double h, double s, double v) {
    h = U_clamp_double(h, 0.0, 1.0);
    s = U_clamp_double(s, 0.0, 1.0);
    v = U_clamp_double(v, 0.0, 1.0);

    Color color;
    color.h = h;
    color.s = s;
    color.v = v;
    C_update_rgb(&color);
    return color;
}

void U_shift_hsv(Color *color, double dh, double ds, double dv) {
    if (color == NULL) {
        return;
    }

    color->h = U_wrap_double(color->h + dh, 0.0, 1.0);
    color->s = U_clamp_double(color->s + ds, 0.0, 1.0);
    color->v = U_clamp_double(color->v + dv, 0.0, 1.0);
    C_update_rgb(color);
}

void U_shift_rgb(Color *color, double dr, double dg, double db) {
    if (color == NULL) {
        return;
    }

    color->r = U_clamp_double(color->r + dr, 0.0, 1.0);
    color->g = U_clamp_double(color->g + dg, 0.0, 1.0);
    color->b = U_clamp_double(color->b + db, 0.0, 1.0);
    C_update_hsv(color);
}

int C_update_hsv(Color *color) {
    if (color == NULL) {
        return 0;
    }

    if (color->r < 0 || color->r > 1 || color->g < 0 || color->g > 1 ||
        color->b < 0 || color->b > 1) {
        return 0;
    }

    double max = fmax(color->r, fmax(color->g, color->b));
    double min = fmin(color->r, fmin(color->g, color->b));
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
    } else if (max == color->r) {
        hue = fmod((color->g - color->b) / delta, 6);
    } else if (max == color->g) {
        hue = (color->b - color->r) / delta + 2;
    } else {
        hue = (color->r - color->g) / delta + 4;
    }
    hue *= 60;

    if (hue < 0) {
        hue += 360;
    }

    color->h = hue / 360.0;
    color->s = saturation;
    color->v = value;
    return 1;
}

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

int U_set_draw_color(Color color) { return G_rgb(color.r, color.g, color.b); }

int U_set_draw_color_hex(int hex) {
    Color color = U_init_color_hex(hex);
    return G_rgb(color.r, color.g, color.b);
}

// ########## DRAWING AND INPUT ##########
int U_wait_key() {
    int p[2];
    int sig;

    do {
        sig = Gi_events(p);
    } while (sig < 0);

    return sig;
}

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

// ########## MATRIX ##########
double **U_create_matrix2d(double n, double m) {
    double **matrix = malloc(n * sizeof(double *));
    for (int i = 0; i < m; ++i) {
        matrix[i] = malloc(m * sizeof(double));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

double **U_create_sq_matrix(double n) { return U_create_matrix2d(n, n); }

void U_free_matrix2d(double **matrix, double n, double m) {
    for (int i = 0; i < n; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void U_free_sq_matrix(double **matrix, double n) {
    U_free_matrix2d(matrix, n, n);
}

// ########## USER INPUT ##########

double U_get_double(char *prompt) {
    double value;
    printf("%s: ", prompt);
    scanf("%lf", &value);
    return value;
}

// ########## LINE TOOLS ##########

Point U_point_pos_by_percent(Point p0, Point p1, double percentage) {
    Point point;
    point.x = p0.x + (p1.x - p0.x) * percentage;
    point.y = p0.y + (p1.y - p0.y) * percentage;
    return point;
}

Point U_line_pos_by_percent(Line line, double percentage) {
    return U_point_pos_by_percent(line.p[0], line.p[1], percentage);
}

double U_distance_two_points(Point p0, Point p1) {
    return sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2));
}

double U_line_length(Line line) {
    return U_distance_two_points(line.p[0], line.p[1]);
}

double U_point_angle(Point p0, Point p1) {
    return atan2(p1.y - p0.y, p1.x - p0.x);
}

double U_line_angle(Line line) { return U_point_angle(line.p[0], line.p[1]); }

double U_perp_angle(double rads) { return fmod(rads + M_PI / 2, 2 * M_PI); }

int U_shift_point(Point *point, double angle, double distance) {
    point->x += cos(angle) * distance;
    point->y += sin(angle) * distance;
    return 1;
}

int U_scale_point(Point origin, Point *point, double scale) {
    Point new_point;
    new_point = U_point_pos_by_percent(origin, *point, scale);
    point->x = new_point.x;
    point->y = new_point.y;
    return 1;
}

int U_scale_line(Point origin, Line *line, double scale) {
    Point new_p0 = U_point_pos_by_percent(origin, line->p[0], scale);
    Point new_p1 = U_point_pos_by_percent(origin, line->p[1], scale);
    line->p[0] = new_p0;
    line->p[1] = new_p1;
    return 1;
}

// ########## SHAPE TOOLS ##########
double U_eq_triangle_height(double base) { return 0.5 * sqrt(3) * base; }

Box U_gen_box_by_size(Point origin, double width, double height) {
    Box box;
    box.p[0].x = origin.x;
    box.p[0].y = origin.y;
    box.p[1].x = origin.x + width;
    box.p[1].y = origin.y + height;
    return box;
}

void U_print_box(Box box) {
    printf("Box: (%f, %f) - (%f, %f)\n", box.p[0].x, box.p[0].y,
           box.p[1].x, box.p[1].y);
}

// ########## COLLISIONS ##########

ClipCode U_point_intersect_box(Point point, Box box) {
    ClipCode code = CLIP_INSIDE;
    double left = fmin(box.p[0].x, box.p[1].x);
    double right = fmax(box.p[0].x, box.p[1].x);
    double bottom = fmin(box.p[0].y, box.p[1].y);
    double top = fmax(box.p[0].y, box.p[1].y);

    if (point.x < left) {
        code |= CLIP_LEFT;
    } else if (point.x > right) {
        code |= CLIP_RIGHT;
    }

    if (point.y < bottom) {
        code |= CLIP_BOTTOM;
    } else if (point.y > top) {
        code |= CLIP_TOP;
    }

    return code;
}

ClipCode U_line_intersect_box(Line line, Box box) {
    ClipCode code[2];
    code[0] = U_point_intersect_box(line.p[0], box);
    code[1] = U_point_intersect_box(line.p[1], box);

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

    double line_length = U_line_length(line);
    double check_step = 1.0 / line_length;
    for (double i = 0; i < line_length; i += check_step) {
        Point check_point = U_line_pos_by_percent(line, i);
        ClipCode check_code = U_point_intersect_box(check_point, box);
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
