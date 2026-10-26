// MDT: the DEF_PRE engine (port/) reads a GUI object KEY into a model. Corpus 42_vdie object 1:
// the node box in doubles is 1112.55952432 x 404.32 x 404.32 (hand calculation from the RZ
// lines, +-556.27976216 and +-202.16). The GUI's Save writes 1.1125596E+003 / 4.0432001E+002 into
// Cogging.MDT (oracle B MDT8) because its 3D view box keeps QVector3D floats (loader-spec.md);
// the last check shows that float rounding of the same extremes gives exactly those values.
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include <defcmd/key_file.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>

namespace fs = std::filesystem;

TEST_CASE(vdie42_billet_box)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    const fs::path key = fs::path(corpus) / "42_vdie" / "Objects" / "Object00001.KEY";
    REQUIRE(fs::exists(key));
    auto* m = new def::model::DB_STEP_DATA(3);
    const char reset = 0;
    const std::int32_t flags = 0;
    const int rc = def::cmd::read_keyword_file(m, key.string().c_str(), &reset, &flags);
    CHECK_EQ(rc, 0);
    def::model::LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 1);
    REQUIRE(o != nullptr);
    double mn[3] = {}, mx[3] = {};
    const bool ok = def::dbio::gendb::node_coord_range(reinterpret_cast<char*>(o) + 0x8a0, mn, mx);
    CHECK(ok);
    char b[128];
    CHECK(mn[0] == -556.27976216 && mx[0] == 556.27976216);
    CHECK(mn[1] == -202.16 && mx[1] == 202.16 && mn[2] == -202.16 && mx[2] == 202.16);
    const double fl = static_cast<double>(static_cast<float>(mx[0])) - static_cast<double>(static_cast<float>(mn[0]));
    const double fd = static_cast<double>(static_cast<float>(mx[1])) - static_cast<double>(static_cast<float>(mn[1]));
    std::snprintf(b, sizeof b, "%.7E %.7E", fl, fd);
    CHECK_EQ(std::string(b), std::string("1.1125596E+03 4.0432001E+02"));
}

int main()
{
    return testkit::run_all();
}
