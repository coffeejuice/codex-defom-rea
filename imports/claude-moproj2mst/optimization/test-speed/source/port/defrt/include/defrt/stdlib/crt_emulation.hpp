// defrt/stdlib/crt_emulation.hpp: how the original process ends when it hits a CRT failure
// (packet p1-01; taken over from def_pre/crt_emulation.hpp of the -m CLI port).
//
// Some branches of the original end in an access violation, a CRT parameter check or a failed
// stack cookie (a NULL argv entry dereferenced, fprintf on a NULL FILE*, a 1024-byte option value
// copied into a 1024-byte stack buffer). The port does not reproduce the undefined behaviour; it
// ends the process with the same NTSTATUS exit code and, like the crash, without flushing stdio
// buffers or running atexit handlers. Every call site cites the faulting instruction.
#pragma once

#include <cstdint>

namespace def::rt {

inline constexpr std::uint32_t kStatusAccessViolation = 0xC0000005u;     // read through NULL
inline constexpr std::uint32_t kStatusInvalidCrtParameter = 0xC0000417u; // _invalid_parameter
inline constexpr std::uint32_t kStatusStackBufferOverrun = 0xC0000409u;  // __report_gsfailure

// Tier-1 tests install a hook that throws, to observe the call instead of ending the process.
using AbnormalExitHook = void (*)(std::uint32_t status);
void set_abnormal_exit_hook(AbnormalExitHook hook);

// Ends the process with `status`, without running atexit handlers or flushing stdio.
[[noreturn]] void abnormal_exit(std::uint32_t status);

} // namespace def::rt
