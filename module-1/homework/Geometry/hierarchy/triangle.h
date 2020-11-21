#include "polygon.h"
#include "circle.h"

class Triangle: public Polygon {
public:
    Triangle(Point p1, Point p2, Point p3);
    Circle circumscribedCircle() const;
    Circle inscribedCircle();

private:
    Line findBisector(int verticeNum) const;
};