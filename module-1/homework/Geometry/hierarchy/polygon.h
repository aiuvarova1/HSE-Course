#include <vector>
#include "shape.h"

#ifndef POLYGON_H
#define POLYGON_H

class Polygon : public Shape {
public:
    Polygon() = default;
    Polygon(const std::vector<Point>& vertices): _vertices(vertices) {};
    int verticesCount() const;
    std::vector<Point> getVertices() const;
    bool isConvex();

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

protected:
    std::vector<Point> _vertices;
private:
    std::vector<std::pair<double, double>> getLengthsAndAngles() const;
};

#endif