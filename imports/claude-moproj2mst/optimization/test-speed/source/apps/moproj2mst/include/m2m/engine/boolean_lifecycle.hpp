#pragma once

namespace m2m {
struct Project;
struct Operation;
namespace engine {
struct FileEffects;

// Bounded same-current, shown Object editor replacement for a Type0 Boolean
// operation: 038950 -> row0 -> 5f36d0 -> 11e0510. Uses the actual class hooks.
// This is not timer readiness, scene fitting, general tree selection, or the
// complete GUI notification chain. Other classes/states are explicitly refused.
// If the old editor is not shown, refusal follows the original current-object
// zero prefix; the unqualified index0 propsToEngine branch is not inferred.
void reselectBooleanObjectEditor(Project&, Operation&, FileEffects&);

}  // namespace engine
}  // namespace m2m
