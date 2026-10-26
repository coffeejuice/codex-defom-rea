// IFace and _DEF_FACE (p1-07). Layout: defmodel/layout/iface.hpp, defmodel/layout/def_face.hpp.
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/iface.hpp>

#include <defmodel/geo/pending.hpp>

namespace def::model {

// @src 1402f8320  ctor: *this = IFace::vftable
IFace::IFace() = default;

// @src 1402f7b20  ctor: IFace ctor, vptr, loops (140284c20), u20 (14019a6b0), u28 and u30
// (1402f8400: CowVector ctor, body 1402f8520), u38 (1402f8490, body 1402f85d0), u40 = 0,
// u10 = 0, u18 = u19 = 0. The id at 0x44 is left for the caller (p1-08 140284b40).
// @src 1402f8400  CowVector handle ctor (new 0x20, body ctor 1402f8520), inlined here
// @src 1402f8520  CowVector body ctor: refs 1, three zero pointers
// @src 1402f8490  CowVector handle ctor (body ctor 1402f85d0)
// @src 1402f85d0  CowVector body ctor
DEF_FACE::DEF_FACE()
{
    u40 = 0;
    u10 = nullptr;
    u18 = false;
    u19 = false;
}

// @src 1402f7be0  dtor: unlink this face from every _DEF_LOOP it is bounded by
// (140412550), then the members in reverse order: u38, u30, u28 (1402f8470), u20, loops
// @src 1402f8470  CowVector handle dtor: release 1402f8570
// @src 1402f8570  release: --refs == 0 -> body scalar deleting dtor 1402f8680
// @src 1402f8680  body scalar deleting dtor (body dtor 1402f8700, free)
// @src 1402f8700  body dtor: free(first)
// @src 1402f82e0  slot 13: scalar deleting dtor (free through MallocObject)
DEF_FACE::~DEF_FACE()
{
    // 140158f10: loops.size() (FUN_140159c20 on this + 8)
    std::int32_t n = static_cast<std::int32_t>(loops.size());
    for (std::int32_t i = 0; i < n; i = i + 1) {
        ILoop* l = loop_at(&i);
        DEF_LOOP* dl = pending::cast_def_loop(l);
        if (dl != nullptr) {
            pending::fun_140412550(dl, this);
        }
    }
}

// @src 1402f7d30  slot 0: empty
void DEF_FACE::vslot0(void*, void*) {}
// @src 1402f7d50  slot 1: empty
void DEF_FACE::vslot1(double, double, bool) {}
// @src 1402f7d00  slot 2: empty
void DEF_FACE::vslot2() {}
// @src 1402f7d70  slot 3
CowVector<double>* DEF_FACE::coords() { return &u20; }
// @src 1402f7d90  slot 4
CowVector<std::int64_t>* DEF_FACE::u28_vector() { return &u28; }
// @src 1402f7db0  slot 5
CowVector<std::int64_t>* DEF_FACE::u30_vector() { return &u30; }
// @src 1402f7dd0  slot 6
CowVector<std::int64_t>* DEF_FACE::u38_vector() { return &u38; }
// @src 1402f7d10  slot 7: empty
void DEF_FACE::vslot7(void*) {}
// @src 1402f8280  slot 8
bool DEF_FACE::flag18() const { return u18; }
// @src 1402f8290  slot 9
void DEF_FACE::set_flag18(bool v) { u18 = v; }
// @src 1402f82b0  slot 10
bool DEF_FACE::flag19() const { return u19; }
// @src 1402f82c0  slot 11
void DEF_FACE::set_flag19(bool v) { u19 = v; }
// @src 1402f7ce0  slot 14: empty
void DEF_FACE::vslot14(void*) {}

// @src 1402f7df0  slot 12: loop i through at(i) (140284de0, detaching) and operator*
ILoop* DEF_FACE::loop_at(const std::int32_t* i)
{
    std::int32_t n = 0;
    n = static_cast<std::int32_t>(loops.size()); // 140158f10
    ILoop* r;
    if (n < 1) {
        r = nullptr;
    } else if (*i < 0) {
        r = nullptr;
    } else if (*i < n) {
        r = loops.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(*i)));
    } else {
        r = nullptr;
    }
    return r;
}

// @src 1402f7e80  loops.push_back (140284d30)
void DEF_FACE::add_loop(ILoop* loop)
{
    ILoop* v = loop;
    loops.push_back(v);
}

// @src 1402f7f90  0x40
void DEF_FACE::set_u40(const std::int32_t* v) { u40 = *v; }

// @src 1402f7fb0  0x40
std::int32_t DEF_FACE::u40_value() const { return u40; }

} // namespace def::model
