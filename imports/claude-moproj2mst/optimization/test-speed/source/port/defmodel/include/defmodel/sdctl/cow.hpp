// defmodel/sdctl/cow.hpp: the copy-on-write container handles the model uses for its member
// arrays and lists (LLOBJ_DATA 0x3950..0x3978, state-variable lists, the int lists that
// 140180390 / 140183900 build). No RTTI. Recovered from code in 0x140159860..0x14015eb10,
// 0x14019a6b0..0x14019cc60 and the identical-code-folded helpers in 0x140002970..0x140005ae0.
//
// A handle is one pointer to a heap body whose first field is a 32-bit reference count:
//
//   CowVector<T>   body 0x20 = { int32 refs (=1, 140001000); pad; T* first 0x08, last 0x10,
//                  end 0x18 } (a VS2010 std::vector<T>), handle 8 bytes
//   CowList<T>     body 0x18 = { int32 refs; pad; node* head 0x08 (sentinel); size 0x10 },
//                  node 0x18 = { next, prev, value } (a VS2010 std::list<T>)
//
// Copying a handle adds a reference (140159910). Mutating calls first detach: when refs > 1
// (compared unsigned) the handle drops one reference and takes a fresh copy with refs = 1.
// Releasing decrements refs (140001010) and deletes the body when it reaches 0.
//
// CowVector keeps the original storage code (buffer from operator new = malloc, released with
// free; growth newcap = size + max(n, size); clone capacity = size), so allocation sizes and
// order match the binary. It is meant for trivially copyable element types (int32, double,
// pointers), which is what every instantiation in the binary holds. CowList nodes are allocated
// one by one in the same order as the original; it uses std::list.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <new>
#include <type_traits>

namespace def::model {

namespace cow_detail {

// operator new (FUN_1404797d8) is malloc that throws; the byte count saturates to -1 on
// overflow (SUB168(ZEXT816(sizeof(T)) * n) check in _Insert_n and the clone).
template <class T>
T* alloc(std::size_t n)
{
    std::size_t bytes = n * sizeof(T);
    if (n != 0 && bytes / n != sizeof(T)) bytes = ~std::size_t{0};
    void* p = std::malloc(bytes);
    if (p == nullptr) throw std::bad_alloc();
    return static_cast<T*>(p);
}

// @src 140002ca0  copy [first, last) to dest, return dest end (8-byte elements)
// @src 140002ac0  the same, 4-byte elements
// The 2D centring branch of 1401fc730 (1401fd240, packets C08-C13) has its own instances; the
// element operator= each one calls belongs to that element type:
// @src 1403e61f0  the same, 0x18-byte trivially copyable elements (byte copy through a temporary)
// @src 1403e6180  the same, 0x28-byte elements (operator= 1403e6260, defcmd/objpos_c2d.hpp)
// @src 1403e81a0  the same, 0x18-byte elements (operator= 1403e38b0)
// @src 1403e82e0  the same, 0x10-byte elements (operator= 1403e3920)
// @src 1403e8350  the same, 0x20-byte elements (operator= 1403e3960)
// @src 1403e83c0  the same, 0x10-byte elements (operator= 1403e39f0)
// @src 1403e8430  the same, 0x18-byte elements (operator= 1403e3a30)
template <class T>
T* copy(const T* first, const T* last, T* dest)
{
    for (; first != last; ++first) *dest++ = *first;
    return dest;
}

// @src 14019c910  copy_backward (8-byte elements)
// @src 14015eac0  the same, 4-byte elements
template <class T>
T* copy_backward(const T* first, const T* last, T* dest_end)
{
    while (first != last) *--dest_end = *--last;
    return dest_end;
}

// @src 14019c970  fill [first, last) with *val (8-byte elements)
// @src 14015eb10  the same, 4-byte elements
template <class T>
void fill(T* first, T* last, const T* val)
{
    for (; first != last; ++first) *first = *val;
}

} // namespace cow_detail

template <class T>
struct CowVectorBody {
    static_assert(std::is_trivially_copyable_v<T>, "CowVector holds trivially copyable values");

    std::int32_t refs;   // 0x00
    T* first;            // 0x08
    T* last;             // 0x10
    T* end;              // 0x18

