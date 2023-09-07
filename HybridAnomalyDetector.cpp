// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    // cfs is correlatedFeatures we are searching correlations from
    vector<correlatedFeatures> cfs = this->getNormalModel();
    // the reports that will be returned by the function
    vector<AnomalyReport> reports;
    // the vector size of the new table recieved in the argument
    int vectorSize = ts.vectorSize();
    // iteration over correlatedFeatures
    for (auto &cfTemp: cfs) {
            // the next 2 columns are correlated according to learnNormal
            Column leftColumn = ts.getColumn(cfTemp.feature1);
            Column rightColumn = ts.getColumn(cfTemp.feature2);
            for (int i = 0; i < vectorSize; i++) {
                Point point(leftColumn.values.at(i), rightColumn.values.at(i));
                if (cfTemp.corrlation >= threshold) {
                    if (dev(point, cfTemp.lin_reg) > cfTemp.threshold) {
                        AnomalyReport newReport = AnomalyReport(cfTemp.feature1 + "-" + cfTemp.feature2, i + 1);
                        reports.push_back(newReport);
                    }
                }
                if ((cfTemp.corrlation > 0.5) && (cfTemp.corrlation < threshold)){
                    Circle minCircle = findMinCircle(cfTemp.points, cfTemp.numOfPoints);
                    minCircle.radius *= 1.1;
                    // computing distance between the circle's center to the point
                    if (dist(minCircle.center, point) > minCircle.radius){
                        // if the radius received is bigger, we got an anomaly point
                        AnomalyReport newReport = AnomalyReport(cfTemp.feature1 + "-" + cfTemp.feature2, i + 1);
                        reports.push_back(newReport);
                    }
                }
            }
    }
    return reports;
}