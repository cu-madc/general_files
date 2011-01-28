#!/usr/bin/python
#
#  XMLSendLocal.cpp
# 
#   Created on: Jan 28, 2011
#       Author: black
# 
# 		Used to send messages to this machine. It assumes that the machine has an
# 		active polling process running. It is helpful to send information to the
# 		thread that has been spawned and cannot be easily contacted via other means.
# 
#  This material is based on research sponsored by DARPA under agreement
#  number FA8750-10-2-0165. The U.S. Government is authorized to
#  reproduce and distribute reprints for Governmental purposes
#  notwithstanding any copyright notation thereon.
# 
#  The views and conclusions contained herein are those of the authors
#  and should not be interpreted as necessarily representing the official
#  policies or endorsements, either expressed or implied, of DARPA or the
#  U.S. Government.
# 
#  ==========================================================================
# 
#  For use by entities other than the U.S. Government the following
#  additional limitations apply:
# 
#  Copyright (c) 2011, Clarkson University
#  All rights reserved.
# 
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are
#  met:
# 
#  * Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
# 
#  * Redistributions in binary form must reproduce the above
#  copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided
#  with the distribution.
# 
#  * Neither the name of the Clarkson University nor the names of its
#  contributors may be used to endorse or promote products derived
#  from this software without specific prior written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
#  (license copied from http://www.opensource.org/licenses/bsd-license)
# 
# 
# 

from XMLParser import XMLParser
from Sender import Sender

class XMLSendLocal (XMLParser,Sender) :


    def __init__(self) :
        XMLParser(self)

        self.objectClassNode = None

	#Call the parts of the sender class to create the socket for communications.
	#Sender::Sender();
	setPortNumber(portNumber)
	setHostName("localhost")
	createSocket()


    def __close__(self) :
        pass


    def createRootNode(self) :
	# Routine to create a null tree that is sent to the local socket server.
	cleanUpDocument();

	#Create the document and a root node.
	#doc = xmlNewDoc(BAD_CAST "1.0");
	#root_node = xmlNewNode(NULL, BAD_CAST "objectModel");
	#xmlDocSetRootElement(doc, root_node);
	#xmlNewProp(root_node, BAD_CAST "xmlns", BAD_CAST "http://standards.ieee.org/IEEE1516-2010");
	#xmlNewProp(root_node, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
	#xmlNewProp(root_node, BAD_CAST "xsi:schemaLocation", BAD_CAST "http://standards.ieee.org/IEEE1516-2010 http://standards.ieee.org/downloads/1516/1516.2-2010/IEEE1516-DIF-2010.xsd");





        def createObjectClass(self) :

            ##
            # Creates the dimensions node in the xml tree. It adds the objectClass
            # node as a child of the dimensions node. Finally a "name" node is added
            # as a child of the dimensions node.
            ##

            ##
            # xmlNewChild() creates a new node, which is "attached" as child node
            # of root_node node.
            ##

            #xmlNodePtr node;
            #node = xmlNewChild(root_node, NULL, BAD_CAST "objects",NULL);
            #objectClassNode = xmlNewChild(node,NULL,BAD_CAST "objectClass",NULL);
            pass



        def addObjectClassName(self,name) :
            # Adds a child node to the objectClassNode. This assumes that the
            # createObjectClass method has already been called.
            if(self.objectClassNode) :
		self.xmlNewChild(self.objectClassNode,NULL,"name","name");





        def createNULLXMLTree(self) :
            self.createRootNode(self)
            self.createObjectClass(self)
            self.addObjectClassName(self,"empty")


        def sendNULLXMLTree(self) :
            self.createNULLXMLTree()
            self.xml2Char()
            theXML = self.getBuffer()
            self.sendBuffer(theXML);



        def copyInformation(self,vec) :
            # empty routine to allow this class to be able to be realized
            return


