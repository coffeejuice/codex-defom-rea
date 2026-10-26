// Step-header cache of the DB info object (packet p67-5, for TCYCLE). See db_header_cache.hpp.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // the original uses the plain CRT string calls
#endif
#include <defdbio/step/db_header_cache.hpp>
#include <defmodel/hdrstats/object_stats.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <emmintrin.h>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/resultset/db_result_sets.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defdbio/step/globals_p67_5.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/port.hpp>

namespace def::dbio::step {

using def::model::DbDirRecord;
using def::model::DbInfoEntryE0;

namespace {

// CVTTSD2SI: truncation, 0x80000000 for NaN and out-of-range values
std::int32_t cvtt(double v) { return _mm_cvttsd_si32(_mm_set_sd(v)); }

// @data 1405d4ac0  2D min/max pair table (pair starts into the 400-double object block). Only
// entries 0 and 1 are read by the ported code; the table is listed up to its first 0.
constexpr std::int32_t kMinMax2d[] = {6, 8, 10, 12, 14, 16, 18, 24, 26, 28, 30, 32, 44, 46, 48, 50, 56, 59, 61, 0};
// @data 1405d54c0  3D min/max pair table, same convention
constexpr std::int32_t kMinMax3d[] = {12, 14, 16, 18, 20, 22, 25, 27, 29, 31, 33, 35, 37, 39,
                                      41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67,
                                      69, 71, 73, 75, 77, 79, 81, 83, 85, 98, 100, 102, 104, 0};

// FUN_1401c7f80 values, word i at 0x1406e8524 + 4*i (the words at 0x1406e85d0 / 0x1406e85d4 are
// written after the branch, 0 and 1, for both dimensions)
constexpr std::int32_t k2d[kHdrIndexWords] = {
    8, 34, 2, 3, 0, 42, 43, 6,  // 1406e8524
    7, 8, 9, 10, 11, 109, 110, 12,  // 1406e8544
    13, 18, 19, 14, 15, 16, 17, 24,  // 1406e8564
    25, 26, 27, 28, 29, 30, 31, 32,  // 1406e8584
    33, 44, 45, 46, 47, 48, 49, 50,  // 1406e85a4
    51, 20, 0, /*5d0*/0, /*5d4*/1, 0, 4, 5,  // 1406e85c4
    0, 21, 22, 23, 52, 53, 54, 55,  // 1406e85e4
    58, 56, 57, 0, 61, 62, 59, 60,  // 1406e8604
    115, 116, 117, 118, 76, 88, 64, 82,  // 1406e8624
    94, 70, 78, 90, 66, 84, 96, 72,  // 1406e8644
    80, 92, 68, 86, 98, 74, 75, 87,  // 1406e8664
    63, 81, 93, 69, 77, 89, 65, 83,  // 1406e8684
    95, 71, 79, 91, 67, 85, 97, 73,  // 1406e86a4
    99, 102, 103, 104, 105, 106, 107, 108,  // 1406e86c4
    108, 112, 111, 114, 113, 0, 0, 100,  // 1406e86e4
    101, 119,  // 1406e8704
};
constexpr std::int32_t k3d[kHdrIndexWords] = {
    5, 106, 3, 4, 5, 88, 89, 12,  // 1406e8524
    13, 31, 32, 14, 15, 125, 126, 16,  // 1406e8544
    17, 22, 23, 18, 19, 20, 21, 79,  // 1406e8564
    80, 81, 82, 83, 84, 85, 86, 33,  // 1406e8584
    34, 98, 99, 100, 101, 102, 103, 104,  // 1406e85a4
    105, 24, 115, /*5d0*/0, /*5d4*/1, 2, 6, 7,  // 1406e85c4
    8, 11, 9, 10, 0, 0, 0, 0,  // 1406e85e4
    0, 0, 0, 127, 37, 38, 35, 36,  // 1406e8604
    51, 52, 65, 66, 54, 40, 68, 60,  // 1406e8624
    46, 74, 56, 42, 70, 62, 48, 76,  // 1406e8644
    58, 44, 72, 64, 50, 78, 53, 39,  // 1406e8664
    67, 59, 45, 73, 55, 41, 69, 61,  // 1406e8684
    47, 75, 57, 43, 71, 63, 49, 77,  // 1406e86a4
    114, 117, 118, 119, 120, 121, 122, 123,  // 1406e86c4
    124, 26, 25, 28, 27, 30, 29, 116,  // 1406e86e4
    0, 128,  // 1406e8704
};

unsigned char* bytes(DbInfoEntryE0& e) { return reinterpret_cast<unsigned char*>(&e); }

const DbDirRecord& dir(const Cls_1401dbfe0& db, std::int32_t i) { return def::model::db_info_records(db)[i]; }

// FUN_140323bf0 (C string to a blank-padded Fortran field, returns strlen)
std::int32_t to_field(char* dst, const char* src, std::int32_t n)
{
    return static_cast<std::int32_t>(rt::to_fortran(dst, src, n));
}

} // namespace

void reset_globals_p67_5()
{
    for (auto& w : dat_1406e8524) w = 0;
    model::p407::g_hdr_label_table = model::p407::HdrLabelTable::none;
    dat_140707f84 = 0;
    dat_140707f90 = 0;
    dat_140707fc0 = 0.0;
    dat_140706e20 = 0.0;
    dat_140706e30 = 0;
    dat_140706e60 = 0.0;
    std::memset(last_open_name(), 0, 200);
}

// ---- records ------------------------------------------------------------------------------------

// @src 1401d9150  per-object record ctor body
// @src 1401d9120  (ctor thunk, calls 1401d9150 and returns this)
void DbObjHeader::init()
{
    for (std::int32_t i = 0; i < 400; ++i) {
        v[i] = 0.0;
    }
    name[0] = 0;
}

std::int32_t& entry_i32(DbInfoEntryE0& e, std::size_t off) { return *reinterpret_cast<std::int32_t*>(bytes(e) + off); }
double& entry_f64(DbInfoEntryE0& e, std::size_t off) { return *reinterpret_cast<double*>(bytes(e) + off); }
char* entry_chars(DbInfoEntryE0& e, std::size_t off) { return reinterpret_cast<char*>(bytes(e) + off); }
DbObjHeader*& entry_objects(DbInfoEntryE0& e) { return *reinterpret_cast<DbObjHeader**>(bytes(e) + 0x20); }
DbInfoEntryE0* cache_entries(Cls_1401dbfe0& db) { return static_cast<DbInfoEntryE0*>(db.uae8); }

// @src 1401d9210  cache record init
// @src 1401d91a0  (element ctor thunk, calls 1401d9210 and returns this)
void init_cache_entry(DbInfoEntryE0& e)
{
    entry_i32(e, 0x00) = 0;
    entry_i32(e, 0x04) = 0;
    entry_i32(e, 0x1c) = 0;
    entry_f64(e, 0x08) = 0.0;
    entry_f64(e, 0x10) = 0.0;
    entry_f64(e, 0x28) = 0.0;
    entry_objects(e) = nullptr;
    entry_i32(e, 0x30) = 0;
    entry_i32(e, 0x34) = 0;
    entry_i32(e, 0x38) = 0;
    entry_i32(e, 0x3c) = 0;
    std::strcpy(entry_chars(e, 0x4c), "OPERATION 1");  // FUN_140484f90 = strcpy
    std::strcpy(entry_chars(e, 0x8d), "SIMULATION 1");
    entry_i32(e, 0x18) = 0;
}

// 1401c7f80 (p4-07's port, def::model::p407::load_hdr_slots): the step-header layout table and
// the min/max pair table for the dimension
void set_header_layout(const std::int32_t* dim) { model::p407::load_hdr_slots(dim); }

// DAT_1406e8710: the rows of the min/max pair table 1401c7f80 selected (null before it ran)
const std::int32_t* header_minmax_table()
{
    switch (model::p407::g_hdr_label_table) {
    case model::p407::HdrLabelTable::table_1405d4ac0: return kMinMax2d;
    case model::p407::HdrLabelTable::table_1405d54c0: return kMinMax3d;
    default: return nullptr;
    }
}

// ---- directory queries --------------------------------------------------------------------------

// @src 1401dde10  SimNo of entry idx
std::int32_t entry_sim_no(const Cls_1401dbfe0& db, std::int32_t idx)
{
    if (db.uad8 == nullptr) return 0;
    if (idx < 0 || db.uac8 <= idx) return 0;
    return dir(db, idx).sim_no;
}

// @src 1401ddb40  dimension of entry idx
std::int32_t entry_dim(const Cls_1401dbfe0& db, std::int32_t idx)
{
    if (db.uad8 == nullptr) return 3;
    if (idx < 0 || db.uac8 <= idx) return 3;
    return dir(db, idx).dim;
}

// @src 1401ddba0  version of entry idx (no lower bound check, as in the original)
std::int32_t entry_version(const Cls_1401dbfe0& db, std::int32_t idx)
{
    if (db.uad8 == nullptr || db.uac8 < 1) return 10000;
    if (idx < db.uac8) return dir(db, idx).version;
    return 10000;
}

// @src 1401e5de0  part holding entry idx
std::int32_t entry_part(const Cls_1401dbfe0& db, std::int32_t idx)
{
    if (1 < db.u008) {
        for (std::int32_t i = 0; i < db.u008; ++i) {
            if (db.u00c[i].a <= idx && idx <= db.u00c[i].b) return i;
        }
    }
    return 0;
}

// @src 1401e5eb0  first entry of a part
std::int32_t part_first_entry(const Cls_1401dbfe0& db, std::int32_t part)
{
    if (db.u008 < 2 || part < 0 || db.u008 <= part) return 0;
    return db.u00c[part].a;
}

// @src 1401e5d60  file name of a part
void part_file_name(const Cls_1401dbfe0& db, char* buf, std::int32_t part)
{
    if (db.u008 < 2 || part < 0 || db.u008 <= part) {
        std::strcpy(buf, db.u650);
    } else {
        std::strcpy(buf, static_cast<const char*>(db.u330[part]));
    }
}

// @src 1401e1de0  object block layout of entry *idx
void object_block_layout(Cls_1401dbfe0& db, const std::int32_t* idx, std::int32_t* first, std::int32_t* stride)
{
    const std::int32_t dim = entry_dim(db, *idx);
    const std::int32_t ver = entry_version(db, *idx);
    if (dim == 2) {
        *first = 0x22;
        *stride = 100;
    } else if (ver < 3000) {
        *first = 8;
        *stride = 100;
    } else {
        *first = 0x22;
        *stride = 200;
    }
    if (10999 < ver) {
        DbInfoEntryE0& e = cache_entries(db)[*idx];
        if (0 < entry_i32(e, 0x30)) *first = entry_i32(e, 0x30);
        if (0 < entry_i32(e, 0x34)) *stride = entry_i32(e, 0x34);
    }
}

// ---- datarm23 front ends ------------------------------------------------------------------------

char* last_open_name()
{
    // DAT_140b6ca20: 200 characters in the gap of p1-12's DbCommon at offset 0x26ea0
    return reinterpret_cast<char*>(&dbcore::g_db) + (0x140b6ca20 - 0x140b45b80);
}

// @src 1403adc74  open the DB on a unit, op 0 when the file changed
void open_db_unit(const char* name, const std::int32_t* /*name_len*/, std::int32_t* unit, std::int32_t* err,
                  const std::int32_t* print)
{
    if (*print == 1) {
        // WRITE 'OPENING DATABASE FILE = ' (and the error echo below): no caller passes 1
        DEF_NOT_PORTED(0x1403adc74);
    }
    if (*unit < 1 || 0x31 < *unit) {
        *unit = 3;
    }
    // OPEN mode 5 (direct, read-only), STATUS 'OLD' (str 140bafe70, 3 characters passed), the
    // name with the dummy's length 200 (1403add27)
    const std::int32_t mode = 5;
    dbcore::fortran_open(&mode, unit, name, "OLD", err, 200, 3);
    if (*err != 0) {
        *err = 99;
    }
    // FUN_1404b4108: Fortran compare of the 200-character name with DAT_140b6ca20 (equal
    // lengths: a byte compare); nonzero = different
    if (std::memcmp(name, last_open_name(), 200) != 0) {
        std::int32_t op = 0;
        std::int32_t slot = 0, step = 0, nh = 0, nmar = 0, na = 0;
        dbcore::db_operation(unit, &op, &slot, &step, &nh, &dat_140706e20, &nmar, &dat_140706e30, &na,
                             &dat_140706e60, err);
        std::memcpy(last_open_name(), name, 200); // FUN_1404b33e4, 200 <- 200
    }
}

// @src 1403daf3e  op 2: step header of a directory slot
void read_step_header(const std::int32_t* unit, std::int32_t* slot, std::int32_t* nh, double* h, std::int32_t* err,
                      const std::int32_t* print)
{
    std::int32_t op = 2;
    std::int32_t nmar = 0;
    std::int32_t na = 0;
    dbcore::db_operation(unit, &op, slot, &dat_140707f84, nh, h, &nmar, &dat_140707f90, &na, &dat_140707fc0, err);
    if (*print == 1) {
        // 'STEP HEADER INFORMATION', SIMNAM, SwapFlag, MAXH, NUMOBJ, PDIE, TNOW, TEND or
        // 'ERROR IN READING STEP HEADER =', then 'END OF READING DATABASE STEP HEADER': no
        // caller passes 1
        DEF_NOT_PORTED(0x1403daf3e);
    }
}

// @src 1403ae06f  CLOSE(unit) (FUN_1404b53a4 without STATUS)
void close_db_unit(const std::int32_t* unit)
{
    const std::int32_t mode = 0;
    std::int32_t iostat = 0;
    dbcore::fortran_open(&mode, unit, "", "", &iostat, 0, 0);
}

// ---- the cache ------------------------------------------------------------------------------------

// @src 1401dfb80  one entry's step header into the cache
void read_entry_header(Cls_1401dbfe0& db, std::int32_t* idx, double* h, std::int32_t* part, bool close_old)
{
    std::int32_t dim = entry_dim(db, *idx);  // local_88
    const bool is2d = dim == 2;              // local_78
    const bool is3d = dim == 3;              // local_4b8
    set_header_layout(&dim);
    std::int32_t err = 0;   // local_74[0]
    std::int32_t print = 0; // local_8d4
    std::int32_t unit = 3;  // local_74[1]
    // local_8cc = db+0xabc: stored, never read
    std::int32_t nh = dir(db, *idx).hsize;                      // local_49c
    const std::int32_t p = entry_part(db, *idx);                // local_4a0
    std::int32_t slot = (*idx - part_first_entry(db, p)) + 1;   // local_4b4
    if (p != *part) {
        if (close_old && -1 < *part) {
            close_db_unit(&unit);
        }
        *part = p;
        char fname[0x400];   // local_488
        char field[0x400];   // local_8b8
        part_file_name(db, fname, p);
        std::int32_t len = to_field(field, fname, 0x400);
        open_db_unit(field, &len, &unit, &err, &print);
        if (err != 0) {
            return;
        }
    }
    read_step_header(&unit, &slot, &nh, h, &err, &print);
    if (err != 0) {
        return;
    }
    DbInfoEntryE0& e = cache_entries(db)[*idx];
    entry_i32(e, 0x00) = cvtt(h[0]);
    entry_i32(e, 0x04) = cvtt(h[1]);
    entry_f64(e, 0x08) = h[2];
    entry_f64(e, 0x10) = h[3];
    entry_i32(e, 0x1c) = cvtt(h[hdr_index(0x1406e8524)]);
    entry_i32(e, 0x30) = cvtt(h[12]);
    entry_i32(e, 0x34) = cvtt(h[9]);
    entry_i32(e, 0x38) = cvtt(h[10]);
    entry_i32(e, 0x3c) = cvtt(h[11]);
    std::strncpy(entry_chars(e, 0x4c), reinterpret_cast<const char*>(h + 0x10), 0x40);
    rt::trim_fortran(entry_chars(e, 0x4c), 0x40);
    std::strncpy(entry_chars(e, 0x8d), reinterpret_cast<const char*>(h + 0x18), 0x50);
    rt::trim_fortran(entry_chars(e, 0x8d), 0x50);
    if (entry_i32(e, 0x38) == 0) {
        entry_i32(e, 0x38) = dir(db, *idx).sim_no;
        std::strncpy(entry_chars(e, 0x4c), entry_chars(e, 0x8d), 0x40);
        rt::trim_fortran(entry_chars(e, 0x4c), 0x40);
    }
    if (is3d) {
        entry_i32(e, 0x18) = cvtt(h[8]);
    }
    // operator new (thunk 1404797d8, malloc based) + vector_constructor_iterator(1401d9120); the
    // record dtor 1401d91d0 releases it with free(). An earlier array is overwritten, not freed.
    const std::int32_t nobj = entry_i32(e, 0x00);
    const std::size_t n = static_cast<std::size_t>(static_cast<std::int64_t>(nobj));
    auto* objs = static_cast<DbObjHeader*>(std::malloc(n * sizeof(DbObjHeader)));
    if (objs != nullptr) {
        for (std::size_t k = 0; k < n; ++k) objs[k].init();
    }
    entry_objects(e) = objs;
    std::int32_t first = 0;  // local_80
    std::int32_t stride = 0; // local_7c
    object_block_layout(db, idx, &first, &stride);
    bool first_obj = true;    // local_498
    const std::int32_t npairs = 2; // local_84
    double* tab = reinterpret_cast<double*>(db.uaf0);
    for (std::int32_t i = 0; i < entry_i32(e, 0x00); ++i) {
        DbObjHeader& o = entry_objects(e)[i];
        const double* blk = h + (first + stride * i);                 // local_4a8
        const double* nm = blk + hdr_index(0x1406e8528);               // local_68
        std::strncpy(o.name, reinterpret_cast<const char*>(nm), 0x40);
        rt::trim_fortran(o.name, 0x40);
        const std::int32_t cnt = stride < 400 ? stride : 400;
        std::memcpy(o.v, blk, static_cast<std::size_t>(static_cast<std::int64_t>(cnt)) << 3);
        if (first_obj) {
            for (std::int32_t k = 0; k < npairs; ++k) {
                const std::int32_t j = header_minmax_table()[k];
                tab[j] = o.v[j];
                tab[j + 1] = o.v[j + 1];
            }
            first_obj = false;
        } else {
            for (std::int32_t k = 0; k < npairs; ++k) {
                const std::int32_t j = header_minmax_table()[k];
                // 1401e0691: COMISD obj, tab; JBE -> obj (also for NaN)
                tab[j] = (o.v[j] > tab[j]) ? tab[j] : o.v[j];
                // 1401e07c3: COMISD tab, obj; JBE -> obj
                tab[j + 1] = (tab[j + 1] > o.v[j + 1]) ? tab[j + 1] : o.v[j + 1];
            }
        }
        if (i == entry_i32(e, 0x04) - 1) {
            const double x = blk[hdr_index(0x1406e852c)];
            const double y = blk[hdr_index(0x1406e8530)];
            if (!is2d) {
                const double z = blk[hdr_index(0x1406e8534)];
                entry_f64(e, 0x28) = std::sqrt((x * x + y * y) + z * z); // FUN_140484644 = sqrt
            } else {
                entry_f64(e, 0x28) = std::sqrt(x * x + y * y);
            }
        }
    }
}

// @src 1401dd250  header cache of all entries, once
void load_header_cache(Cls_1401dbfe0& db, bool flag)
{
    (void)flag; // passed to 1401dfb80 as a sixth argument it never reads
    if (0 < db.uac8 && db.u1900 == 0) {
        if (db.uae8 == nullptr) {
            // operator new[] with the count cookie + eh_vector_constructor_iterator(1401d91a0,
            // dtor 1401d91d0); released by delete_entries_e0 (delete[])
            const std::size_t n = static_cast<std::size_t>(static_cast<std::int64_t>(db.uac8));
            auto* arr = new DbInfoEntryE0[n];
            for (std::size_t k = 0; k < n; ++k) init_cache_entry(arr[k]);
            db.uae8 = arr;
        }
        std::int32_t maxh = 0; // local_4c
        for (std::int32_t i = 0; i < db.uac8; ++i) {
            maxh = dir(db, i).hsize < maxh ? maxh : dir(db, i).hsize;
        }
        double* h = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(maxh)), 8));
        const bool close_old = true; // local_54
        std::int32_t part = -1;      // local_58
        for (std::int32_t i = 0; i < db.uac8; ++i) {
            read_entry_header(db, &i, h, &part, close_old);
        }
        const std::int32_t unit = 3;
        close_db_unit(&unit);
        if (h != nullptr) {
            std::free(h);
        }
        reread_db_header(db);
        db.u1900 = 1;
    }
}

