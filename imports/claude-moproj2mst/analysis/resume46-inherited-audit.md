# Resume 46: inherited XML/property omission audit

## Finding

Checkpoint 45 does not close closure item C13 (`RINHERIT`). The inherited audit's
candidate list is a discovery queue, not a set of proven defects. The best next
step is a source-bound reachability pass for each listed candidate, followed by
an original caller/branch trace only for candidates that feed the selected
fresh-project Open → Save → Close lifecycle. Do not implement from comments,
missing strings, or the inherited ledger alone.

## Settled or narrowed by later evidence

- **Save widget/property dispatch:** `wave3-save-dispatch-validation.json`
  records the recovered original Save widget order, the Boolean reselection
  helper, backup initialization, and DOE StateVar prefix with focused tests.
  This narrows the broad “missing property-save dispatch” hypothesis. It does
  not establish full property-family coverage: the validation explicitly says
  Boolean reselection is not wired to public session event delivery and
  StateVar remains guarded before post-parse display integration. The five
  historical folder differences (including DOE cases) remain differences, not
  closed behavior.
- **Boolean Properties:** it is included in the recovered Save dispatch slice;
  retain this as a settled dispatch-order slice only. The inherited static
  candidates at original `0x1411e1820` (save) and `0x1411e1cd0` (load) still need
  caller/serialization coverage if the next trace shows an uncovered reachable
  effect.
- **Connector class flags:** the independent connector review resolved the
  concrete Cycle and DoeOutput flag omissions (`0x301b`, `0x20097`) for all 28
  currently registered classes. Group and CopyMirror registration/lookup are
  explicitly separate open questions. This is not XML grammar or property-save
  closure.
- **DOE:** checkpoint 45 validated a bounded Session-owned task-factory prefix,
  not DOE completion. The 150-test receipt says objective incomplete, lists five
  historical DIFFs, and leaves active DOE, colorbar cloning and TLS/shared-owner
  evidence for checkpoint 46.

## Still live, in scope, and not established as defects yet

The following candidates are present in the retained static audit, but no
checkpoint 45 artifact binds them to a complete reachable caller and observed
effect:

1. **Convert2Dto3D / CopyMirror property-to-engine application during MST-time
   load** (`closure-audit.json` identifies `Convert2Dto3DProperties +0x60` and
   `CopyMirrorProperties +0x60`; the paired `+0x58` methods are explicitly
   marked not called by their writers). Determine the actual `+0x280` load path,
   ordering relative to property parsing, and whether the result survives into
   MST output or affects Save. The separate explicit refusals for geometry,
   missing second model, and editor modes do not prove these calls unreachable.
2. **Convert status files and recovery-list filling.** The audit marks the
   SimCtrl.PPV recovery-list writer as refused and records recovery-list sites.
   Establish whether normal supported project inputs reach these writers before
   deciding if refusal is an in-scope omission.
3. **XML grammar and UI-derived state.** `dom.cpp` supports a narrower grammar
   than the original XML library, and the audit flags comments about omitted UI
   updates. Compare accepted grammar and DOM mutations only on reachable
   lifecycle paths; comments are not behavioral evidence.
4. **Group `Expanded` policy.** The inherited implementation chooses `yes`,
   while the original value is reported uninitialized. Keep this as a known
   nondeterministic/deviation item unless original evidence supplies a defined
   value; do not silently bless the inherited constant or invent a deterministic
   contract.

Explicitly refused families (DOE variables/constraints/studies, HeatTreat
records, geometry-list boxes, scene subobjects, perspective projection, and
Cogging reheats) are already classified separately in `closure-audit.md`.
They remain objective gaps where reachable, but are not silent omissions and
should not be counted as C13 defects.

## Evidence-backed next action

Create a bounded static crosswalk for the four candidate groups above: source
function and hash, original address, recovered direct/indirect caller, operation
or property ownership, selected-lifecycle reachability, emitted file/state
effect, current port/refusal, and evidence status. Start with the two paired
Convert2Dto3D/CopyMirror `+0x60` versus `+0x58` calls because the audit already
distinguishes called from uncalled methods and names the operation load slot.
Use preserved original decompilation/call sites; do not launch the GUI or mutate
REA state. If a candidate is reachable but its effect is unknown, record the
smallest original branch/state query needed. If unreachable within the agreed
boundary, record the caller-chain proof and close only that candidate. Keep C13
open until all candidates are individually dispositioned and the `Expanded`
deviation is explicit.

## Qualifications

Checkpoint 45's 143 pass, 6 logged-gap, and 1 logged-difference counts are test
status counts, not original parity. Its validation states that no original
executable was launched and historical oracles are not fresh-session acceptance
evidence. `closure-reconciliation-checkpoint35.json` is the inherited queue
record; its C13 row points to earlier Save-dispatch evidence and still says
“qualified discovery; reachable omissions open.” No checkpoint45 reconciliation
artifact supersedes that row.
