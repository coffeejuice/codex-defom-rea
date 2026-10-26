// dbinfo: print the header, step directory and step contents of a DEFORM .DB file.
//   dbinfo <file.DB>                    header + directory
//   dbinfo <file.DB> --step N [--regions] [--dyn]
//   dbinfo <file.DB> --all [--threads T]   decode every step, report timing
#include <chrono>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>

#include "deformdb/database.hpp"
#include "deformdb/model.hpp"

using namespace deformdb;

static void print_header(const Database& db) {
    const auto& h = db.header();
    std::printf("signature : %s (family %d)\n", h.signature.c_str(), static_cast<int>(h.family));
    std::printf("title     : %s\n", h.title.c_str());
    std::printf("version   : %s -> %d\n", h.version_text.c_str(), h.version);
    std::printf("dates     : %s | %s | %s\n", h.dates[0].c_str(), h.dates[1].c_str(), h.dates[2].c_str());
    std::printf("words29-34: %d %d %d %d %d %d\n", h.words[0], h.words[1], h.words[2], h.words[3], h.words[4],
                h.words[5]);
    std::printf("file size : %llu bytes, %zu directory entries\n",
                static_cast<unsigned long long>(db.file_size()), db.entries().size());
}

static void print_directory(const Database& db) {
    std::printf("%5s %6s %5s %6s %8s %8s %4s %4s %3s %6s %10s %10s %10s %10s\n", "slot", "step", "mesh", "hsize",
                "marsize", "asize", "sim", "mode", "dim", "ver", "rec_dyn", "rec_hdr", "rec_mar", "rec_a");
    for (const auto& e : db.entries())
        std::printf("%5d %6d %5d %6d %8d %8d %4d %4d %3d %6d %10d %10d %10d %10d\n", e.slot, e.step, e.mesh_no,
                    e.header_size, e.mar_size, e.a_size, e.sim_no, e.sim_mode, e.dim, e.version, e.rec_dynamic,
                    e.rec_header, e.rec_mar, e.rec_a);
}

