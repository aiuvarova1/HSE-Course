#include "ellipse.h"

Ellipse::Ellipse(Point& p1, Point& p2, double dist) : f1(p1), f2(p2) {
    _a = dist / 2;
}

std::pair<Point, Point> Ellipse::focuses() {
    return {f1, f2};
}

std::pair<Line, Line> Ellipse::directrices() {
//    Point a = {};
//    return {Line(), Line()};
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
    return 2 * Shape::PI * sqrt((_a * _a + b * b) / 2.0);
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
           other_ellipse->_a == _a && (
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

    if(other_ellipse == nullptr)
        return false;

    double factor = _a / other_ellipse->_a;
    return fabs(small_axis() - other_ellipse->small_axis() * factor) < EPSILON;
}


