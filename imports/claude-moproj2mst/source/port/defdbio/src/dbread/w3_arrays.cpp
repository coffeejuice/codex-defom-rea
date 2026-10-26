// ReadArraysFromDB of DBREAD and its partial-load helpers (packet p-dbread, helper W3).
// Declarations: defdbio/dbread/w3.hpp. Analysis: docs/re/dbmap/stepdriver.md sections 1.4, 3.1.
//
// 1401d2b70 and 1401d2bb0 return 1 in this build, so the per-region partial-load code below (the
// branches taken when one of them is 0: 1403dc13f, 1401cf190, 1401cf220, 1403dc957 and the
// region-length tables at +0x2490 / +0x2b24) is never executed. It is ported as written.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/dbread/w3.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "pending_w3.hpp"
#include <defdbio/dbread/w2.hpp>
#include "w3_internal.hpp"

namespace def::dbio::dbread {

using model::Cls_1401dbfe0;
using model::DB_STEP_DATA;
using w3::cvtt;
using w3::remap;

namespace {

bool is_2d(const DB_STEP_DATA* m) { return m->dimension == 2; }                 // 1400015e0
std::int32_t version_of(const DB_STEP_DATA* m) { return m->step_version; }     // 1401cc180
bool fun_1401d2b70(DB_STEP_DATA* m) { return model::names_up_to_date(m); }     // 1401d2b70
bool fun_1401d2bb0(DB_STEP_DATA* m) { return gendb::step_fun_1401d2bb0(m); }   // 1401d2bb0

// 1406f8200 (owner: model::g_timer_read_arrays, p1-02)  timer of _DB_STEP_DATA::ReadArraysFromDB
model::StepTimer& timer() { return model::g_timer_read_arrays; }

// the message of the version test (text as in the decompile; the 2D and 3D branches use the same)
constexpr const char kNoDirectLoad[] =
    "\nDB Version No is %d.\nDirect loading for this version is not supported.";

} // namespace

// @src 140256e20  realloc n ints
std::int32_t* realloc_ints(void* p, std::int32_t n)
{
    if (n == 0) {
        return nullptr;
    }
    void* q = std::realloc(p, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2));
    if (q == nullptr) {
        // str 1405e4e90, 1405e4e80, 1405e4e3c
        fun_14039d3a0("%s Error when re-allocating %s memory of size %d\n", "ERROR    ", "INT", n);
    }
    return static_cast<std::int32_t*>(q);
}

// @src 1402571b0  realloc n doubles
double* realloc_doubles(void* p, std::int32_t n)
{
    if (n == 0) {
        return nullptr;
    }
    void* q = std::realloc(p, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3));
    if (q == nullptr) {
        // str 1405e50e0, 1405e50d0, 1405e50c8
        fun_14039d3a0("%s Error when re-allocating %s memory of size %d\n", "ERROR    ", "DOUBLE", n);
    }
    return static_cast<double*>(q);
}

// @src 1401cf220  read the MAR region starting at `sel` of the current entry into dst
bool read_mar_region(DB_STEP_DATA* m, std::int32_t* dst, std::int32_t sel)
{
    std::int32_t sel_arg = sel; // local_res18
    Cls_1401dbfe0* db = m->db;  // local_440
    if (db == nullptr) {
        return false;
    }
    std::int32_t len = 0;                // local_444
    std::int32_t rec = m->u740 + 1;      // local_448 (replaced below)
    std::int32_t ierr = 0;               // local_24[0]
    std::int32_t verbose = 0;            // local_868[2]
    std::int32_t hsize = 0;              // local_438[0]
    std::int32_t nmar = 0;               // local_28
    std::int32_t na = 0;                 // local_868[1]
    std::int32_t version = 0;            // local_868[3]
    if (!step_dir_sizes(m, db, m->u740, &hsize, &nmar, &na, &version)) {
        return false;
    }
    std::int32_t first = 0; // local_868[0]
    char name[0x400];       // local_428
    char fname[0x400];      // local_848
    db_file_for_step(db, name, &first, m->u740);
    len = static_cast<std::int32_t>(rt::to_fortran(fname, name, 0x400)); // 140323bf0
    rec = (m->u740 - first) + 1;
    ftn_read_mar_region(fname, &len, &rec, &hsize, m->u1b80, &nmar, dst, &na,
                        static_cast<double*>(m->u1b78), &sel_arg, &version, &ierr, &verbose); // 1403dc13f
    return ierr == 0;
}

