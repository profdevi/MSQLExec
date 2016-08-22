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


//v1.6 copyright Comine.com 20160822M0117
#ifndef MSQLParserDatabase_h
#define MSQLParserDatabase_h

///////////////////////////////////////////////////////////////////
#include "MStdLib.h"
#include "MODBCDatabase.h"
#include "MSQLParser.h"

///////////////////////////////////////////////////////////////////
class MSQLParserDatabase:public MSQLParser
	{
	///////////////////////////////////////////////////////////////
	MODBCEnvironment mEnvironment;
	MODBCConnection mConnection;
	MODBCStatement mStatement;

	////////////////////////////////////////////////////////////////
	protected:
	bool OnExec(const char *sql);

	////////////////////////////////////////////////////////////////
	public:
	MSQLParserDatabase(void);
	~MSQLParserDatabase(void);
	bool Create(const char *odbcconnectstring);
	bool Destroy(void);
	};

#endif // MSQLParserDatabase_h