    // @src 14019af90  body ctor: refs = 1 (140001000), three null pointers
    // @src 14019b090  the same, other element type
    // @src 14019b190  the same, other element type
    // @src 14019b2c0  the same, other element type
    // @src 14015a380  the same, int32 elements
    // @src 140001000  refs = 1
    CowVectorBody() : refs(1), first(nullptr), last(nullptr), end(nullptr) {}

    // @src 1400033f0  clone, 8-byte elements: refs = 1; count = (int32)(last - first); count < 1
    //                 gives three null pointers, else a buffer of exactly count elements
    // @src 140002ba0  clone, 4-byte elements
    explicit CowVectorBody(const CowVectorBody& o) : refs(1)
    {
        const std::int32_t count = static_cast<std::int32_t>(o.last - o.first);
        if (count < 1) {
            first = last = end = nullptr;
        } else {
            first = cow_detail::alloc<T>(static_cast<std::size_t>(static_cast<std::int64_t>(count)));
            last = first + count;
            end = first + count;
            cow_detail::copy(o.first, o.last, first);
        }
    }
    CowVectorBody& operator=(const CowVectorBody&) = delete;

    // @src 14019b610  body dtor: free(first)
    // @src 14015a5f0  the same, int32 elements
    // @src 14019b640  body dtor, other element type
    // @src 14019b6c0  body dtor, other element type
    ~CowVectorBody() { std::free(first); }

    // @src 140002970  size ((last - first) >> 3)
    // @src 140002980  size, 4-byte elements
    // @src 1403e51b0  size, 0x18-byte elements ((last - first) / 0x18; 1401fd240 subtree)
    std::size_t size() const { return static_cast<std::size_t>(last - first); }

    // @src 14019bd10  std::vector::_Insert_n(pos, n, *val), 8-byte elements
    // @src 14015e550  the same, 4-byte elements
    void insert_n(T* pos, std::size_t n, const T* val)
    {
        if (static_cast<std::size_t>(end - last) < n) {
            const std::size_t sz = size();
            std::size_t grow = n;
            if (n < sz) grow = sz;
            T* buf = cow_detail::alloc<T>(sz + grow);
            T* p = cow_detail::copy(first, pos, buf);
            for (std::size_t k = n; k != 0; --k) *p++ = *val;
            T* nlast = cow_detail::copy(pos, last, p);
            std::free(first);
            first = buf;
            last = nlast;
            end = buf + (sz + grow);
        } else {
            const std::size_t after = static_cast<std::size_t>(last - pos);
            T* const old_last = last;
            if (n < after) {
                cow_detail::copy(last - n, last, last);
                last += n;
                cow_detail::copy_backward(pos, old_last - n, old_last);
                cow_detail::fill(pos, pos + n, val);
            } else {
                T* p = last;
                for (std::size_t k = n - after; k != 0; --k) *p++ = *val;
                last += n - after;
                cow_detail::copy(pos, old_last, last);
                last += after;
                cow_detail::fill(pos, old_last, val);
            }
        }
    }
};

template <class T>
class CowVector {
public:
    using value_type = T;
    using Body = CowVectorBody<T>;

    // @src 14019a6b0  ctor: operator new(0x20), body ctor 14019af90
    // @src 14019a8a0  the same, other element type (14019b090)
    // @src 14019a960  the same, other element type (14019b190)
    // @src 14019aa20  the same, other element type (14019b2c0)
    // @src 140159ad0  the same, int32 elements (14015a380)
    CowVector() : body_(new Body()) {}

    // @src 140159910  copy = add a reference (refs++)
    CowVector(const CowVector& o) : body_(o.body_) { ++body_->refs; }

    // @src 1402f09f0  assignment: add a reference to the source (140159910), release own
    //                 body (14019afe0), share
    // @src 1402207c0  the same, int32 elements (release 14015a3d0)
    CowVector& operator=(const CowVector& o)
    {
        ++o.body_->refs;
        release(body_);
        body_ = o.body_;
        return *this;
    }

    // @src 14019a720  dtor (~_Timevec in Ghidra) -> release 14019afe0
    // @src 14019a910  dtor, other element type (release 14019b0e0)
    // @src 14019a9d0  dtor, body holding an element array with destructors (release 14019b1e0)
    // @src 14019b670  (Destroy) that body's dtor: eh vector deleting dtor of the array
    // @src 14019b4e0  eh vector deleting dtor (flag 3) of that array
    // @src 14019aa90  dtor, other element type (release 14019b310)
    // @src 140159b40  dtor, int32 elements (release 14015a3d0)
    ~CowVector() { release(body_); }

