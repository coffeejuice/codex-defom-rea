// MDT L6T: the HeatTreat save chain. vt+0x68 (0x1411b1320 / 0x1411aed00) runs 0x1411b71c0(sched, 0)
// (HT_Proc<k>.MOKEY files 0x1411b73c0, then the read-back and data check 0x1411b9080) and
// 0x1411b3b20 (HeatTreatWizard.HTWZ 0x1411b3c40); props vt+0xb8 (0x1411c0910) runs 0x1411b3b20.
// Decompiles: deform-rea-work/gui_mo/s1/mdt/L6/m0.c (0x1411b73c0, 0x1411bb040, 0x1411b3c40), s2.c
// (0x1411b9080), m1.c / m3.c / m4.c (helpers). Files go through the CRT as in the original
// (fopen "wt" / "rt" on scratch files) and are recorded in FileEffects.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include <defcmd/dispatch.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/layout/phase_pair_list.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defkey/kw_g57.hpp>
#include <defkey/kw_g2bc.hpp>
#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/material/material_ctor.hpp>
#include "m2m/mst/writer.hpp"
#include <defcmd/interp.hpp>

#include "engine_internal.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/engine.hpp"
#include "m2m/ht_schedule.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "save_l6.hpp"
#include "../datafiles/cogging_file.hpp"

namespace def::cmd::genbcc {
// defcmd/src/p67_5/genbcc_bc.hpp (private to defcmd)
std::int32_t object_geotyp(const def::model::LLOBJ_DATA& obj);   // 140212420
bool axis_search(def::model::LLOBJ_DATA& obj, std::int32_t* n_axis, std::int32_t* axis_nodes, std::int32_t* n_edges,
                 std::int32_t* edges);                             // 140206d50
std::int32_t edge_number_slot(const def::model::LLOBJ_DATA& obj, const std::int32_t* edge);   // 1403054e0
}  // namespace def::cmd::genbcc

namespace m2m::engine {
namespace {

using data::DYNARY;
using data::HTMedia;
using data::HTNodeEntry;
using data::HTSchedule;
using data::HTStep;
using def::model::Cls_1401e7b10;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using qt::L;
namespace crt = def::rt::crt;

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "HeatTreat save: " + what);
}

const char* kw(int id) { return def::model::keyword_name(id); }   // 0x140921f30 = 1402f7720

// QString::latin1() (Qt 3 support = toLatin1().constData()): code units above 0xff become '?'
std::string toLatin1(const String& s)
{
    std::string r;
    for (char16_t c : s.u16()) r += c > 0xff ? '?' : static_cast<char>(c);
    return r;
}

bool is2D(const GuiModel* m) { return m && m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 2; }
bool is3D(const GuiModel* m) { return m && m->step && *at<std::int32_t>(m->step.get(), 0x1c) == 3; }
int number(const LLOBJ_DATA* o) { return *at<std::int32_t>(o, 8); }

