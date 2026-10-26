// The per boundary edge (2D) / face (3D) value readers of defkey/node_elem_var.hpp (EC*
// keywords, packet kw-g2bc). Spec: docs/re/keywords/ECCDEF.md "KEY grammar".
// Decompile: deform-rea-work/out/g2bc_ec/ec.c.
//
// Both: the rest of the keyword line is the header "count [default]" (no comma replacement);
// with two conversions every slot 0..NS-1 x m gets the default; then `count` free-format records
// "idx n1 n2 v*m" (2D) or "idx n1 n2 n3 n4 v*m" (3D), each stored at the slot of its edge/face,
// and one fgets for the rest of the last line. Only EOF (scanf -1) is an error (1).
#include <defkey/node_elem_var.hpp>

#include <cstdlib>

#include <defkey/crt_scan.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key {

namespace {

// FUN_1400015e0(FUN_140158f50(obj)): the object's model is 2D
bool obj_is_2d(const model::LLOBJ_DATA* obj)
{
    const auto* m = reinterpret_cast<const std::byte*>(obj->get_owner());
    return *reinterpret_cast<const std::int32_t*>(m + 0x1c) == 2;
}

// The record variables of both readers live for the whole call: a record whose fscanf stops
// early (a non-numeric token returns 0, not -1) keeps the previous record's values. The original
// leaves them uninitialised before the first record; the port starts them at 0.
struct RecordVars {
    std::int32_t idx = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0;
};

// fscanf of the record's node part; -1 on EOF
int read_record_nodes(std::FILE* f, model::LLOBJ_DATA* obj, RecordVars& r)
{
    if (!obj_is_2d(obj)) return crt::fscanf(f, "%i %i %i %i %i", &r.idx, &r.n1, &r.n2, &r.n3, &r.n4);
    return crt::fscanf(f, "%i %i %i", &r.idx, &r.n1, &r.n2);
}

std::int32_t record_slot(model::LLOBJ_DATA* obj, const RecordVars& r)
{
    if (!obj_is_2d(obj)) return model::bc::face_slot(*obj, r.n1, r.n2, r.n3, r.n4);
    return model::bc::edge_slot(*obj, r.n1, r.n2);
}

} // namespace

// @src 14041a090  double edge/face array reader
int read_edge_face_values(std::FILE* f, double* dst, model::LLOBJ_DATA* obj, std::int32_t m)
{
    char line[0x400];
    if (std::fgets(line, 0x400, f) == nullptr) return 1;
    std::int32_t count = 0;  // uninitialised in the original when nothing converts
    double def = 0.0;
    int nconv = crt::sscanf(line, "%i %Flf", &count, &def);
    if (nconv == -1) return 1;
    std::int32_t ns = model::bc::edge_face_slot_count(*obj);
    if (1 < nconv) {
        for (std::int32_t i = 0; i < ns; ++i)
            for (std::int32_t c = 0; c < m; ++c) dst[static_cast<std::int64_t>(i * m + c)] = def;
    }
    if (count < 1) return 0;
    double* vals = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(m)), 8));
    RecordVars r;
    for (std::int32_t i = 0; i < count; ++i) {
        if (read_record_nodes(f, obj, r) == -1) return 1;  // vals is leaked, as in the original
        for (std::int32_t c = 0; c < m; ++c)
            if (crt::fscanf(f, "%Flf", &vals[c]) == -1) return 1;
        std::int32_t s = record_slot(obj, r);
        // no upper bound check here (the int reader has one)
        if (-1 < s) {
            for (std::int32_t c = 0; c < m; ++c) dst[static_cast<std::int64_t>(s * m + c)] = vals[c];
        }
    }
    std::free(vals);
    if (std::fgets(line, 0x400, f) == nullptr) return 1;
    return 0;
}

// @src 14041a440  int edge/face array reader (readEdgeFaceVarInt)
int read_edge_face_ints(std::FILE* f, std::int32_t* dst, model::LLOBJ_DATA* obj, std::int32_t m)
{
    char line[0x400];
    if (std::fgets(line, 0x400, f) == nullptr) return 1;
    std::int32_t count = 0;  // uninitialised in the original when nothing converts
    std::int32_t def = 0;
    int nconv = crt::sscanf(line, "%i %i", &count, &def);
    if (nconv == -1) return 1;
    std::int32_t ns = model::bc::edge_face_slot_count(*obj);
    if (1 < nconv) {
        for (std::int32_t i = 0; i < ns; ++i)
            for (std::int32_t c = 0; c < m; ++c) dst[static_cast<std::int64_t>(i * m + c)] = def;
    }
    if (count < 1) return 0;
    auto* vals = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(m)), 4));
    RecordVars r;
    for (std::int32_t i = 0; i < count; ++i) {
        if (read_record_nodes(f, obj, r) == -1) return 1;  // vals is leaked, as in the original
        for (std::int32_t c = 0; c < m; ++c)
            if (crt::fscanf(f, "%i", &vals[c]) == -1) return 1;
        std::int32_t s = record_slot(obj, r);
        if (s < 0 || ns <= s) {
            def::rt::crt::printf("ERROR in readEdgeFaceVarInt, iECCArrayAdr\n");
        } else {
            for (std::int32_t c = 0; c < m; ++c) dst[static_cast<std::int64_t>(s * m + c)] = vals[c];
        }
    }
    std::free(vals);
    if (std::fgets(line, 0x400, f) == nullptr) return 1;
    return 0;
}

} // namespace def::key
