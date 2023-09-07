// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include "timeseries.h"
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>


TimeSeries::TimeSeries(const char *CSVfileName) {
    int index = 0;
    ifstream dataFile;
    dataFile.open(CSVfileName);
    // first line of the file
    string features;
    // specific feature in the line
    string specificFeature;
    getline(dataFile, features);
    // we will make "features" an iStream type named "stringStream", to use getline function properly
    istringstream stringStream(features);
    // the next while loop will insert columns into the table, with only the feature names inside it
    while (getline(stringStream, specificFeature, COMMA)) {
        Column temp;
        temp.name = specificFeature;
        this->table.push_back(temp);
    }
    // the next while loop will fill up the float values inside the columns of the table
    while (dataFile.good()) {
        string sNumber;
        string line;
        float iNumber;
        getline(dataFile, line);
        istringstream stringStream(line);
        // the next while loop will fill a whole line of float values into the columns of the table
        while (getline(stringStream, sNumber, COMMA)) {
            iNumber = stof(sNumber);
            this->table.at(index).values.push_back(iNumber);
            index++;
        }
        index = 0;
    }
}

// return a column by feature name
const Column &TimeSeries::getColumn(string name) const{
    for (auto &column: this->table) {
        if (column.name == name) {
            return column;
        }
    }
}

const unsigned int TimeSeries::featuresAmount() const{
    return this->table.size();
}
// counts the number of variables under a certain feature
const unsigned int TimeSeries::vectorSize() const{
    return this->table.at(0).values.size();
}

// return a column by index
const Column &TimeSeries::getColumn(int i) const{
    return this->table.at(i);
}

void TimeSeries::printColumn(string name) {
    int flag = 0;
    Column column = this->getColumn(name);
    for (auto &number: column.values) {
        cout << number << endl;
    }
}
vector<Column> TimeSeries::getTable() const{
    return this->table;
}

void TimeSeries::printRow(int time) {
    if (time == 0) {
        for (auto &column: this->table) {
            cout << column.name << " ";
        }
        cout << endl;
        return;
    }
    for (auto &column: this->table) {
        cout << column.values.at(time - 1) << " ";
    }
    cout << endl;
}

void TimeSeries::printTable() {
    for (int i = 0; i <= this->vectorSize(); i++) {
        this->printRow(i);
    }
}

