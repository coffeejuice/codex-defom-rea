// Boundary topology slot functions (defmodel/bc/boundary_topology.hpp, packet kw-g2bc).
// Decompiles: deform-rea-work/out/g2bc_ec/ec.c, elm.c, m2.c.
#include <defmodel/bc/boundary_topology.hpp>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::model::bc {

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&base) + off);
}
template <class T, class C>
const T& at(const C& base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&base) + off);
}

constexpr std::size_t kMesh = 0x87c;      // mesh present
constexpr std::size_t kElems = 0x884;     // element count
constexpr std::size_t kSegments = 0x88c;  // boundary segment total (140165d30)
constexpr std::size_t kNBdry = 0xbd8;     // boundary loop / surface count
constexpr std::size_t kBdry = 0xbe0;      // 100 x Cls_14017e740

// FUN_1400015e0 / FUN_1400015f0 on a model pointer: dimension (+0x1c) == 2 / == 3
bool dim_is(const void* model, std::int32_t d)
{
    return *reinterpret_cast<const std::int32_t*>(static_cast<const std::byte*>(model) + 0x1c) == d;
}

const Cls_14017e740& bdry(const LLOBJ_DATA& obj, int k)
{
    return reinterpret_cast<const Cls_14017e740*>(&at<std::byte>(obj, kBdry))[k];
}

const std::int32_t* ints(const void* p) { return static_cast<const std::int32_t*>(p); }

} // namespace

// @src 14039d3a0  message printf (fmt, two strings, one int) through 14039d570: vsprintf into a
// 4096-byte buffer, strcat "\n", fflush(stdout), fputs, fflush(NULL); no exit (moved here from
// p67-2's defcmd/src/ifd/cube/cells.cpp at the P6-7 gate)
void alloc_message(const char* fmt, const char* a, const char* b, std::int32_t n)
{
    char buf[4096];
    def::rt::crt::snprintf(buf, sizeof buf, fmt, a, b, n);
    std::strcat(buf, "\n");
    std::fflush(stdout);
    std::fputs(buf, stdout);
    std::fflush(nullptr);
}

// @src 140256dc0  calloc n ints, null for n == 0; message 14039d3a0 when calloc fails (added at
// the P6-7 gate from p67-2's copy)
std::int32_t* calloc_ints(std::int32_t n)
{
    if (n == 0) return nullptr;
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 4);
    if (p == nullptr) {
        // str 1405e4e50, 1405e4e40, 1405e4e38
        alloc_message("%s Error when allocating %s memory of size %d\n", "ERROR    ", "INT", n);
    }
    return static_cast<std::int32_t*>(p);
}

// @src 140257150  calloc n doubles, null for n == 0; message 14039d3a0 when calloc fails (added at
// the P6-7 gate, as calloc_ints)
double* calloc_doubles(std::int32_t n)
{
    if (n == 0) return nullptr;
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 8);
    if (p == nullptr) {
        // str 1405e5098, 1405e5088, 1405e507c
        alloc_message("%s Error when allocating %s memory of size %d\n", "ERROR    ", "DOUBLE", n);
    }
    return static_cast<double*>(p);
}

// @src 14017ebc0  segments of a boundary record by the dimension of its model (+0x28)
std::int32_t boundary_segment_count(const Cls_14017e740& b)
{
    if (dim_is(b.u028, 2)) return b.u000 - 1;
    if (dim_is(b.u028, 3)) return b.u010;
    return -1;
}

// @src 14017e8a0  (re)allocate the node list and the face arrays of a boundary record
void alloc_boundary_arrays(Cls_14017e740& b)
{
    if (0 < b.u000) {
        if (b.u008 != nullptr) std::free(b.u008);
        b.u008 = nullptr;
        // The original allocates u000 ints. Its readers 140200c90 / 140200ec0 read one entry past
        // the end (index u000); the port allocates one more zeroed int so that read is defined
        // and sees 0 (the original sees heap bytes; 0 matches no node and fails every validity
        // test of the writers).
        b.u008 = calloc_ints(b.u000 + 1);
    }
    if (0 < b.u010) {
        if (b.u018 != nullptr) std::free(b.u018);
        b.u018 = nullptr;
        b.u018 = calloc_ints(b.u010 << 2);
        if (b.u020 != nullptr) std::free(b.u020);
        b.u020 = nullptr;
        b.u020 = calloc_ints(b.u010);
    }
}

// @src 140163300  boundary count, at most 100
void set_boundary_count(LLOBJ_DATA& obj, std::int32_t n)
{
    std::int32_t v = n;
    if (100 < n) v = 100;
    at<std::int32_t>(obj, kNBdry) = v;
}

// @src 140200a70  face count of boundary record `bdry`, then its arrays
void set_boundary_face_count(LLOBJ_DATA& obj, std::int32_t nfaces, std::int32_t bdry_no)
{
    if (bdry_no < at<std::int32_t>(obj, kNBdry)) {
        Cls_14017e740& b = const_cast<Cls_14017e740&>(bdry(obj, bdry_no));
        b.u010 = nfaces;
        alloc_boundary_arrays(b);
    } else {
        def::rt::crt::printf("ERROR: iBdryNo = %d, larger than NoMeshBdry = %d\n", bdry_no,
                             at<std::int32_t>(obj, kNBdry));
    }
}

// @src 140165d30  obj+0x88c = sum of the segment counts of the boundary records
void recount_boundary_segments(LLOBJ_DATA& obj)
{
    at<std::int32_t>(obj, kSegments) = 0;
    for (std::int32_t k = 0; k < at<std::int32_t>(obj, kNBdry); ++k) {
        std::int32_t n = boundary_segment_count(bdry(obj, k));
        at<std::int32_t>(obj, kSegments) = n + at<std::int32_t>(obj, kSegments);
    }
}

