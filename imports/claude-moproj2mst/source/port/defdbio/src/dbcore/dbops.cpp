// DBOperations23: dispatcher and ops 0..4 of datarm23. See dbcore/dbops.hpp.
#include <defdbio/dbcore/dbops.hpp>

#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/dbcore/util.hpp>
#include <defrt/ftn/helpers.hpp>

#include "internal.hpp"

#include <algorithm>
#include <vector>

namespace def::dbio::dbcore {

namespace {

using def::rt::ftn::copy_words4;
using def::rt::ftn::copy_words8;
using def::rt::ftn::ftn_strncmp_b;

constexpr std::int32_t kRead = 1;
constexpr std::int32_t kWrite = 2;
constexpr std::int32_t kEntriesPerIndex = 0x3f80; // 127 chunks x 128 entries
constexpr std::int32_t kChunkWords = 0x680;
constexpr std::int32_t kMarTableWords = 0xea;
constexpr std::int32_t kATableWords = 0x1fa;

// DAT_140613570 / DAT_140613574: the selector values "all" and "none".
constexpr std::int32_t kSelAll = 0;
constexpr std::int32_t kSelNone = -1;

// Word i (1-based) of a region table that starts at `table` (T(1) = count).
std::int32_t& tword(std::int32_t* table, std::int32_t i) { return table[i - 1]; }

// Index-record word that holds the record of chunk `c`, as FUN_1403b92bc finds it: index record
// j covers chunks 127(j-1)+1 .. 127j; the 200th takes every chunk beyond (its position then runs
// past word 127 into what follows, as in the original).
std::int32_t& chunk_record_word(std::int32_t c)
{
    std::int32_t lim = 0x7f;
    std::int32_t j = 1;
    for (;;) {
        if (c <= lim) {
            return g_db.index_word(j, (c - lim) + 0x7f);
        }
        if (j == 200) {
            return g_db.index_word(200, (c - lim) + 0x7f);
        }
        j = j + 1;
        lim = lim + 0x7f;
    }
}

// Swap n words of `p` in place when the platform mode asks for it (mode 2 or 3).
void swap4_platform(void* p, std::int32_t n)
{
    if (platform_swap_mode() == 2) {
        swap4_words(p, &n);
    }
    if (platform_swap_mode() == 3) {
        swap4_words(p, &n);
    }
}
void swap8_platform(void* p, std::int32_t n)
{
    if (platform_swap_mode() == 2) {
        swap8_words(p, &n);
    }
    if (platform_swap_mode() == 3) {
        swap8_words(p, &n);
    }
}
// After a READ: swap when the file's mode (FUN_140407e60) says so; `only_if_ok` for the reads
// that test the error first.
void swap4_file(void* p, std::int32_t n, const std::int32_t* ierr, bool only_if_ok)
{
    if ((!only_if_ok || *ierr == 0) && swap_mode() == 2) {
        swap4_words(p, &n);
    }
    if ((!only_if_ok || *ierr == 0) && swap_mode() == 3) {
        swap4_words(p, &n);
    }
}
void swap8_file(void* p, std::int32_t n)
{
    if (swap_mode() == 2) {
        swap8_words(p, &n);
    }
    if (swap_mode() == 3) {
        swap8_words(p, &n);
    }
}

// REAL*8 transfer: as REAL*8 on a mode-1 platform, else as 2n INTEGER words.
void real_io(std::int32_t mode, const std::int32_t* unit, void* buf, const std::int32_t* n,
             std::int32_t* rec, std::int32_t* ierr)
{
    if (platform_swap_mode() == 1) {
        block_io_real8(&mode, unit, buf, n, rec, ierr);
    } else {
        const std::int32_t n2 = *n * 2;
        block_io_int(&mode, unit, buf, &n2, rec, ierr);
    }
}

// Writes the ints of `p` (n words) swapped to disk order at *rec and swaps them back.
void write_ints_swapped(const std::int32_t* unit, void* p, std::int32_t n, std::int32_t* rec,
                        std::int32_t* ierr)
{
    swap4_platform(p, n);
    block_io_int(&kWrite, unit, p, &n, rec, ierr);
    swap4_platform(p, n);
}

// The step-limit message of FUN_1403b7729 (WRITE(6,*)).
void step_limit_error(std::int32_t* ierr)
{
    detail::ftn_write_text("ERROR: Step limit exceeded!!");
    *ierr = 1;
}

// Finds the entry of step number `step` (or of `step` == 0 for op 5): first the chunk in memory,
// then every chunk from the first. Returns false with *ierr set on a read error or 0xc9.
// part of @src 1403b6679 (and the same code in 1403bcf76)
bool find_step(const std::int32_t* unit, std::int32_t step, std::int32_t* slot, std::int32_t* pos,
               std::int32_t* ierr)
{
    std::int32_t base;
    std::int32_t cnt;
    if (0 < g_db.cur_chunk) {
        base = g_db.cur_chunk * 0x80 - 0x7f;
        cnt = 0x80;
        bool search = true;
        if (g_db.h.nrsets < g_db.cur_chunk * 0x80) {
            cnt = g_db.h.nrsets % 0x80;
            search = 0 < cnt;
        }
        if (search) {
            for (std::int32_t p = 1; p <= cnt; ++p) {
                if (step == g_db.entry(p, 2)) {
                    *pos = p;
                    *slot = base - 1 + p;
                    return true;
                }
            }
        }
    }
    std::int32_t done = 0;
    for (;;) {
        base = done + 1;
        std::int32_t cnum = 0;
        std::int32_t p0 = 0;
        chunk_io(&kRead, unit, &base, &cnum, &p0, ierr);
        if (*ierr != 0) {
            return false;
        }
        done = done + 0x80;
        if (g_db.h.nrsets <= done) {
            done = g_db.h.nrsets;
        }
        cnt = (done - base) + 1;
        for (std::int32_t p = 1; p <= cnt; ++p) {
            if (step == g_db.entry(p, 2)) {
                *pos = p;
                *slot = base - 1 + p;
                return true;
            }
        }
        if (g_db.h.nrsets <= done) {
            *ierr = 0xc9;
            return false;
        }
    }
}

} // namespace

// part of @src 1403b6679, used by op 5 as well
bool find_step_entry(const std::int32_t* unit, std::int32_t step, std::int32_t* slot,
                     std::int32_t* pos, std::int32_t* ierr)
{
    return find_step(unit, step, slot, pos, ierr);
}

// @src 1403b5a04  DBOperations23 dispatcher
// @src 1403b5a04  omitted: licence (entry check FUN_1400d3410, "License Error(1)" and STOP)
void db_operation(const std::int32_t* unit, std::int32_t* op, std::int32_t* slot,
                  std::int32_t* step, std::int32_t* nh, double* h, std::int32_t* nmar,
                  std::int32_t* mar, std::int32_t* na, double* a, std::int32_t* ierr)
{
    const std::int32_t o = *op;
    g_db.sel_mar = kSelAll;
    g_db.sel_a = kSelAll;
    if (o < -0x1d) {
        *op = 3;
        g_db.sel_mar = -(o + 0x1e);
        if (g_db.sel_mar < 0) {
            g_db.sel_mar = o + 0x1e;
        }
        g_db.sel_a = kSelNone;
    } else if (o < 0x1e) {
        switch (o) {
        case 1:
            op_directory(unit, nmar, mar, ierr);
            return;
        case 2:
        case 3:
            break;
        case 4:
            op_write_step(unit, slot, step, nh, h, nmar, mar, na, a, ierr);
            return;
        case 5:
            op_read_dynamic(unit, slot, ierr);
            return;
        default: // 0, and every other op below 30
            op_open(unit, ierr);
            return;
        }
    } else {
        g_db.sel_a = o - 0x1e;
        *op = 3;
        g_db.sel_mar = kSelNone;
    }
    op_read_step(unit, op, slot, step, nh, h, nmar, mar, na, a, ierr);
}

// @src 1403b41d9  read record 2 and the signature
void read_file_header(const std::int32_t* unit, std::int32_t* ierr)
{
    std::int32_t rec = 2; // _DAT_1407073ac
    g_db.cur_chunk = 0;
    std::int32_t m = platform_swap_mode();
    set_swap_mode(&m);
    const std::int32_t n34 = 0x22;
    block_io_int(&kRead, unit, g_db.rec2, &n34, &rec, ierr);
    if (platform_swap_mode() == 1) {
        const std::int32_t n = 0x1c;
        swap4_words(g_db.rec2, &n);
    }
    // ISMTYP is still in disk order here; either way the mode is 2 on this platform.
    if (g_db.h.ismtyp < 0 || 3 < g_db.h.ismtyp) {
        m = 2;
        set_swap_mode(&m);
    } else {
        m = platform_swap_mode();
        set_swap_mode(&m);
    }
    if (swap_mode() == 2) {
        const std::int32_t n = 6;
        swap4_words(&g_db.h.istart, &n);
    }
    if (swap_mode() == 3) {
        const std::int32_t n = 6;
        swap4_words(&g_db.h.istart, &n);
    }
    // The checks below clear *ierr, so a failed read of record 2 is not reported (the block
    // keeps what it held before).
    std::int32_t yes = 0;
    check_sig_2d(unit, &yes, ierr);
    if (*ierr != 0) {
        *ierr = 1;
        return;
    }
    if (yes == 1) {
        g_db.file_family = 2;
    } else {
        check_sig_3d(unit, &yes, ierr);
        if (*ierr != 0) {
            *ierr = 1;
            return;
        }
        if (yes == 1) {
            g_db.file_family = 3;
        } else {
            check_sig_23(unit, &yes, ierr);
            if (*ierr != 0) {
                *ierr = 1;
                return;
            }
            if (yes == 1) {
                g_db.file_family = 0x17;
            }
        }
    }
    std::int32_t v = 0;
    db_version(&v);
    g_db.file_version = v;
}

// @src 1403b600f  op 0: header, version range of the legacy families, index records
void op_open(const std::int32_t* unit, std::int32_t* ierr)
{
    g_db.cur_chunk = 0;
    read_file_header(unit, ierr);
    // 1403b6047: the test (ISMTYP < 0 && 3 < ISMTYP) can never hold.
    if (*ierr != 0) {
        *ierr = 0x65;
        return;
    }
    const std::int32_t len = 8; // _DAT_14070740c
    std::int32_t lo = 0;        // DAT_140707410
    std::int32_t hi = 0;        // DAT_140707414
    bool check = true;
    if (g_db.file_family == 2) {
        ftn_strncmp_b(g_db.h.smi1, " 3.1.1  ", &len, &lo); // str 140616e30
        ftn_strncmp_b(g_db.h.smi1, " 5.0    ", &len, &hi); // str 140616e40
    } else if (g_db.file_family == 3) {
        ftn_strncmp_b(g_db.h.smi1, " 2.0.1  ", &len, &lo); // str 140616e50
        ftn_strncmp_b(g_db.h.smi1, " 2.4    ", &len, &hi); // str 140616e60
    } else {
        check = false;
    }
    if (check) {
        if (lo < 0) {
            *ierr = 100;
            return;
        }
        if (hi < 0) {
            *ierr = 0x12d;
            return;
        }
    }
    // Index records: the first is record 3, each next one is word 128 of the one before. A read
    // error does not stop the loop, and each read clears *ierr first.
    std::int32_t k = 1;
    std::int32_t first = 0;
    std::int32_t rec = 0; // _DAT_140707420
    do {
        if (g_db.h.nrsets <= first) {
            return;
        }
        rec = k == 1 ? 3 : g_db.index_word(k - 1, 0x80);
        const std::int32_t n = 0x80;
        std::int32_t* dst = &g_db.index_word(k, 1);
        block_io_int(&kRead, unit, dst, &n, &rec, ierr);
        swap4_file(dst, 0x80, ierr, true);
        if (*ierr != 0) {
            *ierr = 0x65;
        }
        k = k + 1;
        first = first + kEntriesPerIndex;
    } while (k < 0xc9);
}

// @src 1403b6425  op 1: words 1..10 of entries 1..*count (0: all) into out, 10 per entry
void op_directory(const std::int32_t* unit, std::int32_t* count, std::int32_t* out,
                  std::int32_t* ierr)
{
    if (*count == 0) {
        *count = g_db.h.nrsets;
    }
    std::int32_t done = 0; // DAT_140707428
    for (;;) {
        std::int32_t first = done + 1; // DAT_14070742c
        std::int32_t cnum = 0;
        std::int32_t pos = 0;
        chunk_io(&kRead, unit, &first, &cnum, &pos, ierr);
        if (*ierr != 0) {
            return;
        }
        done = done + 0x80;
        if (*count <= done) {
            done = *count;
        }
        const std::int32_t n = (done - first) + 1;
        std::int32_t k = first * 10 - 10; // DAT_14070743c
        for (std::int32_t e = 1; e <= n; ++e) {
            for (std::int32_t w = 1; w <= 10; ++w) {
                k = k + 1;
                out[k - 1] = g_db.entry(e, w);
            }
        }
        if (*count <= done) {
            return;
        }
    }
}

// @src 1403b92bc  load or write the directory chunk of an entry
void chunk_io(const std::int32_t* mode, const std::int32_t* unit, const std::int32_t* slot,
              std::int32_t* cnum, std::int32_t* pos, std::int32_t* ierr)
{
    const std::int32_t per = 0x80;
    *ierr = 0;
    *pos = mod1(slot, &per);
    const std::int32_t c = (*slot - 1) / 0x80 + 1;
    *cnum = c;
    const std::int32_t n = kChunkWords;
    if (*mode == 2) {
        std::int32_t rec = chunk_record_word(c); // _DAT_1407074b0
        g_db.cur_chunk = c;
        swap4_platform(g_db.chunk, n);
        block_io_int(&kWrite, unit, g_db.chunk, &n, &rec, ierr);
        swap4_platform(g_db.chunk, n);
        if (*ierr != 0) {
            *ierr = 0x6f;
        }
    } else if (g_db.cur_chunk != c) {
        std::int32_t rec = chunk_record_word(c);
        g_db.cur_chunk = c; // set before the read: after a failed read the chunk counts as loaded
        block_io_int(&kRead, unit, g_db.chunk, &n, &rec, ierr);
        swap4_file(g_db.chunk, n, ierr, true);
        if (*ierr != 0) {
            *ierr = 0x65;
        }
    }
}

// @src 1403b9bef  MAR region table of entry *pos into g_db.mar_regions (file version >= 6000)
void read_mar_table(const std::int32_t* unit, const std::int32_t* pos, std::int32_t* ierr)
{
    std::int32_t v = 0;
    db_version(&v);
    if (5999 < v) {
        std::int32_t n = g_db.entry(*pos, 4);
        std::int32_t rec = g_db.entry(*pos, 12);
        if (n != 0) {
            block_io_int(&kRead, unit, g_db.mar_regions, &n, &rec, ierr);
            swap4_file(g_db.mar_regions, n, ierr, false);
            if (*ierr != 0) {
                *ierr = 0x66;
            }
        }
    }
}

// @src 1403badb1  A region table into g_db.a_regions
void read_a_table(const std::int32_t* unit, const std::int32_t* pos, std::int32_t* ierr)
{
    std::int32_t v = 0;
    db_version(&v);
    if (5999 < v) {
        std::int32_t n = g_db.entry(*pos, 5);
        std::int32_t rec = g_db.entry(*pos, 13);
        if (n != 0) {
            block_io_int(&kRead, unit, g_db.a_regions, &n, &rec, ierr);
            swap4_file(g_db.a_regions, n, ierr, false);
            if (*ierr != 0) {
                *ierr = 0x66;
            }
        }
    }
}

namespace {

// Number of the last region that stores data (DAT_14070755c / 65c), the region count if none.
std::int32_t last_stored_region(std::int32_t* t)
{
    const std::int32_t nreg = tword(t, 1);
    std::int32_t last = nreg;
    for (std::int32_t k = 1; k <= nreg; ++k) {
        if (0 < tword(t, 4 * k + 1)) {
            last = k;
        }
    }
    return last;
}

} // namespace

// @src 1403b9d4c  MAR regions. Table: T(1) count, T(2) largest length, region k at T(4k-1..4k+2)
// = decoded length, start address, stored words, record.
void read_mar_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* sel,
                      std::int32_t* mar, const std::int32_t* compressed, std::int32_t* ierr)
{
    if (*sel == kSelNone) {
        return;
    }
    read_mar_table(unit, pos, ierr); // its error is not looked at
    std::int32_t* t = g_db.mar_regions;
    const std::int32_t nreg = tword(t, 1);
    std::int32_t last = nreg;
    if (*sel == kSelAll) {
        last = last_stored_region(t);
    }
    for (std::int32_t k = 1; k <= nreg; ++k) {
        const std::int32_t len = tword(t, 4 * k - 1);
        std::int32_t start = tword(t, 4 * k);
        std::int32_t stored = tword(t, 4 * k + 1);
        std::int32_t rec = tword(t, 4 * k + 2);
        if (stored == 0) {
            continue;
        }
        if (*sel != kSelAll) {
            if (*sel != start) {
                continue;
            }
            start = 1;
        }
        std::int32_t* dst = mar + (start - 1);
        // 1403b9e8a: for the last region with more stored than decoded words the original
        // ALLOCATEs a scratch array and never uses it, reading in place, so the stored words
        // run past the region's end. The port reads that one region through scratch memory and
        // copies the decoded words: the region gets the same values and nothing past it is
        // written (the original's extra words lie beyond the array).
        const bool via_scratch = len < stored && *sel == kSelAll && last == k;
        std::vector<std::int32_t> scratch;
        std::int32_t* rd = dst;
        if (via_scratch) {
            scratch.assign(static_cast<std::size_t>(stored), 0);
            rd = scratch.data();
        }
        block_io_int(&kRead, unit, rd, &stored, &rec, ierr);
        swap4_file(rd, stored, ierr, false);
        if (*ierr != 0) {
            *ierr = 0x67;
            break;
        }
        std::int32_t out = stored; // DAT_140707574
        if (*compressed == 1) {
            rle_decode_int(&stored, rd, &out);
        }
        if (via_scratch) {
            copy_words4(rd, dst, &out);
        }
        if (*sel != kSelAll) {
            break;
        }
    }
}

