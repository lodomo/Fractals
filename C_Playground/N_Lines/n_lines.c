#include "FPToolkit.c"

int main() {
    double screen_width, screen_height;
    double n_lines;
    double n_spacing;

    screen_width = 800;
    screen_height = 800;

    printf("Enter number of lines: ");
    scanf("%lf", &n_lines);
    printf("Number of lines: %lf\n", n_lines);

    n_spacing = screen_width / n_lines;

    G_init_graphics(screen_width, screen_height);
    G_rgb(1,1,1);
    G_clear();

    G_rgb(69.0 / 255, 161.0 / 255, 222.0 / 255);

    double lhs = screen_height - 1;
    double rhs = n_spacing;

    for (int i = 1; i < n_lines; i++) {
        G_line(0, lhs, rhs, 0);
        lhs -= n_spacing;
        rhs += n_spacing;
    }

    double click_pos[2];
    G_wait_click(click_pos);
    return 0;
}
