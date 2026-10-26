// DYNARY (the 0x78 curve/table object, layout/dynary.hpp): ctor, dtor and the scalar deleting dtor
// of the owned object at 0x68. p1-06 owns these three; the template init 14012d2a0 and the array
// release 14012f480 are key-common's (defmodel/dynary_init.hpp).
#include <defmodel/layout/dynary.hpp>

#include <defmodel/dynary_init.hpp>

namespace def::model {

// @src 14012d050  ctor
DYNARY::DYNARY()
{
    u58 = 0;
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        var[i] = 0;
        n[i] = 0;
    }
    values = nullptr;
    axis[2] = nullptr;
    axis[1] = nullptr;
    axis[0] = nullptr;
    u68 = nullptr;
    dynary_init(this, 0); // 14012d2a0
}

// @src 14012f410  dtor
// 14013c170 (map row p1-04): scalar deleting dtor of the owned object at 0x68, the delete below; it runs
//                 14012f410 on it, so the object is another DYNARY
DYNARY::~DYNARY()
{
    dynary_free_arrays(this); // 14012f480
    if (u68 != nullptr) {     // 14012f42e tests the pointer twice
        delete static_cast<DYNARY*>(u68);
    }
}

} // namespace def::model
