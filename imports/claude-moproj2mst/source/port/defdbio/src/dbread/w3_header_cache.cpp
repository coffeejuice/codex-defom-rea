// Header cache of the DB object Cls_1401dbfe0 (db+0xae8, one 0xe0 record per directory entry) and
// the directory accessors around it (packet p-dbread, helper W3). Declarations:
// defdbio/dbread/w3.hpp. Analysis: docs/re/dbmap/stepdriver.md section 1.6.
//
// Cache record (0xe0 bytes, ctor 1401d9210): +0 NOBJ (int)H[0], +4 (int)H[1], +8 H[2], +0x10
// H[3], +0x18 (int)H[8] (3D only), +0x1c (int)H[platform slot], +0x20 NOBJ blocks of 0xcd0 bytes
// (first min(S, 400) header words of each object + its name at +0xc80), +0x28 |stroke| of object
// (int)H[1] - 1, +0x30 F = (int)H[12], +0x34 S = (int)H[9], +0x38 (int)H[10], +0x3c (int)H[11],
// +0x4c OPRNAM (64), +0x8d SIMNAM (80).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // the original uses the plain CRT string calls
#endif
#include <defdbio/dbread/w3.hpp>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>

#include "../gendb/db_header_io.hpp"
#include "pending_w3.hpp"
#include <defdbio/dbread/w2.hpp>
#include "w3_internal.hpp"

