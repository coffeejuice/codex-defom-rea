// Multi-file DB chain of the DB info object Cls_1401dbfe0 (packet mdt-dbquery): a DB whose first
// step is N (|N| >= 2) continues an earlier part `<base><|N|-1>.DB` (or `<base><-|N|>.DB`, whose
// last entry then overlaps and is dropped). Declarations: defdbio/db_query.hpp.
//
// Part tables of the DB info object: db+0x8 part count, db+0xc (first, last) directory index per
// part, db+0x330 part file names (malloc'd 0x400 buffers). The DB module keeps its own copy, set
// by 1403b18c3, in the padding of p1-12's DbCommon (dbcore/globals_p1_12.hpp): part count at
// 0x140b49100, 100 names of 200 characters from 0x140b49104 (name k at 0x140b4903c + 200 k), 4
// words per part from 0x140b4df24 (part k at 0x140b4df14 + 16 k) and the entry total at
// 0x140b4e568. Its only readers in DEF_PRE_64 (1403b11a7, 1403b154c) are not ported.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // the original uses the plain CRT string calls
#endif
#include <defdbio/db_query.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/resultset/db_result_sets.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "../gendb/db_header_io.hpp"

namespace def::dbio::query {

using def::model::DbDirRecord;

namespace {

constexpr std::uint64_t kDbCommon = 0x140b45b80; // first address of dbcore::g_db

template <class T>
T* db_common_at(std::uint64_t addr)
{
    return reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&dbcore::g_db) + (addr - kDbCommon));
}

std::int32_t& g_parts() { return *db_common_at<std::int32_t>(0x140b49100); }   // DAT_140b49100
std::int32_t& g_entries() { return *db_common_at<std::int32_t>(0x140b4e568); } // DAT_140b4e568

// FUN_140484ab8: |v|
std::int32_t iabs(std::int32_t v) { return v < 0 ? -v : v; }

DbDirRecord* records(Cls_1401dbfe0& db) { return static_cast<DbDirRecord*>(db.uad8); }

} // namespace

// @src 140323ce0  cut at the last '.'
void strip_extension(char* name)
{
    std::int32_t i = static_cast<std::int32_t>(std::strlen(name));
    do {
        i = i - 1;
        if (i < 0) {
            return;
        }
    } while (name[i] != '.');
    name[i] = '\0';
}

// @src 1401e4d80  does the DB `name` end at step *step or *step - 1
bool chain_part_matches(const char* name, const std::int32_t* step)
{
    // operator new(0x1918) + ctor 1401dbfe0; never deleted (the original leaks it)
    auto* d = new Cls_1401dbfe0();
    (void)load_db_info(*d, name, false); // 1401dc3f0(d, name, 0), result ignored
    if (d->uac8 < 1) {
        return false;
    }
    // no null check on the directory, as in the original
    const std::int32_t last = records(*d)[d->uac8 - 1].step;
    if (last == *step) {
        return true;
    }
    return last == *step - 1;
}

// @src 1401e4c10  does a previous part exist
bool has_previous_part(const Cls_1401dbfe0& db)
{
    if (db.uad8 == nullptr) {
        return false;
    }
    const DbDirRecord* r = def::model::db_info_records(db);
    if (iabs(r[0].step) < 2) {
        return false;
    }
    char base[0x400];  // local_828
    char name[0x400];  // local_418
    std::strcpy(base, db.u650);
    strip_extension(base);
    std::int32_t n = iabs(r[0].step) - 1; // local_428[0]
    rt::crt::sprintf(name, "%s%d.DB", base, n); // str 14052c188
    if (!file_exists(name)) {
        n = -iabs(r[0].step);
        rt::crt::sprintf(name, "%s%d.DB", base, n); // str 14052c190
        if (!file_exists(name)) {
            return false;
        }
    }
    return chain_part_matches(name, &n);
}

