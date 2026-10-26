// p8-01 seam to packet p8-02 (STL + NASTRAN, defcmd/stl_nas.hpp): the 3D GFREAD table
// FUN_140243a70. Order: test hook if set, else the owner's fun_140243a70 when its header is
// present (program build), else DEF_NOT_PORTED. Signature as published by p8-02
// (origin/claude/p8-02 @80ca824, defcmd/stl_nas.hpp): fun_140243a70(double* loader, file, obj,
// type); the loader is passed as the binary does, as its first double.
#pragma once

#include <cstdint>

#include <defcmd/gfread.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/stl_nas.hpp>)
#include <defcmd/stl_nas.hpp>
#define DEF_P801_HAVE_P8_02 1
#endif

namespace def::cmd::gfread::ext {

using Load3dHook = bool (*)(Loader* self, const char* file, model::LLOBJ_DATA* obj, std::int32_t type);
inline Load3dHook g_load_3d_hook = nullptr;

// FUN_140243a70(loader, file, obj, type): 3D table
inline bool load_3d(Loader* self, const char* file, model::LLOBJ_DATA* obj, std::int32_t type)
{
    if (g_load_3d_hook != nullptr) return g_load_3d_hook(self, file, obj, type);
#ifdef DEF_P801_HAVE_P8_02
    return def::cmd::stlnas::fun_140243a70(&self->tol, file, obj, type);
#else
    DEF_NOT_PORTED(0x140243a70);
#endif
}

} // namespace def::cmd::gfread::ext
