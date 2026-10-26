// HeatTreat schedule (SfOprHeatTreatProperties+0x68) reader and props refresh (owner MDT L6;
// docs/re/moproj2mst/mdt/L6/spec.md section 6). Decompiles: deform-rea-work/gui_mo/s1/mdt/L6/h0.c
// (0x1411b4ab0, 0x1411b5e50), h1.c / h2.c / h3.c (schedule helpers), d4.c / d5.c (engine defaults,
// props vt+0x68). The file is read with the CRT as the original does (fopen "rt", fscanf with the
// old CRT's format reading, fgets, strtok through the DEF_PRE KEY helpers, which are the twins of
// the GUI's 0x140af04f0 / 0x140af20b0 / 0x140af7030 / 0x140af73f0).
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defkey/node_elem_var.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "engine/engine_internal.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/engine.hpp"
#include "m2m/ht_schedule.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::data {

// @src 0x1411b3360  name "Media", radiation on, then one zone (0x1411c2690: new DYNARY, type 0)
HTMedia::HTMedia()
{
    zones.push_back(std::make_unique<DYNARY>());
}

}  // namespace m2m::data

namespace m2m::ht {
namespace {

using data::DYNARY;
using data::HTMedia;
using data::HTNodeEntry;
using data::HTSchedule;
using data::HTStep;
using def::model::Cls_1401e7b10;
using def::model::LLOBJ_DATA;
using engine::at;
using engine::GuiModel;
namespace crt = def::key::crt;

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "HeatTreatWizard.HTWZ: " + what);
}

bool eq(const char* a, const char* b) { return std::strcmp(a, b) == 0; }   // 0x14146f040
int number(const LLOBJ_DATA* o) { return *at<std::int32_t>(o, 8); }

bool is2D(const GuiModel* m) { return m && m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 2; }   // 0x140c7c0e0
bool is3D(const GuiModel* m) { return m && m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 3; }   // 0x140c7c0a0

// @src 0x1411c2730  zone j of a medium, appended (new DYNARY) when j is not an index
DYNARY* zoneAt(HTMedia& md, int j)
{
    if (!md.zones.empty() && j > -1 && j < static_cast<int>(md.zones.size()))
        return md.zones[static_cast<std::size_t>(j)].get();
    md.zones.push_back(std::make_unique<DYNARY>());        // 0x1411c2690
    return md.zones.back().get();
}

// @src 0x1411c2c90 / 0x1411c28c0  record i of the ECC (+0x10) / ECTMFN (+0x18) list, appended
// (0x1411c2d50 / 0x1411c2a60, ctor 0x1411c2ae0) when i is not an index
HTNodeEntry* entryAt(std::vector<std::unique_ptr<HTNodeEntry>>& l, int i)
{
    if (!l.empty() && i > -1 && i < static_cast<int>(l.size())) return l[static_cast<std::size_t>(i)].get();
    l.push_back(std::make_unique<HTNodeEntry>());
    return l.back().get();
}

// @src 0x1411bc580 / 0x1411bc690  index of the record (object, media), -1 for an empty list, the
// count when none matches
int findEntry(const std::vector<std::unique_ptr<HTNodeEntry>>& l, int object, int media)
{
    const int n = static_cast<int>(l.size());
    if (n < 1) return -1;
    int i = 0;
    for (; i < n; ++i)
        if (l[static_cast<std::size_t>(i)]->object == object && l[static_cast<std::size_t>(i)]->media == media) return i;
    return i;
}

// @src 0x1411b3230  index of the pair (node, value), -1 for an empty list, the count otherwise
int findPair(const HTNodeEntry& e, int node, int value)
{
    const int n = static_cast<int>(e.values.size());
    if (n < 1) return -1;
    int i = 0;
    for (; i < n; ++i)
        if (e.values[static_cast<std::size_t>(i)].first == node && e.values[static_cast<std::size_t>(i)].second == value)
            return i;
    return i;
}

// @src 0x1411b32e0  pair idx <- (node, value), appended when idx is not an index
void setPair(HTNodeEntry& e, int idx, int node, int value)
{
    if (!e.values.empty() && idx > -1 && idx < static_cast<int>(e.values.size()))
        e.values[static_cast<std::size_t>(idx)] = {node, value};   // 0x1411c3340
    else
        e.values.emplace_back(node, value);                         // 0x1411c2620
}

