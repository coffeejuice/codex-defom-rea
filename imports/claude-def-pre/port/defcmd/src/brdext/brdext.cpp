// Packet p8-05: BRDEXT, the border extraction through the external AMG_DXT (2D objects) /
// AMG_DXT3 (3D objects) programs. Spec docs/re/commands/BRDEXT.md; decompile and listing
// deform-rea-work/out/p8mg/p8-05/all.c, all.asm, str.asm.
//
// The VS2010 std::vector<std::string> / std::string instantiations the functions use (140006700,
// 14008c3f0, 140158c10, 140159230, 140159c80 .. 1401b5ef0, see docs/re/status/p8-05.tsv) are the
// C++ standard library here.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/brdext.hpp>
#include <defcmd/cmd_p8_05.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/meshgen/p8_seams.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <direct.h>
#include <share.h>
#include <windows.h>

#include <defdbio/gendb/gendb.hpp>
#include <defdbio/step/db_header_cache.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/def_amggeo.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defrt/stdlib/crt_qsort.hpp>
#include <defrt/stdlib/str_helpers.hpp>

namespace def::cmd {

namespace {

using model::LLOBJ_DATA;

std::int32_t& obj_int(LLOBJ_DATA* obj, std::size_t off)
{
    return *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(obj) + off);
}

// 140158ca0 (owner p67-5, inlined there as object_is_3d too): obj+0x39c0 == 3
bool object_is_3d(LLOBJ_DATA* obj) { return model::geo_kw::object_dim(*obj) == 3; }

// 1401b85d0 (P4, defdbio gendb): fopen test, then remove
void remove_file(const char* name) { (void)def::dbio::gendb::remove_if_exists(name); }

// 14047b934: fopen = _fsopen(name, mode, _SH_DENYNO)
std::FILE* open_file(const char* name, const char* mode) { return _fsopen(name, mode, _SH_DENYNO); }

void print(const char* text) { std::fputs(text, stdout); } // fprintf(stdout, text) without args

} // namespace

namespace brdext {

// @src 14013ca60  comparator over the raw slots (see SuffixSlot). The slots' first 8 bytes are
// the never-written allocator bytes of a new[] block (heap contents in the original); the port
// zeroes them, so both strings are "" -> 0 and every pair compares equal. The visible effect is
// the permutation the MSVC qsort applies to equal elements (crt_qsort). Byte runs that happen to
// read as digits, and the read past +0x08 when the 8 bytes hold no NUL, are not reproducible.
int compare_suffix(const void* a, const void* b)
{
    const char* pa = static_cast<const SuffixSlot*>(a)->alloc_bytes;
    const char* pb = static_cast<const SuffixSlot*>(b)->alloc_bytes;
    std::string sa(pa, strnlen(pa, sizeof SuffixSlot::alloc_bytes)); // 1400066a0
    std::string sb(pb, strnlen(pb, sizeof SuffixSlot::alloc_bytes));
    const std::int32_t va = std::abs(def::rt::str_to_int(sa)); // 1401b0870, 140484ab8
    const std::int32_t vb = std::abs(def::rt::str_to_int(sb));
    if (va < vb) return -1;
    if (va == vb) return 0;
    return 1;
}

// @src 1401b1630  directory listing (pattern by value)
std::int32_t list_files(std::string pattern, std::vector<std::string>& out)
{
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        return -1;
    }
    do {
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            out.push_back(std::string(fd.cFileName)); // 1401b3490
        }
    } while (FindNextFileA(h, &fd) != 0);
    FindClose(h);
    return 0;
}

// @src 1401b1770  cwd
void get_cwd(char* buf)
{
    if (_getcwd(buf, 0x104) == nullptr) {
        std::strcpy(buf, "."); // str 140524d20
    }
}

// @src 1401b17f0  file can be opened for reading
bool file_readable(std::string name)
{
    std::FILE* f = nullptr;
    fopen_s(&f, name.c_str(), "r");
    if (f == nullptr) {
        return false;
    }
    std::fclose(f);
    return true;
}

