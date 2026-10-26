// defmodel/topo/alloc.hpp: the binary's operator new (1404797d8: malloc, throws on failure) and
// free, for topology objects that other code releases with free().
#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

namespace def::model::topo {

inline void* malloc_or_throw(std::size_t n) {
    void* p = std::malloc(n == 0 ? 1 : n);
    if (p == nullptr) throw std::bad_alloc();
    return p;
}

inline void free_block(void* p) { std::free(p); }

} // namespace def::model::topo
