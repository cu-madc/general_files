/*
 * Sender.c++
 *
 *  Created on: Jan 20, 2011
 *      Author: black
 *
 *
 *      Class to act as a client using a tcp connection. This
 *      will take a machine name, a port number, and data and
 *      send it to the destination using tcp.
 *
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

#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "Sender.h"

Sender::Sender() {

}

Sender::~Sender() {

}

int Sender::createSocket(){
	createSocket(hostname,portNumber);
}

int Sender::createSocket(const char* newHostName, int newPortNumber) {
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		std::cout << "Error - Could not create a socket." << std::endl;
		return (EXIT_FAILURE);
	}

	memset(&localConnection, 0, sizeof(struct sockaddr_in));
	localConnection.sin_family = AF_INET;
	localConnection.sin_port = htons(4554);
	hp = gethostbyname("localhost");

	memcpy((char*) &localConnection.sin_addr, (char*) hp->h_addr_list[0],
			hp->h_length);

	if (connect(clientSocket, (sockaddr *) &localConnection,
			sizeof(localConnection)) != 0) {
		std::cout << "Error - cannot connect to host." << std::endl;
		return (EXIT_FAILURE);
	}

	return (0);
}

int Sender::closeSocket() {

	if (shutdown(clientSocket, SHUT_RDWR) != 0) {
		std::cout << "Error shutting down the socket." << std::endl;
		return (EXIT_FAILURE);
	}
	close(clientSocket);
	return (0);

}

int Sender::sendBuffer(const char* buffer, int size) {
	return (send(clientSocket, buffer, size, 0));
}

int Sender::recvBuffer() {
	return (recv(clientSocket, localBuffer, BufferSize, 0));
}

