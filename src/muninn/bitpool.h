#pragma once

#include <cstddef.h>
#include <cstdint.h>

class BitPool {
  public:
    void* allocate( size_t size );

    void free( void* data );

    bool contains( void* data );

    BitPool( size_t size, size_t count, void* arena );

  private:
    size_t bytesPerBlock;
    size_t blockCount;
    uint8_t* arena; // Pointer to a memory arena of size equal to ( bytesPerBlock * blockCount )
    void* firstFree;

    void initFree();
};
