// p8-01 seam to packet p8-04 (DXF, defcmd/dxf.hpp): the DXF reader FUN_1404093a0 and the
// Uic2CObjectDxfFile stack object. Order: test hook, else the owner's code when its header is
// present, else DEF_NOT_PORTED. Signature as published by p8-04 (origin/claude/p8-04 @96fca1f, defcmd/dxf.hpp),
// matching the call at 1402446e4: RCX this, RDX file, R8 prefix, R9 obj, [rsp+20] int 2, [28] int
// mode; EAX result. The class def::cmd::Uic2CObjectDxfFile (p8-04 @3b9caab) has the ctor
// Uic2CObjectDxfFile(const char* name = nullptr) (1404092b0).
#pragma once

#include <cstdint>
#include <new>

#include <defcmd/gfread.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/dxf.hpp>)
#include <defcmd/dxf.hpp>
#define DEF_P801_HAVE_P8_04 1
#endif

namespace def::cmd::gfread::ext {

using DxfReadHook = std::int32_t (*)(Uic2CObjectDxfFile* self, const char* file, const char* prefix,
                                     model::LLOBJ_DATA* obj, std::int32_t a, std::int32_t mode);
inline DxfReadHook g_dxf_read_hook = nullptr;

inline std::int32_t dxf_read(Uic2CObjectDxfFile* self, const char* file, const char* prefix, model::LLOBJ_DATA* obj,
                             std::int32_t a, std::int32_t mode)
{
    if (g_dxf_read_hook != nullptr) return g_dxf_read_hook(self, file, prefix, obj, a, mode);
#ifdef DEF_P801_HAVE_P8_04
    return def::cmd::dxf::fun_1404093a0(self, file, prefix, obj, a, mode);
#else
    DEF_NOT_PORTED(0x1404093a0);
#endif
}

class DxfFileOnStack {
public:
    DxfFileOnStack()
    {
#ifdef DEF_P801_HAVE_P8_04
        p_ = new (buf_) Uic2CObjectDxfFile(nullptr);
#else
        p_ = reinterpret_cast<Uic2CObjectDxfFile*>(buf_);
        *reinterpret_cast<void**>(buf_) = nullptr;
        fun_1404092b0(p_, nullptr);
#endif
    }
    ~DxfFileOnStack()
    {
#ifdef DEF_P801_HAVE_P8_04
        p_->~Uic2CObjectDxfFile();
#else
        fun_140409370(p_);
#endif
    }
    DxfFileOnStack(const DxfFileOnStack&) = delete;
    DxfFileOnStack& operator=(const DxfFileOnStack&) = delete;
    Uic2CObjectDxfFile* get() { return p_; }

private:
    alignas(16) unsigned char buf_[0x80];
    Uic2CObjectDxfFile* p_ = nullptr;
};

} // namespace def::cmd::gfread::ext
