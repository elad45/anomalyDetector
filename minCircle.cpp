// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include "minCircle.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// Function dist: simply calculates the distance between 2 points
float dist(const Point &a, const Point &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
// Function is_inside: this function checks whether point p is located in circle c
bool is_inside(const Circle &c, const Point &p) {
    return dist(c.center, p) <= c.radius;
}

// Function is_valid_circle: checks whether a circle encloses the given points
bool is_valid_circle(const Circle &c, const vector<Point> &P) {
    // Iterating through all the points to check whether the points lie inside the circle or not
    for (const Point &p: P)
        if (!is_inside(c, p))
            return false;
    return true;
}

// Function circle_from: returns a unique circle that intersects three points
Circle circle_from(const Point &A, const Point &B, const Point &C) {
    Point I = get_circle_center(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y);

    I.x += A.x;
    I.y += A.y;
    return {I, dist(I, A)};
}

// The following two functions are used to find the equation of the circle when three points are given.

// Function  get_circle_center: gets a circle defined by 3 points
Point get_circle_center(float bx, float by, float cx, float cy) {
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return {(cy * B - by * C) / (2 * D), (bx * C - cx * B) / (2 * D)};
}

// Function circle_from: returns the smallest circle that intersects 2 points
Circle circle_from(const Point &A, const Point &B) {
    // Set the center to be the midpoint of A and B
    Point C = {(A.x + B.x) / 2, (A.y + B.y) / 2};

    // Set the radius to be half the distance AB
    return {C, dist(A, B) / 2};
}
// Function min_circle_trivial: finds the trivial minimum circle in case there are 1-3 points
Circle min_circle_trivial(vector<Point> &P) {
    assert(P.size() <= 3);
    if (P.empty()) {
        return {{0, 0}, 0};
    } else if (P.size() == 1) {
        return {P[0], 0};
    } else if (P.size() == 2) {
        return circle_from(P[0], P[1]);
    }

    // To check if the minimum circle can be determined by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circle_from(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    return circle_from(P[0], P[1], P[2]);
}
// Function welzl: the algorithm to find the minimum circle
Circle welzl(Point **points, vector<Point> R, size_t size) {
    // Base case when all points processed or |R| = 3
    if (size == 0 || R.size() == 3) {
        return min_circle_trivial(R);
    }

    // Pick a random point randomly
    int idx = rand() % size;
    Point randomPoint = *points[idx];

    // Put the picked point at the end of points
    // since it's more efficient than
    // deleting from the middle of the vector
    swap(points[idx], points[size - 1]);

    // Get the MEC circle d from the
    // set of points points - {p}
    Circle d = welzl(points, R, size - 1);

    // If d contains randomPoint, return d
    if (is_inside(d, randomPoint)) {
        return d;
    }

    // Otherwise, must be on the boundary of the MEC
    R.push_back(randomPoint);

    // Return the MEC for points - {p} and R U {p}
    return welzl(points, R, size - 1);
}
//Function welzl: a function to find the minimum circle
Circle findMinCircle(Point **points, size_t size) {
    return welzl(points, {}, size);
}