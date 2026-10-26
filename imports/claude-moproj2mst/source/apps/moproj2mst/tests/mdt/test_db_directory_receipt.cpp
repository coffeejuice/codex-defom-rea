// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <defdbio/db_query.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/step/db_directory_receipt.hpp>
#include <defmodel/step/db_step.hpp>
#include "m2m/engine.hpp"

#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <process.h>

namespace receipts = def::model::directory_receipts;
namespace fs = std::filesystem;
using def::model::DbDirRecord;
using m2m::engine::DbHandle;

namespace {
// Isolated receipt tests publish the real operand of this actual new[], never
// infer it from the deliberately different logical field.
void allocate(DbHandle& handle, std::size_t count)
{
    auto& reader = *handle.reader;
    reader.uac8 = static_cast<std::int32_t>(count);
    reader.uad8 = new DbDirRecord[count];
    receipts::allocationPublished(reader, reader.uad8, count);
}

receipts::Receipt ready(DbHandle& handle)
{
    receipts::allocationReady(*handle.reader, handle.reader->uad8);
    const auto result = receipts::current(&handle, *handle.reader);
    REQUIRE(result.has_value());
    return *result;
}

struct FixtureCopy {
    fs::path directory;
    fs::path main;
    FixtureCopy(const char* tag)
    {
        directory = m2m_test::scratchRoot() /
            ("db_receipt_" + std::to_string(_getpid()) + "_" + tag);
        fs::create_directories(directory);
        main = directory / "X.DB";
        const auto input = m2m_test::configuredPath("M2M_DB_FIXTURE") / "COG_P1_B1.DB";
        REQUIRE(fs::is_regular_file(input));
        fs::copy_file(input, main, fs::copy_options::overwrite_existing);
    }
    ~FixtureCopy()
    {
        std::error_code error;
        fs::remove_all(directory, error);
    }
};
} // namespace

TEST_CASE(directory_receipt_requires_actual_allocation_and_matching_owner)
{
    DbHandle handle;
    DbHandle foreign;
    CHECK(!receipts::current(&handle, *handle.reader));
    // Raw synthetic fixtures do not acquire a capacity receipt from uac8.
    handle.reader->uac8 = 2;
    handle.reader->uad8 = new DbDirRecord[2];
    CHECK(!receipts::current(&handle, *handle.reader));
    CHECK(!receipts::bindOwner(&foreign, handle.reader.get()));
    CHECK(receipts::bindOwner(&handle, handle.reader.get()));
    CHECK(!receipts::current(&foreign, *handle.reader));
}

TEST_CASE(directory_receipt_count_is_allocation_operand_and_publication_waits_for_ready)
{
    DbHandle handle;
    allocate(handle, 2);
    CHECK(!receipts::current(&handle, *handle.reader));
    handle.reader->uac8 = 1000;
    const auto receipt = ready(handle);
    CHECK_EQ(receipt.elementCount, std::size_t{2});
    CHECK(receipt.allocation == handle.reader->uad8);
    CHECK(receipt.owner == &handle);
    CHECK(receipt.reader == handle.reader.get());
    CHECK(receipt.ownerGeneration != 0);
    CHECK(receipt.allocationGeneration != 0);
    handle.reader->uac8 = 1;
    CHECK(receipts::isCurrent(receipt));
    const auto retained = receipts::current(&handle, *handle.reader);
    REQUIRE(retained.has_value());
    CHECK_EQ(retained->elementCount, std::size_t{2});
    handle.reader->uac8 = 2;
}

TEST_CASE(directory_receipt_replacement_preserves_new_receipt_when_old_array_is_deleted)
{
    DbHandle handle;
    allocate(handle, 2);
    const auto first = ready(handle);
    auto* old = static_cast<DbDirRecord*>(handle.reader->uad8);
    allocate(handle, 3);
    CHECK(!receipts::isCurrent(first));
    CHECK(!receipts::current(&handle, *handle.reader));
    def::model::delete_dir_records(old);
    const auto second = ready(handle);
    CHECK_EQ(second.elementCount, std::size_t{3});
    CHECK_EQ(first.ownerGeneration, second.ownerGeneration);
    CHECK(first.allocationGeneration != second.allocationGeneration);
    CHECK(receipts::isCurrent(second));
}

TEST_CASE(directory_receipt_delete_and_reset_retire_only_the_actual_storage)
{
    DbHandle handle;
    allocate(handle, 1);
    const auto first = ready(handle);
    def::model::delete_dir_records(static_cast<DbDirRecord*>(handle.reader->uad8));
    CHECK(!receipts::isCurrent(first));
    CHECK(!receipts::current(&handle, *handle.reader));
    handle.reader->uad8 = nullptr;
    handle.reader->uac8 = 0;
    allocate(handle, 2);
    const auto second = ready(handle);
    def::model::db_info_reset(*handle.reader);
    CHECK(!receipts::isCurrent(second));
    CHECK(handle.reader->uad8 == nullptr);
    CHECK_EQ(handle.reader->uac8, 0);
}