// @src 1403baf0e  A regions; the last region with more stored than decoded words goes through
// an ALLOCATEd scratch array (FUN_1404b62c4) and is copied with FUN_1402380c5.
void read_a_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* sel,
                    double* a, const std::int32_t* compressed, std::int32_t* ierr)
{
    if (*sel == kSelNone) {
        return;
    }
    read_a_table(unit, pos, ierr);
    std::int32_t* t = g_db.a_regions;
    const std::int32_t nreg = tword(t, 1);
    std::int32_t last = nreg;
    if (*sel == kSelAll) {
        last = last_stored_region(t);
    }
    std::int32_t tmp_words = 0; // DAT_140707658
    std::vector<double> scratch;
    for (std::int32_t k = 1; k <= nreg; ++k) {
        const std::int32_t len = tword(t, 4 * k - 1);
        std::int32_t start = tword(t, 4 * k);
        std::int32_t stored = tword(t, 4 * k + 1);
        std::int32_t rec = tword(t, 4 * k + 2);
        if (stored == 0) {
            continue;
        }
        if (*sel != kSelAll) {
            if (*sel != start) {
                continue;
            }
            start = 1;
        }
        if (len < stored && tmp_words == 0 && *sel == kSelAll && last == k) {
            scratch.assign(static_cast<std::size_t>(stored), 0.0);
            tmp_words = tmp_words + stored * 2;
        }
        double* dst = a + (start - 1);
        double* rd = tmp_words < 1 ? dst : scratch.data();
        real_io(kRead, unit, rd, &stored, &rec, ierr);
        swap8_file(rd, stored);
        if (*ierr != 0) {
            *ierr = 0x68;
            break;
        }
        std::int32_t out = stored; // DAT_140707674
        if (*compressed == 1) {
            rle_decode_real(&stored, rd, &out);
        }
        if (0 < tmp_words) {
            copy_words8(rd, dst, &out);
        }
        if (*sel != kSelAll) {
            break;
        }
    }
}

