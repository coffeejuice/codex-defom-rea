// defkey/node_elem_var.hpp: the KEY readers for per-node, per-element and per-edge/face value
// arrays (0x140416c90..0x14041a440 family), shared by the group 2 packets. Owner: kw-g2bc.
// Spec: docs/re/keywords/group2-helpers.md "Per-node and per-element value readers", ECCDEF.md.
// Return values as in the binary: 0, or 1 on EOF / bad header (the parser maps 1 to 2).
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::key {

// 140416c90: header by mode (0 "%i %Flf", 6 "%i"+10 %Flf, 1 "%i %i %Flf", 2 "%i %i"+10 %Flf,
// 3 "%i %Flf %i"), default fill, then records (1404199a0 for mode 0, else 140419b90).
int read_node_values(std::FILE* f, double* dst, std::int32_t n, std::int32_t* ncomp, std::int32_t mode);
// 1404199a0: `count` records "%i" index + m "%Flf", stored when 1 <= index <= n; final fgets.
int read_value_records(std::FILE* f, double* dst, std::int32_t count, std::int32_t n, std::int32_t m);
// 140419b90: the same with k values per record (first min(m, k) stored); no final fgets; k == 0
// is 1404199a0.
int read_value_records_k(std::FILE* f, double* dst, std::int32_t count, std::int32_t n, std::int32_t m,
                         std::int32_t k);
// 140419d40: int arrays, header "%i %i" (count, default), then 140419ea0.
int read_int_values(std::FILE* f, std::int32_t* dst, std::int32_t n, std::int32_t m);
// 140419ea0: int version of 1404199a0.
int read_int_records(std::FILE* f, std::int32_t* dst, std::int32_t count, std::int32_t n, std::int32_t m);
// 14041a090 / 14041a440: per boundary edge (2D) / face (3D) double / int records (ECCDEF.md).
int read_edge_face_values(std::FILE* f, double* dst, def::model::LLOBJ_DATA* obj, std::int32_t m);
int read_edge_face_ints(std::FILE* f, std::int32_t* dst, def::model::LLOBJ_DATA* obj, std::int32_t m);

}  // namespace def::key
