#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include "geometry.h"

using namespace std;

Point::Point() {
	this->x = 0;
	this->y = 0;
}

Point::Point(float first, float second) {
	this->x = first;
	this->y = second;
}

Point::Point(const Point& dot) : x(dot.x), y(dot.y) {}
//fixed that's strange

float Point::getX() const {
	return this->x;
}

float Point::getY() const {
	return this->y;
}

Point& Point::operator= (const Point& dot) {
	this->x = dot.getX();
	this->y = dot.getY();
	return *this;
}

PolygonalChain::PolygonalChain(const int number = 0, Point* points = nullptr) {
	for (int i = 0; i < number; i++)
		this->tops.push_back(points[i]);
}

PolygonalChain::PolygonalChain(const PolygonalChain& chain) : tops(chain.tops) {}
//fixed S P A C E S
PolygonalChain& PolygonalChain::operator=(const PolygonalChain& chain) {
	this->tops = chain.getTops();
	return *this;
}


int PolygonalChain::getN() const {
	return this->tops.size();
}

Point PolygonalChain::getPoint(const int index) const {
	return this->tops[index];
}

vector<Point> PolygonalChain::getTops() const {
	return this->tops;
}

double PolygonalChain::segment(const Point p1, const Point p2) const {
	return sqrt(pow(p1.getX() - p2.getX(), 2.0) + pow(p1.getY() - p2.getY(), 2.0));
}

double PolygonalChain::PolygonalChain::perimeter() const {
	double P = 0;
	for (int i = 1; i < getN(); i++)
		P +=segment(this->tops[i], this->tops[i - 1]);

	return P;
}

PolygonalChain::~PolygonalChain() = default;

ClosedPolygonalChain::ClosedPolygonalChain(const int number = 0, Point* points = nullptr) : PolygonalChain(number, points) {}

ClosedPolygonalChain::ClosedPolygonalChain(const ClosedPolygonalChain& chain) : PolygonalChain(chain) {}

double ClosedPolygonalChain::perimeter() const {
	//fixed copy-paste with PolygonalChain
	return PolygonalChain::perimeter() + segment(getPoint(0), getPoint(getN() - 1));
}

Polygon::Polygon(const int number = 0, Point* points = nullptr) : ClosedPolygonalChain(number, points) {}

Polygon::Polygon(const Polygon& figure) : ClosedPolygonalChain(figure) {}

double Polygon::area() const {
	double square = 0;
	double a, b, c, p;
	//todo Heron's formula is bad
	for (int i = 1; i < getN() - 1; i++) {
		a = segment(getPoint(0), getPoint(i));
		b = segment(getPoint(i + 1), getPoint(i));
		c = segment(getPoint(0), getPoint(i + 1));
		p = (a + b + c) / 2;
		square += sqrt(p * (p - a) * (p - b) * (p - c));
	}

	return square;
}

Triangle::Triangle(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

Triangle::Triangle(const Triangle& triag) : Polygon(triag) {}

//fixed without sqrt
bool Triangle::hasRightAngle() const {
	double a = pow(getPoint(0).getX() - getPoint(1).getX(), 2.0) + pow(getPoint(0).getY() - getPoint(1).getY(), 2.0);
	double b = pow(getPoint(0).getX() - getPoint(2).getX(), 2.0) + pow(getPoint(0).getY() - getPoint(2).getY(), 2.0);
	double c = pow(getPoint(1).getX() - getPoint(2).getX(), 2.0) + pow(getPoint(1).getY() - getPoint(2).getY(), 2.0);

	if (a + b == c || a + c == b || b + c == a)
		return 1;

	return 0;
}

Trapezoid::Trapezoid(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

Trapezoid::Trapezoid(const Trapezoid& trap) : Polygon(trap) {}

double Trapezoid::distance(Point p1, Point p2, Point p3) const {
	Point* buf = new Point[3]{ p1, p2, p3 };
	Polygon triag(3, buf);
	return triag.area() * 2 / segment(p2, p3);
}

double Trapezoid::height() const {
	float x1 = getPoint(0).getX() - getPoint(1).getX();
	float y1 = getPoint(0).getY() - getPoint(1).getY();
	float x2 = getPoint(2).getX() - getPoint(3).getX();
	float y2 = getPoint(2).getY() - getPoint(3).getY();

	if (abs((x1 * x2 + y1 * y2) / (sqrt(pow(x1, 2.0) + pow(y1, 2.0)) * sqrt(pow(x2, 2.0) + pow(y2, 2.0)))) == 1)
		return distance(getPoint(0), getPoint(2), getPoint(3));
	else
		return distance(getPoint(0), getPoint(1), getPoint(2));
}

RegularPolygon::RegularPolygon(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

RegularPolygon::RegularPolygon(const RegularPolygon& reg) : Polygon(reg) {}

double RegularPolygon::perimeter() const {
	return getN() * segment(getPoint(0), getPoint(1));
}

double RegularPolygon::area() const {
	return perimeter() * segment(getPoint(0), getPoint(1)) / (4 * tan(M_PI / getN()));
}