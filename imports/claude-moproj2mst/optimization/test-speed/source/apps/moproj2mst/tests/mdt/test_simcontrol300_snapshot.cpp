// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <cstring>
#include <bit>
#include <cmath>
#include <limits>
#include <utility>
#include <algorithm>
#include <initializer_list>
#include <string>
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/llobj/llobj_parts.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/layout/state_var_base.hpp>
#include <defmodel/bc/boundary_topology.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;

namespace {
GeneralContext generalContext()
{
    return {1, 3, {true, true, true, true, true, true}, false, true};
}
void initialGeneral(SimControl& sc)
{
    std::strcpy(sc.title, "REMEMBERED TITLE");
    std::strcpy(sc.oprnam, "REMEMBERED OPERATION");
    sc.meshno = 30000; sc.unit = 2; sc.stype = 7;
    sc.trans[0] = 1; sc.trans[1] = 1; sc.trans[2] = 2;
    sc.trans[3] = 1; sc.trans[4] = 0; sc.htmthd = 2;
}
void initialIteration(SimControl& sc)
{
    sc.solmtd = 2; sc.u2e8 = 4;
    sc.solmtd_p[0] = 1.9; sc.solmtd_p[1] = 2.9;
    sc.solmtd_p[2] = -3.9; sc.solmtd_p[3] = 5.9;
    sc.itrmth = 2; sc.solmtt = 4;
    sc.cvgerr[0] = 0.12345678; sc.cvgerr[1] = 0.87654321;
    sc.defbwd = 9; sc.tmpbwd = 0; sc.itrmxd = 312; sc.itrmxt = -11;
}
void initialStepSize(SimControl& sc)
{
    sc.stpdef = 1; sc.dttype.u58 = 0;
    sc.dtmax1 = 0.12345678; sc.dsmax = 1.2345678;
    sc.demax = 2.3456789; sc.dplen = 3.4567891; sc.dtsub = 4.5678912;
    sc.dtpmax1 = 5.6789123; sc.dtpmax2_5[0] = 6.7891234;
    sc.dtpmax2_5[1] = 7.8912345; sc.slderr = 8.9123456;
    sc.dttype.value = 9.1234567; sc.dttype.u54 = 33;
}
template <class T> T& field(def::model::LLOBJ_DATA* object, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(object) + offset);
}
void coordinateReference(def::model::LLOBJ_DATA* object)
{
    field<int>(object, 0x1ec8) = 1;
    field<int>(object, 0x1ee8) = 73;
    field<int>(object, 0x87c) = 0;
    field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = -0.0001;
    field<double>(object, 0x1ee0) = -0.0;
}
void timeIdTable(def::model::DYNARY& table, std::initializer_list<double> ids,
    int components, std::initializer_list<double> values)
{
    def::model::dynary_free_arrays(&table);
    table.ndim = 1; table.n[0] = static_cast<int>(ids.size());
    table.n[1] = table.n[2] = 0; table.ncomp = components;
    REQUIRE(values.size() == ids.size() * components);
    def::model::dynary_alloc(&table);
    REQUIRE(table.axis[0]); REQUIRE(table.values);
    std::copy(ids.begin(), ids.end(), table.axis[0]);
    std::copy(values.begin(), values.end(), table.values);
}
}

TEST_CASE(general_applies_remembered_controls_and_explicit_enable_state)
{
    SimControl sc;
    initialGeneral(sc);
    auto context = generalContext();
    context.processEnabled[3] = false;
    const auto form = fillGeneral(sc, context); REQUIRE(form.has_value());
    CHECK_EQ(sc.smode, 3); // fill itself recomputes SMODE
    std::strcpy(sc.title, "LATEST MODEL"); std::strcpy(sc.oprnam, "LATEST OPERATION");
    sc.meshno = 42; sc.unit = 1; sc.stype = 1;
    for (int& flag : sc.trans) flag = 0;
    sc.htmthd = 0;
    REQUIRE(applyGeneral(sc, *form));
    CHECK(std::strcmp(sc.title, "REMEMBERED TITLE") == 0);
    CHECK(std::strcmp(sc.oprnam, "REMEMBERED OPERATION") == 0);
    CHECK_EQ(sc.meshno, 9999); CHECK_EQ(sc.unit, 2); CHECK_EQ(sc.stype, 7);
    CHECK_EQ(sc.trans[0], 1); CHECK_EQ(sc.trans[1], 1); CHECK_EQ(sc.trans[2], 2);
    CHECK_EQ(sc.trans[3], 0); CHECK_EQ(sc.trans[4], 0); CHECK_EQ(sc.htmthd, 1);
    CHECK_EQ(sc.smode, 3);
}

TEST_CASE(general_name_limit_clips_widgets_at_fill_and_model_only_at_save)
{
    const std::size_t lengths[]{0, 19, 20, 21, 64};
    for (const int applicationMode : {1, 5}) {
        Project project;
        auto context = generalContext();
        context.applicationMode = applicationMode;
        context.moProject = applicationMode == 5 ? &project : nullptr;
        for (const auto length : lengths) {
            SimControl sc; initialGeneral(sc);
            std::string original;
            for (std::size_t i = 0; i < length; ++i)
                original.push_back(static_cast<char>('A' + i % 26));
            const auto expected = original.substr(0, 20);
            const std::string title(80, 'T'); // below the original title limit256
            std::strcpy(sc.oprnam, original.c_str());
            std::strcpy(sc.title, title.c_str());
            const auto form = fillGeneral(sc, context); REQUIRE(form);
            CHECK(form->operationName == qt::String::latin1(expected));
            CHECK(!form->operationName.isNull()); // empty line-edit text() is not null
            CHECK(form->title == qt::String::latin1(title));
            CHECK(std::string(sc.oprnam) == original);
            CHECK(std::string(sc.title) == title);
            REQUIRE(applyGeneral(sc, *form));
            CHECK(std::string(sc.oprnam) == expected);
            CHECK(std::string(sc.title) == title);
        }
    }
}

TEST_CASE(general_save_uses_the_remembered_truncated_name)
{
    SimControl sc; initialGeneral(sc);
    const std::string original = "ABCDEFGHIJKLMNOPQRST-extra-original-name";
    std::strcpy(sc.oprnam, original.c_str());
    const auto form = fillGeneral(sc, generalContext()); REQUIRE(form);
    CHECK(std::string(sc.oprnam) == original);
    std::strcpy(sc.oprnam, "LATEST MODEL NAME");
    REQUIRE(applyGeneral(sc, *form));
    CHECK(std::string(sc.oprnam) == "ABCDEFGHIJKLMNOPQRST");
    CHECK(form->operationName == qt::L("ABCDEFGHIJKLMNOPQRST"));
}

TEST_CASE(general_preserved_process_branch_and_mesh_visibility)
{
    SimControl sc; initialGeneral(sc);
    auto context = generalContext();
    context.preserveProcessFlags = true; context.meshNumberEnabled = false;
    const auto form = fillGeneral(sc, context); REQUIRE(form.has_value());
    sc.meshno = 11; sc.trans[0] = 0; sc.trans[1] = 8; sc.trans[2] = 9;
    sc.trans[3] = 7; sc.htmthd = 3;
    REQUIRE(applyGeneral(sc, *form));
    CHECK_EQ(sc.meshno, 11); CHECK_EQ(sc.trans[0], 0); CHECK_EQ(sc.trans[1], 8);
    CHECK_EQ(sc.trans[2], 2); CHECK_EQ(sc.trans[3], 7); CHECK_EQ(sc.htmthd, 1);
    CHECK_EQ(sc.smode, 1);
    sc.meshno = -1;
    const auto low = fillGeneral(sc, generalContext()); REQUIRE(low.has_value());
    CHECK_EQ(low->meshNumber, 1);
}

TEST_CASE(general_rejects_unrecovered_radio_dimension_mode_and_codec_branches)
{
    SimControl sc; initialGeneral(sc); sc.smode = 99;
    auto context = generalContext(); context.dimension = 2;
    CHECK(!fillGeneral(sc, context)); CHECK_EQ(sc.smode, 99);
    context = generalContext(); context.applicationMode = 5;
    CHECK(!fillGeneral(sc, context));
    sc.stype = 2; CHECK(!fillGeneral(sc, generalContext()));
    sc.stype = 1; sc.title[0] = static_cast<char>(0x80);
    CHECK(!fillGeneral(sc, generalContext()));
}

TEST_CASE(step_count_exact_spin_clamps_and_remembered_primary_die)
{
    SimControl sc;
    sc.nstart = 7; sc.nstep = -3; sc.stpinc = 0; sc.pdie1 = 5;
    const auto form = fillStepCount(sc, {1, {2, 5, 8}}); REQUIRE(form.has_value());
    CHECK_EQ(form->start, -8); CHECK_EQ(form->steps, 0); CHECK_EQ(form->increment, 1);
    sc.nstart = -55; sc.nstep = 900; sc.stpinc = 90; sc.pdie1 = 8;
    applyStepCount(sc, *form);
    CHECK_EQ(sc.nstart, -8); CHECK_EQ(sc.nstep, 0); CHECK_EQ(sc.stpinc, 1);
    CHECK_EQ(sc.pdie1, 5);
    sc.nstart = (std::numeric_limits<int>::max)();
    sc.nstep = 1000001; sc.stpinc = 1000001; sc.pdie1 = 20;
    const auto limits = fillStepCount(sc, {1, {2, 5, 8}}); REQUIRE(limits.has_value());
    CHECK_EQ(limits->start, -999999); CHECK_EQ(limits->steps, 999999);
    CHECK_EQ(limits->increment, 999999); CHECK_EQ(limits->primaryDie, 0);
    sc.nstart = (std::numeric_limits<int>::min)();
    const auto negative = fillStepCount(sc, {1, {}}); REQUIRE(negative.has_value());
    CHECK_EQ(negative->start, -999999);
    CHECK(!fillStepCount(sc, {5, {}}));
}

