// Cls_14017a6c0: the DB_STEP_DATA list of LLOBJ_DATA (layout/cls_14017a6c0.hpp).
#include <defmodel/layout/cls_14017a6c0.hpp>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/ext.hpp>

namespace def::model {

// @src 14017a6c0  ctor: owner, then reset
Cls_14017a6c0::Cls_14017a6c0(DB_STEP_DATA* owner_)
{
    owner = owner_;
    reset();
}

// @src 14017a740  dtor (-> 14017ab70)
Cls_14017a6c0::~Cls_14017a6c0() { clear(); }

// @src 14017a700  head = tail = 0, count = 0
void Cls_14017a6c0::reset()
{
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 14017aa80  n-th node, 1-based
LLOBJ_DATA* Cls_14017a6c0::at(std::int32_t n) const
{
    std::int32_t i = 0;
    LLOBJ_DATA* p = head;
    while (p != nullptr && (i = i + 1, i < n))
        p = p->next;
    return p;
}

// @src 14017ab70  delete all nodes, reset
void Cls_14017a6c0::clear()
{
    LLOBJ_DATA* next = head;
    LLOBJ_DATA* p;
    while (p = next, p != nullptr) {
        next = p->next;
        if (p != nullptr)
            llobj::ext::llobj_delete(p);  // 14019a290(p, 1)
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

} // namespace def::model
