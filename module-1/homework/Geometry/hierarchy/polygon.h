
#include "shape.h"

#ifndef POLYGON_H
#define POLYGON_H

class Polygon : public Shape {
public:
    Polygon(std::vector<Point>& vertices);
    int verticesCount() const;
    std::vector<Point> getVertices() const;
    bool isConvex();

    double perimeter() const override;
    double area() const override;
    bool containsPoint(Point point) const override;
    bool operator==(const Shape& another) const override;

protected:
    std::vector<Point> _vertices;
private:
    double getTriangleDet(const Point& p1, const Point& p2, const Point& p3);
};

#endif