TEST_CASE(iteration_remembers_cg_text_combo_and_validation_state)
{
    SimControl sc; initialIteration(sc);
    const auto form = fillIteration3D(sc, 3, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(sc.solmtd_p[0] == 1.9); CHECK(sc.solmtd_p[1] == 2.9);
    CHECK(form->cgSecond == qt::L("2")); CHECK(form->cgThird == qt::L("-3"));
    CHECK(form->convergence[0].text == qt::L("0.123457"));
    sc.solmtd = 0; sc.u2e8 = 0; sc.itrmth = 0; sc.solmtt = 0;
    for (double& value : sc.solmtd_p) value = 123.0;
    sc.cvgerr[0] = 9.0; sc.cvgerr[1] = 9.0;
    sc.defbwd = 0; sc.tmpbwd = 1; sc.itrmxd = 1; sc.itrmxt = 1;
    CHECK(applyIteration3D(sc, *form) == ApplyResult::Completed);
    CHECK_EQ(sc.solmtd, 2); CHECK_EQ(sc.u2e8, 4); CHECK_EQ(sc.itrmth, 2); CHECK_EQ(sc.solmtt, 4);
    CHECK(sc.solmtd_p[0] == 1.0); CHECK(sc.solmtd_p[1] == 2.0);
    CHECK(sc.solmtd_p[2] == -3.0); CHECK(sc.solmtd_p[3] == 5.0);
    CHECK(sc.cvgerr[0] == 0.123457); CHECK(sc.cvgerr[1] == 0.876543);
    CHECK_EQ(sc.defbwd, 1); CHECK_EQ(sc.tmpbwd, 0); CHECK_EQ(sc.itrmxd, 312); CHECK_EQ(sc.itrmxt, -11);
}

TEST_CASE(iteration_invalid_convergence_preserves_original_partial_write_order)
{
    SimControl sc; initialIteration(sc);
    auto form = fillIteration3D(sc, 3, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->convergence[0] = {qt::L("invalid"), ValidatorState::Invalid};
    sc.solmtd = 1; sc.u2e8 = 99; sc.itrmth = 0; sc.solmtt = 0;
    sc.cvgerr[0] = 7.0; sc.cvgerr[1] = 8.0; sc.defbwd = 42; sc.itrmxd = 43;
    CHECK(applyIteration3D(sc, *form) == ApplyResult::ValidationStopped);
    CHECK_EQ(sc.solmtd, 2); CHECK_EQ(sc.u2e8, 4); CHECK_EQ(sc.itrmth, 2); CHECK_EQ(sc.solmtt, 4);
    CHECK(sc.solmtd_p[1] == 2.0); CHECK(sc.cvgerr[0] == 7.0); CHECK(sc.cvgerr[1] == 8.0);
    CHECK_EQ(sc.defbwd, 42); CHECK_EQ(sc.itrmxd, 43);
    form->convergence[0] = {qt::L("0.25"), ValidatorState::Acceptable};
    form->convergence[1] = {qt::L("invalid"), ValidatorState::Invalid};
    CHECK(applyIteration3D(sc, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.cvgerr[0] == 0.25); CHECK(sc.cvgerr[1] == 8.0); CHECK_EQ(sc.defbwd, 42);
}

TEST_CASE(iteration_intermediate_is_not_invalid_and_integer_invalid_defaults200)
{
    SimControl sc; initialIteration(sc);
    auto form = fillIteration3D(sc, 3, NumericLocale::KnownC); REQUIRE(form.has_value());
    // Validator results are supplied explicitly. This test does not claim that
    // this module recovers which arbitrary strings Qt classifies Intermediate.
    form->convergence[0] = {qt::L(""), ValidatorState::Intermediate};
    form->deformationIterations = {qt::L("bad"), ValidatorState::Invalid};
    form->thermalIterations = {qt::L(""), ValidatorState::Intermediate};
    CHECK(applyIteration3D(sc, *form) == ApplyResult::Completed);
    CHECK(sc.cvgerr[0] == 0.0); CHECK_EQ(sc.itrmxd, 200); CHECK_EQ(sc.itrmxt, 0);
}

TEST_CASE(iteration_guards_are_explicit_and_other_solver_parameters_stay_unwritten)
{
    SimControl sc; initialIteration(sc);
    sc.u2e8 = 0;
    CHECK(!fillIteration3D(sc, 3, NumericLocale::Unrecovered)); CHECK_EQ(sc.u2e8, 0);
    CHECK(!fillIteration3D(sc, 2, NumericLocale::KnownC)); CHECK_EQ(sc.u2e8, 0);
    sc.cvgerr[0] = (std::numeric_limits<double>::quiet_NaN)();
    CHECK(!fillIteration3D(sc, 3, NumericLocale::KnownC)); CHECK_EQ(sc.u2e8, 0);
    initialIteration(sc); sc.solmtd = 6; CHECK(!fillIteration3D(sc, 3, NumericLocale::KnownC));
    sc.solmtd = 3; CHECK(!fillIteration3D(sc, 3, NumericLocale::KnownC));
    for (int solver : {0, 1, 4, 5}) {
        initialIteration(sc); sc.solmtd = solver;
        const auto form = fillIteration3D(sc, 3, NumericLocale::KnownC); REQUIRE(form.has_value());
        sc.solmtd_p[0] = 77.0;
        CHECK(applyIteration3D(sc, *form) == ApplyResult::Completed);
        CHECK_EQ(sc.solmtd, solver); CHECK_EQ(sc.u2e8, 0); CHECK(sc.solmtd_p[0] == 77.0);
    }
    static_assert(!InitialEditorSnapshot::completeForPublicSave);
    static_assert(constructorRepeatFillOrder[0] == Page::Stopping);
    static_assert(constructorRepeatFillOrder[1] == Page::Meshing);
    static_assert(LifecycleContract::rotationReselectedAtSave);
    static_assert(LifecycleContract::rotationFallbackObject == 1);
}

TEST_CASE(step_size_preserves_temporary_curve_and_remembered_text)
{
    engine::GuiModel model(nullptr, 3);
    auto& sc = *model.simCtl(); initialStepSize(sc);
    const auto form = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(form->curve.get() != &sc.dttype);
    sc.dsmax = 99.0; sc.dtmax1 = 99.0; sc.dttype.value = 100.0; sc.dttype.u54 = 100;
    sc.dttype.u58 = 1; sc.stpdef = 2; sc.demax = 99.0;
    CHECK(applyStepSize3D(sc, *form) == ApplyResult::Completed);
    CHECK_EQ(sc.dttype.u58, 0); CHECK_EQ(sc.stpdef, 1);
    CHECK(sc.dsmax == 1.23457); CHECK(sc.dtmax1 == 0.0); CHECK(sc.demax == 2.34568);
    CHECK(sc.dplen == 3.45679); CHECK(sc.dtsub == 4.56789);
    CHECK(sc.dtpmax1 == 5.67891); CHECK(sc.dtpmax2_5[0] == 6.78912);
    CHECK(sc.dtpmax2_5[1] == 7.89123); CHECK(sc.slderr == 8.91235);
    // The remembered curve constant is copied exactly; it is not text-rounded.
    CHECK(sc.dttype.value == 9.1234567); CHECK_EQ(sc.dttype.u54, 33);
}

TEST_CASE(step_size_time_and_variable_modes_follow_original_overwrites)
{
    engine::GuiModel model(nullptr, 3);
    auto& sc = *model.simCtl(); initialStepSize(sc);
    sc.dttype.u58 = 1; sc.stpdef = 2;
    const auto time = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(time.has_value());
    CHECK(applyStepSize3D(sc, *time) == ApplyResult::Completed);
    CHECK(sc.dtmax1 == 0.123457); CHECK(sc.dsmax == 0.0); CHECK_EQ(sc.stpdef, 2);
    initialStepSize(sc); sc.stpdef = 3; // mode0 becomes1 during Open callback
    auto variable = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(variable.has_value());
    CHECK_EQ(sc.dttype.u58, 1); CHECK_EQ(variable->modeRadio, 2);
    variable->variableTimeMaximum = {qt::L("8.25"), ValidatorState::Acceptable};
    variable->variableMaximumChanges[1] = {qt::L("9.5"), ValidatorState::Acceptable};
    CHECK(applyStepSize3D(sc, *variable) == ApplyResult::Completed);
    CHECK_EQ(sc.dttype.u58, 1); CHECK_EQ(sc.stpdef, 3);
    CHECK(sc.dtmax1 == 8.25); CHECK(sc.dsmax == 0.0); CHECK(sc.dtpmax2_5[0] == 9.5);
}

TEST_CASE(step_size_partial_validation_keeps_prior_stores_and_defers_curve_copy)
{
    engine::GuiModel model(nullptr, 3);
    auto& sc = *model.simCtl(); initialStepSize(sc);
    auto form = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->displacementMaximum = {qt::L("bad"), ValidatorState::Invalid};
    sc.dttype.u58 = 1; sc.dsmax = 55.0; sc.dtmax1 = 44.0; sc.stpdef = 2;
    CHECK(applyStepSize3D(sc, *form) == ApplyResult::ValidationStopped);
    CHECK_EQ(sc.dttype.u58, 0); CHECK(sc.dsmax == 55.0); CHECK(sc.dtmax1 == 44.0);
    CHECK_EQ(sc.stpdef, 2);
    form->displacementMaximum = {qt::L(""), ValidatorState::Intermediate};
    form->maximumChanges[1] = {qt::L("bad"), ValidatorState::Invalid};
    sc.dtpmax2_5[0] = 66.0; sc.dttype.value = 111.0;
    CHECK(applyStepSize3D(sc, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.dsmax == 0.0); CHECK(sc.dtmax1 == 0.0); CHECK_EQ(sc.stpdef, 1);
    CHECK(sc.dtpmax1 == 5.67891); CHECK(sc.dtpmax2_5[0] == 66.0);
    CHECK(sc.dttype.value == 111.0); // final copy did not run
}

TEST_CASE(step_size_profile_guards_do_not_mutate_rejected_input)
{
    engine::GuiModel model(nullptr, 3);
    auto& sc = *model.simCtl(); initialStepSize(sc); sc.stpdef = 3;
    CHECK(!fillStepSize3D(sc, 2, NumericLocale::KnownC)); CHECK_EQ(sc.dttype.u58, 0);
    CHECK(!fillStepSize3D(sc, 3, NumericLocale::Unrecovered)); CHECK_EQ(sc.dttype.u58, 0);
    sc.dttype.ndim = 1;
    CHECK(!fillStepSize3D(sc, 3, NumericLocale::KnownC)); CHECK_EQ(sc.dttype.u58, 0);
    sc.dttype.ndim = 0; sc.dttype.u58 = 8;
    CHECK(!fillStepSize3D(sc, 3, NumericLocale::KnownC)); CHECK_EQ(sc.dttype.u58, 8);
    sc.dttype.u58 = 0; sc.demax = (std::numeric_limits<double>::infinity)();
    CHECK(!fillStepSize3D(sc, 3, NumericLocale::KnownC)); CHECK_EQ(sc.dttype.u58, 0);
    static_assert(constructorBackedUpRefreshOrder[2] == Page::StepSize);
    static_assert(constructorBackedUpRefreshOrder[5] == Page::Advanced);
}

TEST_CASE(stopping_snapshots_scalars_but_reselects_rotation_at_save)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false);
    auto* second = model.objectWrap(2, true, false);
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 999; sc.mdsobj[2] = 8;
    sc.tmax = 1.2345678; sc.emax = 2.3456789; sc.mdsobj4 = 3.123456;
    sc.pdie1 = 0;
    for (int i = 0; i < 3; ++i) {
        sc.smax[i] = i + 0.25; sc.vmin[i] = i + 0.5; sc.lmax[i] = i + 0.75;
        field<double>(first, 0x29e8 + i * 8) = i + 1.2345678;
        field<double>(first, 0x2a00 + i * 8) = i + 4.2345678;
        field<double>(second, 0x29e8 + i * 8) = 0.0;
    }
    const auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK_EQ(form->mdsModeCombo, -1);
    CHECK(form->mdsThreshold.text == qt::L("   3.12346"));
    // Final constructor stopping fill has snapshot vectors from first. Change
    // the model so the Save selector now finds second: remembered text goes there.
    for (int i = 0; i < 3; ++i) field<double>(first, 0x29e8 + i * 8) = 0.0;
    field<double>(second, 0x29e8) = 9.0;
    sc.tmax = 99.0; sc.emax = 99.0; sc.mdsobj[0] = 2; sc.mdsobj[1] = 2;
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(sc.tmax == 1.23457); CHECK(sc.emax == 2.34568); CHECK(sc.mdsobj4 == 3.12346);
    CHECK_EQ(sc.mdsobj[0], 0); CHECK_EQ(sc.mdsobj[1], 0); CHECK_EQ(sc.mdsobj[2], -1);
    CHECK(field<double>(first, 0x29e8) == 0.0);
    CHECK(field<double>(second, 0x29e8) == 1.23457);
    CHECK(field<double>(second, 0x2a00) == 4.23457);
}

TEST_CASE(stopping_invalid_threshold_still_executes_rotation_tail)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false);
    sc.mdsobj[0] = sc.mdsobj[1] = 0; sc.pdie1 = 0;
    for (int i = 0; i < 3; ++i) {
        field<double>(first, 0x29e8 + i * 8) = 0.0;
        field<double>(first, 0x2a00 + i * 8) = 0.0;
    }
    auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->mdsThreshold = {qt::L("bad"), ValidatorState::Invalid};
    form->rotationCenter[0] = qt::L("8.75");
    sc.mdsobj4 = 77.0;
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(sc.mdsobj4 == 0.0); CHECK(field<double>(first, 0x2a00) == 8.75);
}

TEST_CASE(stopping_partial_validation_and_pdie_default_no_continues_tail)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false);
    sc.mdsobj[0] = sc.mdsobj[1] = 0; sc.pdie1 = 0;
    auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->timeMaximum = {qt::L("12.5"), ValidatorState::Acceptable};
    form->strokeMaximum[1] = {qt::L("bad"), ValidatorState::Invalid};
    sc.smax[1] = 77.0; sc.emax = 99.0;
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.tmax == 12.5); CHECK(sc.smax[1] == 77.0); CHECK(sc.emax == 99.0);
    form->strokeMaximum[1] = {qt::L("0"), ValidatorState::Acceptable};
    sc.pdie1 = 1; field<int>(first, 0x21c8) = 3; sc.vmin[0] = 900.0;
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(sc.tmax == 12.5); CHECK(sc.vmin[0] == 900.0); CHECK(sc.emax == 0.0);
}

TEST_CASE(stopping_node_mds_and_locale_profiles_are_explicitly_guarded)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false);
    field<int>(first, 0x1ec8) = 2;
    field<int>(first, 0x1ee8) = 1; field<int>(first, 0x880) = 1;
    // This changed, valid node reaches the still-guarded GUI database reload.
    model.step->u1b58 = 0; model.step->step_version = 6000;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC));
    field<int>(first, 0xc) = 7; // undefined object is excluded from both MDS combos
    CHECK(fillStopping3D(sc, model, NumericLocale::KnownC).has_value());
    CHECK(!fillStopping3D(sc, model, NumericLocale::Unrecovered));
}

TEST_CASE(stopping_coordinate_selection_preserves_fill_order_and_open_model)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false);
    auto* second = model.objectWrap(2, true, false);
    coordinateReference(first); coordinateReference(second);
    for (const int firstId : {0, 1}) for (const int secondId : {0, 2}) {
        sc.mdsobj[0] = firstId; sc.mdsobj[1] = secondId;
        const auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form);
        REQUIRE(form->referenceChoices.size() == 3);
        CHECK_EQ(form->referenceChoices[0], 0); CHECK_EQ(form->referenceChoices[1], 1);
        CHECK_EQ(form->referenceChoices[2], 2);
        CHECK_EQ(form->referenceFillOrder[0], firstId ? 0 : (secondId ? 1 : -1));
        CHECK_EQ(form->referenceFillOrder[1], secondId ? 1 : -1);
        for (int side = 0; side < 2; ++side) {
            const auto& reference = form->widgets.references[side];
            const bool selected = side == 0 ? firstId != 0 : secondId != 0;
            CHECK_EQ(reference.selectedObject, side == 0 ? firstId : secondId);
            CHECK(reference.coordinateChecked == selected);
            CHECK(reference.nodeChecked == !selected);
            CHECK(reference.nodeEnabled == !selected);
            CHECK(reference.nodeSpinEnabled == !selected);
            CHECK(reference.coordinatesEnabled == selected); CHECK_EQ(reference.node, 0);
            if (selected) {
                CHECK(reference.coordinates[0] == qt::L("1.235"));
                CHECK(reference.coordinates[1] == qt::L("-0.000"));
                CHECK(reference.coordinates[2] == qt::L("0.000"));
            } else {
                CHECK(reference.coordinates[0] == qt::L(""));
                CHECK(!reference.coordinates[0].isNull());
            }
        }
        CHECK(field<double>(first, 0x1ed0) == 1.2345678);
        CHECK(field<double>(second, 0x1ed0) == 1.2345678);
        CHECK(std::signbit(field<double>(first, 0x1ee0)));
        CHECK_EQ(field<int>(first, 0x1ee8), 73);
    }
    const auto savedForm = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(savedForm);
    CHECK(applyStopping3D(sc, model, *savedForm) == ApplyResult::Completed);
    CHECK(field<double>(first, 0x1ee0) == 0.0);
    CHECK(!std::signbit(field<double>(first, 0x1ee0))); // displayed exact -0.0 lost its sign.
}

