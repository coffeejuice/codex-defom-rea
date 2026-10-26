# Atomic runtime observation patch for style24 image state

Implement the runtime binding as a separate patch before the editor hook. It
will populate the existing image helper from qualified actual brush/image
observations, without advancing the editor or admitting pixmap/conversion/active
copy/public300 paths. This plan makes the batch conflict boundary and receiver
alias access explicit.

Owner: `gradient_state`. Exclusive write: this report. Checkpoint51 validation
was reported passed by the orchestrator; source writes remain on hold. No source
edits, builds, GUI control, original execution or mutable REA were performed.
Token measurement unavailable.

Read qualifications in `analysis/resume48-brush-style24-contract.md`,
`analysis/resume50-style24-runtime-binding.md` and
`analysis/resume51-style24-editor-hook.md`. Native field/read order and absent
actual-instance style24 observations remain as those reports state. This plan
does not add guessed native defaults.

Current shared files reviewed, SHA-256:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/engine/view_manager.hpp` | `9dbc2dc6994df6283e2175711fd01990fd27e83081cb93b88b4c7d92187b5bb1` |
| `src/engine/view_manager.cpp` | `e899f00a26020aeec62d5a4ef45857a95b1e9758e0c6669692aec921eba9a6f4` |
| `include/m2m/engine/brush_texture_image.hpp` | `1d11f86a79eb36573e855f9f2ea8e4cac17e8a95b669a2d44f0696d48a4348f0` |
| `src/engine/brush_texture_image.cpp` | `0c6e36bfb02695db032fe07dd55cbc909898efe1acf99769285c78db0b8e7bb3` |

## Exact helper preflight addition

Add these public internal methods to the existing helper states:

```cpp
bool BrushTextureImageDataState::canObserve(
    const BrushTextureImageDataObservation&) const noexcept;
bool BrushTextureImageState::canObserve(
    const BrushTextureImageObservation&) const noexcept;
```

Move each current `observe` conflict predicate into its const `canObserve`.
`observe` first calls `canObserve`, returning false before any merge on failure;
otherwise retain the current unknown-to-known/equal merges. Preflight never
changes fields, helper cursor/effects, or `temporaryValueReferences`. It compares
source data object **and control block** as the current source predicate does.
It accepts explicit known-null pointers and rejects nonnull image-data owners
with alias0. No format/flag/painting values are synthesized or normalized.

These two nonmutating methods eliminate any need to copy transient reference
ledgers or replace existing owners with trial objects. No new image getters are
required for the runtime adapter. Test the methods independently, including a
data owner retained by a pending helper, before relying on batch preflight.

## Exact observation surface

Add `brush_texture_image.hpp` to `view_manager.hpp`. Use dedicated aliases and
an alias-only observation envelope, with names below:

```cpp
using WidgetTextureImageDataAlias = std::uint32_t;
struct WidgetTextureImageDataObservation {
    WidgetTextureImageDataAlias alias = 0;
    BrushTextureImageDataObservation values;
};
struct WidgetBrushTextureImageObservation {
    WidgetBrushAlias brushAlias = 0;
    std::optional<bool> pixmapSelected, cachedPixmapPresent;
    std::optional<std::uint16_t> imagePaintingActiveWord;
    std::optional<WidgetTextureImageDataAlias> imageDataAlias;
};
struct WidgetTextureImageObservation {
    std::weak_ptr<WidgetState> receiver;
    std::string qualification;
    std::vector<WidgetTextureImageDataObservation> imageData;
    std::vector<WidgetBrushTextureImageObservation> brushes;
};
```

Disengaged `imageDataAlias` is unknown; engaged0 is explicitly null; positive
resolves in the dedicated runtime image-data map. The envelope must not accept
arbitrary helper-state shared_ptr objects supplied by callers.

Add `WidgetBrushDataState::textureImageSource() const noexcept`, returning
`shared_ptr<const BrushTextureImageState>`, backed by private
`shared_ptr<BrushTextureImageState> textureImage_`. The actual brush owns that
embedded source state; it is not stored in the editor or keyed independently
from its brush. Distinct brushes may share an image-data owner. Existing shared
brushes expose exactly the same source payload to every palette alias.

Add runtime methods:

```cpp
void observeTextureImageInputs(const std::shared_ptr<WidgetState>&,
    const WidgetTextureImageObservation&);
std::shared_ptr<const BrushTextureImageDataState> textureImageDataFor(
    const WidgetState&, WidgetTextureImageDataAlias) const noexcept;
```

Keep startup/private-construction observation structures unchanged: this dedicated
method refines the existing runtime brush registry after private input binding,
as the gradient/radial observation methods already do. It does not call resume,
select a palette group or rewrite brush style/alpha.

## Runtime maps and receiver ownership

In `WidgetRuntimeState::Values`, add:

```cpp
std::map<WidgetTextureImageDataAlias,
    std::shared_ptr<BrushTextureImageDataState>> textureImageData;
