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


//v1.53 copyright Comine.com 20140518U0936
#ifndef MStdLib_h
#define MStdLib_h

/* Notes:
 MoSync :
		1   create Compiler Define MSTDLIB_OS_MOSYNC
		2.  Link with libraries: mautil.lib,maui.lib,mafs.lib
			Link with libraries(Debugging): mautild.lib,mauid.lib,mafsd.lib

 */

//********************************************
//** Begin of Platform Definitions
//********************************************
#if defined(_MSC_VER)

	// Older version of MSVC did not have wchar.h
	#if (_MSC_VER>1300 )

		#ifndef MSTDLIB_OS_WINDOWS
		#define MSTDLIB_OS_WINDOWS							(1)
		#endif // MSTDLIB_OS_WINDOWS

	#else

		#ifndef MSTDLIB_OS_WINDOWSOLD
		#define MSTDLIB_OS_WINDOWSOLD						(1)
		#endif // MSTDLIB_OS_WINDOWSOLD

	#endif // _MSVCER

/////////////////////////////////////////
#elif ( defined(__MINGW32__) || defined(__MINGW64__) )

	#ifndef MSTDLIB_OS_MINGW
	#define MSTDLIB_OS_MINGW							(1)
	#endif  //MSTDLIB_OS_MINGW

/////////////////////////////////////////
#elif defined(__linux__)

	#ifndef MSTDLIB_OS_LINUX
	#define MSTDLIB_OS_LINUX							(1)
	#endif // MSTDLIB_OS_LINUX

#elif defined(__APPLE__)

	#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
		#ifndef MSTDLIB_OS_IPHONE
		#define MSTDLIB_OS_IPHONE							(1)
		#endif // MSTDLIB_OS_IPHONE

	#else
	
		#ifndef MSTDLIB_OS_MACOS
		#define MSTDLIB_OS_MACOS							(1)
		#endif // MSTDLIB_OS_MACOS
	
	#endif // __IPHONE_OS_VERSION_MIN_REQUIRED

/////////////////////////////////////////
#elif defined(MSTDLIB_OS_MOSYNC)

// No Definition accept by hand

/////////////////////////////////////////
#else

	#ifndef MSTDLIB_OS_OTHER
	#define MSTDLIB_OS_OTHER						(1)
	#endif // MSTDLIB_OS_OTHER

#endif

//********************************************
//** End of Platform Definitions
//********************************************


//********************************************
//** API Defines for simpler defines: Windows, Linux, Other
//********************************************
#if (defined(MSTDLIB_OS_WINDOWS) || defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
	#define	MSTDLIB_API_WINDOWS							(1)

#elif (defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_MACOS) || defined(MSTDLIB_IPHONE) )
	#define MSTDLIB_API_LINUX							(1)

#else
	#define MSTDLIB_API_OTHER							(1)

#endif



//*******************************************
//* Display Standard Header Files
//*******************************************
#if defined(MSTDLIB_OS_WINDOWS)
#include <time.h>
#include <math.h>
#include <new>

#elif (defined(MSTDLIB_OS_WINDOWSOLD) || defined(MSTDLIB_OS_MINGW) )
#include <time.h>
#include <math.h>
#include <wchar.h>
#include <new>

#elif ( defined(MSTDLIB_OS_LINUX) || defined(MSTDLIB_OS_OTHER) || defined(MSTDLIB_OS_MACOS) )
#include <time.h>
#include <math.h>
#include <wchar.h>
#include <new>

#elif defined(MSTDLIB_OS_IPHONE)
#include <time.h>
#include <math.h>
#include <wchar.h>
#include <new>

#elif defined(MSTDLIB_OS_MOSYNC)
#include <matime.h>
#include <madmath.h>
#include <new>

#endif // MSTDLIB_OS_WINDOWS

///////////////////////////////////////////////
#ifndef	NULL
#define NULL			(0)
#endif

//********************************************
//** Assert Routines
//********************************************
#ifdef NDEBUG
#define MStdAssert(exp)
#else
////////////////////////////////////////////////////////////////
#define MStdAssert(exp)	MStdAssertInternal(exp,#exp,__FILE__,__LINE__)
#endif // NDEBUG


//********************************************
//** Error Routines
//********************************************
#ifndef MStdDebug
#define MStdDebug(infostr)		MStdError(infostr,__FILE__,__LINE__)
#endif // MStdDebug