TEST_CASE(stopping_coordinate_save_uses_rounded_memory_and_fresh_node_zero)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    field<double>(object, 0x1ee0) = 1000000000000000.0;
    auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form);
    CHECK(form->widgets.references[0].coordinates[2] == qt::L("1000000000000000.000"));
    field<double>(object, 0x1ed0) = 99.0; field<int>(object, 0x1ec8) = 2;
    sc.mdsobj[0] = 0;
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK_EQ(sc.mdsobj[0], 1); CHECK_EQ(field<int>(object, 0x1ec8), 1);
    CHECK_EQ(field<int>(object, 0x1ee8), 0);
    CHECK(field<double>(object, 0x1ed0) == 1.235);
    CHECK(field<double>(object, 0x1ed8) == 0.0);
    CHECK(std::signbit(field<double>(object, 0x1ed8)));
    CHECK(field<double>(object, 0x1ee0) == 1000000000000000.0);
}

TEST_CASE(stopping_coordinate_refill_retains_unwritten_controls)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingWidgetState previous; previous.references[0].node = 41;
    previous.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    auto first = fillStopping3D(sc, model, NumericLocale::KnownC, previous); REQUIRE(first);
    CHECK_EQ(first->widgets.references[0].node, 41);
    sc.mdsobj[0] = 0;
    auto none = fillStopping3D(sc, model, NumericLocale::KnownC, first->widgets); REQUIRE(none);
    CHECK_EQ(none->referenceFillOrder[0], -1); CHECK_EQ(none->referenceFillOrder[1], -1);
    const auto& retained = none->widgets.references[0];
    CHECK_EQ(retained.selectedObject, 0); CHECK_EQ(retained.node, 41);
    CHECK(retained.coordinateChecked); CHECK(!retained.nodeChecked); CHECK(!retained.nodeEnabled);
    CHECK(retained.coordinatesEnabled); CHECK(!retained.nodeSpinEnabled);
    CHECK(retained.coordinates[0] == qt::L("1.235"));
    sc.mdsobj[0] = 1; field<int>(object, 0x87c) = 1;
    auto unknown = none->widgets; unknown.labelProfile = StoppingLabelProfile::Unknown;
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC, unknown));
    CHECK(unknown.references[0].coordinates[0] == qt::L("1.235"));
    CHECK_EQ(unknown.references[0].selectedObject, 0);
    auto meshed = fillStopping3D(sc, model, NumericLocale::KnownC, none->widgets); REQUIRE(meshed);
    CHECK(meshed->widgets.references[0].nodeEnabled);
    CHECK(!meshed->widgets.references[0].nodeSpinEnabled); // radio stayed unchecked.
    CHECK_EQ(meshed->widgets.references[0].node, 41);
    field<int>(object, 0xc) = 5;
    auto kind5 = fillStopping3D(sc, model, NumericLocale::KnownC, none->widgets); REQUIRE(kind5);
    CHECK(!kind5->widgets.references[0].nodeEnabled); // kind5 leaves it untouched.
}

TEST_CASE(stopping_coordinate_node_save_tests_both_reference_radios)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false); coordinateReference(first);
    auto* second = model.objectWrap(2, true, false); coordinateReference(second);
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingWidgetState previous; previous.references[0].node = 41;
    previous.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    previous.references[1].node = 52;
    auto one = fillStopping3D(sc, model, NumericLocale::KnownC, previous); REQUIRE(one);
    CHECK(!one->widgets.references[0].nodeEnabled); CHECK(one->widgets.references[1].nodeEnabled);
    CHECK(applyStopping3D(sc, model, *one) == ApplyResult::Completed);
    CHECK_EQ(field<int>(first, 0x1ee8), 41); // other enabled radio prevents clearing.
    sc.mdsobj[1] = 2;
    auto both = fillStopping3D(sc, model, NumericLocale::KnownC, previous); REQUIRE(both);
    CHECK(applyStopping3D(sc, model, *both) == ApplyResult::Completed);
    CHECK_EQ(field<int>(first, 0x1ee8), 0); CHECK_EQ(field<int>(second, 0x1ee8), 0);
}

TEST_CASE(stopping_coordinate_save_relooks_up_objects_and_second_reference_wins)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* original = model.objectWrap(1, true, false); coordinateReference(original);
    sc.mdsobj[0] = sc.mdsobj[1] = 1;
    auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form);
    form->widgets.references[0].coordinates[0] = qt::L("11.5");
    form->widgets.references[1].coordinates[0] = qt::L("22.75");
    field<int>(original, 8) = 9; // remembered number now refers to another object.
    auto* replacement = model.objectWrap(1, true, false); coordinateReference(replacement);
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(field<double>(original, 0x1ed0) == 1.2345678);
    CHECK(field<double>(replacement, 0x1ed0) == 22.75);
    field<int>(replacement, 8) = 8;
    form->widgets.references[1].selectedObject = 9;
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK_EQ(sc.mdsobj[0], 1); CHECK_EQ(sc.mdsobj[1], 9);
    CHECK(field<double>(original, 0x1ed0) == 22.75); // missing first did not stop second.
}

TEST_CASE(stopping_coordinate_failure_order_preserves_earlier_stores)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form);
    form->timeMaximum = {qt::L("12.5"), ValidatorState::Acceptable};
    form->strainMaximum = {qt::L("bad"), ValidatorState::Invalid};
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.tmax == 12.5); CHECK(field<double>(object, 0x1ed0) == 1.2345678);
    CHECK_EQ(field<int>(object, 0x1ee8), 73);
    form->strainMaximum = {qt::L("3.5"), ValidatorState::Acceptable};
    form->mdsThreshold = {qt::L("bad"), ValidatorState::Invalid};
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(sc.mdsobj4 == 0.0); CHECK(field<double>(object, 0x1ed0) == 1.235);
}

TEST_CASE(stopping_coordinate_profile_keeps_node_geometry_and_label_guards)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false); coordinateReference(first);
    auto* second = model.objectWrap(2, true, false); coordinateReference(second);
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC, StoppingWidgetState{}));
    auto unknown = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(unknown);
    unknown->widgets.labelProfile = StoppingLabelProfile::Unknown;
    CHECK(applyStopping3D(sc, model, *unknown) == ApplyResult::UnsupportedSnapshot);
    field<int>(first, 0x880) = 73; // source node73 is changed and valid in mode2.
    model.step->u1b58 = 0; model.step->step_version = 6000;
    for (const int mode : {2, 3}) {
        field<int>(first, 0x1ec8) = mode;
        CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC));
    }
    field<int>(first, 0x1ec8) = 1;
    std::memset(first->name, 'A', sizeof first->name);
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC));
    first->name[sizeof first->name - 1] = 0;
    field<int>(first, 8) = (std::numeric_limits<int>::max)(); sc.mdsobj[0] = field<int>(first, 8);
    CHECK(fillStopping3D(sc, model, NumericLocale::KnownC).has_value());
    first->name[0] = static_cast<char>(0x80);
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC)); first->name[0] = 'A';
    field<int>(second, 8) = field<int>(first, 8);
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC)); field<int>(second, 8) = 2;
    field<double>(first, 0x1ed0) = (std::numeric_limits<double>::infinity)();
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC));
    field<int>(first, 0xc) = 7;
    CHECK(fillStopping3D(sc, model, NumericLocale::KnownC).has_value());
}

TEST_CASE(stopping_mode_zero_can_check_a_disabled_node_then_save_mode_two)
{
    for (const int meshCount : {0, 1}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        auto* object = model.objectWrap(1, true, false); coordinateReference(object);
        field<int>(object, 0x1ec8) = 0; field<int>(object, 0x87c) = meshCount;
        sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
        const auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form);
        const auto& reference = form->widgets.references[0];
        CHECK(reference.nodeChecked); CHECK(!reference.coordinateChecked);
        CHECK(reference.nodeEnabled == (meshCount != 0));
        CHECK(reference.nodeSpinEnabled); CHECK(!reference.coordinatesEnabled);
        CHECK_EQ(reference.node, 0); CHECK(reference.coordinates[0] == qt::L("1.235"));
        CHECK_EQ(field<int>(object, 0x1ec8), 0); CHECK_EQ(field<int>(object, 0x1ee8), 73);
        CHECK(field<double>(object, 0x1ed0) == 1.2345678);
        CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
        CHECK_EQ(field<int>(object, 0x1ec8), 2); CHECK_EQ(field<int>(object, 0x1ee8), 0);
        CHECK(field<double>(object, 0x1ed0) == 1.235);
    }
}

TEST_CASE(stopping_mode_zero_refill_preserves_spin_and_second_reference_double_fill)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 1;
    StoppingWidgetState previous;
    previous.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    previous.references[1].node = 41;
    auto coordinate = fillStopping3D(sc, model, NumericLocale::KnownC, previous); REQUIRE(coordinate);
    CHECK(coordinate->widgets.references[1].coordinateChecked);
    CHECK(!coordinate->widgets.references[1].nodeSpinEnabled);
    field<int>(object, 0x1ec8) = 0; field<int>(object, 0x1ee8) = 99;
    auto node = fillStopping3D(sc, model, NumericLocale::KnownC, coordinate->widgets); REQUIRE(node);
    CHECK_EQ(node->referenceFillOrder[0], 1); CHECK_EQ(node->referenceFillOrder[1], 1);
    const auto& reference = node->widgets.references[1];
    CHECK(reference.nodeChecked); CHECK(!reference.nodeEnabled); CHECK(reference.nodeSpinEnabled);
    CHECK(!reference.coordinateChecked); CHECK(!reference.coordinatesEnabled); CHECK_EQ(reference.node, 41);
    CHECK_EQ(field<int>(object, 0x1ec8), 0); CHECK_EQ(field<int>(object, 0x1ee8), 99);
    CHECK(applyStopping3D(sc, model, *node) == ApplyResult::Completed);
    CHECK_EQ(field<int>(object, 0x1ec8), 2);
    CHECK_EQ(field<int>(object, 0x1ee8), 41); // first None radio remains enabled.
}

TEST_CASE(stopping_mode_two_clamps_rejected_nodes_and_preserves_open_model)
{
    for (const int sourceNode : {-1, 0, 1, 999999, 1000000}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        auto* object = model.objectWrap(1, true, false); coordinateReference(object);
        field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = sourceNode;
        field<int>(object, 0x880) = 0;
        sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
        StoppingWidgetState previous;
        previous.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
        previous.references[0].node = 42; // every requested bounded value differs.
        REQUIRE(field<double*>(object, 0x8a8) == nullptr);
        const auto form = fillStopping3D(sc, model, NumericLocale::KnownC, previous); REQUIRE(form);
        const int bounded = (std::min)((std::max)(sourceNode, 0), 999999);
        const auto& reference = form->widgets.references[0];
        CHECK_EQ(reference.node, bounded); CHECK(reference.nodeChecked);
        CHECK(!reference.nodeEnabled); CHECK(reference.nodeSpinEnabled);
        CHECK(!reference.coordinateChecked); CHECK(!reference.coordinatesEnabled);
        CHECK(reference.coordinates[0] == qt::L("1.235"));
        CHECK_EQ(field<int>(object, 0x1ee8), sourceNode);
        CHECK(field<double>(object, 0x1ed0) == 1.2345678);
        CHECK(field<double*>(object, 0x8a8) == nullptr);
        CHECK_EQ(previous.references[0].node, 42);
        CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
        CHECK_EQ(field<int>(object, 0x1ee8), bounded); // the other None radio is enabled.
        CHECK(field<double>(object, 0x1ed0) == 1.235);
        CHECK(field<double*>(object, 0x8a8) == nullptr);
    }
}

TEST_CASE(stopping_mode_two_unchanged_valid_node_does_not_read_geometry)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1000000;
    field<int>(object, 0x880) = 999999; field<int>(object, 0x87c) = 1;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingWidgetState previous;
    previous.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    previous.references[0].node = 999999;
    REQUIRE(field<double*>(object, 0x8a8) == nullptr);
    // Even a model that would need GUI database synchronization is never
    // inspected by a same-value setter.
    model.step->u1b58 = 0; model.step->step_version = 11000;
    const auto form = fillStopping3D(sc, model, NumericLocale::KnownC, previous); REQUIRE(form);
    CHECK_EQ(form->widgets.references[0].node, 999999);
    CHECK(form->widgets.references[0].nodeEnabled);
    CHECK(form->widgets.references[0].coordinates[0] == qt::L("1.235"));
    CHECK_EQ(field<int>(object, 0x1ee8), 1000000);
    CHECK(field<double*>(object, 0x8a8) == nullptr);
    previous.references[0].node = 999998;
    CHECK(!fillStopping3D(sc, model, NumericLocale::KnownC, previous));
    CHECK_EQ(previous.references[0].node, 999998);
    CHECK_EQ(field<int>(object, 0x1ee8), 1000000);
    CHECK(field<double*>(object, 0x8a8) == nullptr);
}

