// defdbio/src/objcheck/check_chain.cpp: the GENDB input checks for materials, inter-material data
// and objects (entered from p4-04's 1401bebf0), the object-list check 14017adb0 and the model
// checks 1401b9d50 that run before GENDB (packet p4-03).
#include <defdbio/objcheck/objcheck.hpp>

#include <cstdint>
#include <cstdio>

#include <defrt/stdlib/legacy_printf.hpp>

#include "ext.hpp"
#include "raw.hpp"

namespace def::dbio::objcheck {

using namespace raw;

namespace {
constexpr double kTol = 1e-06; // DAT_14051f1b8
}

// @src 14001cdc0  empty function (called with the check's second and third arguments)
void fun_14001cdc0() {}

// @src 1401bfdf0  material checks
void check_material_data(DB_STEP_DATA* step, void* unused, std::int32_t* err, std::int32_t* warn)
{
    (void)unused;
    fun_14001cdc0();
    def::rt::crt::fprintf(stdout, "INFO   : Checking material properties\n");
    check_material_list(add(step, 0x1328), err, warn);
}

// @src 1401bfe60  inter-material checks
void check_inter_material_data(DB_STEP_DATA* step, void* unused, std::int32_t* err, std::int32_t* warn)
{
    (void)unused;
    fun_14001cdc0();
    def::rt::crt::fprintf(stdout, "INFO   : Checking inter-material data\n");
    check_phase_pair_list(add(step, 0x1358), err, warn);
}

// @src 1401bfed0  object checks
void check_object_data(DB_STEP_DATA* step, void* unused, std::int32_t* err, std::int32_t* warn)
{
    (void)unused;
    fun_14001cdc0();
    def::rt::crt::fprintf(stdout, "INFO   : Checking object data\n");
    check_object_list(add(step, 0x1380), err, warn);
}

// @src 14017adb0  every object of the list (type 7 skipped) through check_object, then: MOVCTL
// for some object, PDIE(1) / PDIE(2) / MDSOBJ(1..2) name real objects, not every object rigid.
// The model-wide part runs only when deformation is on (simulation control +0x108).
void check_object_list(void* list, std::int32_t* err, std::int32_t* warn)
{
    std::int32_t code = 0;   // local_38
    std::int32_t zero = 0;   // local_4c
    std::int32_t zero2 = 0;  // local_44[1]
    void* step = ptr(list, 0x18);
    const std::int32_t n_obj = ext::fun_14019a280(list);
    if (n_obj < 1) {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : No Object Defined\n");
        code = 4;
        ext::fun_14001cd90(&code, "OBJNAM", &zero, &zero2);
    }
    std::int32_t n_checked = 0;  // local_50
    std::int32_t n_rigid = 0;    // local_44[0]
    char movctl = '*';           // local_54
    char pdie1 = '*';            // local_34
    char pdie2 = '*';            // local_46
    char mds1 = '*';             // local_48
    char mds2 = '*';             // local_3c
    for (void* o = ptr(list, 0); o != nullptr; o = ptr(o, 0x3940)) {
        if (i32(o, 0xc) != 7) {
            n_checked = n_checked + 1;
            check_object(static_cast<LLOBJ_DATA*>(o), &n_rigid, &movctl, &pdie1, &pdie2, &mds1, &mds2, err, warn);
        }
    }
    const bool deform = ext::fun_1401f3c30(add(step, 0x750)) != 0;
    if (!deform) return;
    if (movctl != '!') {
        def::rt::crt::fprintf(stdout, "WARNING: MOVCTL Not Defined for Any Object\n");
        code = 0xd;
        ext::fun_14001cd90(&code, "MOVCTL", &zero, &zero2);
    }
    if (pdie1 != '!') {
        std::int32_t v = i32(step, 0x8b8);   // local_20
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : Invalid Principal Die Defined; PDIE(1) = %3d\n", v);
        code = 0xe;
        ext::fun_14001cd90(&code, "PDIE", &v, &zero2);
    }
    if (pdie2 != '!') {
        std::int32_t v = i32(step, 0x1104);  // local_1c
        if (v != 0) {
            *err = 1;
            def::rt::crt::fprintf(stdout, "ERROR  : Invalid Principal workpiece Defined; PDIE(2) = %3d\n", v);
            code = 0xf;
            ext::fun_14001cd90(&code, "PDIE", &v, &zero2);
        }
    }
    std::int32_t m1 = i32(step, 0x9d8);  // local_28
    std::int32_t m2 = i32(step, 0x9dc);  // local_24
    if (0 < m1 && mds1 != '!') {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : Invalid object/reference for stopping criteria checking\n");
        def::rt::crt::fprintf(stdout, "         MDSOBJ(1) = %3d\n", m1);
        code = 0x10;
        ext::fun_14001cd90(&code, "MDSOBJ", &m1, &zero2);
    }
    if (0 < m2 && mds2 != '!') {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : Invalid object/reference for stopping criteria checking\n");
        def::rt::crt::fprintf(stdout, "         MDSOBJ(2) = %3d\n", m2);
        code = 0x11;
        ext::fun_14001cd90(&code, "MDSOBJ", &m2, &zero2);
    }
    if (n_rigid == n_obj || n_rigid == n_checked) {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : All objects rigid and deformation on\n");
        code = 0x1f;
        ext::fun_14001cd90(&code, "OBJNAM", &zero, &zero2);
    }
}

// ---- model checks before GENDB --------------------------------------------------------------------

// @src 1401b9d50  all four checks run; true when all pass
bool gendb_model_checks(DB_STEP_DATA* step)
{
    bool a = check_aniso(step);
    bool b = check_wpaxis(step);
    bool c = check_transformation(step);
    bool d = check_bearing_surfaces(step);
    return a && b && c && d;
}

// @src 1401b9de0  materials with ANISO data must not be used by deformable objects (2D
// plane-strain-like +0x850 == 3: code 0x37; objects other than type 1 / 4: code 0x38)
bool check_aniso(DB_STEP_DATA* step)
{
    bool ok = true;            // local_20
    std::int32_t obj_no = 0;   // local_38
    std::int32_t code = 0;     // local_30
    std::int32_t mat_no = 0;   // local_1c
    for (std::int32_t m = 1; m <= i32(step, 0x1320); ++m) {
        void* mat = ext::fun_1401d7090(step, m);
        if (mat != nullptr && 0 < i32(mat, 0) && material_has_aniso(mat)) {
            for (std::int32_t k = 1; k <= i32(step, 0x1378); ++k) {
                void* o = ext::fun_14017aaf0(add(step, 0x1380), k);
                if (o == nullptr || i32(o, 0xc) == 0) continue;
                if (is2d(step) && i32(step, 0x850) == 3 &&
                    obj_contains_material(static_cast<LLOBJ_DATA*>(o), i32(mat, 0))) {
                    code = 0x37;
                    mat_no = i32(mat, 0);
                    ok = false;
                    break;
                }
                if (i32(o, 0xc) != 1 && i32(o, 0xc) != 4 &&
                    obj_contains_material(static_cast<LLOBJ_DATA*>(o), i32(mat, 0))) {
                    code = 0x38;
                    mat_no = i32(mat, 0);
                    obj_no = i32(o, 8);
                    ok = false;
                    break;
                }
            }
            if (!ok) break;
        }
    }
    if (!ok) ext::fun_14001cd90(&code, "ANISO   ", &mat_no, &obj_no);
    return ok;
}

// @src 1401b9fe0  3D: the workpiece axis (+0x27a8 direction, +0x2790 point) of every object must
// match its rotational symmetry entries (Cls_1401860a0 at +0x1ea0, list at +0x20a0) other than
// kind 1. Only records the mismatch (code 0x41); always returns true.
bool check_wpaxis(DB_STEP_DATA* step)
{
    if (is2d(step)) return true;
    std::int32_t obj_no = 0;   // local_264
    std::int32_t zero = 0;     // local_200
    std::int32_t code = 0;     // local_238
    bool dead = false;         // [RSP+0x20]: cleared and never set (1401ba3e0 tests it)
    bool bad = false;          // local_1d4
    for (std::int32_t i = 0; i <= i32(step, 0x1378); ++i) {
        void* o = ext::fun_14017aa80(add(step, 0x1380), i + 1);
        if (o == nullptr || !ext::fun_140160f10(o) || i32(o, 0x87c) == 0) continue;
        for (std::int32_t j = 0; j < ext::fun_14019a530(add(o, 0x1ea0)); ++j) {
            void* e = ext::fun_140259b60(add(o, 0x20a0), j + 1);
            if (e == nullptr || i32(e, 4) == 1) continue;
            double edir[3];
            double epnt[3];
            double odir[3];
            double opnt[3];
            vec3_copy(edir, reinterpret_cast<const double*>(add(e, 0x20)));
            vec3_copy(epnt, reinterpret_cast<const double*>(add(e, 8)));
            vec3_copy(odir, reinterpret_cast<const double*>(add(o, 0x27a8)));
            vec3_copy(opnt, reinterpret_cast<const double*>(add(o, 0x2790)));
            if (kTol < ext::fun_140239a30(edir) && kTol < ext::fun_140239a30(odir)) {
                double t[3];
                ext::fun_140239b40(t, edir);
                vec3_copy(edir, t);
                ext::fun_140239b40(t, odir);
                vec3_copy(odir, t);
                if (!vec3_near(edir, odir, kTol) || !vec3_near(epnt, opnt, kTol)) {
                    bad = true;
                    obj_no = i32(o, 8);
                }
            }
        }
    }
    if (dead) {
        code = 0x40;
        ext::fun_14001cd90(&code, "WPAXIS", &obj_no, &zero);
    }
    if (bad) {
        code = 0x41;
        ext::fun_14001cd90(&code, "WPAXIS", &obj_no, &zero);
    }
    return true;
}

// @src 1401ba460  phase transformation model (14029bb50 on the materials): records TRANS when it
// is off (+0x864 == 0) but the materials need it; GRNDAT failure (-99) with it on returns false.
// Sets DB_STEP_DATA+0x898 to 0 for kinds -1/1/4/5 and to 1 for kind 2.
bool check_transformation(DB_STEP_DATA* step)
{
    bool ok = true;            // local_14
    char flag = 1;             // local_24
    std::int32_t zero = 0;     // local_28
    std::int32_t zero2 = 0;    // local_18
    std::int32_t code = 0;     // local_20
    const std::int32_t kind = ext::fun_14029bb50(add(step, 0x1328), &flag);
    if (i32(step, 0x864) == 0) {
        if (kind == -1 || kind == 1 || kind == 2 || kind == 4 || kind == 5) {
            code = 0x2f;
            ext::fun_14001cd60(&code, "TRANS", &zero, &zero2);
        }
    } else if (kind == -99) {
        code = 0xb;
        ext::fun_14001cd70(&code, "GRNDAT", &zero, &zero2);
        ok = false;
    }
    if (kind == -1 || kind == 1 || kind == 4 || kind == 5) {
        if (i32(step, 0x898) != 0) i32(step, 0x898) = 0;
    } else if (kind == 2 && i32(step, 0x898) != 1) {
        i32(step, 0x898) = 1;
    }
    return ok;
}

// @src 1401ba5a0  3D extrusion runs (+0x848 == 4 or 7): some object must have bearing surfaces
bool check_bearing_surfaces(DB_STEP_DATA* step)
{
    std::int32_t zero = 0;   // local_38
    std::int32_t zero2 = 0;  // local_2c
    std::int32_t code = 0;   // local_30
    if (!is3d(step)) return true;
    if (i32(step, 0x848) != 4 && i32(step, 0x848) != 7) return true;
    bool found = false;
    for (std::int32_t i = 0; i <= i32(step, 0x1378); ++i) {
        void* o = ext::fun_14017aa80(add(step, 0x1380), i + 1);
        if (o == nullptr || !ext::fun_140160f10(o) || !ext::fun_140160d90(o)) continue;
        std::int32_t idx = 0;
        void* geo = ext::fun_140198620(o, &idx);
        char flag = 1;
        std::int32_t n = ext::fun_1402fcfe0(geo, &flag);
        if (0 < n) {
            def::rt::crt::fprintf(stdout, "INFO   : Object %d has %d bearing surface(s)\n", i32(o, 8), n);
            code = 0x54;
            ext::fun_14001cd90(&code, "EXTRU", add(o, 8), &n);
            if (!found) found = true;
        }
    }
    if (!found) {
        def::rt::crt::fprintf(stdout, "ERROR  : Bearing surface has not been defined\n");
        code = 0x55;
        ext::fun_14001cd90(&code, "EXTRU", &zero, &zero2);
        return false;
    }
    return true;
}

} // namespace def::dbio::objcheck
