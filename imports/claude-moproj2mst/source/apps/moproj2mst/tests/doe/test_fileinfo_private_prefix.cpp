// ctest-labels: m2m-doe
// Partial adapter construction only; not a returned QFileInfo/addFile or Qt engine.
#include <testkit/check.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_fileinfo_private_prefix.hpp"

using namespace m2m;
using namespace m2m::doe;

TEST_CASE(doe_fileinfo_private_prefix_initializes_only_the_reference_word)
{
    DoeIconShellArena icons;
    const auto& icon = appendDoeIconShellPrefix(icons);
    DoeFileInfoPrivateArena arena;
    const auto& prefix = appendDoeFileInfoPrivatePrefix(arena, icon);
    CHECK_EQ(sizeof(DoeFileInfoPrivateValue), std::size_t(0xe0));
    CHECK_EQ(alignof(DoeFileInfoPrivateValue), std::size_t(8));
    REQUIRE(prefix.value() != nullptr);
    CHECK_EQ(reinterpret_cast<std::uintptr_t>(prefix.value()) % 8, std::uintptr_t(0));
    CHECK_EQ(prefix.value()->referenceWord(), std::uint32_t(0));
    CHECK_EQ(prefix.initializedPrefixBytes(), sizeof(std::uint32_t));
    // No tail bytes are read: path/cache/engine fields remain unavailable.
    CHECK(prefix.stage() == DoeFileInfoPrivateStage::ReferenceInitialized);
    CHECK(prefix.nextBoundary() == DoeFileInfoPrivateNextBoundary::QStringPathOwnership);
}

TEST_CASE(doe_fileinfo_private_prefix_stops_before_qstring_and_keeps_original_shell)
{
    DoeIconShellArena icons;
    const auto& icon = appendDoeIconShellPrefix(icons);
    const auto* shell = icon.shell();
    const auto* resourceData = icon.request()->resource.u16().data();
    DoeFileInfoPrivateArena arena;
    const auto& prefix = appendDoeFileInfoPrivatePrefix(arena, icon);
    CHECK(icon.shell() == shell);
    CHECK(shell->privateData == nullptr);
    CHECK(icon.request()->resource.u16().data() == resourceData);
    CHECK(icon.stage() == DoeIconShellStage::AddFileBoundary);
    CHECK_EQ(prefix.value()->referenceWord(), std::uint32_t(0)); // Not outer publication/ref1.
    CHECK(std::string(prefix.guardMessage()).find("QString path/shared-null") != std::string::npos);
    CHECK(std::string(prefix.guardMessage()).find("unsupported") != std::string::npos);
}

TEST_CASE(doe_fileinfo_private_prefix_has_distinct_stable_partial_allocations)
{
    DoeIconShellArena icons;
    const auto& icon = appendDoeIconShellPrefix(icons);
    DoeFileInfoPrivateArena arena;
    const auto* first = &appendDoeFileInfoPrivatePrefix(arena, icon);
    const auto* firstValue = first->value();
    const auto* second = &appendDoeFileInfoPrivatePrefix(arena, icon);
    REQUIRE(arena.size() == 2);
    CHECK(first != second);
    CHECK(firstValue != second->value());
    CHECK(&arena.at(0) == first);
    CHECK(&arena.at(1) == second);
    CHECK(first->value() == firstValue);
    CHECK_EQ(first->initializedPrefixBytes(), std::size_t(4));
    CHECK_EQ(second->initializedPrefixBytes(), std::size_t(4));
    // Two isolated admissions, not a claim that a native interrupted call retried.
}

TEST_CASE(doe_fileinfo_private_prefix_adapter_guard_retains_diagnostics_without_unwind_claim)
{
    DoeIconShellArena icons;
    const auto& icon = appendDoeIconShellPrefix(icons);
    DoeFileInfoPrivateArena arena;
    const DoeFileInfoPrivateValue* observed = nullptr;
    bool guarded = false;
    try {
        const auto& prefix = appendDoeFileInfoPrivatePrefix(arena, icon);
        observed = prefix.value();
        throw Error(M2M_E_UNSUPPORTED, prefix.guardMessage());
    } catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        guarded = true;
    }
    CHECK(guarded);
    REQUIRE(arena.size() == 1);
    CHECK(arena.at(0).value() == observed);
    CHECK_EQ(observed->referenceWord(), std::uint32_t(0));
    CHECK(icon.shell()->privateData == nullptr);
    // This retained adapter prefix does not replay native constructor exception
    // cleanup, which would free the actual raw0xe0 allocation before return.
}

TEST_CASE(doe_fileinfo_private_prefix_refuses_other_addfile_branches_before_allocation)
{
    DoeIconShellArena icons;
    const auto& icon = appendDoeIconShellPrefix(icons);
    DoeFileInfoPrivateArena arena;
    // Explicit diagnostic alteration of the mutable arena-owned value: an
    // existing private pointer takes addFile's detach branch, not QFileInfo.
    int privateIdentity = 0;
    auto* declaredShell = const_cast<DoeIconShellValue*>(icon.shell());
    declaredShell->privateData = &privateIdentity;
    bool refused = false;
    try { (void)appendDoeFileInfoPrivatePrefix(arena, icon); }
    catch (const Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK_EQ(arena.size(), std::size_t(0));
    declaredShell->privateData = nullptr;

    // Empty QString has its own earlier return. No fake empty suffix/private
    // object is produced by this scope-limited prefix.
    auto& declaredRequest = const_cast<DoeIconAddFileRequest&>(*icon.request());
    declaredRequest.resource = qt::String();
    refused = false;
    try { (void)appendDoeFileInfoPrivatePrefix(arena, icon); }
    catch (const Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK_EQ(arena.size(), std::size_t(0));
}

TEST_CASE(doe_fileinfo_private_prefix_owner_is_independent_of_icon_and_has_no_back_edge)
{
    auto arena = std::make_shared<DoeFileInfoPrivateArena>();
    std::weak_ptr<DoeFileInfoPrivateArena> weak = arena;
    {
        DoeIconShellArena icons;
        const auto& icon = appendDoeIconShellPrefix(icons);
        (void)appendDoeFileInfoPrivatePrefix(*arena, icon);
    }
    REQUIRE(arena->size() == 1);
    CHECK_EQ(arena->at(0).value()->referenceWord(), std::uint32_t(0));
    CHECK_EQ(arena.use_count(), 1);
    arena.reset(); // Adapter storage release, not native QFileInfo destruction.
    CHECK(weak.expired());
}

int main() { return testkit::run_all(); }
