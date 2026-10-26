// GENDB driver (packet p4-01): FUN_1401b9940, FUN_1401bb550, FUN_1401bcbb0 and the small helpers
// they call. Messages go to stdout (FUN_14047903c()+0x30) with the binary's strings.
// Spec: docs/re/dbmap/stepdriver.md section 5 (claude/project-thread-bz5k5e).
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/step_header.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <direct.h>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "db_header_io.hpp"
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/ext.hpp>

namespace def::dbio::gendb {

using model::Cls_1401dbfe0;
using model::field;
namespace crt = def::rt::crt;

namespace {

inline bool is_3d(const DB_STEP_DATA& m) { return m.dimension == 3; } // 1400015f0 (p1-01, inlined)

// @src 14001cd50  error report (code, value, value): empty in this build
void report(const std::int32_t*, const std::int32_t*, const std::int32_t*) {}
// @src 1401c54b0  after a successful write: empty in this build
void after_write(DB_STEP_DATA&) {}

std::int32_t& w(DB_STEP_DATA& m, std::size_t off) { return field<std::int32_t>(&m, off); }

} // namespace

// @src 1401c5690  direct DB writing flag (+0x8)
bool direct_write(const DB_STEP_DATA& m) { return field<std::uint8_t>(&m, 0x8) != 0; }
// @src 1401d7ed0  remesh flag (+0x2f89)
bool remesh_flag(const DB_STEP_DATA& m) { return field<std::uint8_t>(&m, 0x2f89) != 0; }
// @src 1401d7eb0  set the remesh flag
void set_remesh_flag(DB_STEP_DATA& m, std::uint8_t v) { field<std::uint8_t>(&m, 0x2f89) = v; }

// @src 1401c54c0  progress stage
// @src 14001cdb0  (the stage report it calls: empty in this build)
void progress(DB_STEP_DATA&, const std::int32_t*) {}

// @src 1401b9940  GENDB entry
// @src 1401b9a40  (the indirect-writing branch: returns 0, writes nothing)
std::int32_t gendb(DB_STEP_DATA& m, const char* path, std::int32_t /*cur_idx*/, std::int32_t check,
                   std::int32_t dbopt)
{
    if (remesh_flag(m)) {
        w(m, 0x840) = w(m, 0x840) + 1; // MESHNO
        set_remesh_flag(m, 0);
    }
    if (!direct_write(m)) {
        return 0;
    }
    unsigned long err = 0;
    change_dir(path, &err); // result unused
    return write_db(m, path, check, dbopt, 0);
}

// @src 1401b8bf0  _chdir
bool change_dir(const char* path, unsigned long* err)
{
    const int r = _chdir(path);
    if (r != 0) {
        *err = static_cast<unsigned long>(*__doserrno());
    }
    else {
        *err = 0;
    }
    return r == 0;
}

// @src 1401ba780  the file opens for reading
bool file_readable(const char* name)
{
    if (name == nullptr) {
        return false;
    }
    std::FILE* f = _fsopen(name, "r", _SH_DENYNO); // FUN_14047b934, str 1405267cc
    if (f == nullptr) {
        return false;
    }
    std::fclose(f);
    return true;
}

// @src 1401b85d0  remove the file if it exists
std::int32_t remove_if_exists(const char* name)
{
    std::FILE* f = _fsopen(name, "rt", _SH_DENYNO); // str 1405d13a4
    if (f == nullptr) {
        return 1;
    }
    std::fclose(f);
    return std::remove(name) == 0 ? 1 : 0; // 14047e2cc: DeleteFileA
}

// @src 1401b9810  may a step of version *cur go into a DB of version *db
bool versions_compatible(const DB_STEP_DATA& m, const std::int32_t* db, const std::int32_t* cur)
{
    if (10000 <= *cur) {
        return true;
    }
    if (!is_3d(m)) {
        if (*db != *cur) {
            return (*db == 9000 && *cur == 0x2332) || (*db == 9000 && *cur == 0x238c) ||
                   (*db == 0x2332 && *cur == 0x238c);
        }
    }
    else if (*db != *cur) {
        return (*db == 6000 && *cur == 0x177a) || (*db == 6000 && *cur == 0x17d4) ||
               (*db == 0x177a && *cur == 0x17d4);
    }
    return true;
}

// @src 1401ddc10  highest version in the directory
std::int32_t db_max_version(const Cls_1401dbfe0* info)
{
    const std::int32_t n = field<std::int32_t>(info, 0xac8);
    const std::byte* dir = field<const std::byte*>(info, 0xad8);
    if (dir == nullptr || n < 1) {
        return 10000;
    }
    std::int32_t v = 0;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        const std::int32_t e = field<std::int32_t>(dir, 0x20 + static_cast<std::size_t>(i) * 0x28);
        if (v < e) {
            v = e;
        }
    }
    return v;
}