// @src 14013cc10  suffix collection, sort and pick
// @src 1401b15a0  (inlined here: the last n characters of a string, 140006760 substr)
std::int32_t pick_suffix(const std::int32_t* step, const std::string& prob, std::string dir,
                         std::vector<std::string>& arr)
{
    std::int32_t found = -1;              // local_370
    std::string pattern = dir;            // local_368
    pattern += fun_1401b17b0();           // 140159230 (append), "\" str 140524d24
    pattern += "*.DB";                    // 140006730, str 14051cc98
    std::vector<std::string> files;       // local_3a0
    (void)list_files(pattern, files);
    const std::int32_t n = static_cast<std::int32_t>(files.size());
    // the array was sized by 14013d230's own listing of the same directory
    if (arr.size() < files.size()) arr.resize(files.size());
    for (std::int32_t i = 0; i < n; i = i + 1) {
        arr[static_cast<std::size_t>(i)] = ""; // 140006700
    }
    std::vector<std::string> suffix;      // the texts of the slots
    std::int32_t m = 0;                   // local_36c
    for (std::int32_t i = 0; i < n; i = i + 1) {
        const std::string stem = def::rt::Cls_1401b19c0(files[static_cast<std::size_t>(i)]).stem();
        if (!def::rt::str_starts_with(stem, prob.c_str(), true)) continue; // 1401b0940(., ., 1)
        if (stem == prob) continue;                                            // operator== 1400bd310
        const std::int32_t ls = static_cast<std::int32_t>(stem.size());       // 140002800
        const std::int32_t lp = static_cast<std::int32_t>(prob.size());
        // 1401b15a0(stem, ls - lp): the last ls - lp characters
        const std::int32_t cut = static_cast<std::int32_t>(stem.size()) - (ls - lp);
        suffix.push_back(stem.substr(static_cast<std::size_t>(cut)));
        m = m + 1;
    }
    // 14013d0f4: qsort(arr, m, 0x28, 14013ca60) over the slots
    std::vector<SuffixSlot> slots(static_cast<std::size_t>(m));
    for (std::int32_t k = 0; k < m; k = k + 1) {
        std::memset(&slots[static_cast<std::size_t>(k)], 0, sizeof(SuffixSlot));
        slots[static_cast<std::size_t>(k)].index = static_cast<std::uint32_t>(k);
    }
    def::rt::crt_qsort(slots.data(), static_cast<std::size_t>(m), sizeof(SuffixSlot), compare_suffix);
    for (std::int32_t k = 0; k < m; k = k + 1) {
        arr[static_cast<std::size_t>(k)] = suffix[slots[static_cast<std::size_t>(k)].index];
    }
    for (std::int32_t i = 0; i < n; i = i + 1) {
        const std::string& s = arr[static_cast<std::size_t>(i)];
        if (s.size() != 0) {
            const std::int32_t v = std::abs(def::rt::str_to_int(s));
            if (std::abs(*step) <= v) {
                found = i;
                break;
            }
        }
    }
    return found;
}

// @src 14013d230  DB lookup (the bool result is ignored by both callers)
bool find_db(const std::int32_t* step, std::string& prob, std::string cwd, std::string& db)
{
    std::string pattern = cwd;            // local_230
    pattern += fun_1401b17b0();           // 140159230
    pattern += "*.DB";                    // str 14051cca0
    std::string dir = cwd;                // local_290
    dir += fun_1401b17b0();
    std::vector<std::string> files;       // local_260
    (void)list_files(pattern, files);
    // new[] of files.size() strings (thunk 1404797d8 + _eh_vector_constructor_iterator_),
    // filled with the file names (overwritten by 14013cc10)
    std::vector<std::string> arr(files.size());
    for (std::size_t i = 0; i < files.size(); ++i) {
        arr[i] = files[i];
    }
    const std::int32_t k = pick_suffix(step, prob, dir, arr);
    if (k == -1) {
        return false;
    }
    prob = prob + arr[static_cast<std::size_t>(k)]; // 14015a7c0
    db = (dir + prob) + ".DB";                       // str 14051cca8
    return true;
}

// @src 14013d900  remove the outputs of a previous extraction
void remove_outputs(Cls_14013d8d0&, std::string prob, const std::int32_t* step)
{
    std::string db; // local_2b0
    if (prob.size() == 0) {
        return;
    }
    char cwd[0x110]; // local_288
    get_cwd(cwd);
    (void)find_db(step, prob, std::string(cwd), db);
    const std::string geo = prob + ".GEO"; // str 14051cd00
    const std::string res = prob + ".RES"; // str 14051cd08
    std::remove(geo.c_str());              // 14047e29c
    std::remove(res.c_str());
    std::remove("NBCD.DAT");
}