namespace def::dbio::dbread {

using model::Cls_1401dbfe0;
using model::DbDirRecord;
using model::DbInfoEntryE0;
using w3::at;
using w3::cvtt;

namespace {

// First two words of the header label tables DAT_1406e8710 points at (1401c7f80): the header slots
// whose (min, max) pairs 1401dfb80 accumulates in db+0xaf0.
// 1405d4ac0 (owner: defdbio/src/step/db_header_cache.cpp)  2D label table, words 0 and 1
constexpr std::int32_t kLabel2d[2] = {6, 8};
// 1405d54c0 (owner: defdbio/src/step/db_header_cache.cpp)  3D label table, words 0 and 1
constexpr std::int32_t kLabel3d[2] = {12, 14};

std::int32_t label_word(std::int32_t k)
{
    using model::p407::HdrLabelTable;
    switch (model::p407::g_hdr_label_table) {
    case HdrLabelTable::table_1405d4ac0:
        return kLabel2d[k];
    case HdrLabelTable::table_1405d54c0:
        return kLabel3d[k];
    default:
        // DAT_1406e8710 is 0 before 1401c7f80 ran; 1401dfb80 always runs it first
        DEF_NOT_PORTED(0x1401dfb80);
    }
}

std::int32_t slot(std::uint64_t addr) { return model::p407::hdr_slot(addr); }

const DbDirRecord& dir(const Cls_1401dbfe0* db, std::int32_t idx)
{
    return model::db_info_records(*db)[idx];
}

std::byte* cache_record(Cls_1401dbfe0* db, std::int32_t idx)
{
    return static_cast<std::byte*>(db->uae8) + static_cast<std::ptrdiff_t>(idx) * 0xe0;
}

// 1401d91a0 / 1401d9120: the element ctors handed to the vector constructor iterators
void record_ctor(void* p) { hdr_cache_record_init(p); }
void block_ctor(void* p) { hdr_object_block_init(p); }

} // namespace

// @src 1401d9210  0xe0 cache record ctor
// @src 1401d91a0  (thunk: 1401d9210(p), return p)
void hdr_cache_record_init(void* rec)
{
    at<std::int32_t>(rec, 0x00) = 0;
    at<std::int32_t>(rec, 0x04) = 0;
    at<std::int32_t>(rec, 0x1c) = 0;
    at<std::uint64_t>(rec, 0x08) = 0;
    at<std::uint64_t>(rec, 0x10) = 0;
    at<std::uint64_t>(rec, 0x28) = 0;
    at<std::uint64_t>(rec, 0x20) = 0;
    at<std::int32_t>(rec, 0x30) = 0;
    at<std::int32_t>(rec, 0x34) = 0;
    at<std::int32_t>(rec, 0x38) = 0;
    at<std::int32_t>(rec, 0x3c) = 0;
    std::strcpy(&at<char>(rec, 0x4c), "OPERATION 1");  // 140484f90
    std::strcpy(&at<char>(rec, 0x8d), "SIMULATION 1");
    at<std::int32_t>(rec, 0x18) = 0;
}

// @src 1401d9150  0xcd0 object block ctor
// @src 1401d9120  (thunk: 1401d9150(p), return p)
void hdr_object_block_init(void* blk)
{
    for (std::int32_t i = 0; i < 400; i = i + 1) {
        at<std::uint64_t>(blk, static_cast<std::size_t>(i) * 8) = 0;
    }
    at<std::uint8_t>(blk, 0xc80) = 0;
}

// @src 1401e6930  `vector constructor iterator'
void vector_ctor_iterator(void* p, std::size_t size, std::int32_t n, void (*ctor)(void*))
{
    std::byte* q = static_cast<std::byte*>(p);
    while (n = n - 1, -1 < n) {
        ctor(q);
        q = q + size;
    }
}

// @src 1401ddb40  dimension of directory entry idx
std::int32_t dir_dim(const Cls_1401dbfe0* db, std::int32_t idx)
{
    if (db->uad8 == nullptr) {
        return 3;
    }
    if (idx < 0 || db->uac8 <= idx) {
        return 3;
    }
    return dir(db, idx).dim;
}

// @src 1401ddba0  version of directory entry idx (no test of idx < 0)
std::int32_t dir_version(const Cls_1401dbfe0* db, std::int32_t idx)
{
    if (db->uad8 == nullptr || db->uac8 < 1) {
        return 10000;
    }
    if (idx < db->uac8) {
        return dir(db, idx).version;
    }
    return 10000;
}

// @src 1401e1de0  F (first word of object block 0) and S (words per object block)
void header_layout(const Cls_1401dbfe0* db, const std::int32_t* idx, std::int32_t* f,
                   std::int32_t* s)
{
    const std::int32_t d = dir_dim(db, *idx);
    const std::int32_t v = dir_version(db, *idx);
    if (d == 2) {
        *f = 0x22;
        *s = 100;
    }
    else if (v < 3000) {
        *f = 8;
        *s = 100;
    }
    else {
        *f = 0x22;
        *s = 200;
    }
    if (10999 < v) {
        const std::byte* r = static_cast<const std::byte*>(db->uae8) + static_cast<std::ptrdiff_t>(*idx) * 0xe0;
        if (0 < *reinterpret_cast<const std::int32_t*>(r + 0x30)) {
            *f = *reinterpret_cast<const std::int32_t*>(r + 0x30);
        }
        if (0 < *reinterpret_cast<const std::int32_t*>(r + 0x34)) {
            *s = *reinterpret_cast<const std::int32_t*>(r + 0x34);
        }
    }
}

// @src 1401dd4d0  header cache of entry *idx
void read_header_cache(Cls_1401dbfe0* db, std::int32_t* idx, bool keep_raw)
{
    void* raw = nullptr; // local_48
    if (db->u1900 != 0 || db->uac8 < 1) {
        return;
    }
    if (db->uae8 == nullptr) {
        // thunk_FUN_1404797d8(n * 0xe0 + 8) with the count in front, then
        // `eh vector constructor iterator'(p + 8, 0xe0, n, 1401d91a0, 1401d91d0); delete[] in
        // 1401dc220 (1401e67c0)
        const std::size_t n = static_cast<std::size_t>(static_cast<std::int64_t>(db->uac8));
        DbInfoEntryE0* e = new DbInfoEntryE0[n];
        vector_ctor_iterator(e, 0xe0, db->uac8, &record_ctor);
        db->uae8 = e;
    }
    const std::int32_t nh = dir(db, *idx).hsize; // local_50
    raw = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(nh)), 8);
    std::int32_t cur_file = -1; // local_58
    read_header_cache_entry(db, idx, static_cast<double*>(raw), &cur_file, false);
    const std::int32_t unit = 3; // local_3c
    gendb::close_db(&unit);      // 1403ae06f
    if (!keep_raw) {
        if (raw != nullptr) {
            std::free(raw);
        }
    }
    else {
        // the previous block in +0x1908 is overwritten, not freed
        db->u1908 = raw;
        db->u1910 = nh;
    }
    db->uae0 = *idx;
}

