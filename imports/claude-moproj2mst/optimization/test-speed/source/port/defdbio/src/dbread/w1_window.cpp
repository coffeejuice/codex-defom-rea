// dbread/w1_window.cpp: LLWINDEF_DATA::ReadFromDBArrays (FUN_14025de80), one window from its LLWIN
// row and the LLWRZ coordinates (packet p-dbread, helper W1). The friction block (Cls_1401fb020 at
// +8) and the DYNARY members are written at the binary's offsets:
//   +0x008 fric.u000  +0x00c fric.u004 (byte)  +0x00d fric.u005 (byte)  +0x010..0x01c fric.u008..u014
//   +0x020 fric.u018 (DYNARY)  +0x098 fric.u090 (DYNARY)  +0x110.. fric.u108[]
//   +0x380 / 0x3f8 / 0x470 / 0x4e8 t380[0..3], +0x568 / 0x5e0 / 0x658 / 0x6d0 / 0x748 / 0x7c0 t568[0..5]
// The GENDB mirror is the window writer in gendb3d/bclists.cpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbread/w1.hpp>

#include <cmath>
#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "dbread/w1_access.hpp"

namespace def::dbio::dbread {

using w1::at;
using w1::dyn;

namespace {

// the friction law of one window side: code 0 constant, 1..4 a table of one variable, 5 a table
// of three variables with the point counts from the row, 6..8 two constants (cases at
// 14025e9xx / 14025ecxx / 14025f0xx); `dyn_off` is the DYNARY (+0x20 or +0x98)
void friction_side(model::LLWINDEF_DATA* w, std::int32_t code, std::ptrdiff_t dyn_off, double cval,
                   const double* tri, std::ptrdiff_t c6, std::ptrdiff_t c7, std::ptrdiff_t c8)
{
    auto I = [w](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(w, off); };
    auto D = [w](std::ptrdiff_t off) -> double& { return at<double>(w, off); };
    const std::ptrdiff_t nd = dyn_off + 4;    // ndim
    const std::ptrdiff_t v0 = dyn_off + 0x10; // var[0]
    switch (code) {
    case 0:
        I(nd) = 0;
        D(dyn_off + 8) = cval;
        break;
    case 1:
        I(nd) = 1;
        I(v0) = 1;
        break;
    case 2:
        I(nd) = 1;
        I(v0) = 4;
        break;
    case 3:
        I(nd) = 1;
        I(v0) = 3;
        break;
    case 4:
        I(nd) = 1;
        I(v0) = 0x15;
        break;
    case 5:
        I(nd) = 3;
        I(v0) = 4;
        I(v0 + 4) = 3;
        I(v0 + 8) = 0x15;
        I(dyn_off + 0x1c) = static_cast<std::int32_t>(tri[0]);
        I(dyn_off + 0x20) = static_cast<std::int32_t>(tri[1]);
        I(dyn_off + 0x24) = static_cast<std::int32_t>(tri[2]);
        break;
    case 6:
        I(nd) = 0;
        D(c6) = tri[0];
        D(c6 + 8) = tri[1];
        break;
    case 7:
        I(nd) = 0;
        D(c7) = tri[0];
        D(c7 + 8) = tri[1];
        break;
    case 8:
        I(nd) = 0;
        D(c8) = tri[0];
        D(c8 + 8) = tri[1];
        break;
    default:
        break;
    }
}

} // namespace

// @src 14025de80
void read_window(model::LLWINDEF_DATA* w, double* p, double* llwrz, std::int32_t* iifnc, double* llfnc)
{
    auto I = [w](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(w, off); };
    auto D = [w](std::ptrdiff_t off) -> double& { return at<double>(w, off); };
    auto B = [w](std::ptrdiff_t off) -> std::uint8_t& { return at<std::uint8_t>(w, off); };
    auto F = [w, iifnc, llfnc](std::ptrdiff_t off, std::int32_t fn) {
        char with_shape = 0;
        read_func_from_db_array(dyn(w, off), iifnc, llfnc, fn, &with_shape);
    };
    switch (static_cast<std::int32_t>(p[0]) - 1) {
    case 0: w->u174 = 1; break;
    case 1: w->u174 = 2; break;
    case 2: w->u174 = 3; break;
    case 3: w->u174 = 4; break;
    case 4: w->u174 = 5; break;
    case 5: w->u174 = 6; break;
    case 6: w->u174 = 7; break;
    case 7: w->u174 = 8; break;
    case 8: w->u174 = 10; break;
    default:
        rt::crt::printf("ERROR in LLWINDEF_DATA::ReadFromDBArrays(): not implemented windef type\n");
    }
    if (w->u174 != 5 && w->u174 != 6 && w->u174 != 7) {
        if (!gendb::window_owner_is_2d(w)) { // 14025c830
            switch (static_cast<std::int32_t>(p[0xe])) {
            case 0:
            case 1: {
                w->shape = 1;
                const double d = std::fabs(p[2] - p[1]); // 140484558
                if (static_cast<std::int32_t>(d) != 7) {
                    rt::crt::printf("ERROR in LLWINDEF_DATA::ReadFromDBArrays(): cubic window nNoPoints != 8");
                    return;
                }
                const std::int32_t size = model::window_alloc_points(w, 8); // 140263cb0
                std::memcpy(w->pts, llwrz + static_cast<std::int32_t>(p[1] - 1.0) * 3,
                            static_cast<std::size_t>(static_cast<std::int64_t>(size)));
                break;
            }
            case 2: {
                w->shape = 2;
                const double d = std::fabs(p[2] - p[1]);
                if (static_cast<std::int32_t>(d) != 2) {
                    rt::crt::printf(
                        "ERROR in LLWINDEF_DATA::ReadFromDBArrays(): cylindrical window nNoPoints != 3");
                    return;
                }
                std::memcpy(&w->u1b8, &llwrz[static_cast<std::int32_t>(p[1] + 1.0) * 3], 8);
                std::memcpy(&w->u1c0, &llwrz[static_cast<std::int32_t>(p[1] + 1.0) * 3 + 1], 8);
                const std::int32_t size = model::window_alloc_points(w, 2);
                std::memcpy(w->pts, llwrz + static_cast<std::int32_t>(p[1] - 1.0) * 3,
                            static_cast<std::size_t>(static_cast<std::int64_t>(size)));
                break;
            }
            case 3: {
                w->shape = 3;
                (void)std::fabs(p[2] - p[1]); // computed, result unused
                // open: LLWINDEF_DATA's ctor port keeps +0x220 as zeroed bytes (no _OBJ_GEO3D ctor),
                // so this branch needs that ctor before it can run (see the report)
                model::OBJ_GEO3D* g = &w->geo3d; // an _OBJ_GEO3D member since main b5a954f
                std::int32_t nn_in = static_cast<std::int32_t>(p[7]);
                g->resize_nodes(&nn_in); // 1402f9600
                std::int32_t ne_in = static_cast<std::int32_t>(p[8]);
                g->resize_elems(&ne_in); // 1402f96a0
                const std::int32_t nn = g->node_count();                 // 1400034d0
                double* coords = model::geo_kw::geo3d_coords(g);         // 140006410
                std::int32_t* elems = model::geo_kw::geo3d_elems(g);     // 140005510
                std::memcpy(coords, llwrz + static_cast<std::int32_t>(p[1] - 1.0) * 3,
                            static_cast<std::size_t>(static_cast<std::int64_t>(nn * 3) << 3));
                const std::int32_t ne = g->elem_count(); // 140003500
                for (std::int32_t k = 0; k < ne << 2; k = k + 1) {
                    elems[k] = static_cast<std::int32_t>(
                        llwrz[static_cast<std::int32_t>((p[1] + static_cast<double>(nn)) - 1.0) * 3 + k]);
                }
                model::geo_kw::geo3d_fun_1402fbb30(g);
                break;
            }
            case 0xb:
            case 0xc:
            case 0xd: {
                w->shape = 0xb;
                if (p[7] == 0.0) {
                    w->u378 = 1;
                } else if (p[7] == 1.0) {
                    w->u378 = 1;
                }
                const double d = std::fabs(p[2] - p[1]);
                const std::int32_t size = model::window_alloc_points(w, static_cast<std::int32_t>(d) + 1);
                std::memcpy(w->pts, llwrz + static_cast<std::int32_t>(p[1] - 1.0) * 3,
                            static_cast<std::size_t>(static_cast<std::int64_t>(size)));
                break;
            }
            default:
                break;
            }
            std::memcpy(&w->u350[0], &p[3], 8);
            std::memcpy(&w->u350[1], &p[4], 8);
            std::memcpy(&w->u350[2], &p[5], 8);
            if (p[0xf] == 1.0) {
                w->orient = 0;
            } else if (p[0xf] < 0.0) {
                w->orient = 2;
                w->orient_id = -static_cast<std::int32_t>(p[0xf]);
            }
            if (w->shape == 1 || w->shape == 2) {
                model::window_build_faces(w); // 140267760
            }
        } else {
            const double d = std::fabs(p[2] - p[1]);
            const std::int32_t size = model::window_alloc_points(w, static_cast<std::int32_t>(d) + 1);
            std::memcpy(w->pts, llwrz + (static_cast<std::int32_t>(p[1] - 1.0) << 1),
                        static_cast<std::size_t>(static_cast<std::int64_t>(size)));
            std::memcpy(&w->u350[0], &p[4], 8);
            std::memcpy(&w->u350[1], &p[5], 8);
            if (p[3] == 1.0) {
                w->orient = 0;
            } else if (p[3] < 0.0) {
                w->orient = 2;
                w->orient_id = -static_cast<std::int32_t>(p[3]);
            }
        }
    }
    switch (w->u174 - 1) {
    case 0: {
        const std::int32_t k = static_cast<std::int32_t>(p[6]);
        if (k == 1) {
            I(8) = 1;
        } else if (k == 2) {
            I(8) = 2;
        } else if (k == 3) {
            I(8) = 3;
        } else if (k == 4) {
            I(8) = 4;
        }
        if (static_cast<std::int32_t>(p[9]) < 0) {
            B(0xc) = 1;
            I(0x10) = -static_cast<std::int32_t>(p[9]);
            D(0x28) = p[7];
        } else {
            B(0xc) = 0;
            I(0x18) = static_cast<std::int32_t>(p[9]);
            friction_side(w, I(0x18), 0x20, p[7], &p[10], 0x110, 0x120, 0x130);
            if (I(0x24) != 0) {
                F(0x20, static_cast<std::int32_t>(p[8]));
            }
        }
        if (!gendb::window_owner_is_2d(w)) {
            if (I(8) == 4) {
                if (static_cast<std::int32_t>(p[0x10]) < 0) {
                    B(0xd) = 1;
                    I(0x14) = -static_cast<std::int32_t>(p[0x10]);
                    D(0xa0) = p[0xd];
                } else {
                    B(0xd) = 0;
                    I(0x1c) = static_cast<std::int32_t>(p[0x10]);
                    friction_side(w, I(0x1c), 0x98, p[0xd], &p[0x11], 0x140, 0x150, 0x160);
                    if (I(0x9c) != 0) {
                        F(0x98, static_cast<std::int32_t>(p[0xd]));
                    }
                }
            }
        } else if (I(8) == 4) {
            if (static_cast<std::int32_t>(p[0xe]) < 0) {
                B(0xd) = 1;
                I(0x14) = -static_cast<std::int32_t>(p[0xe]);
                D(0xa0) = p[0xd];
            } else {
                B(0xd) = 0;
                I(0x1c) = static_cast<std::int32_t>(p[0xe]);
                friction_side(w, I(0x1c), 0x98, p[0xd], &p[0xf], 0x140, 0x150, 0x160);
                if (I(0x9c) != 0) {
                    F(0x98, static_cast<std::int32_t>(p[0xd]));
                }
            }
        }
        break;
    }
    case 1:
        D(0x370) = p[6];
        break;
    case 2:
    case 4: {
        if (w->u174 - 1 == 4) {
            w->u174 = 5;
        }
        if (static_cast<std::int32_t>(p[6]) == 0) {
            I(0x384) = 0;
            D(0x388) = p[7];
        } else {
            I(0x384) = 1;
            const double fn = p[7];
            I(0x390) = 1;
            F(0x380, static_cast<std::int32_t>(fn));
        }
        const std::int32_t k8 = static_cast<std::int32_t>(p[8]);
        if (k8 == 0) {
            I(0x3fc) = 0;
            D(0x400) = p[9];
        } else if (k8 == 1) {
            I(0x3fc) = 1;
            I(0x408) = 1;
            F(0x3f8, static_cast<std::int32_t>(p[9]));
        } else if (k8 == 2) {
            I(0x3fc) = 1;
            I(0x408) = 3;
            F(0x3f8, static_cast<std::int32_t>(p[9]));
        } else if (k8 == 3) {
            I(0x3fc) = 2;
            const double fn = p[9];
            I(0x408) = 1;
            I(0x40c) = 3;
            I(0x414) = static_cast<std::int32_t>(p[10]);
            I(0x418) = static_cast<std::int32_t>(p[0xb]);
            F(0x3f8, static_cast<std::int32_t>(fn));
        }
        const std::int32_t kc = static_cast<std::int32_t>(p[0xc]);
        if (kc == 0) {
            I(0x474) = 0;
            D(0x478) = p[0xd];
        } else if (kc == 1) {
            I(0x474) = 1;
            I(0x480) = 1;
            F(0x470, static_cast<std::int32_t>(p[0xd]));
        } else if (kc == 2) {
            I(0x474) = 1;
            I(0x480) = 3;
            F(0x470, static_cast<std::int32_t>(p[0xd]));
        }
        if (w->u174 - 1 == 2) {
            // window type 3
            const std::int32_t k10 = static_cast<std::int32_t>(p[0x10]);
            if (k10 == 0) {
                I(0x4ec) = 0;
                D(0x4f0) = p[0x11];
            } else if (k10 == 1) {
                I(0x4ec) = 1;
                I(0x4f8) = 1;
                F(0x4e8, static_cast<std::int32_t>(p[0x11]));
            }
        } else {
            // window type 5
            const std::int32_t ke = static_cast<std::int32_t>(p[0xe]);
            if (ke == 0) {
                I(0x4ec) = 0;
                D(0x4f0) = p[0xf];
            } else if (ke == 1) {
                I(0x4ec) = 1;
                I(0x4f8) = 1;
                F(0x4e8, static_cast<std::int32_t>(p[0xf]));
            }
            if (static_cast<std::int32_t>(p[0x10]) < 1) {
                B(0x560) = 0;
            } else {
                B(0x560) = 1;
            }
            const std::int32_t k10 = static_cast<std::int32_t>(p[0x10]);
            if (k10 == 1) {
                I(0x56c) = 0;
                D(0x570) = p[0x11];
            } else if (k10 == 2) {
                I(0x56c) = 1;
                I(0x578) = 1;
                F(0x568, static_cast<std::int32_t>(p[0x11]));
            } else if (k10 == 3) {
                I(0x56c) = 1;
                I(0x578) = 4;
                F(0x568, static_cast<std::int32_t>(p[0x11]));
            }
            I(0x838) = static_cast<std::int32_t>(p[1]);
        }
        break;
    }
    case 3:
    case 5: {
        const std::int32_t k6 = static_cast<std::int32_t>(p[6]);
        if (k6 == 0) {
            I(0x5e4) = 0;
            D(0x5e8) = p[7];
        } else if (k6 == 1) {
            I(0x5e4) = 1;
            I(0x5f0) = 1;
            F(0x5e0, static_cast<std::int32_t>(p[7]));
        } else if (k6 == 2) {
            I(0x5e4) = 1;
            I(0x5f0) = 3;
            F(0x5e0, static_cast<std::int32_t>(p[7]));
        }
        const std::int32_t k8 = static_cast<std::int32_t>(p[8]);
        if (k8 == 0) {
            I(0x65c) = 0;
            D(0x660) = p[9];
        } else if (k8 == 1) {
            I(0x65c) = 1;
            I(0x668) = 1;
            F(0x658, static_cast<std::int32_t>(p[9]));
        } else if (k8 == 2) {
            I(0x65c) = 1;
            I(0x668) = 0xc;
            F(0x658, static_cast<std::int32_t>(p[9]));
        } else if (k8 == 3) {
            I(0x65c) = 2;
            const double fn = p[9];
            I(0x668) = 1;
            I(0x66c) = 0xc;
            I(0x674) = static_cast<std::int32_t>(p[10]);
            I(0x678) = static_cast<std::int32_t>(p[0xb]);
            F(0x658, static_cast<std::int32_t>(fn));
        }
        if (w->u174 - 1 == 5) {
            const std::int32_t kc = static_cast<std::int32_t>(p[0xc]);
            if (kc == 0) {
                I(0x6d4) = 0;
                D(0x6d8) = p[0xd];
            } else if (kc == 1) {
                I(0x6d4) = 1;
                I(0x6e0) = 1;
                F(0x6d0, static_cast<std::int32_t>(p[0xd]));
            }
            I(0x838) = static_cast<std::int32_t>(p[1]);
        }
        break;
    }
    case 6: {
        const std::int32_t k6 = static_cast<std::int32_t>(p[6]);
        if (k6 == 0) {
            I(0x74c) = 0;
            D(0x750) = p[7];
        } else if (k6 == 1) {
            I(0x74c) = 1;
            I(0x758) = 1;
            F(0x748, static_cast<std::int32_t>(p[7]));
        }
        const std::int32_t kd = static_cast<std::int32_t>(p[0xd]);
        if (kd == 1) {
            I(8) = 1;
        } else if (kd == 2) {
            I(8) = 2;
        } else if (kd == 3) {
            I(8) = 3;
        } else {
            I(8) = 0;
        }
        I(0x18) = static_cast<std::int32_t>(p[8]);
        if (0 <= I(0x18) && I(0x18) <= 5) {
            friction_side(w, I(0x18), 0x20, p[9], &p[10], 0x110, 0x120, 0x130);
            if (I(0x18) != 0) {
                F(0x20, static_cast<std::int32_t>(p[9]));
            }
        }
        I(0x838) = static_cast<std::int32_t>(p[1]);
        break;
    }
    case 7:
        if (static_cast<std::int32_t>(p[6]) == 0) {
            I(0x74c) = 0;
            D(0x750) = p[7];
        } else {
            I(0x74c) = 1;
            I(0x758) = 1;
            F(0x748, static_cast<std::int32_t>(p[7]));
        }
        if (static_cast<std::int32_t>(p[8]) == 0) {
            I(0x7c4) = 0;
            D(0x7c8) = p[9];
        } else {
            I(0x7c4) = 1;
            I(0x7d0) = 1;
            F(0x7c0, static_cast<std::int32_t>(p[9]));
        }
        break;
    case 9:
        break;
    default:
        rt::crt::printf("ERROR in LLWINDEF_DATA::ReadFromDBArrays(): not implemented windef type\n");
        break;
    }
}

} // namespace def::dbio::dbread
