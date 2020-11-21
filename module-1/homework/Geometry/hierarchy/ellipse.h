#include "shape.h"

#ifndef ELLIPSE_H
#define ELLIPSE_H
class Ellipse: public Shape{
public:
    Ellipse(Point& p1, Point& p2, double dist);

    std::pair<Point, Point> focuses();
    double eccentricity();
    Point center();

    double perimeter() const override;
    double area() const override;
    bool containsPoint(Point point) const override;

    bool operator==(const Shape& another) const override;

    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;

    void rotate(Point center, double angle) override;
    void reflex(Point center) override;
    void reflex(Line axis) override;
    void scale(Point center, double coefficient) override;

private:
    Point& f1;
    Point& f2;
    double _a;

    double focalDist() const;
    double small_axis() const;
};

#endif