TEST_CASE(directory_receipt_native_zero_count_reset_retains_storage_without_repair)
{
    DbHandle handle;
    allocate(handle, 2);
    const auto receipt = ready(handle);
    handle.reader->uac8 = 0; // native reset does not delete this array on this path
    def::model::db_info_reset(*handle.reader);
    CHECK(handle.reader->uad8 == receipt.allocation);
    CHECK(receipts::isCurrent(receipt));
    const auto retained = receipts::current(&handle, *handle.reader);
    REQUIRE(retained.has_value());
    CHECK_EQ(retained->elementCount, std::size_t{2});
    handle.reader->uac8 = 2; // test teardown permits the native reset to free it
}

TEST_CASE(directory_receipt_owner_address_reuse_has_a_new_generation)
{
    std::optional<DbHandle> handle;
    handle.emplace();
    const void* firstOwner = &*handle;
    allocate(*handle, 1);
    const auto first = ready(*handle);
    handle.reset();
    CHECK(!receipts::isCurrent(first)); // no expired reader dereference
    handle.emplace();
    CHECK(&*handle == firstOwner);
    allocate(*handle, 1);
    const auto second = ready(*handle);
    CHECK(first.ownerGeneration != second.ownerGeneration);
    CHECK(!receipts::isCurrent(first));
    CHECK(receipts::isCurrent(second));
}

TEST_CASE(directory_receipt_unique_reader_replacement_retires_the_old_owner_binding)
{
    DbHandle handle;
    allocate(handle, 1);
    const auto first = ready(handle);
    handle.reader = std::make_unique<def::model::Cls_1401dbfe0>();
    CHECK(!receipts::isCurrent(first));
    CHECK(!receipts::current(&handle, *handle.reader));
    // No constructor registration or invented default for the replacement.
    allocate(handle, 1);
    receipts::allocationReady(*handle.reader, handle.reader->uad8);
    CHECK(!receipts::current(&handle, *handle.reader));
}

TEST_CASE(directory_receipt_pointer_mismatch_and_init_revoke_the_current_view)
{
    DbHandle handle;
    allocate(handle, 1);
    const auto first = ready(handle);
    handle.reader->uad8 = nullptr;
    CHECK(!receipts::current(&handle, *handle.reader));
    CHECK(!receipts::isCurrent(first));
    handle.reader->uad8 = const_cast<void*>(first.allocation);
    def::model::db_info_init(*handle.reader);
    CHECK(!receipts::isCurrent(first));
    CHECK(!receipts::current(&handle, *handle.reader));
    // init nulls without freeing: explicit fixture cleanup, not a port repair.
    def::model::delete_dir_records(static_cast<DbDirRecord*>(const_cast<void*>(first.allocation)));
}

TEST_CASE(directory_receipt_real_load_header_only_reload_and_missing_file)
{
    FixtureCopy fixture("reload");
    DbHandle handle;
    auto& reader = *handle.reader;
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), false) == 0);
    const auto first = receipts::current(&handle, reader);
    REQUIRE(first.has_value());
    CHECK_EQ(first->elementCount, std::size_t{32});
    reader.uac8 = 999; // logical field is overwritten by the header-only call
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), true) == 0);
    CHECK_EQ(reader.uac8, 32);
    CHECK(receipts::isCurrent(*first));
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), false) == 0);
    const auto second = receipts::current(&handle, reader);
    REQUIRE(second.has_value());
    CHECK_EQ(first->ownerGeneration, second->ownerGeneration);
    CHECK(first->allocationGeneration != second->allocationGeneration);
    CHECK(!receipts::isCurrent(*first));
    CHECK_EQ(second->elementCount, std::size_t{32});
    const auto missing = fixture.directory / "missing.DB";
    REQUIRE(!fs::exists(missing));
    CHECK_EQ(def::dbio::load_db_info(reader, missing.string().c_str(), false), 4);
    CHECK(!receipts::isCurrent(*second));
    CHECK(!receipts::current(&handle, reader));
}

