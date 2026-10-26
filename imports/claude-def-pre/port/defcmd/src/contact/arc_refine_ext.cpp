// Forwarders from the arc refinement's names to the owners' functions: 1404d93ae (column sort,
// def::rt::geoutl, 6cvah9) and 14032beef (vector angle, OBJPOS interference packet p67-2).
#include <defcmd/arc_refine.hpp>

#include <defrt/ftn/geoutl.hpp>

#include "../ifd/d2/d2.hpp"

namespace def::cmd::ftn {

void fun_1404d93ae(double* a, const std::int32_t* key, const std::int32_t* n, const std::int32_t* dir,
                   const std::int32_t* ld)
{
    def::rt::geoutl::fun_1404d93ae(a, key, n, dir, ld);
}

void fun_14032beef(const double* a, const double* b, double* out)
{
    def::cmd::ifd::d2::fun_14032beef(const_cast<double*>(a), const_cast<double*>(b), out);
}

} // namespace def::cmd::ftn
