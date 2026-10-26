// GENDB frame (packet p4-01): DB header, buffers, address tables, dynamic-array collection and the
// small model accessors they read. Offsets the layouts leave unnamed are read with field<T>().
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/step_header.hpp>
#include <defmodel/llobj/object_list.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "db_header_io.hpp"

namespace def::dbio::gendb {

using model::Cls_1401dbfe0;
using model::CowVector;
using model::DYNARY_DATA;
using model::field;
using model::LLOBJ_DATA;
using model::SfLinkedList;

namespace {

inline bool is_2d(const DB_STEP_DATA& m) { return m.dimension == 2; } // 1400015e0 (p1-01, inlined)
inline bool is_3d(const DB_STEP_DATA& m) { return m.dimension == 3; } // 1400015f0
std::int32_t& w(DB_STEP_DATA& m, std::size_t off) { return field<std::int32_t>(&m, off); }
std::int32_t w(const DB_STEP_DATA& m, std::size_t off) { return field<std::int32_t>(&m, off); }

// @src 14001cd50  error report: empty in this build
void report(const std::int32_t*, const std::int32_t*, const std::int32_t*) {}

// @src 140159010  the model's own DB info object (+0x3390)
Cls_1401dbfe0* own_info(DB_STEP_DATA& m) { return field<Cls_1401dbfe0*>(&m, 0x3390); }
// @src 140199cc0  sim control: SMODE (+0x13c of the model)
std::int32_t smode(const DB_STEP_DATA& m) { return w(m, 0x13c); }
// 140160f10: the kw-g4b port
bool not_empty(const LLOBJ_DATA* o) { return model::llobj_not_kind7(o); }
} // namespace

// @src 140160bf0  object has a mesh (+0x87c != 0)
bool has_mesh(const LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x87c) != 0; }

namespace {

// @src 140220910  push_back of a COW int vector
// @src 140220810  (the push_back itself: detach, grow, store)
void push_back(CowVector<std::int32_t>* v, const std::int32_t* x)
{
    v->insert(v->mutable_end(), 1, *x);
}

// @src 14025b980  the record holds data (kind != 0 and a positive size)
bool record_used(const DYNARY_DATA* d)
{
    if (model::dynary_data_kind(d) == 0) {
        return false;
    }
    return 0 < model::dynary_data_size(d);
}

// @src 14025a870  unlink a node and delete it through its virtual dtor; true if it was found
bool remove_node(SfLinkedList* l, model::SfLinkedData* node)
{
    model::SfLinkedData* before = l->head;
    model::SfLinkedData* p = l->head;
    while (p != nullptr && p != node) {
        before = p;
        p = p->next;
    }
    if (p == nullptr) {
        return false;
    }
    before->next = p->next;
    if (l->head == p) {
        l->head = p->next;
    }
    if (l->tail == p) {
        l->tail = before;
    }
    delete p; // (**vtbl)(p, 1)
    l->count = l->count - 1;
    return true;
}

// @src 14025bc60  remove every record without data
void drop_unused(SfLinkedList* l)
{
    bool again = true;
    while (again) {
        again = false;
        for (std::int32_t i = 0; i < model::sf_list_count(l); i = i + 1) {
            DYNARY_DATA* d = model::dynary_list_nth(l, i + 1);
            if (d != nullptr && !record_used(d)) {
                remove_node(l, d);
                again = true;
                break;
            }
        }
    }
}

// @src 14025b700  the record's size into the int or the real list
void add_size(const DYNARY_DATA* d, CowVector<std::int32_t>* ints, CowVector<std::int32_t>* reals)
{
    if (!record_used(d)) {
        return;
    }
    const std::int32_t n = model::dynary_data_size(d);
    if (model::dynary_data_is_int(d) == 0) {
        push_back(reals, &n);
    }
    else {
        push_back(ints, &n);
    }
}

// @src 14025bbc0  sizes of one object's records (empty records dropped first)
void list_sizes(SfLinkedList* l, CowVector<std::int32_t>* ints, CowVector<std::int32_t>* reals)
{
    drop_unused(l);
    for (std::int32_t i = 0; i < model::sf_list_count(l); i = i + 1) {
        DYNARY_DATA* d = model::dynary_list_nth(l, i + 1);
        if (d != nullptr) {
            add_size(d, ints, reals);
        }
    }
}

// @src 14025b780  put one record into the next int or real slot of the tables
void put_record(DYNARY_DATA* d, std::int32_t* int_slot, std::int32_t* real_slot, std::int32_t obj_no)
{
    if (!record_used(d)) {
        return;
    }
    const std::int32_t put = 2;
    std::int32_t n = model::dynary_data_size(d);
    std::int32_t no = obj_no;
    std::int32_t ierr = 0;
    std::int32_t unused2 = 0;
    std::int32_t unused1[3] = {0, 0, 0};
    if (model::dynary_data_is_int(d) == 0) {
        *real_slot = *real_slot + 1;
        dynamic_real(&put, real_slot, model::dynary_data_doubles(d), &n, &no, &d->u0c, &d->u18, &d->u28,
                     unused1, &unused2, &ierr); // 1403bcda9
    }
    else {
        *int_slot = *int_slot + 1;
        dynamic_int(&put, int_slot, model::dynary_data_ints(d), &n, &no, &d->u0c, &d->u18, &d->u28,
                    unused1, &unused2, &ierr); // 1403bcaa6
    }
}

// @src 14025be70  put every record of one object
void list_put(SfLinkedList* l, std::int32_t* int_slot, std::int32_t* real_slot, std::int32_t obj_no)
{
    for (std::int32_t i = 0; i < model::sf_list_count(l); i = i + 1) {
        DYNARY_DATA* d = model::dynary_list_nth(l, i + 1);
        if (d != nullptr) {
            put_record(d, int_slot, real_slot, obj_no);
        }
    }
}

// @src 1401c53d0  sizes of every object's records; true if there is any
// @src 1401c57f0  (vector size, inlined)
bool scan_dynamic(DB_STEP_DATA& m, CowVector<std::int32_t>* ints, CowVector<std::int32_t>* reals)
{
    for (std::int32_t i = 0; i < w(m, 0x1378); i = i + 1) {
        auto* o = static_cast<LLOBJ_DATA*>(model::step_object_at(m, i + 1)); // 1401d6ed0
        if (o != nullptr && not_empty(o)) {
            list_sizes(&o->uba8, ints, reals);
        }
    }
    return ints->size() != 0 || reals->size() != 0; // 1401c57f0
}

} // namespace

