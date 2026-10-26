#include <testkit/check.hpp>

#include <string>

#include "m2m/report_path.hpp"

namespace {
std::string U(const m2m::qt::String& s) { return s.utf8(); }
}  // namespace

TEST_CASE(report_document_path)
{
    // QFileInfo::baseName stops at the first dot; this intentionally differs from
    // completeBaseName for a multi-dot project filename.
    CHECK_EQ(U(m2m::report::documentPath(m2m::qt::L("C:/case/press.v11.project.moproj"))),
             "C:/case\\press.DS");

    // QFileInfo absolutePath resolves and cleans the directory, while the Report
    // expression appends QDir::separator() without cleaning the finished string.
    CHECK_EQ(U(m2m::report::documentPath(m2m::qt::L("c:\\case\\sub\\.\\press.moproj"))),
             "C:/case/sub\\press.DS");

    // A relative project filename uses the current directory through QFileInfo's
    // absolutePath semantics; the derived leaf still uses the QFileInfo baseName.
    m2m::qt::CurrentDirScope cwd(m2m::qt::L("C:/Windows"));
    REQUIRE(cwd.ok());
    CHECK_EQ(U(m2m::report::documentPath(m2m::qt::L("jobs/run.moproj"))),
             "C:/Windows/jobs\\run.DS");
}

int main()
{
    return testkit::run_all();
}
