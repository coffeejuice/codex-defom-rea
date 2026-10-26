// defmodel/geoprim/polygon.hpp: 2D polygon helpers of the GEOPRIM extrude (packet p9-04).
//
//   segment crossing    140308120 / 140308340, box test 140309960
//   simple-profile test 1402e7f40 (only the closing edge against the others)
//   min-area triangulation of a polygon: the solver at 0x140420750..0x1404213c0 (no RTTI,
//   0x48 bytes, Cls_140420750), driven by 1402e24f0
//
// Spec: docs/re/commands/CNVT3D_SWEEP.md sections 10.5 and 10.6. The template code of the
// solver's COW vector/list containers (0x1404219e0..0x140422a40) is replaced by std containers;
// the element order and the results are the same (status file: not-needed, stl).
#pragma once

#include <cstdint>
#include <list>
#include <vector>

namespace def::model::geoprim {

struct ProfileArray;

// @src 140308340  crossing of the lines through (x1,y1)-(x2,y2) and (x3,y3)-(x4,y4) when each
// segment's end points lie on different sides of (or on) the other line: 0 no crossing,
// 2 parallel (|det| < 1e-12), 1 crossing at (*x, *y). Operation order of the listing.
std::int32_t fun_140308340(double x1, double y1, double x2, double y2, double x3, double y3,
                           double x4, double y4, double* x, double* y);

// @src 140308120  the same on two segments stored as two 24-byte points each (s[0], s[1] and
// s[3], s[4]); out = the crossing for 1, else (0, 0). Returns 0, 1 or 2.
std::int32_t fun_140308120(const double* s1, const double* s2, double* out);

// @src 140309960  b[0] <= p.x <= b[2] and b[1] <= p.y <= b[3] (end points as given, unsorted)
bool fun_140309960(const double* p, const double* b);

// @src 1402e7f40  "simple" test of a profile (1-based points): the closing edge p1-pn against
// every edge p_i-p_{i+1}; the answer is cached in p->u10 (true at once when set).
bool fun_1402e7f40(ProfileArray* p);

// One entry of the dynamic-programme table (0x18 bytes in the binary): best middle vertex and
// the cost pair.
struct TriCost {
    double a;  // compared first; the max is kept when combining (180 for every triangle)
    double b;  // compared second; summed when combining (absolute triangle areas)
};
struct TriEntry {
    std::int32_t best;
    TriCost cost;
};

// @src 140421810  combine: (max(a.a, b.a) with a.a <= b.a -> b.a, a.b + b.b)
TriCost fun_140421810(const TriCost& x, const TriCost& y);
// @src 1404218a0  strict lexicographic less
bool fun_1404218a0(const TriCost& x, const TriCost& y);

// The solver object of 1402e24f0 (Cls_140420750).
class Cls_140420750 {
public:
    // @src 140420750  ctor
    Cls_140420750();
    // @src 1404207f0  dtor (containers release themselves)
    ~Cls_140420750() = default;

    // @src 140420890  triangulate the polygon v[0..n) over the points coords (3 doubles per id)
    bool run(std::int32_t n, const std::int32_t* v, const double* coords);
    // @src 1402f0930  the result: 3 ids per triangle
    const std::vector<std::int32_t>& result() const { return tris_; }
    double area() const { return area_; }

    // exposed for tests
    // @src 140421510  split at the shortest diagonal until parts have < 200 vertices
    void split(std::vector<std::int32_t>& poly);
    // @src 140420a30  dynamic programme on one part
    bool solve(const std::vector<std::int32_t>& poly);

private:
    // @src 140420840  reset the part state
    void reset_part();
    // @src 140421060  table of (k + 1) * n entries {-1, (180, 1e29)}
    void init_table();
    // @src 140421920  chord length d(i, j) = (j - i - 1) mod n
    std::int32_t chord(std::int32_t i, std::int32_t j) const;
    // @src 140421120  entry (i, j) = table[d * n + i]
    TriEntry& entry(std::int32_t i, std::int32_t j);
    // @src 140421210  |area| of the triangle of the part's vertices i, j, k
    double tri_area(std::int32_t i, std::int32_t j, std::int32_t k) const;
    // @src 1404211a0  triangle cost (180, |area|)
    TriCost tri_cost(std::int32_t i, std::int32_t j, std::int32_t k) const;
    // @src 1404213c0  emit the triangles of chord (i, j), pre-order
    void emit(std::int32_t i, std::int32_t j);

    std::vector<TriEntry> table_;        // 0x00 (COW vector of 0x18-byte entries)
    const std::int32_t* v_ = nullptr;    // 0x08 vertex ids of the current part
    std::int32_t n_ = 0;                 // 0x10 vertex count of the current part
    const double* coords_ = nullptr;     // 0x18 points, 3 doubles per id
    std::int32_t k_ = -1;                // 0x20 n - 2
    std::vector<std::int32_t> tris_;     // 0x28 result, 3 ids per triangle
    double area_ = 0.0;                  // 0x30 summed |area|
    std::list<std::vector<std::int32_t>> parts_;  // 0x38 parts below 200 vertices
};

} // namespace def::model::geoprim