static void print_objects(const StepData& s) {
    StepModel m(s);
    if (!m.supported()) { std::printf("typed view not supported for this step\n"); return; }
    std::printf("NSTART %d MESHNO %d time %.6g NUMNP %d NUMEL %d objects %d\n", m.nstart(), m.meshno(), m.time(),
                m.numnp(), m.numel(), m.object_count());
    std::printf("MAR widths:");
    for (int i = 1; i <= 26; ++i) std::printf(" %d", m.mar_width(i));
    std::printf("\nA widths:");
    for (int i = 1; i <= 62; ++i) std::printf(" %d", m.a_width(i));
    std::printf("\nELMNOD:");
    for (int v : m.elmnod()) std::printf(" %d", v);
    std::printf("\n");
    for (const auto& o : m.objects()) {
        std::printf("obj %d '%s' status %d deform %d nodes %d (from %d) elems %d (from %d) npe %d\n", o.number,
                    o.name.c_str(), o.status, o.deform_code, o.node_count, o.first_node, o.elem_count, o.first_elem,
                    o.nodes_per_elem);
        if (o.status != 1) continue;
        auto rz = m.real_keyword(o, "RZ");
        for (std::size_t r = 0; rz && r < 2; ++r) std::printf("   RZ %zu: %g %g %g\n", r + 1, rz.at(r, 0), rz.at(r, 1), rz.at(r, 2));
        auto con = m.connectivity(o);
        int lo = 1 << 30, hi = -lo;
        for (int v : con) { lo = v < lo ? v : lo; hi = v > hi ? v : hi; }
        std::printf("   ELMCON %zu values, local node range %d..%d\n", con.size(), lo, hi);
        for (const auto& k : keyword_table()) {
            if (k.kind == FieldKind::NodeReal || k.kind == FieldKind::ElemReal) {
                auto f = m.real_keyword(o, k.keyword);
                if (!f) continue;
                double mn = f.at(0, 0), mx = mn;
                for (std::size_t r = 0; r < f.rows; ++r) for (int c = 0; c < f.width; ++c) { double v = f.at(r, c); mn = v < mn ? v : mn; mx = v > mx ? v : mx; }
                std::printf("   %-7s rows %7zu width %3d  min %-12g max %g\n", k.keyword, f.rows, f.width, mn, mx);
            } else {
                auto f = m.int_keyword(o, k.keyword);
                if (!f) continue;
                std::printf("   %-7s rows %7zu width %3d  first %d\n", k.keyword, f.rows, f.width, f.at(0, 0));
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: dbinfo <file.DB> [--step N [--regions] [--dyn] | --all [--threads T]]\n");
        return 2;
    }
    try {
        auto db = Database::open(argv[1]);
        bool regions = false, dyn = false, all = false, show_header = false, show_objects = false;
        int step = 0;
        bool have_step = false;
        unsigned threads = 0;
        std::vector<std::string> dumps;
        for (int k = 2; k < argc; ++k) {
            if (!std::strcmp(argv[k], "--step") && k + 1 < argc) { step = std::atoi(argv[++k]); have_step = true; }
            else if (!std::strcmp(argv[k], "--regions")) regions = true;
            else if (!std::strcmp(argv[k], "--dyn")) dyn = true;
            else if (!std::strcmp(argv[k], "--header")) show_header = true;
            else if (!std::strcmp(argv[k], "--objects")) show_objects = true;
            else if (!std::strcmp(argv[k], "--dump") && k + 1 < argc) dumps.emplace_back(argv[++k]);
            else if (!std::strcmp(argv[k], "--all")) all = true;
            else if (!std::strcmp(argv[k], "--threads") && k + 1 < argc) threads = static_cast<unsigned>(std::atoi(argv[++k]));
        }
        if (!have_step && !all) {
            print_header(*db);
            print_directory(*db);
            return 0;
        }
        if (have_step) {
            auto e = db->find_step(step);
            if (!e) { std::fprintf(stderr, "step %d not found\n", step); return 1; }
            auto s = db->read_step(*e);
            std::printf("step %d: header %zu, MAR %zu, A %zu, compressed %d, dyn int %zu, dyn real %zu\n", e->step,
                        s.header.size(), s.mar.size(), s.a.size(), s.compressed ? 1 : 0, s.dynamic_ints.size(),
                        s.dynamic_reals.size());
            if (show_header) {
                for (std::size_t k = 0; k < s.header.size() && k < 40; ++k)
                    std::printf("H[%zu] = %.10g\n", k, s.header[k]);
            }
            if (show_objects) print_objects(s);
            for (const auto& kw : dumps) {
                StepModel m(s);
                const auto* info = find_keyword(kw);
                for (const auto& o : m.objects()) {
                    if (kw == "ELMCON") {
                        auto c = m.connectivity(o);
                        for (std::size_t e = 0; o.nodes_per_elem > 0 && e < c.size() / o.nodes_per_elem; ++e) {
                            std::printf("%s %d %zu", kw.c_str(), o.number, e + 1);
                            for (int k = 0; k < o.nodes_per_elem; ++k) std::printf(" %d", c[e * o.nodes_per_elem + k]);
                            std::printf("\n");
                        }
                    } else if (info && (info->kind == FieldKind::NodeInt || info->kind == FieldKind::ElemInt)) {
                        auto f = m.int_keyword(o, kw);
                        for (std::size_t r = 0; f && r < f.rows; ++r) {
                            std::printf("%s %d %zu", kw.c_str(), o.number, r + 1);
                            for (int k = 0; k < f.width; ++k) std::printf(" %d", f.at(r, k));
                            std::printf("\n");
                        }
                    } else {
                        auto f = m.real_keyword(o, kw);
                        for (std::size_t r = 0; f && r < f.rows; ++r) {
                            std::printf("%s %d %zu", kw.c_str(), o.number, r + 1);
                            for (int k = 0; k < f.width; ++k) std::printf(" %.10e", f.at(r, k));
                            std::printf("\n");
                        }
                    }
                }
            }
            if (regions) {
                for (const auto& g : s.mar_regions)
                    std::printf("MAR %3d %-6s start %9d len %9d stored %9d rec %10d\n", g.index, g.name.c_str(),
                                g.start, g.length, g.stored, g.record);
                for (const auto& g : s.a_regions)
                    std::printf("A   %3d %-6s start %9d len %9d stored %9d rec %10d\n", g.index, g.name.c_str(),
                                g.start, g.length, g.stored, g.record);
            }
            if (dyn) {
                std::printf("dyn descriptor:");
                for (int v : s.dynamic_descriptor) std::printf(" %d", v);
                std::printf("\n");
                for (const auto& d : s.dynamic_ints) {
                    std::printf("dyn int  slot %3d len %9zu desc", d.slot, d.values.size());
                    for (int v : d.descriptor) std::printf(" %d", v);
                    std::printf("\n");
                }
                for (const auto& d : s.dynamic_reals) {
                    std::printf("dyn real slot %3d len %9zu desc", d.slot, d.values.size());
                    for (int v : d.descriptor) std::printf(" %d", v);
                    std::printf("\n");
                }
            }
        }
        if (all) {
            std::vector<StepEntry> list;
            for (const auto& e : db->entries())
                if (e.rec_header > 0) list.push_back(e);
            std::mutex m;
            std::size_t words = 0, steps = 0;
            const auto t0 = std::chrono::steady_clock::now();
            db->read_steps(list, {}, threads, [&](StepData&& s) {
                std::lock_guard lock(m);
                ++steps;
                words += s.header.size() * 8 + s.mar.size() * 4 + s.a.size() * 8;
                for (auto& d : s.dynamic_ints) words += d.values.size() * 4;
                for (auto& d : s.dynamic_reals) words += d.values.size() * 8;
            });
            const double sec = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
            std::printf("decoded %zu steps, %.1f MB in memory, %.2f s, %.1f MB/s of file\n", steps, words / 1e6, sec,
                        db->file_size() / 1e6 / sec);
        }
        return 0;
    } catch (const std::exception& ex) {
        std::fprintf(stderr, "error: %s\n", ex.what());
        return 1;
    }
}
