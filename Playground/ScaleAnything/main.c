typedef struct Point {
    double x;
    double y;
} Point;


Point U_point_pos_by_percent(Point p0, Point p1, double percentage) {
    Point point;
    point.x = p0.x + (p1.x - p0.x) * percentage;
    point.y = p0.y + (p1.y - p0.y) * percentage;
    return point;
}

int main(void)
{
    Point stickman[10];
    Point scale_origin = {10.0, 10.0};
    double scale_factor = 0.5;

    for (int i = 0; i < 10; i++) {
        stickman[i] = U_point_pos_by_percent(scale_origin, stickman[i], scale_factor);
    }

    return 0;
}