// @src 1403b6679  ops 2 (step header) and 3 (MAR and A)
void op_read_step(const std::int32_t* unit, const std::int32_t* op, std::int32_t* slot,
                  const std::int32_t* step, std::int32_t* nh, double* h, std::int32_t* nmar,
                  std::int32_t* mar, std::int32_t* na, double* a, std::int32_t* ierr)
{
    std::int32_t pos = 0; // DAT_140707454
    if (*slot < 1) {
        if (!find_step(unit, *step, slot, &pos, ierr)) {
            return;
        }
    } else {
        std::int32_t cnum = 0;
        chunk_io(&kRead, unit, slot, &cnum, &pos, ierr);
        if (*ierr != 0) {
            return;
        }
    }
    if (*op == 2) {
        if (*nh < 1) {
            *nh = g_db.entry(pos, 3);
        }
        std::int32_t rec = g_db.entry(pos, 11); // _DAT_140707464
        real_io(kRead, unit, h, nh, &rec, ierr);
        swap8_file(h, *nh);
        if (*ierr != 0) {
            *ierr = 0x66;
        }
        if (g_db.file_family == 0x17) {
            g_db.step_family = g_db.entry(pos, 8);
            g_db.step_version = g_db.entry(pos, 9);
            return;
        }
        g_db.step_family = g_db.file_family;
        g_db.step_version = g_db.file_version;
        return;
    }
    if (*op != 3) {
        *ierr = 0x12d;
        return;
    }
    if (g_db.file_family == 0x17) {
        g_db.step_family = g_db.entry(pos, 8);
        g_db.step_version = g_db.entry(pos, 9);
    } else {
        g_db.step_family = g_db.file_family;
        g_db.step_version = g_db.file_version;
    }
    const std::int32_t mar_size = g_db.entry(pos, 4);
    if (!(0 < mar_size && 0 < g_db.entry(pos, 5))) {
        *ierr = 0xca; // a header-only step
        return;
    }
    if (g_db.step_family == 0 || g_db.step_version == 0) {
        *ierr = 0x1f9;
        return;
    }
    std::int32_t compressed = 0; // DAT_140707468
    std::int32_t out = 0;        // DAT_14070746c
    const auto read_mar_whole = [&]() -> bool {
        if (*nmar < 1) {
            *nmar = mar_size;
        }
        std::int32_t rec = g_db.entry(pos, 12);
        block_io_int(&kRead, unit, mar, nmar, &rec, ierr);
        swap4_file(mar, *nmar, ierr, false);
        if (*ierr != 0) {
            *ierr = 0x67;
            return false;
        }
        if (compressed == 1) {
            rle_decode_int(nmar, mar, &out);
            *nmar = out;
        }
        return true;
    };
    const auto read_a_whole = [&]() {
        if (*na < 1) {
            *na = g_db.entry(pos, 5);
        }
        std::int32_t rec = g_db.entry(pos, 13);
        real_io(kRead, unit, a, na, &rec, ierr);
        swap8_file(a, *na);
        if (*ierr != 0) {
            *ierr = 0x68;
            return;
        }
        if (compressed == 1) {
            rle_decode_real(na, a, &out);
            *na = out;
        }
    };
    if (g_db.step_family == 2) {
        compressed = 0xc25 < g_db.step_version ? 1 : 0;
        if (!read_mar_whole()) {
            return;
        }
        read_a_whole();
        return;
    }
    if (g_db.step_family != 3) {
        detail::ftn_write_text("Internal error on loading a step data");
        *ierr = 0x2c3;
        return;
    }
    compressed = 999 < g_db.step_version ? 1 : 0;
    if (g_db.step_version < 6000) {
        if (!read_mar_whole()) {
            return;
        }
    } else if (-1 < g_db.sel_mar) {
        read_mar_regions(unit, &pos, &g_db.sel_mar, mar, &compressed, ierr);
        if (g_db.sel_mar == kSelAll) {
            *nmar = detail::nint(h[6]); // H(7): full MAR length
        }
    }
    if (g_db.step_version < 6000) {
        read_a_whole();
    } else if (-1 < g_db.sel_a) {
        read_a_regions(unit, &pos, &g_db.sel_a, a, &compressed, ierr);
        if (g_db.sel_a == kSelAll) {
            *na = detail::nint(h[7]); // H(8): full A length
        }
    }
}

