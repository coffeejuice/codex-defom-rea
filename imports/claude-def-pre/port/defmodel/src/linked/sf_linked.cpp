// SfLinkedData / SfLinkedList and their derived node and list classes: OPSTOP_DATA,
// OpStopLinkedList, DYNARY_DATA, DynAryLinkedList (layout headers in defmodel/layout/).
//
// Nodes are allocated with operator new and deleted through the virtual dtor, as the original
// does through vtable slot 0 (scalar deleting dtor with flag 1).
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/op_stop_linked_list.hpp>
#include <defmodel/layout/opstop_data.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defmodel/layout/sf_linked_list.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

// ---- SfLinkedData -----------------------------------------------------------------------------

// @src 14025a580  ctor
// @src 14025a5e0  init (inlined here): u08 = 1, u0c = 0, next = 0
SfLinkedData::SfLinkedData()
{
    u08 = 1;
    u0c = 0;
    next = nullptr;
}

// @src 14025a5c0  dtor (only restores the vptr)
// @src 14025bf80  slot 0: scalar deleting dtor
SfLinkedData::~SfLinkedData() = default;

// @src 14025a610  slot 1
void SfLinkedData::copy_from(const SfLinkedData* other) { u0c = other->u0c; }

// ---- SfLinkedList -----------------------------------------------------------------------------

// @src 14025a680  ctor
// @src 14025a6f0  init (inlined here): tail = head = 0, count = 0
SfLinkedList::SfLinkedList()
{
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 14025a6c0  dtor
// @src 14025bfc0  slot 0: scalar deleting dtor
SfLinkedList::~SfLinkedList() { clear(); }

// @src 14025ab20  clear
void SfLinkedList::clear()
{
    SfLinkedData* node = head;
    while (node != nullptr) {
        SfLinkedData* following = node->next;
        delete node; // (*vtbl[0])(node, 1)
        node = following;
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// ---- OPSTOP_DATA ------------------------------------------------------------------------------

// @src 14025ac10  ctor
OPSTOP_DATA::OPSTOP_DATA() { reset(); }

// @src 14025ac60  dtor
// @src 14025c000  slot 0: scalar deleting dtor
OPSTOP_DATA::~OPSTOP_DATA() = default;

// @src 14025ac90  init
void OPSTOP_DATA::reset()
{
    u0c = 0;
    value = 0.0; // 14025ac9e: 8-byte zero store
}

// @src 14025acc0  slot 1
void OPSTOP_DATA::copy_from(const SfLinkedData* other)
{
    SfLinkedData::copy_from(other);
    const OPSTOP_DATA* o = cast(other);
    if (o != nullptr) {
        value = o->value;
    }
}

// @src 14025ad20  slot 2
void OPSTOP_DATA::from_values(const double* rec)
{
    u0c = static_cast<std::int32_t>(rec[1]); // cvttsd2si: truncation
    if (u0c == 1) {
        value = rec[2];
    }
}

// @src 14025ad80  slot 3
void OPSTOP_DATA::to_values(double* rec) const
{
    rec[1] = static_cast<double>(u0c);
    if (u0c == 1) {
        rec[2] = value;
    }
}

// @src 14025ade0  __RTDynamicCast(node, SfLinkedData -> OPSTOP_DATA)
OPSTOP_DATA* OPSTOP_DATA::cast(SfLinkedData* node)
{
    if (node == nullptr) {
        return nullptr;
    }
    return dynamic_cast<OPSTOP_DATA*>(node);
}

const OPSTOP_DATA* OPSTOP_DATA::cast(const SfLinkedData* node) // part of @src 14025ade0
{
    return cast(const_cast<SfLinkedData*>(node));
}

// ---- OpStopLinkedList -------------------------------------------------------------------------

// @src 14025ae30  ctor
OpStopLinkedList::OpStopLinkedList() = default;

// @src 14025ae70  dtor (restores the vptr, then SfLinkedList's dtor clears)
// @src 14025c040  slot 0: scalar deleting dtor
OpStopLinkedList::~OpStopLinkedList() = default;

// @src 14025aea0  slot 1
SfLinkedData* OpStopLinkedList::create_item() { return new OPSTOP_DATA(); }

// ---- DYNARY_DATA ------------------------------------------------------------------------------

// @src 14025afe0  ctor
// (the member handles are created first: u30 14019a6b0, u38 140159ad0, p1-05's CowVector)
DYNARY_DATA::DYNARY_DATA()
{
    reset(); // 14025b0b0
}

// @src 14025b050  dtor
// @src 14025c080  slot 0: scalar deleting dtor
DYNARY_DATA::~DYNARY_DATA()
{
    reset(); // 14025b0d0
    // then the member dtors: u38 (140159b40), u30 (14019a720)
}

// @src 14025b0b0  thunk to 14025b0d0
// @src 14025b0d0  zero the fields and clear both arrays
void DYNARY_DATA::reset()
{
    u0c = 0;
    u18 = 0;
    u1c = 0;
    u20 = 0;
    u24 = 0;
    u28 = 0;
    u30.clear(); // 1402568b0: detach, free the buffer (CowVector::clear)
    u38.clear(); // 14019a7d0
}

// @src 14025b150  slot 1: the cast result is not used (14025b172 discards it)
void DYNARY_DATA::copy_from(const SfLinkedData* other)
{
    SfLinkedData::copy_from(other);
    (void)cast(other);
}

// @src 14025b190  slot 2: empty
void DYNARY_DATA::from_values(const double*) {}

// @src 14025b1b0  slot 3: empty
void DYNARY_DATA::to_values(double*) const {}

// @src 14025b1d0  __RTDynamicCast(node, SfLinkedData -> DYNARY_DATA)
DYNARY_DATA* DYNARY_DATA::cast(SfLinkedData* node)
{
    if (node == nullptr) {
        return nullptr;
    }
    return dynamic_cast<DYNARY_DATA*>(node);
}

const DYNARY_DATA* DYNARY_DATA::cast(const SfLinkedData* node) // part of @src 14025b1d0
{
    return cast(const_cast<SfLinkedData*>(node));
}

// ---- DynAryLinkedList -------------------------------------------------------------------------

// @src 14025b9d0  ctor
DynAryLinkedList::DynAryLinkedList() = default;

// @src 14025ba10  dtor
// @src 14025c0c0  slot 0: scalar deleting dtor
DynAryLinkedList::~DynAryLinkedList() = default;

// @src 14025ba40  slot 1
SfLinkedData* DynAryLinkedList::create_item() { return new DYNARY_DATA(); }

} // namespace def::model
