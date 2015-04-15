/*
*
* File Name: FirstFit.h
* Name: Joshua Smith
* Course: CST 238
* Term: Fall 2014
* Assignment: Project 2
* Abstract: FirstFit Base Class
*
*/
#ifndef FIRST_FIT_H
#define FIRST_FIT_H
#include <iostream>
#include "MemoryManager.h"

class FirstFit : public MemoryManager {
public:
// Constructor
FirstFit(size_t total_bytes = 0, size_t block_size = 64) :
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

	vector<size_t> chunks;
	vector<pair<const byte *, const byte *> > chunkPair;
};
#endif