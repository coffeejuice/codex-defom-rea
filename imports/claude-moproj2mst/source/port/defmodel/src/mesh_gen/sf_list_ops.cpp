// SfLinkedList / SfLinkedData / DYNARY_DATA methods for OPSTOP and DYNARY (packet kw-g2mg).
//
// The two arrays of DYNARY_DATA (+0x30 doubles, +0x38 ints) are copy-on-write vector handles;
// p1-06 declares them as linked::RefArrayRep pointers, p1-05 ports the operations (size,
// resize, detach) as sdctl CowVector<T>, whose only member is the same body pointer. The
// functions below view the member as that handle.
#include <defmodel/mesh_gen/sf_list_ops.hpp>

#include <defmodel/sdctl/cow.hpp>

namespace def::model {

namespace {

CowVector<double>& doubles_of(DYNARY_DATA* d)
{
    static_assert(sizeof(CowVector<double>) == sizeof(d->u30));
    return *reinterpret_cast<CowVector<double>*>(&d->u30);
}
CowVector<std::int32_t>& ints_of(DYNARY_DATA* d)
{
    static_assert(sizeof(CowVector<std::int32_t>) == sizeof(d->u38));
    return *reinterpret_cast<CowVector<std::int32_t>*>(&d->u38);
}
const CowVector<double>& doubles_of(const DYNARY_DATA* d)
{
    return *reinterpret_cast<const CowVector<double>*>(&d->u30);
}
const CowVector<std::int32_t>& ints_of(const DYNARY_DATA* d)
{
    return *reinterpret_cast<const CowVector<std::int32_t>*>(&d->u38);
}

} // namespace

// @src 14019a580  count
std::int32_t sf_list_count(const SfLinkedList* l) { return l->count; }

// @src 14025a7d0  append a node made by the list's factory (vtable slot 1)
SfLinkedData* sf_list_append(SfLinkedList* l)
{
    SfLinkedData* d = l->create_item();
    if (l->count == 0) {
        l->tail = d;
        l->head = d;
    }
    else {
        l->tail->next = d;
        l->tail = d;
    }
    l->count = l->count + 1;
    return d;
}

// @src 14025a9e0  1-based node n (n <= 1: the head)
SfLinkedData* sf_list_nth(const SfLinkedList* l, std::int32_t n)
{
    std::int32_t k = 0;
    SfLinkedData* d = l->head;
    while (d != nullptr && (k = k + 1, k < n)) {
        d = d->next;
    }
    return d;
}

// @src 14025a990  append nodes until count >= n, then node n
SfLinkedData* sf_list_get_or_create(SfLinkedList* l, std::int32_t n)
{
    while (sf_list_count(l) < n) {
        sf_list_append(l);
    }
    return sf_list_nth(l, n);
}

// @src 14025a640  set the active byte
void sf_data_set_active(SfLinkedData* d, std::uint8_t on) { d->u08 = on; }

// @src 14025a660  the active byte
std::uint8_t sf_data_active(const SfLinkedData* d) { return d->u08; }

// @src 14025b310  kind
std::int32_t dynary_data_kind(const DYNARY_DATA* d) { return d->u0c; }
// @src 14025b350  subtype
std::int32_t dynary_data_subtype(const DYNARY_DATA* d) { return d->u18; }
// @src 14025b370  count
std::int32_t dynary_data_count(const DYNARY_DATA* d) { return d->u24; }
// @src 14025b3b0  ncomp
std::int32_t dynary_data_ncomp(const DYNARY_DATA* d) { return d->u28; }
// @src 14025b3f0  storage class
std::int32_t dynary_data_class(const DYNARY_DATA* d) { return d->u20; }
// @src 14025b960  isInt
std::uint8_t dynary_data_is_int(const DYNARY_DATA* d) { return d->u1c; }

// @src 14025b220  set the header
// @src 14025b2f0  (kind setter, inlined)
// @src 14025b330  (subtype setter, inlined)
// @src 14025b390  (count setter, inlined)
// @src 14025b3d0  (ncomp setter, inlined)
void dynary_data_set_header(DYNARY_DATA* d, std::int32_t kind, std::int32_t subtype,
                            std::int32_t count, std::int32_t ncomp)
{
    d->u0c = kind;
    d->u18 = subtype;
    d->u24 = count;
    d->u28 = ncomp;
    if (kind == 1) {
        if (subtype == 1) {
            d->u1c = 0;
            d->u20 = 1;
        }
        else if (subtype == 2) {
            d->u1c = 1;
            d->u20 = 2;
        }
        else if (subtype == 3) {
            d->u1c = 0;
            d->u20 = 3;
        }
    }
    dynary_data_resize(d);
}

// @src 14025b4d0  resize the active array to count * ncomp
bool dynary_data_resize(DYNARY_DATA* d)
{
    const std::int32_t n = dynary_data_count(d) * dynary_data_ncomp(d);
    if (dynary_data_size(d) == n) {
        return false;
    }
    if (dynary_data_is_int(d) == 0) {
        doubles_of(d).resize(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 0.0); // 14019a740
    }
    else {
        ints_of(d).resize(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 0); // 140159b60
    }
    return true;
}

// @src 14025b410  element count of the active array
std::int32_t dynary_data_size(const DYNARY_DATA* d)
{
    if (dynary_data_kind(d) == 0) {
        return 0;
    }
    if (dynary_data_is_int(d) == 0) {
        return static_cast<std::int32_t>(doubles_of(d).size()); // 140003090
    }
    return static_cast<std::int32_t>(ints_of(d).size()); // 1400030b0
}

// @src 14025b470  double data (1401d9050: detach 1400051a0, first)
double* dynary_data_doubles(DYNARY_DATA* d) { return doubles_of(d).mutable_begin(); }

// @src 14025b4a0  int data (1401c57c0: detach 140004e60, first)
std::int32_t* dynary_data_ints(DYNARY_DATA* d) { return ints_of(d).mutable_begin(); }

// @src 14025b580  fill the double array
void dynary_data_fill(DYNARY_DATA* d, double v)
{
    double* p = dynary_data_doubles(d);
    const std::int32_t n = dynary_data_size(d);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        p[i] = v;
    }
}

// @src 14025bb50  n-th node cast to DYNARY_DATA
DYNARY_DATA* dynary_list_nth(const SfLinkedList* l, std::int32_t n)
{
    return DYNARY_DATA::cast(sf_list_nth(l, n)); // 14025b1d0
}

} // namespace def::model
