// m2m/data/diestress.hpp: SfOprDiestressProperties payload (owner P16). Filled by the post-load
// reader 0x1402b95a0 (DieStress.Wzd). The Diestress writer 0x1402b5410 only checks that the
// properties exist; no MST byte depends on this value.
#pragma once

namespace m2m::data {

struct DiestressData {                 // equipment block at PROPS+0x38 (0x90, ctor 0x1402a2a20)
    int stepNumber = -1;               // +0x80  DieStressWizard attribute nStepNumber
};

}  // namespace m2m::data
