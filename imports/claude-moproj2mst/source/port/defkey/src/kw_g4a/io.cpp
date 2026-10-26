// kw_g4a/io.hpp over the key-common defkey API (line_io, table_io) and the DYNARY template
// family; find_material forwards to p1-08's material_list_find (14029c2c0).
#include "kw_g4a/io.hpp"

#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <cstdarg>

namespace def::key::g4a::io {

int kfprintf(std::FILE* fp, const char* fmt, ...)
{
    // the VS2008 CRT float formatting (def::rt::crt), as every KEY writer
    va_list ap;
    va_start(ap, fmt);
    const int r = def::rt::crt::vfprintf(fp, fmt, ap);
    va_end(ap);
    return r;
}

int line_i(std::FILE* fp, std::int32_t* a, bool skip_blank) { return def::key::read_i(fp, a, skip_blank); }
int line_ii(std::FILE* fp, std::int32_t* a, std::int32_t* b) { return def::key::read_ii(fp, a, b); }
int line_iii(std::FILE* fp, std::int32_t* a, std::int32_t* b, std::int32_t* c) { return def::key::read_iii(fp, a, b, c); }
int line_d(std::FILE* fp, double* a) { return def::key::read_d(fp, a); }
int line_dd(std::FILE* fp, double* a, double* b) { return def::key::read_dd(fp, a, b); }
int line_d4(std::FILE* fp, double* a, double* b, double* c, double* d) { return def::key::read_d4(fp, a, b, c, d); }
int line_d5(std::FILE* fp, double* v0, double* v1, double* v2, double* v3, double* v4)
{
    double* const v[5] = {v0, v1, v2, v3, v4};
    return def::key::read_d5(fp, v);
}
int line_d7(std::FILE* fp, double* v[7]) { return def::key::read_d7(fp, v); }
int line_d9(std::FILE* fp, double* v[9]) { return def::key::read_d9(fp, v); }
int line_idd(std::FILE* fp, std::int32_t* t, double* a, double* b) { return def::key::read_i_dd(fp, t, a, b); }
int line_id5(std::FILE* fp, std::int32_t* t, double* v[5]) { return def::key::read_i_d5(fp, t, v); }

int read_body(std::FILE* fp, DYNARY* d) { return def::key::read_table_body(fp, d); }
int read_generic(std::FILE* fp, DYNARY* d, int want_extra, double* extra)
{
    return def::key::read_table_header(fp, d, want_extra, extra);
}
int write_body(std::FILE* fp, const DYNARY* d, int mode) { return def::key::write_table_body(fp, d, mode); }

void dyn_reinit_keep(DYNARY* d, int type) { def::model::dynary_reinit_keep(d, type); }
bool dyn_complete(const DYNARY* d) { return def::model::dynary_counts_positive(d); }

// copy of 14029c2c0 (MaterialList::find, p1-08): walk until node->number == number
def::model::Material* find_material(def::model::DB_STEP_DATA* md, std::int32_t number)
{
    return def::model::material_list_find(&md->materials, number);
}

} // namespace def::key::g4a::io
