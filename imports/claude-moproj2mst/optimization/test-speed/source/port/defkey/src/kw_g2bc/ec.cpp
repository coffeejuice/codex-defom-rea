// kw-g2bc edge/face BCs: ECCDEF ECCTMP ECCATM ECDEFN ECTMFN ECATFN ECPRES ECHFLX ECAFLX ECCRHT
// ECRHFN ECRFLX (ids 241..252, parser cases 0x61..0x6c of FUN_14038b8a0), their registry
// column-2 handlers, the block writer FUN_1402abf20 (writer-map step 54) and the parts of the
// state-variable writer FUN_1402a4a80 they go through (codes 0x41..0x4c).
// Spec: docs/re/keywords/ECCDEF.md (shared detail) and the other EC*.md.
// Decompiles: deform-rea-work/out/g2bc_ec/ec.c, out/g2/parser2.c (cases), out/g2/D_wsv.c.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/key_writer.hpp>
#include <defkey/node_elem_var.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#if __has_include(<defkey/kw_g2sv.hpp>)
#include <defkey/kw_g2sv.hpp>  // its write_state_var replaces write_state_var_ec below
#endif

namespace def::key::g2bc {

namespace {

// One EC keyword: parser id (= case + 0x90), state-variable writer code, double or int. The array
// is obj+0xae8 + 8 * (code - 0x41), the component count model+0x16fc + 4 * (code - 0x41).
struct EcKw {
    int id;
    int code;
    bool dbl;
};

// in parser-id order 241..252 (cases 0x61..0x6c)
constexpr EcKw kEc[12] = {
    {241, 0x41, false},  // ECCDEF  obj+0xae8  model+0x16fc
    {242, 0x44, false},  // ECCTMP  obj+0xb00  model+0x1708
    {243, 0x47, false},  // ECCATM  obj+0xb18  model+0x1714
    {244, 0x42, false},  // ECDEFN  obj+0xaf0  model+0x1700
    {245, 0x45, false},  // ECTMFN  obj+0xb08  model+0x170c
    {246, 0x48, false},  // ECATFN  obj+0xb20  model+0x1718
    {247, 0x43, true},   // ECPRES  obj+0xaf8  model+0x1704
    {248, 0x46, true},   // ECHFLX  obj+0xb10  model+0x1710
    {249, 0x49, true},   // ECAFLX  obj+0xb28  model+0x171c
    {250, 0x4a, false},  // ECCRHT  obj+0xb30  model+0x1720
    {251, 0x4b, false},  // ECRHFN  obj+0xb38  model+0x1724
    {252, 0x4c, true},   // ECRFLX  obj+0xb40  model+0x1728
};

const EcKw* find_ec(int id)
{
    if (id < 241 || id > 252) return nullptr;
    return &kEc[id - 241];
}

std::size_t array_off(int code) { return 0xae8 + 8 * static_cast<std::size_t>(code - 0x41); }
std::size_t width_off(int code) { return 0x16fc + 4 * static_cast<std::size_t>(code - 0x41); }

// node number valid for the writers: 1..obj+0x880
bool valid_node(const LLOBJ_DATA& obj, std::int32_t n) { return 0 < n && n <= at<std::int32_t>(obj, 0x880); }

bool obj_is_2d(LLOBJ_DATA& obj) { return is_2d(owner_of(obj)); }

// FUN_1402f6600 and FUN_1402f6a10 differ only in the element type and the value formats.
template <class T>
int write_edge_face_line(std::FILE* f, const char* name, std::uint32_t n, LLOBJ_DATA* obj, const T* a,
                         std::int32_t m, T def, const char* head_fmt, const char* rec2d_fmt,
                         const char* rec3d_fmt)
{
    if (f == nullptr) return 2;
    if (obj == nullptr || a == nullptr) return 0;
    std::int32_t ns = model::bc::edge_face_slot_count(*obj);
    std::int32_t cnt = 0;
    bool uniform = true;
    if (1 < m) return 1;
    for (std::int32_t i = 0; i < ns; ++i) {
        // 2D: only slots whose two nodes are in 1..obj+0x880; 3D: every slot
        bool considered = true;
        if (obj_is_2d(*obj)) {
            std::int32_t p = 0, q = 0;
            model::bc::edge_slot_nodes(*obj, i, &p, &q);
            considered = valid_node(*obj, p) && valid_node(*obj, q);
        }
        if (considered) {
            if (a[i] != a[0]) uniform = false;
            if (a[i] != def) ++cnt;
        }
    }
    if (uniform) {
        rt::crt::fprintf(f, head_fmt, name, n, 0, a[0]);
    } else {
        rt::crt::fprintf(f, head_fmt, name, n, cnt, def);
        std::uint32_t k = 0;
        for (std::int32_t i = 0; i < ns; ++i) {
            if (a[i] != def) {
                ++k;  // before the validity test: skipped slots leave gaps in the numbering
                if (!obj_is_2d(*obj)) {
                    std::int32_t n1 = 0, n2 = 0, n3 = 0, n4 = 0;
                    model::bc::face_nodes(*obj, i, &n1, &n2, &n3, &n4);
                    if (valid_node(*obj, n1) && valid_node(*obj, n2) && valid_node(*obj, n3) &&
                        valid_node(*obj, n4))
                        rt::crt::fprintf(f, rec3d_fmt, k, n1, n2, n3, n4, a[i]);
                } else {
                    std::int32_t p = 0, q = 0;
                    model::bc::edge_slot_nodes(*obj, i, &p, &q);
                    if (valid_node(*obj, p) && valid_node(*obj, q)) rt::crt::fprintf(f, rec2d_fmt, k, p, q, a[i]);
                }
            }
        }
    }
    return 0;
}

// @src 1402f6600  int edge/face array writer
int write_edge_face_ints(std::FILE* f, const char* name, std::uint32_t n, LLOBJ_DATA* obj,
                         const std::int32_t* a, std::int32_t m, std::int32_t def)
{
    return write_edge_face_line<std::int32_t>(f, name, n, obj, a, m, def, "%-6s %7i %7i %7i\n",
                                              " %7i %7i %7i %7i\n", " %7i %7i %7i %7i %7i %7i\n");
}

// @src 1402f6a10  double edge/face array writer
int write_edge_face_doubles(std::FILE* f, const char* name, std::uint32_t n, LLOBJ_DATA* obj,
                            const double* a, std::int32_t m, double def)
{
    return write_edge_face_line<double>(f, name, n, obj, a, m, def, "%-6s %7i %7i %20.10E\n",
                                        " %7i %7i %7i %20.10E\n", " %7i %7i %7i %7i %7i %20.10E\n");
}

// part of @src 1402a4a80  state-variable writer, cases 0x41..0x4c: name FUN_1402f7720(id), then
// FUN_1402f6600 / FUN_1402f6a10 on the object's array with the width model+0x16fc.. and default 0
// With kw-g2sv merged, its write_state_var (all of 1402a4a80) is used and this copy is skipped;
// delete it then.
int write_state_var_ec(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, const EcKw& k, std::uint32_t n)
{
#if __has_include(<defkey/kw_g2sv.hpp>)
    def::key::g2sv::write_state_var(m, f, obj, k.code, n, 0);
    return 0;
#endif
    const char* name = keyword_label(k.id);
    std::int32_t w = at<std::int32_t>(m, width_off(k.code));
    if (k.dbl)
        write_edge_face_doubles(f, name, n, &obj, at<double*>(obj, array_off(k.code)), w, 0.0);
    else
        write_edge_face_ints(f, name, n, &obj, at<std::int32_t*>(obj, array_off(k.code)), w, 0);
    return 0;  // 1402a4a80 discards the line writer's status
}

// The 12 registry handlers: object by ref[0] (14017aaf0), N = obj+0x3f24 when > 0, then
// 1402a4a80(model, f, obj, code, N, 0); always 0. The original dereferences a missing object;
// the port writes nothing then.
int write_handler(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref, int id)
{
    std::int32_t n = ref[0];
    LLOBJ_DATA* obj = find_object(m, n);
    if (obj == nullptr) return 0;
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    write_state_var_ec(m, f, *obj, *find_ec(id), static_cast<std::uint32_t>(n));
    return 0;
}

// @src 1402b6840  ECCDEF handler (code 0x41)
int write_eccdef(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 241); }
// @src 1402b6a80  ECCTMP handler (code 0x44)
int write_ecctmp(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 242); }
// @src 1402b6cc0  ECCATM handler (code 0x47)
int write_eccatm(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 243); }
// @src 1402b6900  ECDEFN handler (code 0x42)
int write_ecdefn(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 244); }
// @src 1402b6b40  ECTMFN handler (code 0x45)
int write_ectmfn(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 245); }
// @src 1402b6d80  ECATFN handler (code 0x48)
int write_ecatfn(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 246); }
// @src 1402b69c0  ECPRES handler (code 0x43)
int write_ecpres(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 247); }
// @src 1402b6c00  ECHFLX handler (code 0x46)
int write_echflx(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 248); }
// @src 1402b6e40  ECAFLX handler (code 0x49)
int write_ecaflx(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 249); }
// @src 1402b6f00  ECCRHT handler (code 0x4a)
int write_eccrht(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 250); }
// @src 1402b6fc0  ECRHFN handler (code 0x4b)
int write_ecrhfn(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 251); }
// @src 1402b7080  ECRFLX handler (code 0x4c)
int write_ecrflx(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_handler(f, m, ref, 252); }

} // namespace

