// ReadStepHeader of DBREAD (packet p-dbread, helper W3). Declarations: defdbio/dbread/w3.hpp.
// Analysis: docs/re/dbmap/stepdriver.md section 2.1.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // the original uses the plain CRT string calls
#endif
#include <defdbio/dbread/w3.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>

#include "pending_w3.hpp"
#include <defdbio/dbread/w2.hpp>
#include "w3_internal.hpp"

namespace def::dbio::dbread {

using model::Cls_1401dbfe0;
using model::DB_STEP_DATA;
using w3::cvtt;

// @src 1401d8f40  return the byte at +0x1b5d
bool step_links_db(const DB_STEP_DATA* m)
{
    return m->u1b5d != 0;
}

// @src 1401cca80  ReadStepHeader
std::int32_t read_step_header(DB_STEP_DATA* m, Cls_1401dbfe0* db)
{
    std::int32_t ierr = 0;    // local_18
    std::int32_t verbose = 0; // local_848[1]
    if (step_links_db(m)) {
        m->db = db;
    }
    const model::DbDirRecord* dir = model::db_info_records(*db);
    // local_848[2] = entry version (+0x20): a dead store
    std::int32_t first = 0;   // local_848[0]
    char name[0x400];         // local_418
    char fname[0x400];        // local_828
    db_file_for_step(db, name, &first, m->u740);
    std::int32_t len = static_cast<std::int32_t>(rt::to_fortran(fname, name, 0x400)); // 140323bf0
    std::int32_t nh = dir[m->u740].hsize; // local_420
    m->u1b70 = nh;
    std::int32_t rec = (m->u740 - first) + 1; // local_428
    m->u1b80 = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(nh)), 8));
    ftn_read_step_header(fname, &len, &rec, &nh, m->u1b80, &ierr, &verbose); // 1403da0b6
    if (ierr == 99) {
        ierr = 3;
    }
    else if (ierr != 0) {
        ierr = 4;
    }
    if (ierr != 0) {
        return ierr;
    }
    model::Cls_1401e7b10& sc = m->sim_ctrl; // +0x750
    const double* h = m->u1b80;
    m->u1378 = cvtt(h[0]);
    if (dir[m->u740].dim == 2) {
        m->u748 = cvtt(h[8]); // +0x40
    }
    else {
        m->u748 = cvtt(h[5]); // +0x28
    }
    read_header_cache(db, &m->u740, false); // 1401dd4d0
    sc.cursim[0] = dir[m->u740].sim_no;     // +0x834
    std::strncpy(sc.simnam, reinterpret_cast<const char*>(m->u1b80 + 0x18), 0x50);
    fun_140324020(sc.simnam, m->u748, 10, 0);
    rt::trim_fortran(sc.simnam, 0x50); // 1403239f0
    // 1400015f0 (3D) and 1401cc180 (step version, +0x1b64)
    if (m->dimension == 3 && m->step_version < 3000) {
        std::strcpy(sc.simnam, "Operation"); // 140484f90
    }
    if (m->step_version < 11000) {
        sc.cursim[1] = sc.cursim[0];
        std::strncpy(sc.oprnam, sc.simnam, 0x40);
        sc.oprnam[0x40] = 0; // +0x7e1
    }
    else {
        sc.cursim[1] = cvtt(m->u1b80[10]);
        sc.cursim[2] = cvtt(m->u1b80[11]);
        std::strncpy(sc.oprnam, reinterpret_cast<const char*>(m->u1b80 + 0x10), 0x40);
        fun_140324020(sc.oprnam, m->u748, 8, 0);
        rt::trim_fortran(sc.oprnam, 0x40);
    }
    std::strcpy(sc.title, db->ua50); // 140484f90
    if (dir[m->u740].dim == 3) {
        m->u1734 = cvtt(m->u1b80[8]);
    }
    else {
        m->u1734 = 0;
    }
    // assigned again for every version (the < 11000 copy above is overwritten)
    sc.cursim[1] = cvtt(m->u1b80[10]);
    sc.cursim[2] = cvtt(m->u1b80[11]);
    return 0;
}

} // namespace def::dbio::dbread
