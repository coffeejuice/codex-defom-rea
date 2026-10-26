# Plan: faithful C++ reconstruction of the solver harness (3D 64-bit and 2D)

Status: draft for the user's revision (2026-10-22). Plan only; no porting has started.

Goal: modern C++ programs that replace, one for one, every program DEFORM v11.0.2 runs when the
user clicks **Run** in DEF_GUI23 on a 3D problem on the 64-bit path, and on a 2D problem (2D has
only a 32-bit solver; added at the user's request, section 1.4). Each program keeps its
original name, command line, environment variables, input and output files, exit codes and
process structure, so DEF_GUI23 (the "mother app") and any mix of original and ported programs
work together. Inside each program every function is recreated as it is in the binary, as in the
DEF_PRE_64 port (`docs/port-plan-def-pre-batch.md`, `docs/port-conventions.md`).

Evidence for this plan: `docs/re/gui-run-chain.md` (strings and leftover run files), a PE census
of the install (bitness, size, `.pdata` function entries, imports), and the member list of the
vendor's user-routine link library `3D\UserRoutine\DEF_SIM\DEF_SIM_USR64_LIB.lib`. Nothing was
decompiled for this plan; module roles marked "(guess)" come from file names only.

## 1. What the harness is

### 1.1 The run chain

```
DEF_GUI23.exe                                  (mother app, not ported)
 └─ DEF_SIMULATION.exe                         job wrapper, env-var job description
     └─ 3D\DEF_ARM_CTL3.COM <prob> <c>          batch controller, MST multi-run loop
         ├─ [MST] DEF_RDMS3.EXE "<prob>" "<n>"  unpack run n of the master file
         ├─ [MST] DEF_PRE_64.EXE < DEF_PRE3.INP  rebuild DEF_PRE.DB  (port in progress elsewhere)
         ├─ 3D\DEF_ARM.COM "<prob>" B >> <prob>.LOG     automatic remeshing driver
         │    ├─ DEF_MPI_CTL_64.COM DEF_MPIenv.DAT DEF_SIM > <msg>
         │    │    └─ mpiexec.exe [-localonly] -channel NEMESIS -np N ... DEF_SIM_64.EXE
         │    ├─ or serial DEF_SIM_64.EXE > <msg>
         │    ├─ remesh: DEF_REM_64.EXE < DEF_INT3.INP, or DEF_MPI_CTL_64.COM ... DEF_REM
         │    ├─ mesh:   DEF_AMG_64.EXE < DEF_AMG3.INP,  AMG_DXT_64.EXE > DEF_BRD3.OUT
         │    ├─ local remesh: DEF_LOCREM3_64.EXE, or DEF_MPI_CTL_64.COM ... DEF_LOC_REM
         │    ├─ volume check: CHK_VOL_64.EXE (also through DEF_MPI_CTL_64 ... CHK_VOL)
         │    ├─ DB ops: DEF_PRE_64.EXE < DEF_PRE3.INP > DEF_PRE3.OUT
         │    └─ helpers: DEF_BRK2.EXE, DEF_SPLDB.exe, DEF_CMESH.exe
         ├─ DEF_TEXT_PST.EXE                     batch report
         └─ FILE_REMOVE.EXE                      clean-up
```

The 64-bit choice is made inside DEF_ARM.COM, by whether `64bit.DAT` exists in the problem
folder (section 1.1a); it then uses the 64-bit command formats and logs "Running 64bit
simulation". The user's experience is that 64-bit runs work when `DEF_MPIp4penv.DAT` is present;
that file holds the MPI settings the 64-bit MPI path reads. DEF_MPI_CTL_64.COM also reads
`DEF_MPIp4penv.DAT`, `DEF_SIM_DIR.DAT`, `DEF_GPU.DAT` and `DEFORM.MAPDRIVE`, and passes
`-envlist DEF_ARM_SESSION,DEF_CFG_DIR,SIM_FEM_BITS_MODE,deform_server_session_id,deform_job_id,
deform_in_defarm_ctl,TEMP,TMP` to MPICH2's `mpiexec.exe` (shipped in `3D\`).

Out of scope for 3D: the 3D 32-bit solver `3D\DEF_SIM.exe`, the 3D 32-bit MPI workers
(DEF_REM.EXE, DEF_AMG.EXE, AMG_DXT.EXE, CHK_VOL.EXE in `3D\`) and DEF_MPI_CTL.COM. Code paths
inside ported programs that select them are kept as they are (same strings, same branches),
because keeping every function as is includes those branches. The serial `3D\DEF_LOCREM3.EXE` is
in scope (1.1a). 2D is in scope with its own chain (1.4).

### 1.1a What can run on the test machine (Sim2)

Only the 64-bit MPICH2 is installed (`C:\Program Files\MPICH2`; `mpiexec.exe` and `smpd.exe` are
64-bit 2009 builds, identical in size to `3D\mpiexec.exe`). The user reports that the 32-bit MPI
does not work on current Windows 11 builds, so it is not installed. Consequences:

* Any program that needs 32-bit MPI cannot be run as an oracle here: `DEF_MPI_CTL.COM`,
  `KillMPI.exe` (imports the 32-bit `mpich.dll`), and the 32-bit MPI workers. Their behaviour is
  taken from static analysis only, and tests check the command lines the ported callers build
  instead of executing them.
* The 32-bit driver apps (DEF_ARM, ARM_CTL3, DEF_SIMULATION, DEF_RDMS3, CLEANMPIJOB, DEF_BRK2,
  DEF_SPLDB, DEF_CMESH) import no MPI library and run here as oracles.
* The user reports that the solver runs fine with 64-bit MPI when `DEF_MPIp4penv.DAT` is in the
  problem folder, and that DEF_PRE shows no problem without the 32-bit MPI.
* The user tested DEF_GUI23's Run options dialog (2026-10-22): choosing **64 bit** writes
  `64bit.dat` into the problem folder; checking **Multiple processor** writes
  `DEF_MPIp4penv.DAT`, `DEF_MPIenv.DAT`, `DEF_MAIL.INI` and `DEFORM3D.PROB`. So the two
  switches are independent: `64bit.dat` picks the 64-bit programs, and the MPI files pick the
  parallel path and its settings. A 64-bit MPI run needs both. These five files are the
  interface between the mother app and the harness, and H0-07 documents their formats; the
  corpus covers all four combinations (32/64 bit × serial/parallel), with 32-bit runs only
  as far as Sim2 can run them.
* First static look at DEF_ARM.COM (dumpbin from the Absoft install, no Ghidra yet):
  * The 64-bit switch is the file **`64bit.DAT`** in the problem folder, not
    `DEF_MPIp4penv.DAT` (corrected after the remesh thread found the same test in
    DEF_REM3_GUI.com). At `0x404409` DEF_ARM tests whether `64bit.DAT` exists and stores the
    result in the global byte `0x541de0`, which selects the `_64` commands and the "Running 64bit
    simulation" log line. DEF_ARM_CTL3.com, DEF_SIM_64.EXE and DEF_GUI23.exe also contain
    `64bit.dat`; the GUI has a `slotCheck64Bit` handler, so the GUI probably writes the file.
  * `DEF_MPIp4penv.DAT` is a separate file. In DEF_ARM it is tested at `0x40303a` (with
    `DEF_MPIenv.DAT` as the fallback) inside what looks like the version-check code, not the
    64-bit switch; DEF_MPI_CTL_64.COM and DEF_SIM_64.EXE also read it, probably as the MPI
    settings for the 64-bit MPICH2 run, which matches the user's test above. What DEF_SIM_64
    itself does with `64bit.dat` and `DEF_MPIp4penv.DAT` is for H0-06 to settle from the code.
  * In 64-bit mode the solver, remesh, mesh and DB steps use the `_64` names
    (`DEF_MPI_CTL_64.COM ... DEF_SIM`, `DEF_REM_64.EXE`, `DEF_AMG_64.EXE`, `DEF_PRE_64.EXE`).
  * Local remeshing does not switch: at `0x405ddf` both branches of the 64-bit test build the same
    commands, `DEF_MPI_CTL.COM" DEF_MPIenv.DAT DEF_LOC_REM` for parallel and `DEF_LOCREM3.EXE"`
    for serial, and the launch helper at `0x401be0` does not rename them.
  * On Sim2 the 32-bit MPICH2 DLLs are absent (only 64-bit ones in System32), so the parallel
    attempt cannot start its 32-bit MPI worker; DEF_ARM then logs "Multi-processor local
    remeshing failed" and runs the serial 32-bit `DEF_LOCREM3.EXE`, which needs no MPI. This fits
    the user's experience that runs complete. H0-06 confirms it with a recorded run, and the
    port reproduces this behaviour exactly, including the fallback.
  * So `DEF_LOCREM3.EXE` (32-bit, serial) is in scope as packet W4-08a, and
    `DEF_LOCREM3_64.EXE` is reached only through DEF_MPI_CTL_64.COM, if at all (H0-06).

### 1.2 Programs in scope

Census of the installed files (`.pdata` counts only functions with unwind data, so the true
count is higher; 32-bit images have no `.pdata`; H0-01 gives exact Ghidra counts).

| Tier | Program | Bits | Size | `.pdata` funcs | Notes |
|---|---|---:|---:|---:|---|
| Control | DEF_SIMULATION.exe | 32 | 0.56 MB | n/a | Qt 4 core, job wrapper; DOE/OPT job types dispatch elsewhere |
| Control | 3D\DEF_ARM_CTL3.COM | 32 | 0.26 MB | n/a | C, controller; "Batch Mode"/"Interactive Mode" banners |
| Control | 3D\DEF_ARM.COM | 32 | 1.32 MB | n/a | C, remeshing loop, 32/64 selection, monitoring thread |
| Control | 3D\DEF_MPI_CTL_64.COM | 64 | 0.32 MB | 1 153 | C++, MPICH2 launcher; links `network_license` objects (skip) |
| Control | 3D\DEF_RDMS3.exe | 32 | 0.77 MB | n/a | Fortran, MST parser |
| Control | 3D\FILE_REMOVE.exe | 32 | 0.06 MB | n/a | clean-up |
| Control | 3D\CLEANMPIJOB.COM, KillMPI.exe | 32 | 0.09 / 0.05 MB | n/a | MPI job clean-up and kill |
| Control | 3D\DEF_TEXT_PST.exe | 32 | 4.0 MB | n/a | Qt 4, batch report (only the path ARM_CTL3 uses is in scope first) |
| Control | 3D\DEF_CTRL.com (+ COG_CTRL3) | 32 | 0.23 MB | n/a | cogging Run path |
| Control | DEFJobGroupSetup.exe, 3D\BATCH_QUEUE.exe | 32 | 1.0 / 0.8 MB | n/a | Run (options), job groups, queue; last tier |
| Solver | 3D\DEF_SIM_64.EXE | 64 | 32.3 MB | 6 619 | Absoft Fortran + C/C++, MPICH2, CUDA 4.2 |
| Worker | 3D\DEF_REM_64.exe | 64 | 2.3 MB | 3 120 | remesh/interpolation, MPICH2 |
| Worker | 3D\DEF_AMG_64.EXE | 64 | 2.6 MB | 3 201 | mesh generation |
| Worker | 3D\AMG_DXT_64.exe | 64 | 1.8 MB | 3 066 | border extraction |
| Worker | 3D\DEF_LOCREM3_64.exe (+ `_P4P_64`) | 64 | 8.8 MB | 14 602 | local remesh; P4P variant links MPICH2 |
| Worker | 3D\CHK_VOL_64.exe | 64 | 2.6 MB | 4 990 | volume check, MPICH2 |
| Worker | 3D\DEF_BRK2.exe, DEF_SPLDB.EXE, DEF_CMESH.EXE | 32 | 1.9 / 1.1 / 0.3 MB | n/a | no 64-bit build exists; called on the 64-bit path |
| Shared | DEF_PRE_64.exe | 64 | 6.8 MB | 13 021 | already being ported; harness work is integration only |

Not ported, used as they are: MPICH2 (`mpiexec.exe`, `smpd.exe`, `mpich2mpi.dll`, `fmpich2.dll`),
NVIDIA CUDA 4.2 runtime DLLs, Windows. Licence, Sentinel and process-monitor code is never
analysed or recreated (section 4).

### 1.3 DEF_SIM_64 is mostly third-party code, and its own modules are named

The vendor ships `DEF_SIM_USR64_LIB.lib` (84 MB, built the same day as DEF_SIM_64.EXE) so users
can relink DEF_SIM_64 with their own `usr_*.f` routines (Absoft 11 makefile next to it). It holds
10 486 COFF objects and 30 257 public symbols. Classified by object path:

| Part | Objects | Object bytes | Public symbols | Treatment |
|---|---:|---:|---:|---|
| DEFORM's own Fortran and C/C++ | ~280 | ~22 MB | ~6 700 | reconstruct function by function |
| GotoBLAS2 kernels + reference BLAS/LAPACK | ~7 000 | 22 MB | ~5 900 | upstream library, matched version |
| MUMPS (d and z) | 48 | 12.5 MB | 1 690 | upstream library, matched version |
| SPOOLES | 319 | 4.3 MB | 5 327 | upstream library, matched version |
| ScaLAPACK / PBLAS / BLACS | ~850 | 3.5 MB | ~1 600 | upstream library, matched version |
| METIS, pARMS/ITSOL, expat, dlmalloc | ~70 | 1.2 MB | ~800 | upstream library, matched version |
| Licence, protection, process monitor, network config | ~95 | 4.0 MB | ~3 200 | skip (section 4) |
| MSVC CRT, compiler runtime, import stubs | ~1 430 | 1.2 MB | ~2 200 | replaced by the build's own runtime |

The Absoft Fortran runtime is not in the library (the Absoft linker adds it); it is inside
DEF_SIM_64.EXE and is recovered from there (packet S2-01).

**Relink works (2026-10-22).** With Absoft 11.1 installed by the user in `C:\Absoft11.1`, the
vendor makefile run on a copy (`deform-rea-work\simrelink`, `amake2 -f
DEF_SIM_USR64_Absoftv110.amake` after `absvars64.bat`) compiled the 17 shipped `usr_*.f` files and
linked a DEF_SIM_64.EXE with only CRT import warnings. It has the same ten imported DLLs as the
shipped exe and 6 609 `.pdata` functions against 6 619. It is 18.6 MB against 32.3 MB (`.text`
16.8 MB against 30.4 MB); H0-05 explains the gap (most likely static data or build differences,
since the shipped exe was built in-house, not through this makefile). It has not been run.

Every DEFORM Fortran source file is one object, and Absoft makes each subroutine a named public
symbol (`STIFF_INF`, `TIMSTP`, ...). The own modules, largest first, with public symbols:

| Area (role is a guess from the name) | Objects (KB, public symbols) |
|---|---|
| driver, input, time stepping | main (1 629, 259), main2 (180, 35), setupa (14, 14), timstp (232, 47), cnritr (208, 49) |
| element, material, constitutive | stiff (560, 96), matr (215, 75), epstf (692, 102), dexp (790, 178) |
| state-variable update | stvupd1 (551, 104), stvupd2 (2 170, 164) |
| solution, equation solvers | soluty (711, 169), eq (333, 143), ne (16, 13), neweq (1 061, 246), banwth (94, 24), blascg (171, 33), cblascg (52, 56), dgmr (103, 21), parm_sol (113, 34), dmumps_sol (225, 33), libdefsolve (160, 134), SymbFac (10, 10) |
| domain decomposition / MPI | ddm4 (775, 194), ddm4c (4, 9), ddm4_null (2, 17), ddm3, dmesh (431, 209) |
| contact, boundary, objects | srch (905, 179), intf (574, 105), cccnt (557, 113), bdycnd (371, 41), movctr (187, 51), rotaxi (233, 62), spring (121, 22), uobjec (293, 108), fresuf (310, 66) |
| remeshing / restart support | rre (1 638, 378), rse (415, 69), tfw (204, 48), explbr (181, 68), layer (99, 22), fractr (75, 26) |
| heat transfer, induction, furnace | 3dt (637, 105), view3 (654, 45), indct_fem3 (926, 145), furnace (151, 43), carbon (75, 21) |
| microstructure | texture (470, 138), grain (186, 46), grain2 (64, 17), trnmgr1 (87, 43), trnmgr2 (103, 28), hdnmgr (16, 6) |
| sensitivity, user routines | sensi (153, 29), `usr_*.f` + `def_usr.f` (shipped as source, 17 files) |
| DB core and Fortran utilities | datarm23 (252, 99; also in DEF_PRE), FTN_forutl1/2/3, FTN_math, FTN_math3f1/f2, FTN_geoutl, FTN_lib_pc |
| C/C++ support libraries | libdefmath3_c (26 objs, 683), libdefbucket3 (9, 385), libdefutl (5, 149), libdeffor_c (4, 57), sdctl_* (190), ckfemlib (256) |

Two facts shape the plan. First, the names give every DEF_SIM_64 function a source module and a
name once the library is matched against the exe (H0-03). Second, several modules are shared with
programs already being ported (`datarm23.f`, `eq.f`, `ne.f`, `neweq.f`, `FTN_*` in DEF_PRE_64),
so their C++ is reused, not redone.

### 1.4 The 2D chain

The `2D\` folder has its own builds of every program in the chain (different hashes from the 3D
ones), except DEF_PRE.EXE, which is the same file in the install root, `2D\` and `3D\`. From the
strings of `2D\DEF_ARM_CTL.com` and `2D\DEF_ARM.com`:

```
DEF_GUI23.exe
 └─ DEF_SIMULATION.exe
     └─ 2D\DEF_ARM_CTL.COM <prob> <c>
         ├─ [MST] DEF_RDMS.EXE "<prob>" [<n>]          (2D build)
         ├─ [MST] DEF_PRE.EXE >> <log>
         ├─ 2D\DEF_ARM.COM "<prob>" B >> <log>
         │    ├─ DEF_SIM.EXE > <msg>                   serial 32-bit solver, no MPI
         │    ├─ DEF_DEL.EXE                           mesh/element deletion (guess)
         │    ├─ DEF_INT.EXE < DEF_INT.INP              interpolation
         │    ├─ DEF_AMG.EXE < DEF_AMG.INP              2D mesh generation
         │    ├─ AMG_DXT.EXE                            2D border extraction
         │    ├─ DEF_RDMG.EXE
         │    └─ DEF_PRE.EXE < DEF_PRE.INP > DEF_PRE.OUT
         └─ FILE_REMOVE.EXE                             (2D build)
```

All of it is 32-bit and none of it imports MPI, so the whole 2D chain runs on Sim2. Census
(32-bit images, no `.pdata`; H0-01 counts functions):

| Program (`2D\`) | Size | Fortran idents | Notes |
|---|---:|---:|---|
| DEF_SIM.exe | 16.9 MB | 84 | Absoft 32-bit, mostly x87 floating point (a byte-pattern count finds about 6 x87 instructions per SSE2 one) |
| DEF_ARM.com | 0.51 MB | 0 | C; reads DEFARM.STA, SPAWN_PRIO.INI, STEP.DAT, DEFORM_FEMRUNPRE.DAT |
| DEF_ARM_CTL.com | 0.25 MB | 0 | C; MST loop |
| DEF_RDMS.exe, FILE_REMOVE.exe | 0.78 / 0.06 MB | 8 / 0 | 2D builds |
| DEF_DEL.exe, DEF_INT.exe | 1.06 / 1.32 MB | 12 / 32 | workers |
| DEF_AMG.exe, AMG_DXT.exe | 1.35 / 0.96 MB | 27 / 10 | workers |
| DEF_RDMG.exe | 0.08 MB | 1 | worker |
| DEF_PRE.EXE | 4.9 MB | 49 | 32-bit build of the program the DEF_PRE port targets as `DEF_PRE_64` |

**2D solver library.** `2D\UserRoutine\DEF_SIM\DEF_SIM_USR_LIB_Absoftv110.lib` (40.7 MB, 9 823
objects, 24 266 public symbols) does for 2D what the 64-bit library does for 3D. DEFORM's own
part is about 90 objects; without the licence and network-config objects it is roughly 45 Fortran
modules: main1, main2, intfce, intfce_pst, interface, steady_tmp, sensi, stiff, epstf, matr,
bdycnd, diepro, impact, spring, uobjec, timstp, cnritr, setupa, elapse, infini, soluty, banwth,
gelim, dmumps_sol, mpi/mpic (serial MPI stand-in, guess), indct, indct_fem, viewfactor, carbon,
grain, grain2, trnmgr, trnmgr_na, hdnmgr, elerst, fractr, tree, amgrem, st, def_sim, datarm23,
the `FTN_*` utilities, sdctl_*, ckfemlib, libdefutl, libdeffor_c, libdefsolve. Third-party: MUMPS
(d, z), SPOOLES, PORD-style ordering (ddcreate, gbisect, nestdiss, multisector, ...), BLAS.
The 2D modules are a separate code base from the 3D ones (different file names and sizes), so
they get their own packets; shared pieces (`datarm23`, `FTN_*`, sdctl, libdefutl) are reused.

**2D relink works too.** On a copy (`deform-rea-work\simrelink2d`, `absvars32.bat` then
`amake2 -f DEF_SIM_USR_Absoftv110.amake`) Absoft 11.1 linked a 32-bit DEF_SIM.EXE with no errors:
same five imported DLLs and 84 Fortran idents as the shipped one, 7.9 MB against 16.9 MB (the
same kind of gap as in 3D, explained in H0-05). Not run.

## 2. Architecture of the port (kept as is)

* One executable per original program, same file name (including the `.COM` extension), placed
  in the same folder layout as the install (`2D\` and `3D\` builds stay separate programs).
* Each port keeps the bitness of its original: 64-bit for the 3D solver and workers, 32-bit for
  the drivers, the 3D 32-bit helpers and all of 2D. The 2D solver does its floating point mostly
  on the x87 unit (80-bit intermediates), so byte-identical 2D results need a 32-bit build with
  x87 code and the same precision-control word; keeping every program's bitness also keeps the
  harness exactly as shipped. (This replaces the earlier default of building the 32-bit drivers
  as x64; the user can revise it.) The shared C++ libraries build for both x86 and x64.
* Programs talk only the way the originals do: command lines, environment variables, stdin
  scripts, redirected stdout, and files in the problem folder. No new IPC, no merged processes.
* DEF_SIM_64 keeps its MPI structure: N ranks started by `mpiexec` through DEF_MPI_CTL_64, the
  same rank roles, the same messages. The port links MPICH2's `mpi.h` ABI against the shipped
  `mpich2mpi.dll`/`fmpich2.dll`, so original and ported ranks never mix in one job but either
  binary runs under the same launcher.
* Third-party numerics are the same upstream libraries at the same versions and build options,
  compiled from upstream source, and checked against the original objects function by function
  (T-packets). "Recreate every internal function as is" is met by using the same code.
* Shared C++ libraries come from the existing superbuild in `port/` (`defrt`, `deformdb`,
  `defdbio`, ...). New libraries: `defabsoft` (Absoft runtime behaviour), `defsim` (solver
  modules), `defmpi` (MPI glue), `defharness` (control-app helpers: spawning, logs, env files),
  `defremesh` (code shared by the remesh workers).
* Traceability rules are those of `docs/port-conventions.md`: `// @src <addr>` on every function,
  per-packet `docs/re/status/<pkt>.tsv`, one ledger per binary
  (`docs/re/<binary>_ledger.tsv`) folded at merge time. For DEF_SIM_64 the ledger also carries the
  library object and symbol name of each function.

## 3. How correctness is proven

Four levels, from cheapest to most complete. Every packet uses the levels that apply to it.

1. **Function oracle (object level).** For DEF_SIM_64, the original object for a routine is linked
   from the vendor library into a test executable next to the C++ port of the same routine, and
   both are called on the same inputs, with COMMON blocks seeded from captured state. Bit-identical
   results are the bar. Only compute objects whose link closure is free of licence and
   process-monitor objects are used this way; others fall to level 3. H0-05 proves this works
   before any S-packet relies on it. For programs without a library, the existing binary-routine
   harness from the DEF_PRE port (`p1-01`) is used where it applies.
2. **Program oracle.** Run the original and the port on a copy of the same input files with the
   same command line and environment; compare every output file (DB with `defcmp`, text with
   masks for times, dates, PIDs and host names, exit code, stdout). Extends the DEF_PRE oracle
   runner and its hash cache (`tools/oracle/`).
3. **Hybrid build (strangler).** The vendor library relinks with the installed Absoft 11.1
   (section 1.3), so a DEF_SIM_64 is linked from ported objects plus the remaining original
   objects, with Absoft's own runtime; S2-01 is then needed only for the pure port, not for the
   hybrid. Each S-packet then swaps its modules in and runs whole simulations
   unchanged otherwise. The hybrid keeps the vendor's licence objects untouched, as the vendor's
   own user-routine relink does; the finished port contains no licence code, as in DEF_PRE.
   If relinking fails, level 2 runs per step instead (one solver call between DB steps).
4. **Chain oracle.** The whole Run chain on a corpus of 3D problems, original against port and
   against mixes ("swap one program at a time"), from DEF_ARM_CTL3 down. The process tree with
   command lines and environments is recorded through Windows process-creation events (no
   debugger, no change to any DEFORM file), and every file in the problem folder is compared
   after each run.

Determinism: byte identity is required for serial runs (`np = 1`) and for each fixed
`np` with the same MPI channel; runs with different rank counts are compared with the masks and
tolerances `defcmp` already has. GPU (`DEF_GPU.DAT`) runs are compared with tolerances only.

Corpus: the shipped `3D\3D_Examples` and `Tutorials` problems, COG_P1_B1, plus problems built by
the DEF_PRE oracle to reach rare branches (remesh types, MST files, heat treatment, induction,
microstructure, user routines). H0-08 picks the set and the step limits that keep a run short.

## 4. Rules carried over

* Never modify a DEFORM binary or file; all runs work on copies in a private work folder.
* Licence, Sentinel, `network_license`, `NP*` protection and process-monitor (`sdspm_*`,
  `DFMProcessMonitor`) code is not analysed, recreated or bypassed. Where a ported function calls
  it, the call site is kept as a documented no-op; the port starts no licence check.
* Static analysis on copies (Ghidra headless for bulk, REA for single functions), recovered code
  stays private, nothing is pushed to a public place.
* Children are spawned with explicit argv; data and cache paths are never opened through the shell.
* Downloads of upstream library sources (T-packets) need the user's approval before they happen.

## 5. Phases and packets

Packets are sized for one thread each. "Deps" are hard dependencies; everything else can run at
the same time. Waves show how the packets fill 17 threads; a thread that finishes early takes the
next packet whose deps are met.

### Phase 0: evidence and tools (wave 1)

| ID | Packet | Output | Deps |
|---|---|---|---|
| H0-01 | Ghidra import of all in-scope binaries, 3D and 2D (own project copy per binary), function lists, string refs, RTTI, Fortran idents | `out/harness/<bin>_funcs.tsv`, census table replacing 1.2 and 1.4 | none |
| H0-02 | Vendor library maps, 64-bit 3D and 32-bit 2D: members, symbols, COMMON blocks, relocations per object; own / third-party / skip classes | `docs/re/sim64/` and `docs/re/sim2d/`: `lib_members.tsv`, `lib_symbols.tsv`, `commons.tsv` | none |
| H0-03 | Name DEF_SIM_64 and 2D DEF_SIM: match library objects to exe functions (bytes with relocations masked), then propagate to the workers that share modules (DEF_REM_64, CHK_VOL_64, DEF_LOCREM3_64, 2D DEF_INT/DEF_AMG, ...) | `names.tsv` per binary, a Ghidra script that applies them | H0-01, H0-02 |
| H0-04 | Identify third-party versions and build options (GotoBLAS2, LAPACK, MUMPS, ScaLAPACK/PBLAS/BLACS, SPOOLES, METIS, pARMS/ITSOL, expat, dlmalloc, MPICH2 ABI) in DEF_SIM_64 and the workers | `docs/re/thirdparty.md` with version evidence; download list for approval | H0-02 |
| H0-05 | Relink and hybrid spike, for both the 64-bit 3D and the 32-bit 2D library. The plain relinks already work with Absoft 11.1 (sections 1.3, 1.4). Remaining: explain the size gap against the shipped exe; run the relinked exe on one corpus problem against the shipped one; link a ported C++ object in place of one original object (MSVC objects into the Absoft link); link single original objects into a C++ test exe (function oracle) | yes/no for levels 1 and 3, with a working example | H0-02 |
| H0-06 | Harness contract: every launch site in DEF_GUI23 (Run, Run options, MST, queue), DEF_SIMULATION, ARM_CTL3, DEF_ARM, MPI_CTL_64, and the 2D DEF_ARM_CTL and DEF_ARM; exact argv, env vars, cwd, redirections, exit codes; the `%c` mode letter; the `64bit.DAT` test in every program that has it, and what `DEF_MPIp4penv.DAT` does in DEF_ARM, DEF_MPI_CTL_64 and DEF_SIM_64 | `docs/re/harness-contract.md` | none |
| H0-07 | File catalogue: every file the chain reads or writes (the Run-options files `64bit.dat`, `DEF_MPIp4penv.DAT`, `DEF_MPIenv.DAT`, `DEF_MAIL.INI`, `DEFORM3D.PROB`; env DATs, DefMpi.tmp, DEF_SIM_DIR.DAT, DEF_GPU.DAT, SPAWN_PRIO.INI, JobGroupSetupConfigureFile.txt, internal.ctl, *.INP/*.OUT/*.INI, FOR0nn, MSG/LOG files, JOBLOG) with formats | `docs/re/harness-files.md` | none |
| H0-08 | Chain oracle runner: run the original chain on copies with step limits, record the process tree and file trees, cache by hash; pick the corpus, 3D and 2D (`2D\2D_Examples`) | `tools/oracle/chain_oracle.py`, `docs/oracle-chain.md` | H0-06 (draft is enough) |
| H0-09 | Comparators: MSG/LOG/OUT text masks, FOR0nn unformatted files, DB step compare modes, folder-tree diff | `defcmp` extensions | none |

The control-app packets of Phase 1 start in the same wave (8 threads), giving 17.

### Phase 1: control apps, drop-in (waves 1 and 2)

Each packet ports one program completely and proves it with the program oracle; the gate swaps
them into the original chain.

| ID | Packet | Deps |
|---|---|---|
| H1-01 | DEF_MPI_CTL_64.COM: env-file parsing, `mpiexec` command building (`-localonly`, `-channel NEMESIS`, `-hosts`, `-map`, `-dir`, `-envlist`), DefMpi.tmp, DEF_SIM_DIR.DAT, DEF_GPU.DAT, kill path, messages | none |
| H1-02 | DEF_ARM.COM A: start-up, argument and mode handling, version check, JOBLOG, run sequencing, timing and elapsed-time text | none |
| H1-03 | DEF_ARM.COM B: remesh pipeline (border extraction, AMG, interpolation, local remesh, brick, parallel mesh), retries and error texts | H1-02 skeleton |
| H1-04 | DEF_ARM.COM C: solver launch, 32/64 and MPI/serial selection, monitoring thread, retry and abort, SPAWN_PRIO, DDM/CMESH helpers | H1-02 skeleton |
| H1-05 | DEF_ARM_CTL3.COM: controller, batch/interactive banner, MST loop with DEF_RDMS3 and DEF_PRE_64, batch report, FILE_REMOVE | none |
| H1-06 | DEF_SIMULATION.exe: env-var job description, job id, job types (MO path in full; DOE/OPT dispatch kept as calls to the original programs), log, clean-up | none |
| H1-07 | DEF_RDMS3.exe: MST parsing and per-run unpacking | none |
| H1-08 | FILE_REMOVE, CLEANMPIJOB.COM, KillMPI, DEF_CTRL.com (cogging path, COG_CTRL3 called as is); KillMPI is checked statically only, since it needs the 32-bit MPI (1.1a) | none |
| H1-09 | DEF_TEXT_PST.exe batch-report path used by ARM_CTL3 (headless), then the rest of the program | H0-06 |
| H1-10 | DEFJobGroupSetup.exe and BATCH_QUEUE.exe (Run options, job groups, queue) | H0-06, G1 |
| G1 | Gate: ported control apps + original workers and solver pass the chain oracle on the corpus; merge to main | H1-01..08, H0-08 |

### Phase 2: solver foundation (wave 2)

| ID | Packet | Deps |
|---|---|---|
| S2-01 | `defabsoft`: Absoft Fortran runtime behaviour recovered from DEF_SIM_64.EXE (formatted and list-directed I/O, unformatted sequential and direct files, OPEN/INQUIRE, string intrinsics, math library, stop/exit), with the same output bytes | H0-03 |
| S2-02 | COMMON blocks and module data as C++ layouts, from object symbols and relocations; static data images from the exe | H0-02, H0-03 |
| S2-03 | Program skeleton: `main.f` entry, rank start-up, argument and file opening, step loop shell with an external-call seam to not-yet-ported routines, licence and monitor call sites as no-ops | S2-01, S2-02 |
| S2-04 | Shared modules: diff DEF_SIM_64's `datarm23`, `eq`, `ne`, `neweq`, `FTN_*` against the DEF_PRE port; reuse or extend the C++ there | H0-03 |
| T-01 | GotoBLAS2 + reference BLAS/LAPACK at the matched version, with the same kernel selection per CPU | H0-04, user approval |
| T-02 | MUMPS + ScaLAPACK/PBLAS/BLACS at matched versions, MPICH2 ABI | H0-04, user approval |
| T-03 | SPOOLES, METIS, pARMS/ITSOL, expat, dlmalloc at matched versions | H0-04, user approval |

### Phase 3: solver modules (waves 2 to 4)

Packets follow source modules, so ownership is by object file and never overlaps. Large modules
are split by subroutine ranges once H0-03 gives the names. Each packet: port, function oracle,
hybrid swap-in (or per-step program oracle), status file.

| ID | Modules | Deps |
|---|---|---|
| S3-01 | main (input, DB read, initialisation half) | S2-03 |
| S3-02 | main (step-loop half), main2, setupa, timstp, cnritr | S2-03 |
| S3-03 | stiff, matr | S2-02 |
| S3-04 | epstf | S2-02 |
| S3-05 | dexp | S2-02 |
| S3-06 | stvupd1 | S2-02 |
| S3-07 | stvupd2, first half | S2-02 |
| S3-08 | stvupd2, second half | S2-02 |
| S3-09 | soluty, banwth | S2-02, S2-04 |
| S3-10 | iterative solvers: blascg, cblascg, dgmr, parm_sol, libdefsolve, SymbFac | T-03 |
| S3-11 | direct solver glue: dmumps_sol, MUMPS and SPOOLES call layers | T-02, T-03 |
| S3-12 | ddm4, ddm4c, ddm4_null, ddm3 (domain decomposition, MPI exchange) | S2-03 |
| S3-13 | dmesh | S2-02 |
| S3-14 | srch (contact search) | S2-02 |
| S3-15 | intf, cccnt | S2-02 |
| S3-16 | bdycnd, movctr, rotaxi, spring, uobjec | S2-02 |
| S3-17 | fresuf, explbr, layer, fractr | S2-02 |
| S3-18 | rre, first half | S2-02 |
| S3-19 | rre, second half; rse | S2-02 |
| S3-20 | tfw, sensi | S2-02 |
| S3-21 | 3dt, view3, furnace, carbon | S2-02 |
| S3-22 | indct_fem3 | S2-02 |
| S3-23 | texture, grain, grain2, trnmgr1, trnmgr2, hdnmgr | S2-02 |
| S3-24 | user routines (`usr_*.f`, `def_usr.f` from shipped source) and their call sites | S2-02 |
| S3-25 | C/C++ support: libdefmath3_c, libdefbucket3, libdefutl, libdeffor_c, sdctl_*, ckfemlib | S2-01 |
| S3-26 | GPU path: `DEF_GPU.DAT`, CUDA 4.2 cuBLAS/cuSPARSE call layer, same fallback rules | S3-10, S3-11 |
| G3 | Gate: pure-port DEF_SIM_64 (no original objects) passes the program oracle per step and the chain oracle, serial and at fixed np; merge | S3-01..25 |

### Phase 4: remesh workers (waves 3 to 5)

Shared modules found by H0-03 go to `defremesh` once, owned by the first packet that needs them.

| ID | Program | Deps |
|---|---|---|
| W4-01 | DEF_REM_64, part A (input, mesh mapping) | H0-03, S2-01 |
| W4-02 | DEF_REM_64, part B (interpolation, MPI path) | W4-01 skeleton |
| W4-03 | DEF_AMG_64, part A | H0-03 |
| W4-04 | DEF_AMG_64, part B | W4-03 skeleton |
| W4-05 | AMG_DXT_64 | H0-03 |
| W4-06 | DEF_LOCREM3_64, part A | H0-03 |
| W4-07 | DEF_LOCREM3_64, part B | W4-06 skeleton |
| W4-08 | DEF_LOCREM3_64, part C, and the `_P4P_64` MPI variant as a build option | W4-06 skeleton |
| W4-08a | `DEF_LOCREM3.EXE`, the 32-bit serial local remesher DEF_ARM runs on the 64-bit path (1.1a); a 32-bit build sharing the W4-06..08 code where the two images match | H0-03, W4-06 |
| W4-09 | CHK_VOL_64 | H0-03 |
| W4-10 | DEF_BRK2, DEF_SPLDB, DEF_CMESH (32-bit helpers, 32-bit builds) | H0-01 |
| W4-11 | DEF_PRE_64 integration: confirm the stdin scripts DEF_ARM and ARM_CTL3 write are covered by the DEF_PRE port | DEF_PRE port gates |
| G4 | Gate: every worker passes the program oracle; chain oracle with all workers ported | W4-01..11 |

### Phase 6: the 2D chain (waves 2 to 6)

Same method as the 3D phases. All 2D programs are 32-bit builds with x87 floating point
(section 2). Nothing in the 2D chain needs MPI, so every 2D program runs as an oracle on Sim2.
The 2D packets carry a `D` in their ids.

| ID | Packet | Deps |
|---|---|---|
| D1-01 | 2D DEF_ARM.com: start-up, DEFARM.STA, run sequencing, solver launch, remesh pipeline (DEF_DEL, DEF_INT, DEF_AMG, AMG_DXT, DEF_RDMG, DEF_PRE), monitoring, logs; shares `defharness` with H1-02..04 | H0-06 |
| D1-02 | 2D DEF_ARM_CTL.com, 2D DEF_RDMS.exe, 2D FILE_REMOVE.exe (MST loop, unpacking, clean-up) | H0-06 |
| DG1 | Gate: ported 2D control apps + original 2D workers and solver pass the 2D chain oracle | D1-01, D1-02, H0-08 |
| D2-01 | 2D solver foundation: 32-bit `defabsoft` (x87 precision control, the 32-bit Absoft runtime recovered from 2D DEF_SIM.exe), COMMON layouts, program skeleton (`def_sim`, `main1` entry) with the external-call seam | H0-03, S2-01 |
| D2-02 | 2D third-party: MUMPS, SPOOLES, PORD ordering, BLAS as 32-bit builds at the versions the 2D library holds (H0-04) | H0-04, user approval |
| D3-01 | main1 | D2-01 |
| D3-02 | main2, setupa, timstp, cnritr, elapse, infini | D2-01 |
| D3-03 | stiff, matr, epstf | D2-01 |
| D3-04 | intfce, intfce_pst, interface | D2-01 |
| D3-05 | steady_tmp | D2-01 |
| D3-06 | sensi | D2-01 |
| D3-07 | bdycnd, diepro, impact, spring, uobjec | D2-01 |
| D3-08 | soluty, banwth, gelim, dmumps_sol, mpi/mpic, libdefsolve, symbfac | D2-01, D2-02 |
| D3-09 | indct, indct_fem, viewfactor, carbon | D2-01 |
| D3-10 | grain, grain2, trnmgr, trnmgr_na, hdnmgr | D2-01 |
| D3-11 | elerst, fractr, tree, amgrem, st; 2D datarm23 and `FTN_*` deltas against S2-04 | D2-01, S2-04 |
| D3-12 | 2D user routines (12 shipped `usr_*.f` + `def_usr.f`); C support (sdctl, ckfemlib, libdefutl, libdeffor_c) reused from S3-25 as 32-bit builds | D2-01, S3-25 |
| DG3 | Gate: pure-port 2D DEF_SIM passes the program oracle per step and the 2D chain oracle | D3-01..12 |
| D4-01 | 2D DEF_INT.exe | H0-03 |
| D4-02 | 2D DEF_AMG.exe | H0-03 |
| D4-03 | 2D AMG_DXT.exe | H0-03 |
| D4-04 | DEF_DEL.exe, DEF_RDMG.exe | H0-03 |
| D4-05 | DEF_PRE.EXE: 32-bit build of the DEF_PRE port, checked against the shipped 32-bit DEF_PRE.EXE on the scripts the 2D chain writes | DEF_PRE port gates |
| DG4 | Gate: every 2D worker passes the program oracle; 2D chain oracle with everything ported | D4-01..05 |

### Phase 5: whole-chain acceptance (wave 6)

| ID | Packet | Deps |
|---|---|---|
| A5-01 | Fully ported chains (3D 64-bit and 2D) against the originals on the whole corpus, serial and MPI, plus every one-program swap | G1, G3, G4, DG1, DG3, DG4 |
| A5-02 | Mother-app check: DEF_GUI23 started from a copied install folder whose programs are the ports; Run, Run (options), MST and queue, done by the user on their machine with a written checklist | A5-01, H1-10 |
| A5-03 | Failure paths: abort by user, missing DB, MPI failure and retry, remesh failure, disk full; messages and exit codes match | A5-01 |
| A5-04 | Ledgers complete: every function of every in-scope binary is ported, matched to an upstream library, or marked skip with a reason | all |

### Waves at 17 threads

| Wave | Packets running | Count |
|---|---|---:|
| 1 | H0-01..H0-09, H1-01..H1-08 | 17 |
| 2 | G1, H1-09, D1-01, S2-01..S2-04, T-01..T-03, S3-03..S3-08, S3-13 | 17 |
| 3 | S3-01, S3-02, S3-09..S3-12, S3-14..S3-21, W4-05, W4-09, D1-02 | 17 |
| 4 | S3-22..S3-26, H1-10, W4-01, W4-03, W4-06, W4-10, W4-11, DG1, D2-01, D2-02, D4-01, D4-02, D4-03 | 17 |
| 5 | W4-02, W4-04, W4-07, W4-08, W4-08a, D3-01..D3-12 | 17 |
| 6 | G3, G4, DG3, D4-04, D4-05, DG4, A5-01..A5-04 | 10 |

The S3 packets in wave 2 overlap S2-02 on purpose: they start from the names and decompiles, and
take each COMMON-block layout as S2-02 publishes it, the same layout-ownership pattern the DEF_PRE
port used in its Phase 1 (`docs/phase1-packets.md`). Waves 2 and 3 assume H0-03 names the exe
quickly and H0-05 succeeds. If the hybrid relink fails,
the S3 packets still run in parallel but are checked by function oracle and per-step program
oracle, and G3 carries more integration work.

## 6. Risks and how the plan handles them

| Risk | Effect | Handling |
|---|---|---|
| Hybrid link of MSVC-built C++ objects with Absoft objects fails (relink itself works) | no module-by-module whole-run check | H0-05 tests it first; per-step program oracle as fallback |
| Relinked exe behaves differently from the shipped one | hybrid is not a valid reference | H0-05 compares them on the corpus; the shipped exe stays the final reference |
| Library build differs from the shipped exe | names misattributed | H0-03 matches bytes, not names; mismatches are listed and handled by hand |
| Floating-point results differ (x87/SSE, FMA, reduction order) | no byte identity | compile flags from `DefPort.cmake` FP rules; MPI reductions recreated in the same order; serial first |
| 2D x87 code: MSVC's 32-bit x87 code generation spills and rounds differently from Absoft's | 2D byte identity hard to reach | D2-01 pins the precision-control word and finds which routines differ through the 2D hybrid; those get hand-placed `long double` or explicit rounding; tolerance compare is the fallback, recorded per routine |
| GotoBLAS2 picks kernels by CPU | results depend on the machine | T-01 builds the same kernel set and the same selection code; oracle runs pin the machine |
| Licence code tangled with solver code | parts look unportable | call sites kept as no-ops; such objects are checked at program level only |
| Original runs need a licence seat and time | slow oracle | step limits, hash cache, runs only on the user's licensed machine |
| Run artefacts depend on time, PID, host | false diffs | masks in H0-09 |

## 7. Defaults chosen for the user to revise

* Scope is the 3D chain on the 64-bit path plus the whole 2D chain (32-bit); the 3D 32-bit solver
  and 3D 32-bit MPI workers stay out.
* Every port keeps its original's bitness (section 2); DOE and optimisation job types stay with
  the original programs.
* Third-party numerics are rebuilt from upstream source, not decompiled.
* DEF_TEXT_PST, DEFJobGroupSetup and BATCH_QUEUE come last; until then the originals are used.
* Work happens on branches `claude/sh-<packet>` from main, one worktree and one Ghidra copy per
  packet (`ghidra_sh<packet>`), merged by gate threads as in the DEF_PRE port.

## 8. How the open-source libraries get into the C++ programs

Rule: the open-source libraries are never translated into C++. They stay in their own languages
and are compiled and linked as they are; only DEFORM's own code is reconstructed in C++.

What the libraries are written in (the original objects show it):

| Library | Language | How C++ calls it |
|---|---|---|
| GotoBLAS2 (kernels, threading, its LAPACK subset) | C + assembly | Fortran-style symbols `DGEMM`/`dgemm_`, or CBLAS |
| Reference BLAS/LAPACK parts (`.\Objs`, `.SRC*.f`) | Fortran 77 | Fortran ABI |
| MUMPS (d, z) | Fortran 90 core, C I/O layer | its C interface (`dmumps_c`, `zmumps_c`) |
| ScaLAPACK | Fortran; PBLAS and BLACS are C | Fortran ABI; `Cblacs_*` |
| SPOOLES, METIS, pARMS/ITSOL, expat, dlmalloc | C | plain C headers |

So only LAPACK, ScaLAPACK and the MUMPS core need a Fortran compiler. Options:

| Option | How | Fit |
|---|---|---|
| **A. Mixed-language superbuild (recommended)** | CMake `enable_language(Fortran)` with Intel ifx (free, links with MSVC) or LLVM flang (MSVC ABI); upstream sources at the matched versions built as static libraries inside `port/thirdparty/`; C libraries built with MSVC; C++ calls through small `extern "C"` headers (one per library) that state the Fortran ABI: upper-case names, by-reference arguments, hidden string-length arguments, 32-bit integers as the original uses | One build, one toolchain, same code paths as the original, easy to step through |
| B. Prebuilt packages | vcpkg or conda-forge builds of OpenBLAS, LAPACK, ScaLAPACK, METIS, MUMPS; linked as static or DLL | Least work, but versions and build options differ from 2014, so results drift from the original |
| C. Vendor maths (Intel oneMKL) | MKL for BLAS, LAPACK, ScaLAPACK; MUMPS and SPOOLES still built | Fastest numerics, but different algorithms, so no byte identity; a build option for later, not the reference |
| D. Machine translation (f2c) | Fortran turned into C | Rejected: the concern you raised, and harder to debug |
| E. Reuse the vendor's original objects | link third-party members of `DEF_SIM_USR64_LIB.lib` | Only as an oracle and in the hybrid (H0-05, works with the installed Absoft 11.1); not ours to ship |

Defaults with option A: the matched-version upstream build is the reference used by every oracle
and gate; B and C can be added later as switches in the same CMake (`DEF_NUMERICS=reference|
openblas|mkl`), and are checked against the reference with tolerances. GotoBLAS2 is unmaintained;
T-01 first tries building it as is with clang-cl, and if that fails it uses OpenBLAS (its fork)
pinned to the same kernels. The Fortran compiler is Intel ifx: the user reports that SFTC itself
built DEFORM v11 successfully with the Intel Fortran compiler in place of Absoft 11, so the
library sources are known to build with it. LLVM flang is the fallback. No Fortran compiler is
installed on this machine yet; installing ifx needs the user's approval and is the first step of
T-01. ifx is also tried in H0-05 for compiling the shipped `usr_*.f`; it does not remove the need
for the Absoft runtime (S2-01) when relinking the vendor's Absoft-built objects.

## 9. Volume compared with the DEF_PRE_64 port

DEF_PRE_64 reference: 5 255 functions in the batch scope, about 2.5 MB of code, ported in about
1.5 days with up to ~15 threads (roughly 3 500 functions a day).

How the numbers below were obtained:
* **Solvers, measured:** function symbols in the vendor libraries' own objects, after removing
  third-party, licence, process-monitor and network-config objects. Code is the code-section
  bytes of those objects.
* **Other programs, measured proxy then estimated:** the measured figure is the number of distinct
  direct-call targets in `.text`; on DEF_PRE_64 that proxy gives 9 574 against 8 666 application
  functions found by Ghidra. The hand-port figure takes off the C runtime and the licence block
  every DEFORM program carries (in DEF_PRE_64 these were 4 469 functions), so it is a range.
  H0-01 replaces these with Ghidra counts.

| Part | Measured | Functions to port by hand (est.) | Already covered by the DEF_PRE port | Code |
|---|---:|---:|---:|---:|
| 3D solver DEF_SIM_64 | 4 110 own functions | 4 100 | up to 576 (datarm23, eq, ne, neweq, FTN_*, sdctl, libdefutl) | 8.7 MB |
| 2D solver DEF_SIM | 1 521 own functions | 1 500 | about 160 (datarm23, FTN_*, sdctl) | 2.3 MB |
| 3D workers (REM_64, AMG_64, AMG_DXT_64, LOCREM3_64, CHK_VOL_64, BRK2, SPLDB, CMESH; LOCREM3 32-bit mostly the same code as _64) | 27 000 call targets | 15 000 to 19 000 | unknown; LOCREM3 carries DEF_PRE-like Fortran | about 17 MB text with runtime |
| 2D workers (DEL, INT, AMG, AMG_DXT, RDMG) | 9 000 call targets | 4 000 to 6 000 | DEF_PRE.EXE is the DEF_PRE port rebuilt 32-bit | about 3.9 MB text with runtime |
| Drivers and control apps, 3D and 2D (DEF_SIMULATION, ARM_CTL3/CTL, DEF_ARM x2, MPI_CTL_64, RDMS3/RDMS, CTRL, small tools) | 13 300 call targets | 4 000 to 6 000 (2D and 3D builds share much code) | none | about 4 MB text with runtime |
| **Core total** | | **29 000 to 37 000** | **under 1 000** | |
| Last tier: DEF_TEXT_PST, DEFJobGroupSetup, BATCH_QUEUE | 26 000 call targets (DEF_TEXT_PST alone 22 000, Qt 4) | 8 000 to 15 000 | none | 4.5 MB text |

Consequences for the plan:
* The core is about **6 to 7 times** DEF_PRE's scope. At DEF_PRE's pace and 17 threads that is
  about **9 to 11 days**; the solver numerics (floating-point identity, MPI, x87 in 2D) are harder
  than DEF_PRE's code, so **10 to 15 days** is the realistic range. The last tier would add
  roughly 3 to 5 days.
* The packet counts in section 5 (about 95) are sized for the solvers; the workers are not.
  DEF_LOCREM3_64 alone (about 10 000 call targets) is larger than DEF_PRE's whole scope. After
  H0-01 the worker rows W4-01..W4-10 and D4-01..D4-04 are split to DEF_PRE's density (about 60
  to 150 functions per packet), which takes the plan to roughly 250 to 350 packets.
* Workers that share Fortran modules with the solvers or with DEF_PRE (H0-03 finds them) shrink
  the hand-port numbers; the ranges above assume no sharing.
