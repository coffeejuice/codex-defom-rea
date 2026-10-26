// ctest-labels: p1-11 topo
// Tier-1 tests of _DEF_LOOP / _DEF_EDGE / _DEF_VERTEX (140412010..140413790, 14041c300..14041d0b0)
// and of the loop -> curve builder 1404126d0, which runs here on a recording curve type because
// _OBJ_GEO2D (p1-07) is not ported yet. Expected orders and call sequences are traced by hand
// through the decompiled code (comments).
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/topo/geo_fallback.hpp>
#include <defmodel/topo/loop_geo2d.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using namespace def::model;

namespace {

std::vector<std::string> g_log;

// Recording stand-in for _OBJ_GEO2D: logs the calls the builder makes.
class MockCurve : public OBJ_GEO_BASE {
public:
    explicit MockCurve(std::string n = "tmp") : name(std::move(n)) {}
    void copy_from(OBJ_GEO_BASE* o) override {
        g_log.push_back("copy " + name + "<-" + static_cast<MockCurve*>(o)->name);
    }
    void reverse() override { g_log.push_back("reverse " + name); }
    void scale(const double*) override {}
    void translate(const double*) override {}
    void rotate(const double*, const void*, double) override {}
    double char_length() override { return 0.0; }
    void notify() override {}
    std::int32_t is_extension() override { return 0; }

    void clear_all() { g_log.push_back("clear " + name); }
    std::int64_t set_curve_type(std::int32_t t) {
        g_log.push_back("type " + name + "=" + std::to_string(t));
        curve_type = t;
        return 0;
    }
    void fun_1402765e0() { g_log.push_back("f65e0 " + name); }
    void append(MockCurve* o) { g_log.push_back("append " + name + "<-" + o->name); }
    bool is_ccw() {
        g_log.push_back("ccw? " + name);
        return ccw;
    }
    bool contains_bbox(MockCurve* o) {
        g_log.push_back("bbox " + name + "," + o->name);
        return true;
    }

    std::int32_t curve_type = 2;
    bool ccw = false;
    std::string name;
};

struct Topo {
    std::vector<std::unique_ptr<DEF_VERTEX>> verts;
    std::vector<std::unique_ptr<DEF_EDGE>> edges;

    explicit Topo(int nv) {
        for (int i = 0; i < nv; ++i) verts.push_back(std::make_unique<DEF_VERTEX>());
    }
    DEF_EDGE* edge(const char* name, int a, int b, std::int32_t type = 2) {
        auto e = std::make_unique<DEF_EDGE>();
        auto* c = new MockCurve(name);
        c->curve_type = type;
        e->set_geo(c); // owned by the edge (14041c3a0 deletes it)
        e->vertices[0] = verts[a].get();
        e->vertices[1] = verts[b].get();
        edges.push_back(std::move(e));
        return edges.back().get();
    }
};

std::vector<DEF_EDGE*> edge_order(DEF_LOOP& l) {
    std::vector<DEF_EDGE*> r;
    for (std::int32_t i = 0; i < l.edge_count(); ++i) r.push_back(dynamic_cast<DEF_EDGE*>(l.edge_at(&i)));
    return r;
}

std::string joined() {
    std::string s;
    for (const auto& x : g_log) s += x + ";";
    return s;
}

} // namespace

TEST_CASE(constructors_zero_fields)
{
    DEF_EDGE e; // 14041c300
    CHECK(e.geo == nullptr);
    CHECK(e.vertices[0] == nullptr && e.vertices[1] == nullptr);
    CHECK_EQ(e.id, 0);
    CHECK_EQ(int{e.u34}, 0);
    CHECK_EQ(int{e.u35}, 0);
    CHECK_EQ(e.vslot3(), -1);                  // 14041cdb0
    CHECK(e.u28_vector() == &e.u28);           // 14041c460
    CHECK(reinterpret_cast<char*>(e.u28_vector()) - reinterpret_cast<char*>(&e) == 0x28);

    DEF_VERTEX v; // 14041ce30
    CHECK_BITS(v.xy[0], 0.0);
    CHECK_BITS(v.xy[1], 0.0);
    CHECK_EQ(int{v.u20}, 0);
    CHECK_EQ(v.edge_count(), 0);

    DEF_LOOP l; // 140412010
    CHECK_EQ(int{l.u1c}, 0);
    CHECK_EQ(l.edge_count(), 0);
    CHECK_EQ(l.face_count(), 0);
}

