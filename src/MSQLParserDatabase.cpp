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
#include "MStdLib.h"
#include "MSQLParser.h"
#include "MODBCDatabase.h"
#include "MSQLParserDatabase.h"


///////////////////////////////////////////////////
// MSQLParserDatabase
///////////////////////////////////////////////////
bool MSQLParserDatabase::OnExec(const char *sql)
	{
	bool ret=mStatement.ExecDirect(sql);
	if(ret==false) {  return this->OnError(mStatement.GetError());  }
	return true;
	}


///////////////////////////////////////////////////
MSQLParserDatabase::MSQLParserDatabase(void)
	{
	}


///////////////////////////////////////////////////
MSQLParserDatabase::~MSQLParserDatabase(void)
	{  Destroy(); }


///////////////////////////////////////////////////
bool MSQLParserDatabase::Create(const char *odbcstring)
	{
	Destroy();

	if(MSQLParser::Create()==false)
		{
		Destroy();
		return false;
		}

	if(mEnvironment.Create()==false)
		{
		MStdPrintf("**Error: Unable to create ODBC Environment:%s\n",mEnvironment.GetError());
		Destroy();
		return false;
		}

	if(mConnection.Create(mEnvironment)==false)
		{
		MStdPrintf("**Error: Unable to create ODBC Connection:%s\n",mConnection.GetError());
		Destroy();
		return false;
		}

	if(mConnection.Open(odbcstring)==false)
		{
		MStdPrintf("**Error: Unable to create ODBC Connection:%s\n",mConnection.GetError());
		Destroy();
		return false;
		}

	if(mStatement.Create(mConnection)==false)
		{
		MStdPrintf("Error: Unable to create statement object(%s)",mStatement.GetError());
		return false;
		}
	
	return true;
	}


///////////////////////////////////////////////////
bool MSQLParserDatabase::Destroy(void)
	{
	mStatement.Destroy();
	mConnection.Destroy();
	mEnvironment.Destroy();
	MSQLParser::Destroy();
	return true;
	}



