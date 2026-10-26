// Reading the .DB header and step directory into Cls_1401dbfe0 (packet p1-03).
// Ported from FUN_1401dc3f0; supersedes the -m thread's local copy (def::pre::load_db_info).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // the original uses the plain CRT string calls
#endif
#include <defdbio/step/db_info_read.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <share.h>
#endif

#include "ext.hpp"

namespace def::dbio {

using def::model::Cls_1401dbfe0;
using def::model::DbDirRecord;

// @src 1401b8560  file exists: fopen(name, "r") (str 1405d13a0) through 14047b934 = _fsopen(.., 0x40)
bool file_exists(const char* name)
{
    if (name == nullptr) {
        return false;
    }
    if (std::strlen(name) == 0) {
        return false;
    }
#ifdef _WIN32
    std::FILE* f = _fsopen(name, "r", _SH_DENYNO);
#else
    std::FILE* f = std::fopen(name, "r");
#endif
    if (f == nullptr) {
        return false;
    }
    std::fclose(f);
    return true;
}

// @src 1401dc3f0  read header and step directory into the DB info object
std::int32_t load_db_info(Cls_1401dbfe0& d, const char* name, bool header_only)
{
    std::int32_t len = 0;           // local_ac
    char smi1[8] = {};              // local_528 version text
    char smi2[8] = {};              // local_90 release date
    char smi3_title[88] = {};       // local_590: [0..8) first-simulation date, [8..88) title
    char smi4[8] = {};              // local_a0 last-simulation date
    std::int32_t err = 0;           // local_84
    std::int32_t print = 0;         // local_530
    std::int32_t istart = 0;        // local_b0
    std::int32_t ismtyp = 0;        // local_88
    std::int32_t nrsets = 0;        // local_598
    std::int32_t nxtrec = 0;        // local_b4
    std::int32_t cols = 0;          // local_534
    float vf = 0.0f;                // local_538
    std::int32_t sig_err = 0;       // local_98
    char fname[0x400];              // local_4b8
    char vtext[80];                 // local_508

    if (!file_exists(name)) {
        def::model::db_info_reset(d);
        return 4;
    }
    d.u1900 = 0;
    if (ext::fun_1402248c0(name, &sig_err) == 0) {
        return 1;
    }
    d.uacc = 0;
    d.u008 = 1;
    for (std::int32_t i = 0; i < 100; ++i) {
        d.u00c[i].b = 0;
        d.u00c[i].a = 0;
        for (std::int32_t k = 0; k < 100; ++k) { // the original frees all 100 inside each pass
            if (d.u330[k] != nullptr) {
                std::free(d.u330[k]);
                d.u330[k] = nullptr;
            }
        }
    }
    std::int32_t family = 0;  // local_a4
    std::int32_t version = 0; // local_a8
    std::strcpy(d.u650, name);
    len = ext::fun_140323bf0(fname, d.u650, 0x400);
    ext::fun_1403d84b0(fname, &len, smi3_title + 8, smi1, smi2, smi3_title, smi4, &istart, &ismtyp,
                       &nrsets, &nxtrec, &family, &version, &err, &print);
    if (err != 0) {
        return 2;
    }
    if (nrsets < 1) {
        return 2;
    }
    std::strncpy(d.ua50, smi3_title + 8, 0x50);
    ext::fun_1403239f0(d.ua50, 0x50);
    std::strncpy(vtext, smi1, 8);
    ext::fun_1403239f0(vtext, 8);
    std::sscanf(vtext, "%f", &vf); // str 14052b0ac
    // float * 1000.0f (DAT_1405094e8, MULSS), widened, + 0.5 (DAT_14051f240), truncated
    d.uabc = static_cast<std::int32_t>(static_cast<double>(vf * 1000.0f) + 0.5);
    std::strncpy(d.uaa1, smi2, 8);
    ext::fun_1403239f0(d.uaa1, 8);
    std::strncpy(d.uaa9, smi3_title, 8);
    ext::fun_1403239f0(d.uaa9, 8);
    std::strncpy(d.uab1, smi4, 8);
    ext::fun_1403239f0(d.uab1, 8);
    d.uac0 = istart;
    d.uac4 = ismtyp;
    d.uac8 = nrsets;
    d.uad0 = nxtrec;
    if (header_only) {
        return 0;
    }

    if (version < 1) {
        version = d.uabc;
    } else {
        d.uabc = version;
    }
    if (family == 2) {
        cols = 10;
    } else if (family == 3) {
        if (d.uabc < 3000) {
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
    // The original callocs nrsets * cols ints; the directory read (1403b6425 under 1403d96fc)
    // stores 10 words per entry whatever `cols` is, so with cols = 5 it writes past the block.
    // The port allocates room for 10 words per entry; the loop below still reads with stride
    // `cols`, as the original does.
    const std::int32_t alloc_cols = cols < 10 ? 10 : cols;
    std::int32_t* words = static_cast<std::int32_t*>(
        std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(nrsets * alloc_cols)), 4));
    ext::fun_1403d96fc(fname, &len, &nrsets, words, &cols, &version, &err, &print);
    if (err != 0) {
        if (words != nullptr) {
            std::free(words);
        }
        return 3;
    }
    if (d.uad8 != nullptr) {
        def::model::delete_dir_records(static_cast<DbDirRecord*>(d.uad8));
        d.uad8 = nullptr;
    }
    if (0 < nrsets) {
        d.uad8 = new DbDirRecord[static_cast<std::size_t>(nrsets)]; // ctor 1401d9080 each
    }
    DbDirRecord* rec = static_cast<DbDirRecord*>(d.uad8);
    std::int32_t dim = family; // local_514
    std::int32_t ver = version; // local_b8
    for (std::int32_t i = 0; i < nrsets; ++i) {
        const std::int32_t b = i * cols; // local_80
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
            d.uacc = d.uacc + 1;
        }
    }
    if (words != nullptr) {
        std::free(words);
    }
    if (d.uae8 != nullptr) {
        def::model::delete_entries_e0(static_cast<def::model::DbInfoEntryE0*>(d.uae8));
        d.uae8 = nullptr;
    }
    return 0;
}

// @src 1401e39b0  operation of the last stored entry
std::int32_t last_stored_sim_no(const Cls_1401dbfe0& d)
{
    const DbDirRecord* rec = def::model::db_info_records(d);
    std::int32_t i = d.uac8;
    do {
        i = i - 1;
        if (i < 0) {
            return 0;
        }
    } while (rec[i].stored == 0);
    return rec[i].sim_no;
}

// @src 1401e3e20  sampling interval
std::int32_t step_interval(std::int32_t n)
{
    std::int32_t r;
    if (n < 0x32) {
        r = 5;
    } else if (n < 100) {
        r = 10;
    } else if (n < 500) {
        r = 0x19;
    } else if (n < 1000) {
        r = 0x32;
    } else if (n < 5000) {
        r = 100;
    } else {
        r = 100;
    }
    return r;
}

} // namespace def::dbio
