// p1-04 functions on DYNARY and its list Cls_1401364a0 (defmodel/llobj/dynary_table.hpp).
#include <defmodel/llobj/dynary_table.hpp>

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>

#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/llobj/ext.hpp>

namespace def::model::llobj {

namespace {
// 32-bit multiply with the binary's wraparound (imul r32).
std::int32_t mul(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}
std::int32_t add(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
}
} // namespace

// @src 140130530  DYNARY index of (i, j, k, c)
std::int32_t dynary_index(const DYNARY& d, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t c)
{
    if (d.ndim != 0 && -1 < i) {
        const std::int32_t n0 = d.n[0] < 1 ? 1 : d.n[0];
        if (i < n0 && -1 < j) {
            const std::int32_t n1 = d.n[1] < 1 ? 1 : d.n[1];
            if (j < n1 && -1 < k) {
                const std::int32_t n2 = d.n[2] < 1 ? 1 : d.n[2];
                if (k < n2 && -1 < c) {
                    const std::int32_t nc = d.ncomp < 1 ? 1 : d.ncomp;
                    if (c < nc) {
                        if (d.ndim != 1) {
                            const std::int32_t a = d.n[0] < 1 ? 1 : d.n[0];
                            const std::int32_t b = d.n[1] < 1 ? 1 : d.n[1];
                            const std::int32_t e = d.n[2] < 1 ? 1 : d.n[2];
                            return add(add(add(mul(j, a), i), mul(mul(mul(c, a), b), e)), mul(mul(k, a), b));
                        }
                        return add(mul(i, d.ncomp), c);  // unclamped ncomp, as the original
                    }
                }
            }
        }
    }
    return -1;
}

// @src 140130720  DYNARY value at (i, j, k, c)
double dynary_get(const DYNARY& d, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t c)
{
    const std::int32_t idx = dynary_index(d, i, j, k, c);
    if (d.values == nullptr || idx < 0)
        return 0.0;
    return d.values[idx];
}

// @src 140130800  DYNARY store with |v| outside (1e-30, 1e30) flushed to 0
void dynary_set(DYNARY& d, double v, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t c)
{
    double stored = 0.0;
    if (std::fabs(v) < kDynaryValueMax) {  // fabs is the CRT's (140484558)
        if (kDynaryValueMin < std::fabs(v))
            stored = v;
    }
    const std::int32_t idx = dynary_index(d, i, j, k, c);
    if (d.values != nullptr && -1 < idx)
        d.values[idx] = stored;
}

// @src 14013c170  DYNARY scalar deleting dtor
DYNARY* dynary_delete(DYNARY* p, unsigned flags)
{
    ext::dynary_dtor(p);
    if ((flags & 1) != 0)
        ::operator delete(p);
    return p;
}

DYNARY* dynary_next(const DYNARY* p)
{
    DYNARY* n;
    std::memcpy(&n, reinterpret_cast<const std::byte*>(p) + 0x60, sizeof n);
    return n;
}

void dynary_set_next(DYNARY* p, DYNARY* next)
{
    std::memcpy(reinterpret_cast<std::byte*>(p) + 0x60, &next, sizeof next);
}

// @src 140158e70  return *(int*)(self + 0x10)
std::int32_t int_at_10(const void* self)
{
    std::int32_t v;
    std::memcpy(&v, static_cast<const std::byte*>(self) + 0x10, sizeof v);
    return v;
}

// @src 140158e90  return *(int*)(self + 0x1c)
std::int32_t int_at_1c(const void* self)
{
    std::int32_t v;
    std::memcpy(&v, static_cast<const std::byte*>(self) + 0x1c, sizeof v);
    return v;
}

} // namespace def::model::llobj

namespace def::model {

// @src 1401364a0  ctor
Cls_1401364a0::Cls_1401364a0() { init(); }

// @src 140136510  dtor (-> clear 140136910)
Cls_1401364a0::~Cls_1401364a0() { clear(); }

// @src 1401364d0  tail = head = 0, count = 0
void Cls_1401364a0::init()
{
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 140136670  append a new DYNARY (operator new(0x78) + ctor 14012d050) and return it
DYNARY* Cls_1401364a0::add()
{
    DYNARY* node = llobj::ext::new_dynary();  // 0 when operator new fails
    if (count == 0) {
        tail = node;
        head = node;
    } else {
        llobj::dynary_set_next(static_cast<DYNARY*>(tail), node);
        tail = node;
    }
    count = count + 1;
    return node;
}

// @src 140136860  n-th node, 1-based (n < 2 gives the head)
DYNARY* Cls_1401364a0::at(std::int32_t n) const
{
    std::int32_t i = 0;
    DYNARY* p = static_cast<DYNARY*>(head);
    while (p != nullptr && (i = i + 1, i < n))
        p = llobj::dynary_next(p);
    return p;
}

// @src 140136910  delete every node (14013c170(p, 1)), reset
void Cls_1401364a0::clear()
{
    DYNARY* next = static_cast<DYNARY*>(head);
    DYNARY* p;
    while (p = next, p != nullptr) {
        next = llobj::dynary_next(p);
        if (p != nullptr)
            llobj::dynary_delete(p, 1);
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 14013c1b0  return count (+0x10)
std::int32_t Cls_1401364a0::size() const { return count; }

} // namespace def::model
