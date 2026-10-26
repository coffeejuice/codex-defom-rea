// dbread/w4_dynary.cpp: ReadDynamicArrayFromDB (1401d2760) and the DYNARY_DATA fills it calls
// (helper W4 of packet p-dbread; docs/re/dbmap/objarrays.md section 8, object.md DYNARY row,
// db-format-spec.md section 5.4).
#include <defdbio/dbread/w4.hpp>

#include <cstdint>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "dbread/w4_access.hpp"
#include "gendb/db_header_io.hpp"

namespace def::dbio::dbread {

using model::CowVector;
using model::DB_STEP_DATA;
using model::DYNARY_DATA;
using model::DynAryLinkedList;
using model::LLOBJ_DATA;

// @src 1403bc272  DAT_140b48e60 / e64 / e88 / e8c of the datarm23 COMMON
void dynamic_array_counts(std::int32_t* ni, std::int32_t* ni_max, std::int32_t* nr, std::int32_t* nr_max)
{
    const std::int32_t v4 = dbcore::g_db.dyn_hdr[11];
    const std::int32_t v2 = dbcore::g_db.dyn_hdr[1];
    const std::int32_t v1 = dbcore::g_db.dyn_hdr[0];
    *nr = dbcore::g_db.dyn_hdr[10];
    *ni_max = v2;
    *ni = v1;
    *nr_max = v4;
}

// @src 1401c57c0  detach (140004e60), return the first element
std::int32_t* cow_ints_begin(CowVector<std::int32_t>* v)
{
    return v->mutable_begin();
}

// @src 14025b5f0  the double array's own size (140003090 on +0x30) decides the count; v is
//                 written through 140005e70 (detach, element i)
void dynary_data_copy_doubles(DYNARY_DATA* d, CowVector<double>* v)
{
    double* dst = model::dynary_data_doubles(d); // 14025b470
    const std::int32_t n = static_cast<std::int32_t>(d->u30.size());
    for (std::int32_t i = 0; i < n; i = i + 1) {
        dst[i] = v->mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
    }
}

// @src 14025b680  the same for the int array (1400030b0 on +0x38, 140005100)
void dynary_data_copy_ints(DYNARY_DATA* d, CowVector<std::int32_t>* v)
{
    std::int32_t* dst = model::dynary_data_ints(d); // 14025b4a0
    const std::int32_t n = static_cast<std::int32_t>(d->u38.size());
    for (std::int32_t i = 0; i < n; i = i + 1) {
        dst[i] = v->mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
    }
}

// @src 14025bdc0  new node (14025a7d0), cast (14025b1d0); with ncomp > 0: header (14025b220) with
//                 n / ncomp rows; values only when the array is not int (14025b960)
void dynary_list_add_real(DynAryLinkedList* list, std::int32_t kind, std::int32_t subtype, std::int32_t n,
                          std::int32_t ncomp, CowVector<double>* v)
{
    model::SfLinkedData* node = model::sf_list_append(list);
    DYNARY_DATA* d = DYNARY_DATA::cast(node);
    if (0 < ncomp && d != nullptr) {
        model::dynary_data_set_header(d, kind, subtype, n / ncomp, ncomp);
        if (model::dynary_data_is_int(d) == 0) {
            dynary_data_copy_doubles(d, v);
        }
    }
}

// @src 14025bd10  the same; values only when the array is int
void dynary_list_add_int(DynAryLinkedList* list, std::int32_t kind, std::int32_t subtype, std::int32_t n,
                         std::int32_t ncomp, CowVector<std::int32_t>* v)
{
    model::SfLinkedData* node = model::sf_list_append(list);
    DYNARY_DATA* d = DYNARY_DATA::cast(node);
    if (0 < ncomp && d != nullptr) {
        model::dynary_data_set_header(d, kind, subtype, n / ncomp, ncomp);
        if (model::dynary_data_is_int(d) != 0) {
            dynary_data_copy_ints(d, v);
        }
    }
}

// @src 1401d2760  real arrays 1..NR (1403bcda9 into a buffer of the max real size), then int
//                 arrays 1..NI (1403bcaa6); row word 5 names the object (1401d6ea0), words 6 / 7 /
//                 8 kind, subtype, components
// @src 1401d9050  (detach + first of the local double buffer, inlined as mutable_begin)
void read_dynamic_arrays(DB_STEP_DATA* m)
{
    std::int32_t ni = 0;     // rsp+0x7c
    std::int32_t ni_max = 0; // rsp+0x90
    std::int32_t nr = 0;     // rsp+0x78
    std::int32_t nr_max = 0; // rsp+0x84
    dynamic_array_counts(&ni, &ni_max, &nr, &nr_max);
    std::int32_t mode = 1;   // rsp+0x70
    std::int32_t n = 0;      // rsp+0x60
    std::int32_t owner = 0;  // rsp+0x6c
    std::int32_t kind = 0;   // rsp+0x64
    std::int32_t subtype = 0; // rsp+0x68
    std::int32_t ncomp = 0;  // rsp+0x80
    std::int32_t unused1 = 0; // rsp+0x8c
    std::int32_t unused2 = 0; // rsp+0x74
    std::int32_t ierr = 0;   // rsp+0x88
    if (0 < nr && 0 < nr_max) {
        CowVector<double> buf;                                       // 14019a6b0
        const double zero = 0.0;
        buf.resize(static_cast<std::size_t>(static_cast<std::int64_t>(nr_max)), zero); // 14019a740
        for (std::int32_t k = 1; k <= nr; k = k + 1) {
            double* data = buf.mutable_begin();                     // 1401d9050
            gendb::dynamic_real(&mode, &k, data, &n, &owner, &kind, &subtype, &ncomp, &unused1, &unused2,
                                &ierr);
            if (ierr == 0) {
                LLOBJ_DATA* obj = model::object_list_find(&m->objects, owner); // 1401d6ea0
                if (obj == nullptr) {
                    rt::crt::printf("Error occurred on loading Dynamic Array (Object %d)\n", owner);
                } else {
                    dynary_list_add_real(&obj->uba8, kind, subtype, n, ncomp, &buf);
                }
            } else {
                rt::crt::printf("Error occurred on loading Dynamic Array (code:%d)\n", ierr);
            }
        }
    } // 14019a720
    if (0 < ni && 0 < ni_max) {
        CowVector<std::int32_t> buf;                                 // 140159ad0
        const std::int32_t zero = 0;
        buf.resize(static_cast<std::size_t>(static_cast<std::int64_t>(ni_max)), zero); // 140159b60
        for (std::int32_t k = 1; k <= ni; k = k + 1) {
            std::int32_t* data = cow_ints_begin(&buf);              // 1401c57c0
            gendb::dynamic_int(&mode, &k, data, &n, &owner, &kind, &subtype, &ncomp, &unused1, &unused2,
                               &ierr);
            if (ierr == 0) {
                LLOBJ_DATA* obj = model::object_list_find(&m->objects, owner);
                if (obj == nullptr) {
                    rt::crt::printf("Error occurred on loading Dynamic Array (Object %d)\n", owner);
                } else {
                    dynary_list_add_int(&obj->uba8, kind, subtype, n, ncomp, &buf);
                }
            } else {
                rt::crt::printf("Error occurred on loading Dynamic Array (code:%d)\n", ierr);
            }
        }
    } // 140159b40
}

} // namespace def::dbio::dbread
