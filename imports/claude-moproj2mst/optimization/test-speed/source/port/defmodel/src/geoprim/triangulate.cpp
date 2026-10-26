// Polygon triangulator of DEF_PRE_64 (packet p9-05): 0x14041f830 and 0x14045a270..0x140460e80.
// Spec: docs/re/commands/GEOPRIM_REVOLVE.md §5. Ported from the decompile; FP expressions and
// comparisons checked against the listing (out/p905/all_l.txt): the code is unoptimised, every
// expression is evaluated left to right as written below.
//
// Records (sizes and offsets of the binary's arrays):
//   segment  0x38: v0 +0, v1 +0x10, is_inserted +0x20, root0 +0x24, root1 +0x28, next +0x2c, prev +0x30
//   qnode    0x28: nodetype +0, segnum +4, yval +8, trnum +0x18, parent +0x1c, left +0x20, right +0x24
//   trap     0x48: lseg +0, rseg +4, hi +8, lo +0x18, u0 +0x28, u1 +0x2c, d0 +0x30, d1 +0x34,
//                  sink +0x38, usave +0x3c, uside +0x40, state +0x44
//   monchain 0x10: vnum +0, next +4, prev +8, marked +0xc
//   vertexchain 0x38: pt +0, vnext[4] +0x10, vpos[4] +0x20, nextfree +0x30
// ctest-labels is on the test, not here.
#include <defmodel/geoprim/triangulate.hpp>

#include <defrt/clock.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::model::geoprim {

