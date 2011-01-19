/*
 * XMLMessageNetwork.h
 *
 *  Created on: Jan 14, 2011
 *      Author: black
 *
 *      This is a class to keep track of the data that come in through
 *      a socket connection that is meant for the network associated
 *      with a vacuum. This class has to parse the tree and figure out
 *      what the relevant values are and for which vacuum it is meant for.
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
 *
 *
 */

#ifndef XMLMESSAGENETWORK_H_
#define XMLMESSAGENETWORK_H_

#include "XMLIncomingDIF.h"

class XMLMessageNetwork : public XMLIncomingDIF {

public:
	XMLMessageNetwork();
	~XMLMessageNetwork();

	// methods to get and set the values of the different values that are being tracked.
	const int getNetworkID() { return networkID; }
	void setNetworkID(int value) { networkID = value; }
	const float getProbSuccessfulTransmission() {return probSuccessfulTransmission; }
	void setProbSuccessfulTransmission(float value) { probSuccessfulTransmission = value; }

	// Methods associated with creating and maintaining the xml tree.
	void createRootNode();     // Creates a root node for the xml tree.
	void createDimensions();   // Creates the "dimensions" field in the xml tree.
	void setNetworkIDNode();   // Adds the number to determine which vacumm this is for.
	void setProbSuccessNode(); // Prob. that a message on the network for this vacuum
	                           // is successfully passed along.

	void copyXMLTree(xmlDocPtr existingDocument);  // makes a copy of the parsed xml document.

protected:

private:

	// information specific to the network data structure
	int networkID;
	double probSuccessfulTransmission;
	xmlNodePtr objectClassNode;
	xmlNodePtr dimensionsNode;
	xmlNodePtr networkIDNode;
	xmlNodePtr probSuccessNode;

};


#endif /* XMLMESSAGENETWORK_H_ */
