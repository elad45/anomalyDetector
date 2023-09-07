// Michael Alayev 207117045
// Elad Vizenblit 315944611

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual vector<AnomalyReport> detect(const TimeSeries& ts) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
