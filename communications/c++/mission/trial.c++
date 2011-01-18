/*
 * trial.c++
 *
 *  Created on: Jan 14, 2011
 *      Author: black
 */

#include <iostream>
#include "XMLMessageNetwork.h"
#include "XMLIncomingDIF.h"

 int main(int argc,char **argv) {
	 char name[100];
	 name[0] = 0;

	XMLMessageNetwork *trial = new XMLMessageNetwork;
	XMLIncomingDIF    *message = new XMLIncomingDIF;

	/*
	trial->setNetworkID(2);
	trial->setProbSuccessfulTransmission(0.5);
	trial->createRootNode();
	trial->createDimensions();
	trial->setNetworkIDNode();
	trial->setProbSuccessNode();
	trial->xml2Char();
	*/

	message->readXMLFile("networkSample.xml");
	message->parseXMLBuffer();
	message->getObjectClassName(name,100);
	std::cout << "name: " << name << std::endl << std::endl;

	trial->copyXMLTree(message->getXMLDocument());
	trial->xml2Char();
	std::cout << message->getBuffer() << std::endl << std::endl;

	delete message;
	return(0);

 }