// @src 1401cc180  stored version word (+0x1b64)
std::int32_t stored_version(const DB_STEP_DATA& m) { return field<std::int32_t>(&m, 0x1b64); }

// @src 1401c5630  simulation mode of the directory entry
std::int32_t sim_mode(const DB_STEP_DATA& m)
{
    if (w(m, 0x85c) == 0 && w(m, 0x860) == 0 && w(m, 0x864) == 0) {
        return smode(m);
    }
    return 4;
}

// @src 1401cafe0  width code of ELMNOD option slot k
// @src 140185530  (element variable set width, Cls_140182880::width at object +0x9e0)
std::int32_t elmnod_width(DB_STEP_DATA& m, std::int32_t k)
{
    if (w(m, 0x888 + static_cast<std::size_t>(k) * 4) != 3) {
        return 1;
    }
    if (is_2d(m)) {
        return 4;
    }
    for (std::int32_t i = 0; i < w(m, 0x1378); i = i + 1) {
        auto* o = static_cast<LLOBJ_DATA*>(model::step_object_at(m, i + 1));
        if (o != nullptr && not_empty(o) && has_mesh(o) &&
            reinterpret_cast<const model::Cls_140182880*>(reinterpret_cast<const std::byte*>(o) + 0x9e0)->width() ==
                8) {
            return 8;
        }
    }
    return 1;
}

// @src 1401bffb0  DB header of the step
void write_db_header(DB_STEP_DATA& m, std::int32_t* step, std::int32_t* pos, std::int32_t* count,
                     std::int32_t* prev, Cls_1401dbfe0* info, const char* keep_version)
{
    (void)prev;
    std::int32_t mesh = w(m, 0x840);
    *step = w(m, 0x8b0);
    std::int32_t interval = w(m, 0x8b4);
    std::int32_t sim_no = w(m, 0x834);
    std::int32_t mode = sim_mode(m);
    std::int32_t family = m.dimension; // 140158e90
    std::int32_t version = stored_version(m);
    if (info == nullptr) {
        info = own_info(m);
    }
    if (*keep_version != 0) {
        version = write_version(&m);
    }
    set_step_words(&mesh, step, &interval, &sim_no, &mode, &family, &version); // 1403b3f73
    std::int32_t month = 0, day = 0, year = 0;
    local_date(&month, &day, &year);
    year = year % 100;
    char date[16];
    def::rt::crt::sprintf(date, "%02d/%02d/%02d", month, day, year);
    char* aa1 = &field<char>(info, 0xaa1);
    char* aa9 = &field<char>(info, 0xaa9);
    char* ab1 = &field<char>(info, 0xab1);
    std::strncpy(aa1, &field<char>(&m, 0x10), 8);
    if (w(m, 0x8b0) == -1 || std::strlen(aa9) == 0) {
        std::strncpy(aa9, date, 8);
        std::strncpy(ab1, date, 8);
    }
    if (!(w(m, 0x8b0) == -1 && std::strlen(ab1) != 0)) {
        std::strncpy(ab1, date, 8);
    }
    // SMI1..SMI4: four 8-character fields side by side; the version text "%5.2lf   " has 8
    // characters, its NUL lands on SMI2 before SMI2 is filled
    char smi[33];
    def::rt::crt::sprintf(smi, "%5.2lf   ", static_cast<double>(write_version(&m)) / 1000.0); // DAT_140522f90
    std::strncpy(smi + 8, aa1, 8);
    std::strncpy(smi + 0x10, aa9, 8);
    std::strncpy(smi + 0x18, ab1, 8);
    std::int32_t ismtyp = w(m, 0x848);
    if (is_3d(m)) {
        ismtyp = 1;
    }
    std::int32_t w33 = 1;
    std::int32_t nxtrec = field<std::int32_t>(info, 0xad0);
    std::int32_t w34 = 0;
    char* title_src = &field<char>(info, 0xa50);
    if (std::strlen(&field<char>(&m, 0x750)) != 0) {
        std::strncpy(title_src, &field<char>(&m, 0x750), 0x50);
    }
    char title[0x50];
    def::rt::to_fortran(title, title_src, 0x50); // 140323bf0
    set_record2(title, smi, pos, &ismtyp, count, &nxtrec, &w33, &w34); // 1403b400b
}

