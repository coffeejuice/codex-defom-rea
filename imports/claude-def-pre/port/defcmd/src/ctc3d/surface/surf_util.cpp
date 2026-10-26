// Fortran STOP for the group "surface" (surf_util.hpp).
#include "surf_util.hpp"

#include <defrt/ftn/fio.hpp>

#include <cstdlib>

namespace def::cmd::ctc3d::surf {

void ftn_stop()
{
    def::rt::ftn::fio::reset(); // 1404b8a7c: the runtime closes its units
    std::exit(0);               // 1404b9e88(0)
}

} // namespace def::cmd::ctc3d::surf