// @src 0x1411bc740 (ECC list) / 0x1411bc7e0 (ECTMFN list)
void addNodeValue(std::vector<std::unique_ptr<HTNodeEntry>>& l, int object, int media, int node, int value)
{
    HTNodeEntry* e = entryAt(l, findEntry(l, object, media));
    e->object = object;                                   // 0x140d33b00
    e->media = media;                                     // 0x1413fd9d0
    setPair(*e, findPair(*e, node, value), node, value);
}

// @src 0x1411bbe10  append a medium; an empty name becomes "Media %d" (count after the append)
HTMedia* addMedia(HTSchedule& s, const qt::String& name)
{
    s.media.push_back(std::make_unique<HTMedia>());       // 0x1411c2980
    HTMedia* md = s.media.back().get();
    qt::String n = name;
    if (n.isEmpty()) {
        char buf[32];
        std::snprintf(buf, sizeof buf, "Media %d", static_cast<int>(s.media.size()));   // @0x142ca5c88
        n = qt::String::latin1(buf);
    }
    md->name = n;                                         // 0x140dfbd00
    return md;
}

// @src 0x1411b3710  media, ECC and ECTMFN lists emptied
void clearLists(HTSchedule& s)
{
    s.media.clear();
    s.ecc.clear();
    s.ectmfn.clear();
}

// @src 0x1411b0a90  step i, appended (0x1411b0b50, ctor 0x1411b3020) when i is not an index
HTStep* stepAt(HTSchedule& s, int i)
{
    if (!s.steps.empty() && i > -1 && i < static_cast<int>(s.steps.size())) return s.steps[static_cast<std::size_t>(i)].get();
    s.steps.push_back(std::make_unique<HTStep>());
    return s.steps.back().get();
}

// @src 0x14089ba40 (= DEF_PRE 14012fcd0): ndim, n = 0, re-allocate (14012fad0)
void setNdim(DYNARY* t, int ndim)
{
    t->ndim = ndim;
    t->n[2] = 0;
    t->n[1] = 0;
    t->n[0] = 0;
    def::model::dynary_alloc(t);
}

// zone CNVCOF (type 1) and medium ACVCOF (type 0x1e) records: 0x14089b9e0 (= 14012fc70), the
// shape by the record type (2 = var 1 for CNVCOF, var 0xc for ACVCOF), then 0x140af73f0
void coefficient(std::FILE* f, DYNARY* t, int tableType, int var2)
{
    std::int32_t type = 0;
    double val = 0.0;
    def::key::read_i_d(f, &type, &val);                   // 0x140af20b0 = 1404154c0
    def::model::dynary_reinit_keep(t, tableType);
    if (type == 0) {
        t->ndim = 0;
        t->value = val;
    } else if (type == 1) {
        t->ndim = 1;
        t->var[0] = 3;
        t->n[0] = static_cast<std::int32_t>(val);
    } else if (type == 2) {
        t->ndim = 1;
        t->var[0] = var2;
        t->n[0] = static_cast<std::int32_t>(val);
    }
    def::key::read_table_body(f, t, 0);                   // 0x140af73f0 = 14041a800
}

// QString(const char*) with every '\n' removed (fgets line of a medium name)
qt::String nameLine(const char* buf)
{
    std::string s(buf);
    std::string out;
    for (char c : s)
        if (c != '\n') out += c;
    return qt::String::latin1(out);
}