// public name of the 1402f6600 body above (callers outside the KEY writer: moproj2mst HT_Proc MOKEY)
int write_edge_face_int_line(std::FILE* f, const char* name, std::uint32_t n, LLOBJ_DATA* obj,
                             const std::int32_t* a, std::int32_t m, std::int32_t def)
{
    return write_edge_face_ints(f, name, n, obj, a, m, def);
}

namespace ec {

bool handles(int id) { return find_ec(id) != nullptr; }

// part of @src 14038b8a0  cases 0x61..0x6c: the int (14041a440) or double (14041a090) reader on the
// object's array with the parser model's component count; nonzero status -> 2
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    const EcKw* k = find_ec(id);
    if (k == nullptr) return 2;
    std::int32_t w = at<std::int32_t>(m, width_off(k->code));
    int st;
    if (k->dbl)
        st = read_edge_face_values(f, at<double*>(obj, array_off(k->code)), &obj, w);
    else
        st = read_edge_face_ints(f, at<std::int32_t*>(obj, array_off(k->code)), &obj, w);
    return st == 0 ? 0 : 2;
}

bool handles_write(int id) { return find_ec(id) != nullptr; }

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    switch (id) {
    case 241: return write_eccdef(f, m, ref);
    case 242: return write_ecctmp(f, m, ref);
    case 243: return write_eccatm(f, m, ref);
    case 244: return write_ecdefn(f, m, ref);
    case 245: return write_ectmfn(f, m, ref);
    case 246: return write_ecatfn(f, m, ref);
    case 247: return write_ecpres(f, m, ref);
    case 248: return write_echflx(f, m, ref);
    case 249: return write_ecaflx(f, m, ref);
    case 250: return write_eccrht(f, m, ref);
    case 251: return write_ecrhfn(f, m, ref);
    case 252: return write_ecrflx(f, m, ref);
    default: return 0;
    }
}

}  // namespace ec

// @src 1402abf20  writer-map step 54: the 12 EC handlers in the order ECCDEF ECDEFN ECPRES ECCTMP
// ECTMFN ECHFLX ECCATM ECATFN ECAFLX ECCRHT ECRHFN ECRFLX, when the object has a mesh (+0x87c)
// and a boundary (+0xbd8 > 0). The key passed is the object's own number obj+8; the N argument is
// computed but unused (the handlers look the object up again and print obj+0x3f24 or that
// number). A nonzero handler status would end the block (the handlers always return 0).
int write_edge_face_bcs(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    (void)n;
    KeyRef ref;
    ref.push_back(at<std::int32_t>(obj, 8));
    if (at<std::int32_t>(obj, 0x87c) != 0 && 0 < at<std::int32_t>(obj, 0xbd8)) {
        static constexpr int kOrder[12] = {241, 244, 247, 242, 245, 248, 243, 246, 249, 250, 251, 252};
        for (int id : kOrder) {
            int st = ec::write_keyword(id, f, m, ref);
            if (st != 0) return st;
        }
    }
    return 0;
}

}  // namespace def::key::g2bc
