// defmodel/mesh_gen/sf_list_ops.hpp: the SfLinkedList / SfLinkedData / DYNARY_DATA methods that
// the OPSTOP and DYNARY keywords use and p1-06 did not port (packet kw-g2mg). Free functions:
// the layout headers are the skeleton thread's. Specs: docs/re/keywords/OPSTOP.md, DYNARY.md.
#pragma once

#include <cstdint>

#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/sf_linked_data.hpp>
#include <defmodel/layout/sf_linked_list.hpp>

namespace def::model {

// ---- SfLinkedList -----------------------------------------------------------------------------
std::int32_t sf_list_count(const SfLinkedList* l);                    // 14019a580 (+0x18)
SfLinkedData* sf_list_append(SfLinkedList* l);                        // 14025a7d0 (slot 1 factory)
SfLinkedData* sf_list_nth(const SfLinkedList* l, std::int32_t n);    // 14025a9e0 (1-based, n<=1: head)
SfLinkedData* sf_list_get_or_create(SfLinkedList* l, std::int32_t n); // 14025a990

// ---- SfLinkedData -----------------------------------------------------------------------------
void sf_data_set_active(SfLinkedData* d, std::uint8_t on);  // 14025a640 (+0x08)
std::uint8_t sf_data_active(const SfLinkedData* d);         // 14025a660

// ---- DYNARY_DATA ------------------------------------------------------------------------------
// Fields: +0x0c kind (SfLinkedData::u0c), +0x18 subtype, +0x1c isInt, +0x20 storage class,
// +0x24 count, +0x28 ncomp, +0x30 double array, +0x38 int array.
std::int32_t dynary_data_kind(const DYNARY_DATA* d);      // 14025b310
std::int32_t dynary_data_subtype(const DYNARY_DATA* d);   // 14025b350
std::int32_t dynary_data_count(const DYNARY_DATA* d);     // 14025b370
std::int32_t dynary_data_ncomp(const DYNARY_DATA* d);     // 14025b3b0
std::int32_t dynary_data_class(const DYNARY_DATA* d);     // 14025b3f0
std::uint8_t dynary_data_is_int(const DYNARY_DATA* d);    // 14025b960
// 14025b220: kind, subtype, count, ncomp (setters 14025b2f0/b330/b390/b3d0), isInt and class
// from (kind 1, subtype 1/2/3), then dynary_data_resize.
void dynary_data_set_header(DYNARY_DATA* d, std::int32_t kind, std::int32_t subtype,
                            std::int32_t count, std::int32_t ncomp);
// 14025b4d0: resizes the active array (int if isInt, else double) to count * ncomp, new
// elements 0; true if the size changed.
bool dynary_data_resize(DYNARY_DATA* d);
// 14025b410: element count of the active array (0 when kind is 0).
std::int32_t dynary_data_size(const DYNARY_DATA* d);
// 14025b470 / 14025b4a0: detach, then the first element of the double / int array.
double* dynary_data_doubles(DYNARY_DATA* d);
std::int32_t* dynary_data_ints(DYNARY_DATA* d);
// 14025b580: every element of the double array (dynary_data_size of them) = v.
void dynary_data_fill(DYNARY_DATA* d, double v);
// 14025bb50: n-th node of the list cast to DYNARY_DATA (null if none or not one).
DYNARY_DATA* dynary_list_nth(const SfLinkedList* l, std::int32_t n);

} // namespace def::model