// NO_PROCS records and START_OPTNO (the same code ends 0x1411b4ab0 and 0x1411b5e50); false when
// the PROC keyword is missing (the caller returns 0)
bool readProcs(HTSchedule& s, std::FILE* f, char* buf, int nProcs, qt::String& keyTok)
{
    s.steps.clear();                                      // 0x14020db90(sched+0x28)
    for (int i = 0; i < nProcs; ++i) {
        HTStep* st = stepAt(s, i);
        if (!st) continue;
        crt::fscanf(f, "%s", buf);
        if (!eq(buf, "PROC")) return false;
        int cnt = 0;
        def::key::read_tokens(f, &cnt, 0xb);              // 0x140af04f0
        const double t1 = std::atof(def::key::token(1));  // 0x1414737a4
        const int md = std::atoi(def::key::token(2)) - 1; // 0x1414737ac
        const double t3 = std::atof(def::key::token(3));
        const double t4 = std::atof(def::key::token(4));
        if (t1 >= 0.0) st->time = t1;                     // 0x1411c2b30 (@0x1421ad1d0 = 0.0)
        st->media = md;                                   // 0x141371aa0
        st->t3 = t3;                                      // 0x1411c2b60: +0x10 and +0x28
        st->t20.value = t3;
        if (t4 >= 0.0) st->t4 = t4;                       // 0x1411c2b90: +0x18 when >= 0, +0xa0
        st->t98.value = t4;
        int r = std::atoi(def::key::token(5));
        if (r < 1) setNdim(&st->t20, 0);
        else {
            setNdim(&st->t20, 1);
            st->t20.var[0] = 1;
            st->t20.n[0] = r;
            def::key::read_table_body(f, &st->t20, 0);
        }
        r = std::atoi(def::key::token(6));
        if (r < 1) setNdim(&st->t98, 0);
        else {
            setNdim(&st->t98, 1);
            st->t98.var[0] = 1;
            st->t98.n[0] = r;
            def::key::read_table_body(f, &st->t98, 0);
        }
        st->flag7 = std::atoi(def::key::token(7)) != 0;   // 0x1411c2bd0
        st->pairs.clear();                                // 0x1409dcbe0(step+0x120)
        const int n8 = std::atoi(def::key::token(8));
        std::int32_t a = 0, b = 0;
        for (int c = 0; c < n8; ++c) {
            crt::fscanf(f, "%d %d\n", &a, &b);
            st->pairs.push_back({a, b, -1});              // 0x1409dc8b0
        }
        keyTok = qt::String::latin1(def::key::token(9));  // QString::operator=(const char*)
        st->userKey = keyTok.isEmpty() ? qt::String(u"None") : keyTok;   // 0x1411c2c10, @0x142ca53fc
        st->flag10 = std::atoi(def::key::token(10)) != 0; // 0x1411c2bf0
    }
    std::int32_t k = 0;
    crt::fscanf(f, "%s  %d\n", buf, &k);
    if (eq(buf, "START_OPTNO") && k - 1 > -1 && k - 1 < static_cast<int>(s.steps.size()))   // 0x1411c2c50
        s.first = k;
    return true;
}

