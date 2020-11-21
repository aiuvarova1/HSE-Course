#include "polygon.h"


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

    bool sign = fabs(crossProduct(*first - *second, *third - *second)) < EPSILON;
    for (size_t i = 1; i < _vertices.size(); i++) {
        first = second;
        second = third;
        third = &_vertices[i];

        if (fabs(crossProduct(*first - *second, *third - *second)) < EPSILON != sign)
            return false;
    }
    return true;
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

        if (getDist(*first, point) + getDist(*second, point)
            - getDist(*first, *second) < EPSILON) {
            return true;
        }
    }
    return fabs(2 * PI - cur_angle) <= EPSILON;
}

bool Polygon::operator==(const Shape& another) const {
    const auto* other_polygon = dynamic_cast<const Polygon*>(&another);
    if (other_polygon == nullptr ||
        other_polygon->verticesCount() != verticesCount())
        return false;
    int offset = std::find(_vertices.begin(), _vertices.end(), other_polygon->_vertices[0])
                 - _vertices.begin();

    if (offset > verticesCount()) {
        return false;
    }

    const Point* corresponding_point;
    double dif_x, dif_y;
    for (int i = 0; i < verticesCount(); i++) {
        corresponding_point = &_vertices[(i + offset) % verticesCount()];
        dif_x = (other_polygon->_vertices[i] - *corresponding_point).x;
        dif_y = (other_polygon->_vertices[i] - *corresponding_point).y;
        if (fabs(dif_x) > EPSILON || fabs(dif_y) > EPSILON) {
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

void Polygon::rotate(Point center, double angle) {
    for (Point& p : _vertices) {
        Shape::rotatePoint(p, center, angle);
    }
}

void Polygon::reflex(Point center) {
    for (Point& p : _vertices) {
        Shape::reflexPoint(p, center);
    }
}

void Polygon::reflex(Line axis) {
    for (Point& p : _vertices) {
        Shape::reflexPoint(p, axis);
    }
}

void Polygon::scale(Point center, double coefficient) {
    for (Point& p : _vertices) {
        Shape::scalePoint(p, center, coefficient);
    }
}



