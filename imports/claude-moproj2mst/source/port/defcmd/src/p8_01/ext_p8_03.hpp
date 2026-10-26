// p8-01 seam to packet p8-03 (IGES, defcmd/iges.hpp + defcmd/uic2c_object.hpp): the IGES reader
// FUN_140409af0 and the Uic2CObjectIgesFile object that FUN_140244500 / FUN_1402442f0 build on
// the stack. Order: test hook, else the owner's code when its headers are present, else
// DEF_NOT_PORTED. Signatures as published by p8-03 (origin/claude/p8-03 @103ec6f, defcmd/iges.hpp),
// matching the call at 14024465e: RCX this, RDX file, R8 prefix, R9 obj, [rsp+20] double*
// (-> 0.0), [28] int 2, [30] byte 0, [38] int 1, [40] int mode (300 / 400); EAX result. The class
// iges::Uic2CObjectIgesFile has the ctor Uic2CObjectIgesFile(const char* name) (1404099d0).
#pragma once

#include <cstdint>
#include <new>

#include <defcmd/gfread.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/iges.hpp>) && __has_include(<defcmd/uic2c_object.hpp>)
#include <defcmd/iges.hpp>
#include <defcmd/uic2c_object.hpp>
#define DEF_P801_HAVE_P8_03 1
#endif

namespace def::cmd::gfread::ext {

using IgesReadHook = std::int32_t (*)(Uic2CObjectIgesFile* self, const char* file, const char* prefix,
                                      model::LLOBJ_DATA* obj, double* x, std::int32_t a, char b, std::int32_t c,
                                      std::int32_t mode);
inline IgesReadHook g_iges_read_hook = nullptr;

inline std::int32_t iges_read(Uic2CObjectIgesFile* self, const char* file, const char* prefix, model::LLOBJ_DATA* obj,
                              double* x, std::int32_t a, char b, std::int32_t c, std::int32_t mode)
{
    if (g_iges_read_hook != nullptr) return g_iges_read_hook(self, file, prefix, obj, x, a, b, c, mode);
#ifdef DEF_P801_HAVE_P8_03
    return def::cmd::iges::fun_140409af0(self, file, prefix, obj, x, a, b, c, mode);
#else
    DEF_NOT_PORTED(0x140409af0);
#endif
}

// The stack object (0x28 bytes in the original). With p8-03's class: its ctor and dtor. Without
// it: raw storage initialised by fun_1404099d0 (no vptr; only a test hook can receive it).
class IgesFileOnStack {
public:
    IgesFileOnStack()
    {
#ifdef DEF_P801_HAVE_P8_03
        p_ = new (buf_) def::cmd::iges::Uic2CObjectIgesFile(nullptr);
#else
        p_ = reinterpret_cast<Uic2CObjectIgesFile*>(buf_);
        *reinterpret_cast<void**>(buf_) = nullptr;
        fun_1404099d0(p_, nullptr);
#endif
    }
    ~IgesFileOnStack()
    {
#ifdef DEF_P801_HAVE_P8_03
        p_->~Uic2CObjectIgesFile();
#else
        fun_140409a90(p_);
#endif
    }
    IgesFileOnStack(const IgesFileOnStack&) = delete;
    IgesFileOnStack& operator=(const IgesFileOnStack&) = delete;
    Uic2CObjectIgesFile* get() { return p_; }

private:
    alignas(16) unsigned char buf_[0x80];
    Uic2CObjectIgesFile* p_ = nullptr;
};

} // namespace def::cmd::gfread::ext