//////////////////////////////////////////////////
void MStdAssertInternal(bool flag,const char *exp,const char *filename,int lineno);
void MStdError(const char *info,const char *filename,int lineno);

//////////////////////////////////////////////////
// Abstract File Handles
typedef void *MStdFileHandle;

//////////////////////////////////////////////////
MStdFileHandle MStdFileOpen(const char *filename,const char *mode);		// Open file
bool MStdSetStdInMode(bool binary=false);				// Reset Mode to binary/text
bool MStdSetStdOutMode(bool binary=false);				// Open StdInput
bool MStdFileClose(MStdFileHandle handle);				// Close File Handle
MStdFileHandle MStdGetStdIn(void);						// Get File stdin
MStdFileHandle MStdGetStdOut(void);						// Get File stdout
bool MStdFileWrite(MStdFileHandle handle,const char *stri);		// Write a string
int MStdFileWrite(MStdFileHandle handle,const void *buffer
		,int elementsize,int elementcount);				// Write to file
int MStdFileRead(MStdFileHandle handle,void *buffer,int elementsize
		,int elementcount);								// Read file
bool MStdFileGetString(MStdFileHandle handle,char *buffer,int buffersize);
bool MStdFileFlush(MStdFileHandle handle);				// Flush the file
bool MStdFileSeek(MStdFileHandle handle,int offset,int type);	//type 0-start,1-middle,2-end
long MStdFileTell(MStdFileHandle handle);				// Get Position in file
bool MStdFileEOF(MStdFileHandle handle);				// =true if endof file

//////////////////////////////////////////////////
// Console Operations
int MStdPrintf(const char *format,...);					// Console Output
int MStdErrPrintf(const char *format,...);				// Console Error
bool MStdGetString(char *buffer,int size);				// Read Console String
bool MStdOpenConsoleIO(void);							// Reopen console Input
bool MStdSetOutput(const char *filename);				// Set Standard ouput to file
bool MStdSetInput(const char *filename);				// Set Standard Input from file
bool MStdSetError(const char *filename);				// Set Standard Error to file

//////////////////////////////////////////////////
// String Operations
int MStdSPrintf(char *target,int targetsize,const char *format,...);	// String Formated Printf
const char *MStdStr(bool value);						// Build a temp string
const char *MStdStr(int value);							// Build a temp string
const char *MStdStr(float value);						// Build a temp string
const char *MStdStr(double value);						// Build a temp string
bool MStdStrCpy(char *dst,const char *src);				// String Copy
bool MStdStrCpy(char *dst,int dstsize,const char *src);	// String Copy
int MStdStrLen(const char *src);						// String Length
int MStdStrCmp(const char *str1,const char *str2);		// String Compare
int MStdStrICmp(const char *str1,const char *str2);		// String Compare
bool MStdStrCat(char *dst,int dstsize,const char *src);	// String Catenate
bool MStdStrBegins(const char *str,const char *prefix);	// Check if string begins with prefix
bool MStdStrIBegins(const char *str,const char *prefix);	// Check if string begins with prefix
bool MStdStrEnds(const char *str,const char *suffix);	// Check if string ends with suffix
bool MStdStrIEnds(const char *str,const char *suffix);	// Check if string ends with suffix
bool MStdIsSubStr(const char *str,const char *substring);	// Check if substring
bool MStdIsISubStr(const char *str,const char *substring);	// Check if insensitive substring
bool MStdStrToLower(char *str);							// Convert string to lower
bool MStdStrToUpper(char *str);							// Convert string to lower
bool MStdStrRemove(char *target,int pos,int length);	// Remove chars in string
bool MStdStrInsert(char *target,int maxtargetsize,int pos,const char *insert);	// insert string at position
int MStdStrCharCount(const char *str,char ch);			// Find Count of chars in string
bool MStdStrTrim(char *str);							// Trim All Spaces in string
bool MStdStrTrimRight(char *str);						// Trim spaces on Right
bool MStdStrTrimLeft(char *str);						// Trim spaces on Left


