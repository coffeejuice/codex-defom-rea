// kw-g2bc internals: offset access, dimension tests, object lookup, and the per-family entry
// points the dispatcher (dispatch.cpp) routes to. One source file per family:
//
//   ec.cpp      edge/face BCs   EC*                    (cases 0x61..0x6c, FUN_1402abf20, handlers)
//   win.cpp     window BCs      WINPRS WINTMP WINATM WINRSE (cases 0x52..0x55, steps 48-52, 1402ab880)
//   nodal.cpp   nodal BCs       BCCDEF BCCDFN BCCANG BCCTMP BCCTFN BCCCRB BCCRHT CRBFLX RHTFLX HDNOBJ
//   local.cpp   BCCFNC, LOC*, RSEDEF (cases 0x11, 0x56..0x58, 0x9e; 1402ac560, 1402b7140, handlers)
//
// LLOBJ_DATA keeps most sub-structures opaque (defmodel/layout/llobj_data.hpp), so fields are
// addressed by the offsets of the specs, exactly like the decompile: at<T>(obj, 0x998) is
// *(T*)(obj + 0x998).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include <defkey/kw_g2bc.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::key::g2bc {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::LLOBJ_DATA;

template <class T, class C>
inline T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&base) + off);
}
template <class T, class C>
inline const T& at(const C& base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&base) + off);
}

// FUN_1400015e0 / FUN_1400015f0: model+0x1c == 2 / == 3.
inline bool is_2d(const DB_STEP_DATA& m) { return at<std::int32_t>(m, 0x1c) == 2; }
inline bool is_3d(const DB_STEP_DATA& m) { return at<std::int32_t>(m, 0x1c) == 3; }
// The object's model (+0x3a98).
inline DB_STEP_DATA& owner_of(LLOBJ_DATA& obj) { return *at<DB_STEP_DATA*>(obj, 0x3a98); }

// The object number a writer prints: obj+0x3f24 when > 0 (set by FUN_14029e250's outNo), else
// obj+8.
inline std::int32_t printed_number(const LLOBJ_DATA& obj)
{
    std::int32_t r = at<std::int32_t>(obj, 0x3f24);
    return r > 0 ? r : at<std::int32_t>(obj, 8);
}

// FUN_14017aaf0(model+0x1380, no): object by number, or null (kw-g2obj's port).
LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t no);

// ---- families ---------------------------------------------------------------------------------
// Each family: handles/read (parser cases) and handles_write/write_keyword (registry column 2).

namespace ec {
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref);
}  // namespace ec

namespace win {
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
}  // namespace win

namespace nodal {
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref);
}  // namespace nodal

namespace local {
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref);
}  // namespace local

}  // namespace def::key::g2bc
