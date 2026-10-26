// defrt/ftn/geoutl_pending.hpp: formerly the FTN_geoutl kernels whose declarations had not landed
// (1404d1b1c 1404d1ed7 1404d3e13 1404e37c4 1404d1d05 1404d203b). All six are now declared in
// geoutl_proj.hpp with their bodies (6cvah9, 6cdff60); this header only forwards to geoutl.hpp
// for the files that still include it. Include <defrt/ftn/geoutl.hpp> in new code.
#pragma once

#include <defrt/ftn/geoutl.hpp>
