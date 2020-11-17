
#include "ellipse.h"

#ifndef CIRCLE_H
#define CIRCLE_H

class Circle: public Ellipse {
public:
    Circle(Point center, double r);
    double radius();
private:
    Point _center;
    double _r;
};

#endif