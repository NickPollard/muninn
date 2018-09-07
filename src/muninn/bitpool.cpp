#include "bitpool.h"
#include <inttypes.h>

static const void* kNoFreeBlock = (void*)-1;

// Initialize the free list used to keep track of what space we can use in the bitpool
// Each bitblock points to the next one as free; Last has kNoFreeBlock
void BitPool::initFree() {
	uint8_t * ptr = arena;
	uint8_t const *const end = end();
	while (ptr != end) {
		uint8_t* next = ptr + bytesPerBlock;
		(*(void**)ptr) = (void*)next;
		ptr = next;
	}
	// Init the last to invalid
	void** entry = (void**)(end - bytesPerBlock);
	*entry = kNoFreeBlock;
}

// Create a new bitpool
BitPool::BitPool( size_t blockSize, size_t count, void* arena ) : 
	bytesPerBlock( blockSize ),
	blockCount( count ),
	arena( arena ),
	firstFree( arena )
{
	vAssert( arena != nullptr );          // Arena must not be null
  vAssert( arena % bytesPerBlock == 0); // Enforce that we are aligned by at least the size of our entries
	vAssert( blockSize > sizeof( void* )) // Cannot have BitPools that cannot store a pointer (used for free list)
	initFree();
}

void* BitPool::allocate( size_t size ) {
	//printf( "Allocating from %d-byte bitpool.\n", (int)bytesPerBlock );
	vAssert( size <= bytesPerBlock );
	void* entry = firstFree;
	if ( entry != kNoFreeBlock ) {
		firstFree = *(void**)firstFree;
		return entry;
	}
	else {
		return NULL;
	}
}

void BitPool::free( void* entry ) {
	// So we make entry point to firstFree, then make firstFree point to entry
	// ie. prepend on to the freelist
	if contains( entry ) {
		*(void**)entry = firstFree;
		firstFree = entry;
	}
}

// Returns a pointer to the byte first-past-the-end
uint8_t const* BitPool::end() {
	return arena + (bytesPerBlock * blockCount);
}

bool BitPool::contains( void* data ) {
	const auto address = (uint8_t const *const)data;
	return ( address != nullptr &&
			address >= arena &&
			address < end() &&
			( address % bytesPerBlock == 0 )); 
}