TEST_CASE(accessors_range_checks)
{
    Topo t(2);
    DEF_EDGE* e = t.edge("A", 0, 1);
    DEF_LOOP l;
    std::int32_t i = 0;
    CHECK(l.edge_at(&i) == nullptr); // empty list
    l.add_edge(e);
    CHECK(l.edge_at(&i) == e);
    i = 1;
    CHECK(l.edge_at(&i) == nullptr); // i >= n
    i = -1;
    CHECK(l.edge_at(&i) == nullptr); // i < 0

    std::int32_t id = 7;
    e->set_id(&id); // 14041c5f0
    CHECK_EQ(e->get_id(), 7);
    i = 1;
    CHECK(e->vertex(&i) == t.verts[1].get()); // 14041c590
    e->add_loop(&l);
    i = 0;
    CHECK(e->loop_at(&i) == &l);
    CHECK_EQ(e->loop_count(), 1);

    DEF_VERTEX* v = t.verts[0].get();
    const double xy[2] = {1.5, -2.25};
    v->set_xy(xy); // 14041cf80
    CHECK(v->get_xy() == v->xy);
    CHECK_BITS(v->xy[0], 1.5);
    CHECK_BITS(v->xy[1], -2.25);
    v->add_edge(e);
    i = 0;
    CHECK(v->edge_at(&i) == e);
    i = 1;
    CHECK(v->edge_at(&i) == nullptr);
}

TEST_CASE(vertex_collect_edges_replaces_contents)
{
    Topo t(3);
    DEF_EDGE* a = t.edge("A", 0, 1);
    DEF_EDGE* b = t.edge("B", 0, 2);
    DEF_VERTEX* v = t.verts[0].get();
    v->add_edge(a);
    v->add_edge(b);
    topo::CowPtrVector<IEdge> out;
    IEdge* junk = b;
    out.push_back(&junk);
    v->collect_edges(&out); // 14041cee0: clear, then edges in list order
    CHECK_EQ(out.size(), std::int64_t{2});
    CHECK(out[0] == a);
    CHECK(out[1] == b);
}

TEST_CASE(order_edges_closed_square)
{
    // square v0-v1-v2-v3; inserted A(v0,v1), C(v2,v3), B(v2,v1), D(v3,v0).
    // 140412da0: start A (start v0, end v1); from v1: B (its v1 matches, end -> v2); from v2: C
    // (end -> v3); from v3: D (end -> v0); nothing new from v0 on the start side; v0 == v0: closed.
    Topo t(4);
    DEF_EDGE* A = t.edge("A", 0, 1);
    DEF_EDGE* B = t.edge("B", 2, 1);
    DEF_EDGE* C = t.edge("C", 2, 3);
    DEF_EDGE* D = t.edge("D", 3, 0);
    DEF_LOOP l;
    l.add_edge(A);
    l.add_edge(C);
    l.add_edge(B);
    l.add_edge(D);
    CHECK(l.order_edges());
    const std::vector<DEF_EDGE*> want = {A, B, C, D};
    CHECK(edge_order(l) == want);
    for (DEF_EDGE* e : want) CHECK(!e->visited());
}

TEST_CASE(order_edges_open_chain_prepends)
{
    // inserted B(v2,v1), A(v0,v1), C(v2,v3): start B (start v2, end v1); from v1: A (end -> v0);
    // start side from v2: C pushed to the front (start -> v3); v3 != v0: open.
    Topo t(4);
    DEF_EDGE* A = t.edge("A", 0, 1);
    DEF_EDGE* B = t.edge("B", 2, 1);
    DEF_EDGE* C = t.edge("C", 2, 3);
    DEF_LOOP l;
    l.add_edge(B);
    l.add_edge(A);
    l.add_edge(C);
    CHECK(!l.order_edges());
    const std::vector<DEF_EDGE*> want = {C, B, A};
    CHECK(edge_order(l) == want);
    CHECK(!DEF_LOOP().order_edges()); // empty loop: false
}

