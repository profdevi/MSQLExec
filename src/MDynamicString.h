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


//v1.9 copyright Comine.com 20150813R1125
#ifndef MDynamicString_h
#define MDynamicString_h

////////////////////////////////////////////////
#include "MDynamicArray.h"

///////////////////////////////////////////////
class MDynamicString
	{
	MDynamicArray mString;
	
	///////////////////////////////////////////////
	void ClearObject(void);

	///////////////////////////////////////////////
	public:
	MDynamicString(void);
	MDynamicString(const char *string,int blocksize=32);
	~MDynamicString(void);
	bool Create(int blocksize=32);
	bool Create(const char *string,int blocksize=32);
	bool Destroy(void);
	const char *Get(void);
	char Get(int index);
	bool Set(int index,char ch);
	bool Set(const char *string);
	bool Remove(int index);
	bool Insert(int index,char ch);
	bool Insert(char ch);
	bool Clear(void);
	int GetLength(void);
	bool Append(const char *str);
	bool TrimLeft(void);
	bool TrimRight(void);
	bool Trim(void);
	bool IsBlank(void);
	};

#endif // MDynamicString_h

