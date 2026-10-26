// Material KEY file writer (GUI twin 0x1408959e0 = DEF_PRE 1401c5ea0). Header:
// defkey/item_writers.hpp. Decompile: deform-rea-work/out/mdt3/f.c.
#define _CRT_SECURE_NO_WARNINGS
#include <defkey/item_writers.hpp>

#include <cstdio>
#include <cstring>
#include <share.h>

#include <defkey/key_writer.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key {

// @src 1401c5ea0  write one material as a material KEY file
int write_material_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no,
                            bool write_unit, const char* comment, const char* name_override)
{
    if (name == nullptr || std::strlen(name) == 0) return 1;
    model::Material* mat = model::material_list_find(&m.materials, no); // 14029c2c0
    if (mat == nullptr) return 3;
    std::FILE* f = _fsopen(name, "wt", _SH_DENYNO); // 14047b934, str 1405287d4
    if (f == nullptr) return 1;
    const int rc = write_material_key_stream(m, f, mat, write_unit, comment, name_override);
    std::fclose(f);
    return rc;
}

int write_material_key_stream(model::DB_STEP_DATA& m, std::FILE* f, model::Material* mat,
                              bool write_unit, const char* comment, const char* name_override)
{
    if (comment != nullptr) {
        std::putc('*', f);
        for (unsigned i = 0; i < std::strlen(comment); i = i + 1) {
            if (comment[i] == '\n')
                rt::crt::fprintf(f, "\n*");
            else
                std::putc(static_cast<int>(comment[i]), f);
        }
        rt::crt::fprintf(f, "\n\n");
    }
    rt::crt::fprintf(f, "*\n");
    rt::crt::fprintf(f, "*  DEFORM MATERIAL KEYWORD FILE\n");
    rt::crt::fprintf(f, "*\n");
    if (write_unit) rt::crt::fprintf(f, "%-6s %7i\n", keyword_label(0x1e), model::field<std::int32_t>(&m, 0x84c));
    int rc = g4::write_material(m, f, mat, name_override); // 1403cdca0
    if (rc != 0) return rc;
    if (mat->mstmtr_flag == 1) {
        for (std::int32_t i = 0; i < mat->mstmtr_count; i = i + 1) {
            model::Material* part = model::material_list_find(&m.materials, mat->mstmtr_list[i]);
            if (part != nullptr && (rc = g4::write_material(m, f, part, nullptr)) != 0) {
                return rc;
            }
        }
        for (std::int32_t i = 0; i < model::phase_pair_count(&m.phase_pairs); i = i + 1) { // 1401b05f0
            model::PhasePair* pair = model::phase_pair_nth(&m.phase_pairs, i + 1);       // 140339fe0
            if (pair != nullptr && model::phase_pair_in_mixture(pair, mat) &&             // 140339ba0
                (rc = g57::write_phase_pair(&m, f, pair, i + 1)) != 0) {                  // 1403cbf00
                return rc;
            }
        }
    }
    return 0;
}

} // namespace def::key
