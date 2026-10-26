// dbraw: print raw words of one step, addressed the way DEF_PRE addresses them (IIAD1/IIAD2 tables
// stored at the start of MAR), for checking keyword <-> DB mappings (docs/re/db-model-mapping.md).
//
//   dbraw <file.DB> <step> addr                 region name, 1-based start, length, width (MAR then A)
//   dbraw <file.DB> <step> header               step header reals, 1-based
//   dbraw <file.DB> <step> mar|a <NAME> [rec]   region words, 0-based inside the region; with rec, only
//                                               record rec (0-based) of the region's stored width
//   dbraw <file.DB> <step> dir                  directory entry words
#include "deformdb/database.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

using namespace deformdb;

namespace {

struct Slot {
    std::string name;
    int start = 0, length = 0, width = 0;
};

// Slot k (0-based) of the MAR (n=70) or A (n=140) address table of a v >= 11000 step.
Slot slot(const StepData& s, bool a, int k) {
    const int n = a ? 140 : 70;
    const int base = s.mar[1];  // IIAD2 (A table) starts where word 2 of IIAD1 says
    auto word = [&](int i) { return s.mar[static_cast<std::size_t>(base - 1 + i)]; };
    Slot r;
    auto names = a ? a_region_names(s.entry.dim, s.entry.version) : mar_region_names(s.entry.dim, s.entry.version);
    r.name = k < static_cast<int>(names.size()) && names[k] ? names[k] : "";
    if (!a) {
        r.start = s.mar[static_cast<std::size_t>(k)];
        r.width = s.mar[static_cast<std::size_t>(n + k)];
        r.length = k + 1 < n ? s.mar[static_cast<std::size_t>(k + 1)] - r.start : 0;
    } else {
        r.start = word(k);
        r.width = word(n + k);
        r.length = k + 1 < n ? word(k + 1) - r.start : 0;
    }
    return r;
}

int find(const StepData& s, bool a, std::string_view name) {
    for (int k = 0; k < (a ? 140 : 70); ++k)
        if (slot(s, a, k).name == name) return k;
    return -1;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 4) {
        std::fprintf(stderr, "usage: dbraw <file.DB> <step> addr|header|dir|mar NAME [rec]|a NAME [rec]\n");
        return 2;
    }
    try {
        auto db = Database::open(argv[1]);
        auto e = db->find_step(std::atoi(argv[2]));
        if (!e) { std::fprintf(stderr, "step %s not found\n", argv[2]); return 1; }
        const std::string what = argv[3];
        if (what == "dir") {
            std::printf("slot %d mesh %d step %d hsize %d mar %d a %d sim %d mode %d dim %d ver %d dyn %d rh %d rm %d ra %d\n",
                        e->slot, e->mesh_no, e->step, e->header_size, e->mar_size, e->a_size, e->sim_no, e->sim_mode,
                        e->dim, e->version, e->rec_dynamic, e->rec_header, e->rec_mar, e->rec_a);
            return 0;
        }
        StepData s = db->read_step(*e);
        if (what == "header") {
            for (std::size_t i = 0; i < s.header.size(); ++i) std::printf("H%zu %.17g\n", i + 1, s.header[i]);
            return 0;
        }
        if (s.mar.empty()) { std::fprintf(stderr, "header-only step\n"); return 1; }
        if (what == "addr") {
            for (int a = 0; a < 2; ++a)
                for (int k = 0; k < (a ? 140 : 70); ++k) {
                    Slot r = slot(s, a != 0, k);
                    std::printf("%s %3d %-6s start %d len %d width %d\n", a ? "A  " : "MAR", k, r.name.c_str(), r.start,
                                r.length, r.width);
                }
            return 0;
        }
        const bool a = what == "a";
        if (!a && what != "mar") { std::fprintf(stderr, "unknown command %s\n", what.c_str()); return 2; }
        if (argc < 5) { std::fprintf(stderr, "region name missing\n"); return 2; }
        int k = find(s, a, argv[4]);
        if (k < 0) { std::fprintf(stderr, "region %s not found\n", argv[4]); return 1; }
        Slot r = slot(s, a, k);
        int from = 0, to = r.length;
        if (argc >= 6) {
            const int rec = std::atoi(argv[5]);
            const int w = r.width > 0 ? r.width : 1;
            from = rec * w;
            to = from + w < r.length ? from + w : r.length;
        }
        for (int i = from; i < to; ++i) {
            const std::size_t g = static_cast<std::size_t>(r.start - 1 + i);
            if (a) std::printf("%s[%d] %.17g\n", r.name.c_str(), i, s.a[g]);
            else std::printf("%s[%d] %d\n", r.name.c_str(), i, s.mar[g]);
        }
    } catch (const std::exception& ex) {
        std::fprintf(stderr, "error: %s\n", ex.what());
        return 1;
    }
    return 0;
}
