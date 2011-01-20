/*
 * PollingServer.c++
 *
 *  Created on: Jan 5, 2011
 *      Author: black
 *
 *      File that contains the methods for the PollingServer class.
 *      This class is used to set up and manage a tcp server. Once
 *      started the polling method waits for an incoming message
 *      over the socket previously created.
 *
 *      This class also keeps track of the data that has come in.
 *      When a message comes in it has to be parsed and then a
 *      decision is made to determine what kind of data class should
 *      be created.
 *
 * This material is based on research sponsored by DARPA under agreement
 * number FA8750-10-2-0165. The U.S. Government is authorized to
 * reproduce and distribute reprints for Governmental purposes
 * notwithstanding any copyright notation thereon.
 *
 * The views and conclusions contained herein are those of the authors
 * and should not be interpreted as necessarily representing the official
 * policies or endorsements, either expressed or implied, of DARPA or the
 * U.S. Government.
 *
 * ==========================================================================
 *
 * For use by entities other than the U.S. Government the following
 * additional limitations apply:
 *
 * Copyright (c) 2011, Clarkson University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * * Neither the name of the Clarkson University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * (license copied from http://www.opensource.org/licenses/bsd-license)
 *
 *
 */

//#define MATLAB

#ifdef MATLAB
#include "mex.h"
#else
#include <iostream>
#endif

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "PollingServer.h"

#define DEBUG 0
#define POLLING_SERVER_BUFFER_SIZE 1024

/* *********************************************************************
 * Constructor for the PollingServer class.
 * ******************************************************************** */
PollingServer::PollingServer(int startThread,int portNumber) :
	running(0), serverSocket(0), numberInBuffer(0) {

#ifdef MATLAB
	mexPrintf("Starting polling server\n");
#else
	std::cout << "Starting polling server" << std::endl;
#endif

	// Initialize the port number to use
	setPort(portNumber);

	// initialize the buffer
	memset(buf, 0, BUFFER_SIZE);

	// Set up the mutex needed to make sure we play nice when sharing data.
	int error = pthread_mutex_init(&mutexUpdateData, NULL);
	if (error) {
		// Could not get a mutex. Return with sad face.
#ifdef MATLAB
		mexPrintf("Error number from trying to initialize the mutex: %d\n",
				error);
		mexErrMsgTxt("Error initializing the mutex\n");
#else
		std::cout << "Error number from trying to initialize the mutex: "
				<< error << std::endl << "Error initializing the mutex"
				<< std::endl;
#endif
		return;
	}

	if (createAndInitializeSocket()) {
		// Could not create and set up the socket for the polling loop.
		destroyMutex();
#ifdef MATLAB
		mexErrMsgTxt("Unable to create a socket.\n");
#else
		std::cout << "Unable to create a socket." << std::endl;
#endif
		return;
	}

	// Create the thread and return to the calling program.
	setRunning(1); // Assume it is okay to receive information.
	if (startThread) {
		pthread_create(&pollingThread, NULL, PollingServer::pollingLoop,
				(void*) this);
	} else {
		pollingLoop();
	}

}

/* *********************************************************************
 * class destructor.
 * ******************************************************************** */
PollingServer::~PollingServer() {

	//mexPrintf("Deleting the PollingServer object. Turn off the socket first.");

	// The server needs to be shut down. Destroy the mutex and shut down the socket.
	if (!shutdownSocket(0)) {
#ifdef MATLAB
		mexWarnMsgTxt("Unable to shutdown the socket.\n");
#else
		std::cout << "Unable to shutdown the socket." << std::endl;
#endif
	} else {
#ifdef MATLAB
		mexPrintf("Turned off the socket server.\n");
#else
		std::cout << "Turned off the socket server." << std::endl;
#endif
	}

	if (running != 0) {
		// The mutex is valid and the process was running a thread for this.
		destroyMutex();
#ifdef MATLAB
		mexPrintf("Exiting the thread.\n");
#else
		std::cout << "Exiting the thread." << std::endl;
#endif
		pthread_exit(NULL);
	}

}

void *PollingServer::pollingLoop(void *ptr) {
	/** *********************************************************************
	 * Static routine to act as a pointer for starting up a new thread.
	 * ******************************************************************** **/

	PollingServer *thePollingServer = (PollingServer*) ptr;
	thePollingServer->pollingLoop(); // Start the polling loop.

}

