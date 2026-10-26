// Cls_140197a50 (defmodel/llobj/cls_140197a50.hpp).
#include <defmodel/llobj/cls_140197a50.hpp>

#include <cstdlib>

#include <defmodel/port.hpp>

namespace def::model {

// ctor 140197a50: defined in src/llobj_ctor/member_ctors.cpp (kw-g2obj)

// @src 140197a80  dtor (-> 140197b40)
Cls_140197a50::~Cls_140197a50() { free_arrays(); }

// @src 140197b40  when count > 0: count = 0 and free the three arrays (pointers kept)
// @src 140199c60  0 < *p (absorbed)
void Cls_140197a50::free_arrays()
{
    if (0 < count) {
        count = 0;
        std::free(u08);
        std::free(u10);
        std::free(u18);
    }
}

} // namespace def::model
