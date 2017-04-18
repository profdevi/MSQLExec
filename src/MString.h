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


//v3.2 copyright Comine.com 20170409U0957
#ifndef MString_h
#define MString_h

////////////////////////////////////////////////
#include "TVector.h"
#include "MStdLib.h"
#include "MIReader.h"
#include "MIWriter.h"

////////////////////////////////////////////////
class MStringWide;  // forward declaration

///////////////////////////////////////////////
class MString
	{
	TVector<char> mString;				// String Storage
	int mLength;						// Holds the length of the string

	/////////////////////////////////////////////
	void ClearObject(void);

	/////////////////////////////////////////////
	public:
	MString(void);
	MString(const char *str);
	MString(const wchar_t *str);
	MString(const MString &str);				// Copy Constructor
	MString(MStringWide &str);
	~MString(void);
	bool Create(const char *str);
	bool Create(const wchar_t *str);
	bool Create(const MString &str);
	bool Create(MStringWide &str);
	bool Create(char ch);
	bool Create(wchar_t ch);
	bool Create(MIReader &reader);
	bool Destroy(void);

	// Operators
	bool operator=(const char *str);
	bool operator=(const wchar_t *str);
	bool operator=(MString &src);
	MString operator +(const char *str);		// return a temp object
	MString operator +(MString &ref);			// return a temp object
	MString operator +(MStringWide &ref);		// return a temp object
	char &operator[](int index);
	const char *Get(void);
	const char *GetEnd(int count);				// Get End String
	bool ToUpper(void);
	bool ToLower(void);
	bool Trim(void);							// Trim Spaces
	bool TrimLeft(void);						// Trim Spaces on the Left
	bool TrimRight(void);						// Trim Spaces on the Right
	bool TrimRight(int length);					// Trim Spaces on the Right
	bool Append(const char *str);
	bool Append(const wchar_t *str);
	bool Append(char ch);
	bool Append(wchar_t ch);
	bool Append(int val);
	bool Append(float val);
	bool Append(double val);
	bool Append(bool val);
	int Length(void);
	int Compare(const char *string);
	int Compare(const wchar_t *string);
	int Compare(MString &string);
	int Compare(MStringWide &string);
	bool Write(MIWriter &writer) const;
	bool Zero(void);										// zero out the memory

	/////////////// Class Methods ////////////////////
	// Basic String Copy
	static bool Copy(char *dest,const char *src);
	static bool Copy(char *dest,const wchar_t *src);
	static bool Copy(wchar_t *dest,const char *src);
	static bool Copy(wchar_t *dest,const wchar_t *src);
	static bool Copy(char *dest,const char *src,int maxcopy);
	static bool Copy(char *dest,const wchar_t *src,int maxcopy);
	static bool Copy(wchar_t *dest,const char *src,int maxcopy);
	static bool Copy(wchar_t *dest,const wchar_t *src,int maxcopy);


	// Basic String Length
	static int Length(const char *str);
	static int Length(const wchar_t *str);

	// Basic String Compare
	static int Compare(const char *src,const char *dst);
	static int Compare(const wchar_t *src,const char *dst);
	static int Compare(const char *src,const wchar_t *dst);
	static int Compare(const wchar_t *src,const wchar_t *dst);

	// Basic String CaseInsensitive Compare
	static int ICompare(const char *src,const char *dst);
	static int ICompare(const wchar_t *src,const char *dst);
	static int ICompare(const char *src,const wchar_t *dst);
	static int ICompare(const wchar_t *src,const wchar_t *dst);


	// Basic Char Operations
	static bool IsSpace(char ch);  // for ' ',/r/n/t
	static bool IsSpace(wchar_t ch);
	static char ToUpper(char ch);
	static char ToLower(char ch);
	static wchar_t ToUpper(wchar_t ch);
	static wchar_t ToLower(wchar_t ch);

	// String Conversions
	static bool ToLower(char *str);
	static bool ToLower(wchar_t *str);
	static bool ToUpper(char *str);
	static bool ToUpper(wchar_t *str);
	static bool Trim(char *str);  // Remove All spaces
	static bool Trim(wchar_t *str);  // Remove All spaces
	static bool TrimLeft(char *str);
	static bool TrimRight(char *str);
	static bool TrimLeft(wchar_t *str);
	static bool TrimRight(wchar_t *str);

	// Append Operations
	static bool Append(char *str,const char *src);
	static bool Append(char *str,const wchar_t *src);
	static bool Append(wchar_t *str,const char *src);
	static bool Append(wchar_t *str,const wchar_t *src);

	// Dynamic Memory Allocation
	static char *Alloc(const char *st,const char *file,int lineno);
	static char *Alloc(const char *st,const char *st2,const char *file,int lineno);
	};



////////////////////////////////////////////////
class MStringWide
	{
	TVector<wchar_t> mString;	// Points to a copy of a string
	int mLength;				// Holds the string length

	/////////////////////////////////////////////
	void ClearObject(void);

	/////////////////////////////////////////////
	public:
	MStringWide(void);
	MStringWide(const char *str);
	MStringWide(const wchar_t *str);
	MStringWide(MStringWide &str);
	MStringWide(MString &str);
	~MStringWide(void);
	bool Create(const char *str);
	bool Create(const wchar_t *str);
	bool Create(MString &str);
	bool Create(MStringWide &str);
	bool Create(char ch);
	bool Create(wchar_t ch);
	bool Create(MIReader &reader);
	bool Destroy(void);
	bool operator=(const char *str);
	bool operator=(const wchar_t *str);
	bool operator=(MStringWide &src);
	wchar_t &operator[](int index);
	const wchar_t *Get(void);
	const wchar_t *GetEnd(int count);			// Get End String
	bool ToUpper(void);
	bool ToLower(void);
	bool Trim(void);
	bool TrimLeft(void);
	bool TrimRight(void);
	bool TrimRight(int length);
	bool Append(const wchar_t *str);
	bool Append(const char *str);
	bool Append(char ch);
	bool Append(wchar_t ch);
	bool Append(int val);
	bool Append(float val);
	bool Append(double val);
	bool Append(bool val);
	int Length(void);
	int Compare(const char *string);
	int Compare(const wchar_t *string);
	int Compare(MString &string);
	int Compare(MStringWide &string);
	bool Write(MIWriter &writer) const;
	};


#endif // MString_h