// @src 1403b44d2  stamp entries with the legacy family and write the combined signature
// (strings 140616750, 140616770; DAT_140616790 = 1000)
void convert_to_combined(const std::int32_t* unit, std::int32_t* ierr)
{
    *ierr = 0;
    if (0 < g_db.h.nrsets) {
        if (g_db.file_family == 0 || g_db.file_version == 0) {
            read_file_header(unit, ierr);
            if (*ierr != 0) {
                *ierr = 1;
                return;
            }
        }
        if (g_db.file_family == 0) {
            *ierr = 1;
            return;
        }
        if (g_db.file_family != 0x17) {
            g_db.cur_chunk = 0;
            std::int32_t done = 0; // DAT_1407073bc
            do {
                std::int32_t first = done + 1;
                std::int32_t cnum = 0;
                std::int32_t pos = 0;
                chunk_io(&kRead, unit, &first, &cnum, &pos, ierr);
                if (*ierr != 0) {
                    *ierr = 1;
                    return;
                }
                done = done + 0x80;
                if (g_db.h.nrsets <= done) {
                    done = g_db.h.nrsets;
                }
                const std::int32_t n = (done - first) + 1;
                for (std::int32_t e = 1; e <= n; ++e) {
                    g_db.entry(e, 8) = g_db.file_family;
                    g_db.entry(e, 9) = g_db.file_version;
                }
                chunk_io(&kWrite, unit, &first, &cnum, &pos, ierr);
                if (*ierr != 0) {
                    *ierr = 1;
                    return;
                }
            } while (done < g_db.h.nrsets);
        }
    }
    std::int32_t rec = 1; // _DAT_1407073d4
    const std::int32_t n5 = 5;
    char sig[21];
    std::memcpy(sig, g_db.step_family == 2 ? "SFTC-DEF-2D-DB100-00" : "SFTC-DEF-3D-DB100-00", 21);
    block_io_char4(&kWrite, unit, sig, &n5, &rec, ierr);
    g_db.file_family = 0x17;
    g_db.file_version = 1000 * 10;
}

