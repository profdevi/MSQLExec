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


//v1.54 copyright Comine.com 20140617T0120
/*
Bug Notice:
	MStdSPrintf(const wchar_t *)  seems to be failing.
*/
/////////////////////////////////////////////////
#include "MStdLib.h"

//*******************************************
//* Display Standard Header Files and definitions
//*******************************************
#if (defined(MSTDLIB_API_WINDOWS) )
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <time.h>
	#include <math.h>
	#include <windows.h>
	#include <conio.h>
	#include <float.h>

	// Link in Libraries if on windows
	#if ( defined(MSTDLIB_OS_WINDOWS) )
	#pragma comment(lib,"user32.lib")
	#pragma comment(lib,"kernel32.lib")
	#endif

///////////////////////////////////////////
#elif ( defined(MSTDLIB_API_LINUX) )
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <wchar.h>

///////////////////////////////////////////
#else

	#if defined(MSTDLIB_OS_MOSYNC)
	#include <maapi.h>
	#include <mastdlib.h>
	#include <matime.h>
	#include <maheap.h>
	#include <madmath.h>
	#include <conprint.h>
	#include <mavsprintf.h>
	#include <MAFS/File.h>

	////////////////////////////
	static MA_FILE *stdin=NULL;
	static MA_FILE *stdout=NULL;
	static MA_FILE *stderr=NULL;

	#endif
#endif



//***************************************************
//** Module Elements
//***************************************************
static char GTempBuffer[30]="";
static wchar_t GTempWideBuffer[30]=L"";

//***************************************************
//** Functions
//***************************************************


/////////////////////////////////////////////////////
void MStdAssertInternal(bool flag,const char *exp,const char *filename,int lineno)
	{
	if(flag==true) { return; }
	char line[1000];
	MStdSPrintf(line,sizeof(line),"Assert Failed: %s:%d  %s",filename,lineno,exp);
	MStdPrintf("ASSERT Failed: %s:%s(%d)\n",exp,filename,lineno);
	return;
	}


///////////////////////////////////////////////////////////
void MStdError(const char *info,const char *filename,int lineno)
	{
	/////////////////////////////
	#if !defined(MSTDLIB_OS_MOSYNC)
	char line[1000];
	MStdSPrintf(line,sizeof(line),"Error: %s %s(%d)",info,filename,lineno);
	MStdPrintf("Error: %s %s(%d)\n",info,filename,lineno);
	return;	

	/////////////////////////////
	#else
	MStdPrintf("Error: %s %s(%d)\n",info,filename,lineno);
	return;

	#endif // MSTDLIB_OS_MOSYNC
	}


//////////////////////////////////////////////////////
MStdFileHandle MStdFileOpen(const char *filename,const char *mode)
	{
	#if defined(MSTDLIB_OS_WINDOWS)
	FILE *filehandle=NULL;
	if(fopen_s(&filehandle,filename,mode)!=0)
		{
		return NULL;
		}

	return (MStdFileHandle)filehandle;

	#else	// MSTDLIB_OS_WINDOWS

	return (MStdFileHandle)fopen(filename,mode);

	#endif // MSTDLIB_OS_WINDOWS
	}


//////////////////////////////////////////////////////
bool MStdSetStdInMode(bool binary)
	{
	const char *mode;
	if(binary==true)
		{  mode="rb";  }
	else
		{  mode="rt";  }

	//////////
	#if defined(MSTDLIB_OS_WINDOWS)
	//Reopen file handled for stdin/stdout to work
	FILE *tmp=NULL;
	if(freopen_s(&tmp,"CONIN$",mode,stdin)!=0)
		{
		return false;
		}

	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	if(freopen("CONIN$",mode,stdin)==NULL)
		{
		return false;
		}
				
	#elif (defined(MSTDLIB_OS_LINUX)  || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(freopen("/dev/tty",mode,stdin)==NULL)
		{
		return false;
		}
	#elif defined(MSTDLIB_OS_MOSYNC)
	stdin=fopen("/dev/tyy",mode);
	if(stdin==NULL)
		{
		return false;
		}

	#endif // MSTDLIB_OS_WINDOWS

	return true;
	}


//////////////////////////////////////////////////////
bool MStdSetStdOutMode(bool binary)
	{
	const char *mode;
	if(binary==true)
		{  mode="wb";  }
	else
		{  mode="wt";  }

	////////////////////////////////////
	#if defined(MSTDLIN_OS_WINDOWS)
	FILE *tmp=NULL;
	if(freopen_s(&tmp,"CONOUT$",mode,stdout)!=0)
		{
		return false;
		}

	return true;

	////////////////////////////////////
	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	if(freopen("CONOUT$",mode,stdout)==NULL)
		{
		return false;
		}

	return true;

	////////////////////////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(freopen("/dev/tty",mode,stdout)==NULL)
		{
		return false;
		}				

	return true;

	////////////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	stdout=fopen("/dev/tty",mode);
	if(stdout==NULL)
		{
		return false;
		}

	return true;
	#else
	return false;
	#endif // MSTDLIB_OS_WINDOWS				
	}


