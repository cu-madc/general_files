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
 */

#include <iostream>
#include "XMLParser.h"

XMLParser::XMLParser() {
	/**
	 * constructor for the XMLParser class. Initializes the xml variables.
	 * Initializes the character buffers.
	 *
	 **/

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

void XMLParser::cleanUpXML() {
	/**
	 * Clean up and free the data and variables associated with the parsed
	 * XML tree.
	 **/

	/*free the document */
	xmlFreeDoc(doc);

	/* free up the parser context */
	xmlFreeParserCtxt(ctxt);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();
	xmlMemoryDump();

}