// @src 1401c4560  buffer lengths and the zeroed buffers
void alloc_buffers(DB_STEP_DATA& m)
{
    std::int32_t frame = 0;
    std::int32_t per_object = 0;
    std::int32_t dim = m.dimension;
    header_block_sizes(&dim, &frame, &per_object);
    std::int32_t hlen = 0;
    if (is_3d(m)) {
        w(m, 0x30) = w(m, 0x1c78) - 1;
        w(m, 0x34) = w(m, 0x1fb8) - 1;
        w(m, 0x38) = w(m, 0x1ca4);
        w(m, 0x3c) = w(m, 0x1fec) + frame + per_object * w(m, 0x7c);
        hlen = frame + per_object * 100;
    }
    if (is_2d(m)) {
        w(m, 0x30) = w(m, 0x1c88) - 1;
        w(m, 0x34) = w(m, 0x1fc4) - 1;
        w(m, 0x38) = w(m, 0x1ca4);
        w(m, 0x3c) = w(m, 0x1fec) + frame + per_object * w(m, 0x7c);
        hlen = frame + per_object * 100;
    }
    const std::int32_t zero = 0;
    std::int32_t code = 2;
    report(&code, &zero, &w(m, 0x3c));
    code = 3;
    report(&code, &zero, &w(m, 0x38));
    w(m, 0x40) = w(m, 0x30);
    const std::int32_t na = w(m, 0x34);
    w(m, 0x44) = na << 1;
    w(m, 0x48) = hlen << 1;
    // thunk_FUN_1404797d8: operator new (malloc); the buffers are released with free() by 1401bb320
    field<void*>(&m, 0x50) = std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(w(m, 0x40))) * 4);
    field<void*>(&m, 0x58) = std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(na)) * 8);
    field<void*>(&m, 0x60) = std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(hlen)) * 8);
    std::memset(field<void*>(&m, 0x50), 0, static_cast<std::size_t>(static_cast<std::int64_t>(w(m, 0x40))) << 2);
    std::memset(field<void*>(&m, 0x58), 0, static_cast<std::size_t>(static_cast<std::int64_t>(na)) << 3);
    std::memset(field<void*>(&m, 0x60), 0, static_cast<std::size_t>(static_cast<std::int64_t>(hlen)) << 3);
}

// @src 1401c4840  the address tables into IIAD1 and IIAD2
void store_address_tables(DB_STEP_DATA& m)
{
    std::int32_t dim = m.dimension;
    model::select_index_tables(&dim); // 1401c7d40
    std::int32_t* mar = field<std::int32_t*>(&m, 0x50);
    auto mar_start = [&](std::int32_t k) -> std::int32_t& { return w(m, 0x1b90 + model::g_mar_index[k] * 4); };
    // IIAD1 (index 0) receives the MAR start table, IIAD2 (index 1) the A start table
    std::memcpy(mar + (mar_start(0) - 1), &mar_start(0),
                static_cast<std::size_t>(static_cast<std::int64_t>(mar_start(1) - mar_start(0))) << 2);
    std::memcpy(mar + (mar_start(1) - 1), &w(m, 0x1dc0 + model::g_a_index[0] * 4),
                static_cast<std::size_t>(static_cast<std::int64_t>(mar_start(2) - mar_start(1))) << 2);
}

// @src 1401c5260  the objects' dynamic arrays into the DB core's tables
void collect_dynamic_arrays(DB_STEP_DATA& m)
{
    CowVector<std::int32_t> ints;  // 140159ad0
    CowVector<std::int32_t> reals; // 140159ad0
    if (!scan_dynamic(m, &ints, &reals)) {
        return;
    }
    std::int32_t ni = static_cast<std::int32_t>(ints.size());   // 1401c57f0
    std::int32_t nr = static_cast<std::int32_t>(reals.size());
    const std::int32_t* real_sizes = reals.mutable_begin();      // 1401c57c0
    const std::int32_t* int_sizes = ints.mutable_begin();
    set_dynamic_tables(&ni, &nr, int_sizes, real_sizes);         // 1403bc2c4
    std::int32_t int_slot = 0;
    std::int32_t real_slot = 0;
    for (std::int32_t i = 0; i < w(m, 0x1378); i = i + 1) {
        auto* o = static_cast<LLOBJ_DATA*>(model::step_object_at(m, i + 1));
        if (o != nullptr && not_empty(o)) {
            list_put(&o->uba8, &int_slot, &real_slot, field<std::int32_t>(o, 8));
        }
    }
}

} // namespace def::dbio::gendb
