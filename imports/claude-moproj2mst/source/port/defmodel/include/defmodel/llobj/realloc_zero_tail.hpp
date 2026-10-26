// defmodel/llobj/realloc_zero_tail.hpp: the realloc of the LLOBJ set resizes (140180150, 140183770)
// with the grown part zeroed.
//
// Port decision (fixed in claude/p67-5-fix). The binary calls the VS2008 CRT realloc (140483e80):
// HeapReAlloc(_crtheap, 0, p, n), no HEAP_ZERO_MEMORY, so the bytes past the old block size are
// whatever the heap holds there. Nothing on the path clears them (1401d0fc0 -> 1401d0320 ->
// 140199650 -> 140180150 / 140183770 -> 140329e60). In the original's runs the moved block comes
// from memory the CRT heap has not handed out before and reads as zero (GEOTYP 3 after KFREAD
// widens BCCDEF from 2 to 3 per node: the rows past old n*2 ints are written as 0 in OUT.KEY);
// in the port the same block is reused heap that still holds freed KEY text. Zeroing the grown
// tail makes the port deterministic and equal to what the original produces.
#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace def::model::llobj {

template <class T>
T* realloc_zero_tail(T* p, std::size_t new_bytes)
{
    const std::size_t old_bytes = p != nullptr ? _msize(p) : 0;
    void* q = std::realloc(p, new_bytes);
    if (q != nullptr && old_bytes < new_bytes)
        std::memset(static_cast<std::byte*>(q) + old_bytes, 0, new_bytes - old_bytes);
    return static_cast<T*>(q);
}

} // namespace def::model::llobj