namespace {

struct Pt { double x, y; };

struct Segment {            // 0x38
    Pt v0, v1;
    std::int32_t is_inserted, root0, root1, next, prev;
    std::int32_t pad;
};
struct QNode {              // 0x28
    std::int32_t nodetype, segnum;
    Pt yval;
    std::int32_t trnum, parent, left, right;
};
struct Trap {               // 0x48
    std::int32_t lseg, rseg;
    Pt hi, lo;
    std::int32_t u0, u1, d0, d1, sink, usave, uside, state;
};
struct MonChain { std::int32_t vnum, next, prev, marked; };   // 0x10
struct VertexChain {        // 0x38
    Pt pt;
    std::int32_t vnext[4], vpos[4], nextfree;
    std::int32_t pad;
};
static_assert(sizeof(Segment) == 0x38 && sizeof(QNode) == 0x28 && sizeof(Trap) == 0x48);
static_assert(sizeof(MonChain) == 0x10 && sizeof(VertexChain) == 0x38);

constexpr int T_X = 1, T_Y = 2, T_SINK = 3;
constexpr int ST_VALID = 1, ST_INVALID = 2;
constexpr int FIRSTPT = 1, LASTPT = 2;
constexpr int S_LEFT = 1, S_RIGHT = 2;
constexpr int TRI_LHS = 1, TRI_RHS = 2;   // triangulate_single_polygon side
constexpr int FROM_UP = 1, FROM_DN = 2;   // traverse_polygon direction

constexpr double C_EPS = 1e-12;             // DAT_14051f260
constexpr double INFINITY_P = 1073741824.0; // DAT_14056af88 (2^30)
constexpr double INFINITY_N = -1073741824.0;// DAT_14056af80
constexpr double ANGLE_MIN = -4.0;          // DAT_14056af78

// Globals of the binary.
// @data 14077abcc  total number of points
int g_n = 0;
// @data 14077abd8  segment table
Segment* g_seg = nullptr;
// @data 14077abe4  query-table size (8 n)
int g_qsize = 0;
// @data 14077abe8  query table
QNode* g_qs = nullptr;
// @data 14077abe0  trapezoid-table size (4 n)
int g_trsize = 0;
// @data 14077abd0  trapezoid table
Trap* g_tr = nullptr;
// @data 140722770  random permutation
int* g_permute = nullptr;
// @data 140722778  next index into the permutation
int g_choose_idx = 0;
// @data 140722760  monotone chain elements
MonChain* g_mchain = nullptr;
// @data 140722740  vertex chain
VertexChain* g_vert = nullptr;
// @data 140722750  monotone polygon starts
int* g_mon = nullptr;
// @data 140722768  trapezoid visited flags
int* g_visited = nullptr;
// @data 140722748  monotone polygon count
int g_mon_idx = 0;
// @data 140722758  chain element count
int g_chain_idx = 0;
// @data 14072274c  triangles written
int g_op_idx = 0;
// @data 14072277c  next trapezoid
int g_tr_idx = 0;
// @data 140722780  next query node
int g_q_idx = 0;

std::uint32_t g_holdrand = 1;   // _getptd()->_holdrand (+0x1c), CRT

FILE* err() { return stderr; }  // 14047903c() + 0x60: stderr of the static CRT

double fabs_(double x) { return std::fabs(x); }   // 140484558

// ---------------------------------------------------------------------------------------------
// @src 14045c7e0  _greater_than(v0, v1): v0 above v1 (y, then x)
int greater_than(const Pt* v0, const Pt* v1)
{
    if (v0->y <= v1->y + C_EPS) {
        if (v1->y - C_EPS <= v0->y)                 // listing: v1.y - eps < v0.y || ==
            return (v1->x <= v0->x && v0->x != v1->x) ? 1 : 0;
        return 0;
    }
    return 1;
}

// @src 14045c870  _equal_to
int equal_to(const Pt* v0, const Pt* v1)
{
    if (fabs_(v0->y - v1->y) <= C_EPS && fabs_(v0->x - v1->x) <= C_EPS)
        return 1;
    return 0;
}

// @src 14045c900  _greater_than_equal_to
int greater_than_equal_to(const Pt* v0, const Pt* v1)
{
    if (v0->y <= v1->y + C_EPS) {
        if (v1->y - C_EPS <= v0->y)
            return (v1->x <= v0->x) ? 1 : 0;
        return 0;
    }
    return 1;
}

// @src 14045c990  _less_than
int less_than(const Pt* v0, const Pt* v1)
{
    if (v1->y - C_EPS <= v0->y) {
        if (v0->y <= v1->y + C_EPS)
            return (v0->x <= v1->x && v1->x != v0->x) ? 1 : 0;
        return 0;
    }
    return 1;
}

// @src 14045d950  _max: the higher of two points
void max_pt(Pt* yval, const Pt* v0, const Pt* v1)
{
    if (v0->y <= v1->y + C_EPS) {
        if (C_EPS < fabs_(v0->y - v1->y)) *yval = *v1;
        else if (v0->x <= v1->x + C_EPS) *yval = *v1;
        else *yval = *v0;
    } else
        *yval = *v0;
}

// @src 14045daa0  _min: the lower of two points
void min_pt(Pt* yval, const Pt* v0, const Pt* v1)
{
    if (v1->y - C_EPS <= v0->y) {
        if (C_EPS < fabs_(v0->y - v1->y)) *yval = *v1;
        else if (v1->x <= v0->x) *yval = *v1;
        else *yval = *v0;
    } else
        *yval = *v0;
}

// @src 14045c6b0  math_logstar_n
int math_logstar_n(int n)
{
    int i = 0;
    double v = (double)n;
    while (1.0 <= v) {
        v = std::log(v);                 // 140bb3c40 = CRT log
        double d = std::log(2.0);
        ++i;
        v = v / d;
    }
    return i - 1;
}

// @src 14045c740  math_N
int math_N(int n, int h)
{
    double v = (double)n;
    for (int i = 0; i < h; ++i) {
        v = std::log(v);
        double d = std::log(2.0);
        v = v / d;
    }
    return (int)std::ceil((1.0 * (double)n) / v);     // 140486e8c = ceil
}

// @src 14045d840  newnode
int newnode()
{
    if (g_q_idx < g_qsize) return g_q_idx++;
    std::fprintf(err(), "newnode: Query-table overflow\n");   // str 1406397e0
    return -1;
}

// @src 14045d8a0  newtrap
int newtrap()
{
    if (g_tr_idx < g_trsize) {
        g_tr[g_tr_idx].lseg = -1;
        g_tr[g_tr_idx].rseg = -1;
        g_tr[g_tr_idx].state = ST_VALID;
        return g_tr_idx++;
    }
    std::fprintf(err(), "newtrap: Trapezoid-table overflow\n");   // str 140639800
    return -1;
}

// @src 14045c670  choose_segment
int choose_segment() { return g_permute[g_choose_idx++]; }

// @src 14045c650  time(NULL) wrapper (returns the value)
std::int64_t time_now() { return def::rt::now(); }

// @src 14045c530  generate_random_ordering
int generate_random_ordering(int n)
{
    int* st = static_cast<int*>(std::calloc((size_t)(g_n + 1), 4));
    g_choose_idx = 1;
    tri_detail::crt_srand((std::uint32_t)time_now());     // 1404801c4 = srand
    for (int i = 0; i <= n; ++i) st[i] = i;
    int* p = st;
    for (int i = 1; i <= n; ++i) {
        int m = tri_detail::crt_rand() % ((n + 1) - i) + 1;
        g_permute[i] = p[m];
        if (m != 1) p[m] = p[1];
        ++p;
    }
    std::free(st);
    return 0;
}

// @src 14041fc50  initialise: clear is_inserted, make the ordering
int initialise(int n)
{
    for (int i = 1; i <= n; ++i) g_seg[i].is_inserted = 0;
    generate_random_ordering(n);
    return 0;
}

// @src 140460920  inserted(segnum, whichpt)
int inserted(int segnum, int whichpt)
{
    if (whichpt == FIRSTPT) return g_seg[g_seg[segnum].prev].is_inserted;
    return g_seg[g_seg[segnum].next].is_inserted;
}

// @src 14045cce0  is_left_of(segnum, v)
int is_left_of(int segnum, const Pt* v)
{
    const Segment* s = &g_seg[segnum];
    double area;
    if (greater_than(&s->v1, &s->v0)) {          // segment going upwards
        if (fabs_(s->v1.y - v->y) <= C_EPS)
            area = (s->v1.x > v->x) ? 1.0 : -1.0;
        else if (fabs_(s->v0.y - v->y) <= C_EPS)
            area = (s->v0.x > v->x) ? 1.0 : -1.0;
        else
            area = (s->v1.x - s->v0.x) * (v->y - s->v0.y) - (s->v1.y - s->v0.y) * (v->x - s->v0.x);
    } else {                                     // v0 > v1
        if (fabs_(s->v1.y - v->y) <= C_EPS)
            area = (s->v1.x > v->x) ? 1.0 : -1.0;
        else if (fabs_(s->v0.y - v->y) <= C_EPS)
            area = (s->v0.x > v->x) ? 1.0 : -1.0;
        else
            area = (s->v0.x - s->v1.x) * (v->y - s->v1.y) - (s->v0.y - s->v1.y) * (v->x - s->v1.x);
    }
    return area > 0.0 ? 1 : 0;
}

// @src 14045ca20  locate_endpoint(v, vo, r)
int locate_endpoint(const Pt* v, const Pt* vo, int r)
{
    QNode* rptr = &g_qs[r];
    switch (rptr->nodetype) {
    case T_SINK:
        return rptr->trnum;
    case T_Y:
        if (greater_than(v, &rptr->yval)) return locate_endpoint(v, vo, rptr->right);
        if (equal_to(v, &rptr->yval)) {
            if (greater_than(vo, &rptr->yval)) return locate_endpoint(v, vo, rptr->right);
            return locate_endpoint(v, vo, rptr->left);
        }
        return locate_endpoint(v, vo, rptr->left);
    case T_X:
        if (equal_to(v, &g_seg[rptr->segnum].v0) || equal_to(v, &g_seg[rptr->segnum].v1)) {
            if (fabs_(v->y - vo->y) <= C_EPS) {          // horizontal segment
                if (v->x <= vo->x) return locate_endpoint(v, vo, rptr->right);
                return locate_endpoint(v, vo, rptr->left);
            }
            if (is_left_of(rptr->segnum, vo)) return locate_endpoint(v, vo, rptr->left);
            return locate_endpoint(v, vo, rptr->right);
        }
        if (is_left_of(rptr->segnum, v)) return locate_endpoint(v, vo, rptr->left);
        return locate_endpoint(v, vo, rptr->right);
    default:
        std::fprintf(err(), "Haggu !!!!!\n");     // str 140639828
        return 0;
    }
}

// @src 140460990  merge_trapezoids(segnum, tfirst, tlast, side)
int merge_trapezoids(int segnum, int tfirst, int tlast, int side)
{
    int t = tfirst, tnext = 0, cond;
    while (t > 0 && greater_than_equal_to(&g_tr[t].lo, &g_tr[tlast].lo)) {
        if (side == S_LEFT)
            cond = (((tnext = g_tr[t].d0) > 0 && g_tr[tnext].rseg == segnum) ||
                    ((tnext = g_tr[t].d1) > 0 && g_tr[tnext].rseg == segnum)) ? 1 : 0;
        else
            cond = (((tnext = g_tr[t].d0) > 0 && g_tr[tnext].lseg == segnum) ||
                    ((tnext = g_tr[t].d1) > 0 && g_tr[tnext].lseg == segnum)) ? 1 : 0;
        if (cond && g_tr[t].lseg == g_tr[tnext].lseg && g_tr[t].rseg == g_tr[tnext].rseg) {
            int ptnext = g_qs[g_tr[tnext].sink].parent;
            if (g_qs[ptnext].left == g_tr[tnext].sink) g_qs[ptnext].left = g_tr[t].sink;
            else g_qs[ptnext].right = g_tr[t].sink;
            if ((g_tr[t].d0 = g_tr[tnext].d0) > 0) {
                if (g_tr[g_tr[t].d0].u0 == tnext) g_tr[g_tr[t].d0].u0 = t;
                else if (g_tr[g_tr[t].d0].u1 == tnext) g_tr[g_tr[t].d0].u1 = t;
            }
            if ((g_tr[t].d1 = g_tr[tnext].d1) > 0) {
                if (g_tr[g_tr[t].d1].u0 == tnext) g_tr[g_tr[t].d1].u0 = t;
                else if (g_tr[g_tr[t].d1].u1 == tnext) g_tr[g_tr[t].d1].u1 = t;
            }
            g_tr[t].lo = g_tr[tnext].lo;
            g_tr[tnext].state = ST_INVALID;
        } else
            t = tnext;
    }
    return 0;
}

// @src 14045d0f0  init_query_structure(segnum)
int init_query_structure(int segnum)
{
    Segment* s = &g_seg[segnum];
    g_tr_idx = 1;
    g_q_idx = 1;
    std::memset(g_tr, 0, 8);           // 140478d30(tr, 0, 8): only the first 8 bytes
    std::memset(g_qs, 0, 8);
    int i1 = newnode();
    g_qs[i1].nodetype = T_Y;
    max_pt(&g_qs[i1].yval, &s->v0, &s->v1);
    int root = i1;
    int i2 = newnode();
    g_qs[i1].right = i2;
    g_qs[i2].nodetype = T_SINK;
    g_qs[i2].parent = i1;
    int i3 = newnode();
    g_qs[i1].left = i3;
    g_qs[i3].nodetype = T_Y;
    min_pt(&g_qs[i3].yval, &s->v0, &s->v1);
    g_qs[i3].parent = i1;
    int i4 = newnode();
    g_qs[i3].left = i4;
    g_qs[i4].nodetype = T_SINK;
    g_qs[i4].parent = i3;
    int i5 = newnode();
    g_qs[i3].right = i5;
    g_qs[i5].nodetype = T_X;
    g_qs[i5].segnum = segnum;
    g_qs[i5].parent = i3;
    int i6 = newnode();
    g_qs[i5].left = i6;
    g_qs[i6].nodetype = T_SINK;
    g_qs[i6].parent = i5;
    int i7 = newnode();
    g_qs[i5].right = i7;
    g_qs[i7].nodetype = T_SINK;
    g_qs[i7].parent = i5;

    int t1 = newtrap();    // middle left
    int t2 = newtrap();    // middle right
    int t3 = newtrap();    // bottom-most
    int t4 = newtrap();    // topmost

    g_tr[t4].lo = g_qs[i1].yval;
    g_tr[t2].hi = g_tr[t4].lo;
    g_tr[t1].hi = g_tr[t2].hi;
    g_tr[t3].hi = g_qs[i3].yval;
    g_tr[t2].lo = g_tr[t3].hi;
    g_tr[t1].lo = g_tr[t2].lo;
    g_tr[t4].hi.y = INFINITY_P;
    g_tr[t4].hi.x = INFINITY_P;
    g_tr[t3].lo.y = INFINITY_N;
    g_tr[t3].lo.x = INFINITY_N;
    g_tr[t2].lseg = segnum;
    g_tr[t1].rseg = segnum;
    g_tr[t2].u0 = t4;
    g_tr[t1].u0 = t4;
    g_tr[t2].d0 = t3;
    g_tr[t1].d0 = t3;
    g_tr[t3].u0 = t1;
    g_tr[t4].d0 = t1;
    g_tr[t3].u1 = t2;
    g_tr[t4].d1 = t2;
    g_tr[t1].sink = i6;
    g_tr[t2].sink = i7;
    g_tr[t3].sink = i4;
    g_tr[t4].sink = i2;
    g_tr[t2].state = ST_VALID;
    g_tr[t1].state = ST_VALID;
    g_tr[t4].state = ST_VALID;
    g_tr[t3].state = ST_VALID;
    g_qs[i2].trnum = t4;
    g_qs[i4].trnum = t3;
    g_qs[i6].trnum = t1;
    g_qs[i7].trnum = t2;
    s->is_inserted = 1;
    return root;
}

// The "continuation with one upper neighbour / two upper neighbours" block that is the same in
// the three branches of the add_segment loop. part of @src 14045dbe0
void fix_upper(int t, int tn, const Pt* sv1)
{
    if (g_tr[t].u0 > 0 && g_tr[t].u1 > 0) {          // continuation of a chain from above
        if (g_tr[t].usave > 0) {                     // three upper neighbours
            if (g_tr[t].uside == S_LEFT) {
                g_tr[tn].u0 = g_tr[t].u1;
                g_tr[t].u1 = -1;
                g_tr[tn].u1 = g_tr[t].usave;
                g_tr[g_tr[t].u0].d0 = t;
                g_tr[g_tr[tn].u0].d0 = tn;
                g_tr[g_tr[tn].u1].d0 = tn;
            } else {                                 // intersects in the right
                g_tr[tn].u1 = -1;
                g_tr[tn].u0 = g_tr[t].u1;
                g_tr[t].u1 = g_tr[t].u0;
                g_tr[t].u0 = g_tr[t].usave;
                g_tr[g_tr[t].u0].d0 = t;
                g_tr[g_tr[t].u1].d0 = t;
                g_tr[g_tr[tn].u0].d0 = tn;
            }
            g_tr[tn].usave = 0;
            g_tr[t].usave = 0;
        } else {                                     // no usave: simple case
            g_tr[tn].u0 = g_tr[t].u1;
            g_tr[tn].u1 = -1;
            g_tr[t].u1 = -1;
            g_tr[g_tr[tn].u0].d0 = tn;
        }
    } else {                                         // fresh segment or upward cusp
        int tmp_u = g_tr[t].u0;
        int td0, td1;
        if ((td0 = g_tr[tmp_u].d0) > 0 && (td1 = g_tr[tmp_u].d1) > 0) {   // upward cusp
            if (g_tr[td0].rseg <= 0 || is_left_of(g_tr[td0].rseg, sv1)) {   // cusp going leftwards
                g_tr[t].u1 = -1;
                g_tr[tn].u1 = -1;
                g_tr[tn].u0 = -1;
                g_tr[g_tr[t].u0].d0 = t;
            } else {
                g_tr[tn].u1 = -1;
                g_tr[t].u1 = -1;
                g_tr[t].u0 = -1;
                g_tr[g_tr[tn].u0].d1 = tn;
            }
        } else {                                     // fresh segment
            g_tr[g_tr[t].u0].d0 = t;
            g_tr[g_tr[t].u0].d1 = tn;
        }
    }
}

// @src 14045dbe0  add_segment(segnum)
int add_segment(int segnum)
{
    Segment s = g_seg[segnum];
    int tfirst, tlast, tfirstr = 0, tlastr = 0;
    int tribot = 0, is_swapped = 0;

    if (greater_than(&s.v1, &s.v0)) {               // get higher vertex in v0
        Pt tpt = s.v0;
        s.v0 = s.v1;
        s.v1 = tpt;
        int tmp = s.root0;
        s.root0 = s.root1;
        s.root1 = tmp;
        is_swapped = 1;
    }

    if (is_swapped ? !inserted(segnum, LASTPT) : !inserted(segnum, FIRSTPT)) {
        // insert v0 in the tree
        int tu = locate_endpoint(&s.v0, &s.v1, s.root0);
        int tl = newtrap();
        g_tr[tl].state = ST_VALID;
        g_tr[tl] = g_tr[tu];
        g_tr[tl].hi.y = s.v0.y;
        g_tr[tu].lo.y = s.v0.y;
        g_tr[tl].hi.x = s.v0.x;
        g_tr[tu].lo.x = s.v0.x;
        g_tr[tu].d0 = tl;
        g_tr[tu].d1 = 0;
        g_tr[tl].u0 = tu;
        g_tr[tl].u1 = 0;
        int tmp_d;
        if ((tmp_d = g_tr[tl].d0) > 0 && g_tr[tmp_d].u0 == tu) g_tr[tmp_d].u0 = tl;
        if ((tmp_d = g_tr[tl].d0) > 0 && g_tr[tmp_d].u1 == tu) g_tr[tmp_d].u1 = tl;
        if ((tmp_d = g_tr[tl].d1) > 0 && g_tr[tmp_d].u0 == tu) g_tr[tmp_d].u0 = tl;
        if ((tmp_d = g_tr[tl].d1) > 0 && g_tr[tmp_d].u1 == tu) g_tr[tmp_d].u1 = tl;
        int i1 = newnode();
        int i2 = newnode();
        int sk = g_tr[tu].sink;
        g_qs[sk].nodetype = T_Y;
        g_qs[sk].yval = s.v0;
        g_qs[sk].segnum = segnum;
        g_qs[sk].left = i2;
        g_qs[sk].right = i1;
        g_qs[i1].nodetype = T_SINK;
        g_qs[i1].trnum = tu;
        g_qs[i1].parent = sk;
        g_qs[i2].nodetype = T_SINK;
        g_qs[i2].trnum = tl;
        g_qs[i2].parent = sk;
        g_tr[tu].sink = i1;
        g_tr[tl].sink = i2;
        tfirst = tl;
    } else {                                        // v0 already present
        tfirst = locate_endpoint(&s.v0, &s.v1, s.root0);
    }

    if (is_swapped ? !inserted(segnum, FIRSTPT) : !inserted(segnum, LASTPT)) {
        // insert v1 in the tree
        int tu = locate_endpoint(&s.v1, &s.v0, s.root1);
        int tl = newtrap();
        g_tr[tl].state = ST_VALID;
        g_tr[tl] = g_tr[tu];
        g_tr[tl].hi.y = s.v1.y;
        g_tr[tu].lo.y = s.v1.y;
        g_tr[tl].hi.x = s.v1.x;
        g_tr[tu].lo.x = s.v1.x;
        g_tr[tu].d0 = tl;
        g_tr[tu].d1 = 0;
        g_tr[tl].u0 = tu;
        g_tr[tl].u1 = 0;
        int tmp_d;
        if ((tmp_d = g_tr[tl].d0) > 0 && g_tr[tmp_d].u0 == tu) g_tr[tmp_d].u0 = tl;
        if ((tmp_d = g_tr[tl].d0) > 0 && g_tr[tmp_d].u1 == tu) g_tr[tmp_d].u1 = tl;
        if ((tmp_d = g_tr[tl].d1) > 0 && g_tr[tmp_d].u0 == tu) g_tr[tmp_d].u0 = tl;
        if ((tmp_d = g_tr[tl].d1) > 0 && g_tr[tmp_d].u1 == tu) g_tr[tmp_d].u1 = tl;
        int i1 = newnode();
        int i2 = newnode();
        int sk = g_tr[tu].sink;
        g_qs[sk].nodetype = T_Y;
        g_qs[sk].yval = s.v1;
        g_qs[sk].segnum = segnum;
        g_qs[sk].left = i2;
        g_qs[sk].right = i1;
        g_qs[i1].nodetype = T_SINK;
        g_qs[i1].trnum = tu;
        g_qs[i1].parent = sk;
        g_qs[i2].nodetype = T_SINK;
        g_qs[i2].trnum = tl;
        g_qs[i2].parent = sk;
        g_tr[tu].sink = i1;
        g_tr[tl].sink = i2;
        tlast = tu;
    } else {                                        // v1 already present
        tlast = locate_endpoint(&s.v1, &s.v0, s.root1);
        tribot = 1;
    }

    // Thread the segment into the query tree, splitting all trapezoids it crosses.
    int t = tfirst;
    while (t > 0 && greater_than_equal_to(&g_tr[t].lo, &g_tr[tlast].lo)) {
        int t_sav, tn_sav;
        int sk = g_tr[t].sink;
        int i1 = newnode();          // left trapezoid sink
        int i2 = newnode();          // right trapezoid sink
        g_qs[sk].nodetype = T_X;
        g_qs[sk].segnum = segnum;
        g_qs[sk].left = i1;
        g_qs[sk].right = i2;
        g_qs[i1].nodetype = T_SINK;
        g_qs[i1].trnum = t;
        g_qs[i1].parent = sk;
        g_qs[i2].nodetype = T_SINK;
        int tn = newtrap();
        g_qs[i2].trnum = tn;
        g_tr[tn].state = ST_VALID;
        g_qs[i2].parent = sk;

        if (t == tfirst) tfirstr = tn;
        if (equal_to(&g_tr[t].lo, &g_tr[tlast].lo)) tlastr = tn;

        g_tr[tn] = g_tr[t];
        g_tr[t].sink = i1;
        g_tr[tn].sink = i2;
        t_sav = t;
        tn_sav = tn;

        // Two lower neighbours: impossible.
        if (g_tr[t].d0 <= 0 && g_tr[t].d1 <= 0) {
            std::fprintf(err(), "add_segment: error\n");   // str 140639838
            break;
        }
        if (g_tr[t].d0 > 0 && g_tr[t].d1 <= 0) {          // only one trapezoid below
            fix_upper(t, tn, &s.v1);
            if (fabs_(g_tr[t].lo.y - g_tr[tlast].lo.y) <= C_EPS &&
                fabs_(g_tr[t].lo.x - g_tr[tlast].lo.x) <= C_EPS && tribot) {
                // bottom forms a triangle
                int tmptriseg = is_swapped ? g_seg[segnum].prev : g_seg[segnum].next;
                if (tmptriseg > 0 && is_left_of(tmptriseg, &s.v0)) {   // L-R downward cusp
                    g_tr[g_tr[t].d0].u0 = t;
                    g_tr[tn].d1 = -1;
                    g_tr[tn].d0 = -1;
                } else {                                               // R-L downward cusp
                    g_tr[g_tr[tn].d0].u1 = tn;
                    g_tr[t].d1 = -1;
                    g_tr[t].d0 = -1;
                }
            } else {
                if (g_tr[g_tr[t].d0].u0 > 0 && g_tr[g_tr[t].d0].u1 > 0) {
                    if (g_tr[g_tr[t].d0].u0 == t) {      // passes through the left
                        g_tr[g_tr[t].d0].usave = g_tr[g_tr[t].d0].u1;
                        g_tr[g_tr[t].d0].uside = S_LEFT;
                    } else {
                        g_tr[g_tr[t].d0].usave = g_tr[g_tr[t].d0].u0;
                        g_tr[g_tr[t].d0].uside = S_RIGHT;
                    }
                }
                g_tr[g_tr[t].d0].u0 = t;
                g_tr[g_tr[t].d0].u1 = tn;
            }
            t = g_tr[t].d0;
        } else if (g_tr[t].d0 <= 0 && g_tr[t].d1 > 0) {   // only one trapezoid below
            fix_upper(t, tn, &s.v1);
            if (fabs_(g_tr[t].lo.y - g_tr[tlast].lo.y) <= C_EPS &&
                fabs_(g_tr[t].lo.x - g_tr[tlast].lo.x) <= C_EPS && tribot) {
                int tmptriseg = is_swapped ? g_seg[segnum].prev : g_seg[segnum].next;
                if (tmptriseg > 0 && is_left_of(tmptriseg, &s.v0)) {
                    g_tr[g_tr[t].d1].u0 = t;
                    g_tr[tn].d1 = -1;
                    g_tr[tn].d0 = -1;
                } else {
                    g_tr[g_tr[tn].d1].u1 = tn;
                    g_tr[t].d1 = -1;
                    g_tr[t].d0 = -1;
                }
            } else {
                if (g_tr[g_tr[t].d1].u0 > 0 && g_tr[g_tr[t].d1].u1 > 0) {
                    if (g_tr[g_tr[t].d1].u0 == t) {
                        g_tr[g_tr[t].d1].usave = g_tr[g_tr[t].d1].u1;
                        g_tr[g_tr[t].d1].uside = S_LEFT;
                    } else {
                        g_tr[g_tr[t].d1].usave = g_tr[g_tr[t].d1].u0;
                        g_tr[g_tr[t].d1].uside = S_RIGHT;
                    }
                }
                g_tr[g_tr[t].d1].u0 = t;
                g_tr[g_tr[t].d1].u1 = tn;
            }
            t = g_tr[t].d1;
        } else {                                          // two trapezoids below
            int tnext;
            int i_d0 = 0, i_d1 = 0;
            double y0 = g_tr[t].lo.y;
            if (C_EPS < fabs_(y0 - s.v0.y)) {
                double yt = (y0 - s.v0.y) / (s.v1.y - s.v0.y);
                Pt tmppt;
                tmppt.x = s.v0.x + yt * (s.v1.x - s.v0.x);
                tmppt.y = y0;
                if (less_than(&tmppt, &g_tr[t].lo)) i_d0 = 1;
                else i_d1 = 1;
            } else {
                if (g_tr[t].lo.x <= s.v0.x) i_d1 = 1;   // listing: lo.x <= v0.x
                else i_d0 = 1;
            }
            fix_upper(t, tn, &s.v1);
            if (fabs_(g_tr[t].lo.y - g_tr[tlast].lo.y) <= C_EPS &&
                fabs_(g_tr[t].lo.x - g_tr[tlast].lo.x) <= C_EPS && tribot) {
                // only the lower trapezoids need to be fixed
                g_tr[g_tr[t].d0].u0 = t;
                g_tr[g_tr[t].d0].u1 = -1;
                g_tr[g_tr[t].d1].u0 = tn;
                g_tr[g_tr[t].d1].u1 = -1;
                g_tr[tn].d0 = g_tr[t].d1;
                g_tr[tn].d1 = -1;
                g_tr[t].d1 = -1;
                tnext = g_tr[t].d1;
            } else if (i_d0) {                            // intersecting d0
                g_tr[g_tr[t].d0].u0 = t;
                g_tr[g_tr[t].d0].u1 = tn;
                g_tr[g_tr[t].d1].u0 = tn;
                g_tr[g_tr[t].d1].u1 = -1;
                g_tr[t].d1 = -1;
                tnext = g_tr[t].d0;
            } else {                                      // intersecting d1
                g_tr[g_tr[t].d0].u0 = t;
                g_tr[g_tr[t].d0].u1 = -1;
                g_tr[g_tr[t].d1].u0 = t;
                g_tr[g_tr[t].d1].u1 = tn;
                g_tr[tn].d0 = g_tr[t].d1;
                g_tr[tn].d1 = -1;
                tnext = g_tr[t].d1;
            }
            t = tnext;
        }
        g_tr[tn_sav].lseg = segnum;
        g_tr[t_sav].rseg = segnum;
    }

    merge_trapezoids(segnum, tfirst, tlast, S_LEFT);
    merge_trapezoids(segnum, tfirstr, tlastr, S_RIGHT);
    g_seg[segnum].is_inserted = 1;
    return 0;
}

// @src 140460e80  find_new_roots(segnum)
int find_new_roots(int segnum)
{
    Segment* s = &g_seg[segnum];
    if (s->is_inserted) return 0;
    s->root0 = locate_endpoint(&s->v0, &s->v1, s->root0);
    s->root0 = g_tr[s->root0].sink;
    s->root1 = locate_endpoint(&s->v1, &s->v0, s->root1);
    s->root1 = g_tr[s->root1].sink;
    return 0;
}

// @src 14045cfa0  construct_trapezoids(nseg)
int construct_trapezoids(int nseg)
{
    int root = init_query_structure(choose_segment());
    for (int i = 1; i <= nseg; ++i) {
        g_seg[i].root1 = root;
        g_seg[i].root0 = root;
    }
    for (int h = 1; h <= math_logstar_n(nseg); ++h) {
        for (int i = math_N(nseg, h - 1) + 1; i <= math_N(nseg, h); ++i)
            add_segment(choose_segment());
        for (int i = 1; i <= nseg; ++i) find_new_roots(i);
    }
    for (int i = math_N(nseg, math_logstar_n(nseg)) + 1; i <= nseg; ++i)
        add_segment(choose_segment());
    return 0;
}

// ---------------------------------------------------------------------------------------------
// Monotone decomposition.

// @src 14045a270  allocate the monotonation arrays
void alloc_monotone()
{
    g_mchain = static_cast<MonChain*>(std::calloc((size_t)(g_trsize + 1), sizeof(MonChain)));
    g_vert = static_cast<VertexChain*>(std::calloc((size_t)(g_n + 1), sizeof(VertexChain)));
    g_mon = static_cast<int*>(std::calloc((size_t)(g_n + 1), 4));
    g_visited = static_cast<int*>(std::calloc((size_t)(g_trsize + 1), 4));
}

// @src 14045a300  free them
void free_monotone()
{
    std::free(g_mchain);
    std::free(g_vert);
    std::free(g_mon);
    std::free(g_visited);
}

// @src 14045c4e0  allocate the permutation
void alloc_permute() { g_permute = static_cast<int*>(std::calloc((size_t)(g_n + 1), 4)); }
// @src 14045c510  free it
void free_permute() { std::free(g_permute); }

// @src 14045a5c0  inside_polygon(t)
int inside_polygon(const Trap* t)
{
    int rseg = t->rseg;
    if (t->state == ST_INVALID) return 0;
    if (t->lseg <= 0 || t->rseg <= 0) return 0;
    if ((t->u0 <= 0 && t->u1 <= 0) || (t->d0 <= 0 && t->d1 <= 0))   // triangle
        return greater_than(&g_seg[rseg].v1, &g_seg[rseg].v0);
    return 0;
}

// @src 14045a670  newmon
int newmon() { return ++g_mon_idx; }

// @src 14045b900  new_chain_element
int new_chain_element() { return ++g_chain_idx; }

// @src 14045bab0  get_angle(vp0, vpnext, vp1)
double get_angle(const Pt* vp0, const Pt* vpnext, const Pt* vp1)
{
    double v0x = vpnext->x - vp0->x;
    double v0y = vpnext->y - vp0->y;
    double v1x = vp1->x - vp0->x;
    double v1y = vp1->y - vp0->y;
    if (v0x * v1y - v1x * v0y >= 0.0)     // sine positive (listing: COMISD; JC -> else)
        return ((v0x * v1x + v0y * v1y) / std::sqrt(v0x * v0x + v0y * v0y)) /
               std::sqrt(v1x * v1x + v1y * v1y);
    double d = -1.0 * (v0x * v1x + v0y * v1y);
    double a = std::sqrt(v0x * v0x + v0y * v0y);
    d = d / a;
    double b = std::sqrt(v1x * v1x + v1y * v1y);
    return d / b - 2.0;
}

// @src 14045b920  get_vertex_positions(v0, v1, &ip, &iq)
int get_vertex_positions(int v0, int v1, int* ip, int* iq)
{
    VertexChain* vp0 = &g_vert[v0];
    VertexChain* vp1 = &g_vert[v1];
    double angle = ANGLE_MIN, temp;
    int tp = 0, tq = 0;   // the binary leaves them uninitialised if no neighbour qualifies
    for (int i = 0; i < 4; ++i) {
        if (vp0->vnext[i] <= 0) continue;
        if ((temp = get_angle(&vp0->pt, &g_vert[vp0->vnext[i]].pt, &vp1->pt)) > angle) {
            angle = temp;
            tp = i;
        }
    }
    *ip = tp;
    angle = ANGLE_MIN;
    for (int i = 0; i < 4; ++i) {
        if (vp1->vnext[i] <= 0) continue;
        if ((temp = get_angle(&vp1->pt, &g_vert[vp1->vnext[i]].pt, &vp0->pt)) > angle) {
            angle = temp;
            tq = i;
        }
    }
    *iq = tq;
    return 0;
}

// @src 14045b630  make_new_monotone_poly(mcur, v0, v1)
int make_new_monotone_poly(int mcur, int v0, int v1)
{
    int ip, iq;
    int mnew = newmon();
    VertexChain* vp0 = &g_vert[v0];
    VertexChain* vp1 = &g_vert[v1];
    get_vertex_positions(v0, v1, &ip, &iq);
    int p = vp0->vpos[ip];
    int q = vp1->vpos[iq];
    int i = new_chain_element();
    int j = new_chain_element();
    g_mchain[i].vnum = v0;
    g_mchain[j].vnum = v1;
    g_mchain[i].next = g_mchain[p].next;
    g_mchain[g_mchain[p].next].prev = i;
    g_mchain[i].prev = j;
    g_mchain[j].next = i;
    g_mchain[j].prev = g_mchain[q].prev;
    g_mchain[g_mchain[q].prev].next = j;
    g_mchain[p].next = q;
    g_mchain[q].prev = p;
    int nf0 = vp0->nextfree;
    int nf1 = vp1->nextfree;
    vp0->vnext[ip] = v1;
    vp0->vpos[nf0] = i;
    vp0->vnext[nf0] = g_mchain[g_mchain[i].next].vnum;
    vp1->vpos[nf1] = j;
    vp1->vnext[nf1] = v0;
    vp0->nextfree++;
    vp1->nextfree++;
    g_mon[mcur] = p;
    g_mon[mnew] = i;
    return mnew;
}

// @src 14045a690  traverse_polygon(mcur, trnum, from, dir)
// Transcribed from the decompile case by case (the order of the recursive calls matters: it
// numbers the monotone pieces and so the order of the output triangles).
int traverse_polygon(int mcur, int trnum, int from, int dir)
{
    if (trnum <= 0 || g_visited[trnum]) return 0;
    Trap* t = &g_tr[trnum];
    int mnew, retval = 0;
    g_visited[trnum] = 1;

    if (t->u0 <= 0 && t->u1 <= 0) {                     // no upper neighbour
        if (t->d0 <= 0 || t->d1 <= 0) {
            retval = -1;
            traverse_polygon(mcur, t->u0, trnum, FROM_DN);
            traverse_polygon(mcur, t->u1, trnum, FROM_DN);
            traverse_polygon(mcur, t->d0, trnum, FROM_UP);
            traverse_polygon(mcur, t->d1, trnum, FROM_UP);
        } else {                                        // downward opening triangle
            int v0 = g_tr[t->d1].lseg;
            if (from == t->d1) {
                mnew = make_new_monotone_poly(mcur, t->lseg, v0);
                traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                traverse_polygon(mnew, t->d0, trnum, FROM_UP);
            } else {
                mnew = make_new_monotone_poly(mcur, v0, t->lseg);
                traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                traverse_polygon(mnew, t->d1, trnum, FROM_UP);
            }
        }
    } else if (t->d0 <= 0 && t->d1 <= 0) {              // no lower neighbour
        if (t->u0 <= 0 || t->u1 <= 0) {
            retval = -1;
            traverse_polygon(mcur, t->u0, trnum, FROM_DN);
            traverse_polygon(mcur, t->u1, trnum, FROM_DN);
            traverse_polygon(mcur, t->d0, trnum, FROM_UP);
            traverse_polygon(mcur, t->d1, trnum, FROM_UP);
        } else {                                        // upward opening triangle
            int v1 = g_tr[t->u0].rseg;
            if (from == t->u1) {
                mnew = make_new_monotone_poly(mcur, v1, t->rseg);
                traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                traverse_polygon(mnew, t->u0, trnum, FROM_DN);
            } else {
                mnew = make_new_monotone_poly(mcur, t->rseg, v1);
                traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                traverse_polygon(mnew, t->u1, trnum, FROM_DN);
            }
        }
    } else if (t->u0 <= 0 || t->u1 <= 0) {              // one upper neighbour
        if (t->d0 <= 0 || t->d1 <= 0) {                 // one lower neighbour
            if (equal_to(&t->hi, &g_seg[t->lseg].v0) && equal_to(&t->lo, &g_seg[t->rseg].v0)) {
                retval = 2;
                if (dir == FROM_UP) {
                    mnew = make_new_monotone_poly(mcur, t->lseg, t->rseg);
                    traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                    traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d1, trnum, FROM_UP);
                    traverse_polygon(mnew, t->d0, trnum, FROM_UP);
                } else {
                    mnew = make_new_monotone_poly(mcur, t->rseg, t->lseg);
                    traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                    traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                    traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                }
            } else if (equal_to(&t->hi, &g_seg[t->rseg].v1) &&
                       equal_to(&t->lo, &g_seg[t->lseg].v1)) {
                int rn = g_seg[t->rseg].next;
                int ln = g_seg[t->lseg].next;
                retval = 1;
                if (dir == FROM_UP) {
                    mnew = make_new_monotone_poly(mcur, ln, rn);
                    traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                    traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d1, trnum, FROM_UP);
                    traverse_polygon(mnew, t->d0, trnum, FROM_UP);
                } else {
                    mnew = make_new_monotone_poly(mcur, rn, ln);
                    traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                    traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                    traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                }
            } else {                                    // no split possible
                retval = -1;
                traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                traverse_polygon(mcur, t->d1, trnum, FROM_UP);
            }
        } else {                                        // two lower: upward cusp
            int v0 = g_tr[t->d1].lseg;
            if (!equal_to(&t->hi, &g_seg[t->lseg].v0)) {
                int rn = g_seg[t->rseg].next;
                retval = 7;
                if (dir == FROM_DN && t->d1 == from) {
                    mnew = make_new_monotone_poly(mcur, rn, v0);
                    traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d0, trnum, FROM_UP);
                } else {
                    mnew = make_new_monotone_poly(mcur, v0, rn);
                    traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                    traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                    traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d1, trnum, FROM_UP);
                }
            } else {
                retval = 6;
                if (dir == FROM_DN && t->d0 == from) {
                    mnew = make_new_monotone_poly(mcur, v0, t->lseg);
                    traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                    traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d1, trnum, FROM_UP);
                } else {
                    mnew = make_new_monotone_poly(mcur, t->lseg, v0);
                    traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                    traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                    traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d0, trnum, FROM_UP);
                }
            }
        }
    } else {                                            // two upper neighbours
        if (t->d0 <= 0 || t->d1 <= 0) {                 // one lower: downward cusp
            int v0 = g_tr[t->u0].rseg;
            if (!equal_to(&t->lo, &g_seg[t->lseg].v1)) {
                retval = 5;
                if (dir == FROM_UP && t->u1 == from) {
                    mnew = make_new_monotone_poly(mcur, v0, t->rseg);
                    traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d1, trnum, FROM_UP);
                    traverse_polygon(mnew, t->d0, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                } else {
                    mnew = make_new_monotone_poly(mcur, t->rseg, v0);
                    traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                    traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                    traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                }
            } else {
                int ln = g_seg[t->lseg].next;
                retval = 4;
                if (dir == FROM_UP && t->u0 == from) {
                    mnew = make_new_monotone_poly(mcur, ln, v0);
                    traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d0, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                    traverse_polygon(mnew, t->d1, trnum, FROM_UP);
                } else {
                    mnew = make_new_monotone_poly(mcur, v0, ln);
                    traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                    traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                    traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                    traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                }
            }
        } else {                                        // downward and upward cusps
            int v0 = g_tr[t->d1].lseg;
            int v1 = g_tr[t->u0].rseg;
            retval = 3;
            if ((dir == FROM_DN && t->d1 == from) || (dir == FROM_UP && t->u1 == from)) {
                mnew = make_new_monotone_poly(mcur, v1, v0);
                traverse_polygon(mcur, t->u1, trnum, FROM_DN);
                traverse_polygon(mcur, t->d1, trnum, FROM_UP);
                traverse_polygon(mnew, t->u0, trnum, FROM_DN);
                traverse_polygon(mnew, t->d0, trnum, FROM_UP);
            } else {
                mnew = make_new_monotone_poly(mcur, v0, v1);
                traverse_polygon(mcur, t->u0, trnum, FROM_DN);
                traverse_polygon(mcur, t->d0, trnum, FROM_UP);
                traverse_polygon(mnew, t->u1, trnum, FROM_DN);
                traverse_polygon(mnew, t->d1, trnum, FROM_UP);
            }
        }
    }
    return retval;
}

