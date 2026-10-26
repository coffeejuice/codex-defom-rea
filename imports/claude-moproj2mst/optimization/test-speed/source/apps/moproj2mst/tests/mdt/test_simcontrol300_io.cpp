// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <map>
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/engine.hpp"
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj/llobj_parts.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;

namespace {
struct Output {
    std::string bytes;
    bool open = true, write = true;
    int closes = 0;
};
struct Device final : mst::OutputDevice {
    explicit Device(std::shared_ptr<Output> value) : value(std::move(value)) {}
    std::shared_ptr<Output> value;
    std::int64_t write(std::string_view bytes) override
    {
        if (!value->write) return -1;
        value->bytes.append(bytes); return static_cast<std::int64_t>(bytes.size());
    }
    bool flush() override { return true; }
    void close() override { notifyAboutToClose(); ++value->closes; }
    bool opened() const override { return value->open && value->closes == 0; }
};
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    std::map<std::string, std::vector<double>> doubleFiles;
    std::map<std::string, std::vector<int>> intFiles;
    std::map<std::string, std::shared_ptr<Output>> outputs;
    std::vector<std::string> calls;
    bool removeSucceeds = true, writesOpen = true, writesSucceed = true;
    bool springOpened = false;
    std::vector<SpringRecord> springs;
    bool readDoubles(const qt::String& path, std::span<double> output) override
    {
        const auto name = path.utf8(); calls.push_back("double:" + name + ":" + std::to_string(output.size()));
        std::fill(output.begin(), output.end(), 0.0);
        const auto found = doubleFiles.find(name); if (found == doubleFiles.end()) return false;
        std::copy_n(found->second.begin(), (std::min)(output.size(), found->second.size()), output.begin());
        return true;
    }
    bool readInts(const qt::String& path, std::span<int> output) override
    {
        const auto name = path.utf8(); calls.push_back("int:" + name + ":" + std::to_string(output.size()));
        std::fill(output.begin(), output.end(), 0);
        const auto found = intFiles.find(name); if (found == intFiles.end()) return false;
        std::copy_n(found->second.begin(), (std::min)(output.size(), found->second.size()), output.begin());
        return true;
    }
    bool readSprings(std::vector<SpringRecord>& output) override
    {
        calls.push_back("springs"); output = springs; return springOpened;
    }
    bool removeIfReadable(const qt::String& path) override
    {
        calls.push_back("remove:" + path.utf8()); return removeSucceeds;
    }
    void nbcRemoveWarning() override { calls.push_back("warning:NBC"); }
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String& path) override
    {
        const auto name = path.utf8(); calls.push_back("write:" + name);
        auto value = std::make_shared<Output>(); value->open = writesOpen; value->write = writesSucceed;
        outputs[name] = value; return std::make_unique<Device>(value);
    }
};
}

