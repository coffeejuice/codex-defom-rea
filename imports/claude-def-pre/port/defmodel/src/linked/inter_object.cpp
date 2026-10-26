// Inter-object data: the DB_STEP_DATA list Cls_1401fa020 (step+0x13c8), its record
// Cls_1401f46e0, the record's leading block Cls_1401fb020 and the window list Cls_140269250.
// Layouts: defmodel/layout/cls_1401fa020.hpp, cls_1401f46e0.hpp, cls_1401fb020.hpp,
// cls_140269250.hpp.
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/cls_1401fb020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/window/llwindef_data.hpp>

#include <defmodel/dynary_init.hpp>
#include <defmodel/port.hpp>

#if defined(_MSC_VER)
#pragma warning(disable : 4702) // code after the DEF_NOT_PORTED stubs of other packets' functions
#endif

namespace def::model {

namespace {

// LLWINDEF_DATA (0x920, vtable 0x140535fe0, packet kw-g3: defmodel/window/llwindef_data.hpp):
// slot 0 is the scalar deleting dtor 14026a380, i.e. `delete node`.
void delete_llwindef(LLWINDEF_DATA* node) { delete node; }

// The node link sits at LLWINDEF_DATA+0x840 (1402699c0 reads local_30[0x108]).
LLWINDEF_DATA* llwindef_next(LLWINDEF_DATA* node)
{
    return *reinterpret_cast<LLWINDEF_DATA**>(reinterpret_cast<std::byte*>(node) + 0x840);
}

} // namespace

// ---- Cls_140269250 ----------------------------------------------------------------------------

// @src 140269250  ctor
Cls_140269250::Cls_140269250(DB_STEP_DATA* owner_)
{
    owner = owner_;
    reset();
}

// @src 140269320  dtor
Cls_140269250::~Cls_140269250() { clear(); }

// @src 140269290  init
void Cls_140269250::reset()
{
    tail = nullptr;
    head = nullptr;
    count = 0;
    u18 = 2;
    u28 = 0;
    u10 = -1;
    if (owner != nullptr) {
        std::int32_t dim = owner->dimension; // 140158e90
        set_dimension(&dim);
    }
}

// @src 14026a120  set owner
void Cls_140269250::set_owner(DB_STEP_DATA* owner_)
{
    owner = owner_;
    if (owner != nullptr) {
        std::int32_t dim = owner->dimension; // 140158e90
        set_dimension(&dim);
    }
}

// @src 14026a180  set dimension
void Cls_140269250::set_dimension(const std::int32_t* dim) { u18 = *dim; }

// @src 1402699c0  clear
void Cls_140269250::clear()
{
    LLWINDEF_DATA* node = head;
    while (node != nullptr) {
        LLWINDEF_DATA* following = llwindef_next(node);
        delete_llwindef(node); // (*vtbl[0])(node, 1)
        node = following;
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// ---- Cls_1401fb020 ----------------------------------------------------------------------------

// @src 1401fb020  ctor (the two curves are constructed first, as members)
Cls_1401fb020::Cls_1401fb020()
{
    u010 = 0;
    u014 = 0;
    u000 = 1;
    u004 = 0;
    u005 = 0;
    u008 = 0;
    u00c = 0;
    for (std::int32_t i = 0; i < 12; i = i + 1) {
        u108[i] = 0;
    }
    dynary_init(&u018, 0x15); // 14012d2a0
    dynary_init(&u090, 0x15);
}

// @src 1401fb1a0  dtor: curve 0x90, then curve 0x18 (member destruction order)
Cls_1401fb020::~Cls_1401fb020() = default;

// ---- Cls_1401f46e0 ----------------------------------------------------------------------------

// @src 1401f46e0  ctor (members first: Cls_1401fb020, the window list with owner 0, 3 curves)
Cls_1401f46e0::Cls_1401f46e0(DB_STEP_DATA* owner_)
{
    owner = owner_;
    next = nullptr;
    u168 = 0;
    u16c = 0;
    u170 = 0;
    u174 = 1;
    u178 = 0;
    u180 = 0;
    dynary_init(&u1b8, 0x17); // 14012d2a0
    u230 = 0;
    u234 = 0;
    u238 = 0;
    dynary_init(&u240, 0x16);
    u330 = 0;
    u334 = 1;
    u338 = 0;
    u340 = 0;
    u348[3] = 0; // 1401f47ef..: 0x360, 0x358, 0x350, 0x348, 0x370, 0x368
    u348[2] = 0;
    u348[1] = 0;
    u348[0] = 0;
    u348[5] = 0;
    u348[4] = 0;
    u378 = 0;
    u37c = 0;
    u384 = 0;
    u380 = 0;
    u388 = 0;
    u188.set_owner(owner); // 14026a120
}

// @src 1401f4950  dtor: curves 0x2b8, 0x240, 0x1b8, window list (140269320), block (1401fb1a0),
//                 all as member destruction
// @src 1401fb200  scalar deleting dtor: `delete record`
Cls_1401f46e0::~Cls_1401f46e0() = default;

// ---- Cls_1401fa020 ----------------------------------------------------------------------------

// @src 1401fa020  ctor
Cls_1401fa020::Cls_1401fa020(DB_STEP_DATA* owner_)
{
    owner = owner_;
    reset();
}

// @src 1401fa060  dtor
Cls_1401fa020::~Cls_1401fa020() { clear(); }

// @src 1401fa080  reset
void Cls_1401fa020::reset()
{
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 1401fade0  clear
void Cls_1401fa020::clear()
{
    Cls_1401f46e0* node = head;
    while (node != nullptr) {
        Cls_1401f46e0* following = node->next;
        delete node; // 1401fb200(node, 1)
        node = following;
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

} // namespace def::model
