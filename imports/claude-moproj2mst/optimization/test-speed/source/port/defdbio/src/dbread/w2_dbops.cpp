// dbread/w2_dbops.cpp: the old-format (version < 2301, 3D "DB230") DB operations and the single
// A-region reader of ReadSingleAArrayFromDB (packet p-dbread, helper W2).
//
// Reference: listing of 1403dc957, 1403bef07, 1403c0e60, 140444458, 140469eb0
// (deform-rea-work/out/dbread/w2/listing.txt); the decompile only for structure. The Fortran
// interface (arguments by pointer, unit numbers, error codes in *ierr) is kept. File layout of the
// old format: record 1 signature, record 2 header (dbcore::g_db.rec2), record 3 index (128
// words: first file record of directory record i), directory records of 128 entries x 8 words
// (8 file records each): w1 mesh, w2 step, w3 header words, w4 MAR words, w5 A words,
// w6 header record, w7 MAR record, w8 A record.
#include <defdbio/dbread/w2.hpp>

#include <cstdio>
#include <cstring>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/dbcore/util.hpp>
#include <defrt/ftn/helpers.hpp>

#include "dbcore/internal.hpp"
#include "dbread/globals_w2.hpp"
#include "gendb/db_header_io.hpp"

namespace def::dbio::dbread {

namespace {

using dbcore::block_io_char4;
using dbcore::block_io_int;
using dbcore::block_io_real8;
using dbcore::g_db;
using dbcore::platform_swap_mode;
using dbcore::swap4_words;
using dbcore::swap8_words;
using dbcore::swap_mode;

constexpr std::int32_t kRead = 1;
constexpr std::int32_t kWrite = 2;

// 1404b4108: Fortran runtime comparison of CHARACTER a(1:la) with b(1:lb), the shorter one
// blank padded; <0, 0, >0 like memcmp (bytes compared unsigned). Runtime routine, not ported;
// only its sign is used here.
std::int32_t ftn_compare(const char* a, const char* b, std::int32_t la, std::int32_t lb)
{
    const std::int32_t n = la > lb ? la : lb;
    for (std::int32_t k = 0; k < n; ++k) {
        const unsigned char ca = static_cast<unsigned char>(k < la ? a[k] : ' ');
        const unsigned char cb = static_cast<unsigned char>(k < lb ? b[k] : ' ');
        if (ca != cb) {
            return ca < cb ? -1 : 1;
        }
    }
    return 0;
}

// 1404b53a4: CLOSE(unit) of the Fortran runtime (as gendb::close_db does it)
void close_unit(const std::int32_t* unit)
{
    const std::int32_t mode = 0;
    std::int32_t ios = 0;
    dbcore::fortran_open(&mode, unit, "", "", &ios, 0, 0);
}

// 1404b5ef4: list-directed WRITE(6,*) of two CHARACTER items (no separator between character
// items in list-directed output; the record starts with a blank)
void ftn_write_text2(const char* a, const char* b, std::int32_t nb)
{
    std::printf(" %s%.*s\n", a, nb, b);
}

// REAL*8 block I/O as the old code does it: REAL*8 on a mode-1 platform, else 2n INTEGER words
void real_io(std::int32_t mode, const std::int32_t* unit, void* buf, const std::int32_t* n, std::int32_t* rec,
             std::int32_t* ierr)
{
    std::int32_t pm = platform_swap_mode();
    if (pm == 1) {
        block_io_real8(&mode, unit, buf, n, rec, ierr);
    } else {
        std::int32_t n2 = *n * 2;
        block_io_int(&mode, unit, buf, &n2, rec, ierr);
    }
}

// swaps when the platform mode (140407e6d) is 2, then again when it is 3 (never both)
void swap4_platform(void* p, std::int32_t* n)
{
    if (platform_swap_mode() == 2) swap4_words(p, n);
    if (platform_swap_mode() == 3) swap4_words(p, n);
}
void swap8_platform(void* p, std::int32_t* n)
{
    if (platform_swap_mode() == 2) swap8_words(p, n);
    if (platform_swap_mode() == 3) swap8_words(p, n);
}
// same by the file's mode (140407e60)
void swap4_file(void* p, std::int32_t* n)
{
    if (swap_mode() == 2) swap4_words(p, n);
    if (swap_mode() == 3) swap4_words(p, n);
}
void swap8_file(void* p, std::int32_t* n)
{
    if (swap_mode() == 2) swap8_words(p, n);
    if (swap_mode() == 3) swap8_words(p, n);
}

// SMI1 >= " 1.0    ": the arrays are run-length coded (g_old_compressed)
void check_compressed()
{
    std::int32_t eight = 8;
    g_old_compressed = 0;
    copy_chars(g_db.h.smi1, g_old_tmp.smi, &eight);
    if (-1 < ftn_compare(g_old_tmp.smi, kSmi10, 8, 8)) {
        g_old_compressed = 1;
    }
}

} // namespace

// @src 140469eb0  one character
void copy_char(char* dst, const char* src, std::int64_t /*n*/)
{
    dst[0] = src[0]; // 140469eb0 MOVSX EAX,[RDX]; MOV [RCX],AL
}

// @src 140444458  copy *n characters one at a time; the index is the global DAT_14070dd00
void copy_chars(const char* src, char* dst, const std::int32_t* n)
{
    g_copy_index = 1;
    if (0 < *n) {
        do {
            const std::int32_t mode = dbcore::platform_swap_mode();
            char* d = dst + (g_copy_index - 1);
            const char* s = src + (g_copy_index - 1);
            if (mode == 2) {
                copy_char(d, s, 1);
            } else {
                d[0] = s[0]; // 140469eb6: the same single-character copy (db_header_io.cpp)
            }
            g_copy_index = g_copy_index + 1;
        } while (g_copy_index <= *n);
    }
}

// @src 1403c0e60  old-format directory record of entry *slot: mode 2 writes the record in
// memory, any other mode loads it unless it is the loaded one. *rec = (slot-1)/128 + 1,
// *pos = position in it (1..128).
void chunk_io_old(const std::int32_t* mode, const std::int32_t* unit, const std::int32_t* slot, std::int32_t* rec,
                  std::int32_t* pos, std::int32_t* ierr)
{
    std::int32_t m = 0x80;
    *ierr = 0;
    std::int32_t p = dbcore::mod1(slot, &m); // 14044ac11
    *pos = p;
    const std::int32_t r = (*slot - 1) / 128 + 1; // 1403c0ec4..ed4: SAR with the bias for negatives
    *rec = r;
    std::int32_t n = 0x400;
    if (*mode == 2) {
        g_old_tmp.chunk_rec = g_old_db.index_word(r);
        g_old_db.cur_rec = r;
        swap4_platform(g_old_db.chunk, &n);
        n = 0x400;
        std::int32_t wm = kWrite;
        block_io_int(&wm, unit, g_old_db.chunk, &n, &g_old_tmp.chunk_rec, ierr);
        swap4_platform(g_old_db.chunk, &n);
        if (*ierr != 0) {
            *ierr = 0x6f;
        }
    } else if (g_old_db.cur_rec != r) {
        g_old_tmp.chunk_rec = g_old_db.index_word(r);
        g_old_db.cur_rec = r;
        std::int32_t rm = kRead;
        block_io_int(&rm, unit, g_old_db.chunk, &n, &g_old_tmp.chunk_rec, ierr);
        if (*ierr == 0 && swap_mode() == 2) {
            swap4_words(g_old_db.chunk, &n);
        }
        if (*ierr == 0 && swap_mode() == 3) {
            swap4_words(g_old_db.chunk, &n);
        }
        if (*ierr != 0) {
            *ierr = 0x65;
        }
    }
}

namespace {

// part of @src 1403bef07: op 0 (also any op outside 0..4)
void old_open(const std::int32_t* unit, std::int32_t* ierr)
{
    auto& t = g_old_tmp;
    t.rec = 2;
    g_old_db.cur_rec = 0;
    t.swap = platform_swap_mode();
    dbcore::set_swap_mode(&t.swap);
    std::int32_t mode = kRead;
    std::int32_t n = 0x22;
    block_io_int(&mode, unit, g_db.rec2, &n, &t.rec, ierr);
    if (platform_swap_mode() == 1) {
        n = 0x1c;
        swap4_words(g_db.rec2, &n);
    }
    const std::int32_t ty = g_db.h.ismtyp;
    if (ty == 0 || ty == 1 || ty == 2) {
        t.swap = platform_swap_mode();
        dbcore::set_swap_mode(&t.swap);
    } else {
        std::int32_t two = 2;
        dbcore::set_swap_mode(&two);
    }
    n = 6;
    swap4_file(&g_db.h.istart, &n);
    // 1403bf06f: ISMTYP is read again, after the swap
    if (g_db.h.ismtyp != 0 && g_db.h.ismtyp != 1 && g_db.h.ismtyp != 2) {
        *ierr = 1;
    }
    t.cmp_len = 8;
    t.cmp_res = 0;
    t.u894 = 0;
    t.u898 = 0;
    def::rt::ftn::ftn_strncmp_b(g_db.h.smi1, kSmi201, &t.cmp_len, &t.cmp_res); // 140235670
    if (*ierr != 0) {
        *ierr = 0x65;
        return;
    }
    if (t.cmp_res < 0) {
        *ierr = 100;
        return;
    }
    t.rec = 3;
    n = 0x80;
    block_io_int(&mode, unit, g_old_db.index, &n, &t.rec, ierr);
    if (*ierr == 0 && swap_mode() == 2) {
        swap4_words(g_old_db.index, &n);
    }
    if (*ierr == 0 && swap_mode() == 3) {
        swap4_words(g_old_db.index, &n);
    }
    if (*ierr != 0) {
        *ierr = 0x65;
    }
}

// part of @src 1403bef07: op 1, words 1..5 of entries 1..*count (0: all) into out, 5 per entry
void old_directory(const std::int32_t* unit, std::int32_t* count, std::int32_t* out, std::int32_t* ierr)
{
    auto& t = g_old_tmp;
    if (*count == 0) {
        *count = g_db.h.nrsets;
    }
    t.last = 0;
    for (;;) {
        t.first = t.last + 1;
        const std::int32_t one = 1;
        chunk_io_old(&one, unit, &t.first, &t.drec, &t.pos2, ierr);
        if (*ierr != 0) {
            return;
        }
        t.last = t.last + 0x80;
        if (*count <= t.last) {
            t.last = *count;
        }
        t.out = t.first * 5 - 5;
        t.count = (t.last - t.first) + 1;
        t.i = 1;
        if (0 < t.count) {
            do {
                t.j = 1;
                do {
                    out[t.out] = g_old_db.entry(t.i, t.j);
                    t.out = t.out + 1;
                    t.j = t.j + 1;
                } while (t.j <= 5);
                t.i = t.i + 1;
            } while (t.i <= t.count);
        }
        if (*count <= t.last) {
            return;
        }
    }
}

// part of @src 1403bef07: ops 2 (step header) and 3 (MAR and A) of the entry at g_old_tmp.pos
void old_read(const std::int32_t* unit, const std::int32_t* op, std::int32_t* nh, double* h, std::int32_t* nmar,
              std::int32_t* mar, std::int32_t* na, double* a, std::int32_t* ierr)
{
    auto& t = g_old_tmp;
    const std::int32_t p = t.pos;
    if (*op == 2) {
        if (*nh < 1) {
            *nh = g_old_db.entry(p, 3);
        }
        t.rec = g_old_db.entry(p, 6);
        real_io(kRead, unit, h, nh, &t.rec, ierr);
        swap8_file(h, nh);
        if (*ierr != 0) {
            *ierr = 0x66;
        }
        return;
    }
    if (*op != 3) {
        return;
    }
    if (0 < g_old_db.entry(p, 4) && 0 < g_old_db.entry(p, 5)) {
        check_compressed();
        if (*nmar < 1) {
            *nmar = g_old_db.entry(p, 4);
        }
        t.rec = g_old_db.entry(p, 7);
        std::int32_t mode = kRead;
        block_io_int(&mode, unit, mar, nmar, &t.rec, ierr);
        swap4_file(mar, nmar);
        if (*ierr != 0) {
            *ierr = 0x67;
            return;
        }
        if (g_old_compressed == 1) {
            dbcore::rle_decode_int(nmar, mar, &t.n);
            *nmar = t.n;
        }
        if (*na < 1) {
            *na = g_old_db.entry(p, 5);
        }
        t.rec = g_old_db.entry(p, 8);
        real_io(kRead, unit, a, na, &t.rec, ierr);
        swap8_file(a, na);
        if (*ierr != 0) {
            *ierr = 0x68;
            return;
        }
        if (g_old_compressed == 1) {
            dbcore::rle_decode_real(na, a, &t.n);
            *na = t.n;
        }
        return;
    }
    *ierr = 0xca;
}

// part of @src 1403bef07: op 4, write the step (header h, MAR, A) at entry *slot (< 1: append)
void old_write(const std::int32_t* unit, std::int32_t* slot, const std::int32_t* step, std::int32_t* nh, double* h,
               std::int32_t* nmar, std::int32_t* mar, std::int32_t* na, double* a, std::int32_t* ierr)
{
    auto& t = g_old_tmp;
    t.keep = 1;
    t.appended = 0;
    t.swap = platform_swap_mode();
    dbcore::set_swap_mode(&t.swap);
    if (t.swap == 0) {
        // 1403c000b: COMISD " 2.0.1  " (as a double), SMI1 (as a double); JP / JBE continue
        double lit = 0.0;
        double smi = 0.0;
        std::memcpy(&lit, kSmi201, 8);
        std::memcpy(&smi, g_db.h.smi1, 8);
        if (lit > smi) {
            *ierr = 100;
            return;
        }
    }
    const std::int32_t one = 1;
    if (*slot < 1) {
        t.appended = 1;
        if (0 < (*step - 1) % g_db.put_interval) {
            t.keep = 0;
        }
        if (g_db.h.istart == g_db.h.nrsets) {
            t.keep = 1;
        }
        chunk_io_old(&one, unit, &g_db.h.nrsets, &t.drec, &t.pos, ierr);
        if (*ierr != 0) {
            return;
        }
        if (*step == g_old_db.entry(t.pos, 2)) {
            g_db.h.nxtrec = g_old_db.entry(t.pos, 6);
        } else {
            if (t.keep == 0) {
                // the last step is dropped: its MAR record is where the next step goes
                g_db.h.nxtrec = g_old_db.entry(t.pos, 7);
                g_old_db.entry(t.pos, 5) = 0;
                g_old_db.entry(t.pos, 4) = 0;
                const std::int32_t two = 2;
                chunk_io_old(&two, unit, &g_db.h.nrsets, &t.drec, &t.pos, ierr);
                if (*ierr != 0) {
                    return;
                }
            }
            t.pos = t.pos + 1;
            g_db.h.nrsets = g_db.h.nrsets + 1;
            if (t.pos == 0x81) {
                t.pos = 1;
                g_old_db.index_word(t.drec + 1) = g_db.h.nxtrec;
                g_db.h.nxtrec = g_db.h.nxtrec + 8;
            }
        }
        *slot = g_db.h.nrsets;
    } else {
        if (*slot == 1) {
            g_old_db.index[0] = 4;
            g_db.h.nxtrec = 0xc;
            t.pos = 1;
        } else if (g_db.h.nrsets < *slot) {
            std::int32_t m = 0x80;
            t.pos = dbcore::mod1(slot, &m);
            if (t.pos == 1) {
                t.drec = (*slot - 1) / 128 + 1;
                g_old_db.index_word(t.drec) = g_db.h.nxtrec;
                g_db.h.nxtrec = g_db.h.nxtrec + 8;
            } else {
                chunk_io_old(&one, unit, &g_db.h.nrsets, &t.drec, &t.pos2, ierr);
                if (*ierr != 0) {
                    return;
                }
            }
        } else {
            chunk_io_old(&one, unit, slot, &t.drec, &t.pos, ierr);
            if (*ierr != 0) {
                return;
            }
            g_db.h.nxtrec = g_old_db.entry(t.pos, 6);
        }
        g_db.h.nrsets = *slot;
    }

    const std::int32_t p = t.pos;
    g_old_db.entry(p, 2) = g_db.put_step;
    g_old_db.entry(p, 1) = g_db.put_mesh_no;
    g_old_db.entry(p, 5) = *na;
    g_old_db.entry(p, 4) = *nmar;
    g_old_db.entry(p, 3) = *nh;
    const std::int32_t pm = platform_swap_mode();
    h[5] = static_cast<double>(pm); // 1403bfa94 CVTSI2SD
    g_old_db.entry(p, 6) = g_db.h.nxtrec;
    h[6] = static_cast<double>(*nmar);
    h[7] = static_cast<double>(*na);
    const std::int32_t wm = kWrite;
    if (0 < *nh) {
        t.rec = g_db.h.nxtrec;
        swap8_platform(h, nh);
        real_io(kWrite, unit, h, nh, &t.rec, ierr);
        swap8_platform(h, nh);
        if (*ierr != 0) {
            *ierr = 0x70;
            return;
        }
        g_db.h.nxtrec = t.rec + 1;
    }

    // MAR
    std::int32_t eight = 8;
    g_old_compressed = 0;
    copy_chars(g_db.h.smi1, t.smi, &eight);
    const bool coded = ftn_compare(t.smi, kSmi10, 8, 8) >= 0;
    bool mar_done = false;
    std::int32_t rec = g_db.h.nxtrec;
    std::int32_t cnt = 0;
    if (!coded) {
        g_old_db.entry(p, 7) = g_db.h.nxtrec;
        if (*nmar < 1) {
            mar_done = true;
        } else {
            cnt = *nmar;
        }
    } else {
        g_old_compressed = 1;
        g_old_db.entry(p, 7) = g_db.h.nxtrec;
        if (*nmar < 1) {
            mar_done = true;
        } else {
            t.n = *nmar;
            dbcore::rle_encode_int(nmar, mar, &t.n);
            rec = g_old_db.entry(p, 7);
            g_old_db.entry(p, 4) = t.n;
            cnt = t.n;
        }
    }
    if (!mar_done) {
        t.n = cnt;
        t.rec = rec;
        swap4_platform(mar, &t.n);
        block_io_int(&wm, unit, mar, &t.n, &t.rec, ierr);
        swap4_platform(mar, &t.n);
        if (*ierr != 0) {
            *ierr = 0x71;
            return;
        }
        g_db.h.nxtrec = t.rec + 1;
        if (g_old_compressed == 1) {
            dbcore::rle_decode_int(&t.n, mar, nmar); // the caller's MAR back in plain form
        }
    }

    // A (1403c02c3)
    rec = g_db.h.nxtrec;
    g_old_db.entry(p, 8) = g_db.h.nxtrec;
    if (0 < *na) {
        t.n = *na;
        if (g_old_compressed == 1) {
            dbcore::rle_encode_real(na, a, &t.n);
            rec = g_old_db.entry(p, 8);
            g_old_db.entry(p, 5) = t.n;
        }
        t.rec = rec;
        swap8_platform(a, &t.n);
        real_io(kWrite, unit, a, &t.n, &t.rec, ierr);
        swap8_platform(a, &t.n);
        if (*ierr != 0) {
            *ierr = 0x72;
            return;
        }
        g_db.h.nxtrec = t.rec + 1;
        if (g_old_compressed == 1) {
            dbcore::rle_decode_real(&t.n, a, na);
        }
    }

    // record 1: signature (5 CHARACTER*4 items)
    t.rec = 1;
    std::int32_t five = 5;
    char sig[20];
    std::memcpy(sig, kSig3d230, 20);
    block_io_char4(&wm, unit, sig, &five, &t.rec, ierr);
    if (*ierr != 0) {
        *ierr = 0x6f;
        return;
    }
    // record 2 (32 words)
    t.rec = 2;
    std::int32_t n = 0x1c;
    if (platform_swap_mode() == 1) {
        n = 0x1c;
        swap4_words(g_db.rec2, &n);
    }
    if (platform_swap_mode() == 2) {
        n = 4;
        swap4_words(&g_db.h.istart, &n);
    }
    if (platform_swap_mode() == 3) {
        n = 4;
        swap4_words(&g_db.h.istart, &n);
    }
    n = 0x20;
    block_io_int(&wm, unit, g_db.rec2, &n, &t.rec, ierr);
    if (platform_swap_mode() == 1) {
        n = 0x1c;
        swap4_words(g_db.rec2, &n);
    }
    if (platform_swap_mode() == 2) {
        n = 4;
        swap4_words(&g_db.h.istart, &n);
    }
    if (platform_swap_mode() == 3) {
        n = 4;
        swap4_words(&g_db.h.istart, &n);
    }
    if (*ierr != 0) {
        *ierr = 0x6f;
        return;
    }
    // record 3: index
    t.rec = 3;
    n = 0x80;
    swap4_platform(g_old_db.index, &n);
    n = 0x80;
    block_io_int(&wm, unit, g_old_db.index, &n, &t.rec, ierr);
    n = 0x80;
    swap4_platform(g_old_db.index, &n);
    if (*ierr != 0) {
        *ierr = 0x6f;
        return;
    }
    const std::int32_t two = 2;
    chunk_io_old(&two, unit, &g_db.h.nrsets, &t.drec, &t.pos, ierr);
    if (t.appended == 1) {
        // @src 1403c4590  omitted: process monitor (message after an append)
    }
}

} // namespace

// @src 1403bef07  old-format DB operations: 0 open, 1 directory, 2 step header, 3 MAR and A,
// 4 write a step; any other op is op 0
void db_operation_old(const std::int32_t* unit, std::int32_t* op, std::int32_t* slot, std::int32_t* step,
                      std::int32_t* nh, double* h, std::int32_t* nmar, std::int32_t* mar, std::int32_t* na,
                      double* a, std::int32_t* ierr)
{
    auto& t = g_old_tmp;
    const std::int32_t o = *op;
    t.u83c = 0;
    if (o == 1) {
        old_directory(unit, nmar, mar, ierr);
        return;
    }
    if (o == 2 || o == 3) {
        const std::int32_t one = 1;
        if (0 < *slot) {
            chunk_io_old(&one, unit, slot, &t.drec, &t.pos, ierr);
            if (*ierr != 0) {
                return;
            }
            old_read(unit, op, nh, h, nmar, mar, na, a, ierr);
            return;
        }
        bool found = false;
        bool search_all = true;
        if (0 < g_old_db.cur_rec) {
            // the loaded directory record first (1403bf15d)
            t.first = g_old_db.cur_rec * 0x80 - 0x7f;
            t.count = 0x80;
            search_all = false;
            if (g_db.h.nrsets < g_old_db.cur_rec * 0x80) {
                t.count = g_db.h.nrsets % 0x80;
                t.pos = 1;
                if (t.count < 1) {
                    search_all = true;
                }
            } else {
                t.pos = 1;
            }
            if (!search_all) {
                do {
                    if (*step == g_old_db.entry(t.pos, 2)) {
                        found = true;
                        break;
                    }
                    t.pos = t.pos + 1;
                } while (t.pos <= t.count);
                search_all = !found;
            }
        }
        if (search_all) {
            t.last = 0;
            for (;;) {
                t.first = t.last + 1;
                chunk_io_old(&one, unit, &t.first, &t.drec, &t.pos2, ierr);
                if (*ierr != 0) {
                    return;
                }
                t.last = t.last + 0x80;
                if (g_db.h.nrsets <= t.last) {
                    t.last = g_db.h.nrsets;
                }
                t.count = (t.last - t.first) + 1;
                t.pos = 1;
                if (0 < t.count) {
                    do {
                        if (*step == g_old_db.entry(t.pos, 2)) {
                            found = true;
                            break;
                        }
                        t.pos = t.pos + 1;
                    } while (t.pos <= t.count);
                }
                if (found) {
                    break;
                }
                if (g_db.h.nrsets <= t.last) {
                    *ierr = 0xc9;
                    return;
                }
            }
        }
        *slot = t.first - 1 + t.pos; // 1403bf94d
        old_read(unit, op, nh, h, nmar, mar, na, a, ierr);
        return;
    }
    if (o == 4) {
        old_write(unit, slot, step, nh, h, nmar, mar, na, a, ierr);
        return;
    }
    old_open(unit, ierr);
}

// @src 1403dc957  open `file` on unit 3, read A region *region of step slot *slot, close
void read_a_region_from_file(const char* file, const std::int32_t* nchar, std::int32_t* slot, std::int32_t* nh,
                             double* h, std::int32_t* nmar, std::int32_t* mar, std::int32_t* na, double* a,
                             const std::int32_t* region, const std::int32_t* version, std::int32_t* ierr,
                             const std::int32_t* iprint)
{
    if (*iprint == 1) {
        // 1403dce95: the name with max(*nchar, 0) characters; str 140bb0b00
        const std::int32_t n = *nchar < 0 ? 0 : *nchar;
        ftn_write_text2("OPENING DATABASE FILE = ", file, n);
    }
    char status[4];
    gendb::char_assign(status, "OLD ", 4, 4); // 1404b33e4; str 140bb0b20
    const std::int32_t open_mode = 5;
    g_rdarya.unit = 3;
    dbcore::fortran_open(&open_mode, &g_rdarya.unit, file, status, ierr, 200, 3); // 140407e81
    if (*ierr != 0) {
        *ierr = 99;
        if (*iprint == 1) {
            // str 140bb0b30, first 0x20 characters
            dbcore::detail::ftn_write_text_int("ERROR IN OPENING DATABASE FILE =", *ierr);
        }
    } else {
        char* last = last_db_file();
        if (ftn_compare(file, last, 200, 200) != 0) {
            // another file than last time: op 0 (header, index) first
            std::int32_t op = 0;
            std::int32_t s0 = 0;
            std::int32_t st0 = 0;
            std::int32_t nh0 = 0;
            std::int32_t nmar0 = 0;
            std::int32_t na0 = 0;
            dbcore::db_operation(&g_rdarya.unit, &op, &s0, &st0, &nh0, h, &nmar0, mar, &na0, a, ierr);
            gendb::char_assign(last, file, 200, 200); // 1404b33e4
        }
        if (*version < 0x8fd) {
            std::int32_t op = 3;
            db_operation_old(&g_rdarya.unit, &op, slot, &g_rdarya.step, nh, h, nmar, mar, na, a, ierr);
        } else {
            // 1403dcae0: NEG / CMOVS = abs (INT_MIN stays), + 30: op 3 for the A region only
            const std::int32_t r = *region;
            std::int32_t v = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(r));
            if (v < 0) {
                v = r;
            }
            g_rdarya.op = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + 0x1eu);
            dbcore::db_operation(&g_rdarya.unit, &g_rdarya.op, slot, &g_rdarya.step, nh, h, nmar, mar, na, a,
                                 ierr);
        }
        if (*ierr != 0 && *iprint == 1) {
            dbcore::detail::ftn_write_text_int("ERROR READING A ARRAY REGION", *ierr); // str 140bb0e60
        }
    }
    if (*iprint == 1) {
        dbcore::detail::ftn_write_text("END OF READING DATABASE A ARRAY REGION"); // str 140bb0e80
    }
    close_unit(&g_rdarya.unit); // 1404b53a4
}

} // namespace def::dbio::dbread