// @src 1401dd0d0  re-read the DB header
std::int32_t reread_db_header(Cls_1401dbfe0& db)
{
    std::int32_t err = 0;   // local_1c
    std::int32_t print = 0; // local_468
    if (!file_exists(db.u650)) {
        return -1;
    }
    char field[0x400]; // local_448
    std::int32_t len = to_field(field, db.u650, 0x400);
    resultset::DbHeader hdr; // all outputs are locals
    resultset::read_db_header(field, &len, &hdr, &err, &print);
    return err == 0 ? 0 : -2;
}

// @src 1401df8a0  first / last saved entries of an operation
bool sim_first_last(const Cls_1401dbfe0& db, std::int32_t sim, std::int32_t* dim, std::int32_t* nobj,
                    std::int32_t* pdie, std::int32_t* step_first, std::int32_t* step_last, double* t_first,
                    double* t_last, double* s_first, double* s_last)
{
    if (db.uad8 == nullptr || db.uae8 == nullptr) {
        return false; // the original clears only the low byte of `this` (the return value)
    }
    auto* entries = static_cast<DbInfoEntryE0*>(db.uae8);
    bool first = true;
    for (std::int32_t i = 0; i < db.uac8; ++i) {
        const DbDirRecord& r = dir(db, i);
        if (r.stored != 0 && r.sim_no == sim) {
            DbInfoEntryE0& e = entries[i];
            if (first) {
                *dim = r.dim;
                *nobj = entry_i32(e, 0x00);
                *pdie = entry_i32(e, 0x04);
                *step_first = r.step;
                *step_last = r.step;
                *t_first = entry_f64(e, 0x08);
                *s_first = entry_f64(e, 0x28);
                first = false;
            } else {
                *step_last = r.step;
                *t_last = entry_f64(e, 0x10);
                *s_last = entry_f64(e, 0x28);
            }
        }
    }
    return !first;
}

} // namespace def::dbio::step
