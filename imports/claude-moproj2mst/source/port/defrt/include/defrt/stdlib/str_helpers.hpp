// defrt/stdlib/str_helpers.hpp: small std::string helpers of DEF_PRE_64's C++ layer
// (0x1401b0870..0x1401b1bbf) that several command packets call (BRDEXT p8-05, DEFAMG p8-07,
// DEFINT/REMESH p8-08, the GENDB menu p4-01). Ported by packet p8-05.
// def_pre/src/menu7.cpp (p4-01) has its own copies of 1401b0870 / 1401b0940 that switch to
// these at integration.
#pragma once

#include <cstdint>
#include <string>

namespace def::rt {

// FUN_1401b0870: sscanf(s, "%d") into a value that starts at 0 (str 140524d18); s by value.
std::int32_t str_to_int(std::string s);

// FUN_1401b0940: does s start with p (strlen(p) characters)? exact (third argument non-zero):
// strncmp; otherwise both sides with A-Z folded to lower case (only A-Z). s by value.
bool str_starts_with(std::string s, const char* p, bool exact);

// FUN_1401b1060: s without leading and trailing blanks (\t \n \v \f \r, space, '?'); an all-blank
// string is returned whole (first stays 0, last stays n - 1). A malloc(n + 1) is leaked. s by value.
std::string str_trim(std::string s);

// Path-name wrapper of 0x28 bytes (one std::string), no RTTI.
//   ctor 1401b19c0 (takes the string by value), dtor 1401b1a10, stem 1401b1a30.
class Cls_1401b19c0 {
public:
    // @src 1401b19c0 (declared here, defined in str_helpers.cpp)
    explicit Cls_1401b19c0(std::string s);
    // @src 1401b1a10  dtor: destroys the string
    ~Cls_1401b19c0();

    // FUN_1401b1a30: basename without the last extension: the part after the last '\' or '/'
    // that precedes the last '.', up to (not including) that '.'; no '.' -> up to the end.
    std::string stem() const;

    std::string path;
};

} // namespace def::rt
