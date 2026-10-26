// Header and result-set directory reads of datarm23. See db_result_sets.hpp.
#include <defdbio/resultset/db_result_sets.hpp>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defdbio/resultset/globals_p1_13.hpp>
#include <defrt/port.hpp>

#include <cstring>

namespace def::dbio::resultset {

namespace {

// FUN_1404b53a4: CLOSE(unit) of the Fortran runtime (p1-12's fortran_open mode 0)
void close_unit(const std::int32_t* unit)
{
    const std::int32_t mode = 0;
    std::int32_t iostat = 0;
    dbcore::fortran_open(&mode, unit, "", "", &iostat, 0, 0);
}

} // namespace

void reset_globals()
{
    dat_140707dd0 = 0;
    dat_140707e60 = 0;
    dat_140707e64 = 0;
    dat_140707e68 = 0;
    g_numrst = 0;
    g_numrms = 0;
    dat_140707ed8 = 0;
    dat_140707edc = 0;
}

// @src 1403d84b0  open the DB, read its header (op 0), close
void read_db_header(const char* name, const std::int32_t* name_len, DbHeader* out, std::int32_t* err,
                    const std::int32_t* print)
{
    if (*print == 1) {
        // 1403d84b0: WRITE(*,*) 'OPENING DATABASE FILE = ', name(1:len) and the header echo.
        // No caller passes 1 (see db_result_sets.hpp).
        DEF_NOT_PORTED(0x1403d84b0);
    }
    // OPEN(UNIT=3, FILE=name, STATUS='OLD' (str 140bb0b20), mode 5, IOSTAT=err) through
    // FUN_140407e81 (p1-12 dbcore::fortran_open); the unit number lives in DAT_140707dd0. The
    // name is passed with the dummy's length 200 (1403d8636), not *name_len.
    (void)name_len;
    const std::int32_t open_mode = 5;
    dat_140707dd0 = 3;
    dbcore::fortran_open(&open_mode, &dat_140707dd0, name, "OLD", err, 200, 3);
    if (*err == 0) {
        // FUN_1403b5a04 op 0 (p1-12 dbcore::db_operation). Its other outputs (DAT_140707dd8,
        // DAT_140707de0, DAT_140707e10 and five locals) are not read here.
        std::int32_t op = 0;
        std::int32_t slot = 0, step = 0, nh = 0, nmar = 0, na = 0;
        double h_out = 0.0;
        std::int32_t mar_out = 0;
        double a_out = 0.0;
        dbcore::db_operation(&dat_140707dd0, &op, &slot, &step, &nh, &h_out, &nmar, &mar_out, &na, &a_out, err);
        if (*err == 0) {
            const char* block = dbcore::g_db.h.title; // DAT_140b6cb00
            dat_140707e60 = 0x50;
            rt::ftn::last_nonblank_plus1(&dat_140707e60, block); // FUN_140443fc4, print only
            // DO 1..10: the title as 10 doubles (DAT_140707e64 is the DO variable).
            for (dat_140707e64 = 1; dat_140707e64 <= 10; ++dat_140707e64) {
                std::memcpy(out->title + (dat_140707e64 - 1) * 8, block + (dat_140707e64 - 1) * 8, 8);
            }
            std::memcpy(out->smi1, block + 80, 8);  // DAT_140b6cb50
            std::memcpy(out->smi2, block + 88, 8);  // DAT_140b6cb58
            std::memcpy(out->smi3, block + 96, 8);  // DAT_140b6cb60
            std::memcpy(out->smi4, block + 104, 8); // DAT_140b6cb68
            out->istart = dbcore::g_db.h.istart;
            out->ismtyp = dbcore::g_db.h.ismtyp;
            out->nrsets = dbcore::g_db.h.nrsets;
            out->nxtrec = dbcore::g_db.h.nxtrec;
            out->family = dbcore::g_db.file_family;   // DAT_140b490f8
            out->version = dbcore::g_db.file_version; // DAT_140b490fc
            // The print branch would echo the header ('DB HEADER INFORMATION', formats 1000..1040
            // at 140620070.., ISTART.., IPTYPE when the family is 2).
        }
        // else: 'ERROR IN READING STEP HEADER =' (print branch only)
    } else {
        *err = 99; // 'ERROR IN OPENING DATABASE FILE =' (print branch only)
    }
    // 'END OF READING DATABASE HEADER' (print branch only), then CLOSE(3) (FUN_1404b53a4).
    close_unit(&dat_140707dd0);
}

// @src 1403d96fc  open the DB, read the result-set directory (op 1), count, close
void read_result_sets(const char* name, const std::int32_t* name_len, std::int32_t* count,
                      std::int32_t* words, const std::int32_t* ncols, const std::int32_t* /*unused6*/,
                      std::int32_t* err, const std::int32_t* print)
{
    if (*print == 1) {
        // 1403d96fc: WRITE(*,*) 'OPENING DATABASE FILE = ' and the summary. No caller passes 1.
        DEF_NOT_PORTED(0x1403d96fc);
    }
    (void)name_len;
    const std::int32_t open_mode = 5;
    dat_140707e68 = 3;
    dbcore::fortran_open(&open_mode, &dat_140707e68, name, "OLD", err, 200, 3); // 1403d97b6
    if (*err == 0) {
        // FUN_1403b5a04 op 1: directory words 1..10 of entries 1..*count into words
        std::int32_t op = 1;
        std::int32_t slot = 0, step = 0, nh = 0, na = 0;
        double h_out = 0.0;
        double a_out = 0.0;
        dbcore::db_operation(&dat_140707e68, &op, &slot, &step, &nh, &h_out, count, words, &na, &a_out, err);
        if (*err == 0) {
            // Over NRSETS of the datarm23 header (DAT_140b6cb78), not *count; word 4 and word 2
            // of each entry, entries *ncols words apart (re-read from *ncols every pass).
            g_numrst = 0;
            g_numrms = 0;
            dat_140707ed8 = 1;
            const std::int32_t nrsets = dbcore::g_db.h.nrsets; // DAT_140b6cb78
            if (0 < nrsets) {
                std::int32_t i2 = 2;
                std::int32_t i4 = 4;
                std::int32_t off = 0;
                do {
                    dat_140707edc = off;
                    if (words[i4 - 1] != 0) {
                        ++g_numrst;
                    }
                    if (words[i2 - 1] < 0) {
                        ++g_numrms;
                    }
                    ++dat_140707ed8;
                    const std::int32_t nc = *ncols;
                    i2 += nc;
                    i4 += nc;
                    off = dat_140707edc + nc;
                } while (dat_140707ed8 <= nrsets);
            }
            // 'RESULT SET INFORMATION SUMMARY', NUMRST, NUMRMS (print branch only)
        }
        // else: 'ERROR IN OPENING DATABASE FILE =' with the code (print branch only)
    } else {
        *err = 99;
    }
    // 'END OF READING DATABASE RESULT SET INFORMATION' (print branch only), then CLOSE(3).
    close_unit(&dat_140707e68);
}

} // namespace def::dbio::resultset
