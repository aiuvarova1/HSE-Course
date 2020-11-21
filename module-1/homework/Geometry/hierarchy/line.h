#include "point.h"

#ifndef LINE_H
#define LINE_H

class Line {
public:
    Line(Point p1, Point p2);
    const Point& getFirstPoint() const;
    const Point& getSecondPoint() const;
    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
    static Point intersection(const Line& first, const Line& second);
private:
    Point p1;
    Point p2;
    //ax+by+c=0
    double a;
    double b;
    double c;
};

#endif