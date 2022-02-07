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
	double p = 0;
	for (int i = 1; i < getN(); i++)
		p +=segment(this->tops[i], this->tops[i - 1]);

	return p;
}

PolygonalChain::~PolygonalChain() = default;

ClosedPolygonalChain::ClosedPolygonalChain(const int number = 0, Point* points = nullptr) : PolygonalChain(number, points) {}

double ClosedPolygonalChain::perimeter() const {
	return PolygonalChain::perimeter() + segment(getPoint(0), getPoint(getN() - 1));
}

Polygon::Polygon(const int number = 0, Point* points = nullptr) : ClosedPolygonalChain(number, points) {}

double Polygon::area() const {
	double buf_1 = 0, buf_2 = 0;
	for (int i = 0; i < getN() - 1; i++) {
		buf_1 += getPoint(i).getX() * getPoint(i + 1).getY();
		buf_2 += getPoint(i).getY() * getPoint(i + 1).getX();
	}
	buf_1 += getPoint(getN() - 1).getX() * getPoint(0).getY();
	buf_2 += getPoint(getN() - 1).getY() * getPoint(0).getX();

	return abs(buf_1 - buf_2) / 2;
}

Triangle::Triangle(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

//fixed without sqrt
bool Triangle::hasRightAngle() const {
	double a = pow(getPoint(0).getX() - getPoint(1).getX(), 2.0) + pow(getPoint(0).getY() - getPoint(1).getY(), 2.0);
	double b = pow(getPoint(0).getX() - getPoint(2).getX(), 2.0) + pow(getPoint(0).getY() - getPoint(2).getY(), 2.0);
	double c = pow(getPoint(1).getX() - getPoint(2).getX(), 2.0) + pow(getPoint(1).getY() - getPoint(2).getY(), 2.0);
	//fixed true
	if (a + b == c || a + c == b || b + c == a)
		return true;

	return false;
}

Trapezoid::Trapezoid(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

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

double RegularPolygon::perimeter() const {
	return getN() * segment(getPoint(0), getPoint(1));
}

double RegularPolygon::area() const {
	return perimeter() * segment(getPoint(0), getPoint(1)) / (4 * tan(M_PI / getN()));
}