// @src 0x1411b5e50  V1 / V2 files (HEAT_TREATMENT_SPECS_FILE[_2D_V2|_3D|_3D_V2]); needs the model,
// its data and object objNo. byNodeCount (param_4) is 0 from the only caller.
int readV12(HTSchedule& s, std::FILE* f, int objNo, bool byNodeCount, GuiModel* m)
{
    if (!m || !m->step) return 0;
    def::model::DB_STEP_DATA* e = m->step.get();
    LLOBJ_DATA* o = m->object(objNo);                      // 0x140830270
    if (!o) return 0;
    const int ns = def::model::bc::edge_face_slot_count(*o);   // 0x140839380 = 140165170
    const int nobj = e->u1378;                             // 0x140830180
    std::rewind(f);                                        // 0x141470298
    char buf[0x400] = {};                                  // auStack_588 (the original's is not cleared)
    crt::fscanf(f, "%s\n", buf);
    int ver = 1;
    if (!is2D(m)) {
        if (!is3D(m)) return 0;
        if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_3D")) ver = 1;
        else if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_3D_V2")) ver = 2;
    } else {
        if (eq(buf, "HEAT_TREATMENT_SPECS_FILE")) ver = 1;
        else if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_2D_V2")) ver = 2;
    }
    std::int32_t nMedia = 0, nZones = 0, k = 0;
    crt::fscanf(f, "%s  %d\n", buf, &nMedia);
    if (!eq(buf, "NO_MEDIA")) return 0;
    clearLists(s);
    auto addFor = [&](const std::vector<std::int32_t>& arr, int i) {
        for (int kk = 0; kk < ns; ++kk) {
            const int v = arr[static_cast<std::size_t>(kk)];
            if (v <= 0) continue;
            if (!byNodeCount) {
                addNodeValue(s.ecc, number(o), i, kk + 1, v);
                addNodeValue(s.ectmfn, number(o), i, kk + 1, v);
            } else {
                for (int q = 1; q <= nobj; ++q) {
                    LLOBJ_DATA* oq = e->objects.at(q);     // 0x1408302a0
                    if (oq && def::model::bc::edge_face_slot_count(*oq) == ns) {
                        addNodeValue(s.ecc, number(oq), i, kk + 1, v);
                        addNodeValue(s.ectmfn, number(oq), i, kk + 1, v);
                    }
                }
            }
        }
    };
    for (int i = 0; i < nMedia; ++i) {
        crt::fscanf(f, "%s   %d\n", buf, &k);
        if (!eq(buf, "MEDIA") || k != i + 1) return 0;
        std::fgets(buf, 0x400, f);
        HTMedia* md = addMedia(s, nameLine(buf));
        crt::fscanf(f, "%s   %d\n", buf, &nZones);
        if (!eq(buf, "NO_ZONES")) return 0;
        for (int j = 0; j < nZones; ++j) {
            crt::fscanf(f, "%s   %d\n", buf, &k);
            if (!eq(buf, "ZONE") || k != j) return 0;
            DYNARY* z = zoneAt(*md, j);
            if (is2D(m) && ver == 1)
                unsupported("2D version-1 zone region (0x140a9e8f0 = DEF_PRE 1403056a0 is not ported)");
            crt::fscanf(f, "%s", buf);
            if (!eq(buf, "CNVCOF")) return 0;
            coefficient(f, z, 1, 1);
        }
        if (ver == 2) {
            crt::fscanf(f, "%s %d", buf, &k);
            if (!eq(buf, "ECCTMP")) return 0;
            const int w1 = e->u15f8[68];                   // data+0x1708
            if (ns > 0 && w1 > 1)
                unsupported("ECCTMP of " + std::to_string(w1) + " values per face into a " + std::to_string(ns) +
                            "-int buffer (0x1409c8020 allocates one int per face)");
            std::vector<std::int32_t> a1(static_cast<std::size_t>(ns));
            if (def::key::read_edge_face_ints(f, ns ? a1.data() : nullptr, o, w1) != 0) return 0;   // 0x140af7030
            crt::fscanf(f, "%s %d", buf, &k);
            if (!eq(buf, "ECTMFN")) return 0;
            const int w2 = e->u15f8[69];                   // data+0x170c
            if (ns > 0 && w2 > 1)
                unsupported("ECTMFN of " + std::to_string(w2) + " values per face into a " + std::to_string(ns) +
                            "-int buffer (0x1409c8020 allocates one int per face)");
            std::vector<std::int32_t> a2(static_cast<std::size_t>(ns));
            if (def::key::read_edge_face_ints(f, ns ? a2.data() : nullptr, o, w2) != 0) return 0;
            addFor(a2, i);
            crt::fscanf(f, "%s %d\n", buf, &k);
            if (!eq(buf, "RADIATION")) return 0;
            md->radiation = k != 0;                        // 0x1411c2b10
            crt::fscanf(f, "%s", buf);
            if (!eq(buf, "ACVCOF")) return 0;
            coefficient(f, &md->acvcof, 0x1e, 0xc);
        }
    }
    std::int32_t nProcs = 0;
    crt::fscanf(f, "%s  %d\n", buf, &nProcs);
    if (!eq(buf, "NO_PROCS")) return 0;
    qt::String keyTok;
    readProcs(s, f, buf, nProcs, keyTok);
    return 0;                                              // void in the original; the caller's value
}

}  // namespace

// public names of the helpers above for the save side (src/engine/ht_save.cpp)
HTStep* stepAtOf(HTSchedule& s, int i) { return stepAt(s, i); }
DYNARY* zoneAtOf(HTMedia& md, int j) { return zoneAt(md, j); }
HTNodeEntry* entryAtOf(std::vector<std::unique_ptr<HTNodeEntry>>& l, int i) { return entryAt(l, i); }
int findEntryOf(const std::vector<std::unique_ptr<HTNodeEntry>>& l, int object, int media) { return findEntry(l, object, media); }
int findPairOf(const HTNodeEntry& e, int node, int value) { return findPair(e, node, value); }
void setPairOf(HTNodeEntry& e, int idx, int node, int value) { setPair(e, idx, node, value); }

