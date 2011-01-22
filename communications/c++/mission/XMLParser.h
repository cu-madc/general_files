/*
 * XMLParser.h
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

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <libxml/tree.h>
#include <libxml/parser.h>

#define SIZE_READ_FILE_BUFFER	131072
#define SIZE_READ_DTD_BUFFER    262144

class XMLParser {

public:

	XMLParser();
	~XMLParser();

	int readXMLFile(const char *fileName); // routine to read an xml file. (Used for debugging.)
	int readDTDFile(const char *fileName); // routine to read a dtd file.

	const char* getBuffer() { return fileBuffer;};         // returns the pointer to the buffer
	xmlDoc* getXMLDocument() { return doc; };              // returns the pointer to the document.
	void setXMLBuffer(const char *buf, int size);          // copies the buffer over into the local buffer
	virtual void copyXMLTree(xmlDocPtr existingDocument);  // makes a copy of the parsed xml document.

	// Routine to walk through the tree and find the node that contains a child with the given name and associated contents.
	xmlNode* walkObjectChildrenByNameContents(xmlNode *currentNode,const char *nodeName,const char *name,const char *contents);
	int checkChildrenForNameAndContents(xmlNode *currentNode,const char *name,const char *contentsToMatch);

	void xml2Char();                 // Returns the (char) xml data
	void parseXMLBuffer();           // Parses the xml file in the current buffer.
	void cleanUpDocument();          // Frees up the document if it exists.
	void cleanUpXML();               // cleans up the data associated with the parsed xml tree

	xmlNode* getRootNode() { return root_node;};


	enum InformationType { EMPTY, VACUUM_NETWORK, VACUUM_STATE, CONTROLER_NETWORK};
	void setMyInformationType(InformationType value){ myType = value;}
	InformationType getMyInformationType() {return myType;}


protected:
	// xml parser and tree variables
	xmlParserCtxtPtr ctxt;
	xmlDoc *doc;
	xmlNode *root_node;

	// My type
	InformationType myType;

	// Buffer variables
	char fileBuffer[SIZE_READ_FILE_BUFFER];
	char dtdBuffer[SIZE_READ_DTD_BUFFER];

private:

};

#endif /* XMLPARSER_H_ */
