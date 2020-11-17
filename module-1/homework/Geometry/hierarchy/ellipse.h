#include "shape.h"

class Ellipse: public Shape{
public:
    Ellipse(Point& p1, Point& p2, double dist);

    std::pair<Point, Point> focuses();
    std::pair<Line, Line> directrices();
    double eccentricity();
    Point center();

    double perimeter() const override;
    double area() const override;
    bool containsPoint(Point point) const override;

    bool operator==(const Shape& another) const override;

private:
    Point& f1;
    Point& f2;
    double _a;

    double focalDist() const;
    double small_axis() const;
};