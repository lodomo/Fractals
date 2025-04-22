#include "FPToolkit.c"

// Struct for a solar body
typedef struct {
    double x, y;
    double radius;
    double orbit_position;
    double orbit_radius;
    double r, g, b;
    double day_rads;
} Body;

int main() {
    int keydown;
    double screen_width, screen_height;

    screen_width = 1000;
    screen_height = 1000;

    G_init_graphics(screen_width, screen_height);
    G_rgb(0, 0, 0);
    G_clear();

    Body sun;
    sun.x = screen_width / 2;
    sun.y = screen_height / 2;
    sun.radius = 200;
    sun.r = 243.0 / 255;
    sun.g = 208.0 / 255;
    sun.b = 64.0 / 255;
    G_rgb(sun.r, sun.g, sun.b);
    G_fill_circle(sun.x, sun.y, sun.radius);

    Body earth;
    earth.orbit_radius = 350;
    earth.orbit_position = 0;
    earth.radius = 50;
    // Real orbit is 365.26 days, but 365 to get a clean end orbit.
    earth.day_rads = 2 * M_PI / 365;
    earth.x = sun.x + earth.orbit_radius * cos(earth.orbit_position);
    earth.y = sun.y + earth.orbit_radius * sin(earth.orbit_position);
    earth.r = 0;
    earth.g = 0;
    earth.b = 1;
    G_rgb(earth.r, earth.g, earth.b);
    G_fill_circle(earth.x, earth.y, earth.radius);

    Body moon;
    moon.orbit_radius = 100;
    moon.orbit_position = 0;
    moon.radius = 20;
    // Real orbit is 27.3 but 28 to get a clean end orbit
    moon.day_rads = 2 * M_PI / 28;
    moon.x = earth.x + moon.orbit_radius * cos(moon.orbit_position);
    moon.y = earth.y + moon.orbit_radius * sin(moon.orbit_position);
    moon.r = 1;
    moon.g = 1;
    moon.b = 1;
    G_rgb(moon.r, moon.g, moon.b);
    G_fill_circle(moon.x, moon.y, moon.radius);

    double click_pos[2];
    G_wait_click(click_pos);

    while (1) {
        // Clear the screen to black
        G_rgb(0, 0, 0);
        G_clear();

        // Draw the sun
        G_rgb(sun.r, sun.g, sun.b);
        G_fill_circle(sun.x, sun.y, sun.radius);

        // Update earth position
        earth.orbit_position += earth.day_rads;
        earth.orbit_position = fmod(earth.orbit_position, 2 * M_PI);
        earth.x = sun.x + earth.orbit_radius * cos(earth.orbit_position);
        earth.y = sun.y + earth.orbit_radius * sin(earth.orbit_position);
        G_rgb(earth.r, earth.g, earth.b);
        G_fill_circle(earth.x, earth.y, earth.radius);

        // Update moon position
        moon.orbit_position += moon.day_rads;
        moon.orbit_position = fmod(moon.orbit_position, 2 * M_PI);
        moon.x = earth.x + moon.orbit_radius * cos(moon.orbit_position);
        moon.y = earth.y + moon.orbit_radius * sin(moon.orbit_position);
        G_rgb(moon.r, moon.g, moon.b);
        G_fill_circle(moon.x, moon.y, moon.radius);

        G_wait_key();
    }

    // double click_pos[2];
    // G_wait_click(click_pos);
}
