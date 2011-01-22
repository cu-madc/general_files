/*
 * XMLParser.c++
 *
 *  Created on: Jan 14, 2011
 *      Author: black
 *
 *  Base class for the data class. This class is inherited by
 *  most of the data classes. It is used to manipulate the char
 *  buffer that has the xml data and convert it into a parsed
 *  xml tree. It also has the associated routines to clean up
 *  the xml data structures.
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
 */

#include <iostream>
#include <fstream>
#include <string.h>

#include "XMLParser.h"

XMLParser::XMLParser() {
	/**
	 * constructor for the XMLParser class. Initializes the xml variables.
	 * Initializes the character buffers.
	 *
	 **/

	// initialize my type
	myType = EMPTY;

	// initialize the xml parameters
	doc = NULL;
	root_node = NULL;

	/* create a parser context */
	ctxt = xmlNewParserCtxt();

	// zero out the buffers
	memset(fileBuffer, 0, SIZE_READ_FILE_BUFFER);
	memset(dtdBuffer, 0, SIZE_READ_DTD_BUFFER);

}

XMLParser::~XMLParser() {
	/**
	 * Destructor for the XMLParser class. Release and delete the parsed
	 * XML data structures.
	 *
	 **/


	// Delete/release the xml data structures.
	cleanUpXML();
}



void XMLParser::parseXMLBuffer() {
	/**
	 * Parse the contents of the buffer and put them into an XML tree.
	 **/

	/*parse the file and get the DOM */
	//doc = xmlReadFile("HLAstandardMIM.xml","IEEE1516-DIF-2010.xsd",XML_PARSE_DTDVALID);
	doc = xmlCtxtReadDoc(ctxt, (xmlChar *) fileBuffer, NULL, dtdBuffer, 0
			& XML_PARSE_DTDVALID);

	/*Get the root element node */
	root_node = xmlDocGetRootElement(doc);


}

int XMLParser::readXMLFile(const char *fileName) {
	/**
	 * Read an XML file and put it into the local buffer.
	 * This routine is mostly in place for testing and debugging 4
	 * the xml codes. Returns the number of characters read from the
	 * file.
	 **/

	std::ifstream inputFile;
	char *charPtr = fileBuffer;
	int size = 0;

	inputFile.open(fileName, std::ios::in);           // Open the file.
	memset(fileBuffer, 0, SIZE_READ_FILE_BUFFER);     // Initialize the buffer.
	while ((++size < SIZE_READ_FILE_BUFFER-1) && !inputFile.eof()) {
		// Get the next character.
		*charPtr++ = inputFile.get();
	}

	*(--charPtr) = 0;    // Make sure that the last character is null
	inputFile.close();   // close the file.
	return (size);       // return the number of characters read.

}

int XMLParser::readDTDFile(const char *fileName) {
	memset(dtdBuffer, 0, SIZE_READ_DTD_BUFFER);     // Initialize the buffer.
	int size = readXMLFile(fileName);
	if (size > 0) {
		memcpy(dtdBuffer, fileBuffer, size);
	}
}

void XMLParser::setXMLBuffer(const char *buf, int size) {
	/**
	 * Copy the given buffer into the local buffer. This needs to be
	 * done before an xml file is parsed into a tree.
	 **/
	if (size < SIZE_READ_FILE_BUFFER) {
		memcpy(fileBuffer, buf, size);
	}

}

void XMLParser::copyXMLTree(xmlDocPtr existingDocument){
	/**
	 * Copy the given parsed XML tree into the local tree.
	 **/

	if(doc) {
		xmlFreeDoc(doc);
	}
	doc = xmlCopyDoc(existingDocument,1);

	/*Get the root element node */
	root_node = xmlDocGetRootElement(doc);
	//std::cout << " root node pointer: " << (void *)root_node << std::endl;


}

xmlNode* XMLParser::walkObjectChildrenByNameContents(xmlNode *currentNode,
													const char *nodeName,
													const char *name,
													const char *contents){
	/**
	 * Routine to walk through the tree and find the node that contains a child with the given name
	 * and associated contents.
	 **/

	xmlNode *sibling;       // siblings of current node
	xmlNode *checkChildren; // result from checking children of each sibling

	//std::cout << std::endl << "Searching for node name: "
	//			<< name << " with contents "
	//			<< contents
	//			<< " node pointer: " << (void *)currentNode
	//			<< std::endl;

	if(currentNode==NULL) {
		return(NULL); // The node passed in was null.
	}

	for(sibling=currentNode;sibling;sibling=sibling->next){
		// Go through each of the children of the passed node.

		if(sibling->type == XML_ELEMENT_NODE) {
			// This node is an element.
			//std::cout << "   now checking node name: " << sibling->name << std::endl;
			if (strcmp((char *) sibling->name, nodeName) == 0) {
				if (checkChildrenForNameAndContents(sibling->children, name,contents)) {
					return (sibling);
				}
			}
		}

		// Check to see if the target is any of this node's children.
		checkChildren = walkObjectChildrenByNameContents(sibling->children,nodeName,name,contents);
		if(checkChildren) {
			// A match was found. Return it.
			return(checkChildren);
		}

	}

	// No match was found. Return null.
	return(NULL);

}

int XMLParser::checkChildrenForNameAndContents(xmlNode *currentNode,
												const char *name,
												const char *contentsToMatch){
	/*
	 * Routine to walk through each of the children of the current node. If it has a
	 * node with the given name and whose contents match the given value then the
	 * result is "true." Otherwise return "false."
	 */

	xmlNode *sibling;       // siblings of current node
	xmlChar *content;       // content of a node

	for (sibling = currentNode; sibling; sibling = sibling->next) {
		content = xmlNodeGetContent(sibling);
		if (sibling->type == XML_ELEMENT_NODE) {
			//std::cout << "      checking node name:" << sibling->name
			//		<< "#" << name << "# "
			//		<< "* contents:" << (char *)content << "*"
			//		<< contentsToMatch << "*"
			//			<< std::endl;
			if ((strcmp((char *) sibling->name, name) == 0) &&
				(strcmp((char *) content, contentsToMatch) == 0)) {
				// The name of the node matches the name that was passed.
				// Return this node.
				//std::cout << "   THIS IS A MATCH!" << std::endl;
				return (1);
			}
		}
	}

	return(0);

}


void XMLParser::xml2Char() {
	/**
	 * Convert the parsed XML file in the local root and then
	 * put the char file into the local buffer.
	 **/


	xmlChar *xmlbuff;
	int buffersize;

	memset(fileBuffer, 0, SIZE_READ_FILE_BUFFER);          // initialize the local buffer

	// Dump the parsed tree into a char value.
	xmlDocDumpFormatMemoryEnc(doc, &xmlbuff, &buffersize,"UTF-8",1);
	if(buffersize<SIZE_READ_FILE_BUFFER){
		// The buffer has room. Copy the xml file over into the local buffer.
		memcpy(fileBuffer,(char*)xmlbuff,buffersize);
	}
	xmlFree(xmlbuff);

}

void XMLParser::cleanUpDocument() {
	/*free the document */
	if(doc) {
		xmlFreeDoc(doc);
	}

}

void XMLParser::cleanUpXML() {
	/**
	 * Clean up and free the data and variables associated with the parsed
	 * XML tree.
	 **/

	// Clean up the document
	cleanUpDocument();

	/* free up the parser context */
	xmlFreeParserCtxt(ctxt);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();
	xmlMemoryDump();

}