std::string readAll(const std::filesystem::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

std::filesystem::path scratchFile(const char* tag)
{
    return std::filesystem::temp_directory_path() / ("m2m_ht_" + std::string(tag) + "_" +
                                                      std::to_string(GetCurrentProcessId()) + "_" +
                                                      std::to_string(GetCurrentThreadId()) + ".tmp");
}

// fopen(local8(target), "wt") on a scratch file; the bytes are recorded for target. False when the
// original's fopen would fail (empty name).
bool writeText(const String& target, FileEffects& fx, const std::function<void(std::FILE*)>& body,
               const Ctx* ctx = nullptr)
{
    if (target.isEmpty()) return false;
    if (ctx && ctx->nativePropertyFiles) {
        // 0x1411b3d87: toLocal8Bit, unlike Cogging's QString constchar codec.
        datafiles::detail::CoggingFile file(qt::encodeLocale(target, ctx->codepage));
        if (!file.get()) return false;
        body(file.get());
        file.close(); // individual printf and fclose results are ignored
        return true;
    }
    const auto tmp = scratchFile("w");
    std::FILE* f = _wfopen(tmp.wstring().c_str(), L"wt");
    if (!f) return false;
    try {
        body(f);
    } catch (...) {
        std::fclose(f);
        std::error_code ec;
        std::filesystem::remove(tmp, ec);
        throw;
    }
    std::fclose(f);                                         // 0x14146fb28
    fx.writes.push_back({target, readAll(tmp)});
    std::error_code ec;
    std::filesystem::remove(tmp, ec);
    return true;
}

// the bytes a later fopen(path, "rt") reads: this save's own write of the file, else the disk
bool fileBytes(Project& p, const String& path, const FileEffects& fx, std::string& out)
{
    for (auto it = fx.writes.rbegin(); it != fx.writes.rend(); ++it)
        if (qt::cleanPath(it->path) == qt::cleanPath(path)) {
            out = it->bytes;
            return true;
        }
    return datafiles::readFileBytes(p, path, out);
}

// ---- schedule helpers (the reader's bodies, heattreat_htwz.cpp, except 0x1411c2860) -------------

// @src 0x1411c2860  medium i, or a new one appended (0x1411c2980) when i is not an index (0x1411c2920)
HTMedia* mediaAt(HTSchedule& s, int i)
{
    if (!s.media.empty() && i > -1 && i < static_cast<int>(s.media.size())) return s.media[static_cast<std::size_t>(i)].get();
    s.media.push_back(std::make_unique<HTMedia>());
    return s.media.back().get();
}

// @src 0x1411bc690 / 0x1411bc630  ECTMFN record (object, media), null when there is none
HTNodeEntry* findEctmfn(HTSchedule& s, int object, int media)
{
    const int i = ht::findEntryOf(s.ectmfn, object, media);   // 0x1411bc690
    if (i < 0 || i >= static_cast<int>(s.ectmfn.size())) return nullptr;
    return s.ectmfn[static_cast<std::size_t>(i)].get();
}

// @src 0x1408916a0 (= DEF_PRE 14021d7a0)  per boundary slot 1, except 0 on the edges off the
// symmetry axis (GEOTYP 1 or 3 only; else all 0). calloc'd ints (0x1409c8020), NBE of them.
std::int32_t* axisEdgeFlags(LLOBJ_DATA* o)
{
    const int nbe = *at<std::int32_t>(o, 0x88c);
    auto ints = [](int n, int extra) {
        return n == 0 ? nullptr : static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(n + extra), 4));
    };
    // the axis list can take NBE + 1 entries (the first edge adds both nodes): one spare int here,
    // where the original's NBE-int block would be overrun
    std::int32_t* axis = ints(nbe, 1);
    std::int32_t* edges = ints(nbe, 0);
    std::int32_t* flags = ints(nbe, 0);
    const int ns = def::model::bc::edge_face_slot_count(*o);   // 0x140839380
    if (ns > nbe) unsupported("object " + std::to_string(number(o)) + ": more boundary slots than NBE (0x1408916a0)");
    for (int i = 0; i < ns; ++i) flags[i] = 0;
    const int geo = def::cmd::genbcc::object_geotyp(*o);  // 0x140886280 = 140212420
    if (geo == 1 || geo == 3) {
        std::int32_t nAxis = 0, nEdges = 0;
        def::cmd::genbcc::axis_search(*o, &nAxis, axis, &nEdges, edges);   // 0x14087abb0 = 140206d50
        for (int i = 0; i < ns; ++i) flags[i] = 1;
        for (int i = 0; i < nEdges; ++i) {
            std::int32_t ed = edges[i];
            if (ed < 1 || ed > nbe) continue;
            const int slot = def::cmd::genbcc::edge_number_slot(*o, &ed);   // 0x140a9e730 = 1403054e0
            if (slot < 0 || slot > ns) continue;
            if (slot == ns) unsupported("axis edge slot " + std::to_string(slot) + " past the array (0x1408916a0)");
            flags[slot] = 0;
        }
    }
    std::free(axis);                                       // 0x141471b7c (only in the GEOTYP branch
    std::free(edges);                                      // in the original; the rest leaks)
    return flags;
}

// @src 0x140f8a560  the 2D object's boundary-edge ECC ints (element set +0x9e0, slot +0x10), built
// by 0x140891660 (0x1408916a0 when obj+0x39c0 == 2, else null) and stored (0x140f8a5b0) when missing
std::int32_t* objectEcc(LLOBJ_DATA* o)
{
    std::int32_t*& slot = *at<std::int32_t*>(o, 0x9e0 + 0x10);   // 0x140cd7b70; freed by 140182a50
    if (!slot) slot = *at<std::int32_t>(o, 0x39c0) == 2 ? axisEdgeFlags(o) : nullptr;
    return slot;
}

