// ctest-labels: p67-1 objpos c07
// Census packet C07 rows of p67-1: the repeated-node test 1401fcc50 (p67-1-2d's surface2d.cpp; its
// boundary-loop curves 1401fcd50 are tested there) and the 0x28-byte element of the 1401fd240 subtree (1403e6260)
// with its copy loop 1403e6180 (model::cow_detail::copy).
//
// Expected values: node coordinates and connectivity as written in the corpus seed
// 2D/SI/Forging/SPIKE_SI.KEY (RZ 1: node 1 = (0, 0), node 2 = (1.9743107, 0), node 3 =
// (3.9439832, 0), node 333 = (19.526181, 57.15); ELMCON 1: 289 quads, none repeats a node), and
// reference counts worked out by hand from the binary's 1402207c0 (source refs + 1, own body
// refs - 1). The copy loop and the element assignment were also checked against the routines of
// DEF_PRE_64.exe with the binary-routine harness (deform-rea-work/scripts_c07/c2d_copy_check.cpp).
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <vector>

#include <defcmd/objpos.hpp>
#include <defcmd/objpos_c2d.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace op = def::cmd::objpos;
namespace gk = def::model::geo_kw;

namespace {

template <class T, class C>
T& at(C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}

const char* kCorpus = "C:/Users/alext/Projects/DEFORM_corpus/key/";

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        switch (group) {
        case 1: return def::key::read_group1(m, f, id);
        case 2: return def::key::g2obj::read_object_keyword(*m, f, id);
        case 3: return def::key::g3::read_keyword(m, f, id);
        case 4: return def::key::g4::read_keyword(*m, f, id);
        case 5: return def::key::g57::read_phase_keyword(m, f, id);
        case 7: return def::key::g57::read_dim_keyword(m, f, id);
        default: break;
        }
        char buf[0x400];
        (void)std::fgets(buf, sizeof buf, f);
        return 0;
    }
};

DB_STEP_DATA* load(const char* rel)
{
    const fs::path src = fs::path(kCorpus) / rel;
    if (!fs::exists(src)) {
        std::printf("corpus file %s missing: skipped\n", src.string().c_str());
        return nullptr;
    }
    const fs::path tmp = fs::temp_directory_path() / keytest::tmp_name("objpos_c07_seed.key");
    fs::copy_file(src, tmp, fs::copy_options::overwrite_existing);
    auto* m = new DB_STEP_DATA(2);  // leaked on purpose (the dtor is not under test)
    Hooks h(m);
    std::FILE* f = std::fopen(tmp.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    def::key::read_key_stream(f, h, sink);
    std::fclose(f);
    std::fclose(sink);
    std::error_code ec;
    fs::remove(tmp, ec);
    return m;
}

} // namespace

TEST_CASE(repeated_node_test_spike)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = load("2D/SI/Forging/SPIKE_SI.KEY");
    if (m == nullptr) return;
    LLOBJ_DATA* o = def::model::object_list_find(&m->objects, 1);
    REQUIRE(o != nullptr);
    CHECK_EQ(at<std::int32_t>(o, 0x884), 289);
    CHECK(at<std::int32_t>(o, 0x87c) != 0);
    CHECK(!op::fun_1401fcc50(o));  // ELMCON 1: all quads
    std::int32_t* el = at<std::int32_t*>(o, 0x9e8);
    const std::int32_t keep = el[288 * 4 + 3];
    el[288 * 4 + 3] = el[288 * 4 + 0];  // last element, nodes 1 and 4
    CHECK(op::fun_1401fcc50(o));
    el[288 * 4 + 3] = keep;
    CHECK_EQ(el[1], 115);  // ELMCON 1, element 1: 128 115 124 129
    el[1] = el[2];         // element 1, nodes 2 and 3
    CHECK(op::fun_1401fcc50(o));
    el[1] = 115;
    CHECK(!op::fun_1401fcc50(o));
    const std::int32_t flag = at<std::int32_t>(o, 0x87c);
    at<std::int32_t>(o, 0x87c) = 0;  // no mesh flag: false whatever the connectivity
    el[1] = el[2];
    CHECK(!op::fun_1401fcc50(o));
    el[1] = 115;
    at<std::int32_t>(o, 0x87c) = flag;
}

TEST_CASE(rec28_assign_and_copy_refcounts)
{
    using V = def::model::CowVector<std::int32_t>;
    std::vector<op::C2dRec28> src(2), dst(2);
    src[0].u08 = 8;
    src[0].u20 = 32;
    src[0].u24 = 36;
    src[1].u08 = -1;
    src[1].u20 = 7;
    src[1].u24 = 9;
    V keep = dst[0].u10;  // dst[0].u10's old body: refs 2 -> 1 after the copy
    CHECK_EQ(keep.use_count(), 2);
    const op::C2dRec28* end = def::model::cow_detail::copy(src.data(), src.data() + 2, dst.data());
    CHECK(end == dst.data() + 2);
    for (int i = 0; i < 2; ++i) {
        CHECK(dst[i].u00.body() == src[i].u00.body());
        CHECK(dst[i].u10.body() == src[i].u10.body());
        CHECK(dst[i].u18.body() == src[i].u18.body());
        CHECK_EQ(src[i].u00.use_count(), 2);
        CHECK_EQ(src[i].u18.use_count(), 2);
        CHECK_EQ(dst[i].u08, src[i].u08);
        CHECK_EQ(dst[i].u20, src[i].u20);
        CHECK_EQ(dst[i].u24, src[i].u24);
    }
    CHECK_EQ(keep.use_count(), 1);
}

TEST_CASE(cow_body_size_0x18)
{
    struct E18 {
        double a, b, c;
    };
    def::model::CowVectorBody<E18> b;
    E18 buf[5] = {};
    b.first = buf;
    b.last = buf + 5;
    b.end = buf + 5;
    CHECK_EQ(b.size(), 5u);  // 1403e51b0: (last - first) / 0x18
    b.first = b.last = b.end = nullptr;
}

int main() { return testkit::run_all(); }
