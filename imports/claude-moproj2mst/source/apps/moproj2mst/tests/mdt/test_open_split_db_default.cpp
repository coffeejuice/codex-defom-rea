#include "../support/test_paths.hpp"

#include <testkit/check.hpp>

#include <process.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <deformdb/database.hpp>
#include <deformdb/writer.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

const fs::path kCog = m2m_test::configuredPath("M2M_DB_FIXTURE");

fs::path oracleB()
{
    if (const char* value = std::getenv("M2M_ORACLE_B")) return fs::path(value);
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

struct DbCoreReset {
    DbCoreReset() { def::dbio::dbcore::reset_globals(); }
    ~DbCoreReset() { def::dbio::dbcore::reset_globals(); }
};

struct ProblemScratch {
    fs::path root = LR"(C:\PROBLEM)";
    fs::path rootResolved;
    fs::path path;

    ProblemScratch()
    {
        fs::create_directories(root);
        rootResolved = fs::canonical(root);
        std::random_device random;
        for (int attempt = 0; attempt < 10; ++attempt) {
            path = root / ("o" + std::to_string(_getpid()) + "_" + std::to_string(random()));
            std::error_code ec;
            if (fs::create_directory(path, ec)) {
                const fs::path resolved = fs::canonical(path);
                if (resolved.parent_path() != rootResolved) {
                    fs::remove_all(resolved, ec);
                    throw std::runtime_error("Open split fixture escaped C:/PROBLEM");
                }
                return;
            }
            if (ec) throw fs::filesystem_error("Create Open split fixture", path, ec);
        }
        throw std::runtime_error("Could not allocate a unique C:/PROBLEM fixture directory");
    }

    ~ProblemScratch()
    {
        if (path.empty()) return;
        std::error_code ec;
        const fs::path resolved = fs::canonical(path, ec);
        if (!ec && resolved.parent_path() == rootResolved) fs::remove_all(resolved, ec);
    }
};

deformdb::StepData headerOnlyStep(const deformdb::Database& source, std::int32_t stepNo)
{
    deformdb::ReadOptions options;
    options.header = true;
    options.arrays = false;
    options.dynamic = false;
    deformdb::StepData step = source.read_step(source.entries().front(), options);
    step.entry.step = stepNo;
    return step;
}

void writeHeaderOnlyDb(const fs::path& path, const deformdb::WriterHeader& header,
                       const deformdb::StepData& step)
{
    auto writer = deformdb::Writer::create(path, header);
    writer->append_step(step, true);
    writer->flush();
}

std::string partName(const def::model::Cls_1401dbfe0& reader, std::size_t index)
{
    return fs::path(static_cast<const char*>(reader.u330[index])).filename().string();
}

} // namespace

// The pair is a synthetic, format-valid header-only chain, not a captured 2-GB split DB. This
// test reaches the DB default-Yes branch through File > Open's project caller, but does not emulate
// Qt's modal event delivery or claim full original GUI Open equivalence.
TEST_CASE(project_open_split_db_default_yes)
{
    const fs::path sourceProject = oracleB() / "125_B_DBSTEP_NODB_before" / "COG_P1_B1.moproj";
    if (!fs::exists(sourceProject) || !fs::exists(kCog / "COG_P1_B1.DB")) {
        std::printf("SKIP: missing Open split-DB project or COG header fixture\n");
        return;
    }

    DbCoreReset reset;
    ProblemScratch scratch;
    const fs::path projectFile = scratch.path / "COG_P1_B1.moproj";
    const fs::path primaryDb = scratch.path / "COG_P1_B1.DB";
    const fs::path previousDb = scratch.path / "COG_P1_B125.DB";
    CHECK(projectFile.string().size() < 100);
    CHECK(primaryDb.string().size() < 100);
    CHECK(previousDb.string().size() < 100);

    fs::copy_file(sourceProject, projectFile, fs::copy_options::overwrite_existing);
    auto source = deformdb::Database::open(kCog / "COG_P1_B1.DB");
    REQUIRE(source != nullptr);
    REQUIRE(!source->entries().empty());
    const deformdb::WriterHeader writerHeader = deformdb::writer_header_from(source->header());
    writeHeaderOnlyDb(previousDb, writerHeader, headerOnlyStep(*source, 25));
    writeHeaderOnlyDb(primaryDb, writerHeader, headerOnlyStep(*source, 26));

    qt::CurrentDirScope restoreCurrentDir(qt::currentPath());
    REQUIRE(restoreCurrentDir.ok());
    Project project;
    ProjectScope projectScope(project);
    Ctx ctx;
    const qt::String file(projectFile.u16string());
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(project);

    // This is the caller used by the public Open route. It reaches openProjectDb before the
    // project XML walk; stop after Open and do not imply subproject activation, Save, or Close.
    REQUIRE(openProject(project, file, ctx));
    CHECK(project.openedOk);
    REQUIRE(project.dataDb != nullptr);
    const std::string expectedDbPath = qt::cleanPath(qt::String(primaryDb.u16string())).utf8();
    CHECK_EQ(project.dataDb->path.utf8(), expectedDbPath);
    CHECK(project.dataDb->loaded);
    CHECK(project.dataDb->cacheStarted);
    CHECK(project.dataDb->reader->u1900 != 0);
    CHECK_EQ(project.dataDb->reader->u008, 2);
    CHECK_EQ(project.dataDb->reader->uac8, 2);
    CHECK_EQ(partName(*project.dataDb->reader, 0), std::string("COG_P1_B125.DB"));
    CHECK_EQ(partName(*project.dataDb->reader, 1), std::string("COG_P1_B1.DB"));
    CHECK_EQ(project.dataDb->reader->u00c[0].a, 0);
    CHECK_EQ(project.dataDb->reader->u00c[0].b, 0);
    CHECK_EQ(project.dataDb->reader->u00c[1].a, 1);
    CHECK_EQ(project.dataDb->reader->u00c[1].b, 1);
    CHECK_EQ(project.diagramViewHeight, 175);
    const auto warning = std::find_if(ctx.warnings.begin(), ctx.warnings.end(), [](const std::string& value) {
        return value.find("2G Split DBs") != std::string::npos;
    });
    REQUIRE(warning != ctx.warnings.end());
    CHECK(warning->find("default button Yes") != std::string::npos);
}

int main() { return testkit::run_all(); }
