// kw-g2geo boundary family: the boundary build of ELMCON (1401fc290). The Fortran finders are
// in boundary_fl2.cpp / boundary_fl3.cpp, the sort in boundary_qsort.cpp; the boundary record
// and EC* array helpers (14017e8a0, 140163300, 140200a70, 140165d30, 1401631e0, ...) are
// packet kw-g2bc's (defmodel/bc/).
#include <defmodel/geo_kw/boundary.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>

namespace def::model::geo_kw {

namespace {

template <class T>
T& at(LLOBJ_DATA& obj, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&obj) + off);
}

Cls_14017e740& record(LLOBJ_DATA& obj, std::int32_t i)
{
    return reinterpret_cast<Cls_14017e740*>(&at<std::byte>(obj, 0xbe0))[i];
}

// FUN_1400015e0 on the owner: dimension (+0x1c) == 2
bool owner_is_2d(LLOBJ_DATA& obj)
{
    const auto* m = reinterpret_cast<const std::byte*>(obj.owner);
    return *reinterpret_cast<const std::int32_t*>(m + 0x1c) == 2;
}

} // namespace

// @src 1401fc290  ELMCON's boundary build
void build_boundary(LLOBJ_DATA& obj)
{
    if (at<std::int32_t>(obj, 0x87c) == 0 || at<std::int32_t*>(obj, 0x9e8) == nullptr ||
        obj.owner == nullptr)
        return;
    std::int32_t* conn = at<std::int32_t*>(obj, 0x9e8);

    if (owner_is_2d(obj)) {
        std::int32_t nelem = at<std::int32_t>(obj, 0x884);
        std::int32_t nchain = 0;
        // node buffer: max(2 * nelem + 0x3ea, 10000) ints (140256dc0). FL2_FNDBND does not
        // check it (nor the 20000-int chain range array on the stack, rsp+0x60); the port gives
        // both room for the worst case (every element edge on the boundary) so that the copy
        // below reads what the original reads whenever the original stays in bounds.
        std::int32_t nbuf = static_cast<std::int32_t>(static_cast<std::uint32_t>(nelem) * 2u + 0x3eau);
        if (nbuf < 10000) nbuf = 10000;
        const std::int64_t ne = nelem < 0 ? 0 : nelem;
        std::vector<std::int32_t> nodes(static_cast<std::size_t>(std::max<std::int64_t>(nbuf, 6 * ne + 2)), 0);
        std::vector<std::int32_t> ranges(static_cast<std::size_t>(std::max<std::int64_t>(20000, 8 * ne + 2)), 0);
        fl2_fndbnd(&nelem, conn, &nchain, ranges.data(), nodes.data());  // 14032d65e, flag 0
        at<std::int32_t>(obj, 0xbd8) = nchain < 0x65 ? nchain : 100;
        for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0xbd8) && i < 100; ++i) {
            Cls_14017e740& r = record(obj, i);
            r.u000 = ranges[static_cast<std::size_t>(i) * 2 + 1] - ranges[static_cast<std::size_t>(i) * 2] + 1;
            bc::alloc_boundary_arrays(r);  // 14017e8a0
            const std::size_t bytes = static_cast<std::size_t>(static_cast<std::int64_t>(r.u000)) << 2;
            if (bytes != 0)  // (memcpy of 0 bytes from/to null is a no-op in the original)
                std::memcpy(r.u008, nodes.data() + (ranges[static_cast<std::size_t>(i) * 2] + -1), bytes);
        }
    } else {
        std::int32_t nelem = at<std::int32_t>(obj, 0x884);
        std::int32_t nnpe = at<std::int32_t>(obj, 0x894);
        std::int32_t nface = 0;
        // face arrays: obj+0x898 (faces per element) * nelem faces (140256dc0); the finder
        // writes up to nfe * nelem, which is the same for the element types ELMCON sets; the port
        // allocates the larger of the two.
        const std::int32_t fpe = at<std::int32_t>(obj, 0x898);
        const std::int64_t ne = nelem < 0 ? 0 : nelem;
        const std::int64_t want = std::max<std::int64_t>(std::int64_t(fpe) * ne, 6 * ne);
        std::vector<std::int32_t> faces(static_cast<std::size_t>(want * 4 + 4), 0);
        std::vector<std::int32_t> elem(static_cast<std::size_t>(want + 1), 0);
        fl3_fndbnd(&nelem, conn, &nnpe, &nface, faces.data(), elem.data());  // 140354ed0, flag 0
        bc::set_boundary_count(obj, 1);           // 140163300
        bc::set_boundary_face_count(obj, nface, 0);  // 140200a70
        Cls_14017e740& r = record(obj, 0);
        // 1401fc659: (nface << 2) as int, then << 2 as 64-bit
        const std::size_t fbytes = static_cast<std::size_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(
                                       static_cast<std::uint32_t>(nface) << 2)) << 2);
        const std::size_t ebytes = static_cast<std::size_t>(static_cast<std::int64_t>(nface) << 2);
        if (fbytes != 0) std::memcpy(r.u018, faces.data(), fbytes);
        if (ebytes != 0) std::memcpy(r.u020, elem.data(), ebytes);
    }
    bc::recount_boundary_segments(obj);  // 140165d30
    bc::realloc_edge_face_arrays(obj);   // 1401631e0
}

} // namespace def::model::geo_kw
