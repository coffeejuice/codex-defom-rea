// kw_g1b/common.hpp: small helpers shared by the kw-g1b reader and writer.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>

namespace def::key::g1b {

using def::model::DB_STEP_DATA;

// FUN_1400015e0: the model is 2D (`+0x1c == 2`). Owned by the model skeleton; a one-liner here.
inline bool is_2d(const DB_STEP_DATA& m) { return m.dimension == 2; }

// Raw access at a byte offset from the model, for the places where the original reads a field
// with a type other than its own (SOLMTD's S4 message reads the ITRMXD/DEFBWD ints as one double)
// or where the layout header still types a double slot as uint64_t. memcpy keeps it bit-exact.
inline double load_double(const void* base, std::size_t off)
{
    double v;
    std::memcpy(&v, static_cast<const std::byte*>(base) + off, sizeof v);
    return v;
}

inline void store_double(void* base, std::size_t off, double v)
{
    std::memcpy(static_cast<std::byte*>(base) + off, &v, sizeof v);
}

}  // namespace def::key::g1b
