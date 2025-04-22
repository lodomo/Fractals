#include "FPToolkit.c"

int main() {
    int keydown;
    double screen_width, screen_height;
    double grid_count;
    double grid_size;

    screen_width = 800;
    screen_height = 800;

    printf("Enter grid size: ");
    scanf("%lf", &grid_count);
    printf("Grid size: %lf\n", grid_count);

    G_init_graphics(screen_width, screen_height);
    G_rgb(1,1,1);
    G_clear();

    G_rgb(69.0 / 255, 161.0 / 255, 222.0 / 255);

    grid_size = screen_width / grid_count;

    for (int i = 1; i < grid_count; i++) {
        G_line(i * grid_size, 0, i * grid_size, screen_height-1);
        G_line(0, i * grid_size, screen_width-1, i * grid_size);
    }

    double click_pos[2];
    G_wait_click(click_pos);
    return 0;
}
