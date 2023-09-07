// Michael Alayev 207117045
// Elad Vizenblit 315944611

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	// you can add data members
    //vector that holds Command* for the execution (it's the Command** we were asked to implement)
    vector <Command*> commandsVector;
    MemberSaver* memberSaver = new MemberSaver;

public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
