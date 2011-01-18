/*
 * XMLIncomingDIF.h
 *
 *  Created on: Jan 15, 2011
 *      Author: black
 *
 *   Class associated with dealing wit the incoming DIF file.
 *   The data that just came in from the internet has to be tested
 *   to determine what aspect of the system it needs to specify.
 *   This class is used to do that testing.
 */

#ifndef XMLINCOMINGDIF_H_
#define XMLINCOMINGDIF_H_

#include "XMLParser.h"

class XMLIncomingDIF : public XMLParser {

public:
	XMLIncomingDIF();
	~XMLIncomingDIF();

	// Routine to copy the class name into the given buffer.
	void getObjectClassName(char *name,int size);

	// Routine to walk through and determine which node has the dimensions
	// part of the tree.
	xmlNode* walkObjectName(xmlNode *currentNode,const char *name);

protected:

private:

};


#endif /* XMLINCOMINGDIF_H_ */
