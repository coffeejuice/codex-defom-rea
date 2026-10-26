// GENDB check 1, simulation control (packet p4-04): 1401ee940, 1401ef470 and their helpers.
// Messages go to stdout (__iob_func() + 0x30); the 14001cd60 calls are the binary's message
// hook (an empty function in this build) and keep the original's codes.
// FP: every test is a single comparison; the COMISD/UCOMISD branches in out/p404/p404_lst.txt
// give C++'s ordered semantics for <, <=, > and ==, != (NaN: false, true).
#include <cstdio>
#include <vector>

#include <defdbio/gendb/checks_p4_04.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "ext_p4_04.hpp"

namespace def::dbio::gendb {

namespace {
// @data 1405d4680  STYPE names. Entries 8..12 are the next table in .rdata (GEOTYP names), which
// the binary reads for those values; larger values are not reachable from a valid model.
const char* const kSimTypeNames[13] = {
    "Undefined", "Lagrangian incremental", "Discrete", "Steady-state machining",
    "Steady-state extrusion", "Steady-state rolling", "Ring rolling", "ALE extrusion",
    "Undefined", "Axisymmetry", "Plain strain", "Torsion", "Plane stress",
};

void hook(std::uint32_t code, const char* keyword, const std::uint32_t* value, const std::int32_t* aux)
{
    const std::int32_t c = static_cast<std::int32_t>(code);
    ext::fun_14001cd60(&c, keyword, value, aux);
}

FILE* out() { return stdout; } // __iob_func() + 0x30
} // namespace

// @src 1401d9010
const char* sim_type_name(const Cls_1401e7b10& sim) { return kSimTypeNames[sim.stype]; }

// @src 1401ee5d0
bool sim_ctrl_step_size_defined(const Cls_1401e7b10& sim)
{
    bool r = true;
    if (sim.dttype.ndim == 0) {
        if (sim.dsmax <= 0.0 && sim.dtmax1 <= 0.0) { // 1401ee5f5 / 1401ee608: COMISD 0, x; JC
            r = false;
        }
    } else if (dynary_values_zero(&sim.dttype)) {
        r = false;
    }
    return r;
}

// @src 1401ee940
void sim_ctrl_check(Cls_1401e7b10& sim, std::int32_t* err, std::int32_t* warn)
{
    std::uint32_t value = 0;   // local_28[0]
    std::int32_t followed = 0; // local_14[0]
    DB_STEP_DATA* const owner = sim.owner;

    const char* name = sim_type_name(sim);
    def::rt::crt::fprintf(out(), "INFO   : Simulation Type : %s\n", name);
    value = static_cast<std::uint32_t>(sim.stype);
    hook(0x36, "STYPE", &value, &followed);

    if (sim.trans[1] == 1 && sim.pdie1 == 0) {
        sim.pdie1 = 1;
        def::rt::crt::fprintf(out(), "WARNING: PDIE(0) not defined; Object 1 is assumed\n");
        hook(7, "PDIE", &value, &followed); // str 14052da94
    }
    if (sim.nstart == 0) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : NSTART must not be 0\n");
        hook(0x3c, "NSTEP", &value, &followed);
    }
    if (sim.nstep < 1) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : NSTEP must be > 0\n");
        hook(9, "NSTEP", &value, &followed);
    }
    if (sim.stpinc < 1) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : STPINC must be > 0\n");
        hook(10, "NSTEP", &value, &followed);
    }
    if (!sim_ctrl_step_size_defined(sim)) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : DSMAX or DTMAX must be defined and > 0\n");
        hook(5, "DSMAX", &value, &followed);
    }
    if (0.0 < sim.dsmax && 0.0 < sim.dtmax1) { // 1401eeb8d / 1401eeba4: COMISD x, 0; JBE
        def::rt::crt::fprintf(out(), "WARNING: DSMAX and DTMAX cannot be defined together. DSMAX will be used.\n");
        hook(0x2d, "DSMAX", &value, &followed);
        sim.dtmax1 = 0.0;
    }
    if (ext::fun_1401ee580(&sim) == 0 && !primary_die_moves(*owner) && !primary_die_follows(*owner, &followed)) {
        *err = 1;
        if (followed < 1 || 100 < followed) {
            // 1401eec89: the %d argument is local_28[0], still STYPE here (the hook is empty)
            def::rt::crt::fprintf(out(),
                                  "ERROR  : If stroke per step is used, the primary die (%d) must have assigned "
                                  "translational movement.\n",
                                  value);
        } else {
            def::rt::crt::fprintf(out(), "ERROR  : Primary die's follow object movement control not defined!\n");
        }
        value = static_cast<std::uint32_t>(sim.pdie1);
        hook(0x1b, "PDIE", &value, &followed); // str 14052dc04
    }
    if (sim.trans[1] == 0) {
        if (ext::fun_1401ee580(&sim) == 1) {
            if (sim.dttype.var[0] == 2) { // +0x1f8
                *err = 1;
                def::rt::crt::fprintf(out(),
                                      "ERROR  : DTMAX cannot be defined as function of stroke for Heat Transfer\n");
                hook(0x2e, "DTMAX", &value, &followed);
            }
        } else {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : DTMAX not defined for Heat Transfer\n");
            hook(6, "DTMAX", &value, &followed);
        }
    }

    sim_ctrl_check_solver(sim, err, warn);

    if (def::rt::dimension_is_3(owner)) {
        if (sim.trans[1] == 1 && sim.dtsub <= 0.999) { // 1401eedd8: COMISD 0.999, x; JC (DAT_140522f68)
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: DTSUB for sub-stepping = %9.4g, not 1.0.\n", sim.dtsub);
            def::rt::crt::fprintf(out(), "         This can increase the time for running a\n");
            def::rt::crt::fprintf(out(), "         simulation\n");
            hook(0xf, "DTSUB", &value, &followed);
        }
        if (sim.trans[1] == 1) {
            if (sim.dplen == 0.0) {
                def::rt::crt::fprintf(out(), "WARNING: DPLEN  = 0.00, auto-time stepping not\n");
                def::rt::crt::fprintf(out(), "         activated\n");
                hook(0x10, "DPLEN", &value, &followed);
            } else if (1.0 <= sim.dplen) { // 1401eeefb: COMISD x, 1.0; JC (DAT_140509c90)
                def::rt::crt::fprintf(out(), "WARNING: DPLEN >= 1.00, can cause severe distortion\n");
                def::rt::crt::fprintf(out(), "         of the mesh during the simulation\n");
                hook(0x11, "DPLEN", &value, &followed);
            }
        }
    }
    if (0 < sim.envtmp.ndim && def::model::dynary_value_count(&sim.envtmp) < 1) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Invalid Function Definition for Environment Temp.\n");
        hook(0xb, "ENVTM", &value, &followed);
    }
    if (sim.trans[0] == 1 && 0 < sim.cnvcof.ndim && def::model::dynary_value_count(&sim.cnvcof) < 1) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Invalid Function Definition for Convection Coef.\n");
        hook(0xc, "CNVCO", &value, &followed);
    }
    if (sim.trans[3] == 1) {
        if (0 < sim.envatm.ndim && def::model::dynary_value_count(&sim.envatm) < 1) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : Invalid Function Definition for Environment Carbon Potential\n");
            hook(0xd, "ENVATM", &value, &followed);
        }
        if (0 < sim.acvcof.ndim && def::model::dynary_value_count(&sim.acvcof) < 1) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : Invalid Function Definition for Surface Reaction Coef.\n");
            hook(0xe, "ACVCOF", &value, &followed);
        }
    }
    if (sim.trans[4] == 1 && sim.trans[0] == 0) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Grain Model requires Heat Transfer to be on\n");
        hook(0x13, "TRANS", &value, &followed);
    }
    if (sim.htmthd == 2) {
        if (sim.envmpr == 0.0) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : Undefined Keyword ENVMPR\n");
            hook(0, "ENVMPR", &value, &followed);
        }
        if (sim.envmpt == 0.0) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : Undefined Keyword ENVMPT\n");
            hook(0, "ENVMPT", &value, &followed);
        }
    }
    if (sim.trans[0] == 1) {
        if (0.0 < sim.dtpmax1 && (sim.dtpmax2_5[0] == 0.0 || sim.dtpmax2_5[1] == 0.0)) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : DTPMAX must have min/max time step defined\n");
            hook(0x12, "DTPMA", &value, &followed);
        }
        if (any_mechanical_press(*owner) && ext::fun_1401ee580(&sim) == 0 && sim.dtpmax2_5[1] == 0.0) {
            def::rt::crt::fprintf(out(), "WARNING: Maximum time step should be specified\n");
            def::rt::crt::fprintf(out(), "         to avoid a possible huge time step\n");
            def::rt::crt::fprintf(out(), "         after the mechanical press reverse its direction.\n");
            hook(0x30, "DTPMA", &value, &followed);
        }
    }
    std::int32_t* const st = sim.stnout; // local_20 = S + 0x11c
    const std::int32_t sum = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(st[0]) + static_cast<std::uint32_t>(st[1]) + static_cast<std::uint32_t>(st[2]) +
        static_cast<std::uint32_t>(st[3]) + static_cast<std::uint32_t>(st[4]) + static_cast<std::uint32_t>(st[5]) +
        static_cast<std::uint32_t>(st[6]));
    if (sum < 1) {
        *warn = 1;
        st[4] = 1; // S + 300
        def::rt::crt::fprintf(out(), "WARNING: STNOUT not specified.  Total strain will be stored.\n");
        hook(0x1f, "STNOUT", &value, &followed);
    }
    if (st[4] == 0) {
        st[4] = 1;
    }
}

