// defmodel/members/custom_state_var.hpp: the list held by DB_STEP_DATA+0x33c0 (Cls_14032f100 /
// Cls_14032f920) and its element type (packet p1-10).
//
// The original hand-rolls copy-on-write lists: a handle (8 bytes) points to a heap
// representation {refcount, VS2010 std::list sentinel, size}. Two instantiations live here:
//
//   StrListRep / StrListHandle  list<std::string>   rep 140330130, handle 14032f7d0
//   VarListRep                  list<CustomVarDef>  rep 1403303a0, handle Cls_14032f920
//
// CustomVarDef (Cls_14032ece0, the 0x38-byte list value): an int, a std::string and a string
// list. It sits next to the StateVarCustom code, so it probably describes a user-defined state
// variable (name and component names); not confirmed.
//
// Layout: only the handles are embedded in model classes (Cls_14032f100 at DB_STEP_DATA+0x33b8),
// so only they keep the binary's size. Inside the heap objects std::string is the modern one
// (VS2010 debug std::string was 0x28 bytes); no ported code reads those offsets.
#pragma once

#include <cstdint>
#include <string>

namespace def::model {

struct StrListNode {
    StrListNode* next;  // 0x00
    StrListNode* prev;  // 0x08
    std::string value;  // 0x10
    ~StrListNode();
};

struct StrListRep {
    std::int32_t refs;   // 0x00 140001000 sets 1
    StrListNode* head;   // 0x08 sentinel (holds a constructed, empty string: 140330820)
    std::uint64_t size;  // 0x10

    StrListRep();
    ~StrListRep();
    void clear();
};

// Cls_14032f7d0: handle of a copy-on-write list<std::string>.
class StrListHandle {
public:
    StrListHandle();
    ~StrListHandle();
    StrListHandle(const StrListHandle&) = delete;
    StrListHandle& operator=(const StrListHandle&) = delete;

    void clear();

    StrListRep* rep; // 0x00
};
static_assert(sizeof(StrListHandle) == 8);

// Cls_14032ece0: value of the DB_STEP_DATA+0x33c0 list.
class CustomVarDef {
public:
    CustomVarDef();
    ~CustomVarDef();
    CustomVarDef(const CustomVarDef&) = delete;
    CustomVarDef& operator=(const CustomVarDef&) = delete;

    void reset();
    void clear_items();
    void set_id(std::int32_t v);
    void set_name(const std::string& s);

    std::int32_t id;      // 0x00
    std::string name;     // 0x08 (0x28 bytes in the original)
    StrListHandle items;  // 0x30
};

struct VarListNode {
    VarListNode* next;   // 0x00
    VarListNode* prev;   // 0x08
    CustomVarDef value;  // 0x10
    VarListNode() : next(nullptr), prev(nullptr), value() {}
    ~VarListNode();
};

struct VarListRep {
    std::int32_t refs;   // 0x00
    VarListNode* head;   // 0x08 sentinel (holds a constructed CustomVarDef: 140330880)
    std::uint64_t size;  // 0x10

    VarListRep();
    ~VarListRep();
};

// Release one reference; the last one deletes the representation (14032fdb0 / 14032fe10).
void release(StrListRep* rep);
void release(VarListRep* rep);

} // namespace def::model
