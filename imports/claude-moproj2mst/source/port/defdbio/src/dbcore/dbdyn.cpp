// Dynamic arrays of a step (version >= 11000): op 5 and the writer side of op 4. See
// dbcore/dbops.hpp. Layout in docs/db-format-spec.md section 5.4: a 20-word descriptor at entry
// word 10, a table of 10 words per array (w1 size, w2 RMALLOC index, w3 stored words, w4
// record), each array run-length coded.
#include <defdbio/dbcore/dbops.hpp>

#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/dbcore/util.hpp>
#include <defrt/ftn/helpers.hpp>

#include "internal.hpp"

namespace def::dbio::dbcore {

namespace {

using def::rt::ftn::copy_words4;
using def::rt::ftn::copy_words8;

constexpr std::int32_t kRead = 1;
constexpr std::int32_t kWrite = 2;

// Row k (1-based) word w of the integer table (DAT_140b48658 + k*0x28) and the real table
// (DAT_140b46718 + k*0x28); addressed through the block as the original does.
std::int32_t& int_row(std::int32_t k, std::int32_t w)
{
    return g_db.words()[(0x140b48658 - 0x140b45b80) / 4 + static_cast<std::ptrdiff_t>(k) * 10 + (w - 1)];
}
std::int32_t& real_row(std::int32_t k, std::int32_t w)
{
    return g_db.words()[(0x140b46718 - 0x140b45b80) / 4 + static_cast<std::ptrdiff_t>(k) * 10 + (w - 1)];
}

// IA(idx) / the REAL*8 block of index idx (FUN_1403bd3b5: DAT_14061357c + idx*4, FUN_1403bda24:
// DAT_140613580 + idx*4). A block the port does not know is an invalid address in the original.
void* int_block(std::int32_t idx)
{
    void* p = rm_word(kRmIntBase, idx);
    if (p == nullptr) {
        runtime_fault(kStatusAccessViolation);
    }
    return p;
}
void* real_block(std::int32_t idx)
{
    void* p = rm_word(kRmRealBase, idx);
    if (p == nullptr) {
        runtime_fault(kStatusAccessViolation);
    }
    return p;
}

// part of @src 1403bd3b5 / 1403bda24: the error report (list-directed WRITE(*,*))
void report(const char* what, std::int32_t ierr, std::int32_t actual, std::int32_t stored,
            std::int32_t decoded)
{
    detail::ftn_write_text(what);
    detail::ftn_write_text_int("IERR =         ", ierr);
    detail::ftn_write_text_int("ISIZE_ACTUAL = ", actual);
    detail::ftn_write_text_int("ISIZE        = ", stored);
    detail::ftn_write_text_int("ISIZE_NEW    = ", decoded);
}

// @src 1403be165  read `*stored` ints at *rec, swap, decode in place into *out words
void read_int_array(const std::int32_t* unit, std::int32_t* buf, const std::int32_t* stored,
                    std::int32_t* rec, std::int32_t* out, std::int32_t* ierr)
{
    block_io_int(&kRead, unit, buf, stored, rec, ierr);
    if (swap_mode() == 2) {
        swap4_words(buf, stored);
    }
    if (swap_mode() == 3) {
        swap4_words(buf, stored);
    }
    rle_decode_int(stored, buf, out);
    if (*ierr != 0) {
        *ierr = 1;
    }
}

// @src 1403be25d  same for reals
void read_real_array(const std::int32_t* unit, double* buf, const std::int32_t* stored,
                     std::int32_t* rec, std::int32_t* out, std::int32_t* ierr)
{
    if (platform_swap_mode() == 1) {
        block_io_real8(&kRead, unit, buf, stored, rec, ierr);
    } else {
        const std::int32_t n2 = *stored * 2;
        block_io_int(&kRead, unit, buf, &n2, rec, ierr);
    }
    if (swap_mode() == 2) {
        swap8_words(buf, stored);
    }
    if (swap_mode() == 3) {
        swap8_words(buf, stored);
    }
    rle_decode_real(stored, buf, out);
    if (*ierr != 0) {
        *ierr = 1;
    }
}

// @src 1403bec29  encode `*n` ints of buf, write the stored words at NXTREC
void write_int_array(const std::int32_t* unit, std::int32_t* buf, const std::int32_t* n,
                     std::int32_t* stored, std::int32_t* ierr)
{
    rle_encode_int(n, buf, stored);
    std::int32_t rec = g_db.h.nxtrec; // DAT_140707834
    if (platform_swap_mode() == 2) {
        swap4_words(buf, stored);
    }
    if (platform_swap_mode() == 3) {
        swap4_words(buf, stored);
    }
    block_io_int(&kWrite, unit, buf, stored, &rec, ierr);
    g_db.h.nxtrec = rec + 1;
    if (*ierr != 0) {
        *ierr = 1;
    }
}

// @src 1403bed68  same for reals
void write_real_array(const std::int32_t* unit, double* buf, const std::int32_t* n,
                      std::int32_t* stored, std::int32_t* ierr)
{
    rle_encode_real(n, buf, stored);
    std::int32_t rec = g_db.h.nxtrec; // DAT_140707838
    if (platform_swap_mode() == 2) {
        swap8_words(buf, stored);
    }
    if (platform_swap_mode() == 3) {
        swap8_words(buf, stored);
    }
    if (platform_swap_mode() == 1) {
        block_io_real8(&kWrite, unit, buf, stored, &rec, ierr);
    } else {
        const std::int32_t n2 = *stored * 2;
        block_io_int(&kWrite, unit, buf, &n2, &rec, ierr);
    }
    g_db.h.nxtrec = rec + 1;
    if (*ierr != 0) {
        *ierr = 1;
    }
}

} // namespace

std::int32_t* dyn_int_data(std::int32_t k) { return static_cast<std::int32_t*>(rm_word(kRmIntBase, int_row(k, 2))); }
double* dyn_real_data(std::int32_t k) { return static_cast<double*>(rm_word(kRmRealBase, real_row(k, 2))); }

// @src 1403be093  read ints at a record, swap
void read_ints_at(const std::int32_t* unit, void* buf, const std::int32_t* n, std::int32_t* rec,
                  std::int32_t* ierr)
{
    block_io_int(&kRead, unit, buf, n, rec, ierr);
    if (swap_mode() == 2) {
        swap4_words(buf, n);
    }
    if (swap_mode() == 3) {
        swap4_words(buf, n);
    }
    if (*ierr != 0) {
        *ierr = 1;
    }
}

// @src 1403beaa9  write ints at NXTREC
void write_ints_next(const std::int32_t* unit, void* buf, const std::int32_t* n, std::int32_t* ierr)
{
    std::int32_t rec = g_db.h.nxtrec; // DAT_140707830
    if (platform_swap_mode() == 2) {
        swap4_words(buf, n);
    }
    if (platform_swap_mode() == 3) {
        swap4_words(buf, n);
    }
    block_io_int(&kWrite, unit, buf, n, &rec, ierr);
    if (platform_swap_mode() == 2) {
        swap4_words(buf, n);
    }
    if (platform_swap_mode() == 3) {
        swap4_words(buf, n);
    }
    g_db.h.nxtrec = rec + 1;
    if (*ierr != 0) {
        *ierr = 1;
    }
}

// @src 1403bcf76  op 5: read the dynamic arrays of entry *slot (or of the entry with step
// number 0 when *slot < 1: the step number it searches for, DAT_140707758, is never set)
void op_read_dynamic(const std::int32_t* unit, std::int32_t* slot, std::int32_t* ierr)
{
    *ierr = 0;
    if (g_db.step_version < 11000) {
        return;
    }
    std::int32_t pos = 0; // DAT_140707754
    if (*slot < 1) {
        if (!find_step_entry(unit, 0, slot, &pos, ierr)) {
            return;
        }
    } else {
        std::int32_t cnum = 0;
        const std::int32_t mode = kRead;
        chunk_io(&mode, unit, slot, &cnum, &pos, ierr);
        if (*ierr != 0) {
            return;
        }
    }
    std::int32_t rec = g_db.entry(pos, 10); // _DAT_140707768
    if (rec == 0) {
        return;
    }
    std::int32_t* hdr = g_db.dyn_hdr;
    const std::int32_t n20 = 0x14;
    read_ints_at(unit, hdr, &n20, &rec, ierr);
    // The table reads advance hdr[2] / hdr[12] in place (they pass the descriptor words as REC).
    if (0 < hdr[0] && 0 < hdr[2]) {
        read_ints_at(unit, &int_row(1, 1), &hdr[3], &hdr[2], ierr);
    }
    if (0 < hdr[10] && 0 < hdr[12]) {
        read_ints_at(unit, &real_row(1, 1), &hdr[13], &hdr[12], ierr);
    }
    alloc_dynamic();
    if (0 < hdr[0]) {
        read_int_dynamic(unit, ierr);
    }
    if (0 < hdr[10]) {
        read_real_dynamic(unit, ierr);
    }
}

// @src 1403bc85b  RMALLOC every array with a positive size; hdr[1] / hdr[11] become the largest
void alloc_dynamic()
{
    std::int32_t* hdr = g_db.dyn_hdr;
    const std::int32_t ni = hdr[0];
    g_dyn_words = 0;
    hdr[1] = 0;
    for (std::int32_t k = 1; k <= ni; ++k) {
        std::int32_t size = int_row(k, 1); // DAT_140707718
        if (0 < size) {
            const std::int64_t idx = rmalloc(&size, kRmIntBase, &g_dyn_words);
            int_row(k, 2) = static_cast<std::int32_t>(idx);
        }
        if (hdr[1] < size) {
            hdr[1] = size;
        }
    }
    const std::int32_t nr = hdr[10];
    hdr[11] = 0;
    for (std::int32_t k = 1; k <= nr; ++k) {
        std::int32_t size = real_row(k, 1);
        if (0 < size) {
            std::int32_t words = size * 2;
            const std::int64_t idx = rmalloc(&words, kRmRealBase, &g_dyn_words);
            real_row(k, 2) = static_cast<std::int32_t>(idx);
        }
        if (hdr[11] < size) {
            hdr[11] = size;
        }
    }
}

// @src 1403bd3b5  read the integer arrays (scratch of hdr[1]+2 words, decoded there, copied)
void read_int_dynamic(const std::int32_t* unit, std::int32_t* ierr)
{
    *ierr = 0;
    std::int32_t tot = 0; // _DAT_14070776c
    std::int32_t n = g_db.dyn_hdr[1] + 2;
    auto* tmp = static_cast<std::int32_t*>(rmalloc_scratch(&n, &tot));
    const std::int32_t ni = g_db.dyn_hdr[0];
    for (std::int32_t k = 1; k <= ni; ++k) {
        const std::int32_t idx = int_row(k, 2);    // DAT_140707788
        const std::int32_t actual = int_row(k, 1); // DAT_140707784
        std::int32_t rec = int_row(k, 4);          // DAT_140707790
        std::int32_t stored = int_row(k, 3);       // DAT_14070778c
        std::int32_t out = 0;                      // DAT_140707794
        read_int_array(unit, tmp, &stored, &rec, &out, ierr);
        copy_words4(tmp, out < 1 ? tmp : int_block(idx), &out);
        if (*ierr != 0 || actual != out) {
            report("Error in READ_INTEGER_DYN_ARRAY()", *ierr, actual, stored, out);
        }
    }
    // RFREE(tmp, tot, tot): with both arguments the same word, the global total is not lowered.
    rfree(tmp, &tot, &tot);
}

// @src 1403bda24  read the real arrays (scratch of 2*hdr[11]+4 words)
void read_real_dynamic(const std::int32_t* unit, std::int32_t* ierr)
{
    *ierr = 0;
    std::int32_t tot = 0; // _DAT_140707798
    std::int32_t n = g_db.dyn_hdr[11] * 2 + 4;
    auto* tmp = static_cast<double*>(rmalloc_scratch(&n, &tot));
    const std::int32_t nr = g_db.dyn_hdr[10];
    for (std::int32_t k = 1; k <= nr; ++k) {
        const std::int32_t idx = real_row(k, 2);
        const std::int32_t actual = real_row(k, 1);
        std::int32_t rec = real_row(k, 4);
        std::int32_t stored = real_row(k, 3);
        std::int32_t out = 0;
        read_real_array(unit, tmp, &stored, &rec, &out, ierr);
        copy_words8(tmp, out < 1 ? tmp : real_block(idx), &out);
        if (*ierr != 0 || actual != out) {
            report("Error in READ_REAL_DYN_ARRAY()", *ierr, actual, stored, out);
        }
    }
    rfree(tmp, &tot, &tot);
}

// @src 1403be3d4  descriptor, tables (written twice: before and after the arrays, which fill in
// stored sizes and records), arrays, descriptor again at its first record
void write_dynamic(const std::int32_t* unit, const std::int32_t* pos, std::int32_t* ierr)
{
    std::int32_t* hdr = g_db.dyn_hdr;
    *ierr = 0;
    if (hdr[0] < 1 && hdr[10] < 1) {
        g_db.entry(*pos, 10) = 0;
        return;
    }
    const std::int32_t r0 = g_db.h.nxtrec;
    const std::int32_t n20 = 0x14;
    g_db.entry(*pos, 10) = g_db.h.nxtrec;
    write_ints_next(unit, hdr, &n20, ierr);
    if (hdr[0] < 1) {
        hdr[2] = 0;
    } else {
        hdr[2] = g_db.h.nxtrec;
        write_ints_next(unit, &int_row(1, 1), &hdr[3], ierr);
    }
    if (hdr[10] < 1) {
        hdr[12] = 0;
    } else {
        hdr[12] = g_db.h.nxtrec;
        write_ints_next(unit, &real_row(1, 1), &hdr[13], ierr);
    }
    if (0 < hdr[0]) {
        write_int_dynamic(unit, ierr);
        const std::int32_t save = g_db.h.nxtrec; // DAT_1407077d0
        g_db.h.nxtrec = hdr[2];
        write_ints_next(unit, &int_row(1, 1), &hdr[3], ierr);
        g_db.h.nxtrec = save;
    }
    if (0 < hdr[10]) {
        write_real_dynamic(unit, ierr);
        const std::int32_t save = g_db.h.nxtrec;
        g_db.h.nxtrec = hdr[12];
        write_ints_next(unit, &real_row(1, 1), &hdr[13], ierr);
        g_db.h.nxtrec = save;
    }
    const std::int32_t save = g_db.h.nxtrec;
    g_db.h.nxtrec = r0;
    write_ints_next(unit, hdr, &n20, ierr);
    g_db.h.nxtrec = save;
}

// @src 1403be6af  integer arrays: copy to scratch (hdr[1]+2 words), encode, write
void write_int_dynamic(const std::int32_t* unit, std::int32_t* ierr)
{
    *ierr = 0;
    std::int32_t tot = 0; // DAT_1407077d4
    std::int32_t n = g_db.dyn_hdr[1] + 2;
    auto* tmp = static_cast<std::int32_t*>(rmalloc_scratch(&n, &tot));
    const std::int32_t ni = g_db.dyn_hdr[0];
    for (std::int32_t k = 1; k <= ni; ++k) {
        std::int32_t size = int_row(k, 1);     // _DAT_1407077f4
        const std::int32_t idx = int_row(k, 2); // _DAT_1407077f8
        if (size == 0) {
            int_row(k, 3) = 0;
            int_row(k, 4) = 0;
            continue;
        }
        copy_words4(size < 1 ? tmp : int_block(idx), tmp, &size);
        int_row(k, 4) = g_db.h.nxtrec;
        std::int32_t stored = 0; // DAT_1407077fc
        write_int_array(unit, tmp, &size, &stored, ierr);
        int_row(k, 3) = stored;
    }
    if (0 < tot) {
        rfree(tmp, &tot, &tot);
    }
}

// @src 1403be8ac  real arrays (scratch of 2*hdr[11]+4 words)
void write_real_dynamic(const std::int32_t* unit, std::int32_t* ierr)
{
    *ierr = 0;
    std::int32_t tot = 0; // DAT_140707800
    std::int32_t n = g_db.dyn_hdr[11] * 2 + 4;
    auto* tmp = static_cast<double*>(rmalloc_scratch(&n, &tot));
    const std::int32_t nr = g_db.dyn_hdr[10];
    for (std::int32_t k = 1; k <= nr; ++k) {
        std::int32_t size = real_row(k, 1);
        const std::int32_t idx = real_row(k, 2);
        if (size == 0) {
            real_row(k, 3) = 0;
            real_row(k, 4) = 0;
            continue;
        }
        copy_words8(size < 1 ? tmp : real_block(idx), tmp, &size);
        real_row(k, 4) = g_db.h.nxtrec;
        std::int32_t stored = 0; // DAT_14070782c
        write_real_array(unit, tmp, &size, &stored, ierr);
        real_row(k, 3) = stored;
    }
    if (0 < tot) {
        rfree(tmp, &tot, &tot);
    }
}

} // namespace def::dbio::dbcore
