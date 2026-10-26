#include "deformdb/deformdb.h"

#include <cstring>
#include <string>

#include "deformdb/database.hpp"
#include "deformdb/model.hpp"

using namespace deformdb;

struct deformdb_db {
    std::unique_ptr<Database> db;
};

struct deformdb_step {
    StepData data;
    std::unique_ptr<StepModel> model;  // built on first typed access; points into `data`

    const StepModel* typed() {
        if (!model) model = std::make_unique<StepModel>(data);
        return model->supported() ? model.get() : nullptr;
    }
};

namespace {

thread_local std::string g_error;

int fail(const char* msg) {
    g_error = msg;
    return 1;
}

template <class F>
int guard(F&& f) {
    try {
        return f();
    } catch (const std::exception& e) {
        return fail(e.what());
    } catch (...) {
        return fail("unknown error");
    }
}

ReadOptions options(uint32_t flags) {
    if (flags == 0) return {};
    return {(flags & 1u) != 0, (flags & 2u) != 0, (flags & 4u) != 0};
}

deformdb_entry to_c(const StepEntry& e) {
    return {e.slot, e.mesh_no, e.step, e.header_size, e.mar_size, e.a_size, e.sim_no, e.sim_mode, e.dim,
            e.version, e.rec_dynamic, e.rec_header, e.rec_mar, e.rec_a};
}

}  // namespace