// @src 0x1411b4ab0
int readFile(HTSchedule& s, std::FILE* f, int objNo, GuiModel* m)
{
    std::int32_t nMedia = 0, nZones = 0, k = 0;
    int ver = 1;
    char buf[0x400] = {};                                  // auStack_5c8 (not cleared in the original)
    qt::String keyTok;                                     // auStack_1a8
    crt::fscanf(f, "%s\n", buf);
    if (!m) {
        if (!eq(buf, "HEAT_TREATMENT_SPECS_FILE_V3")) return 0;
        ver = 3;
    } else {
        if (!is2D(m)) {
            if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_3D")) ver = 1;
            else if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_3D_V2")) ver = 2;
        } else {
            if (eq(buf, "HEAT_TREATMENT_SPECS_FILE")) ver = 1;
            else if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_2D_V2")) ver = 2;
        }
        if (eq(buf, "HEAT_TREATMENT_SPECS_FILE_V3")) ver = 3;
    }
    if (ver < 3) return readV12(s, f, objNo, false, m) != 0;
    crt::fscanf(f, "%s  %d\n", buf, &nMedia);
    if (!eq(buf, "NO_MEDIA")) return 0;
    clearLists(s);
    for (int i = 0; i < nMedia; ++i) {
        crt::fscanf(f, "%s   %d\n", buf, &k);
        if (!eq(buf, "MEDIA")) return 0;
        if (k != i + 1) return 0;
        std::fgets(buf, 0x400, f);
        HTMedia* md = addMedia(s, nameLine(buf));
        crt::fscanf(f, "%s   %d\n", buf, &nZones);
        if (!eq(buf, "NO_ZONES")) return 0;
        for (int j = 0; j < nZones; ++j) {
            crt::fscanf(f, "%s   %d\n", buf, &k);
            if (!eq(buf, "ZONE")) return 0;
            if (k != j) return 0;
            DYNARY* z = zoneAt(*md, j);
            crt::fscanf(f, "%s", buf);
            if (!eq(buf, "CNVCOF")) return 0;
            coefficient(f, z, 1, 1);
        }
        crt::fscanf(f, "%s %d\n", buf, &k);
        if (!eq(buf, "RADIATION")) return 0;
        md->radiation = k != 0;
        crt::fscanf(f, "%s", buf);
        if (!eq(buf, "ACVCOF")) return 0;
        coefficient(f, &md->acvcof, 0x1e, 0xc);
    }
    std::int32_t nEct = 0;
    crt::fscanf(f, "%s  %d\n", buf, &nEct);
    if (!eq(buf, "NO_ECTMFN")) return 0;
    for (int i = 0; i < nEct; ++i) {
        crt::fscanf(f, "%s   %d\n", buf, &k);
        if (!eq(buf, "ECTMFN_NO")) return 0;
        if (k != i + 1) return 0;
        crt::fscanf(f, "%s   %d\n", buf, &k);
        if (!eq(buf, "OBJECT_NO")) return 0;
        std::int32_t objN = k;                             // iStack_1b8
        crt::fscanf(f, "%s   %d\n", buf, &k);
        if (!eq(buf, "MEDIA_NO")) return 0;
        const std::int32_t mediaN = k;                     // iStack_190
        if (!m) unsupported("ECTMFN record without an engine model (0x140c7ae80 on a null model)");
        LLOBJ_DATA* o = m->object(objN);                   // 0x140c7ae80
        HTNodeEntry* en = entryAt(s.ectmfn, i);            // 0x1411c28c0
        en->object = objN;
        en->media = mediaN;
        int ns = 0;
        if (o && engine::objHasMesh(o)) ns = def::model::bc::edge_face_slot_count(*o);   // 0x140834c80, 0x140839380
        std::int32_t rows = 0;                             // iStack_158
        crt::fscanf(f, "%s   %d   %d   %d", buf, &objN, &rows, &k);
        if (!eq(buf, def::model::keyword_name(0xf5))) return 0;   // 0x140921f30: ECTMFN
        if (objN != en->object) return 0;                  // 0x1413ee240
        bool ok = true;
        std::int32_t node = 0, value = 0;                  // iStack_1c8, aiStack_19c (not cleared in the original)
        for (int r = 0; r < rows; ++r) {
            crt::fscanf(f, "%d  %d  %d\n", &k, &node, &value);
            if (value == 0) continue;
            if (ns < 1 || node < 1 || ns < node) {
                if (ns == 0 && node > 0) setPair(*en, -1, node, value);
                else ok = false;
            } else {
                setPair(*en, -1, node, value);
            }
        }
        if (!ok) en->values.clear();                       // 0x1411c3210(entry+8)
    }
    std::int32_t nProcs = 0;
    crt::fscanf(f, "%s  %d\n", buf, &nProcs);
    if (!eq(buf, "NO_PROCS")) return 0;
    if (!readProcs(s, f, buf, nProcs, keyTok)) return 0;
    return 1;
}

