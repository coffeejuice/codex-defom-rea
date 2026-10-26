// Cls_1403331d0: SdctlGList+0x30 helper and its copy-on-write pointer list (packet p1-10).
// See defmodel/members/sdctl_index.hpp. operator new (malloc) / free pairs of the original are
// new / delete here.
#include <defmodel/members/sdctl_index.hpp>

namespace def::model {

// ---- iterator ---------------------------------------------------------------------------------

// @src 1403337e0  iterator deref: node + 0x10
SdctlIndexRecord*& PtrListIter::operator*() const { return node->value; }

// @src 140333800  prefix ++
PtrListIter& PtrListIter::operator++()
{
    node = node->next;
    return *this;
}

// @src 140333f00  postfix ++ (copies through 140333c80)
PtrListIter PtrListIter::operator++(int)
{
    PtrListIter old = *this; // 140333c80
    node = node->next;
    return old;
}

// @src 1403337a0  iterator != (Ghidra library match "ATL::CFileTimeSpan::operator!=")
bool PtrListIter::operator!=(const PtrListIter& o) const { return node != o.node; }

// ---- rep --------------------------------------------------------------------------------------

PtrListRep::PtrListRep()
{
    // part of @src 140333ce0 (its empty-list half)
    refs = 1;
    head = new PtrListNode; // 140333cd0 constructs nothing
    head->prev = head;
    head->next = head->prev;
    size = 0;
}

// @src 140333ce0  copy ctor used by detach: refs = 1, empty list, then every value appended
// @src 140333cd0  (sentinel "ctor": returns its argument)
// @src 140333850  (iterator from node)
// @src 140333c80  (iterator copy)
PtrListRep::PtrListRep(const PtrListRep& other)
{
    refs = 1; // 140001000
    head = new PtrListNode;
    head->prev = head;
    head->next = head->prev;
    size = 0;
    PtrListIter it{other.head->next};
    PtrListIter last{other.head};
    PtrListIter where{head};
    while (it != last) {
        SdctlIndexRecord*& v = *(it++);
        insert(where.node, v);
    }
}

// @src 140333ac0  insert before `where` (node from operator new(0x18), value set by 140333ca0)
// @src 140333ca0  (node value ctor)
PtrListNode* PtrListRep::insert(PtrListNode* where, SdctlIndexRecord* const& value)
{
    PtrListNode* n = new PtrListNode;
    n->value = value;
    n->next = where;
    n->prev = where->prev;
    where->prev->next = n;
    where->prev = n;
    size = size + 1;
    return n;
}

// @src 140333a40  rep dtor: free every node, then the sentinel
// @src 1403338d0  (scalar deleting dtor of the rep)
PtrListRep::~PtrListRep()
{
    PtrListNode* p = head->next;
    while (p != head) {
        PtrListNode* next = p->next;
        delete p;
        p = next;
    }
    delete head;
}

// @src 140333870  release a rep
void release(PtrListRep* rep)
{
    if (--rep->refs == 0 && rep != nullptr) // 140001010
        delete rep;
}

// ---- handle -----------------------------------------------------------------------------------

// @src 140333650  handle dtor
// @src 140333300  (scalar deleting dtor of the handle)
PtrListHandle::~PtrListHandle() { release(rep); }

// @src 140333820  detach when shared
// @src 140333910  (drop the shared reference, copy the rep)
void PtrListHandle::detach()
{
    if (1u < static_cast<std::uint32_t>(rep->refs)) {
        --rep->refs; // 140001010, result ignored
        rep = new PtrListRep(*rep);
    }
}

// @src 140333670  begin (detaches)
PtrListIter PtrListHandle::begin()
{
    detach();
    return PtrListIter{rep->head->next};
}

// @src 1403336b0  end (detaches)
PtrListIter PtrListHandle::end()
{
    detach();
    return PtrListIter{rep->head};
}

// ---- Cls_1403331d0 ----------------------------------------------------------------------------

// @src 140333230  zero the records the helper refers to
void Cls_1403331d0::detach_records(bool full)
{
    if (current != nullptr) {
        if (full)
            current->a = nullptr;
        current->b = nullptr;
    }
    if (list != nullptr) {
        PtrListIter it = list->begin();
        while (it != list->end()) {
            if (full)
                (*it)->a = nullptr;
            (*it)->b = nullptr;
            ++it;
        }
    }
}

// @src 1403331d0  dtor
Cls_1403331d0::~Cls_1403331d0()
{
    detach_records(true);
    if (list != nullptr)
        delete list; // 140333300(list, 1)
}

// @src 140333190  scalar deleting dtor
void Cls_1403331d0::destroy(Cls_1403331d0* p, unsigned flags)
{
    p->~Cls_1403331d0();
    if ((flags & 1) != 0)
        ::operator delete(p);
}

} // namespace def::model
