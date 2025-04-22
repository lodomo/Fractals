#include "FPToolkit.c"

int main() {
    int keydown;
    double screen_width, screen_height;
    double checker_count;
    double checker_size;

    screen_width = 800;
    screen_height = 800;

    printf("Enter checker count: ");
    scanf("%lf", &checker_count);
    printf("Checker count: %lf\n", checker_count);

    G_init_graphics(screen_width, screen_height);
    G_rgb(1,1,1);
    G_clear();

    // Checker color light blue
    G_rgb(69.0 / 255, 161.0 / 255, 222.0 / 255);

    checker_size = screen_width / checker_count;

    for (int i = 0; i < checker_count; i++) {
        for (int j = 0; j < checker_count; j++) {
            if ((i + j) % 2 == 0) {
                G_fill_rectangle(i * checker_size, j * checker_size, checker_size, checker_size);
            }
        }
    }

    double click_pos[2];
    G_wait_click(click_pos);
    return 0;
}
