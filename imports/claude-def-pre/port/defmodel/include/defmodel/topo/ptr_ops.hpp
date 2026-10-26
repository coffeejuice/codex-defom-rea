// defmodel/topo/ptr_ops.hpp: five tiny identical-code-folded helpers at 0x14008c3c0..0x14008c410
// that many classes share (pointer-wrapper ctor, assign, compare, test; an empty function). They
// sit in p1-11's address range; their callers (1401b4f30.., 14042c3f0.., 1405000a0..) use them
// on 8-byte handles and iterators.
#pragma once

namespace def::model::topo {

// @src 14008c3c0  empty function (trivial dtor / no-op callback)
inline void noop() {}

// @src 14008c3d0  *p = 0; return p
template <class P>
P* ptr_reset(P* p) {
    *p = P{};
    return p;
}

// @src 14008c3e0  *dst = *src
template <class P>
void ptr_assign(P* dst, const P* src) { *dst = *src; }

// @src 14008c400  *a == *b
template <class P>
bool ptr_equal(const P* a, const P* b) { return *a == *b; }

// @src 14008c410  *p != 0
template <class P>
bool ptr_nonnull(const P* p) { return *p != P{}; }

} // namespace def::model::topo
