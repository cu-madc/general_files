/*
 * XMLParser.h
 *
 *  Created on: Jan 14, 2011
 *      Author: black
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <iostream>
#include <fstream>
#include <string.h>

#define SIZE_READ_FILE_BUFFER	131072
#define SIZE_READ_DTD_BUFFER    262144

class XMLParser {

public:

	XMLParser();
	~XMLParser();

	int readXMLFile(const char *fileName); // routine to read an xml file. (Used for debugging.)
	int readDTDFile(const char *fileName); // routine to read a dtd file.

	const char* getBuffer() { return fileBuffer;}; // returns the pointer to the buffer
	xmlDoc* getXMLDocument() { return doc; };// returns the pointer to the document.
	void setXMLBuffer(const char *buf, int size);  // copies the buffer over into the local buffer
	void copyXMLTree(xmlDocPtr existingDocument);  // makes a copy of the parsed xml document.

	void xml2Char();                 // Returns the (char) xml data
	void parseXMLBuffer();           // Parses the xml file in the current buffer.
	void cleanUpXML();               // cleans up the data associated with the parsed xml tree


protected:
	// xml parser and tree variables
	xmlParserCtxtPtr ctxt;
	xmlDoc *doc;
	xmlNode *root_node;

	// Buffer variables
	char fileBuffer[SIZE_READ_FILE_BUFFER];
	char dtdBuffer[SIZE_READ_DTD_BUFFER];

private:

};

#endif /* XMLPARSER_H_ */
