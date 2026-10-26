// defmodel/members/sdctl_index.hpp: Cls_1403331d0, the helper object SdctlGList owns at +0x30
// (packet p1-10; SdctlGList itself is p1-05's).
//
// The helper holds a pointer to a handle of a copy-on-write list of pointers, and one more
// pointer. Each list element and the extra pointer point to a two-pointer record {a, b}. On
// destruction the helper zeroes a (only when `full`) and b in every record it knows, so that the
// records no longer refer to the list being destroyed.
//
//   dtor      1403331d0 (detach 140333230(this, true), then delete the handle 140333300)
//   del dtor  140333190 (scalar deleting dtor, called by SdctlGList's dtor 140331030)
//   rep       PtrListRep: {refcount, VS2010 list sentinel, size}; nodes {next, prev, value} are
//             0x18 bytes from operator new and are released with free (140333a40), the sentinel
//             holds no value (140333cd0 constructs nothing).
//   ctor      not in Phase 1 (no caller found in the Phase 1 functions); the class is named after
//             its dtor until the ctor is found.
#pragma once

#include <cstdint>

namespace def::model {

// Record the list elements point to: two pointers that the helper zeroes.
struct SdctlIndexRecord {
    void* a; // 0x00 zeroed when detaching fully
    void* b; // 0x08 always zeroed
};

struct PtrListNode {
    PtrListNode* next;        // 0x00
    PtrListNode* prev;        // 0x08
    SdctlIndexRecord* value;  // 0x10
};

struct PtrListRep {
    std::int32_t refs;   // 0x00
    PtrListNode* head;   // 0x08 sentinel
    std::uint64_t size;  // 0x10

    PtrListRep(const PtrListRep& other); // 140333ce0 (copy for copy-on-write)
    // Port only: an empty rep, as the first half of 140333ce0 builds it (the original's own
    // empty-rep ctor is outside Phase 1).
    PtrListRep();
    ~PtrListRep(); // 140333a40
    PtrListRep& operator=(const PtrListRep&) = delete;

    // 140333ac0: insert value before `where`, return the new node
    PtrListNode* insert(PtrListNode* where, SdctlIndexRecord* const& value);
};

// Iterator over PtrListRep nodes (VS2010 list iterator: one node pointer).
struct PtrListIter {
    PtrListNode* node;
    SdctlIndexRecord*& operator*() const;      // 1403337e0 (node + 0x10)
    PtrListIter& operator++();                 // 140333800
    PtrListIter operator++(int);               // 140333f00
    bool operator!=(const PtrListIter& o) const; // 1403337a0
};

// Handle (8 bytes): pointer to the shared rep.
class PtrListHandle {
public:
    // Port only: adopt a rep (the original's ctor is outside Phase 1).
    explicit PtrListHandle(PtrListRep* r) : rep(r) {}
    ~PtrListHandle();                // 140333650
    PtrListHandle(const PtrListHandle&) = delete;
    PtrListHandle& operator=(const PtrListHandle&) = delete;

    void detach();                   // 140333820 (copy when shared)
    PtrListIter begin();             // 140333670
    PtrListIter end();               // 1403336b0

    PtrListRep* rep; // 0x00
};
static_assert(sizeof(PtrListHandle) == 8);

class Cls_1403331d0 {
public:
    // Port only: the original's ctor is outside Phase 1.
    Cls_1403331d0() : list(nullptr), current(nullptr) {}
    ~Cls_1403331d0();
    Cls_1403331d0(const Cls_1403331d0&) = delete;
    Cls_1403331d0& operator=(const Cls_1403331d0&) = delete;

    // 140333230: zero the records (a only when full)
    void detach_records(bool full);
    // 140333190: scalar deleting dtor
    static void destroy(Cls_1403331d0* p, unsigned flags);

    PtrListHandle* list;        // 0x00 owned handle (deleted by the dtor)
    SdctlIndexRecord* current;  // 0x08
};
static_assert(sizeof(Cls_1403331d0) == 0x10);

// 140333870: release one reference to a rep; the last one deletes it.
void release(PtrListRep* rep);

} // namespace def::model
