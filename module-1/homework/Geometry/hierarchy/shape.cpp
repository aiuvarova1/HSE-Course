#include "shape.h"

double Shape::getDist(const Point& p1, const Point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double Shape::getAngle(const Point& first, const Point& second, const Point& middlePoint) {
    double a = getDist(first, second);
    double b = getDist(first, middlePoint);
    double c = getDist(second, middlePoint);

    //cos theorem
    double cos = (b * b + c * c - a * a) / (2 * b * c);
    return acos(cos);
}

Point Shape::getVectorDividedByFactor(const Point& p1, const Point& p2, double factor) {
    return Point((p1.x + factor * p2.x) / (1 + factor), (p1.y + factor * p2.y) / (1 + factor));
}

Point Shape::getVectorCenter(const Point& p1, const Point& p2) {
    return getVectorDividedByFactor(p1, p2, 1);
}

void Shape::rotatePoint(Point& point, const Point& center, double angle) {
    Point vec = point - center;

    double radians_angle = angle * PI / 180;

    point.x = vec.x * cos(radians_angle) - vec.y * sin(radians_angle);
    point.y = vec.x * sin(radians_angle) + vec.y * cos(radians_angle);

    point = point + center;

}

void Shape::reflexPoint(Point& point, const Point& center) {
    Point vec = center - point;
    point = point + vec * 2;
}

void Shape::reflexPoint(Point& point, const Line& line) {
    Point line_vec = line.getSecondPoint() - line.getFirstPoint();
    double line_vec_length = getDist(line.getFirstPoint(), line.getSecondPoint());
    Point normed_line_vector = {line_vec.x / line_vec_length, line_vec.y / line_vec_length};

    Point vec = point - line.getFirstPoint();

    double proj_length = (dotProduct(vec, line_vec)) / line_vec_length;
    Point proj_vec = line.getFirstPoint() + normed_line_vector * proj_length;

    Point perp = proj_vec - point;

    point = point + perp * 2;
}

void Shape::scalePoint(Point& point, const Point& center, double coefficient) {
    Point cur_vec = point - center;
    point = center + cur_vec * coefficient;
}

double Shape::crossProduct(const Point& p1, const Point& p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

double Shape::dotProduct(const Point& p1, const Point& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}