// @src 1403ba54c  MAR regions of a 3D step (version >= 6000): table, then each region (copied
// to scratch, compressed there), then the table again with the stored sizes and records
void write_mar_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* nh,
                       const std::int32_t* nmar, const std::int32_t* mar,
                       const std::int32_t* compressed, std::int32_t* ierr)
{
    (void)nh;
    const std::int32_t r0 = g_db.h.nxtrec; // _DAT_1407075d4
    const std::int32_t e = *pos;
    if (*nmar < 1) {
        g_db.entry(e, 4) = 0;
        g_db.entry(e, 12) = r0;
        return;
    }
    const std::int32_t saved = g_db.h.nxtrec; // local_58
    g_db.entry(e, 4) = kMarTableWords;
    g_db.entry(e, 12) = r0;
    std::int32_t rec = r0; // DAT_1407075d8
    write_ints_swapped(unit, g_db.mar_regions, kMarTableWords, &rec, ierr);
    if (*ierr != 0) {
        *ierr = 0x73;
        return;
    }
    g_db.h.nxtrec = rec + 1;
    std::int32_t* t = g_db.mar_regions;
    // ALLOCATE(scratch(T(2)+2)); the port sizes it to the region at hand when that is larger.
    std::vector<std::int32_t> scratch(static_cast<std::size_t>(std::max(tword(t, 2) + 2, 0)));
    const std::int32_t nreg = tword(t, 1);
    for (std::int32_t k = 1; k <= nreg; ++k) {
        const std::int32_t start = tword(t, 4 * k);
        std::int32_t len = tword(t, 4 * k - 1);
        if (len == 0) {
            tword(t, 4 * k + 1) = 0;
            tword(t, 4 * k + 2) = g_db.h.nxtrec;
            continue;
        }
        if (static_cast<std::int64_t>(scratch.size()) < static_cast<std::int64_t>(len) + 2) {
            scratch.resize(static_cast<std::size_t>(len) + 2);
        }
        copy_words4(mar + (start - 1), scratch.data(), &len);
        if (*compressed == 1) {
            std::int32_t st = 0; // DAT_1407075ec
            rle_encode_int(&len, scratch.data(), &st);
            len = st;
            tword(t, 4 * k + 1) = st;
        }
        rec = g_db.h.nxtrec;
        tword(t, 4 * k + 2) = g_db.h.nxtrec;
        swap4_platform(scratch.data(), len);
        block_io_int(&kWrite, unit, scratch.data(), &len, &rec, ierr);
        if (*ierr != 0) {
            *ierr = 0x71;
            return;
        }
        g_db.h.nxtrec = rec + 1;
    }
    rec = saved;
    write_ints_swapped(unit, g_db.mar_regions, kMarTableWords, &rec, ierr);
    if (*ierr != 0) {
        *ierr = 0x73;
    }
}

