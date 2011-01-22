/*
 * XMLIncomingDIF.c++
 *
 *  Created on: Jan 15, 2011
 *      Author: black
 *
 *      Methods for the XMLIncomingDIF class. Used to decide what
 *      kind of object is associated with the information passed
 *      in through the socket connection.

 This material is based on research sponsored by DARPA under agreement
 number FA8750-10-2-0165. The U.S. Government is authorized to
 reproduce and distribute reprints for Governmental purposes
 notwithstanding any copyright notation thereon.

 The views and conclusions contained herein are those of the authors
 and should not be interpreted as necessarily representing the official
 policies or endorsements, either expressed or implied, of DARPA or the
 U.S. Government.

 ==========================================================================

 For use by entities other than the U.S. Government the following
 additional limitations apply:

 Copyright (c) 2011, Clarkson University
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above
 copyright notice, this list of conditions and the following
 disclaimer in the documentation and/or other materials provided
 with the distribution.

 * Neither the name of the Clarkson University nor the names of its
 contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 (license copied from http://www.opensource.org/licenses/bsd-license)


 *
 */

#include <iostream>
#include <fstream>
#include <string.h>
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



