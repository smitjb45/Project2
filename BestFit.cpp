/*
*
* File Name: BestFit.cpp
* Name:Joshua Smith
* Course: CST 238
* Term: Fall 2014
* Assignment: Project 2
* Abstract: BestFit Base Class
*
*/

#include <iostream>
#include "BestFit.h"
#include <math.h>
#include <algorithm>

using namespace std;

// Allocate size number of bytes
// Returns a pointer to the newly
// allocated chunk of memory
//
// Invaraint: newly allocated memory
// must be zero'ed out
//
// Errors: requested size is larger
// than the greatest chunk

void *BestFit::allocate(size_t size) {
	
	byte * copy;
	double totalChunksNeeded = 0;
	auto it = freeList.begin();
	auto it2 = it;
	bool flag = false;
	const byte * addressKey;
	chunkSizes(chunks);
	chunkSizes(chunkPair);
	
	totalChunksNeeded = size / BLOCK_SIZE;
	
	for(unsigned i = 0; i < chunks.size(); i++)
	{
			if(chunks[i] >= size && size > 0)
			{
				flag = true;
			}
	}

	if(flag == false)
	{
		throw allocationException;
	}

	if(size % BLOCK_SIZE > 0)
	{
		totalChunksNeeded++;
	}
	
	for(unsigned i = 0; i < chunks.size(); i++)
	{
		keySizeAddress[chunks[i]] = chunkPair[i];
	}

	for(unsigned i = 0; i < chunks.size(); i++)
	{
		keyAddressSize[chunkPair[i]] = chunks[i];
	}
	

	sort(chunks.begin(), chunks.end());

		for (size_t i = 0; i < chunks.size(); i++)
		{
			if (chunks[i] >= size)
			{			
				addressKey = keySizeAddress[chunks[i]];

				for(list<FreeBlock>::const_iterator it = freeList.begin(); it != freeList.end(); it++)
	            {
					if(addressKey == it->start)
					{
						auto it2 = it;
						
						advance(it2, totalChunksNeeded);
				
						allocatedMap[it->start] = size;
						
						copy = it->start;
						
						freeList.erase(it, it2);
						
						return copy;
					}
				}
			}
		}
	return NULL;
}