extern "C" {

const char* deformdb_last_error(void) { return g_error.c_str(); }

int deformdb_open(const char* utf8_path, deformdb_db** out) {
    if (!utf8_path || !out) return fail("null argument");
    return guard([&] {
        auto h = std::make_unique<deformdb_db>();
        h->db = Database::open(std::filesystem::path(reinterpret_cast<const char8_t*>(utf8_path)));
        *out = h.release();
        return 0;
    });
}

void deformdb_close(deformdb_db* db) { delete db; }

int deformdb_refresh(deformdb_db* db) {
    if (!db) return fail("null argument");
    return guard([&] { db->db->refresh(); return 0; });
}

int deformdb_title(const deformdb_db* db, const char** title) {
    if (!db || !title) return fail("null argument");
    *title = db->db->header().title.c_str();
    return 0;
}

int deformdb_version(const deformdb_db* db, int32_t* version) {
    if (!db || !version) return fail("null argument");
    *version = db->db->header().version;
    return 0;
}

int deformdb_entry_count(const deformdb_db* db, int32_t* count) {
    if (!db || !count) return fail("null argument");
    *count = static_cast<int32_t>(db->db->entries().size());
    return 0;
}

int deformdb_get_entry(const deformdb_db* db, int32_t index0, deformdb_entry* out) {
    if (!db || !out) return fail("null argument");
    const auto& v = db->db->entries();
    if (index0 < 0 || static_cast<std::size_t>(index0) >= v.size()) return fail("entry index out of range");
    *out = to_c(v[static_cast<std::size_t>(index0)]);
    return 0;
}

int deformdb_find_step(const deformdb_db* db, int32_t step, int32_t* index0) {
    if (!db || !index0) return fail("null argument");
    auto e = db->db->find_step(step);
    if (!e) return fail("step not found");
    *index0 = e->slot - 1;
    return 0;
}

int deformdb_step_read(const deformdb_db* db, int32_t index0, uint32_t flags, deformdb_step** out) {
    if (!db || !out) return fail("null argument");
    return guard([&] {
        const auto& v = db->db->entries();
        if (index0 < 0 || static_cast<std::size_t>(index0) >= v.size()) return fail("entry index out of range");
        auto s = std::make_unique<deformdb_step>();
        s->data = db->db->read_step(v[static_cast<std::size_t>(index0)], options(flags));
        *out = s.release();
        return 0;
    });
}

void deformdb_step_free(deformdb_step* step) { delete step; }

int deformdb_steps_read(const deformdb_db* db, const int32_t* indices0, int32_t count, uint32_t flags,
                        uint32_t threads, deformdb_step** out) {
    if (!db || (!indices0 && count > 0) || (!out && count > 0)) return fail("null argument");
    return guard([&] {
        const auto& v = db->db->entries();
        std::vector<StepEntry> list;
        list.reserve(static_cast<std::size_t>(count));
        for (int32_t k = 0; k < count; ++k) {
            if (indices0[k] < 0 || static_cast<std::size_t>(indices0[k]) >= v.size())
                return fail("entry index out of range");
            list.push_back(v[static_cast<std::size_t>(indices0[k])]);
            out[k] = nullptr;
        }
        // Map each finished step back to its position by slot.
        db->db->read_steps(list, options(flags), threads, [&](StepData&& s) {
            for (int32_t k = 0; k < count; ++k)
                if (list[static_cast<std::size_t>(k)].slot == s.entry.slot && out[k] == nullptr) {
                    out[k] = new deformdb_step{std::move(s), nullptr};
                    return;
                }
        });
        return 0;
    });
}

int deformdb_step_header(const deformdb_step* s, const double** data, int64_t* count) {
    if (!s || !data || !count) return fail("null argument");
    *data = s->data.header.data();
    *count = static_cast<int64_t>(s->data.header.size());
    return 0;
}

int deformdb_step_mar(const deformdb_step* s, const int32_t** data, int64_t* count) {
    if (!s || !data || !count) return fail("null argument");
    *data = s->data.mar.data();
    *count = static_cast<int64_t>(s->data.mar.size());
    return 0;
}

int deformdb_step_a(const deformdb_step* s, const double** data, int64_t* count) {
    if (!s || !data || !count) return fail("null argument");
    *data = s->data.a.data();
    *count = static_cast<int64_t>(s->data.a.size());
    return 0;
}

int deformdb_step_region_count(const deformdb_step* s, int is_real, int32_t* count) {
    if (!s || !count) return fail("null argument");
    *count = static_cast<int32_t>(is_real ? s->data.a_regions.size() : s->data.mar_regions.size());
    return 0;
}

int deformdb_step_region(const deformdb_step* s, int is_real, int32_t index0, deformdb_region* out) {
    if (!s || !out) return fail("null argument");
    const auto& v = is_real ? s->data.a_regions : s->data.mar_regions;
    if (index0 < 0 || static_cast<std::size_t>(index0) >= v.size()) return fail("region index out of range");
    const auto& g = v[static_cast<std::size_t>(index0)];
    *out = {g.index, g.start, g.length, g.stored, g.record, {}};
    std::strncpy(out->name, g.name.c_str(), sizeof(out->name) - 1);
    return 0;
}

int deformdb_step_dynamic_count(const deformdb_step* s, int is_real, int32_t* count) {
    if (!s || !count) return fail("null argument");
    *count = static_cast<int32_t>(is_real ? s->data.dynamic_reals.size() : s->data.dynamic_ints.size());
    return 0;
}

int deformdb_step_dynamic(const deformdb_step* s, int is_real, int32_t index0, int32_t* slot, const void** data,
                          int64_t* count) {
    if (!s || !slot || !data || !count) return fail("null argument");
    if (is_real) {
        const auto& v = s->data.dynamic_reals;
        if (index0 < 0 || static_cast<std::size_t>(index0) >= v.size()) return fail("index out of range");
        const auto& d = v[static_cast<std::size_t>(index0)];
        *slot = d.slot;
        *data = d.values.data();
        *count = static_cast<int64_t>(d.values.size());
    } else {
        const auto& v = s->data.dynamic_ints;
        if (index0 < 0 || static_cast<std::size_t>(index0) >= v.size()) return fail("index out of range");
        const auto& d = v[static_cast<std::size_t>(index0)];
        *slot = d.slot;
        *data = d.values.data();
        *count = static_cast<int64_t>(d.values.size());
    }
    return 0;
}

int deformdb_step_globals(deformdb_step* s, int32_t* nstart, int32_t* meshno, int32_t* numnp, int32_t* numel,
                          double* time) {
    if (!s) return fail("null argument");
    auto m = s->typed();
    if (!m) return fail("typed view not available for this step");
    if (nstart) *nstart = m->nstart();
    if (meshno) *meshno = m->meshno();
    if (numnp) *numnp = m->numnp();
    if (numel) *numel = m->numel();
    if (time) *time = m->time();
    return 0;
}

int deformdb_step_object_count(deformdb_step* s, int32_t* count) {
    if (!s || !count) return fail("null argument");
    auto m = s->typed();
    *count = m ? m->object_count() : 0;
    return 0;
}

int deformdb_step_object(deformdb_step* s, int32_t index0, deformdb_object* out) {
    if (!s || !out) return fail("null argument");
    auto m = s->typed();
    if (!m || index0 < 0 || index0 >= m->object_count()) return fail("object index out of range");
    const auto& o = m->objects()[static_cast<std::size_t>(index0)];
    *out = {o.number, o.status, o.deform_code, o.porous ? 1 : 0, o.first_node, o.node_count, o.first_elem,
            o.elem_count, o.nodes_per_elem, {}};
    std::strncpy(out->name, o.name.c_str(), sizeof(out->name) - 1);
    return 0;
}

int deformdb_step_keyword(deformdb_step* s, int32_t index0, const char* keyword, deformdb_field* out) {
    if (!s || !keyword || !out) return fail("null argument");
    auto m = s->typed();
    if (!m || index0 < 0 || index0 >= m->object_count()) return fail("object index out of range");
    const auto* k = find_keyword(keyword);
    if (!k) return fail("unknown keyword");
    const auto& o = m->objects()[static_cast<std::size_t>(index0)];
    *out = {};
    if (k->kind == FieldKind::NodeInt || k->kind == FieldKind::ElemInt) {
        auto f = m->int_keyword(o, keyword);
        if (!f) return fail("keyword not present for this object");
        *out = {f.data, static_cast<int64_t>(f.rows), f.width, 1, static_cast<int64_t>(f.row_stride),
                static_cast<int64_t>(f.comp_stride)};
    } else {
        auto f = m->real_keyword(o, keyword);
        if (!f) return fail("keyword not present for this object");
        *out = {f.data, static_cast<int64_t>(f.rows), f.width, 0, static_cast<int64_t>(f.row_stride),
                static_cast<int64_t>(f.comp_stride)};
    }
    return 0;
}

int deformdb_step_connectivity(deformdb_step* s, int32_t index0, int32_t* buffer, int64_t capacity, int64_t* count) {
    if (!s || !count) return fail("null argument");
    auto m = s->typed();
    if (!m || index0 < 0 || index0 >= m->object_count()) return fail("object index out of range");
    auto c = m->connectivity(m->objects()[static_cast<std::size_t>(index0)]);
    *count = static_cast<int64_t>(c.size());
    if (buffer) {
        if (capacity < *count) return fail("buffer too small");
        std::copy(c.begin(), c.end(), buffer);
    }
    return 0;
}

}  // extern "C"