// @src 14013e230  INI, launch, read-back
std::uint32_t run_extraction(Cls_14013d8d0& self, std::string prob, const std::int32_t* step,
                             LLOBJ_DATA* obj)
{
    model::DEF_AMGGEO geo;                                   // 140246d00 / dtor 140246d40
    const std::int32_t objno = obj_int(obj, 8);              // local_85c
    std::uint32_t rc = 0;                                    // local_858
    const bool is2d = model::p407::obj_kind_is_2(obj);       // 140158d00
    const bool is3d = object_is_3d(obj);                     // 140158ca0
    remove_file("DEF_BRD.OUT");
    if (is3d) {
        remove_file("CHK_VOL.INI");
    }
    std::FILE* f;
    if (!is2d) {
        remove_file("AMG_DXT3.INI");
        f = open_file("AMG_DXT3.INI", "wt"); // str 14051cddc
    } else {
        remove_file("AMG_DXT.INI");
        f = open_file("AMG_DXT.INI", "wt");  // str 14051cd44
    }
    if (f == nullptr) {
        return 1;
    }
    std::fprintf(f, "%s\n", prob.c_str());
    std::fprintf(f, "%d\n", *step);
    std::fprintf(f, "%d\n", objno);
    std::fclose(f);
    char cmd[208]; // local_998
    if (!is2d) {
        std::sprintf(cmd, "AMG_DXT3 > %s", "DEF_BRD.OUT");
        launch_program_3d(1, cmd); // 140224130
    } else {
        std::sprintf(cmd, "AMG_DXT > %s", "DEF_BRD.OUT");
        launch_program_2d(1, cmd); // 140224100
    }
    remove_file("DEF_BRD.OUT");
    char geo_name[0x400]; // local_438
    char res_name[0x400]; // local_848
    std::sprintf(geo_name, "%s.GEO", prob.c_str());
    std::sprintf(res_name, "%s.RES", prob.c_str());
    if (!def::dbio::file_exists(geo_name)) { // 1401b8560
        remove_file(res_name);
        remove_file("NBCD.DAT");
        return 1;
    }
    bool ok;
    if (!is2d) {
        const char close = 0;                                          // local_2f
        ok = geolib::fun_140246f70(&geo, geo_name, &close, 3, nullptr); // local_9d8 = 0
    } else {
        const char close = 1;                                          // local_30
        ok = geolib::fun_140246f70(&geo, geo_name, &close, 4, obj);
    }
    if (!ok) {
        remove_file(geo_name);
        remove_file(res_name);
        remove_file("NBCD.DAT");
        return 1;
    }
    if (!is2d) {
        ok = geolib::fun_14024bac0(&geo, obj, 3);
    }
    rc = ok ? 0u : 1u;
    if (self.keep_usrwin == 0) { // 140158d40
        remove_file("USRWIN.DAT");
    }
    if (!is2d) {
        remove_file("AMG_DXT3.INI");
    } else {
        remove_file("AMG_DXT.INI");
    }
    return rc;
}