//////////////////////////////////////////////////////
bool MStdFileClose(MStdFileHandle handle)
	{
	if(fclose((FILE *)handle)!=0)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////
MStdFileHandle MStdGetStdIn(void)
	{  return (MStdFileHandle)stdin;  }


//////////////////////////////////////////////////////
MStdFileHandle MStdGetStdOut(void)
	{  return (MStdFileHandle)stdout;  }


////////////////////////////////////////////////////
bool MStdFileWrite(MStdFileHandle handle,const char *str)
	{
	int length=MStdStrLen(str);

	if(length!=(int)fwrite(str,sizeof(char),length,(FILE *)handle))
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////////////
int MStdFileWrite(MStdFileHandle handle,const void *buffer
		,int elementsize,int elementcount)
	{
	int newcount=(int)fwrite(buffer,(size_t)elementsize
			,(size_t)elementcount,(FILE *)handle);
	if(newcount!=elementcount)
		{
		return newcount;
		}

	return newcount;
	}


///////////////////////////////////////////////////////////////
int MStdFileRead(MStdFileHandle handle,void *buffer
		,int elementsize,int elementcount)
	{
	int newcount=(int)fread(buffer,elementsize,elementcount,(FILE *)handle);
	return newcount;
	}


/////////////////////////////////////////////////////////////////
bool MStdFileGetString(MStdFileHandle handle,char *buffer,int buffersize)
	{
	if(fgets(buffer,buffersize,(FILE *)handle)==NULL)
		{  return false; }

	return true;
	}


///////////////////////////////////////////////////////
bool MStdFileFlush(MStdFileHandle handle)
	{
	#if !defined(MSTDLIB_OS_MOSYNC)
	if(fflush((FILE *)handle)!=0)
		{
		return false;
		}

	return true;

	////////////////////////////////
	#else
	return true;

	#endif // MSTDLIB_OS_MOSYNC	
	}


///////////////////////////////////////////////////////
bool MStdFileSeek(MStdFileHandle handle,int offset,int type)
	{
	if(fseek((FILE*)handle,offset,type)!=0)
		{
		return false;
		}

	return true;
	}


///////////////////////////////////////////////////
long MStdFileTell(MStdFileHandle handle)
	{
	return ftell((FILE *)handle);
	}


//////////////////////////////////////////////////////
bool MStdFileEOF(MStdFileHandle handle)
	{
	if(feof((FILE *)handle)==0)
		{
		return false;
		}

	return true;
	}



//////////////////////////////////////////////////////
int MStdPrintf(const char *format,...)
	{
	va_list args;
	va_start(args,format);
	int length=vprintf(format,args);
	va_end(args);

	return length;
	}


//////////////////////////////////////////////////////
int MStdErrPrintf(const char *format,...)
	{
	va_list args;
	va_start(args,format);

	///////////////////////////////
	#if defined(MSTDLIB_OS_MOSYNC)
	char buf[1000];
	int length=vsprintf(buf,format,args);

	if(length>=sizeof(buf)-1)
		{
		// Cut the length
		length=sizeof(buf)-1;
		buf[length]=0;
		}

	if(fwrite(buf,1,length,stderr)!=length)
		{
		return 0;
		}

	return length;

	//////////////////////////////
	#else
	int length=vfprintf(stderr,format,args);
	va_end(args);

	return length;

	#endif // MSTDLIB_OS_MOSYNC
	}


//////////////////////////////////////////////////////
bool MStdGetString(char *buffer,int size)
	{
	fgets(buffer,size,stdin);
	buffer[size-1]=0;
	return true;
	}


//////////////////////////////////////////////////////
bool MStdOpenConsoleIO(void)
	{
	#if defined(MSTDLIB_OS_WINDOWS) 
	if(AllocConsole()==FALSE) { return true; }

	FILE *tmp=NULL;
	if(freopen_s(&tmp,"CONIN$","r",stdin)!=0)
		{
		return false;
		}
				
	if(freopen_s(&tmp,"CONOUT$","w",stdout)!=0)
		{
		return false;
		}

	return true;

	/////////////////////////////////////////
	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	if(freopen("CONIN$","r",stdin)==NULL)
		{
		return false;
		}
				
	if(freopen("CONOUT$","w",stdout)==NULL)
		{
		return false;
		}

	return true;
	#elif( defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) )
	if(freopen("/dev/tty","r",stdin)==NULL)
		{
		return false;
		}
				
	if(freopen("/dev/tty","w",stdout)==NULL)
		{
		return false;
		}

	if(freopen("/dev/tty","w",stderr)==NULL)
		{
		return false;
		}

	return true;
	
	////////////////////////////////////////////////////
	#elif( defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(stdin==NULL && freopen("/dev/tty","r",stdin)==NULL)
		{
		return false;
		}
				
	if(stdout==NULL && freopen("/dev/tty","w",stdout)==NULL)
		{
		return false;
		}

	if(stderr==NULL && freopen("/dev/tty","w",stderr)==NULL)
		{
		return false;
		}

	return true;
	
	#elif defined(MSTDLIB_OS_MOSYNC)
	if(stdin!=NULL) { fclose(stdin);  stdin=NULL; }
	if(stdout!=NULL) { fclose(stdout); stdout=NULL; }
	if(stderr!=NULL) { fclose(stderr); stderr=NULL; }

	if((stdin=fopen("/dev/tty","r"))==NULL)
		{
		return false;
		}
				
	if((stdout=fopen("/dev/tty","w"))==NULL)
		{
		return false;
		}

	if((stderr=fopen("/dev/tty","w"))==NULL)
		{
		return false;
		}

	return true;

	#else
	return false;
	#endif // MSTDLIB_OS_WINDOWS
	}


/////////////////////////////////////////////////////////
bool MStdSetOutput(const char *filename)
	{
	#if defined(MSTDLIB_OS_WINDOWS)
	FILE *newfileout=NULL;
	if(freopen_s(&newfileout,filename,"a",stdout)!=0)
		{
		return false;
		}

	return true;

	////////////////////////////////////////
	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) || defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_IS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	FILE *newfileout=freopen(filename,"a",stdout);
	if(newfileout==NULL)
		{
		return false;
		}
	
	return true;

	////////////////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	if(stdout!=NULL) { fclose(stdout);  stdout=NULL; }

	if((stdout=fopen(filename,"w"))==NULL)
		{
		return false;
		}

	return true;
	#endif // MSTDLIB_OS_WINDOWS
	}


/////////////////////////////////////////////////////////
bool MStdSetInput(const char *filename)
	{
	#if defined(MSTDLIB_OS_WINDOWS)
	FILE *newfileout=NULL;
	if(freopen_s(&newfileout,filename,"r",stdin)!=0)
		{
		return false;
		}

	return true;

	////////////////////////////////
	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) || defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	FILE *newfilein=freopen(filename,"r",stdin);
	if(newfilein==NULL)
		{
		return false;
		}
	
	return true;

	////////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	if(stdin!=NULL) { fclose(stdin);  stdin=NULL; }

	if((stdin=fopen(filename,"r"))==NULL)
		{
		return false;
		}

	return true;

	#else
	return false;

	#endif // MSTDLIB_OS_WINDOWS
	}


////////////////////////////////////////////////////////
bool MStdSetError(const char *filename)
	{
	//////////////////////////////////////////////
	#if defined(MSTDLIB_OS_WINDOWS)
	FILE *newfileout=NULL;
	if(freopen_s(&newfileout,filename,"a",stderr)!=0)
		{
		return false;
		}

	return true;

	//////////////////////////////////////////////
	#elif ( defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) ||defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )

	FILE *newfilein=freopen(filename,"a",stderr);
	if(newfilein==NULL)
		{
		return false;
		}
	
	return true;

	//////////////////////////////////////////////	
	#elif defined(MSTDLIB_OS_MOSYNC)
	if(stderr!=NULL) { fclose(stderr);  stderr=NULL; }

	if((stderr=fopen("/dev/tty","r"))==NULL)
		{
		return false;
		}

	return true;

	//////////////////////////////////////////////
	#else
	return false;
	#endif // _MSC_VER
	}


//////////////////////////////////////////////////////
int MStdSPrintf(char *target,int targetsize,const char *format,...)
	{

	////////////////////////////////
	#if defined(MSTDLIB_OS_WINDOWS)
	va_list args;
	va_start(args,format);

	int length=vsnprintf_s(target,targetsize,targetsize,format,args);

	va_end(args);

	target[targetsize-1]=0;
	return length;

	////////////////////////////////
	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	va_list args;
	va_start(args,format);

	int length=_vsnprintf(target,targetsize,format,args);
	
	va_end(args);

	target[targetsize-1]=0;
	return length;

	////////////////////////////////
	#elif ( defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	va_list args;
	va_start(args,format);

	int length=vsnprintf(target,targetsize,format,args);
	
	va_end(args);

	target[targetsize-1]=0;
	return length;

	////////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	va_list args;
	va_start(args,format);

	int length=vsprintf(target,format,args);

	if(length>targetsize)
		{
		target[targetsize-1]=0;
		return length;
		}

	va_end(args);

	target[targetsize-1]=0;
	return length;

	///////////////////////////////////
	#else
	return 0;

	#endif // _MSC_VER
	}


/////////////////////////////////////////////////////
const char *MStdStr(bool value)
	{
	if(value==true)
		{  MStdStrCpy(GTempBuffer,"true");  }
	else
		{  MStdStrCpy(GTempBuffer,"false");  }

	return GTempBuffer;
	}


/////////////////////////////////////////////////////
const char *MStdStr(int value)
	{
	MStdSPrintf(GTempBuffer,sizeof(GTempBuffer),"%d",value);
	return GTempBuffer;
	}


/////////////////////////////////////////////////////
const char *MStdStr(float value)
	{
	MStdSPrintf(GTempBuffer,sizeof(GTempBuffer),"%f",value);
	return GTempBuffer;
	}


/////////////////////////////////////////////////////
const char *MStdStr(double value)
	{
	MStdSPrintf(GTempBuffer,sizeof(GTempBuffer),"%lf",value);
	return GTempBuffer;
	}