// @src 1403bb996  A regions of a 3D step (version >= 6000)
void write_a_regions(const std::int32_t* unit, const std::int32_t* pos, const std::int32_t* nh,
                     const std::int32_t* na, const double* a, const std::int32_t* compressed,
                     std::int32_t* ierr)
{
    (void)nh;
    const std::int32_t r0 = g_db.h.nxtrec;
    const std::int32_t e = *pos;
    if (*na < 1) {
        g_db.entry(e, 5) = 0;
        g_db.entry(e, 13) = r0;
        return;
    }
    const std::int32_t saved = g_db.h.nxtrec;
    g_db.entry(e, 5) = kATableWords;
    g_db.entry(e, 13) = r0;
    std::int32_t rec = r0;
    write_ints_swapped(unit, g_db.a_regions, kATableWords, &rec, ierr);
    if (*ierr != 0) {
        *ierr = 0x73;
        return;
    }
    g_db.h.nxtrec = rec + 1;
    std::int32_t* t = g_db.a_regions;
    // ALLOCATE(scratch(2*T(2)+4)) in 4-byte words = T(2)+2 doubles.
    std::vector<double> scratch(static_cast<std::size_t>(std::max(tword(t, 2) + 2, 0)));
    const std::int32_t nreg = tword(t, 1);
    for (std::int32_t k = 1; k <= nreg; ++k) {
        std::int32_t len = tword(t, 4 * k - 1);
        const std::int32_t start = tword(t, 4 * k);
        if (len == 0) {
            tword(t, 4 * k + 1) = 0;
            tword(t, 4 * k + 2) = g_db.h.nxtrec;
            continue;
        }
        if (static_cast<std::int64_t>(scratch.size()) < static_cast<std::int64_t>(len) + 2) {
            scratch.resize(static_cast<std::size_t>(len) + 2);
        }
        copy_words8(a + (start - 1), scratch.data(), &len);
        if (*compressed == 1) {
            std::int32_t st = 0; // DAT_1407076ec
            rle_encode_real(&len, scratch.data(), &st);
            len = st;
            tword(t, 4 * k + 1) = st;
        }
        rec = g_db.h.nxtrec;
        tword(t, 4 * k + 2) = g_db.h.nxtrec;
        swap8_platform(scratch.data(), len);
        real_io(kWrite, unit, scratch.data(), &len, &rec, ierr);
        if (*ierr != 0) {
            *ierr = 0x72;
            return;
        }
        g_db.h.nxtrec = rec + 1;
    }
    rec = saved;
    write_ints_swapped(unit, g_db.a_regions, kATableWords, &rec, ierr);
    if (*ierr != 0) {
        *ierr = 0x73;
    }
}

