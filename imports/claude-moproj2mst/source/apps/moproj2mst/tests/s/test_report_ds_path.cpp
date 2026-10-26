// Pure Report .DS sibling path contract. This test does not construct a Report
// document or exercise Report Open/Save/Close.
#include <testkit/check.hpp>

#include "m2m/qt4path.hpp"
#include "m2m/report_ds_path.hpp"

using m2m::qt::String;

namespace {
String S(const char* value) { return String::latin1(value); }
}

TEST_CASE(report_ds_path_uses_project_directory_and_base_name)
{
    CHECK(m2m::reportDsPath(S("C:/work/Project.moproj")) == S("C:\\work\\Project.DS"));
}

TEST_CASE(report_ds_path_uses_qfileinfo_base_name_before_first_dot)
{
    CHECK(m2m::reportDsPath(S("C:\\work\\Project.v2.moproj")) == S("C:\\work\\Project.DS"));
}

TEST_CASE(report_ds_path_preserves_dotted_parent_directory)
{
    CHECK(m2m::reportDsPath(S("C:/work.v2/Project.moproj")) == S("C:\\work.v2\\Project.DS"));
}

int main() { return testkit::run_all(); }
