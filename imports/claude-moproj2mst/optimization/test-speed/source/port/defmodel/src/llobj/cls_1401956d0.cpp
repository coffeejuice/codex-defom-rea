// Cls_1401956d0 (defmodel/llobj/cls_1401956d0.hpp).
#include <defmodel/llobj/cls_1401956d0.hpp>

#include <defmodel/llobj/ext.hpp>
#include <defmodel/port.hpp>

namespace def::model {

// ctor 1401956d0: defined in src/llobj_ctor/member_ctors.cpp (kw-g2obj)

// @src 1401958b0  dtor: reset, the seven Cls_140269250 from the last, the two p1-06 objects;
// the DYNARY list at 0 is destroyed last by C++ (140136510)
Cls_1401956d0::~Cls_1401956d0()
{
    reset();
    for (int k = 6; k >= 0; --k)
        llobj::ext::f140269320(s048[k]);
    llobj::ext::f140257c60(s030);
    llobj::ext::f140257c60(s018);
}

// @src 140195a40  reset 0x18 (1402581b0), delete the tables (140136910), reset the seven
// Cls_140269250 (1402699c0) in the order 0x48, 0x78, 0xa8, 0x168, 0xd8, 0x108, 0x138
void Cls_1401956d0::reset()
{
    llobj::ext::f1402581b0(s018);
    tables.clear();
    static constexpr int order[7] = {0, 1, 2, 6, 3, 4, 5};
    for (int k : order)
        llobj::ext::f1402699c0(s048[k]);
}

} // namespace def::model
