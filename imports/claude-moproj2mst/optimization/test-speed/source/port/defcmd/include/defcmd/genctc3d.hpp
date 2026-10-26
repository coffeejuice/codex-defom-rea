// defcmd/genctc3d.hpp: the GENCTC 3D contact engine (packet p67-7): everything below the C++ 3D
// driver FUN_1401a6c80 (owned by p67-6) and the shared engine the OBJPOS drop solver (p67-3)
// calls. Specs: docs/re/commands/GENCTC_3d_search.md, GENCTC_3d_decide.md, GENCTC.md sec. 5.
//
// API: every routine is def::cmd::ctc3d::fun_<address> with the binary's argument list in order;
// Fortran routines take every argument by pointer (INTEGER = std::int32_t, REAL*8 = double),
// arrays column-major, 1-based in the comments. Static storage (COMMON, SAVE) is the image of
// defcmd/ctc3d/common.hpp. The groups below are the porting units of the packet; each header lists
// its routines with their @src address.
//
// Entry points other packets call:
//   cube table           fun_140225640 (create), fun_140225820 (free), fun_1401a7710 (all objects)
//   master surface       fun_14004f56b (build one cube), fun_1400407e7 (normals and areas)
//   partner list         fun_1400511e4, fun_140082ed6
//   pipeline             fun_1400512aa (GENCTC), fun_14005150d (drop: + p67-3's fun_140049361)
//   per-node decision    fun_14005190e (and fun_14004946f)
//   search kernel        fun_14006798b (MDSPS2_REC)
//   cube getters         fun_140228830 NI, fun_140228b10 NR, fun_140228260 EI, fun_140228540 ER,
//                        fun_140227e00 polygon nodes, fun_140227fc0 point, fun_14004f363
//   ray kernels          fun_140064f58 (ray-quad), fun_14007ff21, fun_1400653e5;
//                        fun_1400647a0 (ray-triangle) is p67-3's, declared here with its signature
#pragma once

#include <defcmd/ctc3d/common.hpp>

#include <defcmd/ctc3d/cube.hpp>     // C cube / locator library, C++ bucket build
#include <defcmd/ctc3d/surface.hpp>  // master surface build
#include <defcmd/ctc3d/pipeline.hpp> // FUN_1400512aa / FUN_14005150d and stages 1, 2, 4, 6, 7
#include <defcmd/ctc3d/srcnt.hpp>    // SRCNT stage (stage 5)
#include <defcmd/ctc3d/search.hpp>   // MDSPS2_REC and kernels
#include <defcmd/ctc3d/decide.hpp>   // per-node decision and write-back