//////////////////////////////////////////////////////
bool MStdStrCpy(char *dst,const char *src)
	{
	char *q=dst;
	const char *s=src;
	while((*q++=*s++)!=0)
		{
		// Empty Body
		}

	return true;
	}


///////////////////////////////////////////////////////
bool MStdStrCpy(char *dst,int dstsize,const char *src)
	{
	char *q=dst;
	const char *s=src;
	int length;
	for(length=1;(*q++=*s++)!=0;length=length+1)
		{
		if(length>=dstsize)
			{
			dst[dstsize-1]=0;
			return false;
			}
		}
	
	return true;
	}


//////////////////////////////////////////////////////
int MStdStrLen(const char *src)
	{
	int length;
	for(length=0;*src!=0;++src)
		{  length=length+1;  }

	return length;
	}


//////////////////////////////////////////////////////
int MStdStrCmp(const char *str1,const char *str2)
	{
	const char *p=str1;
	const char *q=str2;
	for(;;)
		{
		if(*p!=*q)
			{
			// Return first difference
			return *p - *q;
			}

		// check for exit
		if(*p==0) { return 0; }

		// Increase pointers
		p=p+1;
		q=q+1;
		}
	}


//////////////////////////////////////////////////////
int MStdStrICmp(const char *str1,const char *str2)
	{
	char ch1;
	char ch2;
	const char *p=str1;
	const char *q=str2;
	for(;;)
		{
		ch1=MStdToLower(*p);
		ch2=MStdToLower(*q);
		if(ch1!=ch2)
			{
			// Return first difference
			return ch1 - ch2;
			}

		// check for exit
		if(ch1==0) { return 0; }

		// Increase pointers
		p=p+1;
		q=q+1;
		}	
	}


//////////////////////////////////////////////////////////
bool MStdStrCat(char *dst,int dstsize,const char *src)
	{
	// Find end of string
	int endindex;
	for(endindex=0;dst[endindex]!=0;endindex=endindex+1)
		{
		// Nothing to Do
		}

	//=End index is at the \0 of the first string

	if(endindex+1>dstsize)
		{
		return false;
		}

	int i;
	for(i=0; ;++i)
		{
		dst[endindex]=src[i];
		if(src[i]==0) { return true; }

		// Increase the endindex by 1
		endindex = endindex + 1;

		if(endindex>=dstsize)
			{
			dst[endindex]=0;
			return false;
			}
		}
	}


//////////////////////////////////////////////
bool MStdStrBegins(const char *str,const char *prefix)
	{
	if(str==NULL || prefix==NULL || *prefix==0)
		{
		// Bad Input strings
		return false;
		}

	int i;
	for(i=0;prefix[i]!=0;++i)
		{
		if(str[i]!=prefix[i])
			{
			return false;
			}
		}

	//=We checked all the chars upto 0

	return true;
	}


//////////////////////////////////////////////
bool MStdStrIBegins(const char *str,const char *prefix)
	{
	if(str==NULL || prefix==NULL || *prefix==0)
		{
		// Bad Input strings
		return false;
		}

	int i;
	for(i=0;prefix[i]!=0;++i)
		{
		if(MStdToUpper(str[i])!=MStdToUpper(prefix[i]) )
			{
			return false;
			}
		}

	//=We checked all the chars upto 0

	return true;
	}


/////////////////////////////////////////////////////
bool MStdStrEnds(const char *str,const char *suffix)
	{
	MStdAssert(str!=NULL);
	MStdAssert(suffix!=NULL);

	// Find the end of the string
	int length1=MStdStrLen(str);
	int length2=MStdStrLen(suffix);

	if(length2>length1)
		{  return false;  }

	if(length2==length1)
		{  return MStdStrBegins(str,suffix);  }
	
	//=Now we should check for comparison
	
	return MStdStrBegins(str+length1-length2,suffix);
	}


////////////////////////////////////////////////////
bool MStdStrIEnds(const char *str,const char *suffix)
	{
	MStdAssert(str!=NULL);
	MStdAssert(suffix!=NULL);

	// Find the end of the string
	int length1=MStdStrLen(str);
	int length2=MStdStrLen(suffix);

	if(length2>length1)
		{  return false;  }

	if(length2==length1)
		{  return MStdStrIBegins(str,suffix);  }
	
	//=Now we should check for comparison
	
	return MStdStrIBegins(str+length1-length2,suffix);
	}


/////////////////////////////////////////////
bool MStdIsSubStr(const char *str,const char *substring)
	{
	// Check if either are strings
	if(str==NULL || substring==NULL || *substring==0)
		{
		return false;
		}

	int i;
	for(i=0;str[i]!=0;++i)
		{
		if(MStdStrBegins(str+i,substring)==true)
			{
			//=We have found the substring
			return true;
			}
		}

	return false;
	}


/////////////////////////////////////////////
bool MStdIsISubStr(const char *str,const char *substring)
	{
	// Check if either are strings
	if(str==NULL || substring==NULL || *substring==0)
		{
		return false;
		}

	int i;
	for(i=0;str[i]!=0;++i)
		{
		if(MStdStrIBegins(str+i,substring)==true)
			{
			//=We have found the substring
			return true;
			}
		}

	return false;
	}


/////////////////////////////////////////////
bool MStdStrToLower(char *str)
	{
	if(str==NULL)
		{  return false;  }

	int i;
	for(i=0;str[i]!=0;++i)
		{  str[i]=MStdToLower(str[i]);  }

	return true;
	}


/////////////////////////////////////////////
bool MStdStrToUpper(char *str)
	{
	if(str==NULL)
		{  return false;  }

	int i;
	for(i=0;str[i]!=0;++i)
		{  str[i]=MStdToUpper(str[i]);  }

	return true;
	}