// The 3D face analysis of 0x1411ba540 and 0x1411b73c0: the value of node BC code 6 that all three
// first nodes of face k carry (+0x990 codes, +0x998 values, 3 per node), else 0
int faceBcValue(LLOBJ_DATA* o, int k)
{
    std::int32_t a = 0, b = 0, c = 0, d = 0;
    def::model::bc::face_nodes(*o, k, &a, &b, &c, &d);     // 0x140875070 = 140201210
    const std::int32_t* code = *at<std::int32_t*>(o, 0x990);
    const std::int32_t* val = *at<std::int32_t*>(o, 0x998);
    if (!code || !val)
        unsupported("object " + std::to_string(number(o)) + " has no node BC arrays (+0x990 / +0x998)");
    const int nn = *at<std::int32_t>(o, 0x880);
    auto inRange = [&](int n) {
        if (n < 1 || n > nn) unsupported("face node " + std::to_string(n) + " out of range (0x1411ba540)");
    };
    inRange(a);
    for (int e = 0; e < 3; ++e) {
        if (code[(a - 1) * 3 + e] != 6) continue;
        const int v = val[(a - 1) * 3 + e];
        unsigned cnt = 1;
        inRange(b);
        for (int g = 0; g < 3; ++g)
            if (code[(b - 1) * 3 + g] == 6 && val[(b - 1) * 3 + g] == v) {
                cnt = 2;
                break;
            }
        inRange(c);
        for (int g = 0; g < 3; ++g)
            if (code[(c - 1) * 3 + g] == 6 && val[(c - 1) * 3 + g] == v) {
                ++cnt;
                break;
            }
        if (cnt > 2) return v;
    }
    return 0;
}

// @src 0x1411bb040  (0x1411bae90 mode 1) 2D: ECC / ECTMFN records from the objects' boundary ECC
void fillFromModel(Project& p, HTSchedule& s)
{
    GuiModel* m = ht::scheduleModel(p, s);                 // 0x1411b3760
    if (!m || is3D(m) || !m->step) return;
    int n = static_cast<int>(s.ectmfn.size());             // 0x140159d00
    if (n == 0) n = m->step->u1378;                        // 0x140830180
    // the loop variable is reused by the inner loop below, as in the original
    for (int i = 0; i < n; ++i) {
        HTNodeEntry* e1 = ht::entryAtOf(s.ecc, i);
        HTNodeEntry* e2 = ht::entryAtOf(s.ectmfn, i);
        int obj = e2->object;
        if (obj == 0) {
            obj = i + 1;
            if (e1->media < 0) {
                e1->media = 0;
                e1->object = obj;
            }
            if (e2->media < 0) {
                e2->media = 0;
                e2->object = obj;
            }
            if (e1->object < 1) e1->object = obj;
            if (e2->object < 1) e2->object = obj;
        }
        LLOBJ_DATA* o = m->object(obj);                    // 0x140c7ae80
        if (!o || e1->media < 0 || e2->media < 0) continue;
        const std::int32_t* a = objectEcc(o);
        if (!a) continue;
        const int ns = def::model::bc::edge_face_slot_count(*o);   // 0x140839380
        for (i = 0; i < ns; ++i) {
            if (a[i] <= 0) continue;
            const int node = i + 1;
            ht::setPairOf(*e2, ht::findPairOf(*e2, node, a[i]), node, -a[i]);
            ht::setPairOf(*e1, ht::findPairOf(*e1, node, a[i]), node, 0);
        }
    }
}

// @src 0x1411ba540  ECTMFN pairs on faces / edges that carry an ECC value are dropped
void pruneEctmfn(Project& p, HTSchedule& s)
{
    GuiModel* m = ht::scheduleModel(p, s);
    if (!m || !m->step) return;
    for (int i = 0; i < static_cast<int>(s.ectmfn.size()); ++i) {
        HTNodeEntry* e = s.ectmfn[static_cast<std::size_t>(i)].get();
        LLOBJ_DATA* o = m->object(e->object);
        if (!o) continue;
        const int ns = def::model::bc::edge_face_slot_count(*o);
        std::vector<std::int32_t> local(static_cast<std::size_t>(ns), 0);   // 0x1409c8020 (zeroed)
        const std::int32_t* a = ns ? local.data() : nullptr;
        for (int k = 0; k < def::model::bc::edge_face_slot_count(*o); ++k)
            if (!is2D(m)) local[static_cast<std::size_t>(k)] = faceBcValue(o, k);
        if (is2D(m)) a = objectEcc(o);
        if (!a) continue;
        for (int j = static_cast<int>(e->values.size()); j > 0; --j) {
            const int node = e->values[static_cast<std::size_t>(j - 1)].first;   // 0x1411c2500
            if (node < 1 || node > ns) unsupported("ECTMFN node " + std::to_string(node) + " out of range (0x1411ba540)");
            if (a[node - 1] != 0) e->values.erase(e->values.begin() + (j - 1));   // 0x1411c3390
        }
    }
}

