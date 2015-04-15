/*
*
* File Name: BestFit.h
* Name: Joshua Smith
* Course: CST 238
* Term: Fall 2014
* Assignment: Project 2
* Abstract: BestFit Base Class
*
*/
#ifndef BEST_FIT_H
#define BEST_FIT_H

#include "memoryManager.h"

class BestFit : public MemoryManager {
public:

	BestFit(size_t total_bytes = 0, size_t block_size = 64) :
MemoryManager(total_bytes, block_size) { }
	
	// Allocate size number of bytes
	// Returns a pointer to the newly
	// allocated chunk of memory
	//
	// Invaraint: newly allocated memory
	// must be zero'ed out
	//
	// Errors: requested size is larger
	// than the greatest chunk
	void *allocate(size_t size);
private:
	map<const byte*, size_t> keyAddressSize;
	vector<unsigned long> chunks;
	vector<const byte *> chunkPair;
};
#endif