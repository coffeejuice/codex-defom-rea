// defmodel/layout/layout_check.hpp: compile-time checks that a recovered class keeps the binary's
// layout (docs/port-conventions.md section 4, docs/re/model-layout.md).
//
//     static_assert(sizeof(DB_STEP_DATA) == 0x3408);
//     DEF_LAYOUT_OFFSET(DB_STEP_DATA, u1b60, 0x1b60);
//
// offsetof on a class that is not standard-layout (virtual functions, base classes) is
// "conditionally supported". MSVC, clang and gcc all accept it for single and non-virtual multiple
// inheritance, which is all the model uses; the warnings they give for it are silenced here.
#pragma once

#include <cstddef>

#if defined(_MSC_VER) && !defined(__clang__)
// C4644: macro-based offsetof pattern in constant expressions is non-standard.
#define DEF_LAYOUT_OFFSET(T, member, off)                                                            \
    __pragma(warning(suppress : 4644)) static_assert(offsetof(T, member) == (off),                  \
                                                     #T "::" #member " is not at " #off)
#else
#define DEF_LAYOUT_OFFSET(T, member, off)                                                            \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")         \
    static_assert(offsetof(T, member) == (off), #T "::" #member " is not at " #off)               \
    _Pragma("GCC diagnostic pop")
#endif
