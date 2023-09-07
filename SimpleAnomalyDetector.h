// Michael Alayev 207117045
// Elad Vizenblit 315944611

#ifndef _SIMPLEANOMALYDETECTOR_H_
#define _SIMPLEANOMALYDETECTOR_H_
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
    Point** points;
    float numOfPoints;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> cf;
    float threshold = 0.9;
public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);

    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    void setCorrelationThreshold(float threshold) {
        this->threshold = threshold;
    }
    float getCorrelationThreshold() {
        return this->threshold;
    }
};


#endif /* _SIMPLEANOMALYDETECTOR_H_ */