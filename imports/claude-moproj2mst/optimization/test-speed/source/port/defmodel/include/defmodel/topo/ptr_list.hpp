// defmodel/topo/ptr_list.hpp: the 8-byte container handles that the topology classes hold.
//
// The binary keeps lists and vectors of pointers behind one 8-byte pointer to a reference-counted
// representation (copy on write: a writer first detaches when the count is above 1).
//
// * Lists (_DEF_LOOP +0x08/+0x10, _DEF_EDGE +0x10, _DEF_VERTEX +0x18) are p1-08's CowList<T>
//   (LoopList, EdgeList, FaceList in <defmodel/mesh2d/cow_list.hpp>). When that header is not in
//   the tree yet, a same-size placeholder with the same member names stands in (conventions
//   section 4). The topology code reaches the lists only through list_size/list_at/list_push_back/
//   list_push_front/list_clear below.
// * CowPtrVector<T>: the vector handle (_DEF_EDGE +0x28, OBJ_GEO2D_DIGITIZED +0x48, the argument of
//   _DEF_VERTEX slot 0). Representation {int32 refs; T** first; T** last; T** end} (0x20, ctor
//   1402f8520 in p1-07). Its push_back/clear/copy-on-write functions 14041d830..14041dc40 are p1-11
//   and are ported here; the handle ctor (1402f8400, p1-07) and release (14015a320, p1-04) are local
//   equivalents. (p1-05's CowVector<T> has the same body; its growth rule differs from 14041d830.)
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <list>

#if __has_include(<defmodel/mesh2d/cow_list.hpp>)
#include <defmodel/mesh2d/cow_list.hpp>
#define DEF_P1_11_LIST_FALLBACK 0
#else
#define DEF_P1_11_LIST_FALLBACK 1
#endif

namespace def::model {

class IFace;
class ILoop;
class IEdge;
class IVertex;

#if DEF_P1_11_LIST_FALLBACK
// placeholder of the canonical CowList<T> (p1-05 sdctl/cow.hpp, aliased by p1-08); same API subset
template <class T>
class CowList {
public:
    CowList() : body_(new std::list<T>()) {}
    ~CowList() { delete body_; }
    CowList(const CowList&) = delete;
    CowList& operator=(const CowList&) = delete;

    std::size_t size() const { return body_->size(); }
    void push_back(const T& v) { body_->push_back(v); }
    std::list<T>& mutable_items() { return *body_; }
    T& mutable_at(std::size_t i) {
        auto it = body_->begin();
        for (std::size_t k = 0; k < i; ++k) ++it;
        return *it;
    }
    void clear() { body_->clear(); }

private:
    std::list<T>* body_;
};
static_assert(sizeof(CowList<void*>) == 8);

using LoopList = CowList<ILoop*>;     // A
using EdgeList = CowList<IEdge*>;     // B
using FaceList = CowList<IFace*>;     // C
using VertexList = CowList<IVertex*>; // D
#endif

namespace topo {

// size through 140159c20 / 140159c40 / 140159c60 (p1-04): body size, truncated to the caller's int
template <class L>
std::int32_t list_size(const L& l) { return static_cast<std::int32_t>(l.size()); }

// at(i) (140284ff0 family: detach, node at index) then operator* (140285820 family)
template <class L>
auto list_at(L& l, std::int64_t i) { return l.mutable_at(static_cast<std::size_t>(i)); }

// push_back (140284f40 family, p1-08)
template <class L, class T>
void list_push_back(L& l, T v) { l.push_back(v); }

// @src 140413790  push_front of the IEdge* list: detach (140285e20), begin (140284ec0), body
// insert before it (140286ff0)
template <class L, class T>
void list_push_front(L& l, T v) {
    auto& items = l.mutable_items();
    items.insert(items.begin(), v);
}

// clear (140286770 family, p1-08)
template <class L>
void list_clear(L& l) { l.clear(); }

template <class T>
class CowPtrVector {
public:
    struct Rep {
        std::int32_t refs;
        T** first;
        T** last;
        T** end;