// @src 0x1411b37c0  the owner operation is the head of its chain (true without an owner)
bool ownerIsChainHead(HTSchedule& s)
{
    if (s.owner && s.owner->owner) return isChainHead(*s.owner->owner);   // 0x140050bc0
    return true;
}

// 0x1409dcb60  index of the pair (from, to) in a phase-pair list: the last index when none matches,
// -1 for an empty list
int pairIndex(const def::model::PhasePairList& l, int from, int to)
{
    int i = -1;
    for (const def::model::PhasePair* q = l.head; q; q = q->next) {
        ++i;
        if (q->from == from && q->to == to) break;
    }
    return i;
}

// @src 0x1411b73c0(sched, writeOne = 0, which = -1)  HT_Proc<k>.MOKEY for every step
bool writeProcFiles(Project& p, HTSchedule& s, FileEffects& fx)
{
    GuiModel* m = ht::scheduleModel(p, s);
    DB_STEP_DATA* e = m ? m->step.get() : nullptr;
    const bool head = ownerIsChainHead(s);                 // cStack_218
    pruneEctmfn(p, s);                                     // 0x1411ba540
    double sum = 0.0;                                      // dStack_1d0
    double acc = m ? *at<double>(m->simCtl(), 0x640) : 0.0;   // dStack_1e0: SC+0x640 TNOW 1
    for (int i = 0; i < data::htStepCount(s); ++i) {
        HTStep* st = ht::stepAtOf(s, i);
        int mi = st->media - 1;
        const double t = st->time;
        const double t3 = st->t3;
        const double t4 = st->t4;
        sum += t;
        acc += t;
        if (!(!s.media.empty() && mi + 1 > -1 && mi + 1 < static_cast<int>(s.media.size()))) {   // 0x1411c2920
            st->media = 0;
            mi = -1;
        }
        HTMedia* md = mediaAt(s, mi + 1);
        if (!md) continue;
        if (!m || !m->simCtl()) unsupported("HT_Proc MOKEY without an engine model (0x140c7b2d0 used unchecked)");
        Cls_1401e7b10* sc = m->simCtl();
        const String path = ht::procPath(s, i);
        writeText(path, fx, [&](std::FILE* f) {
            const std::string mname = local8(md->name);
            char title[0x40];
            std::snprintf(title, sizeof title, "Proc. %d: %s", i + 1, mname.c_str());   // 0x14146de38
            crt::fprintf(f, "%-6s\n%s\n", kw(0x19), title);
            const bool noTables = st->t20.ndim == 0 && st->t98.ndim == 0;   // 0x14089b800
            if (!head) {
                crt::fprintf(f, "%-6s %20.10E\n", kw(8), t);
                if (st->flag10 && !noTables) sum = acc;
            } else {
                if (!st->flag10 || noTables) crt::fprintf(f, "%-6s %20.10E\n", kw(0x2a), sum);
                else {
                    crt::fprintf(f, "%-6s %20.10E\n", kw(0x2a), t);
                    sum = acc;
                }
            }
            acc = sum;
            crt::fprintf(f, "%-6s %20.10E\n", kw(0x32), *at<double>(sc, 0x1d0));
            crt::fprintf(f, "*\n*  %s\n*\n", mname.c_str());
            if (!head) {
                if (!noTables) {
                    st->flag10 = true;                                     // 0x1411c2bf0(st, 1)
                    crt::fprintf(f, "%-6s %20.10E %20.10E %7i\n", kw(0x1f), -1e21, 0.0, st->flag10 ? 1 : 0);
                }
            } else if (noTables) {
                crt::fprintf(f, "%-6s %20.10E %20.10E %7i\n", kw(0x1f), -1e21, 0.0, 0);
            } else {
                crt::fprintf(f, "%-6s %20.10E %20.10E %7i\n", kw(0x1f), -1e21, 0.0, st->flag10 ? 1 : 0);
            }
            if (!md->radiation) crt::fprintf(f, "%-6s %20.10E\n", kw(0x4d), 0.0);
            else crt::fprintf(f, "%-6s %20.10E\n", kw(0x4d), *at<double>(sc, 0x4b8));
            if (st->t20.ndim != 0) {
                crt::fprintf(f, "%-6s %7i %7i\n", kw(0x4b), 1, st->t20.n[0]);
                def::key::write_table_body(f, &st->t20, 0);
            } else {
                crt::fprintf(f, "%-6s %7i %20.10E\n", kw(0x4b), 0, t3);
            }
            DYNARY* z0 = ht::zoneAtOf(*md, 0);
            if (z0->ndim == 0) crt::fprintf(f, "%-6s %7i %20.10E\n", kw(0x4e), 0, z0->value);
            else if (z0->ndim == 1 && z0->var[0] == 3) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x4e), 1, z0->n[0]);
            else crt::printf("ERROR in pDefaultHTZone");
            def::key::write_table_body(f, z0, 0);
            if (*at<std::int32_t>(sc, 0x110) != 0) {                       // 0x1408dfdc0
                if (st->t98.ndim != 0) {
                    crt::fprintf(f, "%-6s %7i %7i\n", kw(0x4f), 1, st->t98.n[0]);
                    def::key::write_table_body(f, &st->t98, 0);
                } else {
                    crt::fprintf(f, "%-6s %7i %20.10E\n", kw(0x4f), 0, t4);
                }
                DYNARY& a = md->acvcof;
                if (a.ndim == 0) crt::fprintf(f, "%-6s %7i %20.10E\n", kw(0x50), 0, a.value);
                else if (a.ndim == 1 && a.var[0] == 3) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x50), 1, a.n[0]);
                else if (a.ndim == 1 && a.var[0] == 0xc) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x50), 2, a.n[0]);
                else crt::printf("ERROR in pMediaItem->getSurfReaCoef()");
                def::key::write_table_body(f, &a, 0);
            }
            if (*at<std::int32_t>(sc, 0x10c) != 0) {                       // 0x1408dfda0
                for (const HTStep::Pair& q : st->pairs)
                    if (q.c == -1) crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0x86), q.a, q.b, 0);
                if (i > 0) {
                    HTStep* prev = ht::stepAtOf(s, i - 1);
                    for (const HTStep::Pair& q : prev->pairs) {
                        if (q.c != -1) continue;
                        const HTStep::Pair* r = nullptr;                    // 0x1409dcb00 on the step's list
                        for (const HTStep::Pair& x : st->pairs)
                            if (x.a == q.a && x.b == q.b) {
                                r = &x;
                                break;
                            }
                        if ((!r || r->c != -1) && e) {
                            def::model::PhasePair* rec = def::model::phase_pair_find(&e->phase_pairs, q.a, q.b);
                            const int idx = pairIndex(e->phase_pairs, q.a, q.b);
                            def::key::g57::write_phase_pair(e, f, rec, idx + 1);   // 0x140b523c0 = 1403cbf00
                        }
                    }
                }
            }
            int nobj = 1;
            LLOBJ_DATA* o = nullptr;
            if (e) {
                nobj = e->u1378;
                for (int oi = 0; oi < nobj; ++oi) {
                    o = m->objectAt(oi);                                    // 0x140c7aed0(m, oi)
                    if (!o) continue;
                    for (int zi = 1; zi < static_cast<int>(md->zones.size()); ++zi) {
                        DYNARY* z = ht::zoneAtOf(*md, zi);
                        int type = 0;
                        if (z->ndim == 0) type = 0;
                        else if (z->ndim == 1 && z->var[0] == 1) type = 1;
                        else if (z->ndim == 1 && z->var[0] == 3) type = 2;
                        const int tab = st->t20.ndim != 0 ? 1 : 0;
                        crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i\n", kw(0xe6), number(o), zi, 0, tab, type,
                                     0, 0);
                        if (st->t20.ndim != 0) def::key::write_table_body(f, &st->t20, 1);
                        else crt::fprintf(f, " %20.10E\n", t3);
                        def::key::write_table_body(f, z, 1);
                        crt::fprintf(f, " %20.10E\n", 0.0);
                        crt::fprintf(f, " %20.10E\n", 0.0);
                    }
                    HTNodeEntry* en = findEctmfn(s, number(o), st->media);   // 0x1411bc630
                    const int ns = def::model::bc::edge_face_slot_count(*o);
                    std::vector<std::int32_t> a138(static_cast<std::size_t>(ns), 2), a128(static_cast<std::size_t>(ns), 0),
                        a140(static_cast<std::size_t>(ns), 2), a120(static_cast<std::size_t>(ns), 0);
                    for (int k = 0; k < def::model::bc::edge_face_slot_count(*o); ++k)
                        if (!is2D(m)) a120[static_cast<std::size_t>(k)] = faceBcValue(o, k);
                    const std::int32_t* ecc = ns ? a120.data() : nullptr;
                    if (is2D(m)) ecc = objectEcc(o);
                    if (ecc) {
                        if (en)
                            for (const auto& pr : en->values) {
                                const int value = pr.second;               // 0x1411c2560
                                const int node = pr.first;                 // 0x1411c2500
                                if (value > 0 && node > 0) {
                                    if (node > ns) unsupported("ECTMFN node " + std::to_string(node) + " out of range");
                                    const auto nk = static_cast<std::size_t>(node - 1);
                                    if (ecc[node - 1] == 0) {
                                        a138[nk] = 4;
                                        a128[nk] = value;
                                    } else {
                                        a138[nk] = 0;
                                        a128[nk] = 0;
                                    }
                                }
                            }
                        for (int k = 0; k < ns; ++k)
                            if (ecc[k] != 0) {
                                const auto kk = static_cast<std::size_t>(k);
                                a138[kk] = 0;
                                a128[kk] = 0;
                                a140[kk] = 0;
                            }
                    }
                    def::key::g2bc::write_edge_face_int_line(f, kw(0xf2), static_cast<std::uint32_t>(number(o)), o,
                                                         ns ? a138.data() : nullptr, e->u15f8[68], 0);   // data+0x1708
                    def::key::g2bc::write_edge_face_int_line(f, kw(0xf5), static_cast<std::uint32_t>(number(o)), o,
                                                         ns ? a128.data() : nullptr, e->u15f8[69], 0);   // data+0x170c
                    if (*at<std::int32_t>(sc, 0x110) != 0)
                        def::key::g2bc::write_edge_face_int_line(f, kw(0xf3), static_cast<std::uint32_t>(number(o)), o,
                                                             ns ? a140.data() : nullptr, e->u15f8[71], 0);   // data+0x1714
                }
                if (*at<std::int32_t>(sc, 0x10c) != 0 && st->flag7) {        // 0x1411c2800
                    if (!o) unsupported("PHASE line after an empty object position (0x1411b73c0 reads it unchecked)");
                    crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(0xcf), number(o), 0, *at<std::int32_t>(e, 0x16c8),
                                 0.0);
                }
            }
        });
    }
    return true;
}