TEST_CASE(stopping_mode_two_second_only_refill_and_both_disabled_save)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 73;
    field<int>(object, 0x880) = -1;
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 1;
    const auto first = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(first);
    CHECK_EQ(first->referenceFillOrder[0], 1); CHECK_EQ(first->referenceFillOrder[1], 1);
    CHECK_EQ(first->widgets.references[1].node, 73);
    CHECK(field<double*>(object, 0x8a8) == nullptr);
    // The same remembered value avoids geometry even after node availability
    // changes between original constructor fills.
    field<int>(object, 0x880) = 73;
    const auto repeated = fillStopping3D(sc, model, NumericLocale::KnownC, first->widgets);
    REQUIRE(repeated); CHECK_EQ(repeated->widgets.references[1].node, 73);
    CHECK(field<double*>(object, 0x8a8) == nullptr);
    field<int>(object, 0x880) = 0; sc.mdsobj[0] = 1;
    const auto both = fillStopping3D(sc, model, NumericLocale::KnownC, repeated->widgets); REQUIRE(both);
    CHECK(!both->widgets.references[0].nodeEnabled); CHECK(!both->widgets.references[1].nodeEnabled);
    CHECK(applyStopping3D(sc, model, *both) == ApplyResult::Completed);
    CHECK_EQ(field<int>(object, 0x1ec8), 2); CHECK_EQ(field<int>(object, 0x1ee8), 0);
    CHECK(field<double*>(object, 0x8a8) == nullptr);
}

TEST_CASE(stopping_node_reads_keep_native_getter_counts_and_final_stored_text)
{
    const std::array<std::array<int, 2>, 3> selections{{{1, 0}, {0, 1}, {1, 1}}};
    for (const auto selected : selections) {
        for (const bool existing : {false, true}) {
            engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
            auto* object = model.objectWrap(1, true, false); coordinateReference(object);
            field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
            field<int>(object, 0x87c) = 1; field<int>(object, 0x880) = 1;
            auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
            nodes.n = 1; model.step->u15f8[0] = 3;
            // Both source-closed no-sync reasons are exercised independently.
            model.step->u1b58 = existing ? 0 : 1;
            model.step->step_version = existing ? 5999 : 11000;
            if (existing) {
                nodes.u008 = def::model::bc::calloc_doubles(3); REQUIRE(nodes.u008);
                nodes.u008[0] = 12.345678; nodes.u008[1] = -20.25; nodes.u008[2] = 30.5;
            }
            sc.mdsobj[0] = selected[0]; sc.mdsobj[1] = selected[1];
            StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
            const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
            REQUIRE(report.complete()); REQUIRE(nodes.u008);
            const std::size_t calls = selected[0] ? (selected[1] ? 20 : 8) : 4;
            CHECK_EQ(report.coordinateGetterCalls, calls);
            CHECK_EQ(report.stateVariableDispatches, calls);
            CHECK_EQ(report.coordinateAllocationAttempts, existing ? 0u : 1u);
            CHECK_EQ(report.nodeCallbacks, selected[0] && selected[1] ? 2u : 1u);
            for (int side = 0; side < 2; ++side) if (selected[side]) {
                CHECK_EQ(page.widgets.references[side].node, 1);
                CHECK(page.widgets.references[side].coordinates[0] == qt::L("1.235"));
                CHECK(page.widgets.references[side].coordinates[1] == qt::L("-0.000"));
            }
            CHECK(field<double>(object, 0x1ed0) == 1.2345678);
            CHECK_EQ(field<int>(object, 0x1ee8), 1);
            CHECK(nodes.u008[0] == (existing ? 12.345678 : 0.0));
            const auto repeated = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
            REQUIRE(repeated.complete()); CHECK_EQ(repeated.coordinateGetterCalls, 0u);
        }
    }
}

TEST_CASE(stopping_node_null_result_continues_to_stored_text_without_fabricating_geometry)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x880) = 1;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0); nodes.n = 0;
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 1;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    REQUIRE(report.complete()); CHECK_EQ(report.coordinateGetterCalls, 1u);
    CHECK_EQ(report.stateVariableDispatches, 0u); CHECK_EQ(report.coordinateAllocationAttempts, 0u);
    CHECK(nodes.u008 == nullptr); CHECK_EQ(page.widgets.references[1].node, 1);
    CHECK(page.widgets.references[1].coordinates[0] == qt::L("1.235"));
    CHECK_EQ(page.referenceFillOrder[0], 1); CHECK_EQ(page.referenceFillOrder[1], 1);
}

TEST_CASE(stopping_node_callback_reuses_none_reference_radio_and_local_vector_path)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x880) = 1;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = 1; model.step->u15f8[0] = 3;
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 1;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    page.widgets.references[0].nodeChecked = false;
    page.widgets.references[0].coordinateChecked = true;
    page.widgets.references[0].coordinates[0] = qt::L("remembered invalid text");
    const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    REQUIRE(report.complete()); CHECK_EQ(report.coordinateGetterCalls, 8u);
    CHECK_EQ(report.referenceVectorZeroings, 1u);
    CHECK(page.widgets.references[0].coordinates[0] == qt::L("remembered invalid text"));
    CHECK(page.widgets.references[1].coordinates[0] == qt::L("1.235"));
}

TEST_CASE(stopping_node_sync_refusal_retains_allocation_cow_spin_and_scalar_prefix)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x87c) = 1; field<int>(object, 0x880) = 1;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = 1; model.step->u15f8[0] = 3;
    auto& variables = *reinterpret_cast<def::model::CowList<def::model::StateVarBase*>*>(&nodes);
    const auto shared = variables; REQUIRE(variables.use_count() == 2);
    model.step->u1b58 = 0; model.step->step_version = 6000;
    sc.tmax = 12.5; sc.emax = 34.5; sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    page.mdsModeCombo = 3; page.mdsThreshold.text = qt::L("old threshold");
    page.rotationVector[0] = qt::L("old rotation");
    page.widgets.references[0].coordinates[0] = qt::L("old coordinate");
    const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(!report.complete()); CHECK(report.issue == StoppingFillIssue::CoordinateSync);
    CHECK(report.stage == StoppingFillStage::NodeCallback); CHECK_EQ(report.reference, 0);
    CHECK_EQ(report.invocation, 0); CHECK_EQ(report.coordinateGetterCalls, 1u);
    CHECK_EQ(report.coordinateAllocationAttempts, 1u); CHECK_EQ(report.stateVariableDispatches, 1u);
    REQUIRE(nodes.u008); CHECK(nodes.u008[0] == 0.0); CHECK(nodes.u008[2] == 0.0);
    CHECK(variables.body() != shared.body()); CHECK_EQ(variables.use_count(), 1);
    CHECK_EQ(shared.use_count(), 1); CHECK_EQ(variables.size(), 28u);
    CHECK(page.timeMaximum.text == qt::L("12.5")); CHECK(page.strainMaximum.text == qt::L("34.5"));
    CHECK_EQ(page.widgets.references[0].selectedObject, 1); CHECK_EQ(page.widgets.references[0].node, 1);
    CHECK(page.widgets.references[0].coordinates[0] == qt::L("old coordinate"));
    CHECK_EQ(page.mdsModeCombo, 3); CHECK(page.mdsThreshold.text == qt::L("old threshold"));
    CHECK(page.rotationVector[0] == qt::L("old rotation"));
    CHECK(field<double>(object, 0x1ed0) == 1.2345678);
}

TEST_CASE(stopping_node_extent_refusal_occurs_after_the_axis_getter)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 2;
    field<int>(object, 0x880) = 2;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = 1; model.step->u15f8[0] = 3;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::CoordinateExtent); CHECK_EQ(report.axis, 0);
    CHECK_EQ(report.coordinateGetterCalls, 2u); CHECK_EQ(report.stateVariableDispatches, 2u);
    CHECK_EQ(report.coordinateAllocationAttempts, 1u); REQUIRE(nodes.u008);
    CHECK_EQ(page.widgets.references[0].node, 2); CHECK(nodes.u008[2] == 0.0);
}

TEST_CASE(stopping_node_unsafe_allocation_and_foreign_owner_are_reached_guards)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    engine::GuiModel foreign(nullptr, 3);
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x880) = 1;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = (std::numeric_limits<int>::max)(); model.step->u15f8[0] = 3;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::CoordinateExtent); CHECK_EQ(page.widgets.references[0].node, 1);
    CHECK_EQ(report.coordinateAllocationAttempts, 0u); CHECK_EQ(report.stateVariableDispatches, 0u);
    CHECK(nodes.u008 == nullptr);
    nodes.n = 1; nodes.u130 = foreign.step.get(); page.widgets.references[0].node = 0;
    report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::CoordinateOwner); CHECK_EQ(page.widgets.references[0].node, 1);
    CHECK_EQ(report.coordinateGetterCalls, 1u); CHECK(nodes.u008 == nullptr);
    nodes.u130 = model.step.get(); // restore the actual object's owner before teardown.
}

TEST_CASE(stopping_node_nonfinite_format_refusal_retains_prior_callback_text)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false); coordinateReference(object);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x880) = 1;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = 1; model.step->u15f8[0] = 3;
    nodes.u008 = def::model::bc::calloc_doubles(3); REQUIRE(nodes.u008);
    nodes.u008[0] = 12.345678; nodes.u008[1] = (std::numeric_limits<double>::infinity)();
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    page.widgets.references[0].coordinates[1] = qt::L("old y");
    const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::NonfiniteValue);
    CHECK(report.stage == StoppingFillStage::NodeCallback); CHECK_EQ(report.axis, 1);
    CHECK_EQ(report.coordinateGetterCalls, 4u); CHECK_EQ(page.widgets.references[0].node, 1);
    CHECK(page.widgets.references[0].coordinates[0] == qt::L("12.3457"));
    CHECK(page.widgets.references[0].coordinates[1] == qt::L("old y"));
    CHECK(field<double>(object, 0x1ed0) == 1.2345678); // stored %.3f overwrite was not reached.
}

TEST_CASE(stopping_partial_scalar_and_selector_refusals_follow_source_order)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false); coordinateReference(first);
    auto* second = model.objectWrap(2, true, false); coordinateReference(second);
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0; sc.tmax = 12.5;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    page.velocityMinimum[0].text = qt::L("old vmin"); page.referenceChoices = {99};
    page.widgets.references[0].selectedObject = 99;
    sc.vmin[0] = (std::numeric_limits<double>::infinity)();
    auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.stage == StoppingFillStage::Scalars); CHECK_EQ(report.scalar, 4);
    CHECK(page.timeMaximum.text == qt::L("12.5")); CHECK(page.velocityMinimum[0].text == qt::L("old vmin"));
    CHECK_EQ(page.referenceChoices[0], 99); CHECK_EQ(page.widgets.references[0].selectedObject, 99);
    sc.vmin[0] = 7.5; page.widgets.labelProfile = StoppingLabelProfile::Unknown;
    report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::UnknownLabelProfile);
    CHECK(page.velocityMinimum[0].text == qt::L("7.5")); CHECK(page.referenceChoices.empty());
    CHECK_EQ(page.widgets.references[0].selectedObject, 0);
    page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    second->name[0] = static_cast<char>(0x80);
    report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::UnsupportedObjectLabel);
    REQUIRE(page.referenceChoices.size() == 2); CHECK_EQ(page.referenceChoices[1], 1);
    CHECK_EQ(page.widgets.references[0].selectedObject, 0); // selections follow complete lists.
}

TEST_CASE(stopping_partial_second_reference_and_threshold_preserve_completed_first)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false); coordinateReference(first);
    auto* second = model.objectWrap(2, true, false); coordinateReference(second);
    field<int>(second, 0x1ec8) = 3;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 2; sc.mdsobj[2] = 2;
    StoppingPage page{}; page.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    page.widgets.references[1].coordinates[0] = qt::L("old second");
    page.mdsModeCombo = 1; page.mdsThreshold.text = qt::L("old threshold");
    page.rotationVector[0] = qt::L("old rotation");
    auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.issue == StoppingFillIssue::UnsupportedMdsMode); CHECK_EQ(report.invocation, 1);
    CHECK(page.widgets.references[0].coordinates[0] == qt::L("1.235"));
    CHECK(page.widgets.references[1].coordinates[0] == qt::L("old second"));
    CHECK(!page.widgets.references[1].nodeEnabled); // availability preceded both fills.
    CHECK_EQ(page.mdsModeCombo, 1);
    field<int>(second, 0x1ec8) = 1; sc.mdsobj4 = (std::numeric_limits<double>::infinity)();
    report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.stage == StoppingFillStage::Threshold); CHECK_EQ(page.mdsModeCombo, 2);
    CHECK(page.widgets.references[1].coordinates[0] == qt::L("1.235"));
    CHECK(page.mdsThreshold.text == qt::L("old threshold"));
    CHECK(page.rotationVector[0] == qt::L("old rotation"));
}