namespace {

// Puts the record `nxtrec` of a new directory chunk number `cnum + 1` into the index records
// (the next index record when record j is full). `cnum` here is compared with `lim` (append
// path of FUN_1403b7729) or `lim2 = lim + 1` (explicit slot path); both forms are kept.
// part of @src 1403b7729
void index_new_chunk_append(std::int32_t cnum, std::int32_t* ierr)
{
    std::int32_t lim = 0x7f;
    for (std::int32_t j = 1; j < 0xc9; ++j, lim += 0x7f) {
        if (cnum < lim) {
            g_db.index_word(j, (cnum - lim) + 0x80) = g_db.h.nxtrec;
            return;
        }
        if (cnum == lim) {
            if (j < 200) {
                g_db.index_word(j, 0x80) = g_db.h.nxtrec;
                g_db.h.nxtrec = g_db.h.nxtrec + 1;
                g_db.index_word(j + 1, 1) = g_db.h.nxtrec;
            } else {
                step_limit_error(ierr);
            }
            return;
        }
    }
}

// part of @src 1403b7729
void index_new_chunk_at(std::int32_t cnum, std::int32_t* ierr)
{
    std::int32_t lim = 0x7f;
    std::int32_t lim2 = 0x80;
    for (std::int32_t j = 1; j < 0xc9; ++j, lim += 0x7f, lim2 += 0x7f) {
        if (cnum < lim2) {
            g_db.index_word(j, (cnum - lim) + 0x7f) = g_db.h.nxtrec;
            return;
        }
        if (cnum == lim2) {
            if (j < 200) {
                g_db.index_word(j, 0x80) = g_db.h.nxtrec;
                g_db.h.nxtrec = g_db.h.nxtrec + 1;
                g_db.index_word(j + 1, 1) = g_db.h.nxtrec;
            } else {
                step_limit_error(ierr);
            }
            return;
        }
    }
}

} // namespace

