// defrt/ftn/common_140b84720.hpp: the Fortran COMMON at 140b84720..140b84730, owned at the lowest
// layer that uses it (owner decision: defrt, def::rt::geoutl). Every other name for these
// addresses is a reference to this object: def::model::g_common_140b84720 (p1-09, the 2D curve
// kernels 14032b34b / 14032c4a0) and def::model::dat_140b84720..30 (p1-10, the extent 140345f5b).
// The geometry kernels of defrt (1404e305a and neighbours) read the same object.
#pragma once

namespace def::rt::geoutl {

// @data 140b84720  Fortran COMMON (bss) of three doubles. A zero entry is replaced by its
// default on first use in the 2D curve kernels: 1e-6, 1e-6, 1.0.
struct Common140b84720 {
    double u00;           // 140b84720 segment-length tolerance (14032b34b); copy of 140b74428
    double u08;           // 140b84728 radius / point-merge / projection tolerance; copy of 140b74430
    double u10;           // 140b84730 scale: the extent 140345f5b, the characteristic length
};
inline Common140b84720 g_common_140b84720{};

} // namespace def::rt::geoutl
