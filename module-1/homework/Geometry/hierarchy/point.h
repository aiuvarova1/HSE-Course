
struct Point {

    double x;
    double y;

    Point(double x, double y);
    ~Point()= default;

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    Point operator+(const Point& other) const;
};