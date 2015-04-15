/*
 *
 *   File Name: MemoryManager.cpp
 *        Name: Joshua Smith
 *      Course: CST 238
 *        Term: Fall 2014
 *  Assignment: Project 2
 *    Abstract: MemoryManager Super Class
 *
 */

#include <list>
#include <cmath> // pow
#include <cstring> // memset
#include <exception>
#include <iostream>
#include <vector>
#include <iterator>
#include "memoryManager.h"
#include <algorithm>
#include <utility>

using std::exception;
using std::cout;
using std::ostream;
using std::vector;
using std::list;
using namespace std;

// Utility function used by the constructor
unsigned int bytes(size_t num, memory_unit_t unit) {
  return (unsigned int) pow(2.0, (double) unit);
}

/*
 *
 *   MemoryManager Method Definitions
 *
 */
// Constructor: initializes the freeList
MemoryManager::MemoryManager(size_t total_bytes, size_t block_size) {
  // Check for the default argument
  if (!total_bytes) {
    block_size = 64;
    MEMORY_SIZE = bytes(16, MB);
  } else {
    // Must be a multiple of the block size
    if (total_bytes % block_size != 0) { 
      total_bytes += (total_bytes % block_size);
    }
    MEMORY_SIZE = total_bytes;
  }

  // Allocate memory and zero it out
  memory = new byte[MEMORY_SIZE];
  memset(memory, 0, MEMORY_SIZE);

  // Store the block size
  BLOCK_SIZE = block_size;

  // Add all FreeBlocks to the freeList
  for (unsigned i = 0; i < MEMORY_SIZE; i += BLOCK_SIZE) {
    freeList.push_back(FreeBlock(memory + i, 
                                 memory + i + BLOCK_SIZE - 1));
  }
}

// A chunk is a contiguous sequence of free blocks
// Stores a list of contiguous chunk sizes in
// the vector passed in
// You can assume the vector passed in is empty
void MemoryManager::chunkSizes(vector<size_t> &vcs) const {

	size_t i = 0;

	unsigned chunkSize = BLOCK_SIZE;

	for (list<FreeBlock>::const_iterator it = freeList.begin(); it != freeList.end(); ++it)
	{

		if (it->end == (next(it))->start - 1)
		{
			chunkSize += (BLOCK_SIZE);
		}
		else
		{
			if(chunkSize != BLOCK_SIZE)
			{
				vcs.push_back(chunkSize);
			}

			chunkSize = BLOCK_SIZE;
		}
	}
}

// Stores starting and ending addresses
// of all free chunks in memory as pairs
void MemoryManager::chunkSizes(vector<pair<const byte *, const byte *> > & vca) const {
//cout << ff overload
//use for best fit
//firstfit first perameter is total mem second is block size

	byte *begin = freeList.front().start;
	byte *end = freeList.front().end;
	bool flag = false;

	for(list<FreeBlock>::const_iterator it = freeList.begin(); it != freeList.end(); it++)
	{

		if(it->end + 1 == next(it)->start)
		{
			flag = true;

			end = next(it)->end;
		}
		else
		{
			if (flag == true)
			{
				vca.push_back(make_pair(begin, end));
			}

			begin = it->start;

			flag = false;
		}
	}
}

// overload for differents vector
void MemoryManager::chunkSizes(vector<const byte *> & vca) const {

	byte *begin = freeList.front().start;
	byte *end = freeList.front().end;
	bool flag = false;

	for(list<FreeBlock>::const_iterator it = freeList.begin(); it != freeList.end(); it++)
	{

		if(it->end + 1 == next(it)->start)
		{
			flag = true;

			end = next(it)->end;
		}
		else
		{
				vca.push_back(begin);
			
			begin = it->start;

			flag = false;
		}
	}
}

// overload for differents vector
void MemoryManager::chunkSizes(vector<unsigned long> &vcs) const {

	size_t i = 0;

	unsigned chunkSize = BLOCK_SIZE;

	for (list<FreeBlock>::const_iterator it = freeList.begin(); it != freeList.end(); ++it)
	{

		if (it->end == (next(it))->start - 1)
		{
			chunkSize += (BLOCK_SIZE);
		}
		else
		{
				vcs.push_back(chunkSize);
		}
	}
}
// Returns the address of the first free block
// Or NULL if all of memory has been allocated
byte *MemoryManager::firstFreeBlock() const {
	// ask in office hours use davids code
	
	if (freeList.empty())
	{
		return NULL;
	}
	else
	{
		freeList.front().start;
	}
	
}
// Free previously allocated memory
//
// Invariant: freed memory must be zero'd out
//
// Errors: trying to free memory not currently
// allocated
// trying to free memory not in the range
// [memory start, memory start + MEMORY_SIZE)
void MemoryManager::free(void *ptr) {

	if (allocatedMap.count((byte*)ptr) == 0)
	{
		throw freeException;
	}

	size_t size = allocatedMap[(byte*)ptr];

	allocatedMap.erase((byte*)ptr);

	memset((byte*)ptr, 0, size);

	for (size_t i = 0; i < size; i += BLOCK_SIZE) {
    freeList.push_back(FreeBlock((byte*)ptr + i, 
                                 (byte*)ptr + i + BLOCK_SIZE - 1));
	
	sortFreeList();

	}
}

// Returns the size in bytes of the largest chunk available
// A chunk is defined as >= 2 contiguous blocks
// Return 0 if only single blocks are available
size_t MemoryManager::largestChunkAvailable() const {

	vector<size_t> chunks;

	chunkSizes(chunks);

	if(chunks.empty())
	{
		return 0;
	}

	sort(chunks.begin(), chunks.end());

	return chunks.back();
}
// Returns the number of bytes available for allocation
size_t MemoryManager::memoryAvailable() const {

	unsigned freeMemory = 0;
	
	freeMemory = freeList.size() * BLOCK_SIZE;

	return freeMemory;
}
// Returns the number of allocated blocks
unsigned MemoryManager::numAllocatedBlocks() const {

	unsigned totalBlocks = 0;
	unsigned totalBlocksAlocated = 0;

	totalBlocks = MEMORY_SIZE / BLOCK_SIZE;

	totalBlocksAlocated = totalBlocks - freeList.size();

	return totalBlocksAlocated;
}
// Returns the number of free blocks
unsigned MemoryManager::numFreeBlocks() const {
	
	return freeList.size();
}
// Returns in the size in bytes of the smallest chunk available
// A chunk is defined as >= 2 contiguous blocks
// Return 0 if only individual blocks are available
size_t MemoryManager::smallestChunkAvailable() const {

	vector<size_t> chunks;

	chunkSizes(chunks);

	if(chunks.empty())
	{
		return 0;
	}

	sort(chunks.begin(), chunks.end());

	return chunks.front();
}

ostream& operator<<(ostream& os, const MemoryManager& mm) {
	os << "[Free Blocks]: "
		<< mm.numFreeBlocks()
		<< " | [Allocated Blocks]: "
		<< mm.numAllocatedBlocks()
		<< " | [Available Memory]: "
		<< mm.memoryAvailable()
		<< endl
		<< " | [Smallest chunk available]: "
		<< mm.smallestChunkAvailable()
		<< "\n";
	return os;
}