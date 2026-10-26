// gendb3d/functions.cpp: global function table helpers of the GENDB writers (packet p4-05).
#include <defdbio/gendb3d/obj3d.hpp>

#include <cstdio>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::dbio::g3d {

namespace {
// FUN_14047903c() + 0x30: the CRT's second FILE (stdout) of the VS2010 runtime (FILE is 0x30 bytes)
std::FILE* out_stream() { return stdout; }
} // namespace

// @src 1401bb3d0  allocate the next global function: count m+0x10c, IIFNC pair (id, LLFNC start)
void alloc_function(DB_STEP_DATA* m, const std::int32_t* len, std::int32_t* iifnc,
                    const std::int32_t* id, double* number, std::int32_t* pos)
{
    m->u10c = m->u10c + 1;
    *number = static_cast<double>(m->u10c);
    iifnc[m->u10c * 2 - 1] = *id;
    if (m->u10c < 2) {
        iifnc[0] = 1;
    }
    iifnc[m->u10c * 2] = iifnc[m->u10c * 2 - 2] + *len;
    *pos = iifnc[m->u10c * 2 - 2];
    const std::int32_t nfn = m->u074[(0x88 - 0x74) / 4];    // +0x88 functions allocated
    const std::int32_t llfnc = m->u074[(0x8c - 0x74) / 4];  // +0x8c LLFNC length
    if (nfn < m->u10c) {
        // str 140527418
        rt::crt::fprintf(out_stream(), "WARNING: FNCLOC Func # %d exceeds allocated number! %d\n",
                         m->u10c, nfn);
    }
    if (llfnc < iifnc[m->u10c * 2] - 1) {
        // str 140527450
        rt::crt::fprintf(out_stream(), "WARNING: FNCLOC address exceeds allocated size!\n");
    }
}

// @src 140131210  FUNCTION_DATA::WriteFuncToDBArray: only type 0x7c; ncomp (+0x40) values, or
//                 map[(int)value - 1] when map is given
void write_func_to_db_array(const DYNARY* f, double* out, const std::int32_t* map)
{
    if (f->type == 0x7c) {
        if (map == nullptr) {
            for (std::int32_t i = 0; i < f->ncomp; i = i + 1) {
                out[i] = f->values[i];
            }
        }
        else {
            for (std::int32_t i = 0; i < f->ncomp; i = i + 1) {
                out[i] = static_cast<double>(map[static_cast<std::int32_t>(f->values[i]) - 1]);
            }
        }
    }
    else {
        // str 14051b878
        rt::crt::fprintf(out_stream(), "FUNCTION_DATA::WriteFuncToDBArray - Wrong function call\n");
    }
}

// @src 140131360  DYNARY to the LLFNC block: ndim 0 (listed types only) the ncomp values; ndim 1
//                 rows (x, v[ncomp]); otherwise the axes one after the other, then all values
void write_dynary_to_db_array(const DYNARY* f, double* out)
{
    if (out == nullptr) return;
    if (f->ndim == 0) {
        if (f->type != 0xa7 && f->type != 0xa8 && f->type != 0xaa && f->type != 0xab &&
            f->type != 0xad && f->type != 0xb3 && f->type != 0xb5) {
            return;
        }
        for (std::int32_t i = 0; i < f->ncomp; i = i + 1) {
            out[i] = f->values[i];
        }
    }
    if (f->ndim == 1) {
        for (std::int32_t r = 0; r < f->n[0]; r = r + 1) {
            out[r * (f->ncomp + 1)] = f->axis[0][r];
            for (std::int32_t i = 0; i < f->ncomp; i = i + 1) {
                out[r * (f->ncomp + 1) + 1 + i] = f->values[r * f->ncomp + i];
            }
        }
    }
    else {
        // ndim 0 also arrives here (the constant's values are written again by the memcpy)
        std::int32_t k = 0;
        for (std::int32_t d = 0; d < f->ndim; d = d + 1) {
            for (std::int32_t r = 0; r < f->n[d]; r = r + 1) {
                out[k + r] = f->axis[d][r];
            }
            k = k + f->n[d];
        }
        const std::int32_t nv = model::dynary_value_count(f);
        std::memcpy(out + k, f->values, static_cast<std::size_t>(static_cast<std::int64_t>(nv) << 3));
    }
}

// @src 1401317c0  rows (x = axis[0][r], v[ncomp]) for r < n[0]
void write_table_1d_to_db_array(const DYNARY* f, double* out)
{
    for (std::int32_t r = 0; r < f->n[0]; r = r + 1) {
        out[r * (f->ncomp + 1)] = f->axis[0][r];
        for (std::int32_t i = 0; i < f->ncomp; i = i + 1) {
            out[r * (f->ncomp + 1) + 1 + i] = f->values[r * f->ncomp + i];
        }
    }
}

// @src 14029ba10  table[n] := 0, mark the numbers of materials whose +4 is set (1401b0620), then
//                 number the marked entries 1, 2, ...; returns the count
std::int32_t material_positions(void* materials, std::int32_t* table, const std::int32_t* n)
{
    auto* list = static_cast<model::MaterialList*>(materials);
    for (std::int32_t i = 0; i < *n; i = i + 1) {
        table[i] = 0;
    }
    for (std::int32_t i = 0; i < list->count; i = i + 1) { // 140199c50: list count (+0x10)
        auto* mat = reinterpret_cast<std::int32_t*>(model::material_list_nth(list, i + 1));
        if (mat != nullptr) {
            if (mat[1] != 0) { // 1401b0620: *(int *)(mat + 4) != 0
                table[mat[0] - 1] = 1;
            }
        }
    }
    std::int32_t used = 0;
    for (std::int32_t i = 0; i < *n; i = i + 1) {
        if (table[i] != 0) {
            used = used + 1;
            table[i] = used;
        }
    }
    return used;
}

} // namespace def::dbio::g3d