// @src 1401cf190  ReadSingleMARArrayFromDB
void read_single_mar_array(DB_STEP_DATA* m, std::int32_t* dst, std::int32_t* sel)
{
    if (m->db == nullptr || fun_1401d2bb0(m)) {
        return;
    }
    if (m->u1b68 == 0 || m->u1b6c == 0) {
        // FUN_14047903c() + 0x30: stdout
        rt::crt::fprintf(stdout,
                         "ERROR: _DB_STEP_DATA::ReadSingleMARArrayFromDB() was called before\nthe step header is read. This is illegal.\n");
    }
    else {
        read_mar_region(m, dst, *sel);
    }
}

// @src 1401cd1f0  ReadArraysFromDB
std::int32_t read_arrays(DB_STEP_DATA* m, Cls_1401dbfe0* db)
{
    std::int32_t len = 0;     // local_474
    std::int32_t rec = 0;     // local_478
    std::int32_t st[11] = {}; // local_58: [0] status, [1] region selector, [3] / [8..10] work
    std::int32_t verbose = 0; // local_8a0
    std::int32_t hsize = 0;   // local_46c
    std::int32_t nmar = 0;    // local_64
    std::int32_t na = 0;      // local_8b0[0]
    std::int32_t version = 0; // local_894
    if (!step_dir_sizes(m, db, m->u740, &hsize, &nmar, &na, &version)) {
        st[0] = 999;
        return st[0];
    }
    if (!is_2d(m)) {
        model::select_index_tables_3d(); // 1401c7d10
    }
    else {
        model::select_index_tables_2d(); // 1401c7ce0
    }
    if (step_links_db(m)) {
        m->db = db;
    }
    timer().start("Start"); // 140199dc0
    std::int32_t first = 0; // local_8b8
    char name[0x400];       // local_468
    char fname[0x400];      // local_878
    db_file_for_step(db, name, &first, m->u740);
    len = static_cast<std::int32_t>(rt::to_fortran(fname, name, 0x400)); // 140323bf0
    rec = (m->u740 - first) + 1;
    m->u1b68 = cvtt(m->u1b80[6]);
    m->u1b6c = cvtt(m->u1b80[7]);
    if (m->u1b68 == 0 || m->u1b6c == 0) {
        m->u1b68 = 2000000;
        m->u1b6c = 2000000;
        rt::crt::fprintf(stdout, // FUN_14047903c() + 0x30
                         "WARNING: The database does not store the sizes\nof A / MAR arrays sizes, default values are used.\n");
    }
    timer().checkpoint("calloc : MARptr, Aptr", nullptr); // 140199df0
    if (!fun_1401d2bb0(m)) {
        m->u1b88 = realloc_ints(m->u1b88, m->u1b68);
        if (m->u1b88 == nullptr) {
            return 5;
        }
    }
    else {
        m->u1b88 = realloc_ints(m->u1b88, m->u1b68);
        if (m->u1b88 == nullptr) {
            return 5;
        }
    }
    if (fun_1401d2b70(m)) {
        m->u1b78 = realloc_doubles(m->u1b78, m->u1b6c);
        if (m->u1b78 == nullptr) {
            return 5;
        }
    }
    timer().checkpoint("DBGALL", nullptr);
    std::int32_t* mar = static_cast<std::int32_t*>(m->u1b88);
    if (!is_2d(m)) {
        if (!fun_1401d2b70(m)) {
            st[1] = 0;
            ftn_read_mar_region(fname, &len, &rec, &hsize, m->u1b80, &nmar,
                                static_cast<std::int32_t*>(m->u1b88), &na,
                                static_cast<double*>(m->u1b78), &st[1], &version, &st[0], &verbose); // 1403dc13f
        }
        else {
            ftn_read_mar_a(fname, &len, &rec, &hsize, m->u1b80, &nmar,
                           static_cast<std::int32_t*>(m->u1b88), &na,
                           static_cast<double*>(m->u1b78), &st[0], &verbose); // 1403db9e8
            std::memset(m->u2260, 1, 0x230); // 140478d30: every byte 1
            std::memset(m->u26c4, 1, 0x460);
        }
        mar = static_cast<std::int32_t*>(m->u1b88);
        if (!fun_1401d2bb0(m)) {
            st[3] = 1;
            read_single_mar_array(m, mar, &st[3]);
            st[3] = mar[1];
            read_single_mar_array(m, mar + (st[3] - 1), &st[3]);
            m->u2260[0] = 1;
            m->u2260[1] = 1;
        }
        else if (!fun_1401d2b70(m)) {
            st[2] = 0;
            read_mar_region(m, mar, 0);
            std::memset(m->u2260, 1, 0x230);
        }
    }
    else {
        ftn_read_mar_a(fname, &len, &rec, &hsize, m->u1b80, &nmar,
                       static_cast<std::int32_t*>(m->u1b88), &na,
                       static_cast<double*>(m->u1b78), &st[0], &verbose); // 1403db9e8
    }
    if (st[0] == 99) {
        st[0] = 3;
    }
    else if (st[0] != 0) {
        st[0] = 5;
    }
    if (10999 < version_of(m)) {
        // the descriptor counts are not used further; op 5 allocates the dynamic arrays in the
        // datarm23 tables, which ReadDynamicArrayFromDB (1401d2760) reads later
        st[5] = 0;
        st[7] = 0;
        st[4] = 0;
        st[6] = 0;
        ftn_read_dynamic_counts(fname, &len, &rec, &st[5], &st[7], &st[4], &st[6], &st[0],
                                &verbose); // 1403dd16d
    }
    // any dimension: 1401d2b70 makes its 3D test but returns 1 either way
    if (fun_1401d2b70(m) && version_of(m) < 6000 && (nmar != m->u1b68 || na != m->u1b6c)) {
        st[0] = 5;
    }
    if (st[0] != 0) {
        return st[0];
    }

    std::int32_t nads1_n = 0; // local_470
    std::int32_t nads2_n = 0; // local_898
    (void)is_2d(m);           // 1400015e0, result not used
    nads1_n = 0x8c;
    nads2_n = 0x118;
    timer().checkpoint("calloc : NADS1_Buf, NADS2_Buf", nullptr);
    std::int32_t* nads1 = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(nads1_n), 4)); // local_60
    std::int32_t* nads2 = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(nads2_n), 4)); // local_8a8
    timer().checkpoint("DBMADR", nullptr);
    dbmadr(static_cast<std::int32_t*>(m->u1b88), nads1, nads2, &verbose); // 1403dd950
    if (st[0] != 0) {
        // unreachable: st[0] was tested above
        st[0] = 5;
        if (nads1 != nullptr) {
            std::free(nads1);
        }
        if (nads2 != nullptr) {
            std::free(nads2);
        }
        return st[0];
    }

    const std::int32_t v = version_of(m);
    bool unsupported = false;
    if (!is_2d(m)) {
        if (0x8fc <= v && v <= 2999) {
            for (std::int32_t k = 0; k < 0x50; k = k + 1) {
                m->u1b90[remap(0x1405d6de0, k)] = nads1[k];
            }
            for (std::int32_t k = 0; k < 0x74; k = k + 1) {
                m->u1dc0[remap(0x1405d6f20, k)] = nads2[k];
            }
        }
        else if (3000 <= v && v <= 3999) {
            for (std::int32_t k = 0; k < 0x50; k = k + 1) {
                m->u1b90[remap(0x1405d70f0, k)] = nads1[k];
            }
            for (std::int32_t k = 0; k < 0x8c; k = k + 1) {
                m->u1dc0[remap(0x1405d7230, k)] = nads2[k];
            }
        }
        else if (4000 <= v && v <= 10999) {
            // region counts from the first three MAR starts: (IIAD2 - IIAD1) / 2 MAR regions,
            // (next - IIAD2) / 2 A regions; both from NADS1
            const std::int32_t n1 = (nads1[1] - nads1[0]) / 2; // local_8b4
            const std::int32_t n2 = (nads1[2] - nads1[1]) / 2; // local_88c
            const std::int32_t c1 = 0x45 < n1 ? 0x46 : n1;     // local_20
            const std::int32_t c2 = 0x8b < n2 ? 0x8c : n2;     // local_1c
            for (std::int32_t k = 0; k < c1; k = k + 1) {
                m->u1b90[remap(0x1405d7460, k)] = nads1[k];
                m->u1b90[remap(0x1405d7460, k + 0x28)] = nads1[n1 + k];
            }
            for (std::int32_t k = 0; k < c2; k = k + 1) {
                m->u1dc0[remap(0x1405d75a0, k)] = nads2[k];
                m->u1dc0[remap(0x1405d75a0, k + 0x46)] = nads2[n2 + k];
            }
        }
        else if (v < 11000) {
            unsupported = true;
        }
        else {
            for (std::int32_t k = 0; k < nads1_n; k = k + 1) {
                m->u1b90[k] = nads1[k];
            }
            for (std::int32_t k = 0; k < nads2_n; k = k + 1) {
                m->u1dc0[k] = nads2[k];
            }
        }
    }
    else {
        if (6000 <= v && v <= 6999) {
            for (std::int32_t k = 0; k < 0x3c; k = k + 1) {
                m->u1b90[remap(0x1405d5f00, k)] = nads1[k];
            }
            for (std::int32_t k = 0; k < 0x8c; k = k + 1) {
                m->u1dc0[remap(0x1405d5ff0, k)] = nads2[k];
            }
        }
        else if (7000 <= v && v <= 10999) {
            const std::int32_t n1 = (nads1[1] - nads1[0]) / 2; // local_8b4
            const std::int32_t n2 = (nads1[2] - nads1[1]) / 2; // local_88c
            const std::int32_t c1 = 0x45 < n1 ? 0x46 : n1;     // local_28
            const std::int32_t c2 = 0x8b < n2 ? 0x8c : n2;     // local_24
            for (std::int32_t k = 0; k < c1; k = k + 1) {
                m->u1b90[remap(0x1405d6220, k)] = nads1[k];
                m->u1b90[remap(0x1405d6220, k + 0x1e)] = nads1[n1 + k];
            }
            for (std::int32_t k = 0; k < c2; k = k + 1) {
                m->u1dc0[remap(0x1405d6310, k)] = nads2[k];
                m->u1dc0[remap(0x1405d6310, k + 0x46)] = nads2[n2 + k];
            }
        }
        else if (v < 11000) {
            unsupported = true;
        }
        else {
            for (std::int32_t k = 0; k < nads1_n; k = k + 1) {
                m->u1b90[k] = nads1[k];
            }
            for (std::int32_t k = 0; k < nads2_n; k = k + 1) {
                m->u1dc0[k] = nads2[k];
            }
        }
    }
    if (unsupported) {
        // NADS1 / NADS2 are not freed on this return
        rt::crt::sprintf(reinterpret_cast<char*>(&m->u1401), kNoDirectLoad, version_of(m));
        m->u1400 = 1;
        return 1;
    }
    timer().checkpoint("free : NADS1_Buf, NADS2_Buf", nullptr);
    if (nads1 != nullptr) {
        std::free(nads1);
    }
    nads1 = nullptr;
    if (nads2 != nullptr) {
        std::free(nads2);
    }
    nads2 = nullptr;
    timer().checkpoint("read Simulation Control and geometry/node data", nullptr);

    if (m->dimension == 3 && !fun_1401d2b70(m)) { // 1400015f0
        // region lengths of the address tables (dead branch)
        m->u2490 = 0;
        for (std::int32_t k = 0; k < 0x8b; k = k + 1) {
            if (m->u1b90[k] != 0) {
                m->u2494[k] = m->u1b90[k + 1] - m->u1b90[k];
                if (k == 0x17) {
                    m->u2494[0x17] = m->u1b90[0x3a] - m->u1b90[0x17]; // +0x24f0 = +0x1c78 - +0x1bec
                }
                if (m->u2490 < m->u2494[k]) {
                    m->u2490 = m->u2494[k];
                }
            }
        }
        st[8] = 1;
        m->u2b24 = 0;
        for (std::int32_t k = 1; k < 0x118; k = k + 1) {
            if (m->u1dc0[k] != 0) {
                m->u2b28[k - 1] = m->u1dc0[k] - st[8];
                st[8] = m->u1dc0[k];
                if (m->u2b24 < m->u2b28[k - 1]) {
                    m->u2b24 = m->u2b28[k - 1];
                }
            }
        }
        m->u1b74 = m->u1dc0[0xb] + 10 + m->u2b28[0xb]; // +0x1dec, +0x2b54
        m->u1b78 = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(m->u1b74)), 8);
        for (st[9] = 0; st[9] < 0xc; st[9] = st[9] + 1) {
            if (m->u1dc0[st[9]] != 0) {
                double* a_dst = static_cast<double*>(m->u1b78) + (m->u1dc0[st[9]] - 1);
                read_a_region_from_file(fname, &len, &rec, &hsize, m->u1b80, &nmar,
                              static_cast<std::int32_t*>(m->u1b88), &na, a_dst, &m->u1dc0[st[9]],
                              &version, &st[0], &verbose);
                m->u26c4[st[9]] = 1;
            }
        }
    }
    if (!fun_1401d2bb0(m)) {
        m->u1b88 = realloc_ints(m->u1b88, m->u1b90[3] + 10 + m->u2494[3]); // +0x1b9c, +0x24a0
        for (st[10] = 0; st[10] < 4; st[10] = st[10] + 1) {
            if (m->u1b90[st[10]] != 0 && m->u2260[st[10]] == 0) {
                read_single_mar_array(m, static_cast<std::int32_t*>(m->u1b88) + (m->u1b90[st[10]] - 1),
                                      &m->u1b90[st[10]]);
                m->u2260[st[10]] = 1;
            }
        }
        m->u2240 = realloc_ints(m->u2240, m->u2490 + 2);
    }
    const char report = 0; // local_2c
    timer().stop(&report); // 140199eb0
    st[0] = 0;
    return st[0];
}

} // namespace def::dbio::dbread