TEST_CASE(directory_receipt_real_multipart_replacement_advances_only_allocation_generation)
{
    FixtureCopy fixture("multipart");
    fs::copy_file(fixture.main, fixture.directory / "X25.DB", fs::copy_options::overwrite_existing);
    DbHandle handle;
    auto& reader = *handle.reader;
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), false) == 0);
    const auto first = receipts::current(&handle, reader);
    REQUIRE(first.has_value());
    static_cast<DbDirRecord*>(reader.uad8)[0].step = 26;
    REQUIRE(def::dbio::query::setup_part_chain(reader) == 0);
    const auto second = receipts::current(&handle, reader);
    REQUIRE(second.has_value());
    CHECK_EQ(reader.uac8, 64);
    CHECK_EQ(second->elementCount, std::size_t{64});
    CHECK_EQ(first->ownerGeneration, second->ownerGeneration);
    CHECK(first->allocationGeneration != second->allocationGeneration);
    CHECK(!receipts::isCurrent(*first));
    CHECK(receipts::isCurrent(*second));
    CHECK_EQ(def::dbio::query::prepend_previous_part(reader), 5);
    CHECK(receipts::isCurrent(*second));
    def::model::db_info_reset(reader);
    CHECK(!receipts::isCurrent(*second));
}

TEST_CASE(directory_receipt_failed_reload_keeps_actual_extent_after_header_count_changes)
{
    FixtureCopy fixture("failed_reload");
    DbHandle handle;
    auto& reader = *handle.reader;
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), false) == 0);
    const auto original = receipts::current(&handle, reader);
    REQUIRE(original.has_value());
    REQUIRE(original->elementCount == std::size_t{32});
    CHECK_EQ(reader.uac8, 32);
    CHECK_EQ(reader.uacc, 14);
    std::array<unsigned char, 32 * sizeof(DbDirRecord)> originalRecords{};
    std::memcpy(originalRecords.data(), reader.uad8, originalRecords.size());
    const auto originalSize = fs::file_size(fixture.main);

    // This fixture's numeric words are big endian. Header op0 reads the index
    // record, but only directory op1 dereferences its chunk-record word.
    using Word = std::array<char, 4>;
    constexpr Word count32{0, 0, 0, 0x20};
    constexpr Word count33{0, 0, 0, 0x21};
    constexpr Word chunk4{0, 0, 0, 4};
    constexpr Word chunk0{0, 0, 0, 0};
    const auto rewriteCopy = [&](const Word& expectedCount, const Word& expectedChunk,
                                 const Word& newCount, const Word& newChunk) {
        std::fstream file(fixture.main, std::ios::binary | std::ios::in | std::ios::out);
        REQUIRE(file.is_open());
        Word bytes{};
        file.seekg(632); // record2 word31: NRSETS
        file.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        REQUIRE(file.good());
        REQUIRE(bytes == expectedCount);
        file.seekg(1024); // record3 word1: chunk1's record number
        file.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        REQUIRE(file.good());
        REQUIRE(bytes == expectedChunk);
        file.seekp(632);
        file.write(newCount.data(), static_cast<std::streamsize>(newCount.size()));
        REQUIRE(file.good());
        file.seekp(1024);
        file.write(newChunk.data(), static_cast<std::streamsize>(newChunk.size()));
        file.flush();
        REQUIRE(file.good());
        file.close();
        REQUIRE(!file.fail());
    };
    rewriteCopy(count32, chunk4, count33, chunk0);
    CHECK_EQ(fs::file_size(fixture.main), originalSize);

    // The new header is committed, then record0 fails with directory error3.
    // No new[]/receipt publication or record32 access may follow this failure.
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), false) == 3);
    CHECK_EQ(reader.uac8, 33);
    REQUIRE(reader.uad8 == original->allocation);
    CHECK_EQ(reader.uacc, 0); // preserve early native partial effects
    CHECK_EQ(reader.u1900, 0);
    const auto retained = receipts::current(&handle, reader);
    REQUIRE(retained.has_value());
    CHECK(retained->allocation == original->allocation);
    CHECK_EQ(retained->elementCount, std::size_t{32});
    CHECK_EQ(retained->ownerGeneration, original->ownerGeneration);
    CHECK_EQ(retained->allocationGeneration, original->allocationGeneration);
    CHECK(receipts::isCurrent(*original));
    CHECK(std::memcmp(originalRecords.data(), reader.uad8, originalRecords.size()) == 0);

    // Restore only the disposable file; let the next real reload perform every
    // state transition, including retiring the formerly retained allocation.
    rewriteCopy(count33, chunk0, count32, chunk4);
    CHECK_EQ(fs::file_size(fixture.main), originalSize);
    REQUIRE(def::dbio::load_db_info(reader, fixture.main.string().c_str(), false) == 0);
    const auto recovered = receipts::current(&handle, reader);
    REQUIRE(recovered.has_value());
    CHECK_EQ(reader.uac8, 32);
    CHECK_EQ(reader.uacc, 14);
    CHECK_EQ(recovered->elementCount, std::size_t{32});
    CHECK_EQ(recovered->ownerGeneration, original->ownerGeneration);
    CHECK(recovered->allocationGeneration != original->allocationGeneration);
    CHECK(!receipts::isCurrent(*original));
    CHECK(receipts::isCurrent(*recovered));
}

int main() { return testkit::run_all(); }