TEST_CASE(loop_to_curve_square)
{
    // 1404126d0 on the ordered square A(v0,v1) B(v2,v1) C(v2,v3) D(v3,v0), curve type 2:
    // clear out, copy A; edge 1 (B) touches A's second vertex -> walk from v1;
    // B ends at v1 -> tmp = reversed B, appended; C starts at v2 -> appended; D starts at v3 ->
    // appended; is_ccw false -> reverse out; type 2 -> no 1402765e0.
    Topo t(4);
    DEF_LOOP l;
    l.add_edge(t.edge("A", 0, 1));
    l.add_edge(t.edge("C", 2, 3));
    l.add_edge(t.edge("B", 2, 1));
    l.add_edge(t.edge("D", 3, 0));
    MockCurve out("out");
    g_log.clear();
    topo::loop_to_geo2d<MockCurve>(&l, &out);
    CHECK_EQ(joined(), std::string("clear out;copy out<-A;clear tmp;copy tmp<-B;reverse tmp;"
                                   "append out<-tmp;append out<-C;append out<-D;ccw? out;"
                                   "reverse out;"));
}

TEST_CASE(loop_to_curve_type1_converts_and_back)
{
    // first curve type 1: every edge curve to type 2, build, every edge back to 1, out to 1,
    // then out (type 1) gets 1402765e0. Two edges F(v1,v0), A(v0,v1): edge 1 touches F's first
    // vertex v1 -> out reversed, walk from v1; A ends at v1 -> reversed copy appended.
    Topo t(2);
    DEF_LOOP l;
    l.add_edge(t.edge("F", 1, 0, 1));
    l.add_edge(t.edge("A", 0, 1, 1));
    MockCurve out("out");
    out.ccw = true;
    g_log.clear();
    topo::loop_to_geo2d<MockCurve>(&l, &out);
    CHECK_EQ(joined(), std::string("clear out;type F=2;type A=2;copy out<-F;reverse out;"
                                   "clear tmp;copy tmp<-A;reverse tmp;append out<-tmp;"
                                   "type F=1;type A=1;type out=1;ccw? out;f65e0 out;"));
}

TEST_CASE(loop_to_curve_single_edge_checks_orientation_twice)
{
    // n == 1: copy, ccw check (reverse), then the common tail checks again (reverse again)
    Topo t(2);
    DEF_LOOP l;
    l.add_edge(t.edge("A", 0, 1));
    MockCurve out("out");
    g_log.clear();
    topo::loop_to_geo2d<MockCurve>(&l, &out);
    CHECK_EQ(joined(), std::string("clear out;copy out<-A;ccw? out;reverse out;ccw? out;reverse out;"));

    DEF_LOOP empty;
    g_log.clear();
    topo::loop_to_geo2d<MockCurve>(&empty, &out); // n < 1: clear only
    CHECK_EQ(joined(), std::string("clear out;"));
}

TEST_CASE(loop_bbox_compare_builds_both)
{
    Topo t(2);
    DEF_LOOP a;
    a.add_edge(t.edge("A", 0, 1));
    DEF_LOOP b;
    b.add_edge(t.edge("B", 0, 1));
    g_log.clear();
    CHECK(topo::loop_inside_bbox<MockCurve>(&a, &b)); // 140412610
    CHECK_EQ(joined(), std::string("clear tmp;copy tmp<-A;ccw? tmp;reverse tmp;ccw? tmp;reverse tmp;"
                                   "clear tmp;copy tmp<-B;ccw? tmp;reverse tmp;ccw? tmp;reverse tmp;"
                                   "bbox tmp,tmp;"));
}

int main() { return testkit::run_all(); }