// @src 0x1411b9e70(sched, dialogs)  volume fractions of mixture materials; true = an error (the
// message box needs dialogs)
bool phaseDataError(Project& p, HTSchedule& s)
{
    GuiModel* m = ht::scheduleModel(p, s);
    if (!m || !m->step || !m->simCtl() || *at<std::int32_t>(m->simCtl(), 0x10c) == 0) return false;
    DB_STEP_DATA* e = m->step.get();
    const int n = e->u1378;
    for (int oi = 0; oi < n; ++oi) {
        LLOBJ_DATA* o = m->objectAt(oi + 1);
        if (!o || *at<std::int32_t>(o, 0xc) == 5) continue;
        const int matId = *at<std::int32_t>(o, 0x3980);       // 0x140876860
        def::model::Material* mat = m->material(matId);       // 0x140c7cb20
        if (!mat || *at<std::int32_t>(mat, 0x1cc8) != 1) continue;
        const int nph = *at<std::int32_t>(mat, 0x1ccc);
        if (nph < 1) return false;
        void* set = at<std::byte>(o, 0x9e0);
        if (!def::dbio::objcheck::elem_block_volume_fractions(set) || !def::cmd::cnvt3d_sweep::fun_140184370(set))
            return true;
        const std::int32_t* ph = *at<std::int32_t*>(mat, 0x1cd0);
        std::vector<int> col(static_cast<std::size_t>(nph));
        for (int k = 0; k < nph; ++k)
            col[static_cast<std::size_t>(k)] = def::model::material_list_index_of(&e->materials, ph[k]);   // 0x1408bc900
        for (int el = 0; el < *at<std::int32_t>(o, 0x884); ++el) {
            double a = 0.0, b = 0.0;
            for (int k = 0; k < nph; ++k) {
                const int idx = el * *at<std::int32_t>(e, 0x16c0) + col[static_cast<std::size_t>(k)];
                a += def::dbio::objcheck::elem_block_volume_fractions(set)[idx];
                b += def::cmd::cnvt3d_sweep::fun_140184370(set)[idx];
            }
            if (std::fabs(a - 1.0) >= 0.01) return true;     // @0x142203f60
            if (*at<std::int32_t>(e, 0x85c) != 1 && std::fabs(b - 1.0) >= 0.01) return true;
        }
    }
    return false;
}

