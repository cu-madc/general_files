/*
 * XMLMessageNetwork.c++
 *
 *  Created on: Jan 14, 2011
 *      Author: black
 *
 *      Methods for the class that keeps track of the information specific
 *      to the network associated with a given vacumm.
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
	node = xmlNewChild(node,NULL,BAD_CAST "objectClass",NULL);
	xmlNewChild(node,NULL,BAD_CAST "name",BAD_CAST "vacummNetwork");
	dimensionsNode = xmlNewChild(node, NULL, BAD_CAST "dimensions",NULL);

}

void XMLMessageNetwork::setNetworkIDNode() {
	/**
	 * Method to set the value of the id for this vacuum. It indicates
	 * which vacumm this structure is associated with. The value is then
	 * added to the xml tree under the dimensions node.
	 **/
	xmlNodePtr node;
	char numberRef[8];
	node = xmlNewChild(dimensionsNode, NULL, BAD_CAST "dimension",NULL);
	sprintf(numberRef, "%d", networkID);
	xmlNewChild(node, NULL, BAD_CAST "name",BAD_CAST "networkID");
	xmlNewChild(node, NULL, BAD_CAST "value",BAD_CAST numberRef);

}

// xmlNodeSetContent to set a node's content.

void XMLMessageNetwork::setProbSuccessNode() {
	/**
	 * Method to set the value of the probability that a message from the
	 * vacuum is successfully received. The value is added to the xml tree
	 * under the dimensions node.
	 **/

	xmlNodePtr node;
	char numberRef[21];
	node = xmlNewChild(dimensionsNode, NULL, BAD_CAST "dimension",NULL);
	sprintf(numberRef, "%20.14f", probSuccessfulTransmission);
	xmlNewChild(node, NULL, BAD_CAST "name",BAD_CAST "probabilitySuccessfulTransmission");
	xmlNewChild(node, NULL, BAD_CAST "value",BAD_CAST numberRef);

}

/*Get the root element node */
//root_element = xmlDocGetRootElement(doc);
//print_element_names(root_element);

