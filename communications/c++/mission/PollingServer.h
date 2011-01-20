/*
 * PollingServer.h
 *
 *  Created on: Jan 5, 2011
 *      Author: black
 *      File that contains the definition for the PollingServer class.
 *      This class is used to set up and manage a tcp server.

 This material is based on research sponsored by DARPA under agreement
 number FA8750-10-2-0165. The U.S. Government is authorized to
 reproduce and distribute reprints for Governmental purposes
 notwithstanding any copyright notation thereon.

 The views and conclusions contained herein are those of the authors
 and should not be interpreted as necessarily representing the official
 policies or endorsements, either expressed or implied, of DARPA or the
 U.S. Government.

 ==========================================================================

 For use by entities other than the U.S. Government the following
 additional limitations apply:

 Copyright (c) 2011, Clarkson University
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above
 copyright notice, this list of conditions and the following
 disclaimer in the documentation and/or other materials provided
 with the distribution.

 * Neither the name of the Clarkson University nor the names of its
 contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 (license copied from http://www.opensource.org/licenses/bsd-license)


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
	PollingServer(int startThread=1,int portNumber=POLLING_SERVER_DEFAULT_PORT);
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