// @src 0x1411b9080(sched, dialogs = 0)  the first step's MOKEY read into the owner's model (its
// phase pairs kept), the user keyword file read into a scratch model, the phase data check
bool checkFirstStep(Project& p, HTSchedule& s, const FileEffects& fx)
{
    GuiModel* m = ht::scheduleModel(p, s);
    if (!m || !m->step || !m->simCtl()) return false;
    DB_STEP_DATA* e = m->step.get();
    // the mixture-material scan and its two questions (phase transformation on / off) need dialogs
    def::model::PhasePairList keep;                       // 0x1409dc750(new 0x20, 0)
    if (e->phase_pairs.count > 0) def::cmd::phase_pair_list_copy(&keep, &e->phase_pairs);   // 0x1409dc7f0
    auto scratch = std::make_unique<DB_STEP_DATA>(3);      // 0x140821bf0(stack, 3)
    const int idx = s.first - 1;                           // 0x140514c40
    const String path = ht::procPath(s, idx);                  // 0x1411b38e0
    std::string bytes;
    if (!path.isEmpty() && fileBytes(p, path, fx, bytes)) {   // QFile::exists, fopen "rt"
        const auto tmp = scratchFile("r");
        { std::ofstream(tmp, std::ios::binary) << bytes; }
        int rc = 2;
        if (std::FILE* f = _wfopen(tmp.wstring().c_str(), L"rt")) {
            def::cmd::ModelKeyHooks hooks(e);
            rc = def::key::read_key_stream(f, hooks, stdout);   // 0x140896fc0 (= DEF_PRE 1401b6520)
            std::fclose(f);
        }
        std::error_code ec;
        std::filesystem::remove(tmp, ec);
        if (rc != 0) return false;                         // "ERROR reading file %s!" (dialog)
        def::cmd::phase_pair_list_copy(&e->phase_pairs, &keep);   // the model keeps its own pairs
        keep.clear();                                      // 0x1409dcbe0
    }
    HTStep* st = ht::stepAtOf(s, idx);
    const String uk = st->userKey;                         // 0x1412c4c80
    if (!uk.isEmpty() && !(uk == L("None"))) {
        int rc = 2;
        std::string ub;
        if (datafiles::readFileBytes(p, uk, ub)) {          // fopen(local8(uk), "rt")
            const auto tmp = scratchFile("u");
            { std::ofstream(tmp, std::ios::binary) << ub; }
            if (std::FILE* f = _wfopen(tmp.wstring().c_str(), L"rt")) {
                def::cmd::ModelKeyHooks hooks(scratch.get());
                rc = def::key::read_key_stream(f, hooks, stdout);
                std::fclose(f);
            }
            std::error_code ec;
            std::filesystem::remove(tmp, ec);
        }
        if (rc != 0) return false;                         // without dialogs the answer is "abort"
    }
    if (*at<std::int32_t>(m->simCtl(), 0x10c) != 0 && phaseDataError(p, s)) return false;   // 0x1411b9e70
    return true;
}