// @src 1401e51d0  prepend the previous part
std::int32_t prepend_previous_part(Cls_1401dbfe0& db)
{
    bool minus = false;       // local_d08: the part was found under the `-N` name
    std::int32_t err = 0;     // local_9c
    std::int32_t print = 0;   // local_8f4
    std::int32_t family = 0;  // local_b4
    std::int32_t version = 0; // local_b8
    char base[0x400];         // local_cf8
    char name[0x400];         // local_4b8
    char field[0x400];        // local_8c8

    if (iabs(records(db)[0].step) < 2) {
        return 5;
    }
    std::strcpy(base, db.u650);
    strip_extension(base);
    std::int32_t n = iabs(records(db)[0].step) - 1; // local_8d0
    rt::crt::sprintf(name, "%s%d.DB", base, n);     // str 14052c188
    if (!file_exists(name)) {
        n = -iabs(records(db)[0].step);
        rt::crt::sprintf(name, "%s%d.DB", base, n); // str 14052c190
        minus = true;
        if (!file_exists(name)) {
            return 5;
        }
    }
    std::int32_t len = static_cast<std::int32_t>(rt::to_fortran(field, name, 0x400)); // local_4bc
    resultset::DbHeader hdr{}; // TITLE, SMI1..4, ISTART, ISMTYP, NRSETS (local_d68[0]), NXTREC
    resultset::read_db_header(field, &len, &hdr, &err, &print);
    family = hdr.family;
    version = hdr.version;
    if (err != 0) {
        return 2;
    }
    if (version < 1) {
        version = db.uabc;
    } else {
        db.uabc = version;
    }
    std::int32_t cols; // local_8f8
    if (family == 2) {
        cols = 10;
    } else if (family == 3) {
        if (db.uabc < 3000) {
            cols = 5;
        } else {
            cols = 10;
        }
    } else {
        if (family != 0x17) {
            return 2;
        }
        cols = 10;
    }
    std::int32_t count = hdr.nrsets; // local_d68[0]
    // The original callocs count * cols ints, and the directory read stores 10 words per entry
    // whatever `cols` is (see load_db_info, 1401dc3f0, the same overrun with cols = 5); the port
    // allocates 10 per entry and still reads with stride `cols`.
    const std::int32_t alloc_cols = cols < 10 ? 10 : cols;
    std::int32_t* words = static_cast<std::int32_t*>(
        std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(count * alloc_cols)), 4)); // local_8e8
    resultset::read_result_sets(field, &len, &count, words, &cols, &version, &err, &print);
    if (err != 0) {
        if (words != nullptr) {
            std::free(words);
        }
        return 3;
    }
    if (minus) {
        count = count - 1; // the `-N` part repeats the first step of this one
    }
    if (count < 1) {
        return 5; // `words` is not freed on this path in the original
    }

    // part tables: shift every part one place up, the new part becomes part 0
    db.u330[db.u008] = std::malloc(0x400); // thunk_FUN_1404797d8(0x400)
    for (std::int32_t k = db.u008 - 1; -1 < k; k = k - 1) {
        std::strcpy(static_cast<char*>(db.u330[k + 1]), static_cast<const char*>(db.u330[k]));
        db.u00c[k + 1].a = db.u00c[k].a + count;
        db.u00c[k + 1].b = db.u00c[k].b + count;
    }
    std::strcpy(static_cast<char*>(db.u330[0]), name);
    db.u00c[0].a = 0;
    db.u00c[0].b = count - 1;
    db.u008 = db.u008 + 1;
    db.uac8 = db.uac8 + count;

    DbDirRecord* old = records(db); // local_8d8
    db.uad8 = new DbDirRecord[static_cast<std::size_t>(db.uac8)]; // ctor 1401d9080 each
    DbDirRecord* rec = records(db);
    std::int32_t dim = family; // local_8cc
    std::int32_t ver = version; // local_4c8
    for (std::int32_t i = 0; i < count; i = i + 1) {
        const std::int32_t b = i * cols; // local_98
        DbDirRecord& r = rec[i];
        r.mesh_no = words[b];
        r.step = words[b + 1];
        r.hsize = words[b + 2];
        r.mar_size = words[b + 3];
        r.a_size = words[b + 4];
        if (9999 < version) {
            dim = words[b + 7];
            ver = words[b + 8];
        }
        r.dim = dim;
        r.version = ver;
        if (dim == 2) {
            r.sim_no = words[b + 5];
            r.sim_mode = words[b + 6];
        } else if (ver < 3000) {
            r.sim_no = 1;
            r.sim_mode = 0;
        } else {
            r.sim_no = words[b + 5];
            r.sim_mode = words[b + 6];
        }
        if (r.mar_size < 1) {
            r.stored = 0;
        } else {
            r.stored = 1;
            db.uacc = db.uacc + 1;
        }
    }
    for (std::int32_t i = count; old != nullptr && i < db.uac8; i = i + 1) {
        DbDirRecord tmp = old[i - count]; // 0x28-byte copy through local_60
        rec[i] = tmp;
    }
    if (words != nullptr) {
        std::free(words);
    }
    if (old != nullptr) {
        def::model::delete_dir_records(old); // vector deleting dtor, flag 3
    }
    return 0;
}

