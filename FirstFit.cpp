/*
 *   
 *   File Name: FirstFit.cpp
 *        Name: Joshua Smith
 *      Course: CST 238
 *        Term: Fall 2014
 *  Assignment: Project 2
 *    Abstract: FirstFit Base Class
 *
 */

#include "FirstFit.h"
#include <iostream>
#include <algorithm>

using namespace std;
// Allocate size number of bytes
// Returns a pointer to the newly
// allocated chunk of memory
//
// Invaraint: newly allocated memory
//            must be zero'ed out
//
//    Errors: requested size is larger
//            than the greatest chunk
void *FirstFit::allocate(size_t size) {
  
	byte * copy;
	double totalChunksNeeded = 0;
	auto it = freeList.begin();
	auto it2 = it;
	bool flag = false;
	
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
		
	if (totalChunksNeeded == 1)
	{	
		copy = it->start;
						
		advance(it2, totalChunksNeeded);
						
		allocatedMap[it->start] = size;
						
		freeList.erase(it, it2);
					
		return copy;
	}

		for (size_t i = 0; i < chunks.size(); i++)
		{
			if (chunks[i] >= size)
			{			
				for(list<FreeBlock>::const_iterator it = freeList.begin(); it != freeList.end(); it++)
	            {
					
					if(chunkPair[i].first == it->start)
					{
						auto it2 = it;
						
						copy = it->start;
						
						advance(it2, totalChunksNeeded);
						
						allocatedMap[it->start] = size;
						
						freeList.erase(it, it2);
						
						return copy;
					}
				}
			}
		}
		return NULL;
}