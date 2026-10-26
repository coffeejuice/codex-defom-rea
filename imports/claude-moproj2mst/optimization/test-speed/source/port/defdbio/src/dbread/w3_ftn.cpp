// Fortran routines of the DBREAD path that wrap DBOperations23 (packet p-dbread, helper W3):
// open the DB on a unit, run op 0 when the file name differs from the cached one, run op 2 / 3 /
// 5, close. Declarations: defdbio/dbread/w3.hpp.
//
// Unit numbers (DAT_140707ee0, DAT_14070801c, DAT_140708024, DAT_14070803c), the op word
// DAT_14070802c, the step words passed with slot >= 1 (DAT_140707f70, DAT_140707f84,
// DAT_140708020, DAT_140708028, DAT_140708058, all 0 and never written) and the DBMADR counters
// (DAT_14070805c I1, DAT_140708060 I2, DAT_140708064 I, DAT_140708068 J) are Fortran locals the
// compiler put in static storage; each is set before use and read by no other routine (Ghidra
// references, out/dbread/w3/refs_cache.txt), so they are locals here. The dummy array arguments
// of op 0 / op 2 / op 5 (DAT_140706e20.., DAT_140707ef0.., DAT_140708040..) are not touched by
// those ops and are locals too.
#include <defdbio/dbread/w3.hpp>

#include <cstdint>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defrt/port.hpp>

#include "../gendb/db_header_io.hpp"
#include "pending_w3.hpp"
#include <defdbio/dbread/w2.hpp>