//////////////////////////////////////////////////////
bool MStdStrRemove(char *target,int pos,int length)
	{
	if(target==NULL)
		{  return false; }

	int strlength=MStdStrLen(target);
	if(pos<0 || pos>strlength)
		{
		return false;
		}

	if(pos+length>strlength)
		{  length=strlength-pos;  }

	// Now we start copying overwriting original
	int i;
	for(i=0; ;++i)
		{
		target[pos+i]=target[pos+length+i];
		if(target[pos+i]==0)
			{  break;  }
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MStdStrInsert(char *target,int maxtargetsize,int pos,const char *insert)
	{
	if(target==NULL || insert==NULL)
		{  return false;  }

	int targetlength=MStdStrLen(target);
	int sourcelength=MStdStrLen(insert);
	if(targetlength+sourcelength>=maxtargetsize)
		{
		return false;
		}

	if(pos<0 || pos>targetlength)
		{
		return false;
		}
	
	// Copy backwards
	int i;
	for(i=targetlength;i>=pos;--i)
		{
		target[i+sourcelength]=target[i];
		}

	// Place String at position
	for(i=0;i<sourcelength;++i)
		{
		target[pos+i]=insert[i];
		}

	return true;
	}


/////////////////////////////////////////////
int MStdStrCharCount(const char *str,char ch)
	{
	if(str==NULL)
		{  return 0; }

	const char *p;
	int count;

	count=0;
	for(p=str;*p!=0;++p)
		{
		if(*p==ch)
			{ count=count+1;  }
		}

	return count;
	}


/////////////////////////////////////////////////////////
bool MStdStrTrim(char *str)
	{
	char *src;
	char *dst;
	if(str==NULL)
		{
		return false;
		}

	for(src=dst=str;*src!=0;++src)
		{
		if(MStdIsSpace(*src)==true)
			{ continue; }

		*dst++=*src;
		}

	*dst=0;
	return true;
	}


/////////////////////////////////////////////////////////
bool MStdStrTrimRight(char *str)
	{
	char *src;
	for(src=str;*src!=0;++src) { } // Find End

	for(src=src-1; src>=str ;--src)
		{
		if(MStdIsSpace(*src)==false)
			{ break; }

		*src=0;
		}

	return true;	
	}


/////////////////////////////////////////////////////////
bool MStdStrTrimLeft(char *str)
	{
	char *src;
	char *dst;
	for(src=str;*src!=0;++src)
		{
		if(MStdIsSpace(*src)==false)
			{ break; }
		}

	// Copy String
	for(dst=str; ;++dst)
		{
		if((*dst=*src)==0)
			{ break; }

		src=src+1;
		}

	return true;	
	}


/////////////////////////////////////////////
int MStdSPrintf(wchar_t *target,int targetsize,const wchar_t *format,...)
	{

	///////////////////////////////////
	#if defined(MSTDLIB_OS_WINDOWS)
	va_list args;
	va_start(args,format);

	int length=0;

	length=_vsnwprintf_s(target,targetsize,_TRUNCATE,format,args);

	va_end(args);
	return length;	

	///////////////////////////////////
	#elif defined(MSTDLIB_OS_WINDOWSOLD)
	va_list args;
	va_start(args,format);

	int length=0;

	length=_vsnwprintf(target,targetsize,format,args);

	va_end(args);
	return length;	
	
	////////////////////////////////////
	#elif ( defined(MSTDLIB_OS_MINGW) )
	va_list args;
	va_start(args,format);

	int length=0;

	// MINGW Does not have a size argument
	length=vswprintf(target,format,args);

	va_end(args);
	return length;	
	
	
	///////////////////////////////////
	#elif ( defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	va_list args;
	va_start(args,format);

	int length=0;

	length=vswprintf(target,targetsize,format,args);

	va_end(args);
	return length;	
	
	#endif // MSTDLIB_OS_WINDOWS

	return 0;
	}


//////////////////////////////////////////////////////////
const wchar_t *MStdStrWide(bool value)
	{
	if(value==true)
		{  MStdStrCpy(GTempBuffer,"true");  }
	else
		{  MStdStrCpy(GTempBuffer,"false");  }

	return (const wchar_t *)GTempBuffer;
	}


////////////////////////////////////////////////////////
const wchar_t *MStdStrWide(int value)
	{
	MStdSPrintf(GTempWideBuffer,sizeof(GTempWideBuffer)/sizeof(wchar_t),L"%d",value);
	return GTempWideBuffer;
	}


/////////////////////////////////////////////////////////
const wchar_t *MStdStrWide(float value)
	{
	MStdSPrintf(GTempWideBuffer,sizeof(GTempWideBuffer)/sizeof(wchar_t),L"%f",value);
	return GTempWideBuffer;
	}


////////////////////////////////////////////////////////////
const wchar_t *MStdStrWide(double value)
	{
	MStdSPrintf(GTempWideBuffer,sizeof(GTempWideBuffer)/sizeof(wchar_t),L"%lf",value);
	return GTempWideBuffer;
	}


////////////////////////////////////////////////////////////
bool MStdStrCpy(wchar_t *dst,const wchar_t *src)
	{
	wchar_t *q=dst;
	const wchar_t *s=src;
	while((*q++=*s++)!=0)
		{
		// Empty Body
		}

	return true;
	}


///////////////////////////////////////////////////////////
bool MStdStrCpy(wchar_t *dst,int dstsize,const wchar_t *src)
	{
	wchar_t *q=dst;
	const wchar_t *s=src;
	int length;
	for(length=1;(*q++=*s++)!=0;length=length+1)
		{
		if(length>=dstsize)
			{
			dst[dstsize-1]=0;
			return false;
			}
		}
	
	return true;
	}


///////////////////////////////////////////////////////////////
int MStdStrLen(const wchar_t *src)
	{
	int length;
	for(length=0;*src!=0;++src)
		{  length=length+1;  }

	return length;
	}


/////////////////////////////////////////////////////////////////
int MStdStrCmp(const wchar_t *str1,const wchar_t *str2)
	{
	const wchar_t *p=str1;
	const wchar_t *q=str2;
	for(;;)
		{
		if(*p!=*q)
			{
			// Return first difference
			return *p - *q;
			}

		// check for exit
		if(*p==0) { return 0; }

		// Increase pointers
		p=p+1;
		q=q+1;
		}
	}


/////////////////////////////////////////////////////////////////
int MStdStrICmp(const wchar_t *str1,const wchar_t *str2)
	{
	wchar_t ch1;
	wchar_t ch2;
	const wchar_t *p=str1;
	const wchar_t *q=str2;
	for(;;)
		{
		ch1=MStdToLower(*p);
		ch2=MStdToLower(*q);
		if(ch1!=ch2)
			{
			// Return first difference
			return ch1 - ch2;
			}

		// check for exit
		if(ch1==0) { return 0; }

		// Increase pointers
		p=p+1;
		q=q+1;
		}	
	}


///////////////////////////////////////////////////////
bool MStdStrCat(wchar_t *dst,int dstsize,const wchar_t *src)
	{
	// Find end of string
	int endindex;
	for(endindex=0;dst[endindex]!=0;endindex=endindex+1)
		{
		// Nothing to Do
		}

	//=End index is at the \0 of the first string

	if(endindex+1>=dstsize)
		{
		return false;
		}

	int i;
	for(i=0; ;++i)
		{
		dst[endindex]=src[i];
		if(src[i]==0) { return true; }

		// Increase the endindex by 1
		endindex = endindex + 1;

		if(endindex>=dstsize)
			{
			dst[endindex]=0;
			return false;
			}
		}	
	}


///////////////////////////////////////////////////////
bool MStdStrBegins(const wchar_t *str,const wchar_t *prefix)
	{
	if(str==NULL || prefix==NULL || *prefix==0)
		{
		// Bad Input strings
		return false;
		}

	int i;
	for(i=0;prefix[i]!=0;++i)
		{
		if(str[i]!=prefix[i])
			{
			return false;
			}
		}

	//=We checked all the chars upto 0

	return true;
	}


//////////////////////////////////////////////////////
bool MStdStrIBegins(const wchar_t *str,const wchar_t *prefix)
	{
	if(str==NULL || prefix==NULL || *prefix==0)
		{
		// Bad Input strings
		return false;
		}

	int i;
	for(i=0;prefix[i]!=0;++i)
		{
		if(MStdToUpper(str[i])!=MStdToUpper(prefix[i]) )
			{
			return false;
			}
		}

	//=We checked all the chars upto 0

	return true;	
	}


/////////////////////////////////////////////////////
bool MStdStrEnds(const wchar_t *str,const wchar_t *suffix)
	{
	MStdAssert(str!=NULL);
	MStdAssert(suffix!=NULL);

	// Find the end of the string
	int length1=MStdStrLen(str);
	int length2=MStdStrLen(suffix);

	if(length2>length1)
		{  return false;  }

	if(length2==length1)
		{  return MStdStrBegins(str,suffix);  }
	
	//=Now we should check for comparison
	
	return MStdStrBegins(str+length1-length2,suffix);
	}


////////////////////////////////////////////////////
bool MStdStrIEnds(const wchar_t *str,const wchar_t *suffix)
	{
	MStdAssert(str!=NULL);
	MStdAssert(suffix!=NULL);

	// Find the end of the string
	int length1=MStdStrLen(str);
	int length2=MStdStrLen(suffix);

	if(length2>length1)
		{  return false;  }

	if(length2==length1)
		{  return MStdStrIBegins(str,suffix);  }
	
	//=Now we should check for comparison
	
	return MStdStrIBegins(str+length1-length2,suffix);
	}


////////////////////////////////////////////////
bool MStdIsSubStr(const wchar_t *str,const wchar_t *substring)
	{
	// Check if either are strings
	if(str==NULL || substring==NULL || *substring==0)
		{
		return false;
		}

	int i;
	for(i=0;str[i]!=0;++i)
		{
		if(MStdStrBegins(str+i,substring)==true)
			{
			//=We have found the substring
			return true;
			}
		}

	return false;
	}


////////////////////////////////////////////////////
bool MStdIsISubStr(const wchar_t *str,const wchar_t *substring)
	{
	// Check if either are strings
	if(str==NULL || substring==NULL || *substring==0)
		{
		return false;
		}

	int i;
	for(i=0;str[i]!=0;++i)
		{
		if(MStdStrIBegins(str+i,substring)==true)
			{
			//=We have found the substring
			return true;
			}
		}

	return false;
	}


//////////////////////////////////////////////////////
bool MStdStrToLower(wchar_t *str)
	{
	if(str==NULL)
		{  return false;  }

	int i;
	for(i=0;str[i]!=0;++i)
		{  str[i]=MStdToLower(str[i]);  }

	return true;
	}


//////////////////////////////////////////////////////
bool MStdStrToUpper(wchar_t *str)
	{
	if(str==NULL)
		{  return false;  }

	int i;
	for(i=0;str[i]!=0;++i)
		{  str[i]=MStdToUpper(str[i]);  }

	return true;
	}


//////////////////////////////////////////////////////
bool MStdStrRemove(wchar_t *target,int pos,int length)
	{
	if(target==NULL)
		{  return false; }

	int strlength=MStdStrLen(target);
	if(pos<0 || pos>strlength)
		{
		return false;
		}

	if(pos+length>strlength)
		{  length=strlength-pos;  }

	// Now we start copying overwriting original
	int i;
	for(i=0; ;++i)
		{
		target[pos+i]=target[pos+length+i];
		if(target[pos+i]==0)
			{  break;  }
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MStdStrInsert(wchar_t *target,int maxtargetsize,int pos,const wchar_t *insert)
	{
	if(target==NULL || insert==NULL)
		{  return false;  }

	int targetlength=MStdStrLen(target);
	int sourcelength=MStdStrLen(insert);
	if(targetlength+sourcelength>=maxtargetsize)
		{
		return false;
		}

	if(pos<0 || pos>targetlength)
		{
		return false;
		}
	
	// Copy backwards
	int i;
	for(i=targetlength;i>=pos;--i)
		{
		target[i+sourcelength]=target[i];
		}

	// Place String at position
	for(i=0;i<sourcelength;++i)
		{
		target[pos+i]=insert[i];
		}

	return true;
	}


/////////////////////////////////////////////
int MStdStrCharCount(const wchar_t *str,wchar_t ch)
	{
	if(str==NULL)
		{  return 0; }

	const wchar_t *p;
	int count;

	count=0;
	for(p=str;*p!=0;++p)
		{
		if(*p==ch)
			{ count=count+1;  }
		}

	return count;
	}


/////////////////////////////////////////////////////////
bool MStdStrTrim(wchar_t *str)
	{
	wchar_t *src;
	wchar_t *dst;
	if(str==NULL)
		{
		return false;
		}

	for(src=dst=str;*src!=0;++src)
		{
		if(MStdIsSpace(*src)==true)
			{ continue; }

		*dst++=*src;
		}

	*dst=0;
	return true;
	}


/////////////////////////////////////////////////////////
bool MStdStrTrimRight(wchar_t *str)
	{
	wchar_t *src;
	for(src=str;*src!=0;++src) { } // Find End

	for(src=src-1; src>=str ;--src)
		{
		if(MStdIsSpace(*src)==false)
			{ break; }

		*src=0;
		}

	return true;	
	}


/////////////////////////////////////////////////////////
bool MStdStrTrimLeft(wchar_t *str)
	{
	wchar_t *src;
	wchar_t *dst;
	for(src=str;*src!=0;++src)
		{
		if(MStdIsSpace(*src)==false)
			{ break; }
		}

	// Copy String
	for(dst=str; ;++dst)
		{
		if((*dst=*src)==0)
			{ break; }

		src=src+1;
		}

	return true;	
	}


/////////////////////////////////////////////////////////
bool MStdStrClean(char *buffer)
	{
	char *src;
	char *dst;
	for(src=dst=buffer; ;++src )
		{
		if(*src==0)
			{
			*dst = 0;
			return true;
			}

		if(MStdIsPrintable(*src)==true)
			{
			*dst=*src;
			dst = dst + 1;
			continue;
			}
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MStdStrClean(wchar_t *buffer)
	{
	wchar_t *src;
	wchar_t *dst;
	for(src=dst=buffer; ;++src )
		{
		if(*src==0)
			{
			*dst = 0;
			return true;
			}

		if(MStdIsPrintable(char(*src) )==true)
			{
			*dst=*src;
			dst = dst + 1;
			continue;
			}
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MStdWindowOutput(const char *title,const char *info)
	{
	///////////////////////////////////////
	#if (defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) )
	MessageBoxA(NULL,info,title,MB_OK);

	///////////////////////////////////////
	#elif defined(MSTDLIB_OS_MINGW)
	MessageBox(NULL,info,title,MB_OK);

	///////////////////////////////////////
	#else
	printf("%s",info);
	#endif // MSTDLIB_OS_WINDOWS

	return true;
	}


////////////////////////////////////////////////////////
bool MStdSleep(int ms)
	{
	if(ms<=0) {  return false;  }

	#if ( defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined (MSTDLIB_OS_MINGW) )
	Sleep(ms);
	return true;

	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(ms*1000<0)
		{
		// Do a double call if ms wraps int storage
		sleep(ms/1000);
		usleep((ms%1000)*1000);
		return true;
		}

	usleep(ms*1000);
	return true;

	#elif defined(MSTDLIB_OS_MOSYNC)
	for(;;)
		{
		MAEvent event;
		if(maGetEvent(&event)<=0) { break; }

		if(event.type==EVENT_TYPE_CLOSE)
			{
			maExit(0);
			return true;
			}
		}

	maWait(ms);
	
	return true;

	#else
	return false;
	#endif // MSTDLIB_OS_WINDOWS
	}


////////////////////////////////////////////////////////
bool MStdExit(int value)
	{
	/////////////////////////
	#if ( defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	exit(value);
	return true;

	////////////////////////
	#elif ( defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	exit(value);
	return true;

	////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	maExit(value);
	return true;

	////////////////////////
	#else
	return false;

	#endif // MSTDLIB_OS_WINDOWS
	}


////////////////////////////////////////////////////////
bool MStdIsPrintable(char ch)
	{
	if(ch>=32 && ch<=126) { return true; }
	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsSpace(char ch)
	{
	if(ch==' ' || ch=='\r' || ch=='\n' || ch=='\t')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsDigit(char ch)
	{
	if(ch>='0' && ch<='9')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsAlpha(char ch)
	{
	if(ch>='a' && ch<='z')
		{
		return true;
		}

	if(ch>='A' && ch<='Z')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsAlphaNumeric(char ch)
	{
	if(ch>='a' && ch<='z')
		{
		return true;
		}

	if(ch>='A' && ch<='Z')
		{
		return true;
		}

	if(ch>='0' && ch<='9')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsUpper(char ch)
	{
	if(ch>='A' && ch<='Z')
		{
		return true;
		}

	return false;	
	}


////////////////////////////////////////////////////////
bool MStdIsLower(char ch)
	{
	if(ch>='a' && ch<='z')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsBinary(char ch)
	{
	if(ch=='0' || ch =='1')
		{ return true; }

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsOctal(char ch)
	{
	if(ch>='0' && ch <='7')
		{ return true; }

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsHex(char ch)
	{
	if(ch>='0' && ch <='9')
		{ return true; }

	if(ch>='a' && ch<='f')
		{ return true;  }

	if(ch>='A' && ch<='F')
		{ return true;  }

	return false;	
	}


////////////////////////////////////////////////////////
char MStdToBinary(int value)
	{
	if(value==0)
		{  return '0';  }

	if(value==1)
		{  return '1';  }

	return 0;
	}


////////////////////////////////////////////////////////
char MStdToOctal(int value)
	{
	MStdAssert(value>=0 && value<8);
	return '0'+value;
	}


////////////////////////////////////////////////////////
char MStdToHex(int value)
	{
	MStdAssert(value>=0 && value<16);
	if(value<10)
		{  return '0'+value;  }

	return 'A'+(value-10);
	}


////////////////////////////////////////////////////////
int MStdFromBinary(char ch)
	{
	MStdAssert(ch=='0' || ch=='1');
	if(ch=='0')
		{  return 0; }

	if(ch=='1')
		{  return 1;  }

	return 0;
	}


////////////////////////////////////////////////////////
int MStdFromOctal(char ch)
	{
	MStdAssert(ch>='0' || ch<='7');
	return ch-'0';
	}


////////////////////////////////////////////////////////
int MStdFromHex(char ch)
	{
	if(ch>='0' && ch<='9')
		{  return ch-'0';  }

	if(ch>='a' && ch<='f')
		{  return 10+ch-'a';  }

	if(ch>='A' && ch<='F')
		{  return 10+ch-'A';  }

	return 0;
	}


////////////////////////////////////////////////////////
char MStdToLower(char ch)
	{
	if(ch>='A' && ch<='Z')
		{ ch = ch - 'A' + 'a'; }

	return ch;
	}


///////////////////////////////////////////////////////
char MStdToUpper(char ch)
	{
	if(ch>='a' && ch<='z')
		{ ch = ch - 'a' + 'A'; }

	return ch;
	}


///////////////////////////////////////////////////////
bool MStdIsNan(double val)
	{
	////////////////////
	#if ( defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	if(_isnan(val)==0)
		{  return false; }
	else
		{  return true;  }

	///////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(isnan(val)==0)
		{  return false;  }
	else
		{  return true;  }

	////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	if(isnan(val)==0)
		{  return false;  }
	else
		{  return true;  }

	#endif 

	return false;
	}


//////////////////////////////////////////////////////
bool MStdIsFinite(double val)
	{
	if(isfinite(val)) { return true; }
	return false;
	}


///////////////////////////////////////////////////////
bool MStdIsSpace(wchar_t ch)
	{
	if(ch==' ' || ch=='\r' || ch=='\n' || ch=='\t')
		{
		return true;
		}

	return false;
	}


//////////////////////////////////////////////////////
bool MStdIsDigit(wchar_t ch)
	{
	if(ch>='0' && ch<='9')
		{
		return true;
		}

	return false;
	}


/////////////////////////////////////////////////////
bool MStdIsAlpha(wchar_t ch)
	{
	if(ch>='a' && ch<='z')
		{
		return true;
		}

	if(ch>='A' && ch<='Z')
		{
		return true;
		}

	return false;
	}


///////////////////////////////////////////////////////
bool MStdIsAlphaNumeric(wchar_t ch)
	{
	if(ch>='a' && ch<='z')
		{
		return true;
		}

	if(ch>='A' && ch<='Z')
		{
		return true;
		}

	if(ch>='0' && ch<='9')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsUpper(wchar_t ch)
	{
	if(ch>='A' && ch<='Z')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
bool MStdIsLower(wchar_t ch)
	{
	if(ch>='a' && ch<='z')
		{
		return true;
		}

	return false;
	}


////////////////////////////////////////////////////////
wchar_t MStdToLower(wchar_t ch)
	{
	if(ch>='A' && ch<='Z')
		{ ch = ch - 'A' + 'a'; }

	return ch;
	}


////////////////////////////////////////////////////////
wchar_t MStdToUpper(wchar_t ch)
	{
	if(ch>='a' && ch<='z')
		{ ch = ch - 'a' + 'A'; }

	return ch;
	}


///////////////////////////////////////////////////////
bool MStdMemCpy(void *target,const void *src,int count)
	{
	int newcount=count/sizeof(int);
	int i;

	int *idst=(int *)target;
	int *isrc=(int *)src;

	// Copy Memory Block
	for(i=0;i<newcount;++i)
		{  *idst++=*isrc++;  }

	int copied=newcount*sizeof(int);
	int remaining=count-copied;
	if(remaining==0){  return true;	 }

	char *cdst=((char *)target)+copied;
	char *csrc=((char *)src)+copied;

	// copy the remainint chars
	for(i=0;i<remaining;++i)
		{
		*cdst++ = *csrc++;
		}

	return true;
	}


////////////////////////////////////////////////////////
bool MStdMemSet(void *dst,int value,int size)
	{
	char *cdst=(char *)dst;
	int i;
	for(i=0;i<size;++i)
		{  *cdst++=value;  }

	return true;
	}


////////////////////////////////////////////////////////
bool MStdMemZero(void *dst,int size)
	{
	int intcount=size/sizeof(int);
	int *idst=(int *)dst;
	int i;
	for(i=0;i<intcount;++i)
		{  *idst++=0;  }

	// Remaining bytes 
	int remaining=size-intcount*sizeof(int);
	if(remaining==0) { return true; }

	//=Still a couple of bytes left

	char *cdst=(char *)idst;
	for(i=0;i<remaining;++i)
		{  *cdst=0;  }

	return true;
	}


//////////////////////////////////////////////////////
int MStdMemCmp(const void *src,const void *dst,int size)
	{
	int i;
	const char *p=(const char *)src;
	const char *q=(const char *)dst;
	for(i=0;i<size;++i)
		{
		if(*p!=*q)
			{ return *p-*q; }

		p=p+1;
		q=q+1;
		}

	return 0;
	}


///////////////////////////////////////////////////////
bool MStdMemMove(void *dst,void *src,int count)
	{
	memmove(dst,src,count);
	return true;
	}


////////////////////////////////////////////////////////
void *MStdMalloc(int size)
	{
	return malloc(size);
	}


////////////////////////////////////////////////////////
bool MStdFree(void *memblock)
	{
	free(memblock);
	return true;
	}


///////////////////////////////////////////////////////
void *MStdReAlloc(void *memblock,int newsize)
	{
	return realloc(memblock,newsize);
	}


////////////////////////////////////////////////////
bool MStdKbHit(void)
	{
	////////////////////////////////
	#if (defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	if(_kbhit())
		{ return true; }

	return false;

	///////////////////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	getchar();
	return true;

	//////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	for(;;)
		{
		MAEvent event;
		if(maGetEvent(&event)<=0) { break; }

		if(event.type==EVENT_TYPE_CLOSE)
			{
			maExit(0);
			return true;
			}

		if(event.type==EVENT_TYPE_CHAR)
			{
			break; 
			}
		}

	return true;

	#else
	return false;
	#endif // MSTDLIB_OS_WINDOWS
	}


///////////////////////////////////////////////////
int MStdGetCh(bool forcestdio)
	{
	////////////////////////////
	#if (defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) )
	if(forcestdio==true)
		{  return getchar(); }

	return _getch();

	////////////////////////////
	#elif defined(MSTDLIB_OS_MINGW)
	if(forcestdio==true)
		{  return getchar();  }
	
	return _getch();
	
	///////////////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(forcestdio==true)
		{  return getchar();  }

	char ch;
	if(read(0,&ch,1)<=0)
		{  return 0; }

	return ch;

	///////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	for(;;)
		{
		maWait(0);
		MAEvent event;
		if(maGetEvent(&event)<=0) { continue; }

		if(event.type==EVENT_TYPE_CLOSE)
			{
			maExit(0);
			return true;
			}

		if(event.type==EVENT_TYPE_KEY_PRESSED)
			{
			if(forcestdio==true)
				{
				printf("%c",event.key);
				}

			return event.key;
			}
		}

	
	#endif // MSDLIB_OS_WINDOWS
	}


////////////////////////////////////////////////
time_t MStdGetTime(void)
	{
	////////////////////////////////////////////
	#if defined(MSTDLIB_OS_MOSYNC)
	return (time_t)maTime();

	////////////////////////////////////////////
	#else
	time_t timecount;
	time(&timecount);
	return timecount;

	#endif // MSTDLIB_OS_MOSYNC
	}


////////////////////////////////////////////////
bool MStdCTime(char *buf,int bufsize,time_t *tmdata)
	{
	// Reset size of buffer	
	buf[0]=0;

	///////////////////////////////////////
	#if defined(MSTDLIB_OS_WINDOWS)
	if(ctime_s(buf,bufsize,tmdata))
		{
		return false;
		}
	
	// Return current time
	return true;

	///////////////////////////////////////
	#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) || defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	const char *str=ctime(tmdata);
	if(str==NULL)
		{
		return false;
		}

	if(MStdStrLen(str)>=bufsize)
		{
		return false;
		}

	MStdStrCpy(buf,bufsize,str);
	return true;

	//////////////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	// Convert time_t to 
	struct tm splitdata;
	split_time(*tmdata,&splitdata);
	
	// Buffer required by matime.h
	char tmpbuf[26];
	sprint_tm(&splitdata,tmpbuf);

	MStdStrCpy(buf,bufsize,tmpbuf);
	#endif // MSTDLIB_OS_WINDOWS
	}


/////////////////////////////////////////////////
bool MStdAtExit(void fun(void))
	{
	////////////////////////
	#if !defined(MSTDLIB_OS_MOSYNC)
	if(atexit(fun))
		{
		return false;
		}

	return true;

	////////////////////////
	#else
	return false;
	#endif // MSTDLIB_OS_MOSYNC
	}


/////////////////////////////////////////////////
bool MStdFileRemove(const char *filename)
	{
	/////////////////////////////
	#if !defined(MSTDLIB_OS_MOSYNC)
	if(remove(filename)!=0)
		{
		// Do not generate an error
		return false;
		}

	return true;

	/////////////////////////////
	#else
	MAHandle file;
	file=maFileOpen(filename,MA_ACCESS_READ_WRITE);
	if(file<0)
		{
		return false;
		}

	if(maFileExists(file)==0)
		{
		//=File Does not exist
		maFileClose(file);
		return true;
		}

	if(maFileDelete(file)<0)
		{
		maFileClose(file);
		return false;
		}

	maFileClose(file);

	return true;	
	#endif // MSTDLIB_OS_MOSYNC
	}


/////////////////////////////////////////////////
bool MStdFileRename(const char *filesrc,const char *filetarget)
	{
	/////////////////////////////
	#if !defined(MSTDLIB_OS_MOSYNC)
	if(rename(filesrc,filetarget)!=0)
		{
		return false;
		}

	return true;

	/////////////////////////////
	#else
	MAHandle file;
	file=maFileOpen(filesrc,MA_ACCESS_READ_WRITE);
	if(file<0)
		{
		return false;
		}
	
	if(maFileRename(file,filetarget)<0)
		{
		maFileClose(file);
		return false;
		}

	maFileClose(file);
	return true;
	#endif

	}


//////////////////////////////////////////////////
bool MStdFileExists(const char *filename)
	{
	#if ( defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	DWORD fileinfo=GetFileAttributesA(filename);
	if(fileinfo==INVALID_FILE_ATTRIBUTES) { return false; }
	if((fileinfo & FILE_ATTRIBUTE_DIRECTORY)>0) { return false; }
	if((fileinfo & FILE_ATTRIBUTE_DEVICE)>0) { return false; }

	return true;

	///////////////////////////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) )
	if(access(filename,F_OK)==0)
		{
		return true;
		}

	return false;
	#endif // MSTDLIB_OS_WINDOWS

	return false;	
	}

//////////////////////////////////////////////////
bool MStdExec(const char *cmd)
	{
	////////////////////////////////////
	#if !defined(MSTDLIB_OS_MOSYNC)
	int ret=system(cmd);
	if(ret==0)
		{  return true; }

	return false;
	
	////////////////////////////////////
	#else 
	return false;

	#endif // MSTDLIB_OS_MOSYNC
	}


//////////////////////////////////////////////////
int MStdAToI(const char *str)
	{
	return atoi(str);
	}


//////////////////////////////////////////////////
int MStdAToI(const wchar_t *str)
	{
	char buf[100];
	MStdSPrintf(buf,sizeof(buf),"%S",str);
	return atoi(buf);
	}


/////////////////////////////////////////////////
bool MStdIToA(int inval,char *buffer,int length)
	{
	
	int maxlength=0;

	// Check for -(number) or 0.
	if(inval<=0) { maxlength = maxlength+1; }
	
	if(inval!=0)
		{
		int tmpval;
		for(tmpval=inval;tmpval!=0;tmpval=tmpval/10)
			{
			maxlength = maxlength + 1;
			}
		}

	if(maxlength+1>length)
		{
		buffer[0]=0;
		return false;
		}

	//=We now have the correct write size

	// Check for zero
	if(inval==0)
		{
		buffer[0]='0';
		buffer[1]=0;
		return true;
		}


	if(inval<0)
		{
		buffer[0]='-';
		inval = -inval;
		}
	
	// Now write normally
	int writepos = maxlength-1;
	while(inval>0)
		{
		buffer[writepos] = '0' + (inval % 10 );
		writepos = writepos - 1;
		inval = inval/10;
		}

	buffer[maxlength]=0;
	return true;
	}


/////////////////////////////////////////////////
bool MStdIToA(int inval,wchar_t *buffer,int length)
	{
	
	int maxlength=0;

	// Check for -(number) or 0.
	if(inval<=0) { maxlength = maxlength+1; }
	
	if(inval!=0)
		{
		int tmpval;
		for(tmpval=inval;tmpval!=0;tmpval=tmpval/10)
			{
			maxlength = maxlength + 1;
			}
		}

	if(maxlength+1>length)
		{
		buffer[0]=0;
		return false;
		}

	//=We now have the correct write size

	// Check for zero
	if(inval==0)
		{
		buffer[0]='0';
		buffer[1]=0;
		return true;
		}


	if(inval<0)
		{
		buffer[0]='-';
		inval = -inval;
		}
	
	// Now write normally
	int writepos = maxlength-1;
	while(inval>0)
		{
		buffer[writepos] = '0' + (inval % 10 );
		writepos = writepos - 1;
		inval = inval/10;
		}

	buffer[maxlength]=0;
	return true;
	}




///////////////////////////////////////////////////
unsigned int MStdHToI(const char *str)
	{
	unsigned int retval=0;

	int i;
	for(i=0;str[i]!=0;++i)
		{
		const char ch=str[i];
		int converteddigit;
		if(ch>='0' && ch<='9')
			{
			converteddigit = (ch-'0');
			}
		else if(ch>='A' && ch<='F')
			{
			converteddigit = (ch - 'A') + 10;
			}
		else if(ch>='a' && ch<='f')
			{
			converteddigit = (ch - 'a') + 10;
			}
		else
			{
			return retval;
			}

		retval = retval*16 + converteddigit;
		}

	return retval;
	}


///////////////////////////////////////////////////
unsigned MStdHToI(const wchar_t *str)
	{
	unsigned int retval=0;

	int i;
	for(i=0;str[i]!=0;++i)
		{
		const wchar_t ch=str[i];
		int converteddigit;
		if(ch>='0' && ch<='9')
			{
			converteddigit = (ch-'0');
			}
		else if(ch>='A' && ch<='F')
			{
			converteddigit = (ch - 'A') + 10;
			}
		else if(ch>='a' && ch<='f')
			{
			converteddigit = (ch - 'a');
			}
		else
			{
			return retval;
			}

		retval = retval*16 + converteddigit;
		}

	return retval;
	}


///////////////////////////////////////////////////
double MStdAToF(const char *str)
	{
	return atof(str);
	}


//////////////////////////////////////////////////
double MStdAToF(const wchar_t *str)
	{
	char buf[100];
	MStdSPrintf(buf,sizeof(buf),"%S",str);
	return atof(buf);
	}


///////////////////////////////////////////////////
void *MStdIToP(int val)
	{
	////////////////////////////////////
	#if (defined(MSTDLIB_OS_WINDOWS)  || defined(MSTDLIB_OS_WINDOWSOLD) )
	return (void *)(__int64)val;

	////////////////////////////////////
	#else
	return (void *)(long)val;

	#endif	
	}


///////////////////////////////////////////////////
int MStdPToI(void *val)
	{
	//////////////////////////
	#if (defined(MSTDLIB_OS_WINDOWS)  || defined(MSTDLIB_OS_WINDOWSOLD) )
	return (int)(__int64)val;

	//////////////////////////
	#else	
	return (int)(long)val;
	#endif	// MSTDLIB_OS_WINDOWS
	}


///////////////////////////////////////////////////
int MStdGetMax(int value1,int value2)
	{
	if(value1>value2)
		{  return value1; }

	return value2;
	}


///////////////////////////////////////////////////
float MStdGetMax(float value1,float value2)
	{
	if(value1>value2)
		{  return value1; }

	return value2;
	}


///////////////////////////////////////////////////
double MStdGetMax(double value1,double value2)
	{
	if(value1>value2)
		{  return value1; }

	return value2;
	}


///////////////////////////////////////////////////
int MStdGetMin(int value1,int value2)
	{
	if(value1<value2)
		{  return value1; }

	return value2;
	}


///////////////////////////////////////////////////
float MStdGetMin(float value1,float value2)
	{
	if(value1<value2)
		{  return value1; }

	return value2;
	}


///////////////////////////////////////////////////
double MStdGetMin(double value1,double value2)
	{
	if(value1<value2)
		{  return value1; }

	return value2;
	}



///////////////////////////////////////////////////
int MStdGetAbs(int value)
	{
	if(value>=0)
		{  return value; }

	return -value;
	}



///////////////////////////////////////////////////
float MStdGetAbs(float value)
	{
	if(value>=0)
		{  return value; }

	return -value;
	}


///////////////////////////////////////////////////
double MStdGetAbs(double value)
	{
	if(value>=0)
		{  return value; }

	return -value;
	}


////////////////////////////////////////////////////
void MStdSRand(void)
	{
	time_t tmdata;

	/////////////////////////////////////////
	#if  (defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	time(&tmdata);
	tmdata = (time_t) (tmdata ^ GetCurrentProcessId() );

	srand((unsigned int)tmdata);
	return;

	/////////////////////////////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	time(&tmdata);
	tmdata = (time_t) (tmdata ^ getpid() );

	srand((unsigned int)tmdata);
	return;

	/////////////////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	tmdata=(time_t)maTime();
	srand((unsigned int)tmdata);

	return;

	////////////////////////////////////////
	#else
	return;

	#endif // MSTDLIB_OS_WINDOWS
	}


////////////////////////////////////////////////////
void MStdSRand(int seed)
	{
	// Will Reimplement in terms of x<-(Ax+B) mod C 
	// Need to find Good Values for A/B/C
	srand((unsigned int)seed);
	}


////////////////////////////////////////////////////
int MStdRand(int maxvalue)
	{
	// Will Reimplement in terms of x<-(Ax+B) mod C 
	// Need to find Good Values for A/B/C
	return rand()%maxvalue;
	}


///////////////////////////////////////////////////
bool MStdCompare(int val1,int val2,int error)
	{
	int diff=val1-val2;
	if(diff>=-error && diff<=+error)
		{ return true;  }

	return false;
	}


///////////////////////////////////////////////////
bool MStdCompare(unsigned int val1,unsigned int val2,unsigned int error)
	{
	if(val1>val2)
		{
		unsigned int diff=val1-val2;
		if(diff<=(unsigned int)error)
			{ return true;  }

		return false;
		}
	else
		{
		unsigned int diff=val2-val1;
		if(diff<=(unsigned int)error)
			{ return true;  }

		return false;		
		}
	}


///////////////////////////////////////////////////
bool MStdCompare(float val1,float val2,float error)
	{
	float diff=val1-val2;
	if(diff>=-error && diff<=+error)
		{ return true;  }

	return false;
	}


//////////////////////////////////////////////////
bool MStdCompare(double val1,double val2,double error)
	{
	double diff=val1-val2;
	if(diff>=-error && diff<=+error)
		{ return true;  }

	return false;
	}


///////////////////////////////////////////////////
unsigned int MStdGetTimeOfDay(void)
	{
	////////////////////////////
	#if (defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	return GetTickCount();

	////////////////////////////
	#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	// Defined int <sys/time.h>
	struct timeval tv;
	gettimeofday(&tv,0);

	return ((unsigned int)tv.tv_sec)*1000
			+((unsigned int)tv.tv_usec)/1000;

	////////////////////////////
	#elif defined(MSTDLIB_OS_MOSYNC)
	return maGetMilliSecondCount();

	#endif // MSTDLIB_OS_WINDOWS
	}


////////////////////////////////////////////////////
bool MStdLog(const char *logentry,const char *file,int lineno)
	{
	MStdPrintf("**Log : %s (%s:%d)\n",logentry,file,lineno);
	return true;
	}


/////////////////////////////////////////////////////
// Global Constants
// Reference Constants: http://en.wikipedia.org/wiki/Pi
const double MStdLibConstPi=3.14159265358979323846;


//////////////////////////////////////////////////////
bool MStdGetMachineName(char *buffer,int bufferlen)
	{
	MStdAssert(buffer!=0 && bufferlen>40);
	////////////////////////////////////////
	#if ( defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	DWORD dwbufferlen=bufferlen;
	if(GetComputerNameA(buffer,&dwbufferlen)==FALSE)
		{
		return false;
		}

	return true;

	////////////////////////////////////////
	#elif ( defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) || defined(MSTDLIB_OS_IPHONE) )
	if(gethostname(buffer,bufferlen)!=0)
		{
		return false;
		}

	return true;

	////////////////////////////////////////
	#elif defined (MSTDLIB_OS_MOSYNC)
	MStdStrCpy(buffer,bufferlen,"MoSync");
	return true;

	#endif

	}


