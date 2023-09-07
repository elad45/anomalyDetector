// Michael Alayev 207117045
// Elad Vizenblit 315944611

#include "CLI.h"


CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
//for every new command we have to push it here by the right order
    commandsVector.push_back(new UploadTimeSeries(dio));
    commandsVector.push_back(new AlgoSettings(dio, memberSaver));
    commandsVector.push_back(new detectAnomalies(dio, memberSaver));
    commandsVector.push_back(new displayResults(dio, memberSaver));
    commandsVector.push_back(new uploadAnomalies(dio, memberSaver));
    commandsVector.push_back(new exitCommand(dio));
}

void CLI::start() {
    int option = 0;
    while (option != 6) {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        for (int i = 0; i < commandsVector.size(); i++) {
            dio->write(i + 1);
            dio->write(".");
            dio->write(this->commandsVector.at(i)->getDescription() + "\n");
        }
        string s = dio->read();
        option = s[0] - '0';
        commandsVector[option - 1]->execute(); //the problem continues here
    }
}

CLI::~CLI() {
    for (int i = 0; i < commandsVector.size(); i++) {
        delete commandsVector[i];
    }
}

