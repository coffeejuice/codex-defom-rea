// Cls_1401860a0 (defmodel/llobj/cls_1401860a0.hpp).
#include <defmodel/llobj/cls_1401860a0.hpp>

#include <cstring>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/port.hpp>

namespace def::model {

namespace {
DYNARY* dyn(std::byte* p) { return reinterpret_cast<DYNARY*>(p); }
} // namespace

// ctor 1401860a0: defined in src/llobj_ctor/member_ctors.cpp (kw-g2obj)

// @src 140186130  dtor: reset, then the sub-objects in reverse order
Cls_1401860a0::~Cls_1401860a0()
{
    reset();
    llobj::ext::dynary_dtor(dyn(s238));  // 14012f410
    llobj::ext::f14025ae70(s218);
    llobj::ext::f1402598a0(s200);
    llobj::ext::dynary_dtor(dyn(s140));
    llobj::ext::dynary_dtor(dyn(s0c0));
}

// @src 140186510  free the tables (14012f480) and reset the p1-06 objects; int at 0x278 = 0
void Cls_1401860a0::reset()
{
    llobj::ext::f14012f480(dyn(s0c0));
    llobj::ext::f14012f480(dyn(s140));
    llobj::ext::f140259c70(s200);
    llobj::ext::f14025ab20(s218);
    llobj::ext::f14012f480(dyn(s238));
    const std::int32_t zero = 0;
    std::memcpy(s238 + 0x40, &zero, sizeof zero);  // this + 0x278
}

} // namespace def::model
