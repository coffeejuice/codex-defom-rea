// PhasePair: link stubs (layout only; bodies come with the owning packet).
// Layout: defmodel/layout/phase_pair.hpp, docs/re/model-layout.md.
#include <defmodel/layout/phase_pair.hpp>

#include <defmodel/port.hpp>

namespace def::model {

// ctor 140333f60: packet kw-g57, src/phase/phase_pair_ctor.cpp

// @src 140334490  dtor (p1-10): the twelve curve members are destroyed in reverse declaration
// order, 0x6f0 first and 0x078 last, which is the order of the 14012f410 calls in the original.
PhasePair::~PhasePair() {}

} // namespace def::model
