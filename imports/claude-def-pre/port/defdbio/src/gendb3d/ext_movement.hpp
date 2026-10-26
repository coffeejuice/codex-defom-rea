// gendb3d/ext_movement.hpp: calls from the movement writer (FUN_1401912a0, movement.cpp) into
// functions other packets own. Same scheme as ext.hpp: the test hook first, otherwise the owner's
// port (named next to each wrapper; gate ports in defdbio/gendb/gate_ports.hpp).
#pragma once

#include <cstdint>

#include "ext.hpp"

#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defmodel/window/inter_object_ops.hpp>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
} // namespace def::model

namespace def::dbio::g3d::ext {

// 140130290 (ledger P2, cpp_data_model): LLFNC length of a DYNARY. ndim 0: ncomp (+0x40) for the
// types 0xa7, 0xa8, 0xaa, 0xab, 0xad, 0xb3, 0xb5, else 0; ndim 1: 140130200 (type 0x7c: ncomp,
// else n[0] * 2); ndim 2: 140130160; ndim 3: 14012fff0; other ndim: 0. -> gendb::dynary_words
inline std::int32_t dynary_db_length(const model::DYNARY* f)
{
    std::uint64_t r;
    if (hooked(0x140130290, {u(f)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::dynary_words(f);
}

// 14012eff0 (DYNARY family, defmodel; not in the ledger): copy src into dst. Type, 0x58 (type
// 0x80 only), 0x08, 0x54, ndim, ncomp, 0x50, var[3], n[3], then 14012fad0 (allocate) and memcpy
// of the axes (while both pointers are set) and of 14012ff20(dst) values. -> model::dynary_copy
inline void dynary_copy(model::DYNARY* dst, const model::DYNARY* src)
{
    std::uint64_t r;
    if (hooked(0x14012eff0, {u(dst), u(src)}, &r)) return;
    model::dynary_copy(dst, src);
}

// 140135bc0 (packet p4-02a): DYNARY of type 0x7b with ncomp 3 -> type 0x7a: each point's first
// two values (and 1.0) go through 1402416d0 with the 3x3 frame, the third value is kept. Returns
// true when it converted. The movement writer ignores the result. -> gendb::dynary_rotate_velocity
inline bool dynary_to_frame(model::DYNARY* f, const double* frame)
{
    std::uint64_t r;
    if (hooked(0x140135bc0, {u(f), u(frame)}, &r)) return (r & 0xff) != 0;
    return gendb::dynary_rotate_velocity(f, frame) != 0;
}

// 14018cde0 (packet p4-02a): LLFNC length of the movement's type-6 curve (mv+0x448). Not 3D:
// 140130230(mv+0x448); 3D: 140130230 of a copy transformed by 140135bc0 with 140194d40's frame,
// plus 10 (the frame header the writer puts in front). -> gendb::props_2160_table6_words
inline std::int32_t movement_curve_length(void* mv, model::DB_STEP_DATA* m)
{
    std::uint64_t r;
    if (hooked(0x14018cde0, {u(mv), u(m)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::props_2160_table6_words(mv, m);
}

// 140194d40 (ledger P2): the movement's 3x3 frame mv+0x4c8..0x508, transposed, into frame[0..8];
// returns 1 (the writer ignores it). -> gendb::movement_frame (gate port)
inline std::uint64_t movement_frame(const void* mv, double* frame)
{
    std::uint64_t r = 0;
    if (hooked(0x140194d40, {u(mv), u(frame)}, &r)) return r;
    return gendb::movement_frame(mv, frame);
}

} // namespace def::dbio::g3d::ext