void PollingServer::pollingLoop() {
	/** *********************************************************************
	 * Keep polling the open socket that has been set up prior to calling
	 * this method.
	 * ******************************************************************** **/

	socklen_t addressSize;
	int s;
	int remote;
	ssize_t nread;
	struct sockaddr_in remoteConnection;
	char localBuffer[POLLING_SERVER_BUFFER_SIZE];

	while (getRunning() > 0) {
		// It is okay to poll the socket
		// Wait for a new connection.
		addressSize = sizeof(remoteConnection);
		s = accept(serverSocket, (sockaddr*) &remoteConnection, &addressSize);

		// Got a connection. Clear out the buffer and get the new information.
		memset(localBuffer, 0, POLLING_SERVER_BUFFER_SIZE);
		nread = recv(s, localBuffer, POLLING_SERVER_BUFFER_SIZE, 0);

		if (nread == -1) {
#ifdef MATLAB
			mexPrintf("Accepted and read but got nothing.\n");
#else
			std::cout << "Accepted and read but got nothing." << std::endl;
#endif
			continue; /* Ignore failed request */
		} else {
#ifdef MATLAB
			mexPrintf("Heard:\n%s\n",localBuffer);
#else
			std::cout << "Heard: " << localBuffer << std::endl;
#endif
		}

		char host[NI_MAXHOST], service[NI_MAXSERV];

		// Get the information about the host that initiated the connection.
		remote = getnameinfo((struct sockaddr *) &remoteConnection,
				sizeof(remoteConnection), host, NI_MAXHOST, service,
				NI_MAXSERV, NI_NUMERICSERV);
		if (remote == 0) {
#ifdef MATLAB
			mexPrintf("Received %d  bytes from %s : %d\n",nread,host,service);
#else
			std::cout << "Received " << nread << " bytes from " << host
					<< " : " << service << std::endl;
#endif
		}

		else {
#ifdef MATLAB
			mexPrintf("Error - getnameinfo: %s\n",gai_strerror(remote));
#else
			std::cout << "Error - getnameinfo: " << gai_strerror(remote)
					<< std::endl;
#endif
		}

		if (send(s, buf, nread, 0) != nread) {
#ifdef MATLAB
			mexPrintf("Error sending response\n");
#else
			std::cout << "Error sending response" << std::endl;
#endif
		}

		shutdown(s, SHUT_RDWR);
		close(s);

	}

	delete this;

}

int PollingServer::exchangeInformation() {
	/** *********************************************************************
	 * Method to exchange data from a local buffer into a matlab variable.
	 * ******************************************************************** **/

	pthread_mutex_lock(&(mutexUpdateData));
	sleep(2);
#ifdef MATLAB
	mexPrintf("Got information\n");
#else
	std::cout << "Got information" << std::endl;
#endif
	pthread_mutex_unlock(&(mutexUpdateData));

}

int PollingServer::createAndInitializeSocket() {
	/** *********************************************************************
	 * Set up and create the socket used for waiting for information.
	 * If everything is okay returns zero. Otherwise it returns 1.
	 * ******************************************************************** **/

	// Define the socket.
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
#ifdef MATLAB
		mexWarnMsgTxt("Error - Could not create a socket.\n");
#else
		std::cout << "Error - Could not create a socket." << std::endl;
#endif
		return (1);
	}

	// Set up the local connection information.
	memset(&localConnection, 0, sizeof(struct sockaddr_in));
	localConnection.sin_family = AF_INET;
	localConnection.sin_port = htons(getPort());
	hp = gethostbyname("localhost");

#ifdef MATLAB
	mexPrintf("host information\nname: %s\nname list: %s\nusing: %s\n",
			hp->h_name, hp->h_addr_list[0], hp->h_addr);
#else
	std::cout << "Host information: " << hp->h_name << " name list: "
			<< hp->h_addr_list[0] << " using: " << hp->h_addr << std::endl;
#endif

	memcpy((char*) &localConnection.sin_addr, (char*) hp->h_addr_list[0],
			hp->h_length);

	// Bind the socket to the local address and port.
	if (bind(serverSocket, (struct sockaddr*) &localConnection,
			sizeof(localConnection)) != 0) {
#ifdef MATLAB
		mexPrintf("Error returned to bind address: %d\n",errno);
		mexWarnMsgTxt("Could not bind an address\n");
#else
		std::cout << "Error returned to bind address: " << errno
				<< "Could not bind an address" << std::endl;
#endif
		return (1);
	}

	// Start listening on the socket.
	if (listen(serverSocket, 100) != 0) {
#ifdef MATLAB
		mexWarnMsgTxt("Could not post the listen with the desired backlog.\n");
#else
		std::cout << "Could not post the listen with the desired backlog."
				<< std::endl;
#endif
		return (1);
	}

	return (0);
}

int PollingServer::shutdownSocket(int silent) {
	/** *********************************************************************
	 * Close down and turn off the socket. Returns zero if successful.
	 * Otherwise it returns -1.
	 * ******************************************************************** **/
	int value = 0;

	if (serverSocket) {
		// The server socket has been defined. Close it and then shut it down.
		if (!silent) {
#ifdef MATLAB
			mexPrintf("Shutting down the tcp server.\n");
#else
			std::cout << "Shutting down the tcp server." << std::endl;
#endif
		}
		close(serverSocket);
		value = shutdown(serverSocket, SHUT_RDWR);
	} else if (!silent) {
#ifdef MATLAB
		mexWarnMsgTxt(
				"Cannot shut down the server. It has not been started yet.\n");
#else
		std::cout
				<< "Cannot shut down the server. It has not been started yet."
				<< std::endl;
#endif
	}

	serverSocket = 0;
	return (value);

}

/* *********************************************************************
 * Destory the mutex that has been created in the constructor.
 * ******************************************************************** */
int PollingServer::destroyMutex() {
	return (pthread_mutex_destroy(&mutexUpdateData));
}

