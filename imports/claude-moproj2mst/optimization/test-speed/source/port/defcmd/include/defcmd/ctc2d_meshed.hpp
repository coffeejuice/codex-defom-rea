// defcmd/ctc2d_meshed.hpp: meshed-master contact of 2D GENCTC (GENCTC.md 4.5): the driver
// 14034fc56 (declared in ctc2d_ftn.hpp) and its nearest node / segment search 140434952, with the
// SAVE storage of both routines. PathScale keeps the locals at fixed addresses; several are read
// before they are written in a call (14034fc56: the best segment 1407036b8 after the loop loop,
// the segment/orientation outputs 1407036b0/1407036b4 that 140434952 leaves untouched when it
// finds nothing; 140434952: the best signed distance 14070d6c8 after its segment loop), so all of
// them are kept here at their addresses. reset_ctc2d_meshed_save() zeroes them (.bss state).
//
// Note on DAT_140bb1558 / DAT_140bb1560 (+-1.5707950592041016): Ghidra's decompile shows a write
// "DAT_140bb1558 = DAT_14070d700"; the listing (14043537c..1404353ad, 140435447..140435480) only
// loads the constant into the SAVE angle 14070d700 and passes &14070d700 to 14043556b. The
// constants are never written, so they stay constexpr in ctc2d_meshed.cpp.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::cmd::ftn {

// @src 140434952  nearest master node / segment of one master boundary loop to point p.
//   mxy      master coordinates xy(2, *)
//   p        the slave point (read only; a copy is moved by the line kernel)
//   foot     out: foot point on the nearest segment (written only when a segment is accepted)
//   dist     in: start value of the nearest-node search and the bound of the segment search;
//            out: |d| of the nearest segment (ties: the later segment wins)
//   iseg     out: position (1..*n, in the loop list) of the nearest segment, or of the nearest
//            node when the corner rule applies; untouched when no segment beats *dist
//   n        number of positions of the loop list
//   orient   out: 1 / -1 sign of the side, 0 when *dist == 0; untouched when *iseg == 0
//   closed   1: closed loop (*n segments, wrap-around neighbours), else *n - 1 segments
//   list     the loop's node list (list(k) node of position k), used when *use_list != 0
//   use_list 0: positions are node numbers
//   p11      passed by the caller (2), never read (no reference to [RSP+0x120] in the listing)
void fun_140434952(const double* mxy, const double* p, double* foot, double* dist,
                   std::int32_t* iseg, const std::int32_t* n, std::int32_t* orient,
                   const std::int32_t* closed, const std::int32_t* list,
                   const std::int32_t* use_list, const std::int32_t* p11);

// @data 140703660  SAVE locals of 14034fc56 (meshed master driver)
struct Save14034fc56 {
    std::int32_t i;        // 140703660 slave position 1..*n
    std::int32_t i_mod20;  // 140703664 i % 20
    std::int32_t u668;     // 140703668 set to 0 when i % 20 == 0, never read here
    std::int32_t pad66c;
    double dbest;          // 140703670 signed best distance (carried: read after the loop loop)
    std::int32_t node;     // 140703678 slave node number nodes(i)
    std::int32_t pad67c;
    double dist;           // 140703680 |d| bound / result of 140434952 (kept over the loops)
    std::int32_t j;        // 140703688 master loop 1..*nloops
    std::int32_t first;    // 14070368c loops(1, j)
    std::int32_t last;     // 140703690 loops(2, j)
    std::int32_t count;    // 140703694 last - first
    std::int32_t n1;       // 140703698 segment start node
    std::int32_t n2;       // 14070369c segment end node
    double foot[2];        // 1407036a0 foot point from 140434952
    std::int32_t iseg;     // 1407036b0 segment position from 140434952 (carried)
    std::int32_t orient;   // 1407036b4 orientation from 140434952 (carried)
    std::int32_t bseg;     // 1407036b8 best segment position (carried, read after the loops)
    std::int32_t pad6bc;
    double len;            // 1407036c0 segment length (line kernel)
    double d;              // 1407036c8 distance of the measuring call
    double s;              // 1407036d0 position along the segment
    double s2;             // 1407036d8 copy of s
};
static_assert(offsetof(Save14034fc56, dbest) == 0x10);
static_assert(offsetof(Save14034fc56, dist) == 0x20);
static_assert(offsetof(Save14034fc56, foot) == 0x40);
static_assert(offsetof(Save14034fc56, bseg) == 0x58);
static_assert(offsetof(Save14034fc56, s2) == 0x78);
inline Save14034fc56 g_save_14034fc56{};

// @data 14070d670  SAVE locals of 140434952 (nearest node / segment); ends where the SAVE of
// 14043556b (14070d788) begins
struct Save140434952 {
    std::int32_t nseg;     // 14070d670 segments searched (*n or *n - 1)
    std::int32_t knear;    // 14070d674 position of the nearest node (0: none)
    double dnear;          // 14070d678 nearest node distance
    std::int32_t k;        // 14070d680 loop counter
    std::int32_t node;     // 14070d684 node of position k
    double dk;             // 14070d688 distance to that node
    double q[2];           // 14070d690 work copy of p (moved by the line kernel)
    std::int32_t ka;       // 14070d6a0 segment start position
    std::int32_t kb;       // 14070d6a4 segment end position
    std::int32_t na;       // 14070d6a8 segment start node
    std::int32_t nb;       // 14070d6ac segment end node
    double len;            // 14070d6b0 segment length
    double d;              // 14070d6b8 signed distance
    double s;              // 14070d6c0 position along the segment
    double dbest;          // 14070d6c8 signed distance of the accepted segment (carried)
    double sbest;          // 14070d6d0 its position along the segment
    std::int32_t kprev;    // 14070d6d8 position before the corner
    std::int32_t knext;    // 14070d6dc position after the corner
    std::int32_t nprev;    // 14070d6e0 node before the corner
    std::int32_t ncorner;  // 14070d6e4 corner node
    std::int32_t nnext;    // 14070d6e8 node after the corner
    std::int32_t pad6ec;
    double b[2];           // 14070d6f0 next - corner (rotated vector, then unit)
    double ang;            // 14070d700 rotation angle passed to 14043556b
    double pad708;         // 14070d708 not referenced
    double prev[2];        // 14070d710 point before the corner
    double next[2];        // 14070d720 point after the corner
    double len1;           // 14070d730 kernel outputs prev-corner (mode 1)
    double d1;             // 14070d738
    double s1;             // 14070d740
    double len2;           // 14070d748 kernel outputs corner-next (mode 1)
    double d2;             // 14070d750
    double s2;             // 14070d758
    double a[2];           // 14070d760 prev - corner, then unit
    double la;             // 14070d770 |a|
    double lb;             // 14070d778 |b|
    double cross;          // 14070d780 b x a of the unit vectors
};
static_assert(offsetof(Save140434952, dnear) == 0x08);
static_assert(offsetof(Save140434952, q) == 0x20);
static_assert(offsetof(Save140434952, dbest) == 0x58);
static_assert(offsetof(Save140434952, b) == 0x80);
static_assert(offsetof(Save140434952, prev) == 0xa0);
static_assert(offsetof(Save140434952, a) == 0xf0);
static_assert(sizeof(Save140434952) == 0x118);
inline Save140434952 g_save_140434952{};

void reset_ctc2d_meshed_save();

} // namespace def::cmd::ftn