namespace def::dbio::dbread {

using dbcore::db_operation;
using dbcore::fortran_open;
using dbcore::g_db;

namespace {

// OPEN(UNIT=*unit, FILE=name, STATUS='OLD', mode 5) as all routines here do it: status from the
// 4-character constant "OLD " (str 140bafe70 / 140bb0b20) passed with length 3, name with the
// dummy's length 200.
void open_old(std::int32_t* unit, const char* name, std::int32_t* ierr)
{
    const std::int32_t mode = 5;
    char status[4];
    gendb::char_assign(status, "OLD ", 4, 4); // 1404b33e4
    fortran_open(&mode, unit, name, status, ierr, 200, 3); // 140407e81
}

// FUN_1404b53a4: CLOSE(unit)
void close_unit(const std::int32_t* unit)
{
    const std::int32_t mode = 0;
    std::int32_t ios = 0;
    fortran_open(&mode, unit, "", "", &ios, 0, 0);
}

// The op-0 block every routine shares: when name(1:200) differs from the cached name, op 0
// (header, signature, index of the open file) and remember the name.
void open_header_if_new(std::int32_t* unit, char* name, double* h, std::int32_t* mar, double* a,
                        std::int32_t* ierr)
{
    if (ftn_compare(name, db_name_cache(), kDbNameCacheLen, kDbNameCacheLen) != 0) { // 1404b4108
        std::int32_t op = 0, slot = 0, step = 0, nh = 0, nmar = 0, na = 0;
        db_operation(unit, &op, &slot, &step, &nh, h, &nmar, mar, &na, a, ierr); // 1403b5a04
        gendb::char_assign(db_name_cache(), name, 200, 200);                    // 1404b33e4
    }
}

} // namespace

char* db_name_cache()
{
    return reinterpret_cast<char*>(&g_db) + (0x140b6ca20 - 0x140b45b80);
}

// @src 1404b4108  Fortran runtime string comparison
std::int32_t ftn_compare(const char* a, const char* b, std::int32_t la, std::int32_t lb)
{
    std::int32_t i = 0;
    if (0 < la) {
        do {
            if (lb <= i || a[i] != b[i]) {
                break;
            }
            i = i + 1;
        } while (i < la);
    }
    if (la != lb || i != la) {
        if (i == la) {
            // a is used up: the rest of b decides (blanks compare equal)
            while (i < lb && b[i] == ' ') {
                i = i + 1;
            }
            if (i != lb) {
                return static_cast<unsigned char>(b[i]) < 0x21 ? 1 : -1;
            }
        }
        else {
            if (i != lb) {
                return static_cast<unsigned char>(b[i]) < static_cast<unsigned char>(a[i]) ? 1 : -1;
            }
            while (i < la && a[i] == ' ') {
                i = i + 1;
            }
            if (i != la) {
                return static_cast<unsigned char>(a[i]) < 0x21 ? -1 : 1;
            }
        }
    }
    return 0;
}

// @src 1403adc74  open the DB on *unit and read its header when the name changed
void ftn_open_db(char* name, std::int32_t* len, std::int32_t* unit, std::int32_t* ierr,
                 std::int32_t* verbose)
{
    (void)len; // only the print branch uses it
    if (*verbose == 1) {
        // WRITE(*,*) 'OPENING DATABASE FILE = ', name(1:*len): no caller passes 1
        DEF_NOT_PORTED(0x1403adc74);
    }
    if (*unit < 1 || 0x31 < *unit) {
        *unit = 3;
    }
    open_old(unit, name, ierr);
    if (*ierr != 0) {
        *ierr = 99; // 'ERROR IN OPENING DATABASE FILE =' only when *verbose == 1
    }
    // The name test and op 0 run even when the OPEN failed (op 0 then fails on the closed unit
    // and overwrites *ierr).
    std::int32_t mar = 0; // DAT_140706e30
    double h = 0.0;       // DAT_140706e20
    double a = 0.0;       // DAT_140706e60
    open_header_if_new(unit, name, &h, &mar, &a, ierr);
}

// @src 1403da0b6  read the step header of entry *rec
void ftn_read_step_header(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* nh,
                          double* h, std::int32_t* ierr, std::int32_t* verbose)
{
    (void)len;
    if (*verbose == 1) {
        // 'OPENING DATABASE FILE = ', the header echo (STEP HEADER INFORMATION, SIMNAM, SwapFlag,
        // MAXH, NUMOBJ, PDIE, TNOW, TEND) and the end line: no caller passes 1
        DEF_NOT_PORTED(0x1403da0b6);
    }
    std::int32_t unit = 3; // DAT_140707ee0
    open_old(&unit, name, ierr);
    if (*ierr == 0) {
        // op 0 always here (no name test), then the name is cached
        std::int32_t op = 0, slot = 0, step = 0, nhw = 0, nmar = 0, na = 0;
        std::int32_t mar_dummy[12] = {}; // DAT_140707ef0
        double a_dummy[4] = {};          // DAT_140707f20
        db_operation(&unit, &op, &slot, &step, &nhw, h, &nmar, mar_dummy, &na, a_dummy, ierr);
        gendb::char_assign(db_name_cache(), name, 200, 200); // 1404b33e4
        // op 2 (its status replaces op 0's)
        std::int32_t step0 = 0; // DAT_140707f70
        nmar = 0;               // local_98
        na = 0;                 // local_94
        op = 2;
        db_operation(&unit, &op, rec, &step0, nh, h, &nmar, mar_dummy, &na, a_dummy, ierr);
        // *ierr != 0: 'ERROR IN READING STEP HEADER =' (print branch only)
    }
    else {
        *ierr = 99;
    }
    close_unit(&unit);
}

// @src 1403daf3e  read the step header of entry *rec on the open *unit (no OPEN / CLOSE)
void ftn_read_step_header_unit(std::int32_t* unit, std::int32_t* rec, std::int32_t* nh, double* h,
                               std::int32_t* ierr, std::int32_t* verbose)
{
    std::int32_t op = 2;
    std::int32_t step0 = 0;          // DAT_140707f84
    std::int32_t nmar = 0;           // local_1d4
    std::int32_t na = 0;             // local_1d0
    std::int32_t mar_dummy[12] = {}; // DAT_140707f90
    double a_dummy[4] = {};          // DAT_140707fc0
    db_operation(unit, &op, rec, &step0, nh, h, &nmar, mar_dummy, &na, a_dummy, ierr);
    if (*verbose == 1) {
        // header echo or 'ERROR IN READING STEP HEADER =', then the end line: no caller passes 1
        DEF_NOT_PORTED(0x1403daf3e);
    }
}

// @src 1403db9e8  read MAR and A of entry *rec
void ftn_read_mar_a(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* nh, double* h,
                    std::int32_t* nmar, std::int32_t* mar, std::int32_t* na, double* a,
                    std::int32_t* ierr, std::int32_t* verbose)
{
    (void)len;
    if (*verbose == 1) {
        // WRITE(6,*) 'OPENING DATABASE FILE = ' .. 'END OF READING DATABASE A/MAR ARRAYS'
        DEF_NOT_PORTED(0x1403db9e8);
    }
    std::int32_t unit = 3; // DAT_14070801c
    open_old(&unit, name, ierr);
    if (*ierr == 0) {
        open_header_if_new(&unit, name, h, mar, a, ierr);
        std::int32_t op = 3;
        std::int32_t step0 = 0; // DAT_140708020
        db_operation(&unit, &op, rec, &step0, nh, h, nmar, mar, na, a, ierr);
        // *ierr != 0: 'ERROR READING A/MAR ARRAYS' (print branch only)
    }
    else {
        *ierr = 99;
    }
    close_unit(&unit);
}

// @src 1403dc13f  read one MAR region of entry *rec
void ftn_read_mar_region(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* nh,
                         double* h, std::int32_t* nmar, std::int32_t* mar, std::int32_t* na,
                         double* a, std::int32_t* sel, std::int32_t* version, std::int32_t* ierr,
                         std::int32_t* verbose)
{
    (void)len;
    if (*verbose == 1) {
        // 'OPENING DATABASE FILE = ' .. 'END OF READING DATABASE MAR ARRAY REGION'
        DEF_NOT_PORTED(0x1403dc13f);
    }
    std::int32_t unit = 3; // DAT_140708024
    open_old(&unit, name, ierr);
    if (*ierr == 0) {
        open_header_if_new(&unit, name, h, mar, a, ierr);
        std::int32_t step0 = 0; // DAT_140708028
        if (*version < 0x8fd) {
            std::int32_t op = 3;
            db_operation_old(&unit, &op, rec, &step0, nh, h, nmar, mar, na, a, ierr);
        }
        else {
            // |*sel| (wrapping as NEG / CMOVS do), op = -(|*sel| + 30): region selector of op 3
            std::int32_t t = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(*sel));
            if (t < 0) {
                t = *sel;
            }
            std::int32_t op = static_cast<std::int32_t>(0u - (static_cast<std::uint32_t>(t) + 0x1eu)); // DAT_14070802c
            db_operation(&unit, &op, rec, &step0, nh, h, nmar, mar, na, a, ierr);
        }
        // *ierr != 0: 'ERROR READING MAR ARRAY REGION' (print branch only)
    }
    else {
        *ierr = 99;
    }
    close_unit(&unit);
}

