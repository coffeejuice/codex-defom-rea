// Cls_140185c60 (defmodel/llobj/cls_140185c60.hpp).
#include <defmodel/llobj/cls_140185c60.hpp>

#include <defmodel/llobj/ext.hpp>
#include <defmodel/port.hpp>

namespace def::model {

// ctor 140185c60: defined in src/llobj_ctor/member_ctors.cpp (kw-g2obj)

// @src 140185ca0  dtor: 140185d10, base dtor 140329cb0
Cls_140185c60::~Cls_140185c60()
{
    fun_140185d10();
    llobj::ext::f140329cb0(this);
}

// @src 140185d10  empty
void Cls_140185c60::fun_140185d10() {}

} // namespace def::model
