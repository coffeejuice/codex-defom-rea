// def_pre/ext_p10_01.hpp: the call from menus 3-4 (packet p10-01) into p5-01's selection prompt
// FUN_140007d30, through a seam so that p10-01 builds and tests before p5-01 is merged.
//
// select() calls the hook when a test set one, else p5-01's def::pre::select_prompt
// (<def_pre/menu_shell.hpp>) when that header is in the tree, else DEF_NOT_PORTED.
#pragma once

#include <cstdint>
#include <string>

#include <def_pre/forms.hpp>
#include <defrt/port.hpp>

#if __has_include(<def_pre/menu_shell.hpp>)
#include <def_pre/menu_shell.hpp>
#define DEF_PRE_HAS_SELECT_PROMPT 1
#endif

namespace def::pre::ext {

// 140007d30(rec, out, use_args, lo, hi): " Enter Selection (%d-%d, R, E, /): " until an answer is
// accepted; the range is rec->lo..rec->hi unless use_args.
using SelectFn = std::string (*)(MenuRec* rec, bool use_args, std::int32_t lo, std::int32_t hi);

struct Hooks {
    SelectFn select = nullptr;
};
inline Hooks g_hooks;

inline std::string select(MenuRec* rec, bool use_args, std::int32_t lo, std::int32_t hi)
{
    if (g_hooks.select != nullptr) {
        return g_hooks.select(rec, use_args, lo, hi);
    }
#if defined(DEF_PRE_HAS_SELECT_PROMPT)
    return def::pre::select_prompt(&rec->lo, use_args, lo, hi);
#else
    DEF_NOT_PORTED(0x140007d30);
#endif
}

} // namespace def::pre::ext
