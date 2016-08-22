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
#include "MStdLib.h"
#include "MDynamicArray.h"
#include "MDynamicString.h"


//******************************************************
//** MDynamicString Code
//******************************************************
///////////////////////////////////////////////////////
void MDynamicString::ClearObject(void)
	{
	}


///////////////////////////////////////////////////////
MDynamicString::MDynamicString(void)
	{  ClearObject();  }


//////////////////////////////////////////////////////
MDynamicString::MDynamicString(const char *string,int blocksize)
	{
	ClearObject();
	if(Create(string,blocksize)==false)
		{
		return;
		}
	}

///////////////////////////////////////////////////////
MDynamicString::~MDynamicString(void)
	{  Destroy();  }


///////////////////////////////////////////////////////
bool MDynamicString::Create(int blocksize)
	{
	Destroy();
	if(mString.Create(1,1,blocksize)==false)
		{
		return false;
		}

	char ch=0;
	if(mString.Insert(&ch)==false)
		{
		mString.Destroy(); return false;
		}

	return true;
	}


////////////////////////////////////////////////////////
bool MDynamicString::Create(const char *string,int blocksize)
	{
	Destroy();

	if(blocksize<=0)
		{
		Destroy(); return false;
		}
	
	int len=MStdStrLen(string);
	if(mString.Create(1,len+1,blocksize)==false)
		{
		Destroy();  return false;
		}

	for(const char *p=string;*p!=0;++p)
		{  mString.Insert((void *)p);  }

	char endchar=0;
	if(mString.Insert(&endchar)==false)
		{
		Destroy();  return false;
		}

	return true;
	}

///////////////////////////////////////////////////////
bool MDynamicString::Destroy(void)
	{
	mString.Destroy();
	ClearObject();
	return true;
	}


///////////////////////////////////////////////////////
const char *MDynamicString::Get(void)
	{
	return (const char *)mString.Get(0);
	}


///////////////////////////////////////////////////////
char MDynamicString::Get(int index)
	{
	int count=mString.GetCount();
	if(index<0 || index>=count)
		{
		return 0;
		}

	return *(char *)mString.Get(index);
	}


///////////////////////////////////////////////////////
bool MDynamicString::Set(int index,char ch)
	{
	int count=mString.GetCount();
	if(count==0)
		{
		return false;
		}
	
	if(index<0 || index>=count)
		{
		return 0;
		}

	if(index==count-1)
		{
		if(mString.Insert(index,&ch)==false)
			{
			return false;
			}
		return true;
		}

	*(char *)mString.Get(index) = ch;
	return false;
	}


///////////////////////////////////////////////////////
bool MDynamicString::Set(const char *string)
	{
	mString.Clear();

	bool bflag=true;
	for(const char *p=string;*p!=0;++p)
		{
		if(mString.Insert((void*)p)==false) { bflag=false; }
		}
	char endchar=0;
	if(mString.Insert(&endchar)==false) { bflag=false; }

	if(bflag==false)
		{
		return false;
		}

	return true;
	}


///////////////////////////////////////////////////////
bool MDynamicString::Remove(int index)
	{
	int count=mString.GetCount();
	if(index<0 || index>=count-1)
		{
		return false;
		}
	
	if(mString.Remove(index)==false)
		{
		return false;
		}

	return true;
	}


///////////////////////////////////////////////////////
bool MDynamicString::Insert(int index,char ch)
	{

	int count=mString.GetCount();
	if(index<0 || index>=count)
		{
		return false;
		}

	if(mString.Insert(index,&ch)==false)
		{
		return false;  
		}

	return true;
	}


///////////////////////////////////////////////////////
bool MDynamicString::Insert(char ch)
	{
	int count=mString.GetCount();
	if(mString.Insert(count-1,&ch)==false)
		{
		return false;
		}

	return true;
	}


///////////////////////////////////////////////////////
bool MDynamicString::Clear(void)
	{
	mString.Clear();
	char endchar=0;
	if(mString.Insert(&endchar)==false)
		{
		return false;
		}

	return true;
	}


///////////////////////////////////////////////////////
int MDynamicString::GetLength(void)
	{
	return mString.GetCount()-1;
	}


///////////////////////////////////////////////////////
bool MDynamicString::Append(const char *str)
	{
	for(int i=0;str[i]!=0;++i)
		{
		if(Insert(str[i])==false)
			{
			return false;
			}
		}
	return true;
	}


////////////////////////////////////////////////////
bool MDynamicString::TrimRight(void)
	{
	for(int i=GetLength()-1;i>=0;--i)
		{
		char ch=Get(i);
		if(ch==' ' || ch=='\t' || ch=='\r' || ch=='\n')
			{  Remove(i);  continue; }
		break;
		}

	return true;
	}


////////////////////////////////////////////////////
bool MDynamicString::TrimLeft(void)
	{
	for(;;)
		{	char ch=Get(0);
		if(ch==' ' || ch=='\t' || ch=='\r' || ch=='\n')
			{  Remove(0);  continue; }

		break;
		}
	return true;
	}


//////////////////////////////////////////////////////
bool MDynamicString::Trim(void)
	{
	for(int i=GetLength()-1;i>=0;--i)
		{
		char ch=Get(i);
		if(ch==' ' || ch=='\t' || ch=='\r' || ch=='\n')
			{  Remove(i);  continue; }
		}

	return true;
	}


//////////////////////////////////////////////////////////////
bool MDynamicString::IsBlank(void)
	{
	for(int i=0;i<GetLength();++i)
		{
		char ch=Get(i);
		if(ch==' ' || ch=='\t' || ch=='\r' || ch=='\n') { continue; }
		return false;
		}
	return true;
	}



