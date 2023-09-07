// Michael Alayev 207117045
// Elad Vizenblit 315944611

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
float dist(const Point& a, const Point& b);
bool is_inside(const Circle& c, const Point& p);
Point get_circle_center(float bx, float by, float cx, float cy);
Circle circle_from(const Point& A, const Point& B, const Point& C);
Circle circle_from(const Point& A, const Point& B);
Circle min_circle_trivial(vector<Point>& P);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
