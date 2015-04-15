/*
 *
 *   File Name: MemoryManager.h
 *        Name: Joshua Smith
 *      Course: CST 238
 *        Term: Fall 2014
 *  Assignment: Project 2
 *    Abstract: MemoryManager Super Class
 *
 */
#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <exception>
#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <map>

using std::map;
using std::exception;
using std::list;
using std::vector;
using std::ostream;
using std::pair;

typedef unsigned char byte;
enum memory_unit_t { KB = 10, MB = 20, GB = 30 };

unsigned int bytes(size_t, memory_unit_t);

class MemoryManager {
  protected:
    byte *memory; // Pointer to the start of memory
    size_t MEMORY_SIZE;
    size_t BLOCK_SIZE;

	map<byte*, size_t> allocatedMap;
	map<size_t, const byte*> keySizeAddress;
	
    // A FreeBlock has its start and
    // end address along with a pointer
    // to the next FreeBlock available
    struct FreeBlock {
      // Invariant: (end - start + 1) % BLOCK_SIZE == 0
      // The range is: [start, end] (inclusive of both end points)
      byte *start;
      byte *end;

      // Can construct without arguments
      // Or with start and end pointers
      FreeBlock() {}
      FreeBlock(byte *s, byte *e) : start(s), end(e) {}
    };

    // Store a linked list of FreeBlock structs
    // Invariant: all FreeBlocks are sorted in
    // ascending order based on start address
    list<FreeBlock> freeList;

    // The FreeList must maintain sorted order
    static bool blockCmp(const FreeBlock & lhs, const FreeBlock & rhs) {
      return lhs.start < rhs.start;
    }
    void sortFreeList() { freeList.sort(MemoryManager::blockCmp); }
	bool block (const int& value) { return (value == BLOCK_SIZE); }
  public:
    // Constructor: initializes the freeList
    MemoryManager(size_t total_bytes = 0, size_t block_size = 64); 

    // Destructor
    ~MemoryManager() { delete [] memory; }

    /*
     *
     *       PUBLIC INTERFACE
     *
     */
    // Must override in base class
    // Allocate size number of bytes
    // Returns a pointer to the newly
    // allocated chunk of memory
    //
    // Invaraint: newly allocated memory
    //            must be zero'ed out
    //
    //    Errors: requested size is larger
    //            than the greatest chunk
    virtual void *allocate(size_t size) = 0;    
    
    // Free previously allocated memory
    //
    // Invariant: freed memory must be zero'd out
    //
    //    Errors: trying to free memory not currently
    //            allocated
    //            trying to free memory not in the range
    //            [memory start, memory start + MEMORY_SIZE)
    void free(void *ptr);

    // A chunk is a contiguous sequence of free blocks
    // Stores all contiguous chunk sizes in
    // the vector passed in as size_t's
    // All chunks must consist of free memory
    // You can assume the vector passed in is empty
    void chunkSizes(vector<size_t> &) const;
	
    // Stores starting and ending addresses
    // of all free chunks in memory as pairs
    void chunkSizes(vector<pair<const byte *, const byte *> > &) const;

	void chunkSizes(vector<const byte *> &) const;
	
	void chunkSizes(vector<unsigned long> &) const;
    // Returns the address of the first free block
    // Or NULL if all of memory has been allocated
    byte *firstFreeBlock() const;

    // Returns the size in bytes of the largest chunk available
    // A chunk is defined as >= 2 contiguous blocks
    // Return 0 if only single blocks are available
    size_t largestChunkAvailable() const;

    // Returns the number of bytes available for allocation
    size_t memoryAvailable() const;

    // Returns the number of allocated blocks
    unsigned numAllocatedBlocks() const;

    // Returns the number of free blocks
    unsigned numFreeBlocks() const;

    // Returns in the size in bytes of the smallest chunk available
    // A chunk is defined as >= 2 contiguous blocks
    // Return 0 if only individual blocks are available
    size_t smallestChunkAvailable() const;
	
    // Allows for the printing of a MemoryManager object
    friend ostream& operator<<(ostream& os, const MemoryManager& mm);
};

// Exceptions which must be thrown by allocate()
// and free() when an exception occurs in either
class MemoryAllocationException : public exception {
  virtual const char* what() const throw() {
    return "Not enough memory for allocation";
  }
} const allocationException;

class MemoryFreeException : public exception {
  virtual const char* what() const throw() {
    return "Cannot free unallocated memory";
  }
} const freeException;

#endif