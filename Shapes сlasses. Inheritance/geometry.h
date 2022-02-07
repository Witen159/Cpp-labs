#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
class Point {
public:
	Point();

	Point(float first, float second);

	Point(const Point& dot);

	float getX() const;

	float getY() const;

	Point& operator= (const Point& dot);

private:
	float x;
	float y;
};

class PolygonalChain {
public:
	PolygonalChain(const int number, Point* points);

	PolygonalChain(const PolygonalChain& chain);

	PolygonalChain& operator=(const PolygonalChain& chain);

	int getN() const;

	Point getPoint(const int index) const;

	vector<Point> getTops() const;

	double segment(const Point p1, const Point p2) const;

	virtual double perimeter() const;

	virtual ~PolygonalChain();
		//fixed destructor shouldn't do it, use default one

private:
	//fixed N is useless
	vector<Point> tops;
};

class ClosedPolygonalChain : public PolygonalChain {
public:
	ClosedPolygonalChain(const int number, Point* points);

	double perimeter() const override;
};

class Polygon : public ClosedPolygonalChain {
public:
	Polygon(const int number, Point* points);

	virtual double area() const;
};

class Triangle : public Polygon {
public:
	Triangle(const int number, Point* points);
	
	bool hasRightAngle() const;
};

class Trapezoid : public Polygon {
public:
	Trapezoid(const int number, Point* points);

	double height() const;
	
private:
	double distance(Point p1, Point p2, Point p3) const;
};

class RegularPolygon : public Polygon {
public:
	RegularPolygon(const int number, Point* points);

	double perimeter() const override;

	double area() const override;
};