int readBytes(HTSchedule& s, const std::string& bytes, GuiModel* m)
{
    const std::filesystem::path tmp = std::filesystem::temp_directory_path() /
                                      ("m2m_htwz_" + std::to_string(GetCurrentProcessId()) + "_" +
                                       std::to_string(GetCurrentThreadId()) + ".HTWZ");
    {
        std::ofstream out(tmp, std::ios::binary);
        out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    }
    std::FILE* f = _wfopen(tmp.wstring().c_str(), L"rt");
    int r = 0;
    if (f) {
        try {
            r = readFile(s, f, 1, m);
        } catch (...) {
            std::fclose(f);
            std::error_code ec;
            std::filesystem::remove(tmp, ec);
            throw;
        }
        std::fclose(f);                                    // 0x14146fb28
    }
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    return r;
}

GuiModel* scheduleModel(Project& p, HTSchedule& s)
{
    if (!s.owner || !s.owner->owner) return nullptr;      // +0x30, 0x1405f8d90
    return engine::opModel(p, *s.owner->owner);           // 0x14005f770
}

void engineDefaults(Project& p, HTSchedule& s)
{
    GuiModel* m = scheduleModel(p, s);
    if (!m || !m->step) return;
    Cls_1401e7b10* sc = &m->step->sim_ctrl;                // data+0x750
    sc->trans[0] = 1;                                      // data+0x854
    sc->trans[1] = 0;
    sc->trans[2] = 0;
    sc->trans[3] = 0;
    sc->trans[4] = 0;                                      // data+0x864
    *at<std::int32_t>(sc, 0x118) = 0;                      // data+0x868 HTMTHD
    *at<std::int32_t>(sc, 0x16c) = 999999;                 // data+0x8bc
    *at<std::int32_t>(sc, 0x168) = 1;                      // data+0x8b8 PDIE
    *at<double>(sc, 0x178) = 1000.0;                       // data+0x8c8 (@0x1421d94e0)
    *at<std::int32_t>(sc, 0x1c8) = 1;                      // data+0x918 STPDEF
    *at<std::int32_t>(sc, 0x240) = 1;                      // 0x1408da6f0(SC, &1)
    *at<double>(sc, 0x1d0) = 0.001;                        // data+0x920 (@0x1421b5050)
    *at<double>(sc, 0x270) = 5.0;                          // data+0x9c0 (@0x1421d3c60)
    *at<double>(sc, 0x2a0) = 0.001;                        // data+0x9f0
    *at<double>(sc, 0x2a8) = 10.0;                         // data+0x9f8 (@0x142142078)
    *at<std::int32_t>(sc, 0x2e4) = 1;                      // data+0xa34 (2D and 3D store the same)
    *at<std::int32_t>(sc, 0x2e0) = 1;                      // data+0xa30
    std::strcpy(at<char>(sc, 0), "HEAT TREATMENT");        // 0x14146fec0(data+0x750, @0x142ca4fd0)
    s.engineDefaults = true;
}

int readWizard(Project& p, HTSchedule& s, const qt::String& taskDir)
{
    if (!s.engineDefaults) engineDefaults(p, s);          // 0x14172d790 / 0x1411b3540
    const qt::String name = qt::L("HeatTreatWizard") + qt::L(".HTWZ");   // 0x1411b3820
    const qt::String path = qt::convertSeparators(taskDir + qt::kSep + name);
    if (path.isEmpty()) return 0;
    std::string bytes;
    if (!datafiles::readFileBytes(p, path, bytes)) return 0;   // fopen(path, "rt") == null
    return readBytes(s, bytes, scheduleModel(p, s)) != 0;     // 0x1411b4ab0(sched, f, 1)
}