// @src 140165170  number of edge/face slots
std::int32_t edge_face_slot_count(LLOBJ_DATA& obj)
{
    // The original leaves the result uninitialised for a model that is neither 2D nor 3D; the
    // port returns 0 then.
    std::int32_t r = 0;
    if (at<std::int32_t>(obj, kMesh) == 0 || at<std::int32_t>(obj, kElems) == 0 ||
        at<std::int32_t>(obj, kNBdry) == 0) {
        r = 0;
    } else {
        recount_boundary_segments(obj);
        const DB_STEP_DATA* m = obj.get_owner();
        if (dim_is(m, 2)) {
            r = at<std::int32_t>(obj, kSegments) + -2 + at<std::int32_t>(obj, kNBdry) * 2;
        } else if (dim_is(m, 3)) {
            r = at<std::int32_t>(obj, kSegments);
        }
        if (r < 0) r = 0;
    }
    return r;
}

// @src 140200c90  2D edge (n1, n2) -> slot; each loop k reserves node count + 1 slots
std::int32_t edge_slot(const LLOBJ_DATA& obj, std::int32_t n1, std::int32_t n2)
{
    std::int32_t off = 0;
    std::int32_t r = -1;
    bool found = false;
    if (at<std::int32_t>(obj, kMesh) == 0) return -1;
    for (std::int32_t k = 0; k < at<std::int32_t>(obj, kNBdry) && !found; ++k) {
        const Cls_14017e740& b = bdry(obj, k);
        const std::int32_t* nodes = ints(b.u008);
        for (std::int32_t j = 0; j < b.u000; ++j) {
            // j = u000 - 1 reads nodes[u000] (see alloc_boundary_arrays)
            if ((nodes[j] == n1 && nodes[j + 1] == n2) || (nodes[j] == n2 && nodes[j + 1] == n1)) {
                found = true;
                r = off + j;
                break;
            }
        }
        off = off + 1 + b.u000;
    }
    return r;
}

// @src 140200ff0  3D face (n1..n4) -> face index of surface 0
std::int32_t face_slot(const LLOBJ_DATA& obj, std::int32_t n1, std::int32_t n2, std::int32_t n3,
                       std::int32_t n4)
{
    std::int32_t mode = 3;
    if (at<std::int32_t>(obj, kMesh) != 0) {
        const Cls_14017e740& b = bdry(obj, 0);
        const std::int32_t* F = ints(b.u018);
        // The original reads F[0] unconditionally (a crash without faces); the port returns -1.
        if (F == nullptr) return -1;
        if (F[0] != F[3]) mode = 4;
        if (mode == 4) {
            for (std::int32_t i = 0; i < b.u010; ++i) {
                bool all = true;
                for (std::int32_t c = 0; c < 4; ++c) {
                    std::int32_t v = F[static_cast<std::int64_t>(i << 2) + c];
                    if (v != n1 && v != n2 && v != n3 && v != n4) {
                        all = false;
                        break;
                    }
                }
                if (all) return i;
            }
        } else {
            for (std::int32_t i = 0; i < b.u010; ++i) {
                bool all = true;
                for (std::int32_t c = 0; c < 3; ++c) {
                    std::int32_t v = F[static_cast<std::int64_t>(i << 2) + c];
                    if (v != n1 && v != n2 && v != n3) {
                        all = false;
                        break;
                    }
                }
                if (all) return i;
            }
        }
    }
    return -1;
}

// @src 140200ec0  2D slot -> the two nodes of its loop entry
void loop_slot_nodes(std::int32_t s, const std::int32_t* nloops, const Cls_14017e740* loops,
                     std::int32_t* p, std::int32_t* q)
{
    std::int32_t off = 0;
    *q = 0;
    *p = 0;
    if (*nloops != 0) {
        for (std::int32_t k = 0; k < *nloops; ++k) {
            if (off <= s && s < off + loops[k].u000) {
                const std::int32_t* nodes = ints(loops[k].u008);
                *p = nodes[s - off];
                *q = nodes[(s - off) + 1];  // s - off = u000 - 1 reads nodes[u000]
                return;
            }
            off = off + 1 + loops[k].u000;
        }
    }
}

// @src 140200e20  2D slot -> nodes, when the object has mesh, elements and boundary
void edge_slot_nodes(const LLOBJ_DATA& obj, std::int32_t s, std::int32_t* p, std::int32_t* q)
{
    *q = 0;
    *p = 0;
    if (at<std::int32_t>(obj, kMesh) != 0 && at<std::int32_t>(obj, kElems) != 0 &&
        at<std::int32_t>(obj, kNBdry) != 0) {
        loop_slot_nodes(s, &at<std::int32_t>(obj, kNBdry), &bdry(obj, 0), p, q);
    }
}

// @src 140201210  3D face index -> its 4 stored nodes (surface 0)
void face_nodes(const LLOBJ_DATA& obj, std::int32_t i, std::int32_t* a, std::int32_t* b,
                std::int32_t* c, std::int32_t* d)
{
    *d = 0;
    *c = 0;
    *b = 0;
    *a = 0;
    if (at<std::int32_t>(obj, kMesh) != 0 && at<std::int32_t>(obj, kElems) != 0 &&
        at<std::int32_t>(obj, kNBdry) != 0) {
        const std::int32_t* F = ints(bdry(obj, 0).u018);
        *a = F[static_cast<std::int64_t>(i << 2)];
        *b = F[static_cast<std::int64_t>(i * 4 + 1)];
        *c = F[static_cast<std::int64_t>(i * 4 + 2)];
        *d = F[static_cast<std::int64_t>(i * 4 + 3)];
    }
}

} // namespace def::model::bc
