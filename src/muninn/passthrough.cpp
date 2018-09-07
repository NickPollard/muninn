#include "passthrough.h"
#include "allocator.h"

void* PassthroughAllocator::allocate( size_t size, const char* source ) {
	const size_t before = heap->allocatedBytes;
	void* mem = heap_allocate( heap, size, source );
	const size_t after = heap->allocatedBytes;
	allocatedBytes += (after - before);	
	++allocations;
	return mem;
}
	
void PassthroughAllocator::deallocate( void* mem ) {
	const size_t before = heap->allocatedBytes;
	heap_deallocate( heap, mem );
	const size_t after = heap->allocatedBytes;
	allocatedBytes += (after - before);
	--allocations;
}