// @src 1401dc3a0  name and version of a new DB
void db_info_set_new(Cls_1401dbfe0* info, const char* name, const std::int32_t* version)
{
    std::strcpy(&field<char>(info, 0x650), name);
    field<std::int32_t>(info, 0xabc) = *version;
}

// @src 1401b9c00  version check of an existing DB
bool check_db_version(DB_STEP_DATA& m, const char* path)
{
    Cls_1401dbfe0 info;
    if (load_db_info(info, path, false) != 0) { // 1401dc3f0
        return false;
    }
    const std::int32_t db = db_max_version(&info);
    const std::int32_t cur = write_version(&m);
    if (cur < db) {
        crt::fprintf(stdout, "WARNING: The old database was written in a higher version of DEFORM.\n");
        crt::fprintf(stdout, "         New features might be lost if generated in this version.\n");
        const std::int32_t code = 8, a = 0, b = 0;
        report(&code, &a, &b);
    }
    return true;
}

// @src 1401e1f60  place the step in the directory
std::int32_t place_step(const Cls_1401dbfe0* info, const std::int32_t* nstart, std::int32_t* count,
                        std::int32_t* pos, std::int32_t* prev, std::int32_t* prev_step,
                        const std::int32_t* meshno, std::int32_t* last_mesh)
{
    const std::byte* dir = field<const std::byte*>(info, 0xad8);
    auto word = [dir](std::int32_t entry0, std::size_t off) {
        return field<std::int32_t>(dir, static_cast<std::size_t>(entry0) * 0x28 + off);
    };
    const std::int32_t target = std::abs(*nstart); // 140484ab8
    *count = field<std::int32_t>(info, 0xac8);
    *pos = 1;
    while (*pos <= *count && std::abs(word(*pos - 1, 4)) < target) {
        *pos = *pos + 1;
    }
    if (*pos <= *count) {
        const std::int32_t st = word(*pos - 1, 4);
        if (target == std::abs(st) && 0 < *nstart && st < 0) {
            *pos = *pos + 1;
        }
    }
    *prev = *pos - 1;
    std::int32_t k = 1;
    while (true) {
        if (*pos - 1 < k) {
            return 0;
        }
        *last_mesh = word(k - 1, 0);
        if (*last_mesh == word(*pos - 2, 0)) {
            break;
        }
        k = k + 1;
    }
    *prev_step = word(k - 1, 4);
    if (*last_mesh <= *meshno) {
        return 0;
    }
    if (*count < *pos) {
        return 0;
    }
    return -1;
}