TEST_CASE(stopping_partial_rotation_fills_vector_before_center)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false);
    field<double>(object, 0x29e8) = 1.25; field<double>(object, 0x29f0) = 2.5;
    field<double>(object, 0x29f8) = 3.75; field<double>(object, 0x2a00) = 4.5;
    field<double>(object, 0x2a08) = (std::numeric_limits<double>::infinity)();
    StoppingPage page{}; page.rotationCenter[1] = qt::L("old y"); page.rotationCenter[2] = qt::L("old z");
    const auto report = fillStopping3DInto(page, sc, model, NumericLocale::KnownC);
    CHECK(report.stage == StoppingFillStage::Rotation); CHECK_EQ(report.axis, 4);
    CHECK(page.rotationVector[2] == qt::L("3.75")); CHECK(page.rotationCenter[0] == qt::L("4.5"));
    CHECK(page.rotationCenter[1] == qt::L("old y")); CHECK(page.rotationCenter[2] == qt::L("old z"));
}

TEST_CASE(meshing_sorts_labels_and_roundtrips_only_the_selected_first_object)
{
    engine::GuiModel model(nullptr, 3);
    auto* two = model.objectWrap(2, true, false);
    auto* ten = model.objectWrap(10, true, false);
    field<int>(two, 0x87c) = field<int>(ten, 0x87c) = 1;
    field<double>(two, 0x2dc8) = 2.12345678;
    field<double>(two, 0x2dd0) = 3.12345678;
    field<double>(two, 0x2de0) = -4.12345678;
    field<double>(two, 0x2dd8) = 5.75;
    field<double>(ten, 0x2dc8) = -6.12345678;
    field<double>(ten, 0x2dd0) = 7.12345678;
    field<double>(ten, 0x2de0) = -8.12345678;
    field<double>(ten, 0x2dd8) = 9.75;
    RemeshState state;
    const auto form = fillMeshing3D(model, state); REQUIRE(form.has_value());
    REQUIRE(form->objects.size() == 2);
    CHECK_EQ(form->objects[0].number, 10); CHECK_EQ(form->objects[1].number, 2);
    field<double>(two, 0x2dc8) = 99.0;
    field<double>(ten, 0x2dc8) = 99.0;
    field<double>(ten, 0x2d20) = 123.0; // a broad native mesh copy would overwrite it
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
    CHECK(field<double>(ten, 0x2dc8) == -6.12346);
    CHECK(field<double>(ten, 0x2dd0) == 7.12346);
    CHECK(field<double>(ten, 0x2de0) == 8.12346);
    CHECK(field<double>(ten, 0x2dd8) == 9.0);
    CHECK(field<double>(two, 0x2dc8) == 2.12345678);
    CHECK(field<double>(two, 0x2dd0) == 3.12345678);
    CHECK(field<double>(two, 0x2de0) == -4.12345678);
    CHECK(field<double>(two, 0x2dd8) == 5.75);
    CHECK(field<double>(ten, 0x2d20) == 123.0);
}

TEST_CASE(meshing_eligibility_spin_limits_and_signed_zero_are_original_controls)
{
    engine::GuiModel model(nullptr, 3);
    auto* one = model.objectWrap(1, true, false);
    field<int>(one, 0x87c) = 0; field<int>(one, 0xc) = 1;
    RemeshState state;
    const auto excluded = fillMeshing3D(model, state); REQUIRE(excluded.has_value());
    CHECK(excluded->objects.empty());
    field<int>(one, 0xc) = 5; // eligible without existing mesh
    field<double>(one, 0x2dc8) = -0.0;
    field<double>(one, 0x2dd0) = 1.0; field<double>(one, 0x2de0) = -0.0;
    for (const auto [source, expected] : {std::pair{-1.9, 0}, std::pair{12.9, 12},
             std::pair{1500000000.0, 1000000000}, std::pair{2147483648.0, 0}}) {
        field<double>(one, 0x2dd8) = source;
        const auto form = fillMeshing3D(model, state); REQUIRE(form.has_value());
        REQUIRE(form->objects.size() == 1); CHECK_EQ(form->firstNodeCount, expected);
        CHECK(!form->firstInterferenceNegative);
        CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
        CHECK(!std::signbit(field<double>(one, 0x2dc8)));
        CHECK(!std::signbit(field<double>(one, 0x2de0)));
    }
}

TEST_CASE(meshing_unvisited_scalar_bits_and_unrelated_state_are_preserved)
{
    engine::GuiModel model(nullptr, 3);
    auto* one = model.objectWrap(1, true, false);
    auto* two = model.objectWrap(2, true, false);
    field<int>(one, 0x87c) = field<int>(two, 0x87c) = 1;
    for (auto* object : {one, two}) {
        field<double>(object, 0x2dc8) = 0.0; field<double>(object, 0x2dd0) = 1.0;
        field<double>(object, 0x2de0) = 0.0; field<double>(object, 0x2dd8) = 10.0;
    }
    constexpr std::uint64_t nanBits = 0x7ff8000000001234ULL;
    field<double>(two, 0x2dc8) = std::bit_cast<double>(nanBits);
    RemeshState state;
    const auto form = fillMeshing3D(model, state); REQUIRE(form.has_value());
    field<double>(two, 0x2dc8) = 22.0;
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
    CHECK(std::bit_cast<std::uint64_t>(field<double>(two, 0x2dc8)) == nanBits);
}

TEST_CASE(meshing_remesh_snapshot_applies_only_original_written_fields)
{
    engine::GuiModel model(nullptr, 3);
    RemeshState state;
    CHECK_EQ(state.method, 1); CHECK(state.goodInternalChangeable);
    CHECK(state.goodSurfaceChangeable); CHECK(state.scalingFactor == 1.2); CHECK(!state.enabled);
    state.enabled = true; state.method = 3; state.scalingFactor = 2.3456789;
    state.goodInternalChangeable = false;
    const auto form = fillMeshing3D(model, state); REQUIRE(form.has_value());
    state.method = 1; state.scalingFactor = 9.0; state.goodInternalChangeable = true;
    state.filterDistance = 44.0; state.uniformInternalEdge = 55.0;
    state.improveContact = true; state.goodSurfaceChangeable = false;
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
    CHECK(state.enabled); CHECK_EQ(state.method, 3); CHECK(state.scalingFactor == 2.34568);
    CHECK(!state.goodInternalChangeable);
    CHECK(state.filterDistance == 44.0); CHECK(state.uniformInternalEdge == 55.0);
    CHECK(state.improveContact); CHECK(!state.goodSurfaceChangeable);
    state.enabled = false;
    const auto disabled = fillMeshing3D(model, state); REQUIRE(disabled.has_value());
    state.method = 5; state.scalingFactor = 8.0; state.goodInternalChangeable = true;
    CHECK(applyMeshing3D(model, state, *disabled) == ApplyResult::Completed);
    CHECK(!state.enabled); CHECK_EQ(state.method, 5); CHECK(state.scalingFactor == 8.0);
    CHECK(state.goodInternalChangeable);
}

TEST_CASE(meshing_remesh_default_radio_and_scaling_clamp_are_explicit)
{
    engine::GuiModel model(nullptr, 3);
    RemeshState state; state.enabled = true; state.method = 5;
    auto form = fillMeshing3D(model, state); REQUIRE(form.has_value());
    CHECK_EQ(form->remeshMethodRadio, 1); // setupUi default; fill ignores mode5
    form->remeshScalingFactor = qt::L("55");
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
    CHECK_EQ(state.method, 1); CHECK(state.scalingFactor == 10.0);
    form->remeshScalingFactor = qt::L("bad");
    CHECK(applyMeshing3D(model, state, *form) == ApplyResult::Completed);
    CHECK(state.scalingFactor == 1.0);
    state.scalingFactor = (std::numeric_limits<double>::infinity)();
    CHECK(!fillMeshing3D(model, state));
    state.enabled = false; CHECK(fillMeshing3D(model, state).has_value());
}

TEST_CASE(stopping_pdie_default_no_skips_all_six_limits_for_each_warning_kind)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false);
    sc.mdsobj[0] = sc.mdsobj[1] = 0; sc.emax = 5.75;
    for (int i = 0; i < 3; ++i) { sc.vmin[i] = i + 0.5; sc.lmax[i] = i + 0.75; }
    auto form = fillStopping3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->rotationCenter[0] = qt::L("42.5");
    sc.pdie1 = 1;
    for (int kind : {3, 4, 7, 8, 9, 10}) {
        field<int>(first, 0x21c8) = kind;
        for (int i = 0; i < 3; ++i) { sc.vmin[i] = i + 90.0; sc.lmax[i] = i + 91.0; }
        sc.emax = 99.0; field<double>(first, 0x2a00) = 0.0;
        CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
        for (int i = 0; i < 3; ++i) {
            CHECK(sc.vmin[i] == i + 90.0); CHECK(sc.lmax[i] == i + 91.0);
        }
        CHECK(sc.emax == 5.75); CHECK(field<double>(first, 0x2a00) == 42.5);
    }
    field<int>(first, 0x21c8) = 2; // no warning: remembered limits apply normally
    CHECK(applyStopping3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(sc.vmin[0] == 0.5); CHECK(sc.lmax[2] == 2.75);
}

TEST_CASE(environment_remembers_deep_curves_and_seven_scalar_widgets)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.envtmp.value = 1.2345678; sc.cnvcof.value = 2.3456789;
    sc.envatm.value = 3.4567891; sc.acvcof.value = 4.5678912;
    sc.envtmp.u54 = 31; sc.envmpr = 5.6789123; sc.envmpt = 6.7891234;
    sc.ehrate = 7.8912345; sc.peninf = 8.9123456; sc.unte2h = 9.1234567;
    sc.tintgf = 10.234567; sc.blzman = 11.345678;
    const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form.has_value());
    CHECK(form->curves[0].get() != &sc.envtmp);
    sc.envtmp.value = sc.cnvcof.value = sc.envatm.value = sc.acvcof.value = 99.0;
    sc.envtmp.u54 = 99; sc.envmpr = sc.envmpt = sc.ehrate = sc.peninf = 99.0;
    sc.unte2h = sc.tintgf = sc.blzman = 99.0; sc.vfactr_mode = 9;
    CHECK(applyEnvironment3D(sc, *form) == ApplyResult::Completed);
    CHECK(sc.envtmp.value == 1.23457); CHECK(sc.cnvcof.value == 2.34568);
    CHECK(sc.envatm.value == 3.45679); CHECK(sc.acvcof.value == 4.56789);
    CHECK_EQ(sc.envtmp.u54, 31); CHECK(sc.envmpr == 5.67891); CHECK(sc.envmpt == 6.78912);
    CHECK(sc.ehrate == 7.89123); CHECK(sc.peninf == 8.91235); CHECK(sc.unte2h == 9.12346);
    CHECK(sc.tintgf == 10.2346); CHECK(sc.blzman == 11.3457); CHECK_EQ(sc.vfactr_mode, 0);
}

TEST_CASE(environment_disabled_pressure_keeps_empty_intermediate_and_converts_zero)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.envmpr = 7.0; sc.envmpt = 8.0;
    const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, false); REQUIRE(form.has_value());
    CHECK(form->scalars[0].text.isEmpty()); CHECK(form->scalars[1].text.isEmpty());
    CHECK(form->scalars[0].state == ValidatorState::Intermediate);
    sc.envmpr = 99.0; sc.envmpt = 99.0; sc.ehrate = 99.0;
    CHECK(applyEnvironment3D(sc, *form) == ApplyResult::Completed);
    CHECK(sc.envmpr == 0.0); CHECK(sc.envmpt == 0.0); CHECK(sc.ehrate == 0.0);
}

TEST_CASE(environment_curve_invalid_still_copies_before_scalar_partial_return)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.envtmp.value = 1.2345678; sc.cnvcof.value = 2.3456789;
    sc.envatm.value = 3.4567891; sc.acvcof.value = 4.5678912;
    auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form.has_value());
    form->constantValues[0] = {qt::L("bad"), ValidatorState::Invalid};
    form->constantValues[1] = {qt::L(""), ValidatorState::Intermediate};
    form->scalars[0] = {qt::L("6.25"), ValidatorState::Acceptable};
    form->scalars[1] = {qt::L("bad"), ValidatorState::Invalid};
    sc.envtmp.value = sc.cnvcof.value = sc.envatm.value = sc.acvcof.value = 99.0;
    sc.envmpt = 88.0; sc.ehrate = 77.0; sc.vfactr_mode = 6;
    CHECK(applyEnvironment3D(sc, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.envtmp.value == 1.2345678); // Invalid retained remembered raw constant
    CHECK(sc.cnvcof.value == 0.0); // Intermediate still converts
    CHECK(sc.envatm.value == 3.45679); CHECK(sc.acvcof.value == 4.56789);
    CHECK(sc.envmpr == 6.25); CHECK(sc.envmpt == 88.0); CHECK(sc.ehrate == 77.0);
    CHECK_EQ(sc.vfactr_mode, 6);
}

