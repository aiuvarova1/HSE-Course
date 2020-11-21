#include "ellipse.h"

Ellipse::Ellipse(Point& p1, Point& p2, double dist) : f1(p1), f2(p2) {
    _a = dist / 2;
}

std::pair<Point, Point> Ellipse::focuses() {
    return {f1, f2};
}

double Ellipse::eccentricity() {
    return focalDist() / _a;
}

Point Ellipse::center() {
    return Point((f1.x + f2.x) / 2, (f1.y + f2.y) / 2);
}

double Ellipse::focalDist() const {
    return Shape::getDist(f1, f2) / 2.0;
}

double Ellipse::perimeter() const {
    double b = small_axis();
    return 4 * (PI * _a * b + _a - b) / (_a + b);
}

bool Ellipse::containsPoint(Point point) const {
    return Shape::getDist(point, f1) + Shape::getDist(point, f2) <= 2 * _a;
}

double Ellipse::area() const {
    return Shape::PI * _a * small_axis();
}

double Ellipse::small_axis() const {
    double c = focalDist();
    return sqrt(_a * _a - c * c);
}

bool Ellipse::operator==(const Shape& another) const {
    const auto* other_ellipse = dynamic_cast<const Ellipse*>(&another);
    return other_ellipse != nullptr &&
           (other_ellipse->_a - _a) < EPSILON && (
                   (other_ellipse->f1 == f1 && other_ellipse->f2 == f2) ||
                   (other_ellipse->f1 == f2 && other_ellipse->f2 == f1));
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    const auto* other_ellipse = dynamic_cast<const Ellipse*>(&another);

    return (other_ellipse != nullptr &&
            fabs(other_ellipse->_a - _a) < EPSILON &&
            fabs(small_axis() - other_ellipse->small_axis()) < EPSILON
    );
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    const auto* other_ellipse = dynamic_cast<const Ellipse*>(&another);

    if (other_ellipse == nullptr)
        return false;

    double factor = _a / other_ellipse->_a;
    return fabs(small_axis() - other_ellipse->small_axis() * factor) < EPSILON;
}

void Ellipse::rotate(const Point center, double angle) {
    Shape::rotatePoint(f1, center, angle);
    Shape::rotatePoint(f2, center, angle);
}

void Ellipse::reflex(const Point center) {
    Shape::reflexPoint(f1, center);
    Shape::reflexPoint(f2, center);
}

void Ellipse::reflex(const Line axis) {
    Shape::reflexPoint(f1, axis);
    Shape::reflexPoint(f2, axis);
}

void Ellipse::scale(const Point center, double coefficient) {
    Shape::scalePoint(f1, center, coefficient);
    Shape::scalePoint(f2, center, coefficient);
    _a *= coefficient;
}


