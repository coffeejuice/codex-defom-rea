// GUI140854370 /140857990 call CRT1414753c4 directly. Its existing-pointer
// growth path uses HeapReAlloc(heap,0,old,size), without a tail initialization.
// Paired controlled experiments are retained in the converter's
// analysis/wave3-allocator-tail-discrepancy-contract.json. Do not manufacture
// zeros merely because one historical heap happened to supply zeroed bytes.
//
// This reuses the port's CRT allocator; original new-handler/errno/OOM behavior
// and allocator address/heap-history equivalence remain separately qualified.
#pragma once

#include <cstddef>
#include <cstdlib>

namespace def::model::llobj {

template<class T>
T* realloc_uninitialized(T* pointer, std::size_t bytes)
{
    return static_cast<T*>(std::realloc(pointer, bytes));
}

} // namespace def::model::llobj
