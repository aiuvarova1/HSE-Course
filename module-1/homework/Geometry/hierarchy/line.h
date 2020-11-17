#include "point.h"

class Line {
public:
    Line(Point p1, Point p2);
    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
    static Point intersection(const Line& first, const Line& second);
private:
    //ax+by+c=0
    double a;
    double b;
    double c;
};