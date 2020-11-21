
#include "rectangle.h"

Rectangle::Rectangle(Point p1, Point p2, double a) {
    _a = a;

    double x = p2.x - p1.x;
    double y = p2.y - p1.y;
    double diag_length = Shape::getDist(p1, p2);
    //bs^2 + a^2*bs^2 = diag_length^2
    double bigger_side = sqrt(diag_length * diag_length / (1 + a * a));
    double smaller_side = bigger_side * a;

    double sin = bigger_side / diag_length;
    double cos = smaller_side / diag_length;

    double lower_rotated_x = smaller_side * (x * cos - y * sin) / diag_length;
    double lower_rotated_y = smaller_side * (y * cos + x * sin) / diag_length;

    double upper_rotated_x = bigger_side * (x * sin + y * cos) / diag_length;
    double upper_rotated_y = bigger_side * (y * sin - x * cos) / diag_length;

    _vertices = {p1, Point(lower_rotated_x, lower_rotated_y), p2, Point(upper_rotated_x, upper_rotated_y)};

}

Point Rectangle::center() {
    return Shape::getVectorCenter(_vertices[0], _vertices[2]);
}

std::pair<Line, Line> Rectangle::diagonals() {
    return {Line(_vertices[0], _vertices[2]), Line(_vertices[1], _vertices[3])};
}
