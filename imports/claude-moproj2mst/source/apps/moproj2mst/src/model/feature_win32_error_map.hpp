#pragma once

#include <cstdint>

namespace m2m::detail {
// Pure retained-initial-image profile of1414740ec. The45-row table at143b5e160
// is writable native.data: this is not a current original-module observation.
// Original EXE SHA256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc
// Table SHA256 dc5aaf854d47b0fa4deddacb6ff02e9a7d99cb1ac06c16e03e587d4599067fbe
// Explicit input only; never reads LastError/FLS/errno or writes caller state.
// The returned numeric CRT code is not a live errno store or Close admission.
std::int32_t mapRetainedFeatureWin32Error(std::uint32_t error) noexcept;
} // namespace m2m::detail
