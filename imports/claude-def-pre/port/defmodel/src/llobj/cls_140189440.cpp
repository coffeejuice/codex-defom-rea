// Cls_140189440 (defmodel/llobj/cls_140189440.hpp).
#include <defmodel/llobj/cls_140189440.hpp>

#include <cstdlib>

#include <defmodel/llobj/ext.hpp>
#include <defmodel/port.hpp>

namespace def::model {

// ctor 140189440: defined in src/llobj_ctor/member_ctors.cpp (kw-g2obj)

// @src 140189520  dtor: reset, then the three Cls_140269250 from the last
Cls_140189440::~Cls_140189440()
{
    reset();
    llobj::ext::f140269320(s128);
    llobj::ext::f140269320(s0f8);
    llobj::ext::f140269320(s040);
}

// @src 140189ad0  reset the first Cls_140269250 (1402699c0), free the two arrays, u0d8 = 0
void Cls_140189440::reset()
{
    llobj::ext::f1402699c0(s040);
    if (u0e0 != nullptr)
        std::free(u0e0);
    u0e0 = nullptr;
    if (u0e8 != nullptr)
        std::free(u0e8);
    u0e8 = nullptr;
    u0d8 = 0;
}

} // namespace def::model
