/*
 * XMLIncomingDIF.c++
 *
 *  Created on: Jan 15, 2011
 *      Author: black
 *
 *      Methods for the XMLIncomingDIF class. Used to decide what
 *      kind of object is associated with the information passed
 *      in through the socket connection.
 *
 */

#include "XMLIncomingDIF.h"

XMLIncomingDIF::XMLIncomingDIF(){
	// Call the constructor from the base class.
	XMLParser();
}

XMLIncomingDIF::~XMLIncomingDIF() {

}

void XMLIncomingDIF::getObjectClassName(char *name,int size){
	/**
	 * Routine to determine the name associated with objectClass
	 * part of the xml file.
	 **/


	xmlChar *content;
	int len;
	memset(name,0,size);  // initialize the name buffer.

	// Walk through the tree and find the object class node.
	xmlNode *objectClass = walkObjectName(root_node,"objectClass");

	if(objectClass) {
		// The object class node was found.
		xmlNode *sibling;
		for(sibling = objectClass->children;sibling;sibling=sibling->next){
			// Go through each of the sibling and look for a name node.
			//std::cout << "objectClass node name: " << sibling->name << std::endl;
			if(strcmp((char *)sibling->name,"name")==0) {
				// Found the name node. Report the value for this node.
				content = xmlNodeGetContent(sibling);
				len= strlen((char*)content);
				memcpy(name,(char *)content,size<len?size:len);
			}
		}
	}

	else {
		// Zero out the buffer that gets returned.
		//std::cout << "object class not found." << std::endl;
		memset(name,0,size);
	}

}


xmlNode* XMLIncomingDIF::walkObjectName(xmlNode *currentNode,const char *name){
	/**
	 * Walk through the current parsed tree and look for the object node.
	 **/

	xmlNode *sibling;       // siblings of current node
	xmlNode *checkChildren; // result from checking children of each sibling

	if(currentNode==NULL) {
		return(NULL); // The node passed in was null.
	}

	for(sibling=currentNode;sibling;sibling=sibling->next){
		// Go through each of the children of the passed node.

		if(sibling->type == XML_ELEMENT_NODE) {
			// This node is an element.
			if(strcmp((char *)sibling->name,name)==0){
				// The name of the node matches the name that was passed.
				// Return this node.
				return(sibling);
			}
		}

		// Check to see if the target is any of this object's children.
		checkChildren = walkObjectName(sibling->children,name);
		if(checkChildren) {
			// A match was found. Return it.
			return(checkChildren);
		}

	}

	// No match was found. Return null.
	return(NULL);

}



