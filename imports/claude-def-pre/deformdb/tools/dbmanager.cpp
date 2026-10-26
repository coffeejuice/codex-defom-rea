// dbmanager: purge and merge DEFORM .DB files like DEF_GUI23's DB Manager.
//   dbmanager purge <src.DB> <dst.DB> <steps...> [--overwrite] [--date MM/DD/YY]
//       steps: numbers or ranges, e.g. -1 5 10:20; a range a:b keeps every entry
//       whose |step| lies in [a, b], restart (negative) steps included
//   dbmanager merge <a.DB> <b.DB> <dst.DB> [--overwrite] [--date MM/DD/YY]
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "deformdb/database.hpp"
#include "deformdb/manager.hpp"

using namespace deformdb;

static int usage() {
    std::fprintf(stderr,
                 "usage: dbmanager purge <src.DB> <dst.DB> <step|a:b>... [--overwrite] [--date MM/DD/YY]\n"
                 "       dbmanager merge <a.DB> <b.DB> <dst.DB> [--overwrite] [--date MM/DD/YY]\n");
    return 2;
}

int main(int argc, char** argv) {
    if (argc < 5) return usage();
    const std::string cmd = argv[1];
    ManagerOptions opt;
    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i) {
        if (!std::strcmp(argv[i], "--overwrite")) opt.overwrite = true;
        else if (!std::strcmp(argv[i], "--date") && i + 1 < argc) opt.today = argv[++i];
        else args.push_back(argv[i]);
    }
    try {
        ManagerResult r;
        if (cmd == "purge" && args.size() >= 3) {
            auto db = Database::open(args[0]);
            std::vector<std::int32_t> keep;
            for (std::size_t k = 2; k < args.size(); ++k) {
                const auto colon = args[k].find(':', 1);
                if (colon == std::string::npos) {
                    keep.push_back(std::atoi(args[k].c_str()));
                    continue;
                }
                const int a = std::abs(std::atoi(args[k].substr(0, colon).c_str()));
                const int b = std::abs(std::atoi(args[k].substr(colon + 1).c_str()));
                for (const auto& e : db->entries())
                    if (std::abs(e.step) >= std::min(a, b) && std::abs(e.step) <= std::max(a, b))
                        keep.push_back(e.step);
            }
            db.reset();
            r = purge(args[0], args[1], keep, opt);
        } else if (cmd == "merge" && args.size() == 3) {
            r = merge(args[0], args[1], args[2], opt);
        } else {
            return usage();
        }
        std::printf("%zu entries written (%zu header-only)%s\n", r.entries, r.header_only,
                    r.stopped ? ", stopped" : "");
        return 0;
    } catch (const std::exception& ex) {
        std::fprintf(stderr, "error: %s\n", ex.what());
        return 1;
    }
}
