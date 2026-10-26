// defcmd/gfread.hpp: public API of packet p8-01 (GFREAD driver, the 2D reader front end shared
// by the IGES and DXF readers, and the small helpers those readers call).
// Specs: docs/re/commands/GFREAD.md, DXF_IGES_READERS.md. Namespace def::cmd::gfread.
//
// Uic2CObject family. The classes belong to p8-03 (Uic2CObject base, defcmd/uic2c_object.hpp;
// iges::Uic2CObjectIgesFile, vtable 0x140564680) and p8-04 (dxf::Uic2CObjectDxfFile, vtable
// 0x140563800). The packet map gives p8-01 the member bodies (ctor/dtor/name/field accessors).
// They are declared below as fun_<addr> on the raw layout, which the classes keep (MSVC x64, vptr
// at 0):
//   Uic2CObject          size 0x18: +0x08 int32 = 0; +0x10 char* name buffer (malloc'd, 0x80
//                        bytes at first, grown with malloc(len+1) by fun_140454a90; freed with
//                        free() by fun_140454980)
//   Uic2CObjectIgesFile  size 0x28: base, +0x18 int32, +0x1c int32 (boundary count, read by
//   Uic2CObjectDxfFile   fun_1402566b0 / fun_1402566c0), +0x20 double (tolerance); all zeroed.
// With the classes present the fun_ bodies call their C++ members (uic2c_members.cpp), so an
// object is never initialised or freed twice; the raw-layout code is the fallback.
//
// Object fields (LLOBJ_DATA, layout owned by defmodel; accessed here by offset because they lie in
// the layout's unknown gaps): +0x0bc8 int32 boundary count, +0x3a40 int32 geometry file type,
// +0x3a48 double, +0x3a50 int32 (DXF user tolerance flag), +0x3a58 double (DXF user tolerance),
// +0x3a60 SdctlPtrList<_OBJ_GEO2D> (undo copy of the 2D curves).
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class OBJ_GEO2D;
class SdctlGList;
} // namespace def::model

// The classes as p8-03 / p8-04 publish them: def::cmd::Uic2CObject (defcmd/uic2c_object.hpp),
// def::cmd::iges::Uic2CObjectIgesFile (defcmd/iges.hpp), def::cmd::Uic2CObjectDxfFile
// (defcmd/dxf.hpp, p8-04 @3b9caab; it defines the C++ ctor/dtor and calls fun_140409330).
namespace def::cmd {
class Uic2CObject;
class Uic2CObjectDxfFile;  // p8-04 (defcmd/dxf.hpp; also named def::cmd::dxf::Uic2CObjectDxfFile)
} // namespace def::cmd
namespace def::cmd::iges {
class Uic2CObjectIgesFile;
} // namespace def::cmd::iges

