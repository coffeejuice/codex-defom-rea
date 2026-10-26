// defcmd/p8_06_ext.hpp: callees of packet p8-06 owned by packets outside the p8-05..p8-08 thread.
// Each is a function pointer that defaults to the owner's function (tests may install doubles).
#pragma once

#include <cstddef>
#include <cstdio>

#include <defcmd/objpos_centre.hpp>
#include <defcmd/p67_5_gensub.hpp>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::cmd::p806_ext {

// 1401fc730 (owner p67-1, defcmd/objpos_centre.hpp): build the 3D surface geometry of an object
// from its mesh; *flag 1.
using Fn1401fc730 = void (*)(model::LLOBJ_DATA* obj, char* flag);
inline Fn1401fc730 fun_1401fc730 = &objpos::fun_1401fc730;

// 1401b83d0 (owner p67-5, defcmd/p67_5_gensub.hpp read_line, with get_line 1401b8320): read one
// line (getc up to '\n' or n - 1 characters); strlen of it, 0 at EOF with nothing read.
using Fn1401b83d0 = std::size_t (*)(char* buf, int n, std::FILE* f);
inline Fn1401b83d0 fun_1401b83d0 = &gensub::read_line;

} // namespace def::cmd::p806_ext
