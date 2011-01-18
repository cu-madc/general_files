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
