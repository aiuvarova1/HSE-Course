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

double Polygon::getTriangleDet(const Point& p1, const Point& p2, const Point& p3) const {
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

    for (int i = 0; i < verticesCount(); i++) {
        first = &_vertices[i];
        second = &_vertices[(i + 1) % verticesCount()];

        cur_angle += getAngle(*first, *second, point);
    }
    return fabs(2 * PI - cur_angle) <= EPSILON;
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

bool Polygon::isCongruentTo(const Shape& another) const {
    const auto* other_polygon = dynamic_cast<const Polygon*>(&another);

    if (other_polygon == nullptr)
        return false;

    std::vector<std::pair<double, double>> this_lengths_angles = getLengthsAndAngles();
    std::vector<std::pair<double, double>> other_lengths_angles = other_polygon->getLengthsAndAngles();

    if (this_lengths_angles.size() != other_lengths_angles.size())
        return false;

    bool need_break = false;
    double this_length, other_length, this_angle, other_angle;
    for (int offset = 0; !need_break && offset < verticesCount(); offset++) {

        for (int i = 0; i < verticesCount(); i++) {
            this_length = this_lengths_angles[(offset + i) % verticesCount()].first;
            other_length = other_lengths_angles[(offset + i) % verticesCount()].first;

            this_angle = this_lengths_angles[(offset + i) % verticesCount()].second;
            other_angle = other_lengths_angles[(offset + i) % verticesCount()].second;
            if (fabs(this_length - other_length) > EPSILON ||
                fabs(this_angle - other_angle) > EPSILON)
                need_break = true;
        }

        if (!need_break) {
            return true;
        }
    }

    return false;
}

bool Polygon::isSimilarTo(const Shape& another) const {
    const auto* other_polygon = dynamic_cast<const Polygon*>(&another);

    if (other_polygon == nullptr)
        return false;

    std::vector<std::pair<double, double>> this_lengths_angles = getLengthsAndAngles();
    std::vector<std::pair<double, double>> other_lengths_angles = other_polygon->getLengthsAndAngles();

    if (this_lengths_angles.size() != other_lengths_angles.size())
        return false;

    bool need_break = false;
    double this_length, other_length, this_angle, other_angle;

    double factor = this_lengths_angles[0].first / other_lengths_angles[0].first;

    for (int offset = 0; !need_break && offset < verticesCount(); offset++) {

        for (int i = 0; i < verticesCount(); i++) {
            this_length = this_lengths_angles[(offset + i) % verticesCount()].first;
            other_length = other_lengths_angles[(offset + i) % verticesCount()].first * factor;

            this_angle = this_lengths_angles[(offset + i) % verticesCount()].second;
            other_angle = other_lengths_angles[(offset + i) % verticesCount()].second;
            if (fabs(this_length - other_length) > EPSILON ||
                fabs(this_angle - other_angle) > EPSILON)
                need_break = true;
        }

        if (!need_break) {
            return true;
        }
    }

    return false;
}

std::vector<std::pair<double, double>> Polygon::getLengthsAndAngles() const {
    std::vector<std::pair<double, double>> lengthsAndAngles;

    double length, angle;
    for (int i = 0; i < verticesCount(); i++) {
        length = getDist(_vertices[i], _vertices[(i + 1) % verticesCount()]);
        angle = getAngle(_vertices[(i - 1) < 0 ? verticesCount() - 1 : i - 1], _vertices[i],
                         _vertices[(i + 1) % verticesCount()]);
        lengthsAndAngles.emplace_back(length, angle);
    }

    return lengthsAndAngles;
}

double Polygon::getAngle(const Point& first, const Point& second, const Point& middlePoint) const {

    double a = getDist(first, second);
    double b = getDist(first, middlePoint);
    double c = getDist(second, middlePoint);

    //cos theorem
    double cos = (b * b + c * c - a * a) / (2 * b * c);
    return acos(cos);
}


