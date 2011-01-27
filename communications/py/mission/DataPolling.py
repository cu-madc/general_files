#!/usr/bin/python
#
#  DataPolling.py
# 
#   Created on: Jan 27, 2011
#       Author: black
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
# notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following
# disclaimer in the documentation and/or other materials provided
# with the distribution.
#
# * Neither the name of the Clarkson University nor the names of its
# contributors may be used to endorse or promote products derived
# from this software without specific prior written permission.
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
# (license copied from http://www.opensource.org/licenses/bsd-license)
#
#
# 

#include "mex.h"
#include <iostream>
#include "string.h"
#include "PollingServer.h"
#include "XMLMessageNetwork.h"
from PollingServer import PollingServer

theServer = PollingServer()

def DataPolling(nlhs,plh,nrhs,prhs) :

    input_specifier = ""

	 # **********************************************************************************
	 # This is very preliminary. At this point the routine expects just one argument
	 # when called and returns none. The one argument is a string and should either be
	 # "start" or "end." If it is "start" then it creates an object and starts the socket
	 # and polling loop. If it is "end" then it sets a variable to let the polling loop
	 # exit the next time it goes through.
	 # ********************************************************************************** */

    if ((nrhs == 1) and (type(prhs) is "str")) :
        input_specifier = prhs


        if (input_specifier == "start") :
            theServer = PollingServer()
            if(theServer.getRunning()==0) :
                # There was an error trying to start the polling server.
                theServer = None

            elif ((theServer != None) and (input_specifier == "end")) :
                    theServer.setRunning(-1);

            else :
                    print("Unrecognized command.");


        else:
            print("Unrecognized command");




