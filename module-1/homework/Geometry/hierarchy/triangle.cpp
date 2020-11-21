#include "triangle.h"

Triangle::Triangle(Point p1, Point p2, Point p3) {
    _vertices = {p1, p2, p3};
}

Circle Triangle::circumscribedCircle() const {
    Point first_side = _vertices[0] - _vertices[1];
    Point second_side = _vertices[1] - _vertices[2];

    //because scalar product must be zero
    Point first_perp = Point(first_side.y, -first_side.x);
    Point second_perp = Point(second_side.y, -second_side.x);

    Point first_center = Shape::getVectorCenter(_vertices[0], _vertices[1]);
    Point second_center = Shape::getVectorCenter(_vertices[1], _vertices[2]);

    Line first_line_perp = Line(first_center + first_perp, first_center);
    Line second_line_perp = Line(second_center + second_perp, second_center);

    Point intersection = Line::intersection(first_line_perp, second_line_perp);
    return Circle(intersection, Shape::getDist(intersection, _vertices[0]));
}

Circle Triangle::inscribedCircle() {

    Line first_bisector = findBisector(0);
    Line second_bisector = findBisector(1);

    Point center = Line::intersection(first_bisector, second_bisector);

    return Circle(center, 2 * area() / perimeter());
}

Line Triangle::findBisector(int verticeNum) const {
    int vertices_count = _vertices.size();

    Point vertice = getVertices()[verticeNum];
    Point first = _vertices[(verticeNum + 1) % vertices_count];
    Point second = _vertices[(verticeNum + 2) % vertices_count];

    double factor = getDist(first, vertice) / getDist(second, vertice);
    Point bisector_intersection =
            getVectorDividedByFactor(first, second, factor);
    return {bisector_intersection, vertice};
}