// @src 14013dba0  checks and messages around run_extraction
bool extract(Cls_14013d8d0& self, std::string prob, std::string db, std::int32_t* step,
             LLOBJ_DATA* obj)
{
    remove_outputs(self, prob, step);
    if (obj == nullptr) {
        print("ERROR  : Border extraction failed.\n");
        return false;
    }
    if (obj_int(obj, 0xc) == 0) {
        print("ERROR  : Border extraction failed. Invalid object type\n");
        return false;
    }
    if (*step == 0) {
        print("ERROR  : Border extraction failed. Invalid step number\n");
        return false;
    }
    char cwd[0x110]; // local_1c38
    get_cwd(cwd);
    (void)find_db(step, prob, std::string(cwd), db);
    if (!file_readable(db)) {
        print("ERROR  : Border extraction failed.\n");
        std::fprintf(stdout, "Database [%s] does not exist.\n", db.c_str());
        return false;
    }
    if (object_is_3d(obj)) {
        model::Cls_1401dbfe0 info;                        // 1401dbfe0 / dtor 1401dc1c0
        (void)def::dbio::load_db_info(info, db.c_str(), false); // 1401dc3f0(., db, 0)
        (void)def::dbio::step::reread_db_header(info); // 1401dd0d0 (p67-5), result ignored
        if (info.uabc < 0x8fd) {                           // local_103c = info+0xabc
            print("ERROR  : Border extraction does not work with DEFORM-3D V2.3 or earlier databases.\n"
                  "Generate a new DEFORM-3D database using the existing data, read in the new database \n"
                  "and perform border extraction again.\n");
            return false;
        }
    }
    const std::uint32_t rc = run_extraction(self, prob, step, obj);
    if (rc == 0) {
        print("         Success\n");
        return true;
    }
    std::string text; // local_1d8
    if (rc == 0x2711) {
        text = "Border extraction does not work with DEFORM-3D V2.3 or earlier databases. \n"
               "Generate a new DEFORM-3D database using the existing data, read in \n"
               "the new database and perform border extraction.\n\n";
    } else if (rc == 0x2712) {
        text = "Could not open the current database. Make sure the\n"
               "database exists in the current directory.\n\n";
    } else {
        text = "An error occurred when trying to extract the border for the object.\n\n";
    }
    std::fprintf(stdout, "ERROR  : %s (Code %d)\n", text.c_str(), rc);
    return false;
}

// @src 14013eb80  driver
// @src 14013d8d0  ctor of the one-byte local (returns this)
// @src 14013d8f0  its dtor (empty)
// @src 140158e40  *dst = *src (byte)
// @src 140158d40  return *this (byte) -- used in run_extraction
// @src 140158e20  obj+0x3f21 = flag
bool extract_border(MeshGenBase& mg)
{
    Cls_14013d8d0 self;              // local_117
    std::string db = "";             // local_140, str 14051bf16
    g_amg_dxt_ok = 0;                // 14013ebcd: byte store
    g_mesher_progress = 0xd;         // 14013ebd4
    const std::uint8_t zero = 0;     // local_116
    self.keep_usrwin = zero;         // 140158e40
    (void)object_is_3d(mg.u008);     // 140158ca0, result unused
    db = std::string(mg.u3f48->u1748);
    model::DB_STEP_DATA* m = mg.u3f48;
    const bool ok = extract(self, std::string(mg.u3f60), db, &m->u744, mg.u008);
    if (!ok) {
        g_amg_dxt_ok = 0;
        mg.u008->u3f21 = 0;
        remove_outputs(self, std::string(mg.u3f60), &mg.u3f48->u744);
    } else {
        g_amg_dxt_ok = 1;
        mg.u008->u3f21 = 1;
    }
    return true;
}

} // namespace brdext

// @src 1401a9ac0  BRDEXT / REMESH step 1: find the object, bind a _MeshGenBase, run the driver
bool brdext_object(model::DB_STEP_DATA& m, const std::uint32_t* obj)
{
    // 1401d6ea0 (P2; inlined as in defdbio p4-06): object_list_find(model + 0x1380, id)
    LLOBJ_DATA* o = model::object_list_find(&m.objects, static_cast<std::int32_t>(*obj));
    if (o == nullptr) {
        return false;
    }
    std::fprintf(stdout, "INFO   : Extracting border - Object %d...\n", *obj);
    MeshGenBase mg;                       // 14013e790 (dtor 14013e840)
    meshgen_bind(mg, o, &m, m.u340);      // 140148840(mg, obj, model, model + 0x340)
    (void)brdext::extract_border(mg);
    print("INFO   : Extracting border - Done\n");
    return true;
}

// part of @src 140378e00  case 0x11f+5 (BRDEXT)
int cmd_brdext(model::DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t id = 0;                               // auStack_500
    const int rc = def::key::read_i(f, &id, false);    // 140413de0(f, &id, 0)
    if (rc != 0) {
        return 2;
    }
    if (model::object_list_find(&m->objects, id) == nullptr) { // 1401d6ea0
        return 2;
    }
    const std::uint32_t uid = static_cast<std::uint32_t>(id);
    (void)brdext_object(*m, &uid);
    return rc == 0 ? 0 : 2;
}

} // namespace def::cmd