    // @src 140003090  size, 8-byte elements
    // @src 1400030b0  size, 4-byte elements
    std::size_t size() const { return body_->size(); }
    bool empty() const { return body_->first == body_->last; }
    // read access, no detach
    const T& operator[](std::size_t i) const { return body_->first[i]; }
    const T* begin() const { return body_->first; }
    const T* end() const { return body_->last; }
    std::int32_t use_count() const { return body_->refs; }
    const Body* body() const { return body_; }

    // @src 1400051a0  detach, 8-byte elements (clone 1400033f0)
    // @src 140004e60  detach, 4-byte elements (clone 140002ba0)
    void detach()
    {
        if (static_cast<std::uint32_t>(body_->refs) > 1u) {
            --body_->refs;
            body_ = new Body(*body_);
        }
    }

    // @src 140005ae0  detach, then return first
    // @src 14026a4d0  the same (p1-06's instantiation)
    // @src 1400050a0  the same, 4-byte elements
    T* mutable_begin()
    {
        detach();
        return body_->first;
    }

    // @src 14019adb0  detach, then return last
    // @src 140159bf0  the same, 4-byte elements
    T* mutable_end()
    {
        detach();
        return body_->last;
    }

    // @src 140005e70  operator[](i) for writing, 8-byte elements: detach (inlined twice, the
    //                 second is a no-op), return &first[i]
    // @src 140005100  the same, 4-byte elements
    // @src 14026a470  the same (p1-06's instantiation: first + i * 8)
    T& mutable_at(std::size_t i)
    {
        detach();
        detach();
        return body_->first[i];
    }

    // @src 14019ade0  erase [first, last): detach, copy the tail down, last -= count; the
    //                 caller's pointers are used as given (they come from mutable_begin/end)
    // @src 140159fc0  the same, 4-byte elements
    T* erase(T* first, T* last)
    {
        detach();
        cow_detail::copy(last, static_cast<const T*>(body_->last), first);
        body_->last -= (last - first);
        return first;
    }

    // @src 14019b6f0  insert n copies of val at pos: when n != 0, rebase pos from the old
    //                 first onto the detached first, then _Insert_n (14019bd10)
    // @src 14015a730  the same, 4-byte elements (14015e550)
    T* insert(T* pos, std::size_t n, const T& val)
    {
        if (n != 0) {
            T* const old_first = body_->first;
            detach();
            pos = mutable_begin() + (pos - old_first);
            body_->insert_n(pos, n, &val);
        }
        return pos;
    }

    // push_back (std::vector::push_back shape, not _Insert_n): detach; when last == end,
    // reserve(size + 1 + size / 2) (a new buffer of exactly that many elements, the old ones
    // copied over, the old buffer freed); store at last; last++.
    // @src 140220810  push_back, 4-byte elements (detach 140004e60, grow 1402217c0 / 140222010)
    // @src 140220b80  push_back, 1-byte elements (detach 14019aef0, grow 140221970 / 1402225b0)
    // @src 140221970  reserve, 1-byte elements: new buffer (1402225b0), first / last / end
    // @src 1402225b0  operator new(cap), copy [first, last) (14019cb90), free the old buffer
    // @src 1402f0ba0  push_back, 8-byte elements (p9-02, GEOPRIM section list; same body as above):
    //                 detach; when last == end, reallocate to size + 1 + size / 2 (1402f2240
    //                 -> 1402f2580: a buffer of exactly that many, copy 1402f2ce0, free the old
    //                 one); then *last++ = val. 140256800 (p8-09) is the same code.
    // @src 1402f2240  reserve(n) of that body: last/end rebased onto the new buffer
    // @src 1402f2580  allocate n elements, copy [first, last) (1402f2ce0), free the old buffer
    // @src 1402f2ce0  copy [first, last) to dest (8-byte elements)
    void push_back(const T& val)
    {
        detach();
        if (body_->last == body_->end) {
            const std::size_t n = size();
            const std::size_t cap = n + 1 + n / 2;
            T* buf = cow_detail::alloc<T>(cap);
            cow_detail::copy(static_cast<const T*>(body_->first), static_cast<const T*>(body_->last), buf);
            std::free(body_->first);
            body_->first = buf;
            body_->last = buf + n;
            body_->end = buf + cap;
        }
        *body_->last = val;
        body_->last = body_->last + 1;
    }

