#include "polygon.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle: public Polygon {
public:
    Rectangle(Point p1, Point p2, double a);
    Point center();
    std::pair<Line, Line> diagonals();
private:
    double _a;
};

#endif