// @src 14045a340  monotonate_trapezoids(n)
int monotonate_trapezoids(int n)
{
    std::memset(g_vert, 0, 8);       // 140478d30(p, 0, 8): only the first 8 bytes
    std::memset(g_visited, 0, 8);
    std::memset(g_mchain, 0, 8);
    std::memset(g_mon, 0, 8);
    int tr_start = 0;
    while (tr_start < g_trsize && !inside_polygon(&g_tr[tr_start])) ++tr_start;
    for (int i = 1; i <= n; ++i) {
        g_mchain[i].prev = g_seg[i].prev;
        g_mchain[i].next = g_seg[i].next;
        g_mchain[i].vnum = i;
        g_vert[i].pt = g_seg[i].v0;
        g_vert[i].vnext[0] = g_seg[i].next;
        g_vert[i].vpos[0] = i;
        g_vert[i].nextfree = 1;
    }
    g_mon_idx = 0;
    g_chain_idx = n;
    g_mon[0] = 1;
    if (g_tr[tr_start].u0 > 0)
        traverse_polygon(0, tr_start, g_tr[tr_start].u0, FROM_UP);
    else if (g_tr[tr_start].d0 > 0)
        traverse_polygon(0, tr_start, g_tr[tr_start].d0, FROM_DN);
    return newmon();
}