// @src 1401c7d70
void method_codes_2d()
{
    g_method_codes = {0, 2, 1, 0, 2, 3, 4, 5, 6, 0, 3, 2, 4, 5};
}

// @src 1401c7e10
void method_codes_3d()
{
    g_method_codes = {0, 2, 1, 0, 2, 1, 4, 5, 6, 0, 1, 2, 4, 5};
}

// @src 1401c7eb0
void method_codes_select(const std::int32_t* dim)
{
    if (*dim == 2) {
        method_codes_2d();
    } else {
        method_codes_3d();
    }
}

// @src 1401ef470
void sim_ctrl_check_solver(Cls_1401e7b10& sim, std::int32_t* err, std::int32_t* warn)
{
    DB_STEP_DATA* const owner = sim.owner;
    const MethodCodes& g = g_method_codes;
    std::int32_t dim = owner->dimension; // 140158e90
    method_codes_select(&dim);
    std::int32_t code = 0;
    std::uint32_t value = 0;
    std::int32_t aux = 0;
    const auto hook2 = [&](std::int32_t c, const char* keyword) {
        code = c;
        ext::fun_14001cd60(&code, keyword, &value, &aux);
    };

    if (sim.itrmth == g.itr_unset) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Iteration method for deformation analysis has not been set.\n");
        hook2(0x33, "ITRMTD");
    }
    if (sim.solmtd == g.sol_unset) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Solution method for deformation analysis has not been set.\n");
        hook2(0x34, "ITRMTD");
    }
    if (sim.solmtt == g.solt_unset) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Solution method for temperature analysis has not been set.\n");
        hook2(0x35, "ITRMTD");
    }
    if (sim.itrmth == g.itr_direct && 0 < count_elasto_plastic(*owner)) {
        *err = 1;
        def::rt::crt::fprintf(out(), "ERROR  : Direct iteration is not recommended for elasto-plastic simulations. "
                                     "Please select Newton-Raphson.\n");
        hook2(0x24, "ITRMTD");
    }
    if (sim.solmtd == g.sol_cg) {
        if (any_movement_type_5(*owner)) {
            *err = 1;
            def::rt::crt::fprintf(out(),
                                  "ERROR  : Conjugate-gradient solver should not be used with sliding die movement control.\n");
            hook2(0x27, "SOLMTD");
        }
        if (sim.stype != 6 && any_force_movement(*owner)) {
            *err = 1;
            def::rt::crt::fprintf(out(),
                                  "ERROR  : Conjugate-gradient solver is not recommended for force movement control.\n");
            hook2(0x28, "SOLMTD");
        }
        if (sim.stype != 6 && any_torque_movement(*owner)) {
            *err = 1;
            def::rt::crt::fprintf(out(),
                                  "ERROR  : Conjugate-gradient solver is not recommended for torque movement control.\n");
            hook2(0x31, "SOLMTD");
        }
        if (any_mesh_kind(*owner, false) && any_rotational_symmetry_bcc(*owner)) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR_DEF: Conjugate-gradient solver is not recommended for brick mesh with "
                                         "rotational symmetry BCC.\n");
            hook2(0x32, "SOLMTD");
        }
        std::int32_t type = 3;
        const std::int32_t n_elastic = count_objects_of_type(*owner, &type);
        if (0 < n_elastic && 0.001 < sim.cvgerr[0]) { // 1401ef8b6: COMISD x, DAT_14051f1e8; JBE
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: CVGERR for velocity is too large for reliable simulation result for "
                                         "elastic objects.  A value of 0.001 or less is recommended.\n");
            hook2(0x37, "STNOUT");
        }
        if (0 < n_elastic && 0.01 < sim.cvgerr[1]) { // 1401ef920: COMISD x, DAT_14051f258; JBE
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: CVGERR for force is too large for reliable simulation result for "
                                         "elastic objects.  A value of 0.01 or less is recommended.\n");
            hook2(0x38, "STNOUT");
        }
    }
    if (sim.solmtd == g.sol_explicit || sim.solmtt == g.sol_explicit) {
        if (count_elasto_plastic(*owner) == 0) {
            *err = 1;
            def::rt::crt::fprintf(out(), "ERROR  : Explicit solver is recommended for Elasto-plastic objects only.\n");
            hook2(0x39, "SOLMTD");
        } else {
            LLOBJ_DATA* first = ext::fun_1401d6ea0(*owner, 1);
            const std::int32_t first_id = object_u3980(first);       // the binary dereferences it unchecked
            (void)def::model::step_material_by_id(*owner, first_id); // local_70, not used further
            std::int32_t cap = ext::fun_14029c410(&owner->materials);
            std::int32_t n = 0;
            if (cap < 1) {
                cap = 100;
            }
            // operator new(4 * cap) (thunk 1404797d8; never freed). Zeroed here: the original reads
            // entries [cap, n) uninitialised when an object has more distinct ids than the largest
            // material number.
            std::vector<std::int32_t> ids(static_cast<std::size_t>(cap) + 0, 0);
            (void)object_material_ids(first, &n, ids.data(), &cap, owner);
            if (static_cast<std::size_t>(n) > ids.size()) {
                ids.resize(static_cast<std::size_t>(n), 0);
            }
            for (std::int32_t i = 0; i < n; ++i) {
                const def::model::Material* m = def::model::step_material_by_id(*owner, ids[i]);
                if (m != nullptr && m->masden.ndim == 0 && m->masden.value == 0.0) {
                    *err = 1;
                    def::rt::crt::fprintf(out(), "ERROR  : Explicit solver is recommended for objects with Mass "
                                                 "Density defined in the material.\n");
                    hook2(0x3b, "SOLMTD");
                }
            }
        }
    }
    if (sim.solmtd == g.sol_cg && sim.itrmth == g.itr_newton) {
        std::int32_t t1 = 1;
        const std::int32_t n_plastic = count_objects_of_type(*owner, &t1);
        std::int32_t t3 = 3;
        const std::int32_t n_elastic = count_objects_of_type(*owner, &t3);
        std::int32_t t2 = 2;
        const std::int32_t n_porous = count_objects_of_type(*owner, &t2);
        if (0 < n_plastic + n_elastic + n_porous) {
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: Direct method is recommended with CG solver and plastic/elastic/"
                                         "porous object(s).\n");
            hook2(0x29, "ITRMTD");
        }
    }
    if (sim.solmtd == g.sol_cg || sim.solmtd == g.sol_sparse) {
        std::int32_t kind = 1;
        std::uint8_t any_relation = 0;
        std::uint8_t rigid_moving = 0;
        if (any_elasto_plastic_mesh_type(*owner, &kind) && any_type_4(*owner) &&
            (inter_obj_and_rigid_movement(*owner, &any_relation, &rigid_moving), any_relation != 0) &&
            rigid_moving != 0) {
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: TET(STD) is recommended.\n");
            hook2(0x2b, ""); // str 14052c7db (empty)
        }
    }
    if ((sim.solmtd == g.sol_cg || sim.solmtd == g.sol_sparse) && ext::fun_1401f3c10(&sim) != 0) {
        std::int32_t kind = 2;
        if (any_elasto_plastic_mesh_type(*owner, &kind) && any_type_4(*owner)) {
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: Brick(STD) is recommended.\n");
            hook2(0x2c, ""); // str 14052c7dc (empty)
        }
    }
    // the same block again in the binary (1401efe..): the warning prints twice
    if ((sim.solmtd == g.sol_cg || sim.solmtd == g.sol_sparse) && ext::fun_1401f3c10(&sim) != 0) {
        std::int32_t kind = 2;
        if (any_elasto_plastic_mesh_type(*owner, &kind) && any_type_4(*owner)) {
            *warn = 1;
            def::rt::crt::fprintf(out(), "WARNING: Brick(STD) is recommended.\n");
            hook2(0x2c, ""); // str 14052c7dd (empty)
        }
    }
}

} // namespace def::dbio::gendb
