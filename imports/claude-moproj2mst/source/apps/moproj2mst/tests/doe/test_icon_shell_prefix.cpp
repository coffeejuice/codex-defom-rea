// ctest-labels: m2m-doe
// Isolated constructor/request prefix, never Qt icon loading or receiver delivery.
#include <testkit/check.hpp>

#include <cstddef>
#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_icon_shell_prefix.hpp"

using namespace m2m;
using namespace m2m::doe;

TEST_CASE(doe_icon_shell_ctor_is_one_null_private_pointer_with_default_size)
{
    DoeIconShellArena arena;
    const auto& prefix = appendDoeIconShellPrefix(arena);
    CHECK_EQ(sizeof(DoeIconShellValue), std::size_t(8));
    REQUIRE(prefix.shell() != nullptr);
    CHECK(prefix.shell()->privateData == nullptr);
    const auto* bytes = reinterpret_cast<const unsigned char*>(prefix.shell());
    for (std::size_t i = 0; i < sizeof(DoeIconShellValue); ++i) CHECK_EQ(bytes[i], 0);
    REQUIRE(prefix.size().has_value());
    CHECK_EQ(prefix.size()->width, -1);
    CHECK_EQ(prefix.size()->height, -1);
    CHECK(prefix.stage() == DoeIconShellStage::AddFileBoundary);
    REQUIRE(prefix.nextBoundary().has_value());
    CHECK(*prefix.nextBoundary() == DoeIconShellNextBoundary::AddFile);
}

TEST_CASE(doe_icon_shell_request_preserves_exact_source_resource_mode_and_state)
{
    DoeIconShellArena arena;
    const auto& prefix = appendDoeIconShellPrefix(arena);
    REQUIRE(prefix.request().has_value());
    const auto& request = *prefix.request();
    CHECK(request.resource == qt::L(":/SfTool/icon/tool/iconDoe_24_24.png"));
    CHECK_EQ(request.size.width, -1);
    CHECK_EQ(request.size.height, -1);
    CHECK_EQ(request.mode, 0);
    CHECK_EQ(request.state, 1);
    CHECK(std::string(prefix.guardMessage()).find("unsupported") != std::string::npos);
    CHECK(std::string(prefix.guardMessage()).find("addFile") != std::string::npos);
    CHECK(prefix.shell()->privateData == nullptr); // Request preparation did not create d/engine.
}

TEST_CASE(doe_icon_shell_repeated_prefixes_keep_distinct_allocations_and_stable_observers)
{
    DoeIconShellArena arena;
    const auto* first = &appendDoeIconShellPrefix(arena);
    const auto* firstShell = first->shell();
    const auto* second = &appendDoeIconShellPrefix(arena);
    REQUIRE(arena.size() == 2);
    CHECK(first != second);
    CHECK(firstShell != second->shell());
    CHECK(&arena.at(0) == first);
    CHECK(&arena.at(1) == second);
    CHECK(first->shell() == firstShell);
    CHECK(first->shell()->privateData == nullptr);
    CHECK(second->shell()->privateData == nullptr);
}

TEST_CASE(doe_icon_shell_addfile_guard_keeps_preceding_allocation_in_caller_arena)
{
    DoeIconShellArena arena;
    const DoeIconShellValue* allocated = nullptr;
    bool guarded = false;
    try {
        const auto& prefix = appendDoeIconShellPrefix(arena);
        allocated = prefix.shell();
        throw Error(M2M_E_UNSUPPORTED, prefix.guardMessage()); // Reached source boundary only.
    } catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        guarded = true;
    }
    CHECK(guarded);
    REQUIRE(arena.size() == 1);
    CHECK(arena.at(0).shell() == allocated);
    CHECK(allocated->privateData == nullptr);
    CHECK(arena.at(0).request().has_value());
    CHECK(arena.at(0).nextBoundary() == DoeIconShellNextBoundary::AddFile);
}

TEST_CASE(doe_icon_shell_adapter_owner_has_no_back_edge_or_implicit_native_completion)
{
    std::weak_ptr<DoeIconShellArena> weakArena;
    {
        auto arena = std::make_shared<DoeIconShellArena>();
        weakArena = arena;
        const auto& prefix = appendDoeIconShellPrefix(*arena);
        REQUIRE(prefix.shell() != nullptr);
        CHECK(prefix.shell()->privateData == nullptr);
        CHECK(prefix.stage() == DoeIconShellStage::AddFileBoundary);
        // Only this adapter arena owns the payload; no Session/receiver or Qt
        // object was constructed. Dropping it is not native leak cleanup/Close.
        CHECK_EQ(arena.use_count(), 1);
    }
    CHECK(weakArena.expired());
}

int main() { return testkit::run_all(); }
