#include "deformdb/manager.hpp"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "deformdb/database.hpp"
#include "deformdb/writer.hpp"

namespace deformdb {

namespace {

namespace fs = std::filesystem;

void check_destination(const fs::path& dst, std::initializer_list<const fs::path*> sources, bool overwrite) {
    std::error_code ec;
    for (const fs::path* s : sources)
        if (fs::equivalent(*s, dst, ec)) throw std::runtime_error("destination is one of the source databases");
    if (fs::exists(dst) && !overwrite) throw std::runtime_error("destination exists: " + dst.string());
}

struct Job {
    const Database* db;
    StepEntry entry;
    bool header_only;
};

ManagerResult run(const std::vector<Job>& jobs, const fs::path& dst, const FileHeader& src_header,
                  const ManagerOptions& opt) {
    WriterHeader wh = writer_header_from(src_header);
    wh.today = opt.today;
    auto w = Writer::create(dst, wh);
    ManagerResult r;
    for (const auto& j : jobs) {
        ReadOptions ro;
        ro.arrays = !j.header_only;
        ro.dynamic = !j.header_only;
        const StepData s = j.db->read_step(j.entry, ro);
        w->append_step(s, j.header_only);
        ++r.entries;
        if (j.header_only) ++r.header_only;
        if (opt.progress && !opt.progress(r.entries, jobs.size())) {
            r.stopped = true;
            break;
        }
    }
    w->flush();
    return r;
}

bool has_arrays(const StepEntry& e) { return e.mar_size > 0 && e.a_size > 0; }

}  // namespace

ManagerResult purge(const fs::path& src, const fs::path& dst, std::span<const std::int32_t> keep_steps,
                    const ManagerOptions& opt) {
    check_destination(dst, {&src}, opt.overwrite);
    auto db = Database::open(src);
    const std::unordered_set<std::int32_t> keep(keep_steps.begin(), keep_steps.end());
    const auto& entries = db->entries();

    // First and last kept step in directory order (the GUI takes the first and the
    // last ticked row of its step list).
    auto first = std::find_if(entries.begin(), entries.end(), [&](const StepEntry& e) { return keep.count(e.step); });
    if (first == entries.end()) throw std::runtime_error("none of the steps to keep is in " + src.string());
    auto last = std::find_if(entries.rbegin(), entries.rend(), [&](const StepEntry& e) { return keep.count(e.step); });
    const std::int32_t lo = std::abs(first->step), hi = std::abs(last->step);

    std::vector<Job> jobs;
    for (const auto& e : entries) {
        const std::int32_t a = std::abs(e.step);
        if (a < lo || a > hi) continue;
        jobs.push_back({db.get(), e, !has_arrays(e) || !keep.count(e.step)});
    }
    return run(jobs, dst, db->header(), opt);
}

ManagerResult merge(const fs::path& db1, const fs::path& db2, const fs::path& dst, const ManagerOptions& opt) {
    check_destination(dst, {&db1, &db2}, opt.overwrite);
    auto a = Database::open(db1);
    auto b = Database::open(db2);
    if (a->entries().empty() || b->entries().empty()) throw std::runtime_error("cannot merge an empty database");

    const Database* first = nullptr;
    const Database* second = nullptr;
    if (b->entries().front().step == -1 - a->entries().back().step) {
        first = a.get();
        second = b.get();
    } else if (a->entries().front().step == -1 - b->entries().back().step) {
        first = b.get();
        second = a.get();
    } else {
        throw std::runtime_error("the databases do not continue each other: neither starts with -(last step of "
                                 "the other + 1)");
    }

    std::vector<Job> jobs;
    for (const Database* db : {first, second})
        for (const auto& e : db->entries()) jobs.push_back({db, e, !has_arrays(e)});
    return run(jobs, dst, first->header(), opt);
}

}  // namespace deformdb
