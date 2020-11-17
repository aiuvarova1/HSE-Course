
#include "circle.h"

Circle::Circle(Point center, double r) :
        Ellipse(center, center, r * 2),
        _center(center) {
    _r = r;
}

double Circle::radius() {
    return _r;
}


