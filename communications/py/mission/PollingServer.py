#!/usr/bin/python
#
# PollingServer.py
#
#  Created on: Jan 27, 2011
#      Author: black
#
#      File that contains the methods for the PollingServer class.
#      This class is used to set up and manage a tcp server. Once
#      started the polling method waits for an incoming message
#      over the socket previously created.
#
#      This class also keeps track of the data that has come in.
#      When a message comes in it has to be parsed and then a
#      decision is made to determine what kind of data class should
#      be created.
#
# This material is based on research sponsored by DARPA under agreement
# number FA8750-10-2-0165. The U.S. Government is authorized to
# reproduce and distribute reprints for Governmental purposes
# notwithstanding any copyright notation thereon.
#
# The views and conclusions contained herein are those of the authors
# and should not be interpreted as necessarily representing the official
# policies or endorsements, either expressed or implied, of DARPA or the
# U.S. Government.
#
# ==========================================================================
#
# For use by entities other than the U.S. Government the following
# additional limitations apply:
#
# Copyright (c) 2011, Clarkson University
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above
#   copyright notice, this list of conditions and the following
#   disclaimer in the documentation and/or other materials provided
#   with the distribution.
#
# * Neither the name of the Clarkson University nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# (license copied from http:# www.opensource.org/licenses/bsd-license)
#
#
#

from Queue import Queue
from SocketServer import *
import socket
import threading

import re


#include "XMLParser.h"

## Class to handling incoming connections.



