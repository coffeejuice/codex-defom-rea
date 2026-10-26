// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../../src/engine/remesh_invalid_parameter_model.hpp"
#include <cerrno>
#include <string_view>

using namespace m2m::engine::simcontrol300::detail;

TEST_CASE(remesh_unknown_handler_models_errno_before_decode_without_default_prediction)
{
    const auto model = modelFirstNullFilePrintf(InvalidParameterEvidence::Unknown);
    CHECK_EQ(model.sourceAddress, 0x1400abcf0ULL); CHECK_EQ(model.formatAttemptOrdinal, 1U);
    CHECK_EQ(model.modeledErrno, 22);
    CHECK(model.branch == InvalidParameterBranch::Unknown);
    CHECK(model.stage == InvalidParameterStage::ErrnoWritten);
    CHECK(model.next == InvalidParameterNext::DecodeHandlerUnavailable);
    CHECK(model.qualification == InvalidParameterQualification::SourceModelOnly);
    REQUIRE(model.effectCount == 2);
    CHECK(model.effects[0] == InvalidParameterModeledEffect::FirstNullFileChecked);
    CHECK(model.effects[1] == InvalidParameterModeledEffect::ErrnoWritten);
    CHECK(!model.modeledDebugStore); CHECK(!model.predictedProcessStatus);
}

TEST_CASE(remesh_static_default_models_ordered_debug_prefix_then_stops_before_capture)
{
    const auto model = modelFirstNullFilePrintf(InvalidParameterEvidence::StaticFreshDefaultAssumption);
    CHECK(model.evidence == InvalidParameterEvidence::StaticFreshDefaultAssumption);
    CHECK(model.branch == InvalidParameterBranch::Default);
    CHECK(model.qualification == InvalidParameterQualification::StaticDefaultConditional);
    CHECK(model.stage == InvalidParameterStage::DefaultPrefixComplete);
    CHECK(model.next == InvalidParameterNext::CaptureContextUnimplemented);
    REQUIRE(model.effectCount == 4);
    CHECK(model.effects[0] == InvalidParameterModeledEffect::FirstNullFileChecked);
    CHECK(model.effects[1] == InvalidParameterModeledEffect::ErrnoWritten);
    CHECK(model.effects[2] == InvalidParameterModeledEffect::DecodedNullSelected);
    CHECK(model.effects[3] == InvalidParameterModeledEffect::DebugFlagCleared);
    REQUIRE(model.modeledDebugStore);
    CHECK_EQ(model.modeledDebugStore->address, 0x1450540e8ULL); CHECK_EQ(model.modeledDebugStore->value, 0U);
    REQUIRE(model.predictedProcessStatus); CHECK_EQ(*model.predictedProcessStatus, 0xc0000417U);
    CHECK(std::string_view(invalidParameterEvidenceName(model.evidence)) == "StaticFreshDefaultAssumption");
    CHECK(std::string_view(invalidParameterStageName(model.stage)) == "DefaultPrefixComplete");
    CHECK(std::string_view(invalidParameterNextName(model.next)) == "CaptureContextUnimplemented");
}

TEST_CASE(remesh_witnessed_null_keeps_provenance_distinct_and_native_prelude_guarded)
{
    const auto witnessed = modelFirstNullFilePrintf(InvalidParameterEvidence::SameOriginalModuleDecodedNull);
    const auto assumed = modelFirstNullFilePrintf(InvalidParameterEvidence::StaticFreshDefaultAssumption);
    CHECK(witnessed.evidence != assumed.evidence);
    CHECK(witnessed.qualification == InvalidParameterQualification::DecodedValueWitnessOnly);
    CHECK(witnessed.branch == InvalidParameterBranch::Default);
    CHECK(witnessed.stage == InvalidParameterStage::DefaultPrefixComplete);
    CHECK(witnessed.next == InvalidParameterNext::CaptureContextUnimplemented);
    CHECK(witnessed.effects == assumed.effects); CHECK_EQ(witnessed.effectCount, 4U);
    CHECK(witnessed.modeledDebugStore == assumed.modeledDebugStore);
    CHECK(witnessed.predictedProcessStatus == assumed.predictedProcessStatus);
    // Witnessed input class is a hypothetical pure-model input, not an original
    // observation made by this test; capture/filter/termination remain pending.
}

TEST_CASE(remesh_custom_handler_stops_at_call_without_default_debug_store_or_status)
{
    const auto model = modelFirstNullFilePrintf(InvalidParameterEvidence::SameOriginalModuleDecodedCustom);
    CHECK_EQ(model.modeledErrno, 22);
    CHECK(model.branch == InvalidParameterBranch::Custom);
    CHECK(model.stage == InvalidParameterStage::CustomDispatchPending);
    CHECK(model.next == InvalidParameterNext::InvokeCustomHandlerUnimplemented);
    CHECK(model.qualification == InvalidParameterQualification::DecodedValueWitnessOnly);
    REQUIRE(model.effectCount == 3);
    CHECK(model.effects[0] == InvalidParameterModeledEffect::FirstNullFileChecked);
    CHECK(model.effects[1] == InvalidParameterModeledEffect::ErrnoWritten);
    CHECK(model.effects[2] == InvalidParameterModeledEffect::DecodedCustomSelected);
    CHECK(!model.modeledDebugStore); CHECK(!model.predictedProcessStatus);
}

TEST_CASE(remesh_invalid_handler_evidence_conservatively_retains_unknown_decode_guard)
{
    const auto invalid = modelFirstNullFilePrintf(static_cast<InvalidParameterEvidence>(99));
    CHECK(invalid == modelFirstNullFilePrintf(InvalidParameterEvidence::Unknown));
    CHECK(invalid.evidence == InvalidParameterEvidence::Unknown);
    CHECK(!invalid.predictedProcessStatus); CHECK(!invalid.modeledDebugStore);
}

TEST_CASE(remesh_handler_model_repeat_evaluation_never_changes_host_errno)
{
    for (const auto evidence : {InvalidParameterEvidence::Unknown,
             InvalidParameterEvidence::StaticFreshDefaultAssumption,
             InvalidParameterEvidence::SameOriginalModuleDecodedNull,
             InvalidParameterEvidence::SameOriginalModuleDecodedCustom}) {
        for (const int entry : {0, EDOM, ERANGE}) {
            errno = entry;
            const auto first = modelFirstNullFilePrintf(evidence);
            const auto repeated = modelFirstNullFilePrintf(evidence);
            const int after = errno;
            CHECK_EQ(after, entry); CHECK(first == repeated); CHECK_EQ(first.modeledErrno, 22);
        }
    }
}

int main() { return testkit::run_all(); }
