// Equipment KEY file writer (GUI twin 0x140896840 = DEF_PRE 1401c6d00) and its two unported
// callees 1402a83c0, 1403d6690. Header: defkey/item_writers.hpp. Decompile:
// deform-rea-work/out/mdt3/f.c.
#define _CRT_SECURE_NO_WARNINGS
#include <defkey/item_writers.hpp>

#include <cstdio>
#include <share.h>

#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/program_info.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "../kw_g1a/sim_ctrl_ops.hpp"

namespace def::key {

using model::field;

namespace {
double d(const model::LLOBJ_DATA* o, std::size_t off) { return field<double>(o, off); }
std::int32_t i32(const model::LLOBJ_DATA* o, std::size_t off) { return field<std::int32_t>(o, off); }
double md(const model::DB_STEP_DATA& m, std::size_t off) { return field<double>(&m, off); }
} // namespace

// @src 1402a83c0  keyword 0x97 block of an object (movement type 2)
int write_kw97_block(model::DB_STEP_DATA& m, std::FILE* f, const model::LLOBJ_DATA* obj)
{
    const std::int32_t no = i32(obj, 8);
    if (i32(obj, 0x21c8) != 2) return 3;
    if (!rt::dimension_is_2(&m)) { // 1400015e0
        if (i32(obj, 0x236c) == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", keyword_label(0x97), no, 2,
                             d(obj, 0x21b0), d(obj, 0x21b8), d(obj, 0x21c0));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %7i\n", d(obj, 0x2350), d(obj, 0x2358),
                             d(obj, 0x2348), i32(obj, 0x2360));
        } else if (i32(obj, 0x236c) == 1) {
            if (i32(obj, 0x2378) != 5) return 3;
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", keyword_label(0x97), no, 2,
                             d(obj, 0x21b0), d(obj, 0x21b8), d(obj, 0x21c0));
            rt::crt::fprintf(f, " %20.10E %7i %20.10E %7i %7i\n", d(obj, 0x2350), i32(obj, 0x2384),
                             d(obj, 0x2348), i32(obj, 0x2360), 4);
            write_table_body(f, &field<model::DYNARY>(obj, 0x2368), 0); // 1402f3130
        }
    } else {
        const double angle = g2obj::movement_angle_2d(*obj); // 14018c810(obj + 0x2160)
        if (i32(obj, 0x236c) == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i\n", keyword_label(0x97), no,
                             2, angle, d(obj, 0x2350), d(obj, 0x2358), d(obj, 0x2348), i32(obj, 0x2360));
        } else if (i32(obj, 0x236c) == 1) {
            if (i32(obj, 0x2378) != 5) return 3;
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %7i %20.10E %7i %7i\n", keyword_label(0x97),
                             no, 2, angle, d(obj, 0x2350), i32(obj, 0x2384), d(obj, 0x2348),
                             i32(obj, 0x2360), 4);
            write_table_body(f, &field<model::DYNARY>(obj, 0x2368), 0); // 1402f3130
        }
    }
    return 0;
}

namespace {
// part of @src 1403d6690  the keyword 0x2d / 0x2c line: three doubles in 3D, two in 2D
void vec_line(model::DB_STEP_DATA& m, std::FILE* f, int id, std::size_t off)
{
    if (!rt::dimension_is_2(&m)) // 1400015e0
        rt::crt::fprintf(f, "%-6s %20.10E %20.10E %20.10E\n", keyword_label(id), md(m, off), md(m, off + 8),
                         md(m, off + 0x10));
    else
        rt::crt::fprintf(f, "%-6s %20.10E %20.10E\n", keyword_label(id), md(m, off), md(m, off + 8));
}
} // namespace

// @src 1403d6690  keywords 0x2d (types 3, 4, 7..10) and 0x2c (types 7..10)
int write_kw2d_2c(model::DB_STEP_DATA& m, std::FILE* f, const model::LLOBJ_DATA* obj)
{
    if (i32(obj, 0x21c8) == 3 || i32(obj, 0x21c8) == 4) vec_line(m, f, 0x2d, 0x900);
    if (i32(obj, 0x21c8) == 7 || i32(obj, 0x21c8) == 8 || i32(obj, 0x21c8) == 9 || i32(obj, 0x21c8) == 10) {
        vec_line(m, f, 0x2d, 0x900);
        vec_line(m, f, 0x2c, 0x8e8);
    }
    return 0;
}

// @src 1401c6d00  write one object as an equipment KEY file
int write_equipment_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no)
{
    std::FILE* f = _fsopen(name, "wt", _SH_DENYNO); // 14047b934, str 1405290cc
    if (f == nullptr) return 3;
    const int status = write_equipment_key_stream(m, f, no);
    std::fclose(f);
    return status;
}

int write_equipment_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no)
{
    model::LLOBJ_DATA* obj = g1a::ops::find_in_list(m.objects, no); // 14017aaf0
    // 1401c5a20 with dim_option 0: product of the dimension (140224dd0(140158e90(m))), version
    write_file_banner(f, 0, model::product_name_for_dim(m.dimension), model::version());
    g2obj::write_movement_block(m, f, obj, 0); // 1402a7c80, status ignored
    if (0 < i32(obj, 0x2360)) {
        model::LLOBJ_DATA* other = g1a::ops::find_object(m, i32(obj, 0x2360)); // 1401d6ea0
        if (other != nullptr) write_kw97_block(m, f, other);                 // 1402a83c0, status ignored
    }
    return write_kw2d_2c(m, f, obj); // 1403d6690
}

} // namespace def::key
