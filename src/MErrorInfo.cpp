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


//v2.5  copyright Comine.com 20140531S1820
#include "MStdLib.h"
///////////////////////////////////////////
#ifdef MSTDLIB_API_WINDOWS
#include <windows.h>
#endif // _WIN32

////////////////////////////////////////////
#include <errno.h>
#include "MBuffer.h"
#include "MString.h"
#include "MErrorInfo.h"


////////////////////////////////////////////
static const int GMAXERRORSIZE=2000;
static char * const GINTERNALSTRING="";


////////////////////////////////////////////
void MErrorInfo::ClearObject(void)
	{
	}


////////////////////////////////////////////
MErrorInfo::MErrorInfo(void)
	{
	ClearObject();
	mError.Create("");
	}


////////////////////////////////////////////
MErrorInfo::~MErrorInfo(void)
	{  
	mError.Destroy();
	}


////////////////////////////////////////////
void MErrorInfo::SetError(const char *str)
	{ 
	if(mError.Create(str)==false)
		{
		return;
		}
	}


////////////////////////////////////////////
bool MErrorInfo::SetError(void)			// Will Set win32 error
	{
	//////////////////////////////////////////
	#ifdef MSTDLIB_API_WINDOWS
	MBuffer temp;
	if(temp.Create(GMAXERRORSIZE)==false)
		{
		return false;
		}

	if(FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),temp,GMAXERRORSIZE,0)==0)
		{
		SetError(GINTERNALSTRING);
		return false;
		}

	SetError(temp);
	return true;

	//////////////////////////////////////////
	#else

	if(errno==0) { SetError(INTERNALSTRING); return true; }
	SetError(strerror(errno));
	return true;
	#endif // MSTDLIB_API_WINDOWS
	}


////////////////////////////////////////////
bool MErrorInfo::SetError(int errornum)
	{
	//////////////////////////////////////////
	#ifdef MSTDLIB_API_WINDOWS
	MBuffer temp;
	if(temp.Create(GMAXERRORSIZE)==false)
		{
		return false;
		}

	if(FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,NULL,errornum,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),temp.GetBuffer(),GMAXERRORSIZE,0)==0)
		{
		SetError(GINTERNALSTRING);
		return false;
		}	

	SetError(temp.GetBuffer() );
	return true;

	//////////////////////////////////////////
	#else

	if(errornum==0) { SetError(GINTERNALSTRING); return true; }
	SetError(strerror(errornum));  return true;
	#endif // MSTDLIB_API_WINDOWS
	}


////////////////////////////////////////////
const char *MErrorInfo::GetError(void)
	{  return mError.Get(); }


/////////////////////////////////////////////
bool MErrorInfo::Print(unsigned int error)
	{
	//////////////////////////////////////////
	#ifdef MSTDLIB_API_WINDOWS
	if(error==0) { error=GetLastError(); }

	MBuffer temp;
	if(temp.Create(GMAXERRORSIZE)==false)
		{
		return false;
		}

	if(FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,NULL,error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),temp.GetBuffer(),GMAXERRORSIZE,0)==0)
		{
		MStdError("Unable to get last Error from GetLastError()",__FILE__,__LINE__);
		return false;
		}

	MStdError(temp.GetBuffer(),__FILE__,__LINE__);
	return true;

	//////////////////////////////////////////
	#else

	if(error==0) { perror(0); return true; }
	fprintf(stderr,"Error : %s\n",strerror((int)error));
	return true;

	#endif // MSTDLIB_API_WINDOWS
	}