class PollingServer:

	# Static variables.
	POLLING_SERVER_DEFAULT_PORT=4554
	DEBUG = True
	POLLING_SERVER_BUFFER_SIZE = 4096
	BUFFER_SIZE = 4096;                    # Max size of the buffer




	def __init__(self,startThread=False,portNumber=POLLING_SERVER_DEFAULT_PORT) :
	    # *********************************************************************
	    # Constructor for the PollingServer class.
	    # ******************************************************************** */


		#pthread_mutex_t self.mutexUpdateData;          #  The mutex used in the polling thread.
		self.incomingTCP = Queue()
		self.lock = None

		#  Variables for the state of the polling process
		self.setRunning(startThread)


		#  Variables used to keep track of the data.
		self.incomingDataList = Queue();


		#  Initialize the port number to use
		self.setPort(portNumber);
		self.setHostName(socket.gethostname())


		#if (createAndInitializeSocket()) :
			#  Could not create and set up the socket for the polling loop.
		#	destroyMutex();
		#	return;


		#  Create the thread and return to the calling program.
		#setRunning(1); #  Assume it is okay to receive information.
		#if (startThread) :
		#	pthread_create(&pollingThread, None, PollingServer::pollingLoop,
		#			(void*) this);
		#else:
		#	pollingLoop();





	def __del__(self):
		# *********************************************************************
		# * class destructor.
		# ******************************************************************** */


		#  Send an empty message to send to the open socket and shut it down.
		setRunning(False);
                #XMLSendLocal *mySender = new XMLSendLocal(getPort());
                #mySender->sendNULLXMLTree();
                #delete mySender;


		#  The server needs to be shut down. Destroy the mutex and shut down the socket.
                #if (!shutdownSocket(0)) :
		#     print("Unable to shutdown the socket.")
		# else :
                #std::cout << "Turned off the socket server." << std::endl;


		if getRunning() :
			self.stopServerSocket()
			self.destroyMutex()
			print("Exiting the thread.")
			#pthread_exit(None);


		#  Delete all of the data objects in my list of incoming data.
                #for (std::list<XMLParser*>::iterator it = incomingDataList.begin(); it
		#       != incomingDataList.end(); ++it) {
		      # cout << (*it)->getMyOne() << endl;
		      #delete (*it);

                #incomingDataList.clear();


	   
	def setRunning(self,value) :
		#  Sets the value of the running variable.
		self.keepCheckingQueue = value


	def getRunning(self) :
		#  Returns the value of the running variable.
		return (self.keepCheckingQueue);


	def setPort(self,value) :
		# Sets the value of the port to use for the socket
		self.serverPort = value;


	def getPort() :
		# Returns the value of the port used for the socket
		return(self.serverPort);


	def setHostname(self,value) :
		# Sets the value of the port to use for the socket
		self.hostname = value;


	def getHostname() :
		# Returns the value of the port used for the socket
		return(self.hostname);







	def createAndInitializeSocket(self) :
	       # *********************************************************************
	       # Set up and create the socket used for waiting for information.
	       # If everything is okay returns zero. Otherwise it returns 1.
	       # ******************************************************************** **/

	       # Poll the queue periodically
	       self.setRunning(True)
	       threading.Timer(1.0, self.checkIncomingQueue).start()


	       # Start the server on a separate thread.
	       self.socketServer = IncomingSocketServer( \
		       (self.getHostName(),self.getPort()),LocalTCPHandler,self)
	       self.serverThread = threading.Thread(target=self.socketServer.serve_forever)
	       self.serverThread.setDaemon(True)
	       self.serverThread.start()
	       self.printMessage("Started thread: %s listening on %s:%s" % \
				 (self.serverThread.getName(),self.getHostName(),self.getPort()))





	def stopServerSocket(self,printError=False):
		####################################################################
                ## stopServerSocket(self,printError=False)
	        ##
	        ## Stop the existing socket server. 

		self.setRunning(False)
		
		try:
		    self.socketServer.shutdown()
		except:
		    if(printError) :
			print("Error - unable to shut down the socket server.")
		    return(False)

	        return(True)




	####################################################################
	## checkIncomingQueue(self)
	##
	## Routine to check the queue for any completed requests
	def checkIncomingQueue(self) :
		#self.printMessage("checking")

		numberItems = 0
		self.lock = self.serverThread.Lock()
		entry = None
		while(not self.incomingTCP.empty()):
		    # Something has been passed in from the interwebz
		    entry = self.incomingTCP.get()
		    print("Got this: {0}".format(entry))
		    numberItems += 1


		entry = None
		self.lock.release()
		self.lock = None

		if(self.getRunning()) :
		    threading.Timer(1.0, self.checkIncomingQueue).start()

		return(numberItems);




	def destroyMutex(self) :
		# *********************************************************************
		# Destory the mutex that has been created in the constructor.
		# ******************************************************************** */

		if(self.lock) :
			self.lock.release();
			self.lock = None



	def determineIncomingXMLTreeType(self,buffer,bufferLength) :
		# Determine what kind of xml tree was just sent
		# in. Return a pointer to the class that contains
		# it. The XMLParser class is a base class for all of
		# the XML container classes.


		thePointer = None;
		#XMLIncomingDIF *determineType = new XMLIncomingDIF;
		#determineType->setXMLBuffer(buffer,bufferLength);
		#determineType->parseXMLBuffer();

		#xmlNode *parentNode = determineType->walkObjectChildrenByNameContents(determineType->getRootNode(),
																		  #"objectClass","name","empty");
		if(parentNode) :
			print("EMPTY!")
			return(None);

		else :
	              #parentNode = determineType->walkObjectChildrenByNameContents(determineType->getRootNode(),
																	 # "objectClass","name","vacuumNetwork");
																	 pass
																	 
		if(parentNode):
			print("Vacuum!")
			#thePointer = (XMLParser*)new XMLMessageNetwork;
			#thePointer->copyXMLTree(determineType->getXMLDocument());

		# (?) determineType.__del__ or determineType = None
		determineType = None
		return(thePointer);





########################################################################
## Threaded Socket Server class
##
## This class keeps track of the incoming messages. It is run as a
## separate thread.
class IncomingSocketServer (ThreadingMixIn, TCPServer): 


    def __init__(self,connectionInfo,handler,parent) :
        self.myParent = parent
        #ThreadingMixIn.__init__(self)
        TCPServer.__init__(self,connectionInfo,handler)




########################################################################
## TCP Handler class
##
## This class handles the tcp requests coming through the threaded
## server class.
class LocalTCPHandler (BaseRequestHandler): 

    #def __init__(self):
    #    #self.incomingTCP = theQueue
    #    pass

    def handle(self) :
        socket = self.request
        theHost = self.server.myParent.getClientContactInfo(self.client_address[0],0)        
        self.printMessage("Heard from %s-%s" % (self.client_address[0],theHost))

	data = self.request.recv(PollingServer.POLLING_SERVER_BUFFER_SIZE).strip()
	print("Confirmed Client: {0}".format(data))
	self.request.send("OK")
	self.server.myParent.incomingTCP.put({'data':data})




if (__name__ =='__main__') :
	print("testing")