// @src 1401e4e80  set up the part chain
std::int32_t setup_part_chain(Cls_1401dbfe0& db)
{
    const std::int32_t width = 200; // local_68
    db.u008 = 1;
    db.u00c[0].a = 0;
    db.u00c[0].b = db.uac8 - 1;
    db.u330[0] = std::malloc(0x400); // thunk_FUN_1404797d8(0x400)
    std::strcpy(static_cast<char*>(db.u330[0]), db.u650);
    std::int32_t rc = 0; // local_58
    for (std::int32_t k = 1; k < 100; k = k + 1) {
        rc = prepend_previous_part(db);
        if (rc != 0) {
            break;
        }
    }
    char* names = static_cast<char*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(db.u008 * width))));
    std::int32_t* table = static_cast<std::int32_t*>(
        std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(db.u008 * 4)) * 4));
    const DbDirRecord* r = records(db);
    for (std::int32_t k = 0; k < db.u008; k = k + 1) {
        rt::to_fortran(names + k * width, static_cast<const char*>(db.u330[k]), width);
        table[k * 4] = db.u00c[k].a + 1;
        table[k * 4 + 1] = db.u00c[k].b + 1;
        table[k * 4 + 2] = r[db.u00c[k].a].step;
        table[k * 4 + 3] = r[db.u00c[k].b].step;
    }
    std::int32_t n = db.u008; // local_6c
    set_db_parts(names, &n, table);
    std::free(names);
    std::free(table);
    if (db.uae8 != nullptr) {
        def::model::delete_entries_e0(static_cast<def::model::DbInfoEntryE0*>(db.uae8)); // flag 3
        db.uae8 = nullptr;
    }
    if (rc == 5) {
        rc = 0;
    }
    return rc;
}

// @src 1403b18c3  part table into the DB module
void set_db_parts(const char* names, const std::int32_t* n, const std::int32_t* table)
{
    g_parts() = *n;
    std::int32_t nw = g_parts() << 2;
    rt::ftn::copy_words4(table, db_common_at<std::int32_t>(0x140b4df24), &nw); // FUN_140238025
    g_entries() = 0;
    // DAT_140707120 is the loop counter in static storage (a local here, port-conventions s. 5)
    std::int32_t off = 1;
    for (std::int32_t k = 1; k <= g_parts(); k = k + 1) {
        // name(k) = names(off:off+199), CHARACTER assignment of 200 to 200 characters (1404b33e4)
        gendb::char_assign(db_common_at<char>(0x140b4903c + static_cast<std::uint64_t>(k) * 200), names + (off - 1), 200, 200);
        const std::int32_t* w = db_common_at<std::int32_t>(0x140b4df14 + static_cast<std::uint64_t>(k) * 16);
        g_entries() = ((w[1] + g_entries()) - w[0]) + 1;
        off = off + 200;
    }
}

std::int32_t db_part_count() { return g_parts(); }

const char* db_part_name(std::int32_t k) { return db_common_at<char>(0x140b4903c + static_cast<std::uint64_t>(k) * 200); }

const std::int32_t* db_part_words(std::int32_t k)
{
    return db_common_at<std::int32_t>(0x140b4df14 + static_cast<std::uint64_t>(k) * 16);
}

std::int32_t db_part_entries() { return g_entries(); }

} // namespace def::dbio::query
