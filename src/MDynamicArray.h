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


//v1.10 copyright Comine.com 20130202S0022
#ifndef MDynamicArray_h
#define MDynamicArray_h

/////////////////////////////////////////////////////////
class MDynamicArray
	{
	int mItemSize;	char *mItemArray;  int mItemCount; 
	int mDataBlockSize; int mMaxItemCount;

	/////////////////////////////////////////////////////
	void ClearObject(void);
	int GetBlockCountToHoldItems(int itemcount);	//=total blocks to hold itemcount
	int GetItemCountInBlocks(int blockcount);		//=max # of items in blocks

	/////////////////////////////////////////////////////
	public:
	MDynamicArray(void);
	MDynamicArray(int itemsize,int initialcapacity=1,int datablocksize=512);
	~MDynamicArray(void);
	bool Create(int itemsize,int initialcapacity=1,int datablocksize=512);
	bool Destroy(void);
	bool Insert(void *item);					// Insert at End of list
	bool Insert(int index,void *item);			// Insert at position
	bool Remove(int index);						// Remove at index
	void *Get(int index);						// Return memory address of object
	bool Set(int index,void *item);				// Set an item
	int GetCount(void);
	int GetMaxCapacity(void);
	bool Clear(void);							// reset set the total count of items
	bool Resize(int maxitems);					// Reset to maxitems. If Smaller, memory is released
	int GetItemSize(void);						// Return Item Count
	};


#endif // MDynamicArray_h

