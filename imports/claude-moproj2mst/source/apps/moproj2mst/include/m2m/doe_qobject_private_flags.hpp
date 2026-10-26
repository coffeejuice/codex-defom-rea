#pragma once

#include <cstdint>

namespace m2m::doe {

// @src 0x67131224-0x6713122f, QObject private initializer reached by Report.
// Isolated word projection only: caller supplies the previous private+20 word.
// It does not supply an unavailable word, construct QObject, or retain Qt refs.
std::uint32_t isolatedQObjectPrivateFlags(std::uint32_t previousWord) noexcept;

} // namespace m2m::doe
