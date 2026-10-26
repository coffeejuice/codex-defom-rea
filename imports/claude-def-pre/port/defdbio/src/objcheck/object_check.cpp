// defdbio/src/objcheck/object_check.cpp: the GENDB check of one object (140167e90, packet p4-03).
//
// Follows the decompile statement by statement; locals keep the decompile's names in comments.
// Besides printing, it corrects the model: NBCD codes that point at a missing or type-7 object are
// zeroed, edge functions on code-0 edges are cleared (14016c550), unused 3D symmetry planes are
// deleted (14016c780), an inconsistent mechanical-press stroke vector is replaced, and the
// rotational symmetry angle is reset when no node has a rotational symmetry code.
#include <defdbio/objcheck/objcheck.hpp>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <defrt/stdlib/legacy_printf.hpp>

#include "ext.hpp"
#include "raw.hpp"

namespace def::dbio::objcheck {

using namespace raw;

namespace {

constexpr double kTol = 1e-06;     // DAT_14051f1b8
constexpr double kOne = 1.0;       // DAT_140509c90
constexpr double kPenvol = 10.0;   // DAT_14051bee8
constexpr double kTiny = 1e-12;    // DAT_14051f260
constexpr double kVolFrac = 1e-08; // DAT_14051bed0

template <class... A>
void out(const char* fmt, A... a)
{
    def::rt::crt::fprintf(stdout, fmt, a...);
}

} // namespace

// @src 140167e90  check one object (see objcheck.hpp)
void check_object(LLOBJ_DATA* obj, std::int32_t* n_rigid, char* movctl, char* pdie1, char* pdie2,
                  char* mdsobj1, char* mdsobj2, std::int32_t* err, std::int32_t* warn)
{
    void* o = obj;
    void* const step = owner(o);              // *(param_1 + 15000)
    void* const sc = add(step, 0x750);
    std::int32_t code = 0;                    // local_248
    std::int32_t zero_solmtd = 0;             // local_2cc
    std::int32_t val = 0;                     // local_264
    bool solid = false;                       // local_28e
    void* const obj_no = add(o, 8);
    const auto id = [&]() { return i32(o, 8); };
    const auto type = [&]() { return i32(o, 0xc); };
    const std::int32_t pd1 = i32(step, 0x8b8);           // local_24c
    const std::int32_t pd2 = i32(step, 0x1104);          // local_268
    const bool heat = ext::fun_1401f3c10(sc) != 0;       // local_290
    const bool deform = ext::fun_1401f3c30(sc) != 0;     // local_27b
    const bool transf = ext::fun_1401f3c50(sc) != 0;     // local_2b8
    const bool diffus = ext::fun_1401f3c70(sc) != 0;     // local_250
    const bool grain = sim_u114(sc) != 0;                // local_23b (1401f3c90)
    const bool electr = ext::fun_1401f3cb0(sc) != 0;     // local_23c
    const std::int32_t mds1 = i32(step, 0x9d8);          // local_294
    const std::int32_t mds2 = i32(step, 0x9dc);          // local_26c
    void* const lvar7 = step;
    std::int32_t i = 0;                                  // local_2bc
    std::int32_t at = 0;                                 // local_280

    if (i32(o, 0x87c) != 0 && 1 < i32(o, 0xbd8)) {
        out("INFO   : Object %3d has multiple(%d) mesh boundaries.\n", id(), i32(o, 0xbd8));
        code = 0x4c;
        ext::fun_14001cd90(&code, "", obj_no, add(o, 0xbd8));   // DAT_14051f7e7 = ""
    }
    if (!is3d(step)) {
        solid = type() == 1 || type() == 2 || type() == 4 || type() == 6;
    } else {
        solid = type() == 1 || type() == 2 || type() == 4;
    }
    if (deform && type() == 0) {
        *n_rigid = *n_rigid + 1;
        bool no_geo;                                      // local_234
        if (!is3d(step))
            no_geo = !ext::fun_140160d20(o);
        else
            no_geo = !ext::fun_140160d90(o);
        if (no_geo) {
            *err = 1;
            out("ERROR  : The rigid object %4d has no \n", id());
            out("         geometry and deformation is turned on\n");
            code = 0x31;
            ext::fun_14001cd90(&code, "OBJNAM", obj_no, &val);
        }
    }
    code = 0x17;
    ext::fun_14001cd90(&code, "OBJNAM", obj_no, &val);
    if (pd1 == id()) *pdie1 = '!';
    if (pd2 == id()) *pdie2 = '!';
    if (0 < i32(o, 0x1ec8)) {
        if (mds1 == id()) *mdsobj1 = '!';
        if (mds2 == id()) *mdsobj2 = '!';
    }
    if (solid) {
        obj_undefined(obj, "AVGSTR", 1, '!', err);
        obj_undefined(obj, "LMTSTR", 1, '!', err);
        obj_undefined(obj, "PENVOL", 1, '!', err);
        if (f64(o, 0x1eb0) <= kPenvol) {
            out("WARNING: PENVOL < 10 for Object %3d\n", id());
            code = 6;
            ext::fun_14001cd90(&code, "PENVOL", obj_no, &val);
        }
        if (f64(o, 0x1ea8) <= f64(o, 0x1ea0)) {
            out("WARNING: AVGSTR < LMTSTR for Object %3d\n", id());
            code = 7;
            ext::fun_14001cd90(&code, "AVGSTR", obj_no, &val);
        }
    }
    {
        const bool trans = obj_has_translation(obj);   // cVar2
        const bool rot = obj_has_rotation(obj);        // local_2d0
        if (trans || rot) *movctl = '!';
        if (is3d(step) && !movement_direction_set(add(o, 0x2160))) {
            if (i32(o, 0x21c8) != 6 && trans) {
                out("ERROR  : Movement direction is not specified for\n");
                out("         Object No %3d\n", id());
                *err = 1;
                code = 0x22;
                ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
            }
            if (i32(step, 0xa34) == 2 && i32(o, 0x21c8) == 0 && trans) {
                out("ERROR  : Conjugate-gradient solver does not work\n");
                out("         for load controlled dies. Use Sparse solver.\n");
                *err = 1;
                code = 0x16;
                ext::fun_14001cd60(&code, "SOLMTD", &zero_solmtd, &val);
            }
        }
    }
    if (type() == 0) {
        if (is3d(step)) {
            if (obj_has_rot_move1(obj) && !obj_has_rot_axis1(obj)) {
                val = 1;
                *err = 1;
                out("ERROR  : Rotational movement has been defined for Object %6d.\n", id());
                out("         But rotational axis %d is undefined.\n", val);
                code = 0x49;
                ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
            }
            if (obj_has_rot_move2(obj) && !obj_has_rot_axis2(obj)) {
                val = 2;
                *err = 1;
                out("ERROR  : Rotational movement has been defined for Object %6d.\n", id());
                out("         But rotational axis %d is undefined.\n", val);
                code = 0x49;
                ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
            }
        }
        if (is2d(step) && (i32(step, 0x850) == 1 || i32(step, 0x850) == 3) && obj_has_rotation(obj)) {
            out("ERROR  : Invalid rotation defined for axisymmteric/torsion Object %3d\n", id());
            out("         Remove ANGMOV Keyword from Keyword file\n");
            *err = 1;
            code = 0x34;
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
    } else {
        const bool has_ctl = obj_has_movement(obj) || !(i32(o, 0x21c8) == 1 || i32(o, 0x21c8) == 0);  // local_232
        const bool has_bc = obj_has_velocity_code(obj);                                             // local_22f
        const bool rot_only = !obj_has_translation(obj) && obj_has_rot_move1(obj) && !obj_has_rot_move2(obj); // local_233
        const bool rotsym = obj_has_rotsym_bc(obj);                                                  // local_231
        const bool rot_rotsym = rot_only && rotsym;                                                  // local_230
        if (has_ctl != has_bc && !rot_rotsym) {
            *err = 1;
            if (!has_bc || has_ctl) {
                out("ERROR  : Movement control is defined for Object %6d\n", id());
                out("         Movement boundary condition must also be defined.\n");
                val = 2;
            } else {
                out("ERROR  : Movement boundary condition defined for Object %6d\n", id());
                out("         Movement control must also be defined.\n");
                val = 1;
            }
            code = 0x4a;
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
    }
    if (i32(o, 0x21c8) == 4) {
        if (u8(o, 0x2490) != 0 && (i32(o, 0x249c) != 1 || dynary_all_zero(add(o, 0x2498)))) {
            *err = 1;
            out("ERROR  : Knuckle or wedge press is defined for Object %6d\n", id());
            out("         Velocity profile must also be defined.\n");
            code = 0x53;
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
        bool fixed = false;                               // local_22e
        double stroke;                                    // local_228
        if (u8(o, 0x2490) == 0)
            stroke = f64(o, 0x2478);
        else
            stroke = press_curve_range(add(o, 0x2160));
        double stroke0;                                   // local_220
        if (kTol < std::fabs(stroke)) {
            double proj = press_stroke_projection(add(o, 0x2160), reinterpret_cast<const double*>(add(o, 0x22d0)),
                                                  reinterpret_cast<const double*>(add(o, 0x21b0)));   // local_200
            stroke0 = stroke - proj;
        } else {
            stroke0 = 0.0;
        }
        double v[3];                                      // local_218 / 210 / 208
        press_stroke_vector(add(o, 0x2160), stroke0, stroke, v);
        if (v[0] != f64(o, 0x22d0) || v[1] != f64(o, 0x22d8) || v[2] != f64(o, 0x22e0)) {
            f64(o, 0x22d0) = v[0];
            f64(o, 0x22d8) = v[1];
            f64(o, 0x22e0) = v[2];
            fixed = true;
        }
        if (fixed) {
            out("WARNING  : Inconsistent Mechanical Stroke data corrected for Object %6d\n", id());
            *warn = 1;
            code = 0x58;
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
    }
    if (i32(o, 0x21c8) == 10) {
        *err = 1;
        out("ERROR  : Obsolete hydraulic movement type has been defined for Object %6d.\n", id());
        code = 0x4d;
        val = 0;
        ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
    }
    if (i32(o, 0x21c8) == 2 && 0 < i32(o, 0x2360)) {
        void* other = ext::fun_1401d6ea0(step, i32(o, 0x2360));   // local_1f0
        bool valid = true;                                         // local_1f8
        if (other == nullptr)
            valid = false;
        else if (!ext::fun_140160f10(other))
            valid = false;
        if (!valid) {
            *err = 1;
            out("ERROR  : Counter-blow hammer movement control is defined for Object %6d\n", id());
            out("         However, the counter-blow Object %6d is not a valid object any more.\n", i32(o, 0x2360));
            code = 0x4f;
            val = i32(o, 0x2360);
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        } else if (i32(other, 0x21c8) != 2 || i32(other, 0x2360) != id() || i32(other, 0x2360) == 0) {
            *err = 1;
            out("ERROR  : Counter-blow hammer movement control is defined for Object %6d\n", id());
            out("         Object %6d should have valid counter-blow hammer object definition.\n", i32(o, 0x2360));
            code = 0x4e;
            val = i32(o, 0x2360);
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
    }
    if (i32(o, 0x21c8) == 2 || i32(o, 0x21c8) == 3) {
        double eff = 0.0;          // local_1e8
        bool ok = true;            // local_1e0
        if (i32(o, 0x21c8) == 2 && f64(o, 0x2350) != 0.0) {
            if (i32(o, 0x236c) == 0) {
                if (f64(o, 0x2358) <= 0.0 || kOne < f64(o, 0x2358)) ok = false;
            } else {
                for (std::int32_t k = 0; k < i32(o, 0x2384); ++k) {
                    eff = static_cast<const double*>(ptr(o, 0x23b0))[k];
                    if (eff < 0.0 || kOne < eff) {
                        ok = false;
                        break;
                    }
                }
            }
        } else if (i32(o, 0x21c8) == 3) {
            if (i32(o, 0x2404) == 0) {
                if (f64(o, 0x23e8) <= 0.0 || kOne < f64(o, 0x23e8)) ok = false;
            } else {
                for (std::int32_t k = 0; k < i32(o, 0x241c); ++k) {
                    eff = static_cast<const double*>(ptr(o, 0x2448))[k];
                    if (eff < 0.0 || kOne < eff) {
                        ok = false;
                        break;
                    }
                }
            }
        }
        if (!ok) {
            *warn = 1;
            out("WARNING: Hammer/Screw movement efficiency data is not valid for Object %6d\n", id());
            out("         Object %6d should have valid efficiency data (0~1).\n", id());
            code = 0x59;
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
    }
    if (i32(o, 0x21c8) == 5) {
        bool any = false;          // local_1c8
        for (std::int32_t k = 0; k < i32(step, 0x1378); ++k) {
            void* other = ext::fun_1401d6ed0(step, k + 1);   // local_1d0
            if (ext::fun_140160f10(other)) {
                if (i32(other, 8) == id()) {
                    if (obj_has_rotation(static_cast<LLOBJ_DATA*>(other))) {
                        any = true;
                        break;
                    }
                } else if (obj_has_movement(static_cast<LLOBJ_DATA*>(other))) {
                    any = true;
                    break;
                }
            }
        }
        if (!any) {
            out("WARNING: Sliding die movement control is defined for Object %6d\n", id());
            out("         However, no movement contrl is defined for any other object.\n");
            code = 0x50;
            val = i32(o, 0x2360);
            ext::fun_14001cd90(&code, "MOVCTL", obj_no, &val);
        }
    }

    if (!(type() != 0 || (!ext::fun_140167ae0(o) && 3 < i32(o, 0x880) && 0 < i32(o, 0x884)))) return;

    if (is2d(step) && i32(step, 0x850) == 1) {
        std::int32_t nneg = 0;                     // local_1c4
        nneg = obj_negative_radius_nodes(obj);
        if (0 < nneg) {
            out("ERROR: Node coords. for %6d nodes of Object %3d\n", nneg, id());
            out("       substaintially less than ZERO\n");
            *err = 1;
            code = 0x1e;
            ext::fun_14001cd90(&code, "RZ", obj_no, &nneg);   // DAT_14051f8fc
        }
    }
    if (deform && solid) {
        if (i32(o, 0x2098) == -1) {
            if (is3d(step) && type() != 4) {
                out("WARNING: TRGVOL has not been set. Volume\n");
                out("         compensation is not active for Obj %3d\n", id());
                code = 0x1b;
                ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
            }
        } else if (0.0 < f64(o, 0x2090)) {
            if (!is3d(step)) {
                if (type() == 2 && i32(o, 0x2098) != -1) {
                    *err = 1;
                    out("ERROR  : Volume compensation should be deactivated for porous object %3d\n", id());
                    code = 0x2f;
                    ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
                }
                if (type() == 4) {
                    out("WARNING: Volume compensation is not valid for elasto-plastic object %3d\n", id());
                    code = 0x2e;
                    ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
                }
            } else {
                if (type() == 2 && i32(o, 0x2098) != 2) {
                    out("WARNING: Volume compensation needs to be activated on remeshing time for porous object %3d\n",
                        id());
                    code = 0x2d;
                    ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
                }
                if (type() == 4) {
                    out("WARNING: Volume compensation is not valid for elasto-plastic object %3d\n", id());
                    code = 0x2e;
                    ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
                }
            }
            const double vol = obj_volume(obj, 1);                  // local_1c0
            if (vol != 0.0) {
                const double diff = std::fabs(vol - f64(o, 0x2090)); // local_1b8
                if (diff <= vol * 0.1 || u8(o, 0x3f20) == 0) {      // DAT_14051bef8
                    if (vol * 0.006 < diff && u8(o, 0x3f20) != 0) {  // DAT_140522f80
                        *warn = 1;
                        out("WARNING: Volume compensation activated for Obj %3d\n", id());
                        out("         but target volume is significantly\n");
                        out("         different\n");
                        code = 0x1d;
                        ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
                    }
                } else {
                    *err = 1;
                    out("ERROR  : Volume compensation activated for Obj %3d\n", id());
                    out("         but target volume is completely\n");
                    out("         different\n");
                    code = 0x27;
                    ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
                }
            }
        } else {
            *err = 1;
            out("ERROR  : Volume compensation activated for Obj %3d\n", id());
            out("         with an invalid value for the volume\n");
            code = 0x1c;
            ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
        }
        if (f64(o, 0x2dc8) == 0.0) {
            out("WARNING: Interference depth of Obj %3d is not set.\n", id());
            out("         Excessive mesh penetration may occour\n");
            *warn = 1;
            code = 0x21;
            ext::fun_14001cd90(&code, "RMDPTH", obj_no, &val);
        }
        if (i32(o, 0x2098) != -1 && i32(step, 0x850) == 4) {
            *err = 1;
            out("ERROR  : Volume compensation should be deactivated for plane stress geometry %3d\n", id());
            code = 0x30;
            ext::fun_14001cd90(&code, "TRGVOL", obj_no, &val);
        }
    }
    if (deform && solid) {
        if (window_unset(add(o, 0x2d10)) && ext::fun_140189580(add(o, 0x2d10)) == 0) {
            *err = 1;
            out("ERROR  : No mesh window has been defined for object %3d\n", id());
            code = 0x4b;
            ext::fun_14001cd90(&code, "MGWUSR", obj_no, &val);
        }
        if (is2d(step) && i32(o, 0x2d80) == 0) {
            bool ok = 0 < i32(o, 0x2d84);   // local_1b0
            if (f64(o, 0x2d40) == kOne && 0 < ext::fun_140189580(add(o, 0x2d10)) && window_unset(add(o, 0x2d10)))
                ok = true;
            if (!ok) {
                *err = 1;
                out("ERROR  : Absolute mesh density control has been activated with invalid settings for object %3d.\n",
                    id());
                code = 0x51;
                ext::fun_14001cd90(&code, "MESH", obj_no, &val);   // DAT_140521384
            }
        }
    }
    if (deform && solid) {
        double fmax = -1e+21;       // local_180 (DAT_140522f78)
        double fmin = 1e+21;        // local_170 (DAT_140522f70)
        double t = 0.0;             // local_178
        bool beyond = false;        // local_190
        std::int32_t a = 0;         // local_19c
        std::int32_t mno = 0;       // local_1a0
        ext::fun_1401fb270(o, &a, &mno);
        void* mat = ext::fun_1401d7060(step, mno);   // local_1a8
        if (mat != nullptr && !ext::fun_14028d0a0(mat) && i32(mat, 0x50) == 1 && i32(mat, 0x7c) == 3) {
            for (std::int32_t k = 0; k < i32(mat, 0x9c); ++k) {
                t = static_cast<const double*>(ptr(mat, 0xb0))[k];
                if (fmax < t) fmax = t;
                if (t < fmin) fmin = t;
            }
            double tmin;   // local_198
            double tmax;   // local_188
            obj_temperature_range(obj, &tmin, &tmax);
            if (tmin < fmin || fmax < tmax) beyond = true;
        }
        if (beyond) {
            *warn = 1;
            out("WARNING: Nodal temperature for Object %3d\n", id());
            out("         is beyond the range of flow stress data\n");
            code = 9;
            ext::fun_14001cd90(&code, "NDTMP", obj_no, &val);
        }
    }
    {
        char count = 1;            // local_270
        std::int32_t nbad = 0;     // local_2d4
        const bool set = obj_node_temperatures(obj, &count, &nbad);
        if (deform && !set) {
            out("WARNING: NDTMP Not Defined for Object %6d\n", id());
            code = 8;
            ext::fun_14001cd90(&code, "NDTMP", obj_no, &val);
        }
        if (0 < nbad) {
            out("WARNING: Temperature for %6d nodes of Object %3d\n", nbad, id());
            out("         less than or equal to ZERO \n");
            code = 10;
            ext::fun_14001cd90(&code, "NDTMP", obj_no, &nbad);
        }
    }
    {
        bool bad = false;          // local_244
        for (std::int32_t k = 0; k < i32(o, 0x884); ++k) {
            const double rho = elem_block_density(add(o, 0x9e0))[k];   // local_158
            if (type() == 2 || type() == 4 || type() == 3) {
                if (rho < 0.0 || std::fabs(rho) <= kTol) {
                    bad = true;
                    break;
                }
            } else if (kTol < std::fabs(rho - kOne)) {
                bad = true;
                break;
            }
        }
        if (bad) {
            if (type() == 2 || type() == 4 || type() == 3) {
                out("ERROR  : The density for Object %3d must be greater than zero.\n", id());
                val = 0;
            } else {
                out("ERROR  : The density for Object %3d must be equal to 1.\n", id());
                val = 1;
            }
            code = 0x52;
            ext::fun_14001cd90(&code, "DENSTY", obj_no, &val);
        }
    }
    {
        // NBCD codes < 0 name a contact object (201..299 -> 1..99)
        std::int32_t cobj = 0;      // local_2c0
        std::int32_t c = 0;         // local_2c4
        std::int32_t node = 0;      // local_274
        const std::int32_t ndof = i32(lvar7, 0x1644);
        for (i = 0; i < i32(o, 0x880) * ndof; ++i) {
            auto* bcc = static_cast<std::int32_t*>(ptr(o, 0x990));
            c = bcc[i];
            cobj = c;
            if (c < 0) {
                cobj = -c;
                if (200 < cobj && cobj < 300) cobj = cobj - 200;
                void* other = ext::fun_14017aaf0(add(step, 0x1380), cobj);   // local_288
                node = i / ndof + 1;
                if (other == nullptr) {
                    bcc[i] = 0;
                    out("Object %d Node %d has wrong NBCD Code(%d). Initialized.\n", id(), node, c);
                } else if (i32(other, 0xc) == 7) {
                    bcc[i] = 0;
                    out("Object %d Node %d has wrong NBCD Code(%d). Initialized.\n", id(), node, c);
                }
                if (static_cast<std::int32_t*>(ptr(o, 0x990))[i] != 0) {
                    const std::int32_t rel = step_inter_object_relation(static_cast<DB_STEP_DATA*>(step), cobj, id()); // local_150
                    if (rel == -0xc9) {
                        *err = 1;
                        out("ERROR  : Contact BCC for node %6d of object %3d is specified,\n", node, id());
                        out("         but inter-object relation between object %3d and object %3d is not specified.\n",
                            id(), cobj);
                        code = 0x2a;
                        ext::fun_14001cd90(&code, "BCCDEF", obj_no, &cobj);
                        break;
                    }
                    if (rel == -0xca) {
                        *err = 1;
                        out("ERROR  : Contact BCC for node %6d of object %3d is specified,\n", node, id());
                        out("         but inter-object relation between object %3d and object %3d is not specified as "
                            "slave-master.\n",
                            id(), cobj);
                        code = 0x2b;
                        ext::fun_14001cd90(&code, "BCCDEF", obj_no, &cobj);
                        break;
                    }
                }
            }
        }
    }
    {
        std::int32_t n_def = 0, n_tm = 0, n_at = 0, n_rh = 0;   // local_2b4, 278, 25c, 254
        const bool cleaned = obj_clean_edge_bcs(obj, &n_def, &n_tm, &n_at, &n_rh);   // local_2c8
        obj_remove_unused_symmetry_planes(obj);
        if (cleaned) {
            if (0 < n_def) {
                out("INFO   : Invalid Edge Boundary Condition\n");
                out("         %d ECDEFN of Object %3d has(have) been removed.\n", n_def, id());
                code = 0x3a;
                ext::fun_14001cd90(&code, "ECDEFN", obj_no, &n_def);
            }
            if (0 < n_tm) {
                out("INFO   : Invalid Edge Boundary Condition\n");
                out("         %d ECTMFN of Object %3d has(have) been removed.\n", n_tm, id());
                code = 0x3b;
                ext::fun_14001cd90(&code, "ECTMFN", obj_no, &n_tm);
            }
            if (0 < n_at) {
                out("INFO   : Invalid Edge Boundary Condition\n");
                out("         %d ECATFN of Object %3d has(have) been removed.\n", n_at, id());
                code = 0x3c;
                ext::fun_14001cd90(&code, "ECATFN", obj_no, &n_at);
            }
            if (is2d(step) && 0 < n_rh) {
                out("INFO   : Invalid Edge Boundary Condition\n");
                out("         %d ECRHFN of Object %3d has(have) been removed.\n", n_rh, id());
                code = 0x3d;
                ext::fun_14001cd90(&code, "ECRHFN", obj_no, &n_rh);
            }
        }
    }

    char fn_any = 0;                 // local_27c
    std::int32_t n_fn = ext::fun_140136a10(add(o, 0x2e68));   // local_298
    char* fn_used = nullptr;         // local_2a0
    if (0 < n_fn) {
        fn_used = static_cast<char*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n_fn + 1))));
        for (std::int32_t k = 0; k <= n_fn; ++k) fn_used[k] = 0;
    }
    char sym_any = 0;                // local_260
    std::int32_t n_sym = 0;          // local_28c
    char* sym_used = nullptr;        // local_2a8
    if (is3d(step)) n_sym = plane_list_max_id(add(o, 0x2e80));
    if (0 < n_sym) {
        sym_used = static_cast<char*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n_sym + 1))));
        for (std::int32_t k = 0; k <= n_sym; ++k) sym_used[k] = 0;
    }
    {
        const bool contact_dead = false;   // local_258: cleared and never set
        bool lagr = false;                 // local_2b0
        bool code4 = false;                // local_2c7
        bool code5 = false;                // local_28f
        if (ext::fun_1401f3c30(sc) != 0 && i32(step, 0x848) == 1) lagr = true;
        for (i = 0; i < i32(o, 0x880) * i32(lvar7, 0x1644); ++i) {
            const std::int32_t c = static_cast<const std::int32_t*>(ptr(o, 0x990))[i];
            if (lagr) {
                if (c == 4) {
                    at = i;
                    code4 = true;
                    break;
                }
                if (c == 5) {
                    at = i;
                    code5 = true;
                    break;
                }
            }
        }
        auto bccdef_line = [&]() {
            out("         BCCDEF(%6d) of Object %3d is %8d\n", val, id(), static_cast<const std::int32_t*>(ptr(o, 0x990))[at]);
        };
        if (code4) {
            val = at / 3 + 1;
            out("ERROR  : Invalid Boundary Condition\n");
            out("         Begin suface(code=4) cannot be defined in 'Lagrangian Incremental' run.\n");
            bccdef_line();
            *err = 1;
            code = 0x42;
            ext::fun_14001cd90(&code, "BCCDEF", obj_no, &val);
        }
        if (code5) {
            val = at / 3 + 1;
            out("ERROR  : Invalid Boundary Condition\n");
            out("         Free suface(code=5) cannot be defined in 'Lagrangian Incremental' run.\n");
            bccdef_line();
            *err = 1;
            code = 0x43;
            ext::fun_14001cd90(&code, "BCCDEF", obj_no, &val);
        }
        if (contact_dead) {
            val = at / 3 + 1;
            out("ERROR  : Invalid Contact Boundary Condition\n");
            bccdef_line();
            *err = 1;
            code = 0x13;
            ext::fun_14001cd90(&code, "BCCDEF", obj_no, &val);
        }
    }
    if (is3d(step)) {
        bool bad = false;   // local_144
        const std::int32_t ndof = i32(lvar7, 0x1644);
        for (i = 0; i < i32(o, 0x880); ++i) {
            bool vel = false;   // local_143
            bool sym = false;   // local_142
            for (std::int32_t d = 0; d < ndof; ++d) {
                const std::int32_t c = static_cast<const std::int32_t*>(ptr(o, 0x990))[i * 3 + d];
                if (c == 1) vel = true;
                if (c == 6) sym = true;
            }
            if (vel && sym) {
                for (std::int32_t d = 0; d < ndof; ++d) {   // local_100
                    const std::int32_t c = static_cast<const std::int32_t*>(ptr(o, 0x990))[i * 3 + d];   // local_fc
                    if (c == 1) {
                        double e[3] = {0.0, 0.0, 0.0};   // local_120
                        for (std::int32_t k = 0; k < ndof; ++k) e[k] = d == k ? kOne : 0.0;
                        for (std::int32_t k = 0; k < ndof; ++k) {   // local_104
                            if (d != k && static_cast<const std::int32_t*>(ptr(o, 0x990))[i * 3 + k] == 6) {
                                const std::int32_t pid = static_cast<const std::int32_t*>(ptr(o, 0x998))[i * 3 + k];
                                void* plane = ext::fun_1402580f0(add(o, 0x2e80), pid);   // local_f8
                                if (plane != nullptr) {
                                    double n[3];   // local_138
                                    plane_direction(plane, n);
                                    const double dot = e[0] * n[0] + e[1] * n[1] + e[2] * n[2];   // local_f0
                                    if (0.999 < std::fabs(dot)) {   // DAT_140522f68
                                        at = i;
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (bad) break;
                    }
                }
            }
        }
        if (bad) {
            val = at + 1;
            out("ERROR  : Invalid boundary condition BCCDEF(%6d) for object %3d\n", val, id());
            out("         Velocity and symmetry plane cannot be defined together on the same node.\n");
            *err = 1;
            code = 0x44;
            ext::fun_14001cd90(&code, "BCCDEF", obj_no, &val);
        }
    }
    if (obj_has_heat_exchange(obj) && heat && !obj_has_heat_exchange_bc(obj)) {
        val = 0;
        *err = 1;
        out("ERROR  : Heat Exchange BCC for object %3d has not been defined.\n", id());
        code = 0x15;
        ext::fun_14001cd90(&code, "ECCTMP", obj_no, &val);
    }
    if (obj_has_diffusion(obj) && diffus && !obj_has_diffusion_bc(obj)) {
        val = 0;
        *err = 1;
        out("ERROR  : Diffusion BCC for object %3d has not been defined.\n", id());
        code = 0x16;
        ext::fun_14001cd90(&code, "ECCATM", obj_no, &val);
    }
    for (std::int32_t k = 0; k < i32(o, 0x88c); ++k) {   // local_e8
        std::int32_t e = 0;                               // local_e4
        if (!is3d(step)) {
            std::int32_t edge = k + 1;
            e = obj_edge_index(obj, &edge);
        } else {
            e = k;
        }
        auto* ecode = static_cast<const std::int32_t*>(ptr(o, 0xae8));
        auto* efn = static_cast<std::int32_t*>(ptr(o, 0xaf0));
        if (deform) {
            if (!is3d(step)) {
                if (0 < efn[e] && ecode[e] != 9) obj_mark_function(obj, fn_used, &efn[e], &fn_any, &n_fn);
            } else if (0 < efn[e]) {
                if (ecode[e] == 6)
                    obj_mark_function(obj, sym_used, &efn[e], &sym_any, &n_sym);
                else if (ecode[e] != 9)
                    obj_mark_function(obj, fn_used, &efn[e], &fn_any, &n_fn);
            }
        }
        auto* tcode = static_cast<const std::int32_t*>(ptr(o, 0xb00));
        auto* tfn = static_cast<std::int32_t*>(ptr(o, 0xb08));
        if (heat && 0 < tfn[e] && tcode[e] != 4) obj_mark_function(obj, fn_used, &tfn[e], &fn_any, &n_fn);
    }
    if (fn_any != 0) {
        if (n_fn == 0) {
            out("ERROR  : Boundary Function of Object %6d Undefined\n", id());
            *err = 1;
            code = 0x14;
            ext::fun_14001cd90(&code, "BCCFNC", obj_no, &val);
        } else {
            for (std::int32_t f = 1; f <= n_fn; ++f) {   // local_d8
                if (fn_used[f] == 0) continue;
                void* fn = ext::fun_1401368c0(add(o, 0x2e68), f);   // local_e0
                bool undef = false;                                 // local_d4
                if (fn == nullptr)
                    undef = true;
                else if (ext::fun_14012ff20(fn) == 0)
                    undef = true;
                if (undef) {
                    out("ERROR  : Boundary Function # %6d of Object %6d\n", f, id());
                    out("         Undefined\n");
                    *err = 1;
                    code = 0xb;
                    ext::fun_14001cd90(&code, "BCCFNC", obj_no, &f);
                }
            }
        }
    }
    if (0 < n_fn) std::free(fn_used);
    if (is3d(step)) {
        if (sym_any != 0) {
            if (n_sym == 0) {
                out("ERROR  : Symmetrical Plane Boundary Condition of Object %6d Undefined\n", id());
                *err = 1;
                code = 0x3e;
                ext::fun_14001cd90(&code, "SYMSUF", obj_no, &val);
            } else {
                for (std::int32_t p = 1; p <= n_sym; ++p) {   // local_c8
                    if (sym_used[p] == 0) continue;
                    void* plane = ext::fun_1402580f0(add(o, 0x2e80), p);   // local_d0
                    bool undef = false;                                    // local_c4
                    if (plane == nullptr)
                        undef = true;
                    else if (plane_direction_zero(plane))
                        undef = true;
                    if (undef) {
                        out("ERROR  : Symetrical Plane Boundary Condition # %6d of Object %6d\n", p, id());
                        out("         Undefined\n");
                        *err = 1;
                        code = 0x3f;
                        ext::fun_14001cd90(&code, "SYMSUF", obj_no, &p);
                    }
                }
            }
        }
        if (0 < n_sym) std::free(sym_used);
        if (rotsym_angle_set(add(o, 0x1ea0))) {
            if (!obj_has_rotsym_code(obj)) {
                out("INFO   : Rotational symmetry boundary conditions have not been defined for Object %6d.\n", id());
                out("         So the rotational symmetry angle (%g) has been reset to zero.\n", f64(o, 0x2058));
                code = 0x47;
                ext::fun_14001cd90(&code, "ROTSYM", obj_no, &val);
                rotsym_angle_reset(add(o, 0x1ea0));
            } else if (!rotsym_axis_set(add(o, 0x1ea0))) {
                out("ERROR  : Rotational symmetry boundary conditions have been defined for Object %6d.\n", id());
                out("         But rotational symmetry axis is undefined.\n");
                *err = 1;
                code = 0x46;
                ext::fun_14001cd90(&code, "ROTSYM", obj_no, &val);
            }
        }
    }
    if (is3d(step) && (i32(step, 0x848) == 4 || i32(step, 0x848) == 7) && type() == 1) {
        std::int32_t free_code = 5;   // local_50
        if (!obj_has_free_surface_code(obj, &free_code)) {
            out("ERROR  : Workpiece (Object %d) needs to have a free surface\n", id());
            *err = 1;
            code = 0x57;
            ext::fun_14001cd90(&code, "BCC", obj_no, &val);   // DAT_14051f9c4
        }
        std::int32_t die = i32(step, 0x8b8);   // local_c0
        if (!obj_touches_object(obj, die)) {
            out("ERROR  : Workpiece (Object %1) needs to have a contact with the primary die (Object %2)\n", id(), die);
            *err = 1;
            code = 0x56;
            ext::fun_14001cd90(&code, "BCC", obj_no, &die);   // DAT_14051fc24
        }
    }
    if (transf && 0 < i32(o, 0x3590)) {
        obj_undefined(obj, "VOLFC", 2, '!', err);
        obj_undefined(obj, "VOLFS", 2, '!', err);
        obj_undefined(obj, "DVOLF", 2, '!', err);
        obj_undefined(obj, "TICF", 2, '!', err);   // DAT_140521f64
        obj_undefined(obj, "GRAIN", 2, '!', err);
    }
    if (transf && 0 < i32(o, 0x3590)) {
        const std::int32_t nph = i32(lvar7, 0x16bc);   // local_b4
        std::int32_t found = 0;                        // local_bc
        std::int32_t elem = 0;                         // local_b8
        const double* vf = elem_block_volume_fractions(add(o, 0x9e0));   // local_b0
        for (std::int32_t e = 0; e < i32(o, 0x884); ++e) {   // local_a0
            double sum = 0.0;                                // local_a8
            for (i = 0; i < nph; ++i) sum = sum + vf[e * nph + i];
            if (sum <= kVolFrac) {
                elem = e + 1;
                found = 1;
                break;
            }
        }
        if (found == 1) {
            out("WARNING: Transformation is on. But Object %d, Element %d\n", id(), elem);
            out("         has no volume fraction data.\n");
            code = 0x5a;
            ext::fun_14001cd90(&code, "VOLF", obj_no, &elem);   // DAT_140521fe0
        }
    }
    if (grain && type() != 0 && 0 < i32(lvar7, 0x16cc)) {
        const std::int32_t w = i32(lvar7, 0x16cc);   // local_8c
        std::int32_t found = 0;                      // local_9c
        std::int32_t elem = 0;                       // local_90
        const double* g = ext::fun_140184520(add(o, 0x9e0));   // local_98
        for (std::int32_t e = 0; e < i32(o, 0x884); ++e) {      // local_80
            const double a = g[e * w + 1];
            const double b = g[e * w + 2];
            if ((a <= kTiny && kTiny != a) || (b <= kTiny && kTiny != b)) {
                elem = e + 1;
                found = 1;
                break;
            }
        }
        if (found == 1) {
            out("WARNING: Grain(Avrami) is on. But Object %d, Element %d\n", id(), elem);
            out("         has no grain size data.\n");
            code = 0x5b;
            ext::fun_14001cd90(&code, "GRAIN", obj_no, &elem);
        }
    }
    obj_undefined(obj, "CRPTIM", 2, '!', err);
    obj_undefined(obj, "HDNEST", 2, '!', err);
    if (diffus) obj_undefined(obj, "DATOM", 2, '!', err);
    if (electr) obj_undefined(obj, "VOTAGE", 2, '!', err);
}

} // namespace def::dbio::objcheck
