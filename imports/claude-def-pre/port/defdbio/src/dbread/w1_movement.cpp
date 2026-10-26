// dbread/w1_movement.cpp: OMV_DATA::ReadFromDB (FUN_14018d3c0), the movement block of an object
// (Cls_14018b220 at LLOBJ_DATA+0x2160) from the object's first LLBND record (packet p-dbread, W1).
// Offsets are the binary's (the class is an opaque slot in llobj_data.hpp); the GENDB mirror is
// gendb3d/movement.cpp (FUN_1401912a0). Byte-wise int stores of the decompile are int stores here.
#include <defdbio/dbread/w1.hpp>

#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/w1_access.hpp"

namespace def::dbio::dbread {

using w1::at;
using w1::copy8;
using w1::dyn;

// @src 14018d3c0
void read_movement(void* mv, double* rec, std::int32_t* iifnc, double* llfnc, std::int32_t platform)
{
    auto I = [mv](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(mv, off); };
    auto R = [rec](std::ptrdiff_t off) -> double { return at<double>(rec, off); };
    auto RI = [rec](std::ptrdiff_t off) -> std::int32_t { return static_cast<std::int32_t>(at<double>(rec, off)); };
    auto Q = [mv, rec](std::ptrdiff_t dst, std::ptrdiff_t src) { copy8(mv, dst, rec, src); };
    auto F = [mv, iifnc, llfnc](std::ptrdiff_t off, std::int32_t fn) {
        char with_shape = 0;
        read_func_from_db_array(dyn(mv, off), iifnc, llfnc, fn, &with_shape);
    };
    auto* owner = at<model::DB_STEP_DATA*>(mv, 0xa80);
    char* name = static_cast<char*>(mv);
    char* text = reinterpret_cast<char*>(rec);

    const std::int32_t version = owner->step_version; // 1401cc180
    if (!rt::dimension_is_2(owner)) {
        if (rt::dimension_is_3(owner)) {
            std::strncpy(name, text + 0x290, 0x50);
            text_byteswap(name, platform, 10, 0); // 140324020
            rt::trim_fortran(name, 0x50);          // 1403239f0
            Q(0x50, 0x60);
            Q(0x58, 0x68);
            Q(0x60, 0x70);
            switch (RI(0x78)) {
            case 0:
                I(0x68) = 0;
                if (RI(0x80) == 0) {
                    I(0x74) = 0;
                    Q(0x78, 0x88);
                } else if (0 < RI(0x80)) {
                    const std::int32_t k = RI(0x88);
                    if (k == 1) {
                        I(0x74) = 1;
                        I(0x80) = 2;
                        F(0x70, RI(0x80));
                    } else if (k == 2) {
                        I(0x74) = 1;
                        I(0x80) = 1;
                        F(0x70, RI(0x80));
                    }
                }
                break;
            case 1:
                I(0x68) = 1;
                if (RI(0x80) == 0) {
                    I(0xec) = 0;
                    Q(0xf0, 0x88);
                } else if (RI(0x80) < 1) {
                    if (RI(0x80) < -1000 && -2000 < RI(0x80)) {
                        I(0x160) = -1000 - RI(0x80);
                        Q(0x168, 0x88);
                    }
                } else {
                    const std::int32_t k = RI(0x88);
                    if (k == 1) {
                        I(0xec) = 1;
                        I(0xf8) = 2;
                        F(0xe8, RI(0x80));
                    } else if (k == 2) {
                        I(0xec) = 1;
                        I(0xf8) = 1;
                        F(0xe8, RI(0x80));
                    } else if (k == 4) {
                        I(0xec) = 1;
                        I(0xf8) = 5;
                        F(0xe8, RI(0x80));
                    }
                }
                break;
            case 2:
                I(0x68) = 2;
                Q(0x1f8, 0x148);
                Q(0x1e8, 0x150);
                Q(0x1f0, 0x50);
                I(0x200) = RI(0x158);
                if (version < 11000) {
                    if (RI(0x80) == 0) {
                        I(0x20c) = 0;
                    } else if (1 < RI(0x80)) {
                        if (RI(0x88) == 4) {
                            I(0x20c) = 1;
                            I(0x218) = 5;
                            F(0x208, RI(0x80));
                        } else {
                            rt::crt::fprintf(w1::out_stream(), "Need to check OMV_DATA::ReadFromDB()\n");
                        }
                    }
                } else {
                    if (RI(0x338) == 0) {
                        I(0x20c) = 0;
                    } else if (0 < RI(0x338)) {
                        if (RI(0x340) == 4) {
                            I(0x20c) = 1;
                            I(0x218) = 5;
                            F(0x208, RI(0x338));
                        } else {
                            rt::crt::fprintf(w1::out_stream(), "Need to check OMV_DATA::ReadFromDB()\n");
                        }
                    }
                }
                break;
            case 3:
                I(0x68) = 3;
                Q(0x280, 0x50);
                Q(0x288, 0x148);
                Q(0x290, 0x150);
                Q(0x298, 0x158);
                if (RI(0x338) == 0) {
                    I(0x2a4) = 0;
                } else if (0 < RI(0x338)) {
                    if (RI(0x340) == 4) {
                        I(0x2a4) = 1;
                        I(0x2b0) = 5;
                        F(0x2a0, RI(0x338));
                    } else {
                        rt::crt::fprintf(w1::out_stream(), "Need to check OMV_DATA::ReadFromDB()\n");
                    }
                }
                break;
            case 4:
                I(0x68) = 4;
                Q(0x318, 0x150);
                Q(0x320, 0x158);
                Q(0x328, 0x148);
                if (RI(0x80) < 1) {
                    at<std::uint8_t>(mv, 0x330) = 0;
                    I(0x33c) = 0;
                    I(0x340) = 0;
                    I(0x344) = 0;
                } else {
                    at<std::uint8_t>(mv, 0x330) = 1;
                    I(0x33c) = 1;
                    I(0x348) = 10;
                    F(0x338, RI(0x80));
                }
                break;
            case 5:
                I(0x68) = 5;
                if (RI(0x80) == 0) {
                    I(0x3b4) = 0;
                    Q(0x3b8, 0x158);
                } else if (RI(0x88) == 1) {
                    I(0x3b4) = 1;
                    I(0x3c0) = 0x13;
                    F(0x3b0, RI(0x80));
                }
                Q(0x428, 0x260);
                Q(0x430, 0x150);
                Q(0x438, 0x1f8);
                I(0x440) = RI(0x1f0);
                I(0x444) = RI(0x200);
                break;
            case 6:
                I(0x68) = 6;
                model::dynary_init(dyn(mv, 0x448), 0x7a); // 14012d2a0
                I(0x510) = 0;
                if (RI(0x80) < 0) {
                    I(0x510) = 1;
                    I(0x514) = w1::crt_abs(RI(0x80)); // 140484ab8
                } else if (0 < RI(0x80)) {
                    I(0x44c) = 1;
                    I(0x458) = 1;
                    const double fnd = R(0x80);
                    I(0x464) = RI(0x148);
                    I(0x4c0) = RI(0x150);
                    I(0x4c4) = RI(0x158);
                    double* p = func_record(iifnc, llfnc, static_cast<std::int32_t>(fnd)); // 140130920
                    if (p != nullptr) {
                        const double hdr = p[0];
                        std::memcpy(&at<double>(mv, 0x4c8), &p[1], 9 * 8); // 0x4c8..0x508 = p[1..9]
                        read_table_rows_1d(dyn(mv, 0x448), p + static_cast<std::int32_t>(hdr)); // 140131ea0
                        if (I(0x4c0) == 2) {
                            double inv[10];
                            const std::int32_t r = movement_frame_inverse(mv, inv); // 140194e30
                            if (r == 1) {
                                dynary_rotate_7a(dyn(mv, 0x448), inv); // 140135dd0
                            } else if (r == 2) {
                                dynary_rotate_7a_about(dyn(mv, 0x448), inv, at<double>(mv, 0x4c8),
                                                       at<double>(mv, 0x4d0), at<double>(mv, 0x4d8)); // 140135fd0
                            } else {
                                I(0x4c0) = 1;
                            }
                        }
                        if (RI(0x88) == 5) {
                            dynary_time_axis_to_speed(dyn(mv, 0x448)); // 140134b00
                        }
                    }
                }
                break;
            case 7:
                I(0x68) = 7;
                if (RI(0x80) == 0) {
                    I(0x51c) = 0;
                    Q(0x520, 0x88);
                } else if (0 < RI(0x80)) {
                    const std::int32_t k = RI(0x88);
                    if (k == 1) {
                        I(0x51c) = 1;
                        I(0x528) = 2;
                        F(0x518, RI(0x80));
                    } else if (k == 2) {
                        I(0x51c) = 1;
                        I(0x528) = 1;
                        F(0x518, RI(0x80));
                    }
                }
                Q(0x608, 0x150);
                Q(0x610, 0x158);
                break;
            case 8:
                I(0x68) = 8;
                if (RI(0x80) == 0) {
                    I(0x594) = 0;
                    Q(0x598, 0x88);
                } else if (0 < RI(0x80)) {
                    const std::int32_t k = RI(0x88);
                    if (k == 1) {
                        I(0x594) = 1;
                        I(0x5a0) = 2;
                        F(0x590, RI(0x80));
                    } else if (k == 2) {
                        I(0x594) = 1;
                        I(0x5a0) = 1;
                        F(0x590, RI(0x80));
                    }
                }
                Q(0x608, 0x150);
                break;
            case 9:
                I(0x68) = 9;
                Q(0x608, 0x150);
                break;
            case 10:
                I(0x68) = 10;
                Q(0x608, 0x150);
                Q(0x618, 0x158);
                break;
            case 11:
                I(0x68) = 11;
                break;
            default:
                break;
            }
            if (RI(0x80) < 0 && -1000 < RI(0x80)) {
                I(0x620) = 1;
                I(0x624) = w1::crt_abs(RI(0x80));
            }
            Q(0x170, 0x90);
            Q(0x178, 0x98);
            Q(0x180, 0xa0);
            Q(0x188, 0xa8);
            Q(0x190, 0xb0);
            Q(0x198, 0xb8);
            Q(0x1a0, 0xc0);
            Q(0x1a8, 0xc8);
            Q(0x1b0, 0xd0);
            const std::int32_t sel = RI(0xd8);
            if (sel == 0) {
                I(0x628) = 0;
                Q(0x668, 0x130);
                const std::int32_t k = RI(0xe0);
                if (k == 0) {
                    I(0x664) = 0;
                } else if (k == 1) {
                    I(0x664) = 1;
                    I(0x670) = 1;
                    F(0x660, RI(0xe8));
                } else if (k == 2) {
                    I(0x664) = 1;
                    I(0x670) = 2;
                    F(0x660, RI(0xe8));
                } else if (k == 3) {
                    I(0x664) = 1;
                    I(0x670) = 10;
                    F(0x660, RI(0xe8));
                }
            } else if (sel == 1) {
                I(0x628) = 1;
                Q(0x6e0, 0x128);
                const std::int32_t k = RI(0xe0);
                if (k == 0) {
                    I(0x6dc) = 0;
                } else if (k == 1) {
                    I(0x6dc) = 1;
                    I(0x6e8) = 1;
                    F(0x6d8, RI(0xe8));
                } else if (k == 2) {
                    I(0x6dc) = 1;
                    I(0x6e8) = 2;
                    F(0x6d8, RI(0xe8));
                } else if (k == 3) {
                    I(0x6dc) = 1;
                    I(0x6e8) = 10;
                    F(0x6d8, RI(0xe8));
                }
            }
            Q(0x630, 0x108);
            Q(0x638, 0x110);
            Q(0x640, 0x118);
            Q(0x648, 0xf0);
            Q(0x650, 0xf8);
            Q(0x658, 0x100);
            Q(0x750, 0x120);
            const std::int32_t sel2 = RI(0x190);
            if (sel2 == 0) {
                I(0x758) = 0;
                Q(0x798, 0x1e8);
                const std::int32_t k = RI(0x198);
                if (k == 0) {
                    I(0x794) = 0;
                } else if (k == 1) {
                    I(0x794) = 1;
                    I(0x7a0) = 1;
                    F(0x790, RI(0x1a0));
                } else if (k == 2) {
                    I(0x794) = 1;
                    I(0x7a0) = 2;
                    F(0x790, RI(0x1a0));
                } else if (k == 3) {
                    I(0x794) = 1;
                    I(0x7a0) = 10;
                    F(0x790, RI(0x1a0));
                }
            } else if (sel2 == 1) {
                I(0x758) = 1;
                Q(0x810, 0x1e0);
                const std::int32_t k = RI(0x198);
                if (k == 0) {
                    I(0x80c) = 0;
                } else if (k == 1) {
                    I(0x80c) = 1;
                    I(0x818) = 1;
                    F(0x808, RI(0x1a0));
                } else if (k == 2) {
                    I(0x80c) = 1;
                    I(0x818) = 2;
                    F(0x808, RI(0x1a0));
                } else if (k == 3) {
                    I(0x80c) = 1;
                    I(0x818) = 10;
                    F(0x808, RI(0x1a0));
                }
            }
            Q(0x760, 0x1c0);
            Q(0x768, 0x1c8);
            Q(0x770, 0x1d0);
            Q(0x778, 0x1a8);
            Q(0x780, 0x1b0);
            Q(0x788, 0x1b8);
            Q(0x880, 0x1d8);
            if (RI(0x258) != 0) {
                at<std::uint8_t>(mv, 0x8b8) = 1;
                F(0x8c0, RI(0x258));
            }
            Q(0x938, 0x280);
            Q(0x940, 0x288);
            Q(0x948, 0x268);
            Q(0x950, 0x270);
            Q(0x958, 0x278);
            Q(0x888, 0x2d8);
            Q(0x890, 0x2e0);
            Q(0x898, 0x2e8);
            Q(0x8a0, 0x2f0);
            Q(0x8a8, 0x2f8);
            Q(0x8b0, 0x300);
        }
    } else {
        std::strncpy(name, text + 0x1c0, 0x50);
        text_byteswap(name, platform, 10, 0);
        rt::trim_fortran(name, 0x50);
        Q(0x50, 0x50);
        Q(0x58, 0x58);
        at<std::uint64_t>(mv, 0x60) = 0;
        switch (RI(0x38)) {
        case 0:
            I(0x68) = 0;
            if (RI(0x60) == 0) {
                I(0x74) = 0;
                Q(0x78, 0x48);
            } else if (0 < RI(0x60)) {
                const std::int32_t k = RI(0x68);
                if (k == 1) {
                    I(0x74) = 1;
                    I(0x80) = 2;
                    F(0x70, RI(0x70));
                } else if (k == 2) {
                    I(0x74) = 1;
                    I(0x80) = 1;
                    F(0x70, RI(0x70));
                }
            }
            break;
        case 1:
            I(0x68) = 1;
            if (RI(0x60) == 0) {
                I(0xec) = 0;
                Q(0xf0, 0x48);
            } else if (RI(0x60) == 1) {
                const std::int32_t k = RI(0x68);
                if (k == 1) {
                    I(0xec) = 1;
                    I(0xf8) = 2;
                    F(0xe8, RI(0x70));
                } else if (k == 2) {
                    I(0xec) = 1;
                    I(0xf8) = 1;
                    F(0xe8, RI(0x70));
                } else if (k == 4) {
                    I(0xec) = 1;
                    I(0xf8) = 5;
                    F(0xe8, RI(0x70));
                }
            } else if (RI(0x60) == 2) {
                I(0x160) = RI(0x70);
                Q(0x168, 0x48);
            }
            break;
        case 2:
            I(0x68) = 2;
            Q(0x1e8, 0x78);
            Q(0x1f8, 0x80);
            Q(0x1f0, 0x48);
            I(0x200) = RI(0x88);
            if (version < 11000) {
                if (RI(0x60) == 0) {
                    I(0x20c) = 0;
                } else if (RI(0x60) == 1) {
                    if (RI(0x68) == 4) {
                        I(0x20c) = 1;
                        I(0x218) = 5;
                        F(0x208, RI(0x70));
                    } else {
                        rt::crt::fprintf(w1::out_stream(), "Need to check OMV_DATA::ReadFromDB()\n");
                    }
                }
            } else {
                if (RI(0x228) == 0) {
                    I(0x20c) = 0;
                } else if (0 < RI(0x228)) {
                    if (RI(0x230) == 4) {
                        I(0x20c) = 1;
                        I(0x218) = 5;
                        F(0x208, RI(0x228));
                    } else {
                        rt::crt::fprintf(w1::out_stream(), "Need to check OMV_DATA::ReadFromDB()\n");
                    }
                }
            }
            break;
        case 3:
            I(0x68) = 3;
            Q(0x280, 0x48);
            Q(0x290, 0x78);
            Q(0x288, 0x80);
            Q(0x298, 0x88);
            if (RI(0x228) == 0) {
                I(0x2a4) = 0;
            } else if (0 < RI(0x228)) {
                if (RI(0x230) == 4) {
                    I(0x2a4) = 1;
                    I(0x2b0) = 5;
                    F(0x2a0, RI(0x228));
                } else {
                    rt::crt::fprintf(w1::out_stream(), "Need to check OMV_DATA::ReadFromDB()\n");
                }
            }
            break;
        case 4:
            I(0x68) = 4;
            Q(0x318, 0x78);
            Q(0x320, 0x88);
            Q(0x328, 0x80);
            if (RI(0x60) < 1) {
                at<std::uint8_t>(mv, 0x330) = 0;
                I(0x33c) = 0;
                I(0x340) = 0;
                I(0x344) = 0;
            } else {
                at<std::uint8_t>(mv, 0x330) = 1;
                I(0x33c) = 1;
                I(0x348) = 10;
                F(0x338, RI(0x70));
            }
            break;
        case 5:
            I(0x68) = 5;
            if (RI(0x60) == 0) {
                I(0x3b4) = 0;
                Q(0x3b8, 0x88);
            } else if (RI(0x68) == 1) {
                I(0x3b4) = 1;
                I(0x3c0) = 0x13;
                F(0x3b0, RI(0x70));
            }
            Q(0x428, 0x190);
            Q(0x430, 0x78);
            Q(0x438, 0x180);
            I(0x440) = RI(0x178);
            I(0x444) = RI(0x188);
            break;
        case 6:
            I(0x68) = 6;
            model::dynary_init(dyn(mv, 0x448), 0x79);
            I(0x510) = 0;
            if (RI(0x60) == -1) {
                I(0x510) = 1;
                I(0x514) = w1::crt_abs(RI(0x70));
            } else if (RI(0x60) == 1) {
                I(0x44c) = 1;
                I(0x458) = 1;
                const double fnd = R(0x70);
                I(0x464) = RI(0x78);
                F(0x448, static_cast<std::int32_t>(fnd));
                if (RI(0x68) == 5) {
                    dynary_time_axis_to_speed(dyn(mv, 0x448));
                }
            }
            I(0x4c4) = RI(0x80);
            break;
        case 7:
            I(0x68) = 7;
            if (RI(0x60) == 0) {
                I(0x51c) = 0;
                Q(0x520, 0x48);
            } else if (0 < RI(0x60)) {
                const std::int32_t k = RI(0x68);
                if (k == 1) {
                    I(0x51c) = 1;
                    I(0x528) = 2;
                    F(0x518, RI(0x70));
                } else if (k == 2) {
                    I(0x51c) = 1;
                    I(0x528) = 1;
                    F(0x518, RI(0x70));
                }
            }
            Q(0x608, 0x78);
            Q(0x610, 0x80);
            break;
        case 8:
            I(0x68) = 8;
            if (RI(0x60) == 0) {
                I(0x594) = 0;
                Q(0x598, 0x48);
            } else if (0 < RI(0x60)) {
                const std::int32_t k = RI(0x68);
                if (k == 1) {
                    I(0x594) = 1;
                    I(0x5a0) = 2;
                    F(0x590, RI(0x70));
                } else if (k == 2) {
                    I(0x594) = 1;
                    I(0x5a0) = 1;
                    F(0x590, RI(0x70));
                }
            }
            Q(0x608, 0x78);
            break;
        case 9:
            I(0x68) = 9;
            Q(0x608, 0x78);
            break;
        case 10:
            I(0x68) = 10;
            Q(0x608, 0x78);
            Q(0x618, 0x80);
            break;
        case 11:
            I(0x68) = 11;
            break;
        default:
            break;
        }
        if (RI(0x60) < 0) {
            I(0x620) = 1;
            I(0x624) = w1::crt_abs(RI(0x70));
        }
        Q(0x170, 0x90);
        Q(0x178, 0x98);
        at<std::uint64_t>(mv, 0x180) = 0;
        Q(0x188, 0xa0);
        Q(0x190, 0xa8);
        at<std::uint64_t>(mv, 0x198) = 0;
        Q(0x1a0, 0xb0);
        Q(0x1a8, 0xb8);
        at<std::uint64_t>(mv, 0x1b0) = 0;
        Q(0x1b8, 0xc0);
        Q(0x1c0, 0xc8);
        Q(0x1c8, 0xd0);
        Q(0x1d0, 0xd8);
        Q(0x1d8, 0xe0);
        Q(0x1e0, 0xe8);
        const std::int32_t sel = RI(0xf0);
        if (sel == 0) {
            I(0x628) = 0;
            const std::int32_t k = RI(0x118);
            if (k == 0) {
                I(0x664) = 0;
                Q(0x668, 0x100);
            } else if (k == 1) {
                I(0x664) = 1;
                I(0x670) = 1;
                F(0x660, RI(0x120));
            } else if (k == 2) {
                I(0x664) = 1;
                I(0x670) = 10;
                F(0x660, RI(0x120));
            }
        } else if (sel == 1) {
            I(0x628) = 1;
            const std::int32_t k = RI(0x118);
            if (k == 0) {
                I(0x6dc) = 0;
                Q(0x6e0, 0xf8);
            } else if (k == 1) {
                I(0x6dc) = 1;
                I(0x6e8) = 1;
                F(0x6d8, RI(0x120));
            } else if (k == 2) {
                I(0x6dc) = 1;
                I(0x6e8) = 10;
                F(0x6d8, RI(0x120));
            }
        }
        Q(0x630, 0x108);
        Q(0x638, 0x110);
        Q(0x750, 0x128);
        if (RI(0x40) != 0) {
            at<std::uint8_t>(mv, 0x8b8) = 1;
            F(0x8c0, RI(0x40));
        }
        Q(0x938, 0x1b0);
        Q(0x940, 0x1b8);
        Q(0x948, 0x198);
        Q(0x950, 0x1a0);
        Q(0x958, 0x1a8);
        if (RI(0x130) == 0) {
            I(0x960) = -1;
        } else {
            const std::int32_t k = RI(0x138);
            if (k == 0) {
                I(0x960) = 0;
                const std::int32_t t = RI(0x140);
                if (t == 0) {
                    I(0x96c) = 0;
                    Q(0x970, 0x148);
                } else if (t == 1) {
                    I(0x96c) = 1;
                    I(0x978) = 1;
                    F(0x968, RI(0x148));
                } else if (t == 2) {
                    I(0x96c) = 1;
                    I(0x978) = 10;
                    F(0x968, RI(0x148));
                }
            } else if (k == 1) {
                I(0x960) = 1;
                const std::int32_t t = RI(0x140);
                if (t == 0) {
                    I(0x9e4) = 0;
                    Q(0x9e8, 0x148);
                } else if (t == 1) {
                    I(0x9e4) = 1;
                    I(0x9f0) = 1;
                    F(0x9e0, RI(0x148));
                } else if (t == 2) {
                    I(0x9e4) = 1;
                    I(0x9f0) = 10;
                    F(0x9e0, RI(0x148));
                }
            } else if (k == 2) {
                I(0x960) = 2;
                Q(0xa58, 0x148);
                Q(0xa60, 0x150);
                Q(0xa68, 0x158);
            }
            Q(0xa70, 0x170);
        }
        Q(0x888, 0x208);
        Q(0x890, 0x210);
        Q(0x8a0, 0x218);
        Q(0x8a8, 0x220);
    }
    if (at<std::uint8_t>(mv, 0x8b8) != 0 && I(0x68) == 1) {
        I(0x68) = 8;
    }
}

} // namespace def::dbio::dbread