TEST_CASE(environment_fill_propagates_the_shared_general_heat_method_combo)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    initialGeneral(sc); sc.htmthd = 3;
    auto general = fillGeneral(sc, generalContext()); REQUIRE(general.has_value());
    CHECK_EQ(general->heatMethodCombo, 0);
    const auto environment = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true);
    REQUIRE(environment.has_value());
    fillEnvironmentSharedControls(*general, *environment);
    CHECK_EQ(general->heatMethodCombo, 2);
    sc.htmthd = 99;
    REQUIRE(applyGeneral(sc, *general)); CHECK_EQ(sc.htmthd, 3);
    sc.htmthd = 3;
    auto disabledGeneral = fillGeneral(sc, generalContext()); REQUIRE(disabledGeneral.has_value());
    const auto disabled = fillEnvironment3D(sc, 3, NumericLocale::KnownC, false);
    REQUIRE(disabled.has_value()); fillEnvironmentSharedControls(*disabledGeneral, *disabled);
    REQUIRE(applyGeneral(sc, *disabledGeneral)); CHECK_EQ(sc.htmthd, 1);
}

TEST_CASE(environment_viewfactor_remembers_nonzero_checkbox_and_non3d_skips_owned_copy)
{
    for (int mode : {0, 1, -3, 9}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        sc.vfactr_mode = mode;
        const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
        CHECK(form->viewFactorChecked == (mode != 0));
        CHECK(form->viewFactorDefinitionEnabled == (mode != 0));
        CHECK_EQ(sc.vfactr_mode, mode); // Checkbox callbacks do not assign SC.
        sc.vfactr_mode = 77; sc.vfactr.value = 8.912345678; sc.vfactr_n = 13;
        CHECK(applyEnvironment3D(sc, *form, EnvironmentViewFactorState{2, nullptr, 99}) == ApplyResult::Completed);
        CHECK_EQ(sc.vfactr_mode, mode != 0 ? 1 : 0);
        CHECK(sc.vfactr.value == 8.912345678); CHECK_EQ(sc.vfactr_n, 13);
    }
}

TEST_CASE(environment_checked_viewfactor_copies_exact_owned_curve_then_definition)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.vfactr_mode = 1;
    const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
    def::model::DYNARY remembered;
    remembered.type = sc.vfactr.type; remembered.value = 8.912345678;
    remembered.ndim = 1; remembered.n[0] = 2; remembered.ncomp = 2;
    remembered.var[0] = 1; remembered.u54 = 37;
    def::model::dynary_alloc(&remembered);
    REQUIRE(remembered.axis[0]); REQUIRE(remembered.values);
    remembered.axis[0][0] = 0.0; remembered.axis[0][1] = 2.0;
    for (int i = 0; i < 4; ++i) remembered.values[i] = i + 0.123456789;
    sc.vfactr_mode = 0; sc.vfactr.value = 99.0; sc.vfactr_n = 99;
    CHECK(applyEnvironment3D(sc, *form, EnvironmentViewFactorState{3, &remembered, 7}) == ApplyResult::Completed);
    CHECK_EQ(sc.vfactr_mode, 1); CHECK_EQ(sc.vfactr_n, 7);
    CHECK_EQ(sc.vfactr.ndim, 1); CHECK_EQ(sc.vfactr.ncomp, 2); CHECK_EQ(sc.vfactr.u54, 37);
    CHECK(sc.vfactr.value == 8.912345678); // Not a g6 line-edit conversion.
    REQUIRE(sc.vfactr.axis[0]); REQUIRE(sc.vfactr.values);
    CHECK(sc.vfactr.axis[0] != remembered.axis[0]); CHECK(sc.vfactr.values != remembered.values);
    CHECK(sc.vfactr.axis[0][1] == 2.0);
    for (int i = 0; i < 4; ++i) CHECK(sc.vfactr.values[i] == i + 0.123456789);
    remembered.values[0] = 99.0; CHECK(sc.vfactr.values[0] == 0.123456789);
}

TEST_CASE(environment_each_scalar_validation_stop_prevents_all_viewfactor_tail_writes)
{
    for (int invalid = 0; invalid < 7; ++invalid) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        double* values[]{&sc.envmpr, &sc.envmpt, &sc.ehrate, &sc.peninf,
            &sc.unte2h, &sc.tintgf, &sc.blzman};
        for (int i = 0; i < 7; ++i) *values[i] = i + 10.0;
        sc.envtmp.value = 1.25; sc.vfactr_mode = 1;
        auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
        form->scalars[invalid] = {qt::L("bad"), ValidatorState::Invalid};
        for (int i = 0; i < 7; ++i) *values[i] = i + 100.0;
        sc.envtmp.value = 99.0; sc.vfactr_mode = 9; sc.vfactr.value = 77.0; sc.vfactr_n = 88;
        // No owner state is needed: the original validation returns before it.
        CHECK(applyEnvironment3D(sc, *form) == ApplyResult::ValidationStopped);
        CHECK(sc.envtmp.value == 1.25);
        for (int i = 0; i < 7; ++i) CHECK(*values[i] == i + (i < invalid ? 10.0 : 100.0));
        CHECK_EQ(sc.vfactr_mode, 9); CHECK(sc.vfactr.value == 77.0); CHECK_EQ(sc.vfactr_n, 88);
    }
}

TEST_CASE(environment_missing_or_live_model_viewfactor_owner_refuses_at_reached_copy_boundary)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.vfactr_mode = 1; sc.blzman = 12.5;
    const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
    for (const auto owner : {std::optional<EnvironmentViewFactorState>{},
             std::optional<EnvironmentViewFactorState>{EnvironmentViewFactorState{3, nullptr, 0}},
             std::optional<EnvironmentViewFactorState>{EnvironmentViewFactorState{3, &sc.vfactr, 0}}}) {
        sc.blzman = 99.0; sc.vfactr_mode = 9; sc.vfactr.value = 77.0; sc.vfactr_n = 88;
        CHECK(applyEnvironment3D(sc, *form, owner) == ApplyResult::UnsupportedSnapshot);
        CHECK(sc.blzman == 12.5); CHECK_EQ(sc.vfactr_mode, 1);
        CHECK(sc.vfactr.value == 77.0); CHECK_EQ(sc.vfactr_n, 88);
    }
}

TEST_CASE(environment_active_furnace_flag_and_live_owned_state_are_not_editor_snapshot_fields)
{
    for (unsigned flag : {1u, 2u, 255u}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        sc.furnace_on = static_cast<std::uint8_t>(flag);
        std::strcpy(sc.furnace.name, "INITIAL FURNACE"); sc.furnace.u58[5] = 0x7b;
        const std::int32_t firstIndex = 1;
        auto* curve = sc.furnace.curve(&firstIndex); REQUIRE(curve);
        curve->value = 12.3456789; sc.envtmp.value = 1.2345678;
        const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
        CHECK_EQ(sc.furnace_on, flag); CHECK(curve->value == 12.3456789);
        CHECK(std::strcmp(sc.furnace.name, "INITIAL FURNACE") == 0);
        const auto liveFlag = static_cast<std::uint8_t>(flag == 255 ? 0 : 255);
        sc.furnace_on = liveFlag; std::strcpy(sc.furnace.name, "LIVE FURNACE");
        sc.furnace.u58[5] = 0x3a; curve->value = 77.123456789; sc.envtmp.value = 99.0;
        CHECK(applyEnvironment3D(sc, *form) == ApplyResult::Completed);
        CHECK(sc.envtmp.value == 1.23457);
        CHECK_EQ(sc.furnace_on, liveFlag); CHECK_EQ(sc.furnace.u58[5], 0x3a);
        CHECK(std::strcmp(sc.furnace.name, "LIVE FURNACE") == 0);
        CHECK(curve->value == 77.123456789);
    }
}

TEST_CASE(environment_timeid_displays_original_tables_without_mutating_model)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    const auto nan = std::numeric_limits<double>::quiet_NaN();
    timeIdTable(sc.timeid, {2, 3, 4, 2, 1, 99, nan, 3}, 2,
        {1.23456789, 9.87654321, -2.34567891, 8, 345.678912, 9,
         4.56789123, 5.67891234, nan, nan, nan, nan, nan, nan, 6.78912345, nan});
    sc.timeid.u54 = 37; sc.timeid.var[0] = 99;
    const auto* axis = sc.timeid.axis[0]; const auto* values = sc.timeid.values;
    const std::vector<double> axesBefore(axis, axis + 8), valuesBefore(values, values + 16);
    const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
    CHECK(form->timeId.differenceBoundaryChecked);
    REQUIRE(form->timeId.betweenTemperatureRows.size() == 2);
    CHECK(form->timeId.betweenTemperatureRows[0][0] == qt::L("1.23457"));
    CHECK(form->timeId.betweenTemperatureRows[0][1] == qt::L("9.87654"));
    CHECK(form->timeId.betweenTemperatureRows[1][0] == qt::L("4.56789"));
    CHECK(form->timeId.betweenTemperatureRows[1][1] == qt::L("5.67891"));
    REQUIRE(form->timeId.belowTemperatureRows.size() == 2);
    CHECK(form->timeId.belowTemperatureRows[0] == qt::L("-2.34568"));
    CHECK(form->timeId.belowTemperatureRows[1] == qt::L("6.78912"));
    REQUIRE(form->timeId.aboveTemperatureRows.size() == 1);
    CHECK(form->timeId.aboveTemperatureRows[0] == qt::L("345.679"));
    CHECK(sc.timeid.axis[0] == axis); CHECK(sc.timeid.values == values);
    CHECK_EQ(sc.timeid.n[0], 8); CHECK_EQ(sc.timeid.ncomp, 2);
    CHECK_EQ(sc.timeid.ndim, 1); CHECK_EQ(sc.timeid.u54, 37); CHECK_EQ(sc.timeid.var[0], 99);
    CHECK(std::memcmp(sc.timeid.axis[0], axesBefore.data(), 8 * sizeof(double)) == 0);
    CHECK(std::memcmp(sc.timeid.values, valuesBefore.data(), 16 * sizeof(double)) == 0);
}

TEST_CASE(environment_table_curves_keep_owned_data_and_skip_constant_validation)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    def::model::DYNARY* curves[]{&sc.envtmp, &sc.cnvcof, &sc.envatm};
    std::array<std::vector<double>, 3> expected;
    for (int i = 0; i < 3; ++i) {
        auto& curve = *curves[i]; def::model::dynary_free_arrays(&curve);
        curve.ndim = i + 1; curve.ncomp = 2; curve.value = 1.23456789 + i;
        curve.u54 = 37 + i; curve.u50 = 47 + i; curve.u58 = 61; curve.u60 = 71;
        for (int d = 0; d < 3; ++d) { curve.n[d] = d <= i ? 2 : 0; curve.var[d] = d + 3; }
        def::model::dynary_alloc(&curve);
        for (int d = 0; d <= i; ++d) {
            REQUIRE(curve.axis[d]); curve.axis[d][0] = d + 0.123456789;
            curve.axis[d][1] = d + 0.987654321;
        }
        const int count = 2 << (i + 1); REQUIRE(curve.values);
        for (int n = 0; n < count; ++n) curve.values[n] = n + 0.123456789;
        curve.values[count - 1] = std::bit_cast<double>(std::uint64_t{0x7ff8000000000042});
        expected[i].assign(curve.values, curve.values + count);
    }
    auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
    for (int i = 0; i < 3; ++i) {
        const auto& controls = form->curveControls[i];
        CHECK(!controls.constantChecked); CHECK(controls.tableChecked);
        CHECK(!controls.constantInputEnabled); CHECK(!controls.constantUnitEnabled);
        CHECK(controls.definitionEnabled);
        REQUIRE(form->curves[i]); CHECK(form->curves[i]->values != curves[i]->values);
        form->constantValues[i] = {qt::L("not a number"), ValidatorState::Acceptable};
        curves[i]->value = 99.0; curves[i]->u54 = 99; curves[i]->u50 = 99;
        curves[i]->u58 = 99; curves[i]->u60 = 99; curves[i]->values[0] = 99;
        curves[i]->axis[0][0] = 99;
    }
    CHECK(applyEnvironment3D(sc, *form) == ApplyResult::Completed);
    for (int i = 0; i < 3; ++i) {
        const auto& curve = *curves[i]; CHECK_EQ(curve.ndim, i + 1); CHECK_EQ(curve.ncomp, 2);
        CHECK(curve.value == 1.23456789 + i); CHECK_EQ(curve.u54, 37 + i); CHECK_EQ(curve.u50, 47 + i);
        CHECK_EQ(curve.u58, 99); CHECK_EQ(curve.u60, 99); // original field-aware copy omissions
        CHECK(curve.values != form->curves[i]->values);
        CHECK(std::memcmp(curve.values, expected[i].data(), expected[i].size() * sizeof(double)) == 0);
        for (int d = 0; d <= i; ++d) {
            CHECK(curve.axis[d] != form->curves[i]->axis[d]); CHECK_EQ(curve.n[d], 2);
            CHECK_EQ(curve.var[d], d + 3); CHECK(curve.axis[d][0] == d + 0.123456789);
            CHECK(curve.axis[d][1] == d + 0.987654321);
        }
    }
}

