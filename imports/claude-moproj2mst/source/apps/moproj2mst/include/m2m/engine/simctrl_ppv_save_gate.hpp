// Isolated native SimCtrl.PPV writer entry gate; this is not public Save wiring.
#pragma once

#include <cstddef>

namespace m2m::engine::simctrl_ppv {

// Native 0x140307410 calls QListData::isEmpty() on the local list. The writer
// runs only when local points are present; shared points and flags do not affect
// this entry decision.
inline bool ppvWriterGateAllows(std::size_t localPointCount) noexcept
{
    return localPointCount != 0;
}

} // namespace m2m::engine::simctrl_ppv
