/*
 * PollingServer.h
 *
 *  Created on: Jan 5, 2011
 *      Author: black
 *      File that contains the definition for the PollingServer class.
 *      This class is used to set up and manage a tcp server.
 */

#ifndef POLLING_SERVER_
#define POLLING_SERVER_

// Threads related include files
#include <pthread.h>

// socket related include files
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netdb.h>



#define POLLING_SERVER_DEFAULT_PORT 4554

class PollingServer {

public:
	PollingServer(int portNumber=POLLING_SERVER_DEFAULT_PORT);
	~PollingServer();
	static void *pollingLoop(void *ptr);     // Method that contains the separate thread started for the server.
	void pollingLoop();                      // Specific to the class object. Handles the polling of the socket.
	int createAndInitializeSocket();         // Creates and sets up the socket.
	int shutdownSocket(int silent=1);        // Shuts down the socket.
	int destroyMutex();                      // Destroys the mutex creates in the constructor.
	int exchangeInformation();               // Copy local buffer information over to matlab variables.

	void setRunning(const int value) {
		// Sets the value of the running variable.
		running = value;
	}
	const int getRunning() {
		// Returns the value of the running variable.
		return (running);
	}

	void setPort(const int value) {
		// Sets the value of the port to use for the socket
		serverPort = value;
	}

	const int getPort() {
		//Returns the value of the port used for the socket
		return(serverPort);
	}

protected:
	pthread_t pollingThread;                  // For keeping track of the polling thread.
	pthread_mutex_t mutexUpdateData;          // The mutex used in the polling thread.

private:
	// Variables for the state of the polling process
	int running;                              // If true then keep polling the socket.

	// Variables for setting up and keeping track of the sockets.
	struct sockaddr_in localConnection;       // struct with info on the server's socket.
	int myAddrInfo;                           // descriptor returned when setting up the socket.
	struct hostent *hp;                       // information about this host.
	int serverSocket;                         // file descriptor for the socket.

	static const int DEFAULT_PORT = POLLING_SERVER_DEFAULT_PORT;     // Default port number to use
	int serverPort;                                                  // The server port to use.

	// Variables used to define and manipulate the buffer used to get information through the socket.
	static const int BUFFER_SIZE = 4096;     // Max size of the buffer
	char buf[BUFFER_SIZE];                   // buffer used to
	int  numberInBuffer;                     // Number of bytes used in buf

};

#endif /* POLLING_SERVER_ */
