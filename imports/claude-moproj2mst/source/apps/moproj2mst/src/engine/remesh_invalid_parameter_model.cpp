#include "remesh_invalid_parameter_model.hpp"

namespace m2m::engine::simcontrol300::detail {

RemeshInvalidParameterModel modelFirstNullFilePrintf(InvalidParameterEvidence evidence) noexcept
{
    RemeshInvalidParameterModel model;
    model.effects[model.effectCount++] = InvalidParameterModeledEffect::FirstNullFileChecked;
    model.effects[model.effectCount++] = InvalidParameterModeledEffect::ErrnoWritten;
    switch (evidence) {
    case InvalidParameterEvidence::StaticFreshDefaultAssumption:
    case InvalidParameterEvidence::SameOriginalModuleDecodedNull:
        model.evidence = evidence;
        model.branch = InvalidParameterBranch::Default;
        model.qualification = evidence == InvalidParameterEvidence::StaticFreshDefaultAssumption
            ? InvalidParameterQualification::StaticDefaultConditional
            : InvalidParameterQualification::DecodedValueWitnessOnly;
        model.effects[model.effectCount++] = InvalidParameterModeledEffect::DecodedNullSelected;
        model.effects[model.effectCount++] = InvalidParameterModeledEffect::DebugFlagCleared;
        model.modeledDebugStore = InvalidParameterDebugStore{0x1450540e8ULL, 0};
        model.stage = InvalidParameterStage::DefaultPrefixComplete;
        model.next = InvalidParameterNext::CaptureContextUnimplemented;
        // Source prediction under normally returning native prelude + successful
        // Win32 termination; reaching this model stage is not that observation.
        model.predictedProcessStatus = 0xc0000417U;
        break;
    case InvalidParameterEvidence::SameOriginalModuleDecodedCustom:
        model.evidence = evidence;
        model.branch = InvalidParameterBranch::Custom;
        model.qualification = InvalidParameterQualification::DecodedValueWitnessOnly;
        model.effects[model.effectCount++] = InvalidParameterModeledEffect::DecodedCustomSelected;
        model.stage = InvalidParameterStage::CustomDispatchPending;
        model.next = InvalidParameterNext::InvokeCustomHandlerUnimplemented;
        break;
    case InvalidParameterEvidence::Unknown:
    default:
        break;
    }
    return model;
}

const char* invalidParameterEvidenceName(InvalidParameterEvidence evidence) noexcept
{
    switch (evidence) {
    case InvalidParameterEvidence::Unknown: return "Unknown";
    case InvalidParameterEvidence::StaticFreshDefaultAssumption: return "StaticFreshDefaultAssumption";
    case InvalidParameterEvidence::SameOriginalModuleDecodedNull: return "SameOriginalModuleDecodedNull";
    case InvalidParameterEvidence::SameOriginalModuleDecodedCustom: return "SameOriginalModuleDecodedCustom";
    }
    return "Invalid";
}
const char* invalidParameterStageName(InvalidParameterStage stage) noexcept
{
    switch (stage) {
    case InvalidParameterStage::ErrnoWritten: return "ErrnoWritten";
    case InvalidParameterStage::CustomDispatchPending: return "CustomDispatchPending";
    case InvalidParameterStage::DefaultPrefixComplete: return "DefaultPrefixComplete";
    }
    return "Invalid";
}
const char* invalidParameterNextName(InvalidParameterNext next) noexcept
{
    switch (next) {
    case InvalidParameterNext::DecodeHandlerUnavailable: return "DecodeHandlerUnavailable";
    case InvalidParameterNext::InvokeCustomHandlerUnimplemented: return "InvokeCustomHandlerUnimplemented";
    case InvalidParameterNext::CaptureContextUnimplemented: return "CaptureContextUnimplemented";
    }
    return "Invalid";
}

} // namespace m2m::engine::simcontrol300::detail
