// Cls_14042a870: surface boundary-edge helper of _OBJ_GEO3D (packet p1-14).
// Layout and role: defmodel/layout/cls_14042a870.hpp.
#include <defmodel/layout/cls_14042a870.hpp>

namespace def::model {

// @src 14042a7e0  ctor: u10 (140159860), u18 (14042b830), u58 (14042bd30), u98 (14042c230),
//                 ua0, ua8 (140159860), then init (14042a900)
Cls_14042a870::Cls_14042a870() : u00(nullptr), u08(0), pad_0c{}
{
    init();
}

// @src 14042a870  dtor: clear (14042a950), then ua8, ua0 (140159930), u98 (14042c2a0),
//                 u58 (14042b6b0), u18 (14042b690), u10 (140159930): the reverse member order
Cls_14042a870::~Cls_14042a870()
{
    clear();
}

// @src 14042a900  init
void Cls_14042a870::init()
{
    u00 = nullptr;
    u08 = 3;
    u10.clear();  // 14015a620
    reset();      // 14042b0c0
}

// @src 14042a950  clear
void Cls_14042a870::clear()
{
    u10.clear();  // 14015a620
    reset();      // 14042b0c0
}

// @src 14042b0c0  reset
void Cls_14042a870::reset()
{
    u18.value.clear();  // 14042bbc0
    u58.value.clear();  // 14042c0c0
    u98.clear();        // 140431130
    ua0.clear();        // 14015a620
    ua8.clear();        // 14015a620
}

} // namespace def::model
