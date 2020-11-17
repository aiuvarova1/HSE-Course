
#include "line.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#ifndef SHAPE_H
#define SHAPE_H

class Shape {

public:
    constexpr static const double PI=3.1415926;
    constexpr static const double EPSILON = 0.001;

    virtual ~Shape() = default;
    virtual double perimeter() const=0;
    virtual double area() const=0;
    virtual bool containsPoint(Point point) const=0;

    virtual bool operator==(const Shape& another) const=0;
    virtual bool isCongruentTo(const Shape& another) const=0;
    virtual bool isSimilarTo(const Shape& another) const=0;

    virtual void rotate(Point center, double angle)=0;
    virtual void reflex(Point center)=0;
    virtual void reflex(Line axis)=0;
    virtual void scale(Point center, double coefficient)=0;

    static double getDist(const Point& p1, const Point& p2);
    static Point getVectorCenter(const Point& p1, const Point& p2);
};

#endif