// @src 1401bb550  write the step to the DB
std::int32_t write_db(DB_STEP_DATA& m, const char* path, std::int32_t check, std::int32_t dbopt,
                      std::int32_t signmode)
{
    std::int32_t err = 0;
    std::int32_t warn = 0;
    crt::fprintf(stdout, "\n*** Direct DB Writing Scheme On\n");
    crt::fprintf(stdout, "Checking Database Version \n");
    check_db_version(m, path);
    crt::fprintf(stdout, "Checking Input Data \n");
    ext::input_counts(m, &err); // 1401bc230
    if (err != 0) {
        crt::fprintf(stdout, "INFO   : DEFORM Database can NOT be Generated\n");
        return 1;
    }
    if (check == 0 || check == 2) {
        if (!ext::object_checks(m)) { // 1401b9d50
            return 1;
        }
        ext::db_checks(m, &err, &warn); // 1401bebf0
        if (err != 0) {
            crt::fprintf(stdout, "INFO   : DEFORM Database can NOT be Generated\n");
            return 1;
        }
        crt::fprintf(stdout, "INFO   : Input data has no errors \n");
        if (check == 0) {
            return 0;
        }
    }
    else {
        crt::fprintf(stdout, "INFO   : Database checking has been skipped.\n");
    }
    std::int32_t mode = 1;
    if (dbopt < 1) {
        mode = file_readable(path) ? 1 : 2;
    }
    else {
        mode = dbopt;
        if (dbopt == 1 && !file_readable(path)) {
            crt::fprintf(stdout, "ERROR  : Old Database NOT Exist\n");
            const std::int32_t code = 6, a = 0, b = 0;
            report(&code, &a, &b);
            return 1;
        }
    }
    std::int32_t& nst = w(m, 0x8b0); // NSTART
    if (signmode == 0 && 0 < nst) {
        nst = -(nst + 1);
    }
    else if (signmode == 2) {
        if (nst < 1) {
            if (nst < 0) {
                nst = nst - 1;
            }
        }
        else {
            nst = -(nst + 1);
        }
    }
    std::int32_t pos = 1;
    std::int32_t nstart = nst;
    std::int32_t prev = 0;
    std::int32_t count = 0;
    bool overwrite = false;
    char tmp[0x400];
    std::strcpy(tmp, path);
    w(m, 0x68) = nstart;
    Cls_1401dbfe0 info; // 1401dbfe0; dtor 1401dc1c0 on every return below
    if (mode == 1) {
        if (load_db_info(info, path, false) != 0) {
            crt::fprintf(stdout, "ERROR  : Invalid DEFORM Database\n");
            const std::int32_t code = 7, a = 0, b = 0;
            report(&code, &a, &b);
            return 1;
        }
        const std::int32_t db_version = field<std::int32_t>(&info, 0xabc);
        const std::int32_t cur = write_version(&m);
        if (!versions_compatible(m, &db_version, &cur)) {
            crt::fprintf(stdout, "ERROR  : The old database in the current directory\n");
            crt::fprintf(stdout, "         is not a valid Version %g database so a\n",
                         static_cast<double>(cur) / 1000.0); // DAT_140522f90
            crt::fprintf(stdout, "         step cannot be added to it.\n");
            crt::fprintf(stdout, "         Earlier versions of DEFORM databases can\n");
            crt::fprintf(stdout, "         be read in the pre-processor and viewed in\n");
            crt::fprintf(stdout, "         the post-processor but a simulation cannot\n");
            crt::fprintf(stdout, "         be continued in an old database\n");
            const std::int32_t code = 4, b = 0;
            report(&code, &cur, &b);
            return 1;
        }
        std::int32_t last_mesh = 0;
        if (place_step(&info, &nstart, &count, &pos, &prev, &w(m, 0x68), &w(m, 0x840), &last_mesh) ==
            -1) {
            crt::fprintf(stdout, "WARNING: MESHNO %5d less than MESHNO in the\n", w(m, 0x840));
            crt::fprintf(stdout, "         last step of database %5d\n", last_mesh);
            const std::int32_t code = 0x26;
            diag_14001cd60(&code, "MESHNO", &w(m, 0x840), &last_mesh); // p4-02b
        }
    }
    else {
        if (file_readable(path)) {
            crt::fprintf(stdout, "WARNING: Database already Exists; Overwriting\n");
            overwrite = true;
            std::strcat(tmp, "temp"); // str 1405277f0
            remove_if_exists(tmp);
        }
        const std::int32_t v = write_version(&m);
        db_info_set_new(&info, tmp, &v);
    }
    char fname[0x400];
    std::int32_t len = static_cast<std::int32_t>(def::rt::to_fortran(fname, tmp, 0x400)); // 140323bf0
    std::int32_t unit = 3;
    std::int32_t verbose = 0;
    err = 0;
    open_db(fname, &len, &unit, &mode, &err, &verbose); // 1403ae0b6
    if (err != 0) {
        crt::fprintf(stdout, "ERROR  : Error in opening %s\n", path);
        const std::int32_t code = 5, a = 0, b = 0;
        report(&code, &a, &b);
        return 1;
    }
    const char keep_version = 1;
    write_step(m, tmp, &pos, &count, &prev, &info, &keep_version, &err);
    close_db(&unit); // 1403ae06f
    if (err != 0) {
        crt::fprintf(stdout, "ERROR  : Error in generating DEFORM database\n");
        if (overwrite) {
            remove_if_exists(tmp);
        }
        return 1;
    }
    crt::fprintf(stdout, "INFO   : DEFORM Database Generated\n");
    if (overwrite) {
        remove_if_exists(path);
        std::rename(tmp, path); // 14047e68c: MoveFileA
    }
    after_write(m);
    return 0;
}