// @src 0x1411b3c40  HeatTreatWizard.HTWZ (fopen "wt")
int writeHtwz(HTSchedule& s, const String& taskDir, FileEffects& fx, const Ctx* ctx)
{
    const String path = qt::convertSeparators(taskDir + qt::kSep + L("HeatTreatWizard") + L(".HTWZ"));   // 0x1411b3820
    if (path.isEmpty()) return 0;
    const bool ok = writeText(path, fx, [&](std::FILE* f) {
        crt::fprintf(f, "HEAT_TREATMENT_SPECS_FILE_V3\n");
        const int nm = static_cast<int>(s.media.size());   // 0x1411c2840
        crt::fprintf(f, "NO_MEDIA   %d\n", nm);
        for (int i = 0; i < nm; ++i) {
            HTMedia* md = mediaAt(s, i);
            const auto name = ctx && ctx->nativePropertyFiles ? qt::encodeLocale(md->name, ctx->codepage)
                                                             : local8(md->name);
            crt::fprintf(f, "MEDIA    %d\n%s\n", i + 1, name.c_str());
            crt::fprintf(f, "NO_ZONES    %d\n", static_cast<int>(md->zones.size()));   // 0x1411c2710
            for (int z = 0; z < static_cast<int>(md->zones.size()); ++z) {
                crt::fprintf(f, "ZONE    %d\n", z);
                DYNARY* t = ht::zoneAtOf(*md, z);
                if (t->ndim == 0) crt::fprintf(f, "%-6s %7i %20.10E\n", kw(0x4e), 0, t->value);
                else if (t->var[0] == 3) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x4e), 1, t->n[0]);
                else if (t->var[0] == 1) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x4e), 2, t->n[0]);
                def::key::write_table_body(f, t, 0);       // 0x140923320 = 1402f3130
            }
            crt::fprintf(f, "RADIATION    %d\n", md->radiation ? 1 : 0);
            DYNARY& a = md->acvcof;
            if (a.ndim == 0) crt::fprintf(f, "%-6s %7i %20.10E\n", kw(0x50), 0, a.value);
            else if (a.var[0] == 3) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x50), 1, a.n[0]);
            else if (a.var[0] == 0xc) crt::fprintf(f, "%-6s %7i %7i\n", kw(0x50), 2, a.n[0]);
            def::key::write_table_body(f, &a, 0);
        }
        const int ne = static_cast<int>(s.ectmfn.size());  // 0x140159d00
        int k = 0;
        for (int i = 0; i < ne; ++i)
            if (!s.ectmfn[static_cast<std::size_t>(i)]->values.empty()) ++k;
        crt::fprintf(f, "NO_ECTMFN   %d\n", k);
        k = 0;
        for (int i = 0; i < ne; ++i) {
            HTNodeEntry* en = s.ectmfn[static_cast<std::size_t>(i)].get();
            if (en->values.empty()) continue;
            ++k;
            crt::fprintf(f, "ECTMFN_NO    %d\n", k);
            crt::fprintf(f, "OBJECT_NO    %d\n", en->object);
            crt::fprintf(f, "MEDIA_NO    %d\n", en->media);
            crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xf5), en->object, static_cast<int>(en->values.size()), 0);
            for (int j = 0; j < static_cast<int>(en->values.size()); ++j)
                crt::fprintf(f, "%d  %d  %d\n", j + 1, en->values[static_cast<std::size_t>(j)].first,
                             en->values[static_cast<std::size_t>(j)].second);
        }
        crt::fprintf(f, "NO_PROCS    %d\n", data::htStepCount(s));
        for (int i = 0; i < data::htStepCount(s); ++i) {
            HTStep* st = ht::stepAtOf(s, i);
            const int np = static_cast<int>(st->pairs.size());   // 0x1407d3070
            crt::fprintf(f, "PROC    %d   %10.5e   %d   %10.5e   %10.5e   %d   %d   %d   %d  %s   %d\n", i + 1, st->time,
                         st->media + 1, st->t3, st->t4, st->t20.n[0], st->t98.n[0], st->flag7 ? 1 : 0, np,
                         toLatin1(st->userKey).c_str(), st->flag10 ? 1 : 0);
            def::key::write_table_body(f, &st->t20, 0);
            def::key::write_table_body(f, &st->t98, 0);
            for (int j = 0; j < np; ++j)
                crt::fprintf(f, "%d  %d\n", st->pairs[static_cast<std::size_t>(j)].a, st->pairs[static_cast<std::size_t>(j)].b);
        }
        crt::fprintf(f, "START_OPTNO    %d\n", s.first);
    }, ctx);
    return ok ? 1 : 0;
}

}  // namespace

