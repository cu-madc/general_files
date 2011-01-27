#!/usr/bin/python
#
#  Sender.py
# 
#   Created on: Jan 27, 2011
#       Author: black
# 
# 
#       Class to act as a client using a tcp connection. This
#       will take a machine name, a port number, and data and
#       send it to the destination using tcp.
# 
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


import socket

class Sender :

	BufferSize = 4554
	DefaultPortNumber = 4554
        DEBUG = True


        def __init__(self,port=DefaultPortNumber) :
            if(self.DEBUG) :
                print("Creating sender object: {0}".format(port))
            self.setPortNumber(port)


        def setHostname(self,name) :
            hostname = name

        def getHostname(self) :
            return(hostname)

        def setPortNumber(self,value) :
            self.portNumber = value

        def getPortNumber(self) :
            return(self.portNumber)
        

        ####################################################################
        ## createClientSocketAndConnect(self,theHost)
        ##
        ## Create a socket used to connect to another machine.
        def createClientSocketAndConnect(self,theHost) :

            # This host is in the cleared list.
            if(self.DEBUG) :
                print("Creating connection for {0}".format(theHost))
                    
            self.clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            socketInfo = socket.gethostbyaddr(theHost)
            
            try:
                # Try to make the connection.
                if(self.DEBUG) :
                    print("Trying to connect to {0}:{1}".format(socketInfo[0],socketInfo[1]))
                self.clientSocket.connect(socketInfo)

            except:
                # We were unable to make the connection.
                if(self.DEBUG) :
                    print("Connection Error - Error creating connection to the server.")
                    return(False)
                
            return(True)



        ####################################################################
        ## sendVector(self,header,theVec,theHost)
        ##
        ## Routine to send a set of real numbers with the included header
        ## through the current client socket.
        def sendVector(self,header,theVec,theHost) :
            if(self.createClientSocketAndConnect(theHost)) :
                # The socket has been created. Send the message along.
                try: 
                    self.clientSocket.send(header+"\n"+
                                           struct.pack("d"*len(theVec),*[v for v in theVec]))
                    return(self.clientSocket.recv(100))
                except:
                    # Should this generate an error dialog?
                    if(self.DEBUG) :
                        print("Error sending vector over client connection.")


            return(None)


        ####################################################################
        ## sendString(self,theString,theHost)
        ##
        ## Routine to send a set of real numbers with the included header
        ## through the current client socket.
        def sendString(self,theString,theHost) :
            if(self.createClientSocketAndConnect(theHost)) :
                # The socket has been created. Send along the message.
                try:
                    self.clientSocket.send(theString)
                    return(self.clientSocket.recv(100))
                except:
                    # Should this generate an error dialog?
                    if(self.DEBUG) :
                        print("Error sending string over client connection.")

            return(None)



        ####################################################################
        ## closeClientSocket(self)
        ##
        ## Routine to close down the client socket.
        def closeClientSocket(self) :
            self.clientSocket.close()



if (__name__ =='__main__') :
	import time
	
	print("testing sender")
        mySender = Sender();
        mySender.sendString("This is a test",socket.gethostname())
        