int loadSchedule(Project& p, HTSchedule& s)
{
    if (!s.owner || !s.owner->owner) return -1;
    return readWizard(p, s, taskDirAbs(*s.owner->owner)); // 0x140050df0
}

bool propsFromSimCtl(Project& p, HeatTreatProps& hp, void* simCtl)
{
    auto* sc = static_cast<Cls_1401e7b10*>(simCtl);
    if (!sc) return false;
    if (hp.data && !hp.data->schedule.engineDefaults) engineDefaults(p, hp.data->schedule);
    data::HTPropsBlock& b = *hp.block;
    b.b0 = *at<std::int32_t>(sc, 0x108) != 0;              // 0x1408dfd80
    b.b1 = *at<std::int32_t>(sc, 0x110) != 0;              // 0x1408dfdc0
    b.b2 = *at<std::int32_t>(sc, 0x10c) != 0;              // 0x1408dfda0
    b.stpdef = *at<std::int32_t>(sc, 0x1c8) == 1 ? 0 : 1;
    b.dtmax = *at<double>(sc, 0x1d0);
    b.dtpmax1 = *at<double>(sc, 0x270);
    b.dtpmax2 = *at<double>(sc, 0x2a0);
    b.f28 = *at<double>(sc, 0x2a8);
    b.stepIncr = *at<std::int32_t>(sc, 0x164);
    return true;
}

void propsRefresh(Project& p, HeatTreatProps& hp)
{
    if (!hp.data || !hp.block) unsupported("HeatTreat properties without their blocks (+0x58, +0x68)");
    if (hp.owner)                                          // 0x1405f8d90
        if (GuiModel* m = engine::opModel(p, *hp.owner))   // 0x14005f7e0
            if (Cls_1401e7b10* sc = m->simCtl()) propsFromSimCtl(p, hp, sc);   // vt+0x68 = 0x1411c0730
    // 0x1411c0980: the Process<i> nodes removed (L2), the node-pointer lists (+0x60 .. +0x98) and
    // value lists emptied
    scheduleNodesRemove(hp);
    data::HTPropsBlock& b = *hp.block;
    b.time.clear();
    b.media.clear();
    b.t3.clear();
    b.cnvcof.clear();                                      // +0xb8 (L2)
    b.t4.clear();
    b.acvcof.clear();                                      // +0xc8 (L2)
    b.mediaFlag.clear();
    HTSchedule& s = hp.data->schedule;
    const int n = data::htStepCount(s);                    // 0x1411b0a70
    for (int i = 0; i < n; ++i) {
        HTStep* st = stepAt(s, i);                         // 0x1411b0a90
        int mi = st->media;                                // 0x140cbc690
        if (!(!s.media.empty() && mi > -1 && mi < static_cast<int>(s.media.size()))) {   // 0x1411c2920
            mi = 0;
            st->media = 0;                                 // 0x141371aa0
        }
        // 0x1411c2860: medium mi, or a new one appended (0x1411c2980) when there is none
        HTMedia* md = (!s.media.empty() && mi > -1 && mi < static_cast<int>(s.media.size()))
                          ? s.media[static_cast<std::size_t>(mi)].get()
                          : (s.media.push_back(std::make_unique<HTMedia>()), s.media.back().get());
        b.time.push_back(st->time);                        // +0xa0 step+0x00 (0x1414030d0)
        b.media.push_back(md->name);                       // +0xa8 (0x1403047c0)
        b.t3.push_back(st->t3);                            // +0xb0 step+0x10 (0x140c4d920)
        if (b.b1) b.t4.push_back(st->t4);                  // +0xc0 step+0x18 (0x1411c27f0)
        b.mediaFlag.push_back(md->radiation);              // +0xd0 medium+0x08 (0x14075dc00)
    }
    // second loop: the Process<i> nodes under the schedule group (0x140375a70; L2)
    for (int i = 0; i < n; ++i) scheduleNodesAdd(hp, i, *stepAt(s, i));
    b.stepCount = n;                                       // +0x04
}

}  // namespace m2m::ht