namespace def::cmd::gfread {

using model::LLOBJ_DATA;
using iges::Uic2CObjectIgesFile;
using def::cmd::Uic2CObjectDxfFile;

// The GFREAD loader object (FUN_1402437c0 ctor, 0x18 bytes on the stack of FUN_1401aa9a0).
struct Loader {
    double tol;       // 0x00 STL merge tolerance, DAT_140522f98 = 1.0E-5
    double scale;     // 0x08 DAT_140509c90 = 1.0
    void* callback;   // 0x10 0 from GFREAD (FUN_140244e60 returns 1 when it is 0)
};
static_assert(sizeof(Loader) == 0x18);

// ---- loader ------------------------------------------------------------------------------------
// @src 1402437c0  ctor: loader+0x10 = cb, then fun_140244e30
Loader* fun_1402437c0(Loader* self, void* cb);
// @src 140243810  dtor (empty)
void fun_140243810(Loader* self);
// @src 140244e30  tol = 1.0E-5, scale = 1.0
void fun_140244e30(Loader* self);

// ---- extension -> type -> 2D/3D dispatch -------------------------------------------------------
// @src 140243370  extension after a '.' at len-3..len-7; false when none (ext gets the text)
bool fun_140243370(const char* file, char* ext);
// @src 140243500  type code of an extension (KEY 0, DB 1, GEO 2, STL 3, PDA/PAT 4, UNV 5, IGS 6,
//                 DXF 7, NAS 10, other -1)
std::int32_t fun_140243500(const char* ext);
// @src 140243830  load `file` into obj by extension; *type gets the code; obj+0x3a40 = type on
//                 success
bool fun_140243830(Loader* self, const char* file, LLOBJ_DATA* obj, std::int32_t* type);
// @src 140244010  2D table
bool fun_140244010(Loader* self, const char* file, LLOBJ_DATA* obj, std::int32_t type);
// @src 140244200  IGES/DXF import, mode 300 (read), 400 (re-read from obj+0x3a40), other: both;
//                 true unless the reader returned -1 (false also for obj == 0)
bool fun_140244200(Loader* self, LLOBJ_DATA* obj, const char* file, std::int32_t type, std::int32_t mode);
// @src 1402442f0  mode-400 import: reader by obj+0x3a40 (6 IGES, 7 DXF) with file 0, prefix 0
//                 and mode 400; -1 for obj == 0 or another type
std::int32_t fun_1402442f0(Loader* self, LLOBJ_DATA* obj);
// @src 140244500  mode-300 import of `file` (type 6 IGES, 7 DXF; -1 otherwise); on reader result
//                 0 the reader's boundary count goes to obj+0xbc8
std::int32_t fun_140244500(Loader* self, LLOBJ_DATA* obj, const char* file, std::int32_t type);

// ---- object fields -----------------------------------------------------------------------------
// @src 140197bc0  undo copy: clear obj+0x3a60, then append a copy of every 2D curve of geometry 0
//                 (OBJ_GEO2D::assign); returns 0
std::int64_t fun_140197bc0(LLOBJ_DATA* obj);
// @src 14019a650  obj+0xbc8 = n
void fun_14019a650(LLOBJ_DATA* obj, std::int32_t n);
// @src 14019ab80  append item to the SdctlGList (-> 140331790)
void fun_14019ab80(model::SdctlGList* list, void* item);
// @src 140256670  obj+0x3a40 = type
void fun_140256670(LLOBJ_DATA* obj, std::int32_t type);
// @src 140256690  obj+0x3a40
std::int32_t fun_140256690(const LLOBJ_DATA* obj);
// @src 1404098d0  obj+0x3a48 = v (double, XMM1)
void fun_1404098d0(LLOBJ_DATA* obj, double v);
// @src 1404098f0  obj+0x3a50 (through 140409910)
std::int32_t fun_1404098f0(const LLOBJ_DATA* obj);
// @src 140409910  obj+0x3a50
std::int32_t fun_140409910(const LLOBJ_DATA* obj);
// @src 140409930  obj+0x3a58 (double, XMM0: the DXF user tolerance)
double fun_140409930(const LLOBJ_DATA* obj);
// @src 14040cde0  curve+0x18 = data (8-byte store: the 2D curve's point array)
void fun_14040cde0(model::OBJ_GEO2D* curve, double* data);

// ---- Uic2CObject family (member init / teardown, see the top comment) ---------------------------
// 140454900 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObject ctor body: +8 = 0, +0x10 = malloc(0x80) with "" , then set name
void fun_140454900(Uic2CObject* self, const char* name);
// 140454980 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObject dtor body: free(+0x10)
void fun_140454980(Uic2CObject* self);
// 140454a90 (owned by p8-03, C++ member; this is the raw-layout fallback)  set the name: null/empty -> ""; longer than 0x7f -> free + malloc(len+1); strcpy
void fun_140454a90(Uic2CObject* self, const char* name);
// 1404099d0 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObjectIgesFile ctor body: base body, +0x18/+0x1c/+0x20 = 0, 140409a50
void fun_1404099d0(Uic2CObjectIgesFile* self, const char* name);
// 140409a50 (owned by p8-03, C++ member; this is the raw-layout fallback)  +0x18 = 0, +0x1c = 0, +0x20 = 0
void fun_140409a50(Uic2CObjectIgesFile* self);
// 140409a90 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObjectIgesFile dtor body: -> fun_140454980
void fun_140409a90(Uic2CObjectIgesFile* self);
// @src 1404092b0  Uic2CObjectDxfFile ctor body: base body, +0x18/+0x1c/+0x20 = 0, 140409330
void fun_1404092b0(Uic2CObjectDxfFile* self, const char* name);
// @src 140409330  +0x18 = 0, +0x1c = 0, +0x20 = 0
void fun_140409330(Uic2CObjectDxfFile* self);
// @src 140409370  Uic2CObjectDxfFile dtor body: -> fun_140454980
void fun_140409370(Uic2CObjectDxfFile* self);
// @src 1402566b0  Uic2CObjectIgesFile +0x1c (boundary count)
std::int32_t fun_1402566b0(const Uic2CObjectIgesFile* self);
// @src 1402566c0  Uic2CObjectDxfFile +0x1c (boundary count)
std::int32_t fun_1402566c0(const Uic2CObjectDxfFile* self);
// @src 14040ce20  Uic2CObjectIgesFile +0x18 = v
void fun_14040ce20(Uic2CObjectIgesFile* self, std::int32_t v);
// @src 14040ce40  fun_14040ce20(self, 1)
void fun_14040ce40(Uic2CObjectIgesFile* self);
// @src 14040ce60  Uic2CObjectIgesFile +0x1c = v
void fun_14040ce60(Uic2CObjectIgesFile* self, std::int32_t v);

// ---- GFREAD handler ----------------------------------------------------------------------------
// @src 1401aa9a0  handler: find object *id, test the file, load, print Done/Failed
bool fun_1401aa9a0(model::DB_STEP_DATA* m, const std::int32_t* id, const char* file);

} // namespace def::cmd::gfread