TEST_CASE(extra3d_missing_files_keep_defaults_and_delete_in_original_order)
{
    Io io;
    auto form = fillExtra3D(io, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(!form->nbcEnabled && !form->switchToSparseEnabled && !form->stretchEnabled);
    CHECK(!form->weightEnabled && !form->strainDistributionEnabled);
    CHECK(form->nbc[0] == qt::L("75.0")); CHECK(form->switchToSparse == qt::L("140000"));
    CHECK(form->stretchKind == qt::L("1"));
    CHECK((io.calls == std::vector<std::string>{"double:NBC.DAT:2", "int:SW2SP.DAT:1",
        "int:STRETCH.DAT:1", "double:B_WEIGHT.DAT:1", "double:STRAIN_DST.DAT:0", "springs"}));
    constructorRefreshExtra3D(*form);
    CHECK(form->nbc[0] == qt::L("75")); CHECK(form->nbc[1] == qt::L("105"));
    io.calls.clear(); io.removeSucceeds = false;
    CHECK(applyExtra3D(io, *form) == ApplyResult::Completed);
    CHECK((io.calls == std::vector<std::string>{"remove:NBC.DAT", "warning:NBC", "remove:SW2SP.DAT",
        "remove:STRETCH.DAT", "remove:B_WEIGHT.DAT", "remove:STRAIN_DST.DAT",
        "remove:SPRING.INI", "remove:SPRING2.INI"}));
}

TEST_CASE(extra3d_readable_empty_files_enable_controls_and_write_literal_tabs_crlf)
{
    Io io;
    io.doubleFiles = {{"NBC.DAT", {}}, {"STRETCH.DAT", {}}, {"B_WEIGHT.DAT", {}}, {"STRAIN_DST.DAT", {}}};
    io.intFiles = {{"SW2SP.DAT", {}}, {"STRETCH.DAT", {}}};
    auto form = fillExtra3D(io, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(form->nbcEnabled && form->switchToSparseEnabled && form->stretchEnabled);
    CHECK(form->weightEnabled && form->strainDistributionEnabled);
    CHECK(form->nbc[0] == qt::L("0")); CHECK(form->switchToSparse == qt::L("0"));
    constructorRefreshExtra3D(*form);
    CHECK(applyExtra3D(io, *form) == ApplyResult::Completed);
    CHECK(io.outputs["NBC.DAT"]->bytes == "75\t\t\t105\t\r\n");
    CHECK(io.outputs["SW2SP.DAT"]->bytes == "0\t\r\n");
    CHECK(io.outputs["STRETCH.DAT"]->bytes == "0\t\t\t0\t\t\t0\t\r\n");
    CHECK(io.outputs["B_WEIGHT.DAT"]->bytes == "0\t\r\n");
    CHECK(io.outputs["STRAIN_DST.DAT"]->bytes == "\r\n");
    for (const auto& [name, output] : io.outputs) CHECK_EQ(output->closes, 1);
}

TEST_CASE(extra3d_scalar_snapshot_remembers_read_values_and_stretch_reopen_failure)
{
    Io io;
    io.doubleFiles = {{"NBC.DAT", {1.2345678, 2.3456789}}, {"B_WEIGHT.DAT", {3.4567891}}};
    io.intFiles = {{"SW2SP.DAT", {123456}}, {"STRETCH.DAT", {7}}};
    auto form = fillExtra3D(io, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(form->nbc[0] == qt::L("1.23457")); CHECK(form->weight == qt::L("3.45679"));
    CHECK(form->stretchEnabled); CHECK(form->stretchKind == qt::L("7"));
    CHECK(form->stretch[0] == qt::L("0") && form->stretch[1] == qt::L("0"));
    io.doubleFiles.clear(); io.intFiles.clear();
    CHECK(applyExtra3D(io, *form) == ApplyResult::Completed);
    CHECK(io.outputs["NBC.DAT"]->bytes == "1.23457\t\t\t2.34568\t\r\n");
    CHECK(io.outputs["STRETCH.DAT"]->bytes == "7\t\t\t0\t\t\t0\t\r\n");
    io.intFiles["STRETCH.DAT"] = {9}; io.doubleFiles["STRETCH.DAT"] = {9.0, 1.2345678, 2.3456789};
    auto populated = fillExtra3D(io, NumericLocale::KnownC); REQUIRE(populated.has_value());
    CHECK(populated->stretch[0] == qt::L("1.23457")); CHECK(populated->stretch[1] == qt::L("2.34568"));
}

TEST_CASE(extra3d_open_and_later_write_failures_continue_without_rollback)
{
    for (bool open : {false, true}) {
        Io io; io.doubleFiles = {{"NBC.DAT", {}}, {"B_WEIGHT.DAT", {}}, {"STRAIN_DST.DAT", {}}};
        io.intFiles = {{"SW2SP.DAT", {}}, {"STRETCH.DAT", {}}};
        auto form = fillExtra3D(io, NumericLocale::KnownC); REQUIRE(form.has_value());
        io.calls.clear(); io.writesOpen = open; io.writesSucceed = false;
        CHECK(applyExtra3D(io, *form) == ApplyResult::Completed);
        CHECK((io.calls == std::vector<std::string>{"write:NBC.DAT", "write:SW2SP.DAT", "write:STRETCH.DAT",
            "write:B_WEIGHT.DAT", "write:STRAIN_DST.DAT", "remove:SPRING.INI", "remove:SPRING2.INI"}));
        CHECK_EQ(io.outputs["NBC.DAT"]->closes, open ? 1 : 0);
        CHECK(io.outputs["NBC.DAT"]->bytes.empty());
    }
    Io io;
    CHECK(io.writeStringList(qt::L("literal"), {qt::L("a"), qt::L("\n"), qt::L("b")}));
    CHECK(io.outputs["literal"]->bytes == "a\t\nb\t\r\n");
}

TEST_CASE(extra3d_unknown_locale_and_populated_spring_editor_are_explicit_boundaries)
{
    Io io; CHECK(!fillExtra3D(io, NumericLocale::Unrecovered)); CHECK(io.calls.empty());
    io.springOpened = true; io.springs.push_back({1, 0.0, 0.0, 2, {0.0, 0.0, 0.0}, 0});
    CHECK(!fillExtra3D(io, NumericLocale::KnownC)); CHECK(io.outputs.empty());
    io.springOpened = false; // failed original spring parse does not create items
    CHECK(fillExtra3D(io, NumericLocale::KnownC).has_value());
}

TEST_CASE(empty_recovery_tail_continues_after_validation_stop_and_uses_timeid_width)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false);
    auto& nodes = def::model::llobj::parts::part_8a0(*object);
    auto& elements = def::model::llobj::parts::part_9e0(*object);
    nodes.alloc(1, model.step->u15f8); elements.alloc(1, model.step->u15f8, 4);
    auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    sc.timeid.n[0] = 1; sc.erecid.n[0] = 2;
    sc.nrecid.ndim = 1; sc.nrecid.n[0] = 1; sc.nrecid.ncomp = 2;
    def::model::dynary_alloc(&sc.nrecid);
    REQUIRE(sc.nrecid.axis[0] != nullptr); REQUIRE(sc.nrecid.values != nullptr);
    form->time[0] = {qt::L("bad"), ValidatorState::Invalid};
    std::vector<RecoveryEntry> empty;
    CHECK(applyEmptyRecoveryTail(sc, model, *form, &empty, &empty) == ApplyResult::ValidationStopped);
    CHECK_EQ(sc.nrecid.n[0], 0); CHECK_EQ(sc.nrecid.ncomp, 0);
    CHECK(sc.nrecid.axis[0] == nullptr && sc.nrecid.values == nullptr);
    CHECK_EQ(sc.timeid.n[0], 1); CHECK_EQ(sc.erecid.n[0], 2);
    CHECK_EQ(model.step->u15f8[(0x1680 - 0x15f8) / 4], 1);
    CHECK_EQ(model.step->u15f8[(0x16f4 - 0x15f8) / 4], 2);
    auto* timerc = *reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&nodes) + 0xb8);
    auto* erecvl = *reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&elements) + 0xd8);
    REQUIRE(timerc != nullptr); REQUIRE(erecvl != nullptr);
    CHECK(timerc[0] == 0.0); CHECK(erecvl[0] == 0.0 && erecvl[1] == 0.0);
    // We supplied only the original shape fields read by this tail; no arrays
    // were allocated for TIMEID/ERECID, so restore their unused counts for dtor.
    sc.timeid.n[0] = 0; sc.erecid.n[0] = 0;
}

TEST_CASE(nonempty_recovery_lists_refuse_before_mutation_without_guessing_layout)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    std::vector<RecoveryEntry> empty, populated(1);
    sc.nrecid.n[0] = 4; sc.nrecid.ncomp = 7;
    CHECK(applyEmptyRecoveryTail(sc, model, *form, &populated, &empty) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(sc.nrecid.n[0], 4); CHECK_EQ(sc.nrecid.ncomp, 7);
    CHECK(applyEmptyRecoveryTail(sc, model, *form, nullptr, &empty) == ApplyResult::UnsupportedSnapshot);
    sc.nrecid.n[0] = 0;
}

int main() { return testkit::run_all(); }
