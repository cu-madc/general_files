/*
 * trial.c++
 *
 *  Created on: Jan 14, 2011
 *      Author: black
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
#include <string.h>

#include "XMLMessageNetwork.h"
#include "XMLIncomingDIF.h"
#include "PollingServer.h"
#include "Sender.h"

int main(int argc, char **argv) {
	char name[100];
	name[0] = 0;

	if (argc < 2) {
		std::cout << "usage: " << argv[0] << " server|client" << std::endl;
		return (1);
	}

	if (strcmp(argv[1], "server") == 0) {
		PollingServer *server = new PollingServer(0);

	}

	else if (strcmp(argv[1], "client") == 0) {
		XMLMessageNetwork *trial = new XMLMessageNetwork;
		XMLMessageNetwork *message = new XMLMessageNetwork;

		trial->setNetworkID(2);
		trial->setProbSuccessfulTransmission(0.5);
		trial->createRootNode();
		trial->createDimensions();
		trial->setNetworkIDNode();
		trial->setProbSuccessNode();
		trial->xml2Char();
		const char* theXML = trial->getBuffer();

		Sender *theSender = new Sender;
		theSender->setHostName("localhost");
		theSender->setPortNumber(4554);
		theSender->createSocket();
		theSender->sendBuffer(theXML,strlen(theXML));

		delete message;
		delete trial;

	}

	else {
		std::cout << "usage: " << argv[0] << " server|client" << std::endl;
		return (1);
	}
	return (0);

}

