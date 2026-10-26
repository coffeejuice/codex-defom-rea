// Step queries on the DB info object Cls_1401dbfe0: directory step lookups and the operation /
// simulation first/last queries over the header cache (packet mdt-dbquery). Declarations:
// defdbio/db_query.hpp. The cache itself (1401dd250, 1401df8a0) is p67-5's, in
// defdbio/src/step/db_header_cache.cpp.
#include <defdbio/db_query.hpp>

#include <cstdint>

#include <defdbio/step/db_header_cache.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::query {

using def::model::DbDirRecord;
using def::model::DbInfoEntryE0;

namespace {

const DbDirRecord& dir(const Cls_1401dbfe0& db, std::int32_t i) { return def::model::db_info_records(db)[i]; }

DbInfoEntryE0& cache(const Cls_1401dbfe0& db, std::int32_t i)
{
    return static_cast<DbInfoEntryE0*>(db.uae8)[i];
}

// The shared prologue of 1401de240 / 1401de110 / 1401ddfe0: build the header cache when needed,
// then 1401df660 with zeroed locals.
struct OpSpan {
    bool found;
    std::int32_t first;
    std::int32_t last;
};

OpSpan op_span(Cls_1401dbfe0& db, std::int32_t op)
{
    std::int32_t dim = 0, nobj = 0, pdie = 0, first = 0, last = 0;
    double t0 = 0.0, t1 = 0.0, s0 = 0.0, s1 = 0.0;
    if (!header_cache_built(db)) {
        step::load_header_cache(db, false); // 1401dd250(db, 0)
    }
    const bool found = op_first_last(db, op, &dim, &nobj, &pdie, &first, &last, &t0, &t1, &s0, &s1);
    return {found, first, last};
}

} // namespace

// @src 1401e3ee0  header cache built flag
bool header_cache_built(const Cls_1401dbfe0& db) { return db.u1900 != 0; }

// @src 1401ddf00  step number of directory entry idx
std::int32_t entry_step(const Cls_1401dbfe0& db, std::int32_t idx)
{
    if (db.uad8 == nullptr) return 0;
    if (idx < 0 || db.uac8 <= idx) return 0;
    return dir(db, idx).step;
}

// @src 1401ddf60  step number -> directory index
std::int32_t step_index(const Cls_1401dbfe0& db, std::int32_t step)
{
    if (db.uad8 != nullptr) {
        for (std::int32_t i = 0; i < db.uac8; i = i + 1) {
            if (entry_step(db, i) == step) {
                return i;
            }
        }
    }
    return -1;
}

// @src 1401df660  first / last stored entries of an operation (cache +0x38)
bool op_first_last(const Cls_1401dbfe0& db, std::int32_t op, std::int32_t* dim, std::int32_t* nobj,
                   std::int32_t* pdie, std::int32_t* step_first, std::int32_t* step_last,
                   double* t_first, double* t_last, double* s_first, double* s_last)
{
    if (db.uad8 == nullptr || db.uae8 == nullptr) {
        return false; // the original clears only the low byte of `this` (the return value)
    }
    bool first = true;
    for (std::int32_t i = 0; i < db.uac8; i = i + 1) {
        const DbDirRecord& r = dir(db, i);
        DbInfoEntryE0& e = cache(db, i);
        if (r.stored != 0 && step::entry_i32(e, 0x38) == op) {
            if (first) {
                *dim = r.dim;
                *nobj = step::entry_i32(e, 0x00);
                *pdie = step::entry_i32(e, 0x04);
                *step_first = r.step;
                *step_last = r.step;
                *t_first = step::entry_f64(e, 0x08);
                *s_first = step::entry_f64(e, 0x28);
                first = false;
            } else {
                *step_last = r.step;
                *t_last = step::entry_f64(e, 0x10);
                *s_last = step::entry_f64(e, 0x28);
            }
        }
    }
    return !first;
}

// @src 1401de240  number of steps of an operation
std::int32_t op_step_count(Cls_1401dbfe0& db, std::int32_t op)
{
    const OpSpan s = op_span(db, op);
    std::int32_t n = 0; // local_68[2]
    if (s.found) {
        if (s.first == s.last) {
            n = 1;
        } else {
            const std::int32_t a = s.first < 1 ? -1 - s.first : s.first; // local_18
            const std::int32_t b = s.last < 1 ? -s.last : s.last;       // local_14
            n = (b - a) + 1;
        }
    }
    return n;
}

// @src 1401de110  last step of an operation
std::int32_t op_last_step(Cls_1401dbfe0& db, std::int32_t op)
{
    const OpSpan s = op_span(db, op);
    return s.found ? s.last : 0;
}

// @src 1401ddfe0  first step of an operation
std::int32_t op_first_step(Cls_1401dbfe0& db, std::int32_t op)
{
    const OpSpan s = op_span(db, op);
    return s.found ? s.first : 0;
}

// @src 1401de420  first saved step of a simulation (no cache build here)
std::int32_t sim_first_step(const Cls_1401dbfe0& db, std::int32_t sim)
{
    std::int32_t dim = 0, nobj = 0, pdie = 0, first = 0, last = 0;
    double t0 = 0.0, t1 = 0.0, s0 = 0.0, s1 = 0.0;
    if (!step::sim_first_last(db, sim, &dim, &nobj, &pdie, &first, &last, &t0, &t1, &s0, &s1)) {
        first = 0;
    }
    return first;
}

// @src 1401de530  last saved step of a simulation (no cache build here)
std::int32_t sim_last_step(const Cls_1401dbfe0& db, std::int32_t sim)
{
    std::int32_t dim = 0, nobj = 0, pdie = 0, first = 0, last = 0;
    double t0 = 0.0, t1 = 0.0, s0 = 0.0, s1 = 0.0;
    if (!step::sim_first_last(db, sim, &dim, &nobj, &pdie, &first, &last, &t0, &t1, &s0, &s1)) {
        last = 0;
    }
    return last;
}

} // namespace def::dbio::query
