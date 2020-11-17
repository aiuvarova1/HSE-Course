
#include "square.h"

Circle Square::circumscribedCircle() {
    return Circle(Shape::getVectorCenter(_vertices[0], _vertices[2]),
                  Shape::getDist(_vertices[0], _vertices[2]) / 2);
}

Circle Square::inscribedCircle() {
    return Circle(Shape::getVectorCenter(_vertices[0], _vertices[2]),
                  Shape::getDist(_vertices[0], _vertices[1]) / 2);
}
