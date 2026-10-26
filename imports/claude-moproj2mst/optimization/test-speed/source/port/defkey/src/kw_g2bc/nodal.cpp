// kw-g2bc nodal BCs: the parser cases of FUN_14038b8a0 for BCCDEF BCCDFN BCCANG BCCTMP BCCTFN
// HDNOBJ BCCCRB BCCRHT CRBFLX RHTFLX (deform-rea-work/out/g2/parser2.c, p_main.c lines given per
// case). Their writers (registry handler2s and FUN_1402a4a80) belong to kw-g2sv.
// Specs: docs/re/keywords/<KW>.md, group2-helpers.md.
#include "internal.hpp"

#include <defkey/node_elem_var.hpp>


namespace def::key::g2bc {
namespace nodal {

namespace {

// registry ids
constexpr int kBccdef = 162, kBccdfn = 163, kBccang = 164, kBcctmp = 168, kBcctfn = 169;
constexpr int kHdnobj = 217, kBcccrb = 219, kBccrht = 220, kCrbflx = 221, kRhtflx = 222;

// The node / element counts (obj+0x880 / obj+0x884).
std::int32_t nodes(LLOBJ_DATA& obj) { return at<std::int32_t>(obj, 0x880); }
std::int32_t elements(LLOBJ_DATA& obj) { return at<std::int32_t>(obj, 0x884); }

// An int per-node array obj+off with m = *(int*)(model+moff): FUN_140419d40.
int read_ints(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::size_t off, std::size_t moff)
{
    return read_int_values(f, at<std::int32_t*>(obj, off), nodes(obj), at<std::int32_t>(m, moff));
}

}  // namespace

bool handles(int id)
{
    switch (id) {
    case kBccdef: case kBccdfn: case kBccang: case kBcctmp: case kBcctfn:
    case kHdnobj: case kBcccrb: case kBccrht: case kCrbflx: case kRhtflx:
        return true;
    default:
        return false;
    }
}

// part of @src 14038b8a0  cases 0x12 0x13 0x14 0x18 0x19 0x49 0x4b 0x4c 0x4d 0x4e
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    int st = 0;
    switch (id) {
    case kBccdef: {  // case 0x12, p_main.c 1356-1380
        st = read_ints(m, f, obj, 0x990, 0x1640);
        if (is_3d(m)) {
            // a uniform negative code c c c becomes c-200 0 0 (whatever the read status)
            std::int32_t* v = at<std::int32_t*>(obj, 0x990);
            for (std::int32_t i = 0; i < nodes(obj); ++i) {
                if (v[i * 3] < 0 && v[i * 3] == v[i * 3 + 1] && v[i * 3] == v[i * 3 + 2]) {
                    v[i * 3] = v[i * 3] + -200;
                    v[i * 3 + 2] = 0;
                    v[i * 3 + 1] = 0;
                }
            }
        }
        break;
    }
    case kBccdfn:  // case 0x13, 1381-1384
        st = read_ints(m, f, obj, 0x998, 0x1644);
        break;
    case kBccang:  // case 0x14, 1385-1387: nothing read, the rest of the line stays
        st = 0;
        break;
    case kBcctmp:  // case 0x18, 1403-1406
        st = read_ints(m, f, obj, 0x9a0, 0x1648);
        break;
    case kBcctfn:  // case 0x19, 1407-1410
        st = read_ints(m, f, obj, 0x9a8, 0x164c);
        break;
    case kHdnobj:  // case 0x49, 2286-2290: per element, mode 0
        st = read_node_values(f, at<double*>(obj, 0xa90), elements(obj), &at<std::int32_t>(m, 0x16d8), 0);
        break;
    case kBcccrb:  // case 0x4b, 2301-2304
        st = read_ints(m, f, obj, 0x9b0, 0x1650);
        break;
    case kBccrht:  // case 0x4c, 2305-2308
        st = read_ints(m, f, obj, 0x9b8, 0x1654);
        break;
    case kCrbflx:  // case 0x4d, 2309-2313
        st = read_node_values(f, at<double*>(obj, 0x910), nodes(obj), &at<std::int32_t>(m, 0x1630), 0);
        break;
    case kRhtflx:  // case 0x4e, 2314-2318
        st = read_node_values(f, at<double*>(obj, 0x920), nodes(obj), &at<std::int32_t>(m, 0x1638), 0);
        break;
    default:
        return 2;
    }
    return st != 0 ? 2 : 0;
}

// The KEY write handlers of these keywords (1402b60c0 1402b6180 1402b6240 1402b63c0 1402b6480,
// and FUN_1402a4a80 for BCCDEF BCCDFN BCCTMP BCCTFN) belong to kw-g2sv.
bool handles_write(int) { return false; }
int write_keyword(int, std::FILE*, DB_STEP_DATA&, const KeyRef&) { return 0; }

}  // namespace nodal
}  // namespace def::key::g2bc
