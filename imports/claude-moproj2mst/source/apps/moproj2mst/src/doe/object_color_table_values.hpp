#pragma once

#include <array>
#include <cstdint>

namespace m2m::datafiles { class TextFile; class TokenReader; }

namespace m2m::doe {

// Caller-supplied values, not the native process-global palette's ownership or
// unknown initial state. Native bases: +760,+8f0,+a80,+c10,+da0,+f30.
struct ObjectColorTableValues {
    using Color = std::array<std::uint8_t, 4>;
    using Table = std::array<Color, 100>;
    std::array<Table, 6> tables;
};

// Isolated cdeab0 value consumer. A null stream represents the independently
// observed fopen failure branch and leaves all destinations/reader untouched.
// A nonnull stream is borrowed after successful rt open: exact 100 attempts,
// persistent token destinations, immediate stores, then two trailing reads.
// Caller supplies initial tables and actual token-reader mode. No global owner,
// filename conversion, fopen/fclose, DB child, manager or public DBLIST binding.
bool readIsolatedObjectColorTableValues(datafiles::TextFile* openedStream,
    datafiles::TokenReader& reader, ObjectColorTableValues& values,
    std::int32_t& firstTrailingValue, std::int32_t& secondTrailingValue);

} // namespace m2m::doe
