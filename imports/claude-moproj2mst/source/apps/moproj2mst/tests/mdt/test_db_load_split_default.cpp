#include "../support/test_paths.hpp"

#include <testkit/check.hpp>

#include <process.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <system_error>
#include <utility>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <deformdb/database.hpp>
#include <deformdb/writer.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>

#include "m2m/engine.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

const fs::path kCog = m2m_test::configuredPath("M2M_DB_FIXTURE");

struct DbCoreReset {
    DbCoreReset() { def::dbio::dbcore::reset_globals(); }
    ~DbCoreReset() { def::dbio::dbcore::reset_globals(); }
};

struct ScratchTree {
    fs::path path;
    explicit ScratchTree(const char* tag)
        : path(m2m_test::scratchRoot() / (std::string("m2m_db_split_") +
                                             std::to_string(_getpid()) + "_" + tag))
    {
        std::error_code ec;
        fs::remove_all(path, ec);
        fs::create_directories(path);
    }
    ~ScratchTree()
    {
        std::error_code ec;
        fs::remove_all(path, ec);
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

void writeHeaderOnlyDb(const fs::path& path, deformdb::WriterHeader header,
                       const deformdb::StepData& step)
{
    auto writer = deformdb::Writer::create(path, std::move(header));
    writer->append_step(step, true);
    writer->flush();
}

std::string partName(const def::model::Cls_1401dbfe0& reader, std::size_t index)
{
    return fs::path(static_cast<const char*>(reader.u330[index])).filename().string();
}

} // namespace

// This is a synthetic, format-valid two-part chain, not a captured database split at the 2-GB
// limit. It exercises the real Open-side dbOpen predicate, default-Yes route, chain setup and cache.
TEST_CASE(db_open_split_chain_default_yes)
{
    if (!fs::exists(kCog / "COG_P1_B1.DB")) {
        std::printf("SKIP: no %s\n", (kCog / "COG_P1_B1.DB").string().c_str());
        return;
    }

    DbCoreReset reset;
    ScratchTree scratch("default_yes");
    auto source = deformdb::Database::open(kCog / "COG_P1_B1.DB");
    REQUIRE(source != nullptr);
    REQUIRE(!source->entries().empty());

    deformdb::WriterHeader writerHeader = deformdb::writer_header_from(source->header());
    writeHeaderOnlyDb(scratch.path / "X25.DB", writerHeader, headerOnlyStep(*source, 25));
    writeHeaderOnlyDb(scratch.path / "X.DB", writerHeader, headerOnlyStep(*source, 26));

    {
        engine::DbHandle handle;
        Ctx ctx;
        const fs::path primary = scratch.path / "X.DB";
        engine::dbSetPath(handle, qt::String(primary.u16string()), false);

        REQUIRE(engine::dbOpen(handle, ctx, true, false));
        CHECK(handle.loaded);
        CHECK(handle.cacheStarted);
        CHECK(handle.reader->u1900 != 0);
        CHECK_EQ(handle.reader->u008, 2);
        CHECK_EQ(handle.reader->uac8, 2);
        CHECK_EQ(partName(*handle.reader, 0), std::string("X25.DB"));
        CHECK_EQ(partName(*handle.reader, 1), std::string("X.DB"));
        CHECK_EQ(handle.reader->u00c[0].a, 0);
        CHECK_EQ(handle.reader->u00c[0].b, 0);
        CHECK_EQ(handle.reader->u00c[1].a, 1);
        CHECK_EQ(handle.reader->u00c[1].b, 1);
        REQUIRE(ctx.warnings.size() == 1u);
        CHECK(ctx.warnings[0].find("2G Split DBs") != std::string::npos);
        CHECK(ctx.warnings[0].find("default button Yes") != std::string::npos);
    }

    // Existing single-file path is a control: no question warning, but setup_part_chain still
    // publishes the one-part table before header-cache construction.
    def::dbio::dbcore::reset_globals();
    {
        writeHeaderOnlyDb(scratch.path / "Y.DB", writerHeader, headerOnlyStep(*source, -1));
        engine::DbHandle handle;
        Ctx ctx;
        engine::dbSetPath(handle, qt::String((scratch.path / "Y.DB").u16string()), false);
        REQUIRE(engine::dbOpen(handle, ctx, true, false));
        CHECK(handle.loaded);
        CHECK(handle.cacheStarted);
        CHECK(handle.reader->u1900 != 0);
        CHECK_EQ(handle.reader->u008, 1);
        CHECK_EQ(handle.reader->uac8, 1);
        CHECK_EQ(partName(*handle.reader, 0), std::string("Y.DB"));
        CHECK(ctx.warnings.empty());
    }
}

int main() { return testkit::run_all(); }