TEST_CASE(environment_mixed_modes_copy_before_scalar_validation_stop)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.envtmp.value = 1.23456789; sc.envatm.value = 3.45678912; sc.acvcof.value = 4.56789123;
    timeIdTable(sc.cnvcof, {10, 20}, 1, {2.345678912, 9.876543219});
    sc.cnvcof.value = 7.123456789;
    auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(form);
    CHECK(form->curveControls[0].constantChecked); CHECK(!form->curveControls[0].tableChecked);
    CHECK(form->curveControls[0].constantInputEnabled); CHECK(form->curveControls[0].constantUnitEnabled);
    CHECK(!form->curveControls[0].definitionEnabled); CHECK(form->curveControls[1].tableChecked);
    form->constantValues[0] = {qt::L("invalid"), ValidatorState::Invalid};
    form->constantValues[1] = {qt::L("999"), ValidatorState::Acceptable};
    form->constantValues[2] = {qt::L(""), ValidatorState::Intermediate};
    form->scalars[0] = {qt::L("11"), ValidatorState::Acceptable};
    form->scalars[1] = {qt::L("22"), ValidatorState::Acceptable};
    form->scalars[2] = {qt::L("invalid"), ValidatorState::Invalid};
    sc.envtmp.value = sc.cnvcof.value = sc.envatm.value = sc.acvcof.value = 99.0;
    sc.cnvcof.values[0] = 99.0; sc.ehrate = 88.0; sc.peninf = 77.0; sc.vfactr_mode = 9;
    CHECK(applyEnvironment3D(sc, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.envtmp.value == 1.23456789); CHECK(sc.cnvcof.value == 7.123456789);
    CHECK_EQ(sc.cnvcof.ndim, 1); CHECK(sc.cnvcof.values[0] == 2.345678912);
    CHECK(sc.envatm.value == 0.0); CHECK(sc.acvcof.value == 4.56789);
    CHECK(sc.envmpr == 11.0); CHECK(sc.envmpt == 22.0);
    CHECK(sc.ehrate == 88.0); CHECK(sc.peninf == 77.0); CHECK_EQ(sc.vfactr_mode, 9);
}

TEST_CASE(environment_table_profile_accepts_empty_and_guards_unknown_layouts)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.cnvcof.ndim = 1; sc.cnvcof.value = 1.23456789;
    const auto empty = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(empty);
    CHECK(empty->curveControls[1].tableChecked);
    CHECK(applyEnvironment3D(sc, *empty) == ApplyResult::Completed);
    CHECK_EQ(sc.cnvcof.ndim, 1); CHECK(sc.cnvcof.value == 1.23456789);
    CHECK(sc.cnvcof.values == nullptr); CHECK(sc.cnvcof.axis[0] == nullptr);
    timeIdTable(sc.cnvcof, {10, 20}, 1, {1.25, 2.5});
    sc.cnvcof.ndim = 4; CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    sc.cnvcof.ndim = 1; sc.cnvcof.ncomp = (std::numeric_limits<std::int32_t>::max)();
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true)); sc.cnvcof.ncomp = 1;
    auto* axis = sc.cnvcof.axis[0]; sc.cnvcof.axis[0] = nullptr;
    const auto noAxis = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true);
    sc.cnvcof.axis[0] = axis; CHECK(!noAxis);
    sc.cnvcof.n[0] = 0;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true)); sc.cnvcof.n[0] = 2;
    sc.cnvcof.value = std::numeric_limits<double>::infinity();
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    sc.cnvcof.value = 1.25; sc.acvcof.ndim = 1;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true)); sc.acvcof.ndim = 0;
}

TEST_CASE(environment_acvcof_callback_matrix_keeps_existing_temporary_identity)
{
    for (int ndim : {0, 1}) for (int variable : {0, 1, 3, 12, 99})
        for (bool atmosphereTable : {false, true}) {
            engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
            if (ndim) timeIdTable(sc.acvcof, {10, 20}, 1, {1.25, 2.5});
            sc.acvcof.var[0] = variable; sc.acvcof.value = 1.23456789;
            sc.envatm.ndim = atmosphereTable ? 1 : 0;
            auto* oldAxis = sc.acvcof.axis[0]; auto* oldValues = sc.acvcof.values;
            EnvironmentCurveTemporaries owners;
            owners.curves[3]->var[0] = 88; owners.curves[3]->value = 77.0;
            std::array<const def::model::DYNARY*, 4> identities;
            for (int i = 0; i < 4; ++i) identities[i] = owners.curves[i].get();
            const int selection = variable == 3 ? 1 : variable == 12 ? 2 : variable == 1 ? 3 :
                ndim == 0 ? 0 : (atmosphereTable ? 1 : 0);
            const int resultVariables[]{0, 3, 12, 1};
            const auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners);
            REQUIRE(form); CHECK_EQ(form->acvcofControls.selection, selection);
            CHECK(form->acvcofControls.constantInputEnabled == (selection == 0));
            CHECK(form->acvcofControls.constantUnitEnabled == (selection == 0));
            CHECK(form->acvcofControls.definitionEnabled == (selection != 0));
            CHECK_EQ(sc.acvcof.ndim, selection == 0 ? 0 : ndim);
            CHECK_EQ(sc.acvcof.var[0], resultVariables[selection]);
            CHECK(sc.acvcof.axis[0] == oldAxis); CHECK(sc.acvcof.values == oldValues);
            CHECK_EQ(sc.acvcof.n[0], ndim ? 2 : 0); CHECK(sc.acvcof.value == 1.23456789);
            REQUIRE(owners.currentCurveIndex); CHECK_EQ(*owners.currentCurveIndex, 3);
            CHECK_EQ(owners.curves[3]->var[0], resultVariables[selection]);
            CHECK_EQ(owners.curves[3]->ndim, 0); CHECK(owners.curves[3]->value == 77.0);
            for (int i = 0; i < 4; ++i) {
                CHECK(owners.curves[i].get() == identities[i]); CHECK(form->curves[i].get() == identities[i]);
            }
        }
}

TEST_CASE(environment_acvcof_save_uses_owned_curve_and_skips_unselected_text)
{
    for (int ndim : {0, 1}) for (int variable : {1, 3, 12}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        if (ndim) timeIdTable(sc.acvcof, {10, 20}, 2, {1.234567891, 2.345678912, 3.456789123, 4.567891234});
        sc.acvcof.var[0] = variable; sc.acvcof.value = 9.123456789; sc.acvcof.u54 = 37;
        EnvironmentCurveTemporaries owners;
        const def::model::DYNARY* sources[]{&sc.envtmp, &sc.cnvcof, &sc.envatm, &sc.acvcof};
        for (int i = 0; i < 4; ++i) def::model::dynary_copy(owners.curves[i].get(), sources[i]);
        auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners); REQUIRE(form);
        CHECK(form->acvcofControls.selection != 0);
        form->constantValues[3] = {qt::L("not a number"), ValidatorState::Acceptable};
        sc.acvcof.value = 99; sc.acvcof.var[0] = 99; sc.acvcof.u54 = 99;
        if (ndim) sc.acvcof.values[0] = 99;
        CHECK(applyEnvironment3D(sc, *form) == ApplyResult::Completed);
        CHECK_EQ(sc.acvcof.ndim, ndim); CHECK_EQ(sc.acvcof.var[0], variable);
        CHECK(sc.acvcof.value == 9.123456789); CHECK_EQ(sc.acvcof.u54, 37);
        if (ndim) {
            CHECK_EQ(sc.acvcof.ncomp, 2); REQUIRE(sc.acvcof.values);
            CHECK(sc.acvcof.values != owners.curves[3]->values);
            CHECK(sc.acvcof.values[0] == 1.234567891); CHECK(sc.acvcof.values[3] == 4.567891234);
        }
    }
}

TEST_CASE(environment_acvcof_requires_owners_and_collapses_multicomponent_tables)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    timeIdTable(sc.acvcof, {10, 20}, 2, {1, 2, 3, 4}); sc.acvcof.var[0] = 3;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    EnvironmentCurveTemporaries owners; owners.curves[3].reset();
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners));
    owners.curves[3] = owners.curves[0];
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners));
    owners.curves[3] = std::shared_ptr<def::model::DYNARY>(&sc.acvcof, [](auto*) {});
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners));
    owners.curves[3] = std::make_shared<def::model::DYNARY>(); sc.acvcof.var[0] = 99;
    auto* oldAxis = sc.acvcof.axis[0]; auto* oldValues = sc.acvcof.values;
    const auto collapsed = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners);
    REQUIRE(collapsed); CHECK_EQ(collapsed->acvcofControls.selection, 0);
    CHECK_EQ(sc.acvcof.ndim, 0); CHECK_EQ(sc.acvcof.var[0], 0); CHECK_EQ(sc.acvcof.n[0], 2);
    CHECK(sc.acvcof.axis[0] == oldAxis); CHECK(sc.acvcof.values == oldValues);
    CHECK(sc.acvcof.values[3] == 4.0); REQUIRE(owners.currentCurveIndex);
    CHECK_EQ(*owners.currentCurveIndex, 3); CHECK_EQ(owners.curves[3]->ncomp, 1);
    sc.acvcof.ndim = 1; sc.acvcof.var[0] = 99;
    sc.envatm.ndim = 1; // Same unknown variable now selects table/var3, without collapse.
    const auto supported = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners);
    REQUIRE(supported); CHECK_EQ(supported->acvcofControls.selection, 1);
    CHECK_EQ(sc.acvcof.ndim, 1); CHECK_EQ(sc.acvcof.var[0], 3); CHECK_EQ(sc.acvcof.ncomp, 2);
}


TEST_CASE(environment_owned_apply_mutates_curves_in_order_before_scalar_failure)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    def::model::DYNARY* live[]{&sc.envtmp, &sc.cnvcof, &sc.envatm, &sc.acvcof};
    EnvironmentCurveTemporaries owners;
    const double originalConstants[]{30.123456789, 31.123456789, 32.123456789, 33.123456789};
    for (int i = 0; i < 4; ++i) {
        timeIdTable(*live[i], {10, 20}, 2, {1.25, 2.5, 3.75, 4.5});
        live[i]->value = originalConstants[i]; live[i]->var[0] = 3;
        def::model::dynary_copy(owners.curves[i].get(), live[i]);
    }
    auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners); REQUIRE(form);
    form->curveControls[0].constantChecked = true;
    form->curveControls[1].constantChecked = true;
    form->acvcofControls.selection = 0;
    owners.curves[3]->var[0] = 0; sc.acvcof.var[0] = 0; // reached constant-radio callback state
    form->constantValues[0] = {qt::L("101.125"), ValidatorState::Acceptable};
    form->constantValues[1] = {qt::L("invalid"), ValidatorState::Invalid};
    form->constantValues[2] = {qt::L("999"), ValidatorState::Acceptable}; // table radio ignores text
    form->constantValues[3] = {qt::L("104.5"), ValidatorState::Intermediate};
    form->scalars[0] = {qt::L("201"), ValidatorState::Acceptable};
    form->scalars[1] = {qt::L("invalid"), ValidatorState::Invalid};
    std::array<double*, 4> axes, values;
    for (int i = 0; i < 4; ++i) {
        axes[i] = owners.curves[i]->axis[0]; values[i] = owners.curves[i]->values;
        live[i]->value = -1;
    }
    sc.envmpt = 77; sc.ehrate = 88;
    CHECK(applyEnvironment3D(sc, *form, std::nullopt, &owners) == ApplyResult::ValidationStopped);
    for (int i = 0; i < 4; ++i) {
        const auto& owner = *owners.curves[i];
        CHECK(form->curves[i].get() == &owner);
        CHECK(owner.axis[0] == axes[i]); CHECK(owner.values == values[i]);
        CHECK(owner.values[3] == 4.5); CHECK_EQ(owner.n[0], 2); CHECK_EQ(owner.ncomp, 2);
        CHECK_EQ(owner.ndim, i == 2 ? 1 : 0);
        CHECK_EQ(live[i]->ndim, owner.ndim); CHECK_EQ(live[i]->n[0], 2);
        REQUIRE(live[i]->values); CHECK(live[i]->values != owner.values);
        CHECK(live[i]->values[0] == 1.25); CHECK(live[i]->values[1] == 2.5);
        CHECK(live[i]->value == owner.value);
        if (i != 2) CHECK(live[i]->axis[0] == nullptr);
    }
    CHECK(owners.curves[0]->value == 101.125);
    CHECK(owners.curves[1]->value == 31.123456789);
    CHECK(owners.curves[2]->value == 32.123456789);
    CHECK(owners.curves[3]->value == 104.5);
    CHECK(sc.envmpr == 201); CHECK(sc.envmpt == 77); CHECK(sc.ehrate == 88);
}

TEST_CASE(environment_owned_apply_refuses_invalid_owner_binding_before_stores)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    EnvironmentCurveTemporaries owners;
    const def::model::DYNARY* live[]{&sc.envtmp, &sc.cnvcof, &sc.envatm, &sc.acvcof};
    for (int i = 0; i < 4; ++i) def::model::dynary_copy(owners.curves[i].get(), live[i]);
    auto form = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, false, &owners); REQUIRE(form);
    form->constantValues[0] = {qt::L("111"), ValidatorState::Acceptable};
    sc.envtmp.value = 99; owners.curves[0]->value = 77;
    for (int invalid = 0; invalid < 4; ++invalid) {
        auto supplied = owners; auto page = *form;
        if (invalid == 0) supplied.curves[0].reset();
        if (invalid == 1) supplied.curves[0] = supplied.curves[1];
        if (invalid == 2) supplied.curves[0] = std::make_shared<def::model::DYNARY>();
        if (invalid == 3) {
            supplied.curves[0] = std::shared_ptr<def::model::DYNARY>(&sc.envtmp, [](auto*) {});
            page.curves[0] = supplied.curves[0];
        }
        CHECK(applyEnvironment3D(sc, page, std::nullopt, &supplied) == ApplyResult::UnsupportedSnapshot);
        CHECK(sc.envtmp.value == 99); CHECK(owners.curves[0]->value == 77);
    }
}