// @src 14045c030  triangulate_single_polygon(nvert, posmax, side, op)
int triangulate_single_polygon(int nvert, int posmax, int side, int (*op)[3])
{
    int v, ri = 0, endv, tmp, vpos;
    int* rc = static_cast<int*>(std::calloc((size_t)g_n, 4));
    if (side == TRI_RHS) {
        rc[0] = g_mchain[posmax].vnum;
        tmp = g_mchain[posmax].next;
        rc[1] = g_mchain[tmp].vnum;
        ri = 1;
        vpos = g_mchain[tmp].next;
        v = g_mchain[vpos].vnum;
        if ((endv = g_mchain[g_mchain[posmax].prev].vnum) == 0) endv = nvert;
    } else {
        tmp = g_mchain[posmax].next;
        rc[0] = g_mchain[tmp].vnum;
        tmp = g_mchain[tmp].next;
        rc[1] = g_mchain[tmp].vnum;
        ri = 1;
        vpos = g_mchain[tmp].next;
        v = g_mchain[vpos].vnum;
        endv = g_mchain[posmax].vnum;
    }
    while ((v != endv || ri > 1) && ri < g_n) {
        if (ri > 0) {
            const Pt& a = g_vert[rc[ri - 1]].pt;
            const Pt& b = g_vert[rc[ri]].pt;
            const Pt& c = g_vert[v].pt;
            if ((a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x) <= 0.0) {
                if (g_n <= ri + 1) break;         // reflex chain full
                ++ri;
                rc[ri] = v;
                vpos = g_mchain[vpos].next;
                v = g_mchain[vpos].vnum;
            } else {                              // convex: cut a triangle
                op[g_op_idx][0] = rc[ri - 1];
                op[g_op_idx][1] = rc[ri];
                op[g_op_idx][2] = v;
                ++g_op_idx;
                --ri;
            }
        } else {                                  // reflex chain empty
            if (g_n <= ri + 1) break;
            ++ri;
            rc[ri] = v;
            vpos = g_mchain[vpos].next;
            v = g_mchain[vpos].vnum;
        }
    }
    op[g_op_idx][0] = rc[ri - 1];
    op[g_op_idx][1] = rc[ri];
    op[g_op_idx][2] = v;
    ++g_op_idx;
    std::free(rc);
    return 0;
}

