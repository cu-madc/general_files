/*
 * DataPolling.c++
 *
 *  Created on: Jan 7, 2011
 *      Author: black
 */

#include "mex.h"
#include <iostream>
#include "string.h"
#include "PollingServer.h"
#include "XMLMessageNetwork.h"

PollingServer* theServer = NULL;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	char *input_specifier;

	//mexPrintf("Starting up\n");

	/* **********************************************************************************
	 * This is very preliminary. At this point the routine expects just one argument
	 * when called and returns none. The one argument is a string and should either be
	 * "start" or "end." If it is "start" then it creates an object and starts the socket
	 * and polling loop. If it is "end" then it sets a variable to let the polling loop
	 * exit the next time it goes through.
	 * ********************************************************************************** */

	if ((nrhs == 1) && (mxIsChar(prhs[0]))) {
		input_specifier = mxArrayToString(prhs[0]);
		//mexPrintf("Got one character: *%s*\n", input_specifier);


		if (strcmp(input_specifier, "start") == 0) {
			//mexPrintf("Starting server\n");
			theServer = new PollingServer;
			if(theServer->getRunning()==0) {
				// There was an error trying to start the polling server.
				delete theServer;
				theServer = NULL;
			}

		}

		else if ((theServer != NULL) && (strcmp(input_specifier, "end") == 0)) {
			//mexPrintf("Stopping server\n");
			theServer->setRunning(-1);
		}

		else {
			mexErrMsgTxt("Unrecognized command.");
		}

	} else {
		mexErrMsgTxt("Unrecognized command");
	}

	return;

}




