#include <defrt/clock.hpp>
#include <defrt/port.hpp>

#include <atomic>
#include <cstdio>
#include <cstdlib>

namespace def::rt {

void not_ported(std::uint64_t address, const char* file, int line)
{
    std::fflush(stdout);
    std::fprintf(stderr, "def_pre port: function 0x%llx not ported (%s:%d)\n",
                 static_cast<unsigned long long>(address), file, line);
    std::abort();
}

void omitted(std::uint64_t address, const char* why, const char* file, int line)
{
    std::fflush(stdout);
    std::fprintf(stderr, "def_pre port: function 0x%llx omitted: %s (%s:%d)\n",
                 static_cast<unsigned long long>(address), why, file, line);
    std::abort();
}

namespace {

std::atomic<std::int64_t>& pinned()
{
    static std::atomic<std::int64_t> value = [] {
        const char* env = std::getenv("DEF_PORT_FIXED_TIME");
        return env && *env ? static_cast<std::int64_t>(std::strtoll(env, nullptr, 10))
                           : std::int64_t{-1};
    }();
    return value;
}

} // namespace

std::int64_t now()
{
    std::int64_t p = pinned().load();
    return p >= 0 ? p : static_cast<std::int64_t>(std::time(nullptr));
}

std::tm local_now()
{
    std::time_t t = static_cast<std::time_t>(now());
    std::tm out{};
#ifdef _WIN32
    localtime_s(&out, &t);
#else
    localtime_r(&t, &out);
#endif
    return out;
}

void pin_clock(std::int64_t unix_seconds) { pinned().store(unix_seconds); }

} // namespace def::rt