// @src 14045bc60  triangulate_monotone_polygons(nvert, nmonpoly, op)
int triangulate_monotone_polygons(int nvert, int nmonpoly, int (*op)[3])
{
    g_op_idx = 0;
    for (int i = 0; i < nmonpoly; ++i) {
        int vcount = 1;
        int processed = 0;
        int vfirst = g_mchain[g_mon[i]].vnum;
        Pt ymax = g_vert[vfirst].pt;
        Pt ymin = ymax;
        int posmax = g_mon[i];
        [[maybe_unused]] int posmin = g_mon[i];
        g_mchain[g_mon[i]].marked = 1;
        int p = g_mchain[g_mon[i]].next;
        int v;
        while ((v = g_mchain[p].vnum) != vfirst) {
            if (g_mchain[p].marked) {
                processed = 1;
                break;
            }
            g_mchain[p].marked = 1;
            if (greater_than(&g_vert[v].pt, &ymax)) {
                ymax = g_vert[v].pt;
                posmax = p;
            }
            if (less_than(&g_vert[v].pt, &ymin)) {
                ymin = g_vert[v].pt;
                posmin = p;
            }
            p = g_mchain[p].next;
            ++vcount;
        }
        if (processed) continue;
        if (vcount == 3) {                     // already a triangle
            op[g_op_idx][0] = g_mchain[p].vnum;
            op[g_op_idx][1] = g_mchain[g_mchain[p].next].vnum;
            op[g_op_idx][2] = g_mchain[g_mchain[p].prev].vnum;
            ++g_op_idx;
        } else {
            v = g_mchain[g_mchain[posmax].next].vnum;
            if (equal_to(&g_vert[v].pt, &ymin))
                triangulate_single_polygon(nvert, posmax, TRI_LHS, op);
            else
                triangulate_single_polygon(nvert, posmax, TRI_RHS, op);
        }
    }
    return g_op_idx;
}

} // namespace

