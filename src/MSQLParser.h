/*
Copyright (C) 2011-2014, Comine.com ( comine.com@gmail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


//v1.5 copyright Comine.com 20160822M0121
#ifndef MSQLParser_h
#define MSQLParser_h

/////////////////////////////////////////////////////
#include "MStdLib.h"
#include "MDynamicString.h"

/////////////////////////////////////////////////////
class MSQLParser
	{
	enum STATE { SSTART,S1,S2,S3,S4,S5,S6,SERR };

	MDynamicString mString;		// No MAx String Availible
	enum STATE mState;

	////////////////////////////////////////////////
	bool IsBlank(const char *buffer);
	char * Trim(char Buf[]); // Trim Both sides of a string
	protected:

	/////////////////////////////////////////////////
	virtual bool OnExec(const char *sql);
	virtual bool OnError(const char *error);

	////////////////////////////////////////////////
	public:
	MSQLParser(void);
	MSQLParser(int approxsize);
	~MSQLParser(void);
	bool Create(int approxsize=5000);  // Create a 5K work space
	bool Destroy(void);
	void Clear(void);  // Clear Buffer + place into start state
	bool DoChar(char ch);  // Send a character
	void DoEnd(void); // Tell Parser that there is no more input.
	};


#endif MSQLParser_h

