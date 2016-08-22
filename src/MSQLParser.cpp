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
#include "MStdLib.h"
#include "MBuffer.h"
#include "MDynamicString.h"
#include "MSQLParser.h"


//////////////////////////////////////////////////
// MSQLParser
//////////////////////////////////////////////////
bool MSQLParser::IsBlank(const char *buffer)
	{
	for(int i=0;buffer[i]!=0;++i)
		{
		if(MStdIsSpace(buffer[i])==false)	{ return false; }
		}

	return true;
	}


//////////////////////////////////////////////////
char * MSQLParser::Trim(char *buffer) // Trim Both sides of a string
	{
	MStdStrTrim(buffer);
	return buffer;
	}

//////////////////////////////////////////////////
//////////// Call Backs //////////////////////////
//////////////////////////////////////////////////
bool MSQLParser::OnExec(const char *sql) { return true; }
bool MSQLParser::OnError(const char *error) { return true; }

//////////////////////////////////////////////////
MSQLParser::MSQLParser(void)
	{  mState=SSTART;  }


//////////////////////////////////////////////////
MSQLParser::MSQLParser(int approxsize)
	{  mState=SSTART;   Create(approxsize);  }


//////////////////////////////////////////////////
MSQLParser::~MSQLParser(void)
	{  }


//////////////////////////////////////////////////
bool MSQLParser::Create(int approxsize)
	{
	Destroy();
	if(mString.Create(approxsize)==false)
		{
		return false;
		}

	mState=SSTART;  return true;
	}

///////////////////////////////////////////////////
bool MSQLParser::Destroy(void)
	{
	mState=SSTART;  mString.Destroy();
	return true;
	}


/////////////////////////////////////////////////////////////
void MSQLParser::Clear(void)
	{  mState=SSTART;  mString.Clear();  }


/////////////////////////////////////////////////////////////
void MSQLParser::DoEnd(void)
	{
	if(mString.IsBlank()==false)
		{  this->OnError("Last SQL Statement does not have terminating semicolon;");  }
	Clear();
	}


////////////////////////////////////////////////////////////
bool MSQLParser::DoChar(char ch)
	{
	if(ch==0) { DoEnd();  return true; }
	if(mState==SSTART)
		{
		if(ch=='/') { mState=S1; }
		else if(ch=='\"') { mString.Insert(ch); mState=S5; }
		else if(ch=='\'') { mString.Insert(ch); mState=S6; }
		else if(ch==';')
			{	// Execute Statement
			mString.TrimLeft();  mString.TrimRight();
			if(this->OnExec(mString.Get())==false)
				{ mString.Clear(); return false; }
			mString.Clear();  return true;
			}
		else { mString.Insert(ch); }
		return true;
		}
	else if(mState==S1)
		{
		if(ch=='/') { mState=S2; }
		else if(ch=='*') {  mState=S3; }
		else { mString.Insert('/');  mString.Insert(ch); mState=SSTART; }
		return true;
		}
	else if(mState==S2)
		{
		if(ch=='\r' || ch=='\n') { mState=SSTART; }
		return true;
		}
	else if(mState==S3)
		{
		if(ch=='*') { mState=S4; }
		return true;
		}
	else if(mState==S4)
		{
		if(ch=='*') { /*same state*/ }
		else if(ch=='/') { mState=SSTART; }
		else { mState=S3; }
		return true;
		}
	else if(mState==S5)
		{
		if(ch=='\"') { mString.Insert(ch); mState=SSTART; }
		else if(ch=='\r' || ch=='\n') { mState=SERR; }
		else { mString.Insert(ch); }
		return true;
		}
	else if(mState==S6)
		{
		if(ch=='\'') { mString.Insert(ch); mState=SSTART; }
		else if(ch=='\r' || ch=='\n') { mState=SERR; }
		else { mString.Insert(ch); }
		return true;
		}
	else if(mState==SERR)
		{
		if(ch==';') 
			{
			MBuffer errorinfo(mString.GetLength()+100);
			MStdSPrintf(errorinfo,errorinfo.GetSize()-1,"Unenclosed Quotes(%s)",mString.Get());
			if(this->OnError(errorinfo)==false) { return false; }
			}
		else { mString.Insert(ch); }
		return true;
		}

	return true; // Dead Code
	}


