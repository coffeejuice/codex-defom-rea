// Stub of the PNTEDG failure-message hook (geoutl_d41f8.cpp) until a Fortran WRITE runtime exists in
// defrt. Its four FTN_geoutl callees now come from the shared library (geoutl_basic/poly/tri.cpp).
#include <defrt/port.hpp>

namespace def::rt::geoutl {

// part of @src 1404d41f8: failure message of PNTEDG (Fortran WRITE, not ported; reached only when
// every edge distance is NaN or >= 1e10)
void fun_1404d41f8_failure_write() { DEF_NOT_PORTED(0x1404d46e6); }

} // namespace def::rt::geoutl
