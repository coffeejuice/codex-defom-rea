// ctest-labels: m2m-s
// Static expectations from original PE 0x141364140 and its Save caller, retained
// in analysis/resume48-distortion-vt198-rea.json. No fresh original execution or
// original-vs-converter parity is claimed. Invalid-state tests check port guards.
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <defmodel/window/inter_object_ops.hpp>

#include "m2m/data/distortion.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/engine/distortion_contacts.hpp"

using namespace m2m;

namespace {
struct Fixture {
    Operation op;
    std::shared_ptr<DistortionProps> props = std::make_shared<DistortionProps>();
    Fixture(const char16_t* key = u"SfOprDistortion3D")
    {
        op.classKey = String(key);
        props->data = std::make_shared<data::DistortionData>();
        op.props = props;
    }
};

void checkDefaults(const engine::ContactPair& pair)
{
    CHECK_EQ(pair.frc, 0.0);
    CHECK_EQ(pair.ihtc, 0.0);
    CHECK_EQ(pair.lub, 1);
    CHECK_EQ(pair.f19, 1);
}

bool unsupported(Operation& op)
{
    try {
        (void)engine::distortionDefaultPairs(op);
    } catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED;
    }
    return false;
}

// Bind a constructed model without database loading or unrelated viewer work.
struct ModelClass final : mst::OpClass {
    engine::GuiModel* model;
    ModelClass(const char16_t* key, engine::GuiModel& sourceModel)
        : OpClass(String(key)), model(&sourceModel) {}
    engine::GuiModel* classModel(Project&, Operation&) override { return model; }
};
}

TEST_CASE(distortion_contacts_preserve_fixture_order_in_both_dimensions)
{
    for (const auto* key : {u"SfOprDistortion2D", u"SfOprDistortion3D"}) {
        Fixture fixture(key);
        auto& data = *fixture.props->data;
        data.fixtureCount = 2;
        data.workpiece = 4;
        data.fixtures[0] = 7;
        data.fixtures[1] = 2;
        const auto pairs = engine::distortionDefaultPairs(fixture.op);
        REQUIRE(pairs.size() == 2);
        CHECK_EQ(pairs[0].master, 7);
        CHECK_EQ(pairs[1].master, 2);
        for (const auto& pair : pairs) {
            CHECK_EQ(pair.slave, 4);
            checkDefaults(pair);
        }
    }
}

TEST_CASE(distortion_contacts_keep_duplicate_self_and_missing_fixture_ids)
{
    Fixture fixture;
    auto& data = *fixture.props->data;
    data.fixtureCount = 4;
    data.workpiece = 1;
    data.fixtures[0] = 1;
    data.fixtures[1] = 0;
    data.fixtures[2] = -7;
    data.fixtures[3] = 1;
    const auto pairs = engine::distortionDefaultPairs(fixture.op);
    REQUIRE(pairs.size() == 4);
    const int expected[] = {1, 0, -7, 1};
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        CHECK_EQ(pairs[i].master, expected[i]);
        CHECK_EQ(pairs[i].slave, 1);
        checkDefaults(pairs[i]);
    }
}

TEST_CASE(distortion_contacts_empty_gates_require_valid_data)
{
    Fixture fixture;
    auto& data = *fixture.props->data;
    data.fixtureCount = 2;
    data.fixtures[0] = 3;
    data.fixtures[1] = 2;
    for (const int workpiece : {0, -1}) {
        data.workpiece = workpiece;
        CHECK(engine::distortionDefaultPairs(fixture.op).empty());
    }
    data.workpiece = 1;
    data.fixtureCount = 0;
    CHECK(engine::distortionDefaultPairs(fixture.op).empty());
}

TEST_CASE(distortion_contacts_use_the_full_recovered_fixture_array)
{
    Fixture fixture;
    auto& data = *fixture.props->data;
    data.fixtureCount = 100;
    data.workpiece = 5;
    for (std::size_t i = 0; i < data.fixtures.size(); ++i)
        data.fixtures[i] = static_cast<int>(200 - i);
    const auto pairs = engine::distortionDefaultPairs(fixture.op);
    REQUIRE(pairs.size() == 100);
    CHECK_EQ(pairs.front().master, 200);
    CHECK_EQ(pairs.back().master, 101);
    CHECK_EQ(pairs.back().slave, 5);
    checkDefaults(pairs.back());
}

TEST_CASE(distortion_contacts_preserve_invalid_property_and_count_guards)
{
    Fixture fixture;
    fixture.op.props.reset();
    CHECK(unsupported(fixture.op));
    fixture.op.props = std::make_shared<Props>();
    CHECK(unsupported(fixture.op));
    fixture.op.props = fixture.props;
    fixture.props->data.reset();
    CHECK(unsupported(fixture.op));
    fixture.props->data = std::make_shared<data::DistortionData>();
    for (const int count : {-1, 101}) {
        fixture.props->data->fixtureCount = count;
        CHECK(unsupported(fixture.op));
    }
}

TEST_CASE(distortion_contacts_reach_system_rebuild_and_mirrored_records)
{
    for (const int dimension : {2, 3}) {
        const auto* key = dimension == 2 ? u"SfOprDistortion2D" : u"SfOprDistortion3D";
        Fixture fixture(key);
        Project project;
        fixture.op.project = &project;
        project.path = String((m2m_test::scratchRoot() / "distortion.moproj").u16string());
        fixture.op.taskDir = qt::L("Operations/Task00001");
        fixture.op.engineModel = std::make_shared<engine::GuiModel>(nullptr, dimension);
        auto& model = *fixture.op.engineModel;
        auto owner = std::make_unique<ModelClass>(key, model);
        auto& cls = *owner;
        cls.current = &fixture.op;
        fixture.op.opClass = &cls;
        project.registry.emplace(cls.key(), std::move(owner));
        fixture.op.lubSystem = true;
        auto& data = *fixture.props->data;
        data.fixtureCount = 2;
        data.workpiece = 1;
        data.fixtures[0] = 3;
        data.fixtures[1] = 2;
        engine::FileEffects effects;
        REQUIRE(engine::syncOperationFiles(project, fixture.op, effects));
        REQUIRE(model.step != nullptr);
        CHECK_EQ(def::model::pair_count(&model.step->inter_obj), 4);
        const int masters[] = {3, 1, 2, 1};
        const int slaves[] = {1, 3, 1, 2};
        for (int i = 0; i < 4; ++i) {
            const auto* pair = def::model::pair_nth(&model.step->inter_obj, i + 1);
            REQUIRE(pair != nullptr);
            CHECK_EQ(*engine::at<std::int32_t>(pair, 0x168), masters[i]);
            CHECK_EQ(*engine::at<std::int32_t>(pair, 0x16c), slaves[i]);
            CHECK_EQ(*engine::at<std::int32_t>(pair, 0x170), i % 2 == 0 ? 2 : 1);
            CHECK_EQ(*engine::at<double>(pair, 0x20), 0.0);
            CHECK_EQ(*engine::at<double>(pair, 0x1c0), 0.0);
            if (dimension == 3) CHECK_EQ(*engine::at<std::uint32_t>(pair, 0x174), 1U);
        }
        cls.current = nullptr;
    }
}

int main() { return testkit::run_all(); }
