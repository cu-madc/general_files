/*
 * XMLMessageNetwork.c++
 *
 *  Created on: Jan 14, 2011
 *      Author: black
 *
 *      Methods for the class that keeps track of the information specific
 *      to the network associated with a given vacumm.
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
#include "XMLMessageNetwork.h"

XMLMessageNetwork::XMLMessageNetwork() :
	networkID(0), probSuccessfulTransmission(1.0) {

	XMLIncomingDIF();
	dimensionsNode = NULL;
	objectClassNode = NULL;
	networkIDNode = NULL;
	probSuccessNode = NULL;

}

XMLMessageNetwork::~XMLMessageNetwork() {

}

void XMLMessageNetwork::createRootNode() {
	/**
	 *  Method to create the root node in the xml tree. Sets the scheme information as well.
	 **/

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "objectModel");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "xmlns", BAD_CAST "http://standards.ieee.org/IEEE1516-2010");
	xmlNewProp(root_node, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
	xmlNewProp(root_node, BAD_CAST "xsi:schemaLocation", BAD_CAST "http://standards.ieee.org/IEEE1516-2010 http://standards.ieee.org/downloads/1516/1516.2-2010/IEEE1516-DIF-2010.xsd");

}

void XMLMessageNetwork::createDimensions() {

	/**
	 * Creates the dimensions node in the xml tree. It adds the objectClass
	 * node as a child of the dimensions node. Finally a "name" node is added
	 * as a child of the dimensions node.
	 **/

	/*
	 * xmlNewChild() creates a new node, which is "attached" as child node
	 * of root_node node.
	 */

	xmlNodePtr node;
	node = xmlNewChild(root_node, NULL, BAD_CAST "objects",NULL);
	objectClassNode = xmlNewChild(node,NULL,BAD_CAST "objectClass",NULL);
	xmlNewChild(objectClassNode,NULL,BAD_CAST "name",BAD_CAST "vacuumNetwork");
	dimensionsNode = xmlNewChild(objectClassNode, NULL, BAD_CAST "dimensions",NULL);

}

void XMLMessageNetwork::setNetworkIDNode() {
	/**
	 * Method to set the value of the id for this vacuum. It indicates
	 * which vacumm this structure is associated with. The value is then
	 * added to the xml tree under the dimensions node.
	 **/

	char numberRef[8];
	networkIDNode = xmlNewChild(dimensionsNode, NULL, BAD_CAST "dimension",NULL);
	sprintf(numberRef, "%d", networkID);
	xmlNewChild(networkIDNode, NULL, BAD_CAST "name",BAD_CAST "networkID");
	xmlNewChild(networkIDNode, NULL, BAD_CAST "value",BAD_CAST numberRef);

}

// xmlNodeSetContent to set a node's content.

void XMLMessageNetwork::setProbSuccessNode() {
	/**
	 * Method to set the value of the probability that a message from the
	 * vacuum is successfully received. The value is added to the xml tree
	 * under the dimensions node.
	 **/

	char numberRef[21];
	probSuccessNode = xmlNewChild(dimensionsNode, NULL, BAD_CAST "dimension",NULL);
	sprintf(numberRef, "%22.14E", probSuccessfulTransmission);
	xmlNewChild(probSuccessNode, NULL, BAD_CAST "name",BAD_CAST "probabilitySuccessfulTransmission");
	xmlNewChild(probSuccessNode, NULL, BAD_CAST "value",BAD_CAST numberRef);

}


void XMLMessageNetwork::copyXMLTree(xmlDocPtr existingDocument){
	/**
	 * Copy the given parsed XML tree into the local tree. Also set the
	 * relevant nodes that this class tracks from the tree.
	 **/

	XMLParser::copyXMLTree(existingDocument);

	if(root_node) {

	}

}

/*Get the root element node */
//root_element = xmlDocGetRootElement(doc);
//print_element_names(root_element);