/////////////////////////////////////////////////
// Wide String Operations
int MStdSPrintf(wchar_t *target,int targetsize,const wchar_t *format,...);	// String Formated Printf
const wchar_t *MStdStrWide(bool value);								// Build a temp string
const wchar_t *MStdStrWide(int value);								// Build a temp string
const wchar_t *MStdStrWide(float value);							// Build a temp string
const wchar_t *MStdStrWide(double value);							// Build a temp string
bool MStdStrCpy(wchar_t *dst,const wchar_t *src);					// String Copy
bool MStdStrCpy(wchar_t *dst,int dstsize,const wchar_t *src);		// String Copy
int MStdStrLen(const wchar_t *src);									// String Length
int MStdStrCmp(const wchar_t *str1,const wchar_t *str2);			// String Compare
int MStdStrICmp(const wchar_t *str1,const wchar_t *str2);			// String Compare
bool MStdStrCat(wchar_t *dst,int dstsize,const wchar_t *src);		// String Catenate
bool MStdStrBegins(const wchar_t *str,const wchar_t *prefix);		// Check if string begins with prefix
bool MStdStrIBegins(const wchar_t *str,const wchar_t *prefix);		// Check if string begins with prefix
bool MStdStrEnds(const wchar_t *str,const wchar_t *suffix);			// Check if string ends with suffix
bool MStdStrIEnds(const wchar_t *str,const wchar_t *suffix);		// Check if string ends with suffix
bool MStdIsSubStr(const wchar_t *str,const wchar_t *substring);		// Check if substring
bool MStdIsISubStr(const wchar_t *str,const wchar_t *substring);	// Check if insensitive substring
bool MStdStrToLower(wchar_t *str);						// Convert string to lower
bool MStdStrToUpper(wchar_t *str);						// Convert string to lower
bool MStdStrRemove(wchar_t *target,int pos,int length);	// Remove chars in string
bool MStdStrInsert(wchar_t *target,int maxtargetsize,int pos,const wchar_t *insert);	// insert string at position
int MStdStrCharCount(const wchar_t *str,wchar_t ch);	// Find Count of chars in string
bool MStdStrTrim(wchar_t *str);							// Trim All Spaces in string
bool MStdStrTrimRight(wchar_t *str);					// Trim spaces on Right
bool MStdStrTrimLeft(wchar_t *str);						// Trim spaces on Left
bool MStdStrClean(char *buffer);						// Clean out the string of non printable characters. \r \n are not printable
bool MStdStrClean(wchar_t *buffer);						// Clean out the string of non printable characters. \r \n are not printable

//////////////////////////////////////////////////
// Extra Functions
bool MStdWindowOutput(const char *title,const char *info);	// Dialog Box if availible
bool MStdSleep(int ms);									// Sleep for some ms
bool MStdExit(int value);								// Exit application

//////////////////////////////////////////////////
// Char Functions
bool MStdIsPrintable(char ch);							// =true if ch is printable(32-126). \r or \n are not printable
bool MStdIsSpace(char ch);								// =true if space
bool MStdIsDigit(char ch);
bool MStdIsAlpha(char ch);
bool MStdIsAlphaNumeric(char ch);
bool MStdIsUpper(char ch);
bool MStdIsLower(char ch);
bool MStdIsBinary(char ch);
bool MStdIsOctal(char ch);
bool MStdIsHex(char ch);
char MStdToBinary(int value);							// Value is 0 or 1
char MStdToOctal(int value);							// Value is 0-7
char MStdToHex(int value);								// Value is 0 or 15
int MStdFromBinary(char ch);
int MStdFromOctal(char ch);			
int MStdFromHex(char ch);			
char MStdToLower(char ch);
char MStdToUpper(char ch);
bool MStdIsNan(double val);								// Check if float is Not A Number

////////////////////////////////////////////////////
// Wide Char Functions
bool MStdIsSpace(wchar_t ch);
bool MStdIsDigit(wchar_t ch);
bool MStdIsAlpha(wchar_t ch);
bool MStdIsAlphaNumeric(wchar_t ch);
bool MStdIsUpper(wchar_t ch);
bool MStdIsLower(wchar_t ch);
wchar_t MStdToLower(wchar_t ch);
wchar_t MStdToUpper(wchar_t ch);

