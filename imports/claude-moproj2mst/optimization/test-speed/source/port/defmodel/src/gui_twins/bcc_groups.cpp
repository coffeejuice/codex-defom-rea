// BCC groups after a DB copy (GUI twin 0x14087e1a0 = DEF_PRE 14020a340) and its callees
// (header: defmodel/gui_twins.hpp). Decompiles: deform-rea-work/out/mdt3/f.c, g.c.
//
// 14020a340 calls 140206fd0 (owner defcmd p67_5, genbcc_bc.cpp) with mode 1, which only calls
// 140207f50. defmodel cannot call defcmd, and 140207f50 had no port (defcmd's 140206fd0 stops in
// DEF_NOT_PORTED(0x140207f50) for modes 1 and 2), so 140207f50 is ported here and the mode-1
// dispatch is written out inline.
#include <defmodel/gui_twins.hpp>

#include <bit>
#include <cmath>
#include <cstdint>
#include <vector>

#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/rdata.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::model {

namespace {
std::int32_t neg(std::int32_t v) { return static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)); }
} // namespace

// @src 140165eb0  BC code of a node from the first negative entry of its row of obj+0x990
std::int32_t node_bc_code(const LLOBJ_DATA* obj, std::int32_t node, const std::int32_t* tbl)
{
    std::int32_t r;
    if (node < 1) {
        r = 0;
    } else {
        r = 0;
        const std::int32_t w = tbl[0x48 / 4];
        const std::int32_t* a = field<std::int32_t*>(obj, 0x990);
        for (std::int32_t c = 0; c < w; c = c + 1) {
            if (a[(node - 1) * w + c] < 0) {
                r = neg(a[(node - 1) * w + c]);
                if (200 < r && r < 300) r = r - 200;
                break;
            }
        }
        if (r < 0 || 100 < r) r = 0;
    }
    return r;
}

// @src 140207f50  clear BC entries of the listed nodes by mode `code`
void bcc_clear_nodes(const LLOBJ_DATA* obj, const std::int32_t* nodes, std::int32_t n,
                     std::int32_t* a, std::int32_t* b, double* c, std::int32_t code,
                     const std::int32_t* w_a, const std::int32_t* w_b, std::int32_t comp,
                     std::int32_t value)
{
    const std::int32_t lim = (*w_a < *w_b) ? *w_a : *w_b;
    for (std::int32_t k = 0; k < n; k = k + 1) {
        const std::int32_t r = nodes[k] - 1;
        if (!rt::dimension_is_3(field<const void*>(obj, 15000)) || // 1400015f0(owner)
            (code != 5 && code != 4 && code != 6 && code != 7 && code != 8)) {
            if (code == 1) {
                if (comp == -1 && -1 < value) return;
                if (a[r * *w_a + comp] == 1) {
                    a[r * *w_a + comp] = 0;
                    b[r * *w_b + comp] = 0;
                    c[r * *w_a + comp] = 0.0;
                }
            } else if (code == 0) {
                if (comp == -1) return;
                const double v = std::fabs(c[r * *w_a + comp]); // 140484558
                if (rt::rdata::d_14051f260 < v) c[r * *w_a + comp] = 0.0; // DAT_14051f260: 1e-12
                if (a[r * *w_a + comp] == 0 && b[r * *w_b + comp] != 0) {
                    b[r * *w_b + comp] = 0;
                    a[r * *w_b + comp] = 0; // the binary indexes a with *w_b here
                }
            } else if (code < 0) {
                for (std::int32_t j = 0; j < lim; j = j + 1)
                    if (a[r * *w_a + j] == code) a[r * *w_a + j] = 0;
            } else if (code == 3 || code == 0xb || code == 0xc) {
                for (std::int32_t j = 0; j < lim; j = j + 1)
                    if (a[r * *w_a + j] == code) a[r * *w_a + j] = 0;
            }
        } else {
            for (std::int32_t j = 0; j < lim; j = j + 1) {
                if (a[r * *w_a + j] == code && b[r * *w_b + j] == value) {
                    b[r * *w_a + j] = 0; // the binary indexes b with *w_a here
                    a[r * *w_a + j] = 0;
                    break;
                }
            }
        }
    }
}

// @src 14020a340  BCC groups after a DB copy (GUI twin 0x14087e1a0)
void bcc_groups_from_model(LLOBJ_DATA* obj, const std::int32_t* tbl)
{
    // local_80: COW int vector (140159860) of codes; local_70: COW vector of COW int vectors
    // (140220cf0), one node list per code
    std::vector<std::int32_t> codes;
    std::vector<std::vector<std::int32_t>> groups;
    if (tbl == nullptr) return;
    std::int32_t ncomp = tbl[0x48 / 4];
    std::int32_t w4c = tbl[0x4c / 4];
    for (std::int32_t i = 0; i < field<std::int32_t>(obj, 0x880); i = i + 1) {
        const std::int32_t code = node_bc_code(obj, i + 1, tbl); // 140165eb0
        if (code == 0) continue;
        std::int32_t idx;
        std::int32_t count = 0; // 140220750: occurrences of code
        for (std::int32_t v : codes)
            if (v == code) ++count;
        if (count == 0) {
            idx = static_cast<std::int32_t>(codes.size()); // 140159950
            codes.push_back(code);                        // 140220700
            groups.emplace_back();                        // 140159ad0, 140220de0
        } else {
            idx = 0;
            while (static_cast<std::size_t>(idx) < codes.size() && codes[idx] != code) idx = idx + 1;
        }
        groups[idx].push_back(i + 1); // 140220da0, 140220910
    }
    if (groups.size() == 0) return; // 140220d80
    if (!p407::obj_kind_is_2(obj)) { // 140158d00
        for (std::size_t k = 0; k < codes.size(); k = k + 1) {
            const std::int32_t c = codes[k];
            const std::int32_t mc = neg(c);
            const std::int32_t n = static_cast<std::int32_t>(groups[k].size()); // 1400030b0
            const std::int32_t* nodes = groups[k].data();                        // 1401c57c0
            // 140206fd0(obj, nodes, n, *(obj+0x990), *(obj+0x998), 0, 0, -c, 0, 0, &ncomp, &w4c,
            // mode 1, 0, 0, 0): mode 1 is 140207f50(obj, nodes, n, a, b, 0, -c, &ncomp, &w4c, 0, 0)
            bcc_clear_nodes(obj, nodes, n, field<std::int32_t*>(obj, 0x990),
                            field<std::int32_t*>(obj, 0x998), nullptr, mc, &ncomp, &w4c, 0, 0);
        }
    } else {
        const std::int32_t ng = static_cast<std::int32_t>(groups.size());
        for (std::int32_t k = 0; k < ng; k = k + 1) {
            const std::vector<std::int32_t> g = groups[k]; // std::locale copy in the decompile: COW copy
            const std::int32_t gn = static_cast<std::int32_t>(g.size());
            for (std::int32_t j = 0; j < gn; j = j + 1) {
                const std::int32_t r = g[j] - 1; // 140005100
                std::int32_t c = ncomp;
                if (field<std::int32_t*>(obj, 0x990) != nullptr) {
                    while (c - 1 > -1) {
                        c = c - 1;
                        std::int32_t* a = field<std::int32_t*>(obj, 0x990);
                        if (a[r * ncomp + c] < 0) a[r * ncomp + c] = 0;
                    }
                }
            }
        }
    }
}

} // namespace def::model