// @src 1403dd16d  read the dynamic arrays of entry *rec (op 5) and return the descriptor counts
void ftn_read_dynamic_counts(char* name, std::int32_t* len, std::int32_t* rec, std::int32_t* ni,
                             std::int32_t* max_int, std::int32_t* nr, std::int32_t* max_real,
                             std::int32_t* ierr, std::int32_t* verbose)
{
    (void)len;
    if (*verbose == 1) {
        // 'OPENING DATABASE FILE = ' .. 'END OF READING DATABASE DYNAMIC ARRAY'
        DEF_NOT_PORTED(0x1403dd16d);
    }
    std::int32_t unit = 3; // DAT_14070803c
    open_old(&unit, name, ierr);
    if (*ierr == 0) {
        std::int32_t mar_dummy[2] = {}; // DAT_140708048
        double h_dummy[2] = {};         // DAT_140708040
        double a_dummy[2] = {};         // DAT_140708050
        open_header_if_new(&unit, name, h_dummy, mar_dummy, a_dummy, ierr);
        // op 5; the size / array arguments are locals the op does not use
        std::int32_t op = 5;
        std::int32_t step0 = 0; // DAT_140708058
        std::int32_t w_a4 = 0, w_9c = 0, w_98 = 0, w_94 = 0;
        double w_a0 = 0.0, w_90 = 0.0;
        db_operation(&unit, &op, rec, &step0, &w_a4, &w_a0, &w_9c, &w_98, &w_94, &w_90, ierr);
        if (*ierr == 0) {
            *max_real = g_db.dyn_hdr[11]; // DAT_140b48e8c
            *nr = g_db.dyn_hdr[10];       // DAT_140b48e88
            *max_int = g_db.dyn_hdr[1];   // DAT_140b48e64
            *ni = g_db.dyn_hdr[0];        // DAT_140b48e60
        }
        // else 'ERROR READING DYNAMIC ARRAY' (print branch only)
    }
    else {
        *ierr = 99;
    }
    close_unit(&unit);
}

// @src 1403dd950  DBMADR: copy the address tables IIAD1 / IIAD2 out of MAR
void dbmadr(const std::int32_t* mar, std::int32_t* nads1, std::int32_t* nads2,
            const std::int32_t* verbose)
{
    const std::int32_t i1 = mar[1] - mar[0]; // DAT_14070805c
    const std::int32_t i2 = mar[2] - mar[1]; // DAT_140708060
    if (*verbose == 1) {
        // 'COPYING ADDRESSES FROM A AND MAR ARRAYS', I1, I2: no caller passes 1
        DEF_NOT_PORTED(0x1403dd950);
    }
    std::int32_t i = 1; // DAT_140708064
    for (std::int32_t j = 1; j <= i1; j = j + 1) { // DAT_140708068
        i = j + 1;
        nads1[j - 1] = mar[j - 1];
    }
    for (std::int32_t j = 1; j <= i2; j = j + 1) {
        nads2[j - 1] = mar[i - 1];
        i = i + 1;
    }
    // *verbose == 1: 'DONE COPYING ADDRESSES' (unreached, see above)
}

} // namespace def::dbio::dbread
