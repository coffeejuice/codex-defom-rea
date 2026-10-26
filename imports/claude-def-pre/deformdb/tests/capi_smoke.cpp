// Smoke test of the C ABI against a real database: capi_smoke <file.DB> [step]
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "deformdb/deformdb.h"

#define CHECK(call)                                                                \
    do {                                                                           \
        if ((call) != 0) {                                                         \
            std::printf("FAIL %s: %s\n", #call, deformdb_last_error());           \
            return 1;                                                              \
        }                                                                          \
    } while (0)

int main(int argc, char** argv) {
    if (argc < 2) return 2;
    deformdb_db* db = nullptr;
    CHECK(deformdb_open(argv[1], &db));
    int32_t n = 0, idx = 0;
    CHECK(deformdb_entry_count(db, &n));
    const int32_t step = argc > 2 ? std::atoi(argv[2]) : 1;
    CHECK(deformdb_find_step(db, step, &idx));

    // Read the step twice in parallel to exercise deformdb_steps_read.
    int32_t list[2] = {idx, idx};
    deformdb_step* steps[2] = {};
    CHECK(deformdb_steps_read(db, list, 2, 0, 2, steps));

    int32_t nstart = 0, numnp = 0, numel = 0, objs = 0;
    double t = 0;
    CHECK(deformdb_step_globals(steps[0], &nstart, nullptr, &numnp, &numel, &t));
    CHECK(deformdb_step_object_count(steps[0], &objs));
    std::printf("entries %d, step %d: time %g, %d nodes, %d elements, %d objects\n", n, nstart, t, numnp, numel, objs);
    for (int32_t k = 0; k < objs; ++k) {
        deformdb_object o{};
        CHECK(deformdb_step_object(steps[0], k, &o));
        if (o.status != 1) continue;
        deformdb_field rz{};
        CHECK(deformdb_step_keyword(steps[0], k, "RZ", &rz));
        const double* p = static_cast<const double*>(rz.data);
        int64_t cn = 0;
        CHECK(deformdb_step_connectivity(steps[0], k, nullptr, 0, &cn));
        std::vector<int32_t> conn(static_cast<size_t>(cn));
        CHECK(deformdb_step_connectivity(steps[0], k, conn.data(), cn, &cn));
        std::printf("  obj %d %-24s nodes %d elems %d first node (%g, %g, %g) conn %lld\n", o.number, o.name,
                    o.node_count, o.elem_count, p[0], p[rz.comp_stride], p[2 * rz.comp_stride], (long long)cn);
    }
    deformdb_step_free(steps[0]);
    deformdb_step_free(steps[1]);
    deformdb_close(db);
    std::printf("C API smoke test passed\n");
    return 0;
}
