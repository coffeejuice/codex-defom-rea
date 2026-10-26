// Cls_14017e740 (defmodel/llobj/cls_14017e740.hpp).
#include <defmodel/llobj/cls_14017e740.hpp>

#include <cstdlib>

namespace def::model {

// @src 14017e740  ctor: u028 = 0, init
Cls_14017e740::Cls_14017e740()
{
    u028 = nullptr;
    init();
}

// @src 14017e720  dtor (-> 14017e800)
Cls_14017e740::~Cls_14017e740() { free_arrays(); }

// @src 14017e770  u028 = owner, init
void Cls_14017e740::set_owner(void* owner)
{
    u028 = owner;
    init();
}

// @src 14017e7b0  zero the fields (not u028)
void Cls_14017e740::init()
{
    u000 = 0;
    u008 = nullptr;
    u010 = 0;
    u018 = nullptr;
    u020 = nullptr;
}

// @src 14017e800  free the three arrays, zero the counts
void Cls_14017e740::free_arrays()
{
    if (u008 != nullptr)
        std::free(u008);
    u008 = nullptr;
    u000 = 0;
    if (u018 != nullptr)
        std::free(u018);
    u018 = nullptr;
    if (u020 != nullptr)
        std::free(u020);
    u020 = nullptr;
    u010 = 0;
}

} // namespace def::model