// @src 0x1411b3b20
void heatTreatWriteSchedule(Project& p, HTSchedule& s, FileEffects& fx, const Ctx* ctx)
{
    if (!s.owner || !s.owner->owner) return;               // -1
    const String dir = taskDirAbs(*s.owner->owner);        // 0x140050df0
    fillFromModel(p, s);                                   // 0x1411bae90(sched, 1)
    writeHtwz(s, dir, fx, ctx);                            // 0x1411b3c40
}

// @src 0x1411b1320 / 0x1411aed00  after the base 0x1405ef510: 0x1411b71c0(sched, 0), 0x1411b3b20
void heatTreatSaveSchedule(Project& p, Operation& op, FileEffects& fx)
{
    mst::OpClass* c = opBase(op);
    Operation* cur = c ? c->current : nullptr;            // 0x1405ef2b0
    if (!cur) return;
    auto* hp = dynamic_cast<HeatTreatProps*>(cur->props.get());   // TD 0x143a5b500
    if (!hp || !hp->data) return;
    HTSchedule& s = hp->data->schedule;                    // 0x141681de0
    // @src 0x1411b71c0
    GuiModel* m = ht::scheduleModel(p, s);
    if (m && m->step) {
        fillFromModel(p, s);                               // 0x1411bae90(sched, 1)
        if (writeProcFiles(p, s, fx)) checkFirstStep(p, s, fx);   // 0x1411b73c0, 0x1411b9080
    }
    heatTreatWriteSchedule(p, s, fx);                      // 0x1411b3b20
}

}  // namespace m2m::engine
