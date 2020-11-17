#include <vector>
#include "polygon.h"

Polygon::Polygon(std::vector<Point>& vertices) {
    _vertices = vertices;
}

int Polygon::verticesCount() const {
    return _vertices.size();
}

std::vector<Point> Polygon::getVertices() const {
    return _vertices;
}

bool Polygon::isConvex() {
    Point* first = &_vertices[_vertices.size() - 2];
    Point* second = &_vertices[_vertices.size() - 1];
    Point* third = &_vertices[0];

    bool sign = getTriangleDet(*first, *second, *third) > 0;
    for (size_t i = 1; i < _vertices.size(); i++) {
        first = second;
        second = first;
        third = &_vertices[i];

        if (getTriangleDet(*first, *second, *third) > 0 != sign)
            return false;
    }
    return true;
}

double Polygon::getTriangleDet(const Point& p1, const Point& p2, const Point& p3) {
    return p1.x * p2.y + p2.x * p3.y + p1.y * p3.x - p2.y * p3.x - p3.y * p1.x - p1.y * p2.x;
}

double Polygon::perimeter() const {
    double perimeter = 0;
    for (int i = 0; i < verticesCount(); i++) {
        perimeter += getDist(_vertices[i], _vertices[(i + 1) % verticesCount()]);
    }
    return perimeter;
}

double Polygon::area() const {
    double area = 0;
    const Point* first, * second;
    for (int i = 0; i < verticesCount(); i++) {
        first = &_vertices[i];
        second = &_vertices[(i + 1) % verticesCount()];
        area += (first->x + second->x) * (first->y - second->y);
    }
    return fabs(area) / 2.0;
}

bool Polygon::containsPoint(Point point) const {
    double cur_angle = 0;

    const Point* first, * second;
    double a, b, c, cos;
    for (int i = 0; i < verticesCount(); i++) {
        first = &_vertices[i];
        second = &_vertices[(i + 1) % verticesCount()];

        a = getDist(*first, *second);
        b = getDist(*first, point);
        c = getDist(*second, point);

        //cos theorem
        cos = (b * b + c * c - a * a) / (2 * b * c);
        cur_angle += acos(cos);
    }
    return (2 * PI - cur_angle) <= EPSILON;
}

bool Polygon::operator==(const Shape& another) const {
    const auto* other_polygon = dynamic_cast<const Polygon*>(&another);
    if (other_polygon == nullptr ||
        other_polygon->verticesCount() == verticesCount())
        return false;
    int offset = std::find(_vertices.begin(), _vertices.end(), other_polygon->_vertices[0])
                 - _vertices.begin();

    if (offset > verticesCount()) {
        return false;
    }

    const Point* corresponding_point;
    for (int i = 0; i < verticesCount(); i++) {
        corresponding_point = &_vertices[(i + offset) % verticesCount()];
        if (other_polygon->_vertices[i] != *corresponding_point) {
            return false;
        }
    }

    return true;
}