struct TextureImageReceiver {
    std::weak_ptr<WidgetState> widget;
    std::set<WidgetTextureImageDataAlias> aliases;
};
std::map<const WidgetState*, TextureImageReceiver> textureImageReceivers;
```

The alias map is runtime-wide because two actual brush values may share ID.
Each receiver's alias set contains only positive IDs explicitly declared in its
qualified data entries or positively linked by its qualified brush entries.
An empty envelope must not grant access to every previously interned image.
Keep previous aliases only when the stored weak receiver locks to the same
object/control block; a stale raw-address record cannot transfer access to a
new receiver. Alias0 is a null-value marker and is never interned or added to a
receiver set.

For writes, perform the same actual receiver checks as `observeGradientInputs`:
weak receiver lock; same raw object and control block as the argument; live
native owner; matching component owner; same runtime; existing private input;
nonempty qualification equal to that receiver's private-input qualification.
An explicit brush entry is a qualified assertion about an existing actual
style24 brush alias in this runtime, following the current gradient/radial
receipt model. Do not admit an absent or non-style24 brush merely because its
numeric alias exists in another runtime.

For `textureImageDataFor`, require live same-runtime private inputs, a matching
live receiver record, and membership in that receiver's alias set before map
lookup. Return null for alias0, absent, stale/dead/foreign receiver or unqualified
alias. A second widget must explicitly observe/link an alias to use this
diagnostic lookup. Its actual palette-selected brush can still reference an
already-bound shared payload: native shared data ownership is separate from
diagnostic receiver access. No receiver-specific image symbol is constructed.

## Ordered preflight and commit algorithm

All allocations and all checks precede mutations to existing owners:

1. Validate receiver and qualification. Copy the runtime data map and receiver
   map into staged maps; prepare a staged receiver alias set, starting from the
   matching live record only. These host copies add no native QImage refs.
2. Process `imageData` with a per-envelope uniqueness set. Reject alias0 or
   duplicates. Resolve existing actual data owners; create new data objects only
   in the staged map. Call `actual->canObserve(values)` for every row. Store
   planned `{actual, values}` updates without invoking `observe`. Add declared
   positive aliases to the staged receiver set.
3. Process `brushes` with its own uniqueness set. Require positive unique brush
   aliases, existing actual brush owners and known style24. Resolve each positive
   imageDataAlias only against the staged data map, so same-envelope new data is
   usable. Absent alias leaves `BrushTextureImageObservation::imageData`
   disengaged; alias0 supplies an engaged null shared_ptr. Populate the remaining
   primitive fields exactly as supplied. Positively linked aliases join the
   staged receiver set.
4. For each existing brush payload, call `payload->canObserve(proposal)` and
   store its planned update. For a missing payload, create/stage one and preflight
   its proposal, but do not assign it to the live brush yet. Do not clone an
   existing payload or existing data object. Reject any attempted known-null to
   nonnull, nonnull to null, owner replacement or conflicting primitive field.
5. Finish all staging, including the receiver record/map insertion and update
   storage. At this point every failure can return/throw the existing unsupported
   observation error with no runtime alias additions, metadata changes, source
   links or receiver grants committed.
6. Commit in a nonfailing normal path: invoke the already-preflighted noexcept
   `observe` merges on existing/new data and payload owners; swap staged data
   and receiver maps into `Values`; assign any newly staged payloads to their
   actual brush owners using noexcept shared_ptr assignment. No allocation,
   callback, constructor continuation or helper `resume` belongs in this phase.

The converter serializes its runtime activity; no other owner mutates these
states between preflight and commit. Therefore each checked merge must remain
valid. Do not replace a failed preflight with a partial update followed by an
unsupported error. This is observation admission atomicity, not a change to
the reconstructed original file-save failure semantics.

Known cache nullness is monotonic in this restricted profile. A future native
toImage/fromImage mutation requires a new source-qualified state transition;
it cannot be smuggled through an observation replacing a known pointer.

## Tests and integration sequence

First land helper preflight in its exclusive helper/header/test files. Then
serialize the shared runtime patch in `view_manager.hpp/.cpp` and a new
`tests/mdt/test_widget_texture_image_binding.cpp`. Keep editor files untouched
until that binding is frozen and its focused tests pass; then apply the existing
`resume51-style24-editor-hook.md` cursor plan. No CMake edit is needed because
the converter already discovers helper/test source files through globs.

Focused runtime cases:

- Same qualified live receiver; empty/mismatched qualification; absent private
  input; foreign runtime; forged receiver control block; dead receiver.
- Positive unique data aliases; duplicate/zero/missing aliases; duplicate brush
  rows; unknown brush or style1/16 brush; same-envelope new-data link.
- Unknown->known and equal observation; conflicting format/flag; conflicting
  primitive source field; explicit-null pointer versus unknown; owner replacement.
- Late invalid brush row after an otherwise valid existing metadata refinement
  and new alias: verify old metadata, alias registry, brush links **and receiver
  access grants** all remain unchanged. Repeat with a valid earlier source-field
  refinement followed by a later bad source link.
- Shared brush means identical payload. Distinct brushes share ID without sharing
  embedded image values. Two pending helper calls on shared ID retain separate
  temporary refs; runtime preflight/observe changes none of those ref counts.
- Receiver A observation does not grant receiver B diagnostic alias access. Empty
  B envelope grants none; explicit B data/link receipt grants only those aliases
  and preserves the same runtime data owner. Destroyed A exposes no aliases.
- Observation-only test snapshots the existing editor report/effects and QImage
  ref ledger; observe must leave them unchanged without calling resume.
- Known pixmap selection, cache-conversion requirement and active-painting words
  may be represented but remain the existing helper guards. No fabricated
  opacity/has-alpha result input and no public300 admission.

The orchestrator owns configure/build/focused/regression execution and the next
source freeze. This plan supplies no executed test result or original style24
instance acceptance.