// @src 1401dfb80  step header of entry *idx into h and cache record *idx
void read_header_cache_entry(Cls_1401dbfe0* db, std::int32_t* idx, double* h,
                             std::int32_t* cur_file, bool close_prev)
{
    std::int32_t dim = dir_dim(db, *idx); // local_88
    const bool is2d = dim == 2;           // local_78
    const bool is3d = dim == 3;           // local_4b8
    model::p407::load_hdr_slots(&dim);    // 1401c7f80
    std::int32_t ierr = 0;    // local_74[0]
    std::int32_t verbose = 0; // local_8d4
    std::int32_t unit = 3;    // local_74[1]
    // local_8cc = db+0xabc (DB version): not read afterwards
    std::int32_t nh = dir(db, *idx).hsize;          // local_49c
    const std::int32_t file = db_file_index(db, *idx); // local_4a0
    const std::int32_t first = db_file_first(db, file); // local_8d8
    std::int32_t rec = (*idx - first) + 1;             // local_4b4
    if (file != *cur_file) {
        if (close_prev && -1 < *cur_file) {
            gendb::close_db(&unit); // 1403ae06f
        }
        *cur_file = file;
        char name[0x400];   // local_488
        char fname[0x400];  // local_8b8
        db_file_name(db, name, file);
        std::int32_t len = static_cast<std::int32_t>(rt::to_fortran(fname, name, 0x400)); // 140323bf0
        ftn_open_db(fname, &len, &unit, &ierr, &verbose); // 1403adc74
        if (ierr != 0) {
            return;
        }
    }
    ftn_read_step_header_unit(&unit, &rec, &nh, h, &ierr, &verbose); // 1403daf3e
    if (ierr != 0) {
        return;
    }
    std::byte* r = cache_record(db, *idx);
    at<std::int32_t>(r, 0x00) = cvtt(h[0]);
    at<std::int32_t>(r, 0x04) = cvtt(h[1]);
    at<double>(r, 0x08) = h[2];
    at<double>(r, 0x10) = h[3];
    at<std::int32_t>(r, 0x1c) = cvtt(h[slot(0x1406e8524)]);
    at<std::int32_t>(r, 0x30) = cvtt(h[0xc]);
    at<std::int32_t>(r, 0x34) = cvtt(h[9]);
    at<std::int32_t>(r, 0x38) = cvtt(h[10]);
    at<std::int32_t>(r, 0x3c) = cvtt(h[0xb]);
    std::strncpy(&at<char>(r, 0x4c), reinterpret_cast<const char*>(h + 0x10), 0x40);
    rt::trim_fortran(&at<char>(r, 0x4c), 0x40); // 1403239f0
    std::strncpy(&at<char>(r, 0x8d), reinterpret_cast<const char*>(h + 0x18), 0x50);
    rt::trim_fortran(&at<char>(r, 0x8d), 0x50);
    if (at<std::int32_t>(r, 0x38) == 0) {
        at<std::int32_t>(r, 0x38) = dir(db, *idx).sim_no;
        std::strncpy(&at<char>(r, 0x4c), &at<char>(r, 0x8d), 0x40);
        rt::trim_fortran(&at<char>(r, 0x4c), 0x40);
    }
    if (is3d) {
        at<std::int32_t>(r, 0x18) = cvtt(h[8]);
    }

    // operator new (thunk_FUN_1404797d8: malloc, bad_alloc when it fails) of NOBJ blocks, the
    // product saturated to SIZE_MAX on overflow; freed with free() by the record dtor 1401d91d0
    const std::uint64_t n = static_cast<std::uint64_t>(static_cast<std::int64_t>(at<std::int32_t>(r, 0)));
    std::uint64_t bytes = n * 0xcd0;
    if (n != 0 && bytes / n != 0xcd0) {
        bytes = ~static_cast<std::uint64_t>(0);
    }
    void* blocks = std::malloc(static_cast<std::size_t>(bytes));
    if (blocks == nullptr) {
        throw std::bad_alloc();
    }
    vector_ctor_iterator(blocks, 0xcd0, static_cast<std::int32_t>(n), &block_ctor);
    at<void*>(r, 0x20) = blocks;

    std::int32_t f = 0; // local_80
    std::int32_t s = 0; // local_7c
    header_layout(db, idx, &f, &s);
    bool first_obj = true;     // local_498
    const std::int32_t pairs = 2; // local_84
    double* g = reinterpret_cast<double*>(db->uaf0);
    for (std::int32_t i = 0; i < at<std::int32_t>(r, 0); i = i + 1) {
        const double* p = h + (f + s * i);              // local_4a8
        const double* name = p + slot(0x1406e8528);     // local_68
        std::byte* blk = static_cast<std::byte*>(at<void*>(r, 0x20)) + static_cast<std::ptrdiff_t>(i) * 0xcd0;
        std::strncpy(&at<char>(blk, 0xc80), reinterpret_cast<const char*>(name), 0x40);
        rt::trim_fortran(&at<char>(blk, 0xc80), 0x40);
        const std::int32_t cnt = s < 400 ? s : 400; // local_30
        std::memcpy(blk, p, static_cast<std::size_t>(static_cast<std::int64_t>(cnt) << 3));
        if (first_obj) {
            for (std::int32_t k = 0; k < pairs; k = k + 1) {
                const std::int32_t w = label_word(k);
                db->uaf0[w] = at<std::uint64_t>(blk, static_cast<std::size_t>(w) * 8);
                db->uaf0[w + 1] = at<std::uint64_t>(blk, static_cast<std::size_t>(w + 1) * 8);
            }
            first_obj = false;
        }
        else {
            for (std::int32_t k = 0; k < pairs; k = k + 1) {
                const std::int32_t w = label_word(k);
                // 1401e0691: COMISD blk, cur; JBE -> blk (also for NaN)
                const double lo = at<double>(blk, static_cast<std::size_t>(w) * 8);
                const double cur_lo = g[w];
                g[w] = (lo > cur_lo) ? cur_lo : lo;
                // 1401e07c3: COMISD cur, blk; JBE -> blk
                const double hi = at<double>(blk, static_cast<std::size_t>(w + 1) * 8);
                const double cur_hi = g[w + 1];
                g[w + 1] = (cur_hi > hi) ? cur_hi : hi;
            }
        }
        if (i == at<std::int32_t>(r, 4) - 1) {
            // |stroke| of the object (int)H[1] (PDIE) from the header block, 140484644 = sqrt
            const double* x = p;
            const std::int32_t a = slot(0x1406e852c);
            const std::int32_t b = slot(0x1406e8530);
            if (!is2d) {
                const std::int32_t c = slot(0x1406e8534);
                double q = x[a] * x[a];
                q = q + x[b] * x[b];
                q = q + x[c] * x[c];
                at<double>(r, 0x28) = std::sqrt(q);
            }
            else {
                double q = x[a] * x[a];
                q = q + x[b] * x[b];
                at<double>(r, 0x28) = std::sqrt(q);
            }
        }
    }
}

} // namespace def::dbio::dbread
