#include "FPToolkit.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LEFT = 65361;
int UP = 65362;
int RIGHT = 65363;
int DOWN = 65364;
double ISO_STEP = 0.025;

typedef struct Color {
    double r;
    double g;
    double b;
} Color;

typedef struct CelestialBody {
    char name[50];
    double x;
    double y;
    double radius;
    double orbit_position;
    double orbit_radius;
    double rads_per_day;
    double scale;
    Color color;
    struct CelestialBody *satellite;
    struct CelestialBody *host;
} CelestialBody;

void updateIso(double *iso_percentage, int direction);
void updateBody(CelestialBody *body, int direction, double iso_percentage);
void drawBody(CelestialBody *body);
void drawSystem(CelestialBody *body);
void printBody(CelestialBody *body);

int main() {
    double screen_width, screen_height;
    double iso_percentage;
    CelestialBody sun, earth, moon;
    int keydown;
    int direction;

    keydown = 0;
    screen_width = 1000;
    screen_height = 1000;

    iso_percentage = 0.0;

    strcpy(sun.name, "Sun");
    sun.x = screen_width / 2;
    sun.y = screen_height / 2;
    sun.radius = 100;
    sun.scale = 1.0;
    sun.color.r = 243.0 / 255;
    sun.color.g = 208.0 / 255;
    sun.color.b = 64.0 / 255;
    sun.satellite = &earth;
    sun.host = NULL;

    strcpy(earth.name, "Earth");
    earth.orbit_radius = 300;
    earth.orbit_position = 0;
    earth.radius = 40;
    earth.rads_per_day = 2 * M_PI / 365;
    earth.scale = 1.0;
    earth.color.r = 72.0 / 255;
    earth.color.g = 132.0 / 255;
    earth.color.b = 212.0 / 255;
    earth.satellite = &moon;
    earth.host = &sun;

    strcpy(moon.name, "Moon");
    moon.orbit_radius = 75;
    moon.orbit_position = 0;
    moon.radius = 20;
    moon.rads_per_day = 2 * M_PI / 28;
    moon.scale = 1.0;
    moon.color.r = 185.0 / 255;
    moon.color.g = 181.0 / 255;
    moon.color.b = 195.0 / 255;
    moon.satellite = NULL;
    moon.host = &earth;
    moon.satellite = NULL;

    G_init_graphics(screen_width, screen_height);

    while (keydown != 'q') {
        G_rgb(0, 0, 0);
        G_clear();

        // First do the logic
        updateIso(&iso_percentage, direction);
        updateBody(&earth, direction, iso_percentage);
        updateBody(&moon, direction, iso_percentage);
        drawSystem(&sun);
        // drawBody(&sun);
        // drawBody(&earth);
        // drawBody(&moon);

        keydown = G_wait_key();
        direction = keydown;
    }

    return EXIT_SUCCESS;
}

void updateIso(double *iso_percentage, int direction) {
    if (direction == UP) {
        *iso_percentage += ISO_STEP;
        if (*iso_percentage > 1.0) {
            *iso_percentage = 1.0;
        }
    } else if (direction == DOWN) {
        *iso_percentage -= ISO_STEP;
        if (*iso_percentage < 0.0) {
            *iso_percentage = 0.0;
        }
    }
}

void updateBody(CelestialBody *body, int direction, double iso_percentage) {
    if (direction == LEFT) {
        body->orbit_position -= body->rads_per_day;
    } else if (direction == RIGHT) {
        body->orbit_position += body->rads_per_day;
    }

    float scaled_orbit_radius = body->orbit_radius * (body->host->scale);
    float orbit_radius_y = scaled_orbit_radius * (1.0 - iso_percentage);
    float x_offset = cos(body->orbit_position);
    float y_offset = sin(body->orbit_position);

    body->orbit_position = fmod(body->orbit_position, 2 * M_PI);

    body->x = body->host->x + scaled_orbit_radius * x_offset;
    body->y = body->host->y + orbit_radius_y * y_offset;

    body->scale = 1.0 - (0.25 * iso_percentage * y_offset);
}

void drawBody(CelestialBody *body) {
    float host_scale = 1.0;

    if (body->host != NULL) {
        host_scale = body->host->scale;
    }

    G_rgb(body->color.r, body->color.g, body->color.b);
    G_fill_circle(body->x, body->y, body->radius * body->scale * host_scale);
}

void drawSystem(CelestialBody *body) {
    // If the satellite position is within the range of "pi" draw the satellite.
    // else draw the host first
    if (body->satellite == NULL) {
        drawBody(body);
        return;
    }

    if (body->satellite->orbit_position < M_PI) {
        drawSystem(body->satellite);
        drawBody(body);
    } else {
        drawBody(body);
        drawSystem(body->satellite);
    }
}

void printBody(CelestialBody *body) {
    printf("Name: %s\n", body->name);
    printf("X: %f\n", body->x);
    printf("Y: %f\n", body->y);
    printf("Orbit Position: %f\n", body->orbit_position);
    printf("Orbit Radius: %f\n", body->orbit_radius);
    printf("Rads per Day: %f\n", body->rads_per_day);
    printf("Rel y: %f\n", sin(body->orbit_position));
    printf("\n");
}
