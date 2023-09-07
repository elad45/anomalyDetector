// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include <iostream>
#include <cmath>
#include "anomaly_detection_util.h"
using namespace std;
// E a new function which computes the average values of the float array given by the user
float E(float* x, int size);

float E(float* x, int size){
    // sum will be the sum of the array
    float sum = 0;
    // average will be the average value of all the variables in the array
    float average;
        for (int i = 0;i<size;i++){
        sum += x[i];
    }
    average = sum/size;
    return average;
}

float var(float* x, int size){
    // sum1 will be the sum of the array
    float sum1 = 0;
    // sum2 will be a sigma of (xi - "the expectation value")
    float sum2 = 0;
    // totalSum will be the expectation value
    float expValue;
    // variance will be obviously the variance returned value
    float variance;
    for (int i = 0;i<size;i++){
        sum1 += x[i];
    }

    expValue = ((float)1/(float)size)*sum1; 

        for (int i = 0;i<size;i++){
            sum2 += (x[i] - expValue)*(x[i] - expValue);
    }

    variance = ((float)1/(float)size)*sum2;
    return variance;
}

float cov(float* x, float* y, int size){
    // sum will be the sum of the array
    float sum = 0;
    // the average value of array x
    float avgX = E(x, size);
    // the average value of array y
    float avgY = E(y, size);
    // computation of the convariance formula
    for (int i = 0; i < size; i++){
        sum += ((x[i] - avgX)*(y[i]-avgY));
    }
    return (sum/size);
}

float pearson(float* x, float* y, int size){
    float sqrt1 = sqrt(var(x,size)*var(y,size));
    return cov(x,y,size)/sqrt1;
}

Line linear_reg(Point** points, int size){
    // a and b will be the variables of the linear equation
    float a;
    float b;
    // the next arrays will be the x and y arrays from the array of points
    float x[size];
    float y[size];
    // the new line which will be returned by the function
    Line line;
    for(int i = 0;i<size; i++){
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    // computation of a and b variables according to the formulas
    a = cov(x,y,size)/var(x,size);
    b = E(y,size) - (a*E(x,size));
    line.a = a;
    line.b = b;
    return line;
}

float dev(Point p,Point** points, int size){
    // line is the linear regression
    Line line = linear_reg(points, size);
    // linearY is the value of f(x)
    float linearY = (line.a)*p.x + line.b;
    // the returned value is the deviation from our linear regression
    return fabs(p.y - linearY);
}

float dev(Point p,Line l){
    // linearY is the value of f(x)
    float linearY = (l.a)*p.x + l.b;
    // the returned value is the deviation from our linear regression
    return fabs(p.y - linearY);
}
