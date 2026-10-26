// p67-1's OBJPOS helpers that the interference code calls (include/defcmd/ifd/ifd_internal.hpp,
// namespace ext). They forward to p67-1's code (defcmd/objpos.hpp) when it is on the branch; until
// then they are DEF_NOT_PORTED. A tier-1 test may install its own through the hooks.
#include <defcmd/ifd/ifd_internal.hpp>

#include <defrt/port.hpp>

#if __has_include(<defcmd/objpos.hpp>)
#include <defcmd/objpos.hpp>
#define DEF_P672_FORWARD 1
#endif

namespace def::cmd::ifd::ext {

void (*g_translate_hook)(LLOBJ_DATA*, double*, std::int32_t*, char*) = nullptr;
void (*g_move_along_movctl_hook)(void*, double, double, double*) = nullptr;
bool (*g_usable_hook)(LLOBJ_DATA*) = nullptr;
bool (*g_has_geometry_hook)(LLOBJ_DATA*) = nullptr;
bool (*g_has_die_geometry_hook)(LLOBJ_DATA*) = nullptr;

// 14021d0b0 reads both flags as char: the binary passes the dispatcher's int 1 as the first one,
// so its low byte is what the callee sees.
void translate(LLOBJ_DATA* obj, double* v, std::int32_t* a, char* b)
{
    if (g_translate_hook != nullptr) return g_translate_hook(obj, v, a, b);
#ifdef DEF_P672_FORWARD
    const char fa = static_cast<char>(*a & 0xff);
    return objpos::translate(obj, v, &fa, b);
#else
    DEF_NOT_PORTED(0x14021d0b0);
#endif
}

void move_along_movctl(void* mv, double a, double b, double* out)
{
    if (g_move_along_movctl_hook != nullptr) return g_move_along_movctl_hook(mv, a, b, out);
#ifdef DEF_P672_FORWARD
    return objpos::move_along_movctl(mv, a, b, out);
#else
    DEF_NOT_PORTED(0x1401952e0);
#endif
}

bool usable(LLOBJ_DATA* obj)
{
    if (g_usable_hook != nullptr) return g_usable_hook(obj);
#ifdef DEF_P672_FORWARD
    return objpos::usable(obj);
#else
    DEF_NOT_PORTED(0x1401611f0);
#endif
}

bool has_geometry(LLOBJ_DATA* obj)
{
    if (g_has_geometry_hook != nullptr) return g_has_geometry_hook(obj);
#ifdef DEF_P672_FORWARD
    return objpos::has_geometry(obj);
#else
    DEF_NOT_PORTED(0x140160cc0);
#endif
}

bool has_die_geometry(LLOBJ_DATA* obj)
{
    if (g_has_die_geometry_hook != nullptr) return g_has_die_geometry_hook(obj);
#ifdef DEF_P672_FORWARD
    return objpos::has_die_geometry(obj);
#else
    DEF_NOT_PORTED(0x140203980);
#endif
}

} // namespace def::cmd::ifd::ext
