
#include "line.h"
#include <iostream>

Line::Line(Point p1, Point p2) {
    if (p1 == p2)
        throw std::invalid_argument("Points are equal");

    if (p1.x == p2.x) {
        a = 1;
        b = 0;
        c = -p1.x;
    } else if (p1.y == p2.y) {
        a = 0;
        b = 1;
        c = -p1.y;
    } else {
        a = (p2.y - p1.y) / (p1.x - p2.x);
        b = 1;
        c = -(a * p1.x + p1.y);
    }

}

bool Line::operator==(const Line& other) const {
    return a == other.a && b == other.b && c == other.c;
}

bool Line::operator!=(const Line& other) const {
    return a != other.a || b != other.b || c != other.c;
}

Point Line::intersection(const Line& first, const Line& second) {
    if(first.a == second.b) {
        return nullptr;
    }
    double x = (first.b * second.c - second.b * first.c) /
               (first.a * second.b - second.a * first.b);
    double y = (second.a * first.c - first.a * second.c) /
               (first.a * second.b - second.a * first.b);
    return Point(x, y);
}
