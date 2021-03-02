#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
//cpp
class Point {
public:
	Point(float first = 0, float second = 0) : x(first), y(second) {}

	Point(const Point& dot) : x(dot.x), y(dot.y){}
	//todo that's strange
	void set_coordinates(float first, float second) {
		x = first;
		y = second;
	}

	float getX() const {
		return x;
	}

	float getY() const {
		return y;
	}

	Point& operator= (const Point& dot) {
		set_coordinates(dot.x, dot.y);
		return *this;
	}

private:
	float x;
	float y;
};

class PolygonalChain {
public:
	PolygonalChain(const int number = 0, Point* points = nullptr) : N(number) {
		for (int i = 0; i < N; i++)
			tops.push_back(points[i]);
	}

	PolygonalChain(const PolygonalChain& chain): N(chain.N), tops(chain.tops) {}
//todo S P A C E S
	PolygonalChain& operator=(const PolygonalChain& chain){
		N = chain.N;
		tops=chain.tops;
		
		return *this;
	}


	int getN() const {
		return N;
	}

	Point getPoint(const int index) const {
		return tops[index];
	}
	
	vector<Point> getTops() const{
		return tops;
	}

	double segment(const Point p1, const Point p2) const {
		return sqrt(pow(p1.getX() - p2.getX(), 2.0) + pow(p1.getY() - p2.getY(), 2.0));
	}

	virtual float perimeter() const {
		float P = 0;
		for (int i = 1; i < N; i++)
			P += segment(tops[i], tops[i - 1]);

		return P;
	}

	virtual ~PolygonalChain() {
		//todo destructor shouldn't do it, use default one
		tops.clear();
	}

private:
	//todo N is useless
	int N;
	vector<Point> tops;
};

class ClosedPolygonalChain : public PolygonalChain {
public:
	ClosedPolygonalChain(const int number = 0, Point* points = nullptr) : PolygonalChain(number, points) {}
	
	ClosedPolygonalChain(const ClosedPolygonalChain& chain): PolygonalChain(chain) {}

	float perimeter() const override {
	//todo copy-paste with PolygonalChain
		float P = 0;
		for (int i = 1; i < getN(); i++)
			P += segment(getPoint(i), getPoint(i-1));
		P += segment(getPoint(0), getPoint(getN() - 1));
		return P;
	}

	/*ClosedPolygonalChain& operator=(const ClosedPolygonalChain& chain){
		PolygonalChain::operator=(chain);
	}*/
};

class Polygon : public ClosedPolygonalChain {
public:
	Polygon(const int number = 0, Point* points = nullptr) : ClosedPolygonalChain(number, points) {}

	Polygon(const Polygon& figure) : ClosedPolygonalChain(figure) {}

	virtual double area() const{
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

	/*Polygon& operator=(const Polygon& figure){
		ClosedPolygonalChain::operator=(figure);
	}*/
};

class Triangle : public Polygon{
public:
	Triangle(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

	Triangle(const Triangle& triag) : Polygon(triag) {}
	//todo without sqrt
	bool hasRightAngle() const {
		double s = 2 * area();
		double a = segment(getPoint(0), getPoint(1));
		double b = segment(getPoint(0), getPoint(2));
		double c = segment(getPoint(1), getPoint(2));

		if (a * b == s || a * c == s || b * c == s)
			return 1;

		return 0;
	}

	/*Triangle& operator=(const Triangle& triag){
		Polygon::operator=(triag);
	}*/
};

class Trapezoid : public Polygon {
public:
	Trapezoid(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

	Trapezoid(const Trapezoid& trap) : Polygon(trap) {}

	double height() const{
		float x1 = getPoint(0).getX() - getPoint(1).getX();
		float y1 = getPoint(0).getY() - getPoint(1).getY();
		float x2 = getPoint(2).getX() - getPoint(3).getX();
		float y2 = getPoint(2).getY() - getPoint(3).getY();

		if (abs((x1 * x2 + y1 * y2) / (sqrt(pow(x1, 2.0) + pow(y1, 2.0)) * sqrt(pow(x2, 2.0) + pow(y2, 2.0)))) == 1)
			return distance(getPoint(0), getPoint(2), getPoint(3));
		else
			return distance(getPoint(0), getPoint(1), getPoint(2));
	}

	/*Trapezoid& operator=(const Trapezoid& trap){
		Polygon::operator=(trap);
	}*/

private:
	double distance(Point p1, Point p2, Point p3) const {
		Polygon triag(3, new Point[3]{p1, p2, p3});
		return triag.area() * 2 / segment(p2, p3);
	}
};

class RegularPolygon : public Polygon {
public:
	RegularPolygon(const int number = 0, Point* points = nullptr) : Polygon(number, points) {}

	RegularPolygon(const RegularPolygon& reg) : Polygon(reg) {}

	float perimeter() const override {
		return getN() * segment(getPoint(0), getPoint(1));
	}

	double area() const override {
		return perimeter() * segment(getPoint(0), getPoint(1)) / (4 * tan(M_PI / getN()));
	}

	/*RegularPolygon& operator=(const RegularPolygon& reg){
		Polygon::operator=(reg);
	}*/
};