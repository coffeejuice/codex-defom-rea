// Copy-on-write lists of DB_STEP_DATA+0x33c0 and their value type (packet p1-10).
// See defmodel/members/custom_state_var.hpp. Heap blocks come from operator new (malloc) and
// go back through the scalar deleting dtors (free), i.e. `new` / `delete` here.
#include <defmodel/members/custom_state_var.hpp>

namespace def::model {

// ---- list<std::string> ------------------------------------------------------------------------

// @src 1403307b0  StrListNode dtor (destroys the string)
// @src 140330730  StrListNode scalar deleting dtor (dtor + free: `delete node`)
StrListNode::~StrListNode() {}

// @src 140330130  StrListRep ctor: refs = 1, sentinel node, size = 0
// @src 140330820  sentinel node ctor (constructs its string)
StrListRep::StrListRep()
{
    refs = 1; // 140001000
    head = new StrListNode{nullptr, nullptr, std::string()};
    head->prev = head;
    head->next = head->prev;
    size = 0;
}

// @src 1403301e0  StrListRep dtor: delete every node, then the sentinel
// @src 14032fe70  (scalar deleting dtor of the rep: `delete rep`)
StrListRep::~StrListRep()
{
    StrListNode* p = head->next;
    while (p != head) {
        StrListNode* next = p->next;
        if (p != nullptr)
            delete p;
        p = next;
    }
    if (head != nullptr)
        delete head;
}

// @src 140330ad0  StrListRep clear in place
void StrListRep::clear()
{
    size = 0;
    StrListNode* p = head->next;
    while (p != head) {
        StrListNode* next = p->next;
        if (p != nullptr)
            delete p;
        p = next;
    }
    head->prev = head;
    head->next = head->prev;
}

// @src 14032fdb0  release a list<std::string> rep
void release(StrListRep* rep)
{
    if (--rep->refs == 0 && rep != nullptr) // 140001010
        delete rep;
}

// @src 14032f7d0  StrListHandle ctor
StrListHandle::StrListHandle() { rep = new StrListRep(); }

// @src 14032f840  StrListHandle dtor (Ghidra library match "std::_Timevec::~_Timevec")
StrListHandle::~StrListHandle() { release(rep); }

// @src 14032ff10  StrListHandle clear: in place when unshared, else detach to a new empty rep
void StrListHandle::clear()
{
    if (rep->refs == 1) {
        rep->clear();
    } else {
        --rep->refs; // 140001010, result ignored
        rep = new StrListRep();
    }
}

// ---- CustomVarDef -----------------------------------------------------------------------------

// @src 14032ece0  CustomVarDef ctor (string 140005040, items 14032f7d0, then set_id(0))
CustomVarDef::CustomVarDef() : id(0), name(), items() { set_id(0); }

// @src 14032ee00  CustomVarDef dtor: reset, then items and name are destroyed (member order)
CustomVarDef::~CustomVarDef() { reset(); }

// @src 14032ee50  reset: name = "" (str 14054ece7), clear the items
void CustomVarDef::reset()
{
    std::string empty(""); // 1400066a0 from DAT_14054ece7
    set_name(empty);
    clear_items();
}

// @src 14032eeb0  clear the items list
void CustomVarDef::clear_items() { items.clear(); }

// @src 14032eee0  set the int at 0
void CustomVarDef::set_id(std::int32_t v) { id = v; }

// @src 14032ef20  set the name (std::string assign 1400066f0)
void CustomVarDef::set_name(const std::string& s) { name.assign(s); }

// ---- list<CustomVarDef> -----------------------------------------------------------------------

// @src 1403307d0  VarListNode dtor (destroys the value)
// @src 140330770  VarListNode scalar deleting dtor
VarListNode::~VarListNode() {}

// @src 1403303a0  VarListRep ctor
// @src 140330880  sentinel node ctor (constructs a CustomVarDef)
VarListRep::VarListRep()
{
    refs = 1;
    head = new VarListNode();
    head->prev = head;
    head->next = head->prev;
    size = 0;
}

// @src 140330450  VarListRep dtor
// @src 14032feb0  (scalar deleting dtor of the rep)
VarListRep::~VarListRep()
{
    VarListNode* p = head->next;
    while (p != head) {
        VarListNode* next = p->next;
        if (p != nullptr)
            delete p;
        p = next;
    }
    if (head != nullptr)
        delete head;
}

// @src 14032fe10  release a list<CustomVarDef> rep
void release(VarListRep* rep)
{
    if (--rep->refs == 0 && rep != nullptr)
        delete rep;
}

} // namespace def::model
