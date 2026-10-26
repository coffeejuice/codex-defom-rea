// PhasePairList: DB_STEP_DATA+0x1358 list of PhasePair (packet p1-10).
// Layout: defmodel/layout/phase_pair_list.hpp.
#include <defmodel/layout/phase_pair_list.hpp>

#include <defmodel/layout/phase_pair.hpp>

namespace def::model {

// @src 140339c90  ctor: owner at 0x18, then reset
PhasePairList::PhasePairList(DB_STEP_DATA* owner_)
{
    owner = owner_;
    reset();
}

// @src 140339d10  dtor: clear
PhasePairList::~PhasePairList() { clear(); }

// @src 140339cd0  reset
void PhasePairList::reset()
{
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 14033a120  clear: delete each node through its scalar deleting dtor, then reset
// @src 14033a990  (PhasePair scalar deleting dtor: dtor 140334490 + free, `delete node`)
void PhasePairList::clear()
{
    PhasePair* p = head;
    while (p != nullptr) {
        PhasePair* next = p->next; // node+0x770
        if (p != nullptr)
            delete p;
        p = next;
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

} // namespace def::model
