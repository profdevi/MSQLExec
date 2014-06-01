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


//v1.2 copyright Comine.com 20140531S1044
#include "MStdLib.h"
#include "MFile.h"
#include "MSQLExec.h"


//************************************************************************
//  MSQLExec Definition
//************************************************************************
MSQLExec::MSQLExec(void)
	{
	mStopFlag=false;
	mPauseFlag=false;
	mQuiteFlag=false;
	mSQLCount=0;
	mBadCount=0;
	mGoodCount=0;
	}


////////////////////////////////////////////////////////////
MSQLExec::~MSQLExec(void)
	{
	Destroy();
	}


////////////////////////////////////////////////////////////
bool MSQLExec::ProcessSQLFile(const char *odbcdsn,const char *filename
		,bool bstop,bool bpause,bool bquite)
	{
	mStopFlag=bstop;
	mPauseFlag=bpause;
	mQuiteFlag=bquite;

	if(MSQLParserDatabase::Create(odbcdsn)==false) {  return false; }

	MFileInput filein;

	
	if(filein.Create(filename)==false)
		{
		MStdPrintf("**Unable to open file(%s) for reading\n",filename);
		MSQLParserDatabase::Destroy();
		return false;
		}

	char ch;
	mSQLCount=0;
	mBadCount=0;
	mGoodCount=0;

	while(filein.Read(ch)==true)
		{
		if(MSQLParserDatabase::DoChar(ch)==false)
			{
			MStdPrintf("\n                        ==> Execution Stopped \n\n");
			break;
			}
		}

	filein.Destroy();

	MSQLParserDatabase::Destroy();
	MStdPrintf("+-------------------------------------------------------------\n");
	MStdPrintf("|  SQLExec (%s) Tally:  Good=%d   Bad=%d  Total=%d\n"
			,filename,mGoodCount,mBadCount,mSQLCount);
	MStdPrintf("+-------------------------------------------------------------\n\n");
	return true;
	}


////////////////////////////////////////////////////////////
bool MSQLExec::OnExec(const char *sql)
	{
	mSQLCount += 1;
	if(mQuiteFlag==false )
		{
		MStdPrintf(	"\n\n"
			"--------------------------------------------\n"
			"SQL #%d: %s\n\n",mSQLCount,sql);
		}

	bool ret=MSQLParserDatabase::OnExec(sql);
	if(ret==true)
		{
		if(mQuiteFlag==false)
			{
			MStdPrintf("                                ==> SQL #%d OK\n",mSQLCount);
			}

		mGoodCount +=1;
		}
	else
		{
		mBadCount += 1;
		}

	if(mStopFlag==false && mPauseFlag==false)
		{  return true; }
	else if(mStopFlag==false && mPauseFlag==true)
		{
		if(AskStop()==true) { return false; }
		return true;
		}
	else if(mStopFlag==true && mPauseFlag==false)
		{  return ret;  }
	else // if(mStopFlag==true && mPauseFlag==true
		{
		if(ret==false) {  return !AskStop(); }
		return true;
		}
	}


////////////////////////////////////////////////////////////
bool MSQLExec::OnError(const char *error)
	{
	if(mQuiteFlag==false )
		{
		MStdPrintf("ERROR: %s\n",error);
		}

	return false;
	}


////////////////////////////////////////////////
bool MSQLExec::AskStop(void)
	{
	MStdPrintf("\n\n   *** Press S(stop) or other key to continue: "); 
	char ch=MStdGetCh();
	MStdPrintf("\r                                                \r");
	if(ch=='S'||ch=='s'||ch=='Q'||ch=='q') { return true; }
	return false;
	}



	