    // @src 14019a740  resize(n, val): n < size erases [begin + n, end), else inserts
    //                 n - size copies at the end
    // @src 140159b60  the same, 4-byte elements
    void resize(std::size_t n, const T& val)
    {
        if (n < size()) {
            T* e = mutable_end();
            T* b = mutable_begin();
            erase(b + n, e);
        } else {
            const std::size_t s = size();
            T* e = mutable_end();
            insert(e, n - s, val);
        }
    }

    // @src 14019a7d0  clear: detach (140004e60), then free the buffer (14019b040)
    // @src 1402568b0  the same, 8-byte elements of DYNARY_DATA (p1-06's instantiation)
    // @src 14019b040  body clear: free(first), first = last = end = 0
    // @src 140256900  the same body clear (p1-06's instantiation)
    void clear()
    {
        detach();
        std::free(body_->first);
        body_->first = nullptr;
        body_->last = nullptr;
        body_->end = nullptr;
    }

private:
    // @src 14019afe0  release: refs-- (140001010); at 0 the body's scalar deleting dtor
    //                 (14019b420 -> 14019b610); same code: 14019b0e0 (14019b460), 14019b1e0
    //                 (14019b4a0), 14019b310 (14019b570)
    // @src 14015a3d0  release, int32 elements (body scalar deleting dtor 14015a470)
    // @src 14015a470  body scalar deleting dtor, int32 elements
    // @src 14019b0e0  release, other element type
    // @src 14019b1e0  release, other element type
    // @src 14019b310  release, other element type
    // @src 14019b420  body scalar deleting dtor (also 14015a470)
    // @src 14019b460  body scalar deleting dtor, other element type
    // @src 14019b4a0  body scalar deleting dtor, other element type (-> Destroy 14019b670)
    // @src 14019b570  body scalar deleting dtor, other element type
    // @src 140001010  refs--, return refs == 0
    static void release(Body* b)
    {
        if (--b->refs == 0 && b != nullptr) delete b;
    }

    Body* body_;   // 0x00
};

template <class T>
struct CowListBody {
    std::int32_t refs = 1;   // 0x00
    std::list<T> l;          // 0x08 head (sentinel), 0x10 size

    // @src 14019bac0  body ctor: refs = 1, sentinel node (0x18, node ctor 14019bd00) linked to
    //                 itself, size 0
    // @src 14015a960  the same, 8-byte values (sentinel node ctor 14015af10)
    // @src 14015af10  sentinel node ctor (identity)
    // 1403e5cb0 (owner: obj_mesh2d/rc_array.hpp, C09)  the same, 8-byte values (1401fd240 subtree; sentinel node ctor 1403e6320)
    // 1403e6320 (owner: obj_mesh2d/rc_array.hpp, C09)  sentinel node ctor (identity)
    CowListBody() = default;
    // @src 14019c550  clone, 4-byte values: copy in order, each inserted before the sentinel
    //                 (14019bbf0); iterator helpers 14019b5f0, 14019c6d0, 14019c6f0, 14019c730,
    //                 14019c750, node value ctor 14019bce0
    // @src 14019c0a0  clone, 8-byte values (14019c9c0; 14019c7a0, 14019c820, 14019c840,
    //                 14019c860, 14019c8a0, 14019c8c0, node value ctor 14019cc30)
    explicit CowListBody(const CowListBody& o) : refs(1), l(o.l) {}
    CowListBody& operator=(const CowListBody&) = delete;

    // @src 14019bb70  body dtor: free every node, then the sentinel
    // @src 14015aa10  the same, 8-byte values
    ~CowListBody() = default;
};

template <class T>
class CowList {
public:
    using value_type = T;
    using Body = CowListBody<T>;

    // @src 14019ac40  ctor: operator new(0x18), body ctor 14019bac0
    // @src 140159860  the same, 8-byte values (14015a960)
    CowList() : body_(new Body()) {}

    // @src 14019acb0  copy ctor: share the body, refs++ (140159910); Ghidra shows it as
    //                 std::locale::locale(const locale&)
    CowList(const CowList& o) : body_(o.body_) { ++body_->refs; }

    CowList& operator=(const CowList& o)
    {
        ++o.body_->refs;
        release(body_);
        body_ = o.body_;
        return *this;
    }

    // @src 14019acf0  dtor -> release 14019b3c0
    // @src 140159930  dtor, 8-byte values (~_Timevec in Ghidra) -> release 14015a320
    ~CowList() { release(body_); }