//////////////////////////////////////////////////
// Memory Functions
bool MStdMemCpy(void *target,const void *src,int count);		// Copy block of memory
bool MStdMemSet(void *,int value,int size);				// Set Buffer with zero
bool MStdMemZero(void *,int size);						// Set buffer to zero
int MStdMemCmp(const void *src,const void *dst,int size);		// Compare two memory blocks
bool MStdMemMove(void *dst,void *src,int count);		// Move Memory
void *MStdMalloc(int size);								// Allocate block of memory
bool MStdFree(void *memblock);							// Free Block of memory
void *MStdReAlloc(void *memblock,int newsize);			// Realloc memory with different size

//////////////////////////////////////////////////
bool MStdKbHit(void);									// Check if keyboard clicked
int MStdGetCh(bool forcestdio=false);					// Get Entered char

//////////////////////////////////////////////////
time_t MStdGetTime(void);								// Get second count from epoch 1970
bool MStdCTime(char *buf,int bufsize,time_t *tm);		// Convert time into a string
bool MStdAtExit(void fun(void));						// Exit Function
bool MStdFileRemove(const char *filename);				// Remove a file
bool MStdFileRename(const char *filesrc,const char *filetarget);	// Rename a file
bool MStdFileExists(const char *filename);				// Check if file exists
bool MStdExec(const char *cmd);							// Execute Command

///////////////////////////////////////////////////
// Conversion Functions
int MStdAToI(const char *str);							// Convert string to integer
int MStdAToI(const wchar_t *str);						// Convert string to integer
bool MStdIToA(int inval,char *buffer,int length);		// Convert an integer to a string
bool MStdIToA(int inval,wchar_t *buffer,int length);	// Convert an integer to a string
unsigned int MStdHToI(const char *str);					// Convert hex string  to integer
unsigned int MStdHToI(const wchar_t *str);				// Convert hex string to integer
double MStdAToF(const char *str);						// Convert string to double
double MStdAToF(const wchar_t *str);					// Convert string to double
void *MStdIToP(int val);								// Convert integer to pointer
int MStdPToI(void *val);								// Convert pointer to integer

int MStdGetMax(int value1,int value2);					// Get Max of two values
float MStdGetMax(float value1,float value2);			// Get Max of two values
double MStdGetMax(double value1,double value2);			// Get Max of two values
int MStdGetMin(int value1,int value2);					// Get Min of two values
float MStdGetMin(float value1,float value2);			// Get Min of two values
double MStdGetMin(double value1,double value2);			// Get Min of two values
int MStdGetAbs(int value);								// Get Absolute Value
float MStdGetAbs(float value);							// Get Absolute Value
double MStdGetAbs(double value);						// Get Absolute Value


//////////////////////////////////////////////////
// Random Number Generation
void MStdSRand(void);									// Seed based on time
void MStdSRand(int seed);								// Seed
int MStdRand(int range=32767);							// Weak Random Number

//////////////////////////////////////////////////
// Compare Two Numbers
bool MStdCompare(int val1,int val2,int error);			// check |val2-val1|<=error
bool MStdCompare(unsigned int val1,unsigned int val2,unsigned int error);	// check |val2-val1|<=error
bool MStdCompare(float val1,float val2,float error);	// check |val2-val1|<=error
bool MStdCompare(double val1,double val2,double error);	// check |val2-val1|<=error

//////////////////////////////////////////////////
// Time Functions
unsigned int MStdGetTimeOfDay(void);


//////////////////////////////////////////////////
// Log File
bool MStdLog(const char *logentry,const char *file,int lineno);

//////////////////////////////////////////////////
// Power Function
inline double MStdPower(const double &base,const double &exp)
	{  return pow(base,exp);  }

//////////////////////////////////////////////////
// sqrt function
inline double MStdSqrt(const double &val)
	{  return sqrt(val);  }

//////////////////////////////////////////////////
// Absolute functions
inline int MStdAbs(int val)
	{
	if(val>=0) { return val; }
	return -val;
	}


//////////////////////////////////////////////////
inline float MStdAbs(const float &val)
	{
	if(val>=0) { return val; }
	return -val;
	}


//////////////////////////////////////////////////
inline double MStdAbs(const double &val)
	{
	if(val>=0) { return val; }
	return -val;
	}


//////////////////////////////////////////////////
// Some Well Known Constants
extern const double MStdLibConstPi;

//////////////////////////////////////////////////
// Platform Machine Name
bool MStdGetMachineName(char *buffer,int bufferlen);


#endif // MStdLib_h