        // @src 14041d9a0  element count
        std::int64_t count() const { return last - first; }

        // @src 14041d950  free the buffer, zero the three pointers
        void release_buffer() {
            std::free(first);
            first = nullptr;
            last = nullptr;
            end = nullptr;
        }

        // @src 14041dc40  copy [from, to) to dst, return the end of the copy
        static T** copy_range(T** from, T** to, T** dst) {
            for (; from != to; ++from) *dst++ = *from;
            return dst;
        }

        // @src 14041dbc0  allocate n slots, copy [from, to) into them, free the old buffer
        T** reallocate(std::uint64_t n, T** from, T** to) {
            T** p = static_cast<T**>(alloc_slots(n));
            copy_range(from, to, p);
            std::free(first);
            return p;
        }

        // @src 14041da40  grow the capacity to n slots, keeping the elements
        void reserve(std::int64_t n) {
            const std::int64_t c = count();
            T** p = reallocate(static_cast<std::uint64_t>(n), first, last);
            first = p;
            last = p + c;
            end = first + n;
        }

        // @src 14041dad0  copy ctor of the representation: refs 1, capacity == count
        static Rep* copy_of(const Rep& o) {
            Rep* r = new Rep;
            r->refs = 1; // 140001000
            const std::int32_t n = static_cast<std::int32_t>(o.count());
            if (n < 1) {
                r->first = nullptr;
                r->last = nullptr;
                r->end = nullptr;
            } else {
                r->first = static_cast<T**>(alloc_slots(static_cast<std::uint64_t>(n)));
                r->last = r->first + n;
                r->end = r->first + n;
                copy_range(o.first, o.last, r->first);
            }
            return r;
        }

        // operator new of 8*n with the binary's overflow clamp (1404797d8 on SIZE_MAX fails)
        static void* alloc_slots(std::uint64_t n) {
            if (n > SIZE_MAX / 8) std::abort();
            return std::malloc(static_cast<std::size_t>(n * 8));
        }
    };

    // part of @src 1402f8400  handle ctor (p1-07): new representation, refs 1, empty
    CowPtrVector() : rep_(new Rep{1, nullptr, nullptr, nullptr}) {}
    // shares the representation (copy on write)
    CowPtrVector(const CowPtrVector& o) : rep_(o.rep_) { ++rep_->refs; }
    CowPtrVector& operator=(const CowPtrVector&) = delete;
    // part of @src 140159930  release (14015a320): last reference frees buffer and representation
    ~CowPtrVector() { drop(rep_); }

    // @src 14041d900  element count
    std::int64_t size() const { return rep_->count(); }
    T* operator[](std::int64_t i) const { return rep_->first[i]; }
    std::int32_t use_count() const { return rep_->refs; }
    const Rep* rep() const { return rep_; }

    // @src 14041d920  detach when shared
    void detach_if_shared() {
        if (1u < static_cast<std::uint32_t>(rep_->refs)) detach();
    }

    // @src 14041d9c0  drop one reference, continue on a private copy
    void detach() {
        Rep* old = rep_;
        --old->refs; // 140001010 (the old representation stays with its other owners)
        rep_ = Rep::copy_of(*old);
    }

    // @src 14041d830  push_back; a full buffer grows to size + 1 + size/2
    void push_back(T* const* v) {
        detach_if_shared();
        if (rep_->last == rep_->end) {
            const std::int64_t n = size();
            const std::uint64_t half = static_cast<std::uint64_t>(size()) / 2;
            rep_->reserve(n + 1 + static_cast<std::int64_t>(half));
        }
        *rep_->last = *v;
        ++rep_->last;
    }

    // @src 14041d8d0  clear: detach, then free the buffer
    void clear() {
        detach_if_shared();
        rep_->release_buffer();
    }

private:
    static void drop(Rep* r) {
        if (--r->refs == 0) {
            std::free(r->first);
            delete r;
        }
    }

    Rep* rep_;
};

static_assert(sizeof(CowPtrVector<void>) == 8);

} // namespace topo
} // namespace def::model