namespace tri_detail {
// 1404801c4 srand: _getptd()->_holdrand = seed
void crt_srand(std::uint32_t seed) { g_holdrand = seed; }
// 1404801dc rand: holdrand = holdrand * 214013 + 2531011; (holdrand >> 16) & 0x7fff
int crt_rand()
{
    g_holdrand = g_holdrand * 214013u + 2531011u;
    return (int)((g_holdrand >> 16) & 0x7fffu);
}
} // namespace tri_detail

// @src 14041f830  triangulate_polygon(ncontours, cntr, vertices, triangles)
int fun_14041f830(int ncontours, const int* cntr, const double* vertices, int* triangles)
{
    g_n = 0;
    for (int c = 0; c < ncontours; ++c) g_n += cntr[c];
    g_seg = static_cast<Segment*>(std::calloc((size_t)(g_n + 1), sizeof(Segment)));
    std::memset(g_seg, 0, (size_t)(g_n + 1));     // 140478d30(seg, 0, n+1): n+1 bytes
    g_qsize = g_n * 8;
    g_qs = static_cast<QNode*>(std::calloc((size_t)(g_qsize + 1), sizeof(QNode)));
    g_trsize = g_n * 4;
    g_tr = static_cast<Trap*>(std::calloc((size_t)(g_trsize + 1), sizeof(Trap)));
    alloc_permute();
    alloc_monotone();

    int i = 1;
    for (int c = 0; c < ncontours; ++c) {
        int npoints = cntr[c];
        int first = i;
        int last = first + npoints - 1;
        for (int j = 0; j < npoints; ++j, ++i) {
            g_seg[i].v0.x = vertices[2 * i];
            g_seg[i].v0.y = vertices[2 * i + 1];
            if (i == last) {
                g_seg[i].next = first;
                g_seg[i].prev = i - 1;
                g_seg[i - 1].v1 = g_seg[i].v0;
            } else if (i == first) {
                g_seg[i].next = i + 1;
                g_seg[i].prev = last;
                g_seg[last].v1 = g_seg[i].v0;
            } else {
                g_seg[i].prev = i - 1;
                g_seg[i].next = i + 1;
                g_seg[i - 1].v1 = g_seg[i].v0;
            }
            g_seg[i].is_inserted = 0;
        }
    }
    int n = i - 1;
    initialise(n);
    construct_trapezoids(n);
    int nmonpoly = monotonate_trapezoids(n);
    int ntri = triangulate_monotone_polygons(n, nmonpoly, reinterpret_cast<int (*)[3]>(triangles));
    std::free(g_seg);
    std::free(g_qs);
    std::free(g_tr);
    free_permute();
    free_monotone();
    return ntri;
}

} // namespace def::model::geoprim