    // @src 140159c20  size (body + 0x10)
    // @src 140159c40  the same
    // @src 140159c60  the same
    std::size_t size() const { return body_->l.size(); }
    bool empty() const { return body_->l.empty(); }
    std::int32_t use_count() const { return body_->refs; }
    const std::list<T>& items() const { return body_->l; }
    const Body* body() const { return body_; }

    // @src 14019af60  detach, 4-byte values: when refs > 1, 14019b9e0
    // @src 14019ae60  detach, 8-byte values: when refs > 1, 14019b780
    // @src 14019b9e0  refs-- (result ignored), replace the body by a clone (14019c550)
    // @src 14019b780  the same, 8-byte values (14019c0a0)
    void detach()
    {
        if (static_cast<std::uint32_t>(body_->refs) > 1u) {
            --body_->refs;
            body_ = new Body(*body_);
        }
    }

    // @src 14019ad10  push_back: detach, end() (14019af20), insert before the sentinel
    //                 (14019bbf0), size++
    // @src 14019af20  end(): detach, iterator(head) (14019b5f0)
    // @src 14032a4e0  push_back, 8-byte values (detach 14019ae60, end() 14032a5a0, 14019c9c0)
    // @src 14032a5a0  end(), 8-byte values
    void push_back(const T& value)
    {
        detach();
        body_->l.push_back(value);
    }

    // begin() / end() for writing: detach first, as the original's do (end(): 14019af20,
    // 14032a5a0 above; the iterator is the node pointer, here a std::list iterator)
    typename std::list<T>::iterator mutable_begin()
    {
        detach();
        return body_->l.begin();
    }
    typename std::list<T>::iterator mutable_end()
    {
        detach();
        return body_->l.end();
    }
    std::list<T>& mutable_items()
    {
        detach();
        return body_->l;
    }

    // @src 14019a860  operator[](i) for writing: detach (14019ae60), walk i nodes from
    //                 head->next (14019ba60), return &node->value (node + 0x10)
    // @src 14019ba60  walk i nodes from head->next
    T& mutable_at(std::size_t i)
    {
        detach();
        auto it = body_->l.begin();
        for (std::size_t k = 0; k < i; ++k) ++it;
        return *it;
    }

    // @src 140159a40  front() for writing (8-byte values; added by p9-01): detach (140159f90),
    //                 &head->next->value (node + 0x10)
    T& mutable_front()
    {
        detach();
        return body_->l.front();
    }

    // @src 140159970  pop_front (8-byte values; added by p9-01): begin() (140159eb0: detach,
    //                 head->next), erase it (140159f30: detach again, 14015a5d0 iterator copy,
    //                 14015ab80: unlink the node, free it, size--). As in the original the list
    //                 must not be empty.
    // @src 140159f30  erase(iterator), folded in here (its only use)
    void pop_front()
    {
        detach();
        detach();
        body_->l.pop_front();
    }

    // @src 14032a540  operator[](i) for reading: walk i nodes (14019ba60), &node->value
    const T& at(std::size_t i) const
    {
        auto it = body_->l.begin();
        for (std::size_t k = 0; k < i; ++k) ++it;
        return *it;
    }

    // @src 14015a620  clear: refs == 1 clears in place (14015e4b0); otherwise refs-- and a
    //                 new empty body (14015a960), the shared one is left to its other owners
    // @src 14015e4b0  body clear: size = 0, free the nodes, relink the sentinel to itself
    void clear()
    {
        if (body_->refs == 1) {
            body_->l.clear();
        } else {
            --body_->refs;
            body_ = new Body();
        }
    }

private:
    // @src 14019b3c0  release: refs--; at 0 the body's scalar deleting dtor (14019b5b0 ->
    //                 14019bb70)
    // @src 14015a320  release, 8-byte values (14015a430 -> 14015aa10)
    // @src 14019b5b0  body scalar deleting dtor
    // @src 14015a430  the same, 8-byte values
    static void release(Body* b)
    {
        if (--b->refs == 0 && b != nullptr) delete b;
    }

    Body* body_;   // 0x00
};

static_assert(sizeof(CowVector<void*>) == 8);
static_assert(sizeof(CowList<std::int32_t>) == 8);
static_assert(sizeof(CowVectorBody<double>) == 0x20);
static_assert(offsetof(CowVectorBody<double>, last) == 0x10);

} // namespace def::model
