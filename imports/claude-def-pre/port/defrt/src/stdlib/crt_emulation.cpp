#include <defrt/stdlib/crt_emulation.hpp>

#include <cstdlib>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace def::rt {

namespace {
AbnormalExitHook g_abnormal_exit_hook = nullptr;
}

void set_abnormal_exit_hook(AbnormalExitHook hook) { g_abnormal_exit_hook = hook; }

void abnormal_exit(std::uint32_t status)
{
    if (g_abnormal_exit_hook) {
        g_abnormal_exit_hook(status); // a test hook throws; if it returns, terminate anyway
    }
#ifdef _WIN32
    ::TerminateProcess(::GetCurrentProcess(), status);
#endif
    std::_Exit(static_cast<int>(status));
}

} // namespace def::rt
