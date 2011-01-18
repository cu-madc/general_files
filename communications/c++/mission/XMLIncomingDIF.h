/*
 * XMLIncomingDIF.h
 *
 *  Created on: Jan 15, 2011
 *      Author: black
 *
 *   Class associated with dealing wit the incoming DIF file.
 *   The data that just came in from the internet has to be tested
 *   to determine what aspect of the system it needs to specify.
 *   This class is used to do that testing.

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


 */

#ifndef XMLINCOMINGDIF_H_
#define XMLINCOMINGDIF_H_

#include "XMLParser.h"

class XMLIncomingDIF : public XMLParser {

public:
	XMLIncomingDIF();
	~XMLIncomingDIF();

	// Routine to copy the class name into the given buffer.
	void getObjectClassName(char *name,int size);

	// Routine to walk through and determine which node has the dimensions
	// part of the tree.
	xmlNode* walkObjectName(xmlNode *currentNode,const char *name);

protected:

private:

};


#endif /* XMLINCOMINGDIF_H_ */
