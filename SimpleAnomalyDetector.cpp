// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    // getting the amount of features in the given argument
    unsigned int tableSize = ts.featuresAmount();
    // getting the amount of valuables in a single feature
    unsigned int vectorSize = ts.vectorSize();
    // the next loop iterates on the columns of the argument's table
    for (int i = 0; i < tableSize; i++) {
        // iColumn is the current column we are iterating on
        Column iColumn = ts.getColumn(i);
        // maxCorrelation will represent the current max correlation of the current column with the others
        float maxCorrelation = 0;
        // columnIndex will represent the index of the column that has max correlation with "iColumn"
        int columnIndex = -1;
        for (int j = i + 1; j < tableSize; j++) {
            // checking correlation between iColumn and jColumn
            Column jColumn = ts.getColumn(j);
            float correlation = fabs(pearson(iColumn.values.data(), jColumn.values.data(), vectorSize));
            if ((correlation > maxCorrelation) && (correlation > 0.5)) {
                maxCorrelation = correlation;
                columnIndex = j;
            }
        }
        // if columnIndex != -1, we have found the "maxCorrelation" for "iColumn"
        if (columnIndex != -1) {
            // maxColumn has the highest correlation with iColumn
            Column maxColumn = ts.getColumn(columnIndex);
            // inserting the details of the findings into a new correlatedFeatures struct
            correlatedFeatures *cfTemp = new correlatedFeatures;
            cfTemp->feature1 = iColumn.name;
            cfTemp->feature2 = maxColumn.name;
            cfTemp->corrlation = maxCorrelation;
            // the next array will be used to store the feature valuables
            Point **points = new Point *[vectorSize];
            // the next for loop pushes the new feature points into an array
            for (int k = 0; k < vectorSize; k++) {
                Point *tempPoint = new Point(iColumn.values.at(k), maxColumn.values.at(k));
                points[k] = tempPoint;
            }
            cfTemp->points = points;
            cfTemp->numOfPoints = vectorSize;
            // initializing threshold
            float threshold = 0;
            // creating a linear regression according to SimpleAnomalyDetector
            cfTemp->lin_reg = linear_reg(points, vectorSize);
            for (int i = 0; i < vectorSize; i++) {
                float currentDev = dev(*points[i], cfTemp->lin_reg);
                if (currentDev > threshold) {
                    threshold = currentDev;
                }
            }
            cfTemp->threshold = 1.1 * threshold;
            this->cf.push_back(*cfTemp);
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    // the correlatedFeatures we are searching correlations from
    vector<correlatedFeatures> cfs = this->getNormalModel();
    // the reports that will be returned by the function
    vector<AnomalyReport> reports;
    // the vector size of the new table recieved in the argument
    int vectorSize = ts.vectorSize();
    // iteration over correlatedFeatures
    for (auto &cfTemp: cfs) {
        if (cfTemp.corrlation >= 0.9) {
            // the next 2 columns are correlated according to learnNormal
            Column leftColumn = ts.getColumn(cfTemp.feature1);
            Column rightColumn = ts.getColumn(cfTemp.feature2);
            // the next loop will check if the new points are anomalous according to the linear regression
            for (int i = 0; i < vectorSize; i++) {
                Point point(leftColumn.values.at(i), rightColumn.values.at(i));
                if (dev(point, cfTemp.lin_reg) > cfTemp.threshold) {
                    AnomalyReport newReport = AnomalyReport(cfTemp.feature1 + "-" + cfTemp.feature2, i + 1);
                    reports.push_back(newReport);
                }
            }
        }
    }
    return reports;
}
