// Message print of the model code (14039d3a0 / 14039d570): format into a 4096-byte buffer, append
// "\n" (DAT_1406111c4), flush stdout, write, flush all streams. Packet p-dbread (called by the
// realloc helpers 140256e20 / 1402571b0 on failure).
#include <defdbio/dbread/dbread.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace def::dbio::dbread {

// @src 14039d570  vsprintf + "\n" to stdout (fflush(stdout), fputs, fflush(NULL))
void fun_14039d570(int unused, const char* fmt, std::va_list ap)
{
    (void)unused;
    char buf[4096];
    rt::crt::vsnprintf(buf, sizeof buf, fmt, ap); // FUN_14048104c: vsprintf, unbounded in the original
    std::strcat(buf, "\n");
    std::fflush(stdout);
    std::fputs(buf, stdout);
    std::fflush(nullptr);
}

// @src 14039d3a0  message(fmt, a, b, n): the three arguments go on as a va_list
void fun_14039d3a0(const char* fmt, const char* a, const char* b, std::int32_t n)
{
    // the original spills its three register arguments next to each other and passes their
    // address as the va_list; a variadic forwarder reproduces that
    struct Fwd {
        static void call(const char* f, ...)
        {
            std::va_list ap;
            va_start(ap, f);
            fun_14039d570(0, f, ap);
            va_end(ap);
        }
    };
    Fwd::call(fmt, a, b, n);
}

} // namespace def::dbio::dbread