TEST_CASE(environment_timeid_refill_retains_checked_state_but_replaces_rows)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    timeIdTable(sc.timeid, {1, 2, 3, 4}, 2, {0, 0, 10, 20, 30, 0, 40, 0});
    const auto first = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(first);
    CHECK(first->timeId.differenceBoundaryChecked);
    timeIdTable(sc.timeid, {3}, 1, {55.1234567});
    const auto repeated = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true,
        first->timeId.differenceBoundaryChecked); REQUIRE(repeated);
    CHECK(repeated->timeId.differenceBoundaryChecked);
    CHECK(repeated->timeId.betweenTemperatureRows.empty());
    CHECK(repeated->timeId.aboveTemperatureRows.empty());
    REQUIRE(repeated->timeId.belowTemperatureRows.size() == 1);
    CHECK(repeated->timeId.belowTemperatureRows[0] == qt::L("55.1235"));
    const auto fresh = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(fresh);
    CHECK(!fresh->timeId.differenceBoundaryChecked);
    const auto count = sc.timeid.n[0]; sc.timeid.n[0] = -1;
    const auto empty = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true, true);
    sc.timeid.n[0] = count; REQUIRE(empty);
    CHECK(empty->timeId.differenceBoundaryChecked); CHECK(empty->timeId.belowTemperatureRows.empty());
}

TEST_CASE(environment_timeid_guards_only_reached_values_and_preserves_original_row_overlap)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    timeIdTable(sc.timeid, {2, 3}, 1, {11.2345678, 22.3456789});
    const auto overlap = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true); REQUIRE(overlap);
    REQUIRE(overlap->timeId.betweenTemperatureRows.size() == 1);
    CHECK(overlap->timeId.betweenTemperatureRows[0][0] == qt::L("11.2346"));
    CHECK(overlap->timeId.betweenTemperatureRows[0][1] == qt::L("22.3457"));
    REQUIRE(overlap->timeId.belowTemperatureRows.size() == 1);
    CHECK(overlap->timeId.belowTemperatureRows[0] == qt::L("22.3457"));
    sc.timeid.axis[0][1] = 2;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true)); // final ID2 overrun
    CHECK(sc.timeid.values[1] == 22.3456789);
    sc.timeid.axis[0][0] = 99; sc.timeid.axis[0][1] = 1;
    auto* values = sc.timeid.values; sc.timeid.values = nullptr;
    sc.timeid.ncomp = -17; sc.timeid.ndim = 9;
    const auto ignored = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true);
    sc.timeid.ndim = 1; sc.timeid.ncomp = 1; sc.timeid.values = values;
    REQUIRE(ignored); CHECK(ignored->timeId.differenceBoundaryChecked);
    CHECK(ignored->timeId.betweenTemperatureRows.empty());
    auto* axis = sc.timeid.axis[0]; sc.timeid.axis[0] = nullptr;
    const auto noAxis = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true);
    sc.timeid.axis[0] = axis; CHECK(!noAxis);
    sc.timeid.axis[0][0] = 3; sc.timeid.values = nullptr;
    const auto noValues = fillEnvironment3D(sc, 3, NumericLocale::KnownC, true);
    sc.timeid.values = values; CHECK(!noValues);
    sc.timeid.ncomp = (std::numeric_limits<std::int32_t>::max)();
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    sc.timeid.ncomp = 1; sc.timeid.ndim = 2;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    sc.timeid.ndim = 1; const double before = sc.timeid.values[0];
    sc.timeid.values[0] = std::numeric_limits<double>::infinity();
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    sc.timeid.values[0] = before;
    CHECK_EQ(sc.timeid.n[0], 2); CHECK(sc.timeid.values[1] == 22.3456789);
}

TEST_CASE(environment_unrecovered_profiles_refuse_before_mutating_input)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    CHECK(!fillEnvironment3D(sc, 2, NumericLocale::KnownC, true));
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::Unrecovered, true));
    sc.furnace_on = 1; sc.timeid.n[0] = 1;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    CHECK_EQ(sc.furnace_on, 1); sc.timeid.n[0] = 0;
    sc.acvcof.var[0] = 3;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true)); CHECK_EQ(sc.acvcof.var[0], 3);
    sc.acvcof.var[0] = 0; sc.cnvcof.ndim = 4;
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    CHECK_EQ(sc.furnace_on, 1); sc.cnvcof.ndim = 0;
    sc.envmpr = (std::numeric_limits<double>::infinity)();
    CHECK(!fillEnvironment3D(sc, 3, NumericLocale::KnownC, true));
    CHECK(fillEnvironment3D(sc, 3, NumericLocale::KnownC, false).has_value());
}

TEST_CASE(advanced_applies_remembered_text_flags_radios_and_secondary_die)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(2, true, false);
    field<int>(object, 0x87c) = 1; sc.pdie2 = 2;
    sc.tnow1 = 1.2345678; sc.tnow2 = 2.3456789; sc.tnow3 = 1;
    for (int i = 0; i < 4; ++i) sc.geoerr[i] = (i + 1) * 0.0012345678;
    std::strcpy(sc.usrdef[0], "REMEMBERED VARIABLE");
    for (int i = 0; i < 7; ++i) sc.stnout[i] = i % 2 ? -8 : 0;
    sc.elmnod[0] = 0; sc.elmnod[1] = 2; sc.elmnod[2] = 3;
    const auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(form->time[0].text == qt::L("1.23457"));
    CHECK(form->geometryErrorPercent[0].text == qt::L("0.123457"));
    CHECK(form->outputFlags[4]); CHECK_EQ(form->secondaryDieNumber, 2);
    sc.tnow1 = sc.tnow2 = 99.0; sc.tnow3 = 0; sc.pdie2 = 0;
    for (double& value : sc.geoerr) value = 99.0;
    std::strcpy(sc.usrdef[0], "LATEST MODEL");
    for (int& flag : sc.stnout) flag = 77;
    for (int& option : sc.elmnod) option = 77;
    sc.frqncy.values[2] = 99.5; // Save never re-applies remembered frequency text
    CHECK(applyAdvanced3D(sc, model, *form) == ApplyResult::Completed);
    CHECK(sc.tnow1 == 1.23457); CHECK(sc.tnow2 == 2.34568); CHECK_EQ(sc.tnow3, 1);
    CHECK(sc.geoerr[0] == 0.123457 / 100.0); CHECK(sc.geoerr[3] == 0.493827 / 100.0);
    CHECK(std::strcmp(sc.usrdef[0], "REMEMBERED VARIABLE") == 0);
    CHECK_EQ(sc.stnout[0], 0); CHECK_EQ(sc.stnout[1], 1); CHECK_EQ(sc.stnout[4], 1);
    CHECK_EQ(sc.elmnod[0], 0); CHECK_EQ(sc.elmnod[1], 2); CHECK_EQ(sc.elmnod[2], 3);
    CHECK_EQ(sc.elmnod[3], 77); CHECK_EQ(sc.pdie2, 2); CHECK(sc.frqncy.values[2] == 99.5);
}

TEST_CASE(advanced_invalid_returns_keep_exact_earlier_stores_and_skip_final_remap)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.tnow1 = 1.25; sc.tnow2 = 2.5; sc.tnow3 = 1;
    sc.geoerr[0] = 0.125; sc.geoerr[1] = 0.25;
    auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    model.step->u15f8[0] = 1234;
    form->time[1] = {qt::L("bad"), ValidatorState::Invalid};
    sc.tnow1 = 99.0; sc.tnow2 = 98.0; sc.tnow3 = 0;
    CHECK(applyAdvanced3D(sc, model, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.tnow1 == 1.25); CHECK(sc.tnow2 == 98.0); CHECK_EQ(sc.tnow3, 0);
    CHECK_EQ(model.step->u15f8[0], 1234);
    form->time[1] = {qt::L(""), ValidatorState::Intermediate};
    form->geometryErrorPercent[1] = {qt::L("bad"), ValidatorState::Invalid};
    sc.geoerr[0] = 99.0; sc.geoerr[1] = 98.0; sc.stnout[4] = 0;
    std::strcpy(sc.usrdef[0], "LATEST");
    CHECK(applyAdvanced3D(sc, model, *form) == ApplyResult::ValidationStopped);
    CHECK(sc.tnow2 == 0.0); CHECK_EQ(sc.tnow3, 1); CHECK(sc.geoerr[0] == 0.125);
    CHECK(sc.geoerr[1] == 98.0); CHECK_EQ(sc.stnout[4], 0);
    CHECK(std::strcmp(sc.usrdef[0], "LATEST") == 0); CHECK_EQ(model.step->u15f8[0], 1234);
}

TEST_CASE(advanced_final_remap_rebuilds_widths_and_allocates_live_object_state)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* object = model.objectWrap(1, true, false);
    auto& nodes = def::model::llobj::parts::part_8a0(*object);
    nodes.alloc(2, model.step->u15f8);
    for (int i = 0; i < 6; ++i) nodes.u008[i] = i + 0.25;
    sc.elmnod[0] = 2; model.step->u33c8 = 1;
    auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    sc.elmnod[0] = 0; model.step->u15f8[0] = 1;
    CHECK(applyAdvanced3D(sc, model, *form) == ApplyResult::Completed);
    CHECK_EQ(model.step->u15f8[0], 3); CHECK_EQ(model.step->u15f8[0x1c], 1);
    CHECK_EQ(nodes.count(), 2);
    for (int i = 0; i < 6; ++i) CHECK(nodes.u008[i] == i + 0.25);
    bool found = false;
    for (const auto& item : def::model::Cls_14017ef20::kRegs) if (item.id == 0x1c) {
        auto* values = *reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&nodes) + item.slot);
        REQUIRE(values != nullptr); CHECK(values[0] == 0.0); CHECK(values[1] == 0.0); found = true;
    }
    CHECK(found);
}

TEST_CASE(advanced_frequency_snapshot_retains_only_original_populated_text)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    sc.frqncy.values[0] = -1.0; sc.frqncy.values[1] = 0.0;
    sc.frqncy.values[2] = 2147483647.0;
    const auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK_EQ(form->frequencyCombo[0], 1); CHECK_EQ(form->frequencyCombo[1], 0);
    CHECK_EQ(form->frequencyCombo[2], 1); CHECK(!form->populatedFrequencyText[0]);
    CHECK(!form->populatedFrequencyText[1]); REQUIRE(form->populatedFrequencyText[2]);
    CHECK(*form->populatedFrequencyText[2] == qt::L("2147483647"));
    sc.frqncy.values[0] = 1.0;
    auto positive = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(positive.has_value());
    CHECK_EQ(positive->frequencyCombo[0], 2);
    for (double value : {-0.0, -2.0, 1.5, 2147483648.0,
             (std::numeric_limits<double>::infinity)()}) {
        sc.frqncy.values[0] = value; CHECK(!fillAdvanced3D(sc, model, NumericLocale::KnownC));
    }
}

TEST_CASE(advanced_secondary_die_uses_mesh_eligibility_and_missing_number_falls_back)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto* first = model.objectWrap(1, true, false); field<int>(first, 0x87c) = 0;
    auto* second = model.objectWrap(2, true, false); field<int>(second, 0xc) = 5;
    sc.pdie2 = 1;
    auto excluded = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(excluded.has_value());
    CHECK_EQ(excluded->secondaryDieNumber, 0);
    sc.pdie2 = 2;
    auto selected = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(selected.has_value());
    CHECK_EQ(selected->secondaryDieNumber, 2);
    field<int>(second, 0xc) = 7;
    auto undefined = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(undefined.has_value());
    CHECK_EQ(undefined->secondaryDieNumber, 0);
}

TEST_CASE(advanced_unrecovered_profiles_refuse_without_stores)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
    auto form = fillAdvanced3D(sc, model, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(!fillAdvanced3D(sc, model, NumericLocale::Unrecovered));
    sc.elmnod[0] = 1; CHECK(!fillAdvanced3D(sc, model, NumericLocale::KnownC)); sc.elmnod[0] = 0;
    sc.usrdef[0][0] = static_cast<char>(0x80);
    CHECK(!fillAdvanced3D(sc, model, NumericLocale::KnownC)); sc.usrdef[0][0] = 0;
    sc.geoerr[0] = (std::numeric_limits<double>::max)();
    CHECK(!fillAdvanced3D(sc, model, NumericLocale::KnownC)); sc.geoerr[0] = 0.0;
    sc.frqncy_rows = 5; CHECK(!fillAdvanced3D(sc, model, NumericLocale::KnownC)); sc.frqncy_rows = 6;
    auto* object = model.objectWrap(1, true, false); field<int>(object, 0x21c8) = 6;
    CHECK(!fillAdvanced3D(sc, model, NumericLocale::KnownC)); sc.tnow1 = 99.0;
    CHECK(applyAdvanced3D(sc, model, *form) == ApplyResult::UnsupportedSnapshot);
    CHECK(sc.tnow1 == 99.0);
    field<int>(object, 0xc) = 7; // time callback skips undefined objects even with kind6
    CHECK(fillAdvanced3D(sc, model, NumericLocale::KnownC).has_value());
}

int main() { return testkit::run_all(); }