// @src 1401bcbb0  the step write
void write_step(DB_STEP_DATA& m, const char* path, std::int32_t* pos, std::int32_t* count,
                std::int32_t* prev, Cls_1401dbfe0* info, const char* keep_version, std::int32_t* ierr)
{
    std::int32_t dim = m.dimension; // 140158e90
    model::select_index_tables(&dim); // 1401c7d40
    std::int32_t stage = 1;
    progress(m, &stage);
    std::int32_t step = 0;
    write_db_header(m, &step, pos, count, prev, info, keep_version);
    region_sizes(m);
    alloc_buffers(m);
    store_address_tables(m);
    std::int32_t* mar = field<std::int32_t*>(&m, 0x50);
    double* a = field<double*>(&m, 0x58);
    auto mar_at = [&](std::int32_t k) { return mar + (w(m, 0x1b90 + model::g_mar_index[k] * 4) - 1); };
    auto a_at = [&](std::int32_t k) { return a + (w(m, 0x1dc0 + model::g_a_index[k] * 4) - 1); };
    stage = 2;
    progress(m, &stage);
    ext::write_sim_control(m, mar_at(6), a_at(3), a_at(4), a_at(0), mar_at(2), mar_at(11)); // 1401c4950
    stage = 3;
    progress(m, &stage);
    ext::write_materials(m, a_at(1), a_at(0), mar_at(2)); // 1401c49b0
    stage = 4;
    progress(m, &stage);
    ext::write_objects(m, mar, a, &w(m, 0x1b90), &w(m, 0x1dc0), a_at(0), mar_at(2), a_at(9),
                       a_at(10)); // 1401c4d90
    stage = 5;
    progress(m, &stage);
    ext::write_inter_objects(m, a_at(7), a_at(0), mar_at(2), a_at(9), a_at(10), a_at(6)); // 1401c4ea0
    stage = 6;
    progress(m, &stage);
    ext::write_material_pairs(m, a_at(2), a_at(0), mar_at(2)); // 1401c4fa0
    collect_dynamic_arrays(m);
    stage = 7;
    progress(m, &stage);
    std::int32_t nh = 0;
    ext::write_step_header(m, path, prev, &nh, info, keep_version); // 1401c5020
    std::int32_t unit = 3;
    std::int32_t op = 4;
    std::int32_t slot = *pos;
    std::int32_t st = step;
    dbcore::db_operation(&unit, &op, &slot, &st, &nh, field<double*>(&m, 0x60), &w(m, 0x30),
                         field<std::int32_t*>(&m, 0x50), &w(m, 0x34), field<double*>(&m, 0x58),
                         ierr); // 1403b5a04
    model::db_write_var_free_buffers(m); // 1401bb320
    stage = 8;
    progress(m, &stage);
}

} // namespace def::dbio::gendb
