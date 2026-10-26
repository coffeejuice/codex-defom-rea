// defrt/clock.hpp: the port's only source of wall-clock time (plan §4, "Dates").
//
// Every ported function that reads the time (the original calls time(), _strdate, _strtime,
// GetLocalTime, the Fortran DATE_AND_TIME ...) goes through now(), so that tests can pin it.
// Setting DEF_PORT_FIXED_TIME=<unix seconds> in the environment pins it for a whole process
// (used by tier-2/3 runs together with the comparator's date masks).
#pragma once

#include <cstdint>
#include <ctime>

namespace def::rt {

// Seconds since 1970-01-01 UTC, as time() returns.
std::int64_t now();

// Local broken-down time of now(), as localtime() returns.
std::tm local_now();

// Pin the clock (tests). pin_clock(-1) restores the real clock.
void pin_clock(std::int64_t unix_seconds);

} // namespace def::rt
