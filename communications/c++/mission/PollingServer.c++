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
 */

#include "mex.h"
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
PollingServer::PollingServer(int portNumber) :
	running(0),
	serverSocket(0),
	numberInBuffer(0) {

	mexPrintf("Starting polling server\n");

	// Initialize the port number to use
	setPort(portNumber);

	// initialize the buffer
	memset(buf,0,BUFFER_SIZE);


	// Set up the mutex needed to make sure we play nice when sharing data.
	int error = pthread_mutex_init(&mutexUpdateData, NULL);
	if (error) {
		// Could not get a mutex. Return with sad face.
		mexPrintf("Error number from trying to initialize the mutex: %d\n",
				error);
		mexErrMsgTxt("Error initializing the mutex\n");
		return;
	}

	if (createAndInitializeSocket()) {
		// Could not create and set up the socket for the polling loop.
		destroyMutex();
		mexErrMsgTxt("Unable to create a socket.\n");
		return;
	}

	// Create the thread and return to the calling program.
	setRunning(1);   // Assume it is okay to receive information.
	pthread_create(&pollingThread, NULL, PollingServer::pollingLoop,
			(void*) this);

}

/* *********************************************************************
 * class destructor.
 * ******************************************************************** */
PollingServer::~PollingServer(){

	//mexPrintf("Deleting the PollingServer object. Turn off the socket first.");

	// The server needs to be shut down. Destroy the mutex and shut down the socket.
	if (!shutdownSocket(0)) {
		mexWarnMsgTxt("Unable to shutdown the socket.\n");
	} else {
		mexPrintf("Turned off the socket server.\n");
	}

	if(running != 0) {
		// The mutex is valid and the process was running a thread for this.
		destroyMutex();
		mexPrintf("Exiting the thread.\n");
		pthread_exit(NULL);
	}

}


void *PollingServer::pollingLoop(void *ptr) {
	/** *********************************************************************
	 * Static routine to act as a pointer for starting up a new thread.
	 * ******************************************************************** **/

	PollingServer *thePollingServer = (PollingServer*) ptr;
	thePollingServer->pollingLoop();    // Start the polling loop.

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

	while (getRunning()>0) {
		// It is okay to poll the socket
		// Wait for a new connection.
	    addressSize = sizeof(remoteConnection);
	    s = accept(serverSocket,(sockaddr*)&remoteConnection,&addressSize);

	    // Got a connection. Clear out the buffer and get the new information.
	    memset(localBuffer,0,POLLING_SERVER_BUFFER_SIZE);
	    nread = recv(s,localBuffer, POLLING_SERVER_BUFFER_SIZE, 0);


	    if (nread == -1) {
	    	mexPrintf("Accepted and read but got nothing.");
	      continue;               /* Ignore failed request */
	    } else {
	    	mexPrintf("Heard:\n%s\n",localBuffer);
	    }

	    char host[NI_MAXHOST], service[NI_MAXSERV];

	    // Get the information about the host that initiated the connection.
	    remote = getnameinfo((struct sockaddr *) &remoteConnection,
				 sizeof(remoteConnection), host, NI_MAXHOST,
				 service, NI_MAXSERV, NI_NUMERICSERV);
	    if (remote == 0) {
	    	mexPrintf("Received %d  bytes from %s : %d\n",nread,host,service);
	    }

	    else {
	    	mexPrintf("Error - getnameinfo: %s\n",gai_strerror(remote));
	    }

	    if (send(s,buf,nread,0) != nread) {
	    	mexPrintf("Error sending response");
	    }

	    shutdown(s, SHUT_RDWR);
	    close(s);

	}

	delete this;

}


int PollingServer::exchangeInformation(){
	/** *********************************************************************
	 * Method to exchange data from a local buffer into a matlab variable.
	 * ******************************************************************** **/

	pthread_mutex_lock(&(mutexUpdateData));
	sleep(2);
	mexPrintf("Got information\n");
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
		mexWarnMsgTxt("Error - Could not create a socket.");
		return (1);
	}

	// Set up the local connection information.
	memset(&localConnection, 0, sizeof(struct sockaddr_in));
	localConnection.sin_family = AF_INET;
	localConnection.sin_port = htons(getPort());
	hp = gethostbyname("localhost");

	mexPrintf("host information\nname: %s\nname list: %s\nusing: %s\n",
			hp->h_name, hp->h_addr_list[0], hp->h_addr);

	memcpy((char*) &localConnection.sin_addr, (char*) hp->h_addr_list[0],
			hp->h_length);

	// Bind the socket to the local address and port.
	if (bind(serverSocket, (struct sockaddr*) &localConnection,
			sizeof(localConnection)) != 0) {
		mexPrintf("Error returned to bind address: %d\n",errno);
		mexWarnMsgTxt("Could not bind an address");
		return (1);
	}

	// Start listening on the socket.
	if (listen(serverSocket, 100) != 0) {
		mexWarnMsgTxt("Could not post the listen with the desired backlog.");
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
		if(!silent) {
			mexPrintf("Shutting down the tcp server.\n");
		}
		close(serverSocket);
		value = shutdown(serverSocket, SHUT_RDWR);
	} else if(!silent) {
			mexWarnMsgTxt(
					"Cannot shut down the server. It has not been started yet.\n");
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

