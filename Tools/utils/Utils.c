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
    C_hsv_from_rgb(&color);
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
    C_hsv_from_rgb(&color);
    return color;
}

Color U_init_color_hex(int hex) {
    hex = U_clamp_int(hex, 0x000000, 0xFFFFFF);

    Color color;
    color.r = ((hex >> 16) & 0xFF) / 255.0;
    color.g = ((hex >> 8) & 0xFF) / 255.0;
    color.b = (hex & 0xFF) / 255.0;
    C_hsv_from_rgb(&color);
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
    C_rgb_from_hsv(&color);
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
    C_rgb_from_hsv(&color);
    return color;
}

void U_shift_hsv(Color *color, double dh, double ds, double dv) {
    if (color == NULL) {
        return;
    }

    color->h = fmod(color->h + dh, 1.0);
    if (color->h < 0) {
        color->h += 1.0;
    }
    color->s = U_clamp_double(color->s + ds, 0.0, 1.0);
    color->v = U_clamp_double(color->v + dv, 0.0, 1.0);
    C_rgb_from_hsv(color);
}

int C_hsv_from_rgb(Color *color) {
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

int C_rgb_from_hsv(Color *color) {
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

int U_set_color(Color color) { return G_rgb(color.r, color.g, color.b); }

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

Point U_get_pos_by_percent(Line line, double percentage) {
    Point point;
    point.x = line.start.x + (line.end.x - line.start.x) * percentage;
    point.y = line.start.y + (line.end.y - line.start.y) * percentage;
    return point;
}

double U_line_length(Line line) {
    return sqrt(pow(line.end.x - line.start.x, 2) +
                pow(line.end.y - line.start.y, 2));
}

double U_get_angle_rad(Line line) {
    return atan2(line.end.y - line.start.y, line.end.x - line.start.x);
}

double U_perp_angle(double rads) { return fmod(rads + M_PI / 2, 2 * M_PI); }

Point U_shift_point(Point point, double angle, double distance) {
    Point new_point;
    new_point.x = point.x + cos(angle) * distance;
    new_point.y = point.y + sin(angle) * distance;
    return new_point;
}

// ########## SHAPE TOOLS ##########
double U_eq_triangle_height(double base) { return HALF_SQRT_3 * base; }

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

