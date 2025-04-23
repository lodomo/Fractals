#include "main.h"

Production prd[10];
int rule_num = 0;
char u[10000];
char v[10000];
int depth = 0;

Point TURTLE;

double START_ANGLE = 0.5 * M_PI;
double DELTA_ANGLE = 0.5 * M_PI;
double FORWARD_LENGTH = 1;

int main(void) {
    int keydown = 0;
    printf("Please specify the level of depth for the l-system: ");
    scanf("%d", &depth);

    G_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    U_set_color_hex(SM32_BLACK);

    square_wave();
    //  plant() ;
    string_builder();
    autoplacer(SCREEN_WIDTH, SCREEN_HEIGHT);
    string_interpreter();

    return EXIT_SUCCESS;
}

void square_wave() {
    START_ANGLE = 0.5 * M_PI;
    DELTA_ANGLE = 0.5 * M_PI;

    strcpy(prd[rule_num].axiom, "A");
    rule_num++;

    prd[rule_num].var = 'A';
    strcpy(prd[rule_num].rule, "f-f-B");
    rule_num++;

    prd[rule_num].var = 'B';
    strcpy(prd[rule_num].rule, "f+f+A");
    rule_num++;
}

void fern() {
    START_ANGLE = -1 * M_PI / 180 * 25;
    DELTA_ANGLE = M_PI / 180 * 25;

    strcpy(prd[rule_num].axiom, "A");
    rule_num++;

    prd[rule_num].var = 'A';
    strcpy(prd[rule_num].rule, "f-[[A]+A]+f+[A]-A");
    rule_num++;

    prd[rule_num].var = 'f';
    strcpy(prd[rule_num].rule, "f");
}


int find_rule(char s) {
    int k;
    for (k = 1; k < rule_num; k++) {
        if (prd[k].var == s) {
            return k;
        }
    }
    return -1;
}

void string_builder() {
    int k, c, loc;
    // build array u
    u[0] = '\0';
    strcat(u, prd[0].axiom);
    printf("u: %s\n", u);
    // recur depth times to build string
    for (c = 0; c < depth; c++) {
        v[0] = '\0';
        k = 0;
        while (u[k] != '\0') {
            // if symbol is a variable
            loc = find_rule(u[k]);
            if (loc != -1) {
                strcat(v, prd[loc].rule);
            }
            // if symbol is not a variable
            else {
                char temp[2];
                temp[0] = u[k];
                temp[1] = '\0';
                strcat(v, temp);
            }
            k++;
        }
        strcpy(u, v);
        printf("u: %s\n", u);
        printf("\n");
    }
}

void autoplacer(int swidth, int sheight) {
    double left = 0;
    double right = 0;
    double top = 0;
    double bottom = 0;
    double angle = START_ANGLE;
    char cur;

    for (int i = 0; i < strlen(u); i++) {
        cur = u[i];
        if (cur == 'f') {
            Point temp_turtle;
            temp_turtle.x = TURTLE.x + FORWARD_LENGTH * cos(angle);
            temp_turtle.y = TURTLE.y + FORWARD_LENGTH * sin(angle);
            TURTLE.x = temp_turtle.x;
            TURTLE.y = temp_turtle.y;
        } else if (cur == '-') {
            angle -= DELTA_ANGLE;
            angle = fmod(angle, 2 * M_PI);
        } else if (cur == '+') {
            angle += DELTA_ANGLE;
            angle = fmod(angle, 2 * M_PI);
        }

        if (TURTLE.x < left) {
            left = TURTLE.x;
        }
        if (TURTLE.x > right) {
            right = TURTLE.x;
        }
        if (TURTLE.y < bottom) {
            bottom = TURTLE.y;
        }
        if (TURTLE.y > top) {
            top = TURTLE.y;
        }
    }

    double width = right - left;
    double height = top - bottom;

    double scale_x = (double)swidth / width;
    double scale_y = (double)sheight / height;
    double scale = scale_x;
    if (scale_y < scale_x) {
        scale = scale_y;
    }
    scale = scale * 0.9; // Add some padding
    double offset_x = (swidth - width * scale) / 2;
    double offset_y = (sheight - height * scale) / 2;

    // Adjust the turtle's position and scale
    TURTLE.x = left * scale + offset_x;
    TURTLE.y = bottom * scale + offset_y;
    FORWARD_LENGTH *= scale;
}

void string_interpreter() {
    double angle = START_ANGLE;
    char cur;

    for (int i = 0; i < strlen(u); i++) {
        cur = u[i];
        if (cur == 'f') {
            Point temp_turtle;
            temp_turtle.x = TURTLE.x + FORWARD_LENGTH * cos(angle);
            temp_turtle.y = TURTLE.y + FORWARD_LENGTH * sin(angle);
            G_line(TURTLE.x, TURTLE.y, temp_turtle.x, temp_turtle.y);
            TURTLE.x = temp_turtle.x;
            TURTLE.y = temp_turtle.y;
        } else if (cur == '-') {
            angle -= DELTA_ANGLE;
            angle = fmod(angle, 2 * M_PI);
        } else if (cur == '+') {
            angle += DELTA_ANGLE;
            angle = fmod(angle, 2 * M_PI);
        }
    }
    G_wait_key();
}