// @src 1403b7729  op 4: write a step (append with *slot < 1, else into entry *slot)
// @src 1403b7729  omitted: licence (check before the directory write, "License Error(2)")
// @src 1403b7729  omitted: process monitor (FUN_1403c4590 message after an append)
void op_write_step(const std::int32_t* unit, std::int32_t* slot, const std::int32_t* step,
                   std::int32_t* nh, double* h, std::int32_t* nmar, std::int32_t* mar,
                   std::int32_t* na, double* a, std::int32_t* ierr)
{
    std::int32_t cnum = 0; // DAT_140707480
    std::int32_t pos = 0;  // DAT_140707484
    std::int32_t m = platform_swap_mode();
    set_swap_mode(&m);
    if (*slot < 1) {
        std::int32_t keep = 1;
        // A zero step interval divides by zero here, as in the original.
        if (0 < (*step - 1) % g_db.put_interval) {
            keep = 0;
        }
        if (g_db.h.istart == g_db.h.nrsets) {
            keep = 1;
        }
        chunk_io(&kRead, unit, &g_db.h.nrsets, &cnum, &pos, ierr);
        if (*ierr != 0) {
            return;
        }
        if (g_db.entry(pos, 2) == *step) {
            // The last entry is this step again: write over it from its header record on.
            g_db.h.nxtrec = g_db.entry(pos, 11);
        } else {
            if (keep == 0) {
                // The previous step was only kept until now: drop its arrays and reuse them.
                g_db.h.nxtrec = g_db.entry(pos, 12);
                g_db.entry(pos, 5) = 0;
                g_db.entry(pos, 4) = 0;
                chunk_io(&kWrite, unit, &g_db.h.nrsets, &cnum, &pos, ierr);
                if (*ierr != 0) {
                    return;
                }
            }
            pos = pos + 1;
            g_db.h.nrsets = g_db.h.nrsets + 1;
            if (pos == 0x81) {
                index_new_chunk_append(cnum, ierr);
                g_db.h.nxtrec = g_db.h.nxtrec + 0xd;
                pos = 1;
            }
        }
        *slot = g_db.h.nrsets;
    } else {
        convert_to_combined(unit, ierr);
        if (*ierr != 0) {
            *ierr = 0x6f;
            return;
        }
        if (*slot == 1) {
            pos = 1;
            g_db.h.nxtrec = 0x11;
            g_db.index_word(1, 1) = 4;
        } else if (g_db.h.nrsets < *slot) {
            const std::int32_t per = 0x80;
            pos = mod1(slot, &per);
            if (pos == 1) {
                cnum = (*slot - 1) / 0x80 + 1;
                index_new_chunk_at(cnum, ierr);
                g_db.h.nxtrec = g_db.h.nxtrec + 0xd;
            } else {
                std::int32_t pos_unused = 0; // DAT_14070748c
                chunk_io(&kRead, unit, &g_db.h.nrsets, &cnum, &pos_unused, ierr);
                if (*ierr != 0) {
                    return;
                }
            }
        } else {
            chunk_io(&kRead, unit, slot, &cnum, &pos, ierr);
            if (*ierr != 0) {
                return;
            }
            g_db.h.nxtrec = g_db.entry(pos, 11);
        }
        g_db.h.nrsets = *slot;
    }

    const std::int32_t fam = g_db.step_family;
    const std::int32_t e = pos;
    g_db.entry(e, 9) = g_db.step_version;
    g_db.entry(e, 1) = g_db.put_mesh_no;
    g_db.entry(e, 4) = *nmar;
    g_db.entry(e, 3) = *nh;
    g_db.entry(e, 5) = *na;
    g_db.entry(e, 2) = g_db.put_step;
    g_db.entry(e, 6) = g_db.put_sim_no;
    g_db.entry(e, 7) = g_db.put_sim_mode;
    g_db.entry(e, 8) = fam;
    if (fam == 2) {
        h[7] = static_cast<double>(*na);
        h[6] = static_cast<double>(*nmar);
        h[8] = static_cast<double>(platform_swap_mode());
    } else if (fam == 3) {
        h[5] = static_cast<double>(platform_swap_mode());
        h[6] = static_cast<double>(*nmar);
        h[7] = static_cast<double>(*na);
    } else {
        *ierr = 0xde;
        return;
    }
    g_db.entry(e, 11) = g_db.h.nxtrec;
    std::int32_t rec = 0; // DAT_140707490
    if (0 < *nh) {
        rec = g_db.h.nxtrec;
        swap8_platform(h, *nh);
        real_io(kWrite, unit, h, nh, &rec, ierr);
        swap8_platform(h, *nh);
        if (*ierr != 0) {
            *ierr = 0x70;
            return;
        }
        g_db.h.nxtrec = rec + 1;
    }

    std::int32_t compressed = 0; // DAT_140707494
    std::int32_t regions = 0;    // DAT_140707498
    bool write_mar = true;
    if (g_db.step_family == 2) {
        compressed = 0xc25 < g_db.step_version ? 1 : 0;
        g_db.entry(e, 12) = g_db.h.nxtrec;
    } else if (g_db.step_family == 3) {
        compressed = 999 < g_db.step_version ? 1 : 0;
        if (5999 < g_db.step_version) {
            regions = 1;
            g_db.entry(e, 12) = g_db.h.nxtrec;
            if (0 < *nmar) {
                write_mar_regions(unit, &pos, nh, nmar, mar, &compressed, ierr); // error not tested
            }
            write_mar = false;
        } else {
            g_db.entry(e, 12) = g_db.h.nxtrec;
        }
    } else {
        g_db.entry(e, 12) = g_db.h.nxtrec;
    }
    if (write_mar && 0 < *nmar) {
        std::int32_t cnt = *nmar; // DAT_14070749c
        rec = g_db.h.nxtrec;
        if (compressed == 1) {
            rle_encode_int(nmar, mar, &cnt);
            rec = g_db.entry(e, 12);
            g_db.entry(e, 4) = cnt;
        }
        write_ints_swapped(unit, mar, cnt, &rec, ierr);
        if (*ierr != 0) {
            *ierr = 0x71;
            return;
        }
        g_db.h.nxtrec = rec + 1;
        if (compressed == 1) {
            rle_decode_int(&cnt, mar, nmar); // gives the caller its array back
        }
    }
    if (0 < *na) {
        if (regions == 0) {
            std::int32_t cnt = *na;
            g_db.entry(e, 13) = g_db.h.nxtrec;
            rec = g_db.h.nxtrec;
            if (compressed == 1) {
                rle_encode_real(na, a, &cnt);
                rec = g_db.entry(e, 13);
                g_db.entry(e, 5) = cnt;
            }
            swap8_platform(a, cnt);
            real_io(kWrite, unit, a, &cnt, &rec, ierr);
            swap8_platform(a, cnt);
            if (*ierr != 0) {
                *ierr = 0x72;
                return;
            }
            g_db.h.nxtrec = rec + 1;
            if (compressed == 1) {
                rle_decode_real(&cnt, a, na);
            }
        } else {
            write_a_regions(unit, &pos, nh, na, a, &compressed, ierr);
            if (*ierr != 0) {
                *ierr = 0x72;
                return;
            }
        }
        if (0 < g_db.dyn_hdr[0] || 0 < g_db.dyn_hdr[10]) {
            write_dynamic(unit, &pos, ierr);
            if (*ierr != 0) {
                *ierr = 0x74;
                return;
            }
        }
        // 1403b8473: platform mode 3 would also write a CHARACTER*4 marker; mode is 2.
    }

    // Record 2 (ints 29..34 swapped to disk order and back), then the index records.
    rec = 2;
    if (platform_swap_mode() == 1) {
        const std::int32_t n = 0x1c;
        swap4_words(g_db.rec2, &n);
    }
    if (platform_swap_mode() == 2 || platform_swap_mode() == 3) {
        const std::int32_t n = 6;
        swap4_words(&g_db.h.istart, &n);
    }
    const std::int32_t n34 = 0x22;
    block_io_int(&kWrite, unit, g_db.rec2, &n34, &rec, ierr);
    if (platform_swap_mode() == 1) {
        const std::int32_t n = 0x1c;
        swap4_words(g_db.rec2, &n);
    }
    if (platform_swap_mode() == 2 || platform_swap_mode() == 3) {
        const std::int32_t n = 6;
        swap4_words(&g_db.h.istart, &n);
    }
    if (*ierr != 0) {
        *ierr = 0x6f;
        return;
    }
    std::int32_t k = 1;
    std::int32_t first = 0;
    do {
        if (g_db.h.nrsets <= first) {
            break;
        }
        rec = k == 1 ? 3 : g_db.index_word(k - 1, 0x80);
        std::int32_t* p = &g_db.index_word(k, 1);
        write_ints_swapped(unit, p, 0x80, &rec, ierr);
        if (*ierr != 0) {
            *ierr = 0x6f;
            return;
        }
        k = k + 1;
        first = first + kEntriesPerIndex;
    } while (k < 0xc9);
    chunk_io(&kWrite, unit, &g_db.h.nrsets, &cnum, &pos, ierr);
}

} // namespace def::dbio::dbcore
