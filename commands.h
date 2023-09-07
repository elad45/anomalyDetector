// Michael Alayev 207117045
// Elad Vizenblit 315944611

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
virtual string read() = 0;

//it writes a strings to the client and the second one write floats
virtual void write(string text) = 0;

virtual void write(float f) = 0;

virtual void read(float *f) = 0;

virtual ~DefaultIO() {}

// you may add additional methods here
void downloadFile(string fileName) {
    ofstream out(fileName);
    string s=read();
    while(s!="done\n" && s!="done"){
        out<<s<<endl;
        s=read();
    }
    out.close();
}
};
class StandardIO : public DefaultIO {

public:
StandardIO(){}
virtual string read(){
    string s;
    cin>>s;
    return s;
}
virtual void write(string text){
    cout<<text;
}

virtual void write(float f){
    cout<<f;
}

virtual void read(float* f){
    cin>>*f;
}

};

// you may add here helper classes

struct anomalyUnionReport {
int start;
int end;
string description;
};

struct MemberSaver {
HybridAnomalyDetector *hDetector;
vector<AnomalyReport> anomalyReports;
vector<anomalyUnionReport*> aur;
float testSize;
};

// you may edit this class
class Command {
protected:
DefaultIO *dio;
//I added this string seems like he missed it. it's in the UML.
const string m_description;
public:
Command(DefaultIO *dio, const string m_description) : dio(dio), m_description(m_description) {}

virtual void execute() = 0;

string getDescription() {
    return this->m_description;
}

virtual ~Command() {}
};
// implement here your command classes

//option 1 - a read from file function isn't implemented yet.
class UploadTimeSeries : public Command {
public:
//constructor
UploadTimeSeries(DefaultIO *dio) : Command(dio, "upload a time series csv file") {}

virtual void execute() {
    dio->write("Please upload your local train CSV file.\n");
    dio->downloadFile("anomalyTrain.csv"); //debug
    dio->write("Upload complete.\n");
    dio->write("Please upload your local test CSV file.\n");
    dio->downloadFile("anomalyTest.csv");
    dio->write("Upload complete.\n");
}
};

//option 2 - changes threshold - didn't check but I think it should work good
class AlgoSettings : public Command {
MemberSaver *memberSaver;
public:
//constructor
AlgoSettings(DefaultIO *dio, MemberSaver *memberSaver) : Command(dio, "algorithm settings"),
                                                                 memberSaver(memberSaver) {}

virtual void execute() {
    //requested text
    float newThreshold = 0;
    int flag = 1;
    //loops until a change has been made
    HybridAnomalyDetector *hDetector = new HybridAnomalyDetector;
    memberSaver->hDetector = hDetector;
    while (flag) {
        dio->write("The current correlation threshold is ");
        dio->write(memberSaver->hDetector->getCorrelationThreshold());
        dio->write(
                "\nType a new threshold\n"); //they didn't ask for a specific text so we have to check it..
        dio->read(&newThreshold);
        if (newThreshold > 0 && newThreshold < 1) {
            memberSaver->hDetector->setCorrelationThreshold(newThreshold);
            flag = 0;
        } else
            dio->write("please choose a value between 0 and 1.");
    }
}
};

//option 3
class detectAnomalies : public Command {
MemberSaver *memberSaver;
public:
//constructor
detectAnomalies(DefaultIO *dio, MemberSaver *memberSaver) : Command(dio, "detect anomalies"),
                                                                    memberSaver(memberSaver) {}
void unionReports() {
    vector<AnomalyReport> anomalyReports = memberSaver->anomalyReports;
    int size = anomalyReports.size();
    int i = 0;
    while (i < size) {
        string currentDescription = anomalyReports.at(i).description;
        anomalyUnionReport *newReport = new anomalyUnionReport;
        newReport->start = anomalyReports.at(i).timeStep;
        newReport->end = anomalyReports.at(i).timeStep;
        for (; (i + 1 < size) && (anomalyReports.at(i + 1).description == currentDescription); i++) {
            newReport->end += 1;
        }
        newReport->description = currentDescription;
        memberSaver->aur.push_back(newReport);
        i++;
    }
}
virtual void execute() {
    TimeSeries train("anomalyTrain.csv");
    TimeSeries test("anomalyTest.csv");
    HybridAnomalyDetector *hDetector = new HybridAnomalyDetector;
    memberSaver->hDetector = hDetector;
    memberSaver->hDetector->learnNormal(train); //crashes here
    memberSaver->anomalyReports = memberSaver->hDetector->detect(test);
    memberSaver->testSize = test.vectorSize();
    unionReports();
    dio->write("anomaly detection complete.\n");
}
};

//option 4
class displayResults : public Command {
MemberSaver *memberSaver;
public:
//constructor
displayResults(DefaultIO *dio, MemberSaver *memberSaver) :
Command(dio, "display results"),memberSaver(memberSaver) {}

virtual void execute() {
    for (int i = 0; i < memberSaver->anomalyReports.size(); i++) {
        dio->write(memberSaver->anomalyReports.at(i).timeStep);
        dio->write("\t");
        dio->write(memberSaver->anomalyReports.at(i).description + "\n");
    }
    dio->write("Done.\n");
}
};

//option 5
class uploadAnomalies : public Command {
MemberSaver *memberSaver;
public:
//constructor
uploadAnomalies(DefaultIO *dio, MemberSaver *memberSaver) :
Command(dio,"upload anomalies and analyze results"),memberSaver(memberSaver) {}

virtual void execute() {
    float TP = 0;
    float P = 0;
    float N = memberSaver->testSize;
    float anomalyReportSum = 0;
    dio->write("Please upload your local anomalies file.\n");
    string line = dio->read();
    const char *firstNumString;
    char *secondNumString;
    while (line != "done\n") {
        const char *firstNumString;
        const char *secondNumString;
        // converting string to char* with strtok
        char *token = &line[0];
        firstNumString = strtok(token, ",");
        // converting the string to integer
        int firstNum = atoi(firstNumString);
        secondNumString = strtok(NULL, "\n");
        // converting the string to integer
        int secondNum = atoi(secondNumString);
        memberSaver->aur = memberSaver->aur;
        for (int i = 0; i < memberSaver->aur.size(); i++) {
            if (secondNum >= memberSaver->aur.at(i)->start && memberSaver->aur.at(i)->end >= firstNum) {
                TP++;
            }
        }
        anomalyReportSum += secondNum - firstNum + 1;
        line = dio->read();
        P++;
    }
    N = N - anomalyReportSum;
    float TPR = ((int)(1000.0*TP / P))/1000.0f;
    float FPR=((int)(1000.0*(memberSaver->aur.size() - TP)/N))/1000.0f;
    dio->write("Upload complete.\n");
    dio->write("True Positive Rate: ");
    dio->write(TPR);
    dio->write("\n");
    dio->write("False Positive Rate: ");
    dio->write(FPR);
    dio->write("\n");
}
};

//option 6
class exitCommand : public Command {
public:
exitCommand(DefaultIO *dio) : Command(dio, "exit") {}

virtual void execute() {
    dio->write(this->m_description);
}
};

#endif /* COMMANDS_H_ */
