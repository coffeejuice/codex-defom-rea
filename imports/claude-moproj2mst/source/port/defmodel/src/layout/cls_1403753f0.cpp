// Cls_1403753f0: "Furnace model" block of the simulation control (packet p1-10).
// Layout: defmodel/layout/cls_1403753f0.hpp; the other methods are in members/furnace.cpp.
#include <defmodel/layout/cls_1403753f0.hpp>

namespace def::model {

// @src 1403753f0  ctor: curve list (1401364a0), then init_name
Cls_1403753f0::Cls_1403753f0() : curves() { init_name(); }

// @src 140375430  dtor: release_curves, then the curve list dtor (140136510)
Cls_1403753f0::~Cls_1403753f0() { release_curves(); }

} // namespace def::model
