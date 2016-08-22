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


//v3.1 copyright Comine.com 20160822M0122
#ifndef MSQLExec_h
#define MSQLExec_h

///////////////////////////////////////////////
#include "MStdLib.h"
#include "MSQLParserDatabase.h"

//  Flag parameters (stopflag,pauseflag)
//   (true,false) - stop on first sql error
//   (false,true) - pause on every sql statement
//   (true,true)  - pause on first sql error
//   (false,false)- execute past all sql errors


//////////////////////////////////////////////
class MSQLExec:MSQLParserDatabase
	{
	bool mStopFlag,mPauseFlag,mQuiteFlag;
	int mSQLCount,mBadCount,mGoodCount;
	/////////////////////////////////////////////
	virtual bool OnExec(const char *sql);
	virtual bool OnError(const char *error);
	bool AskStop(void);

	/////////////////////////////////////////////
	public:
	MSQLExec(void);
	~MSQLExec(void);
	bool ProcessSQLFile(const char *odbcdsn,const char *file
		,bool bstop=true,bool bpause=false,bool bquite=false);
	};

#endif // MSQLExec_h
	