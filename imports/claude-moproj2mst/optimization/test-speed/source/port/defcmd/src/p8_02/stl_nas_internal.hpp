// p8-02 internal declarations: every function of the packet (FUN_140243a70 subtree), with the
// binary's argument order. Bodies: stl_nas_driver.cpp (3D table, scaling, timers), stl_reader.cpp
// (_DEF_STL), stl_locator.cpp (point list / cell list helpers, defLOC), stl_check.cpp (the
// DEF_STL.OUT surface checker), scratch_readers.cpp (_DEF_PATRAN, _DEF_UNV), nastran.cpp
// (_DEF_NASTRAN). Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md sections 2 and 4.
#pragma once

#include <cstdio>

#include <defcmd/stl_nas.hpp>

#include "stl_types.hpp"

namespace def::cmd::stlnas {

using def::model::LLOBJ_DATA;

// ---- driver (stl_nas_driver.cpp) ----
// @src 140244e60  loader callback (loader+0x10)(arg), 1 when there is none
bool fun_140244e60(const LoaderView* loader, i32 arg);
// @src 1401fffa0  scale every curve (2D model) / surface (3D model) of obj by *factor (slot 6)
void fun_1401fffa0(LLOBJ_DATA* obj, const double* factor);
// @src 140222b60 / 140222b80  timer start / end: (float)GetTickCount() (140235710)
float fun_140222b60();
float fun_140222b80();
// @src 140235710  *t = (float)GetTickCount()  (outside the ledger)
void fun_140235710(float* t);

// ---- _DEF_STL (stl_reader.cpp) ----
StlObj* fun_140244ea0(StlObj* s);                 // ctor
void fun_140244f00(StlObj* s);                    // dtor -> 140244f20
void fun_140244f20(StlObj* s);                    // free the result arrays
bool fun_140244fa0(StlObj* s, const char* file, const float* tol, const char* write_check);
bool fun_140245b90(StlObj* s, const char* file);  // ASCII / binary detection
bool fun_140245cc0(StlObj* s, const char* file);  // ASCII reader
bool fun_140245ee0(StlObj* s, const char* file);  // binary reader
void fun_140246980(StlObj* s, void* value);       // byte-swap hook (empty)
bool fun_1402469a0(StlObj* s, i32 nn, i32 ne, const float* pts, const i32* cells);
bool fun_140246ad0(StlObj* s, LLOBJ_DATA* obj);   // store into surface 0

// ---- point list / cell list helpers and the point locator (stl_locator.cpp) ----
void fun_140411360(PointList* l);                 // trim: cap = count
float* fun_140411640(PointList* l, i32 k);        // point k (1-based), fatal out of range
float* fun_1404116a0(PointList* l);               // bounds (recomputed when dirty)
float* fun_140411850(PointList* l);               // bounds
float fun_140411880(const float* a, const float* b);  // squared distance (float)
void fun_1404051b0(CellList* c, i32 all);         // print the list (stdout)
void fun_1404053b0(CellList* c);                  // trim: cap = count
i32 fun_140405700(const CellList* c);             // width (+0x0c)
void fun_140405800(CellList* c, i32 k);           // reverse record k
void fun_14040ce80(Locator* l, float tol);        // ctor
void fun_14040cf80(Locator* l);                   // dtor
void fun_14040d270(Locator* l, PointList* pts, const float* bounds, i32 verbose);
i32 fun_14040d750(Locator* l, const float* p, i32 verbose);  // tolerant insert
i32 fun_14040dc00(Locator* l, const float* p, i32 verbose);  // exact insert
void fun_14040dfd0(Locator* l, const i32* idx, const i32* dims, i32 ring, i32 verbose);

// ---- surface checker (stl_check.cpp) ----
void fun_14040e980(SurfCheck* s, std::FILE* f, i32 level);   // summary
i32 fun_14040f4e0(SurfCheck* s, std::FILE* f, void* unused, i32 verbose);
bool fun_14040f9f0(SurfCheck* s, i32* seed);                 // next cell without patch
void fun_14040fa70(SurfCheck* s, i32 cell, i32 patch, i32 verbose);
void fun_14040ff60(SurfCheck* s, i32 cell, i32 a, i32 b, CellList* out);
void fun_140410130(SurfCheck* s);                            // patch boundaries
void fun_140410260(SurfCheck* s, i32 patch);
void fun_140410750(SurfCheck* s);                            // redundancy check (empty)
double fun_140410760(SurfCheck* s);                          // areas
double fun_140410d40(PointList* pts, const i32* tri, double* min_edge, double* max_edge);
void fun_140459c70(PatchData* p);
void fun_140459d50(std::FILE* f, PatchData* p, i32 level);
i32 fun_140459fb0(PatchData* p, i32 cell);
PatchData* fun_140459fe0(i32 n);

// ---- _DEF_PATRAN / _DEF_UNV (scratch_readers.cpp) ----
ScratchObj* fun_14024c170(ScratchObj* p);         // _DEF_PATRAN ctor
void fun_14024c1d0(ScratchObj* p);                // dtor
void fun_14024c1f0(ScratchObj* p);
i32 fun_14024c270(const ScratchObj* p);           // mode
bool fun_14024c290(ScratchObj* p, const char* file, const double* scale);
bool fun_14024c4b0(ScratchObj* p, const char* file);
bool fun_14024d4f0(ScratchObj* p, LLOBJ_DATA* obj);
bool fun_14024d650(ScratchObj* p, LLOBJ_DATA* obj, const i32* comp);
ScratchObj* fun_14024d790(ScratchObj* p);         // _DEF_UNV ctor
void fun_14024d7f0(ScratchObj* p);
void fun_14024d810(ScratchObj* p);
i32 fun_14024d890(const ScratchObj* p);
bool fun_14024d8b0(ScratchObj* p, const char* file, const double* scale);
bool fun_14024dad0(ScratchObj* p, const char* file);
bool fun_14024ee10(ScratchObj* p, LLOBJ_DATA* obj);
bool fun_14024ef70(ScratchObj* p, LLOBJ_DATA* obj, const i32* comp);

// ---- _DEF_NASTRAN (nastran.cpp) ----
NasObj* fun_14024f0a0(NasObj* n);                 // ctor (placement on raw storage)
void fun_14024f110(NasObj* n);                    // dtor
void fun_14024f160(NasObj* n);
i32 fun_14024f1b0(const NasObj* n);               // mode
bool fun_14024f1d0(NasObj* n, const char* file);
bool fun_14024f270(NasObj* n, const char* file);  // CTRIA3 fixed fields
bool fun_14024f620(NasObj* n, const char* file);  // CTETRA free, then fixed
bool fun_14024f670(NasObj* n, const char* file);  // CHEXA
bool fun_14024f6a0(NasObj* n, const char* file);  // CTETRA fixed fields
bool fun_14024fa50(NasObj* n, const char* file);  // CTETRA free fields
bool fun_14024fe90(NasObj* n, const char* file);  // CHEXA fixed fields
bool fun_140250280(NasObj* n, const char* file);  // type scan
bool fun_140251be0(NasObj* n, LLOBJ_DATA* obj);   // surface store (mode 1)
bool fun_140251d60(NasObj* n, LLOBJ_DATA* obj, const i32* comp);  // mesh store (modes 2, 4)

}  // namespace def::cmd::stlnas
