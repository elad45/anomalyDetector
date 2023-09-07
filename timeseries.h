// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

#define COMMA ','

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;
struct Column {
    string name;
    vector<float> values;
};
class TimeSeries {
    vector<Column> table;
public:
    // constructor
    TimeSeries(const char *CSVfileName);
    // prints the column of the table
    void printColumn(string name);
    // prints the row of the table
    void printRow(int time);
    // prints all the table
    void printTable();
    // returns a specific column by name
    const Column& getColumn(string name) const;
    // returns a specifi column by number
    const Column& getColumn(int i) const;
    // returns the size of the table
    const unsigned int featuresAmount() const;
    // returns the size of the column vector
    const unsigned int vectorSize() const;
    // returns the table of the object
    vector<Column> getTable() const;

};


#endif /* TIMESERIES_H_ */