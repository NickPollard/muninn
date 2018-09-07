#pragma once

class PassthroughAllocator {
  heapAllocator* heap;
  size_t totalBytesAllocated;
  size_t allocations;

  // Create a new passthrough allocator using the given HEAP
  PassthroughAllocator( heapAllocator* heap );

  // Allocate `size` memory through the passthroughAllocator P
  // (The allocation is actually in the underlying heap)
  void* allocate( size_t size, const char* source );

  // Deallocate allocation `mem` from passthroughAllocator P
  // (The allocation is actually in the underlying heap)
  void deallocate( void* mem );
};


