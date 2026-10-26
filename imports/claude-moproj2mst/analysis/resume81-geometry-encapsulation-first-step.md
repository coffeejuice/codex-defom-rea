# CP81 first converted GuiModel access seam

The first source migration is the retained setDimension callback: it now obtains
the actual step through an internal guarded callback, with no raw model.step
access in class_slots.cpp. This establishes the access-layer pattern while
preserving the native call and CP80 pending refusal. Public step remains a
documented legacy escape in unconverted code; full encapsulation is not claimed.

This began as read-only preparation from resume80-geometry-encapsulation-design.md.
After the CP80 archive existed, root approved the four-file source lane below.
No compiler/build/test execution, GUI or mutable REA action was performed by
this worker. Root owns integration validation and freeze.

## Smallest migration and exclusive files

| Owned file | Exact change |
|---|---|
| include/m2m/engine.hpp | Add only a GuiModelAccess forward declaration and friend declaration. No data member, order, type, access label, public method or native layout changes. |
| New src/engine/gui_model_access.hpp | Define private-source GuiModelAccess::withStep, including its constrained callback contract and pre-effect model gate. |
| src/engine/class_slots.cpp | Convert only the existing retained setDimension callback to withStep. The native DB_STEP_DATA::set_dimension call and its int32 argument remain the same. |
| tests/mdt/test_viewer_box.cpp | Add compile-time callback/native-layout/C-ABI gates and extend the existing actual pending/nonpending regression. No duplicated fixture or new test registration. |

This report is the fifth owned output. No GuiModel implementation, receipt
helper, engine_internal.hpp, CMake, public C header or C ABI source changed.

## Access contract and preserved order

The helper is defined only in the private source tree; the installed/public
engine header exposes only its opaque friend name. It is a stateless static
callback seam, not a new step owner, proxy model, persistent lease or public C API.

`GuiModelAccess::withStep(model, callback)` first calls the existing private
requireGeometrySyncAvailable. Only after that check does it obtain
model.step.get() and synchronously invoke the callback. It does not store the
callback or pointer and does not allocate, initialize, replace or copy a native
step. The caller's existing nullable-pointer behavior is retained; no fabricated
default or additional null-step result is introduced.

The initial C++20 callback constraint accepts only void or bool results. Direct
pointer and reference results, including bool&, are rejected at overload
selection. This prevents the converted API from directly returning a retained
native borrow. Arbitrary C++ captures/pre-held pointers remain a review boundary;
the constraint is not a universal alias-revocation mechanism.

The converted callback captures the dimension value, forms the same int32 local
after admission and calls the same actual step's set_dimension(&d). That maps to
the existing native slot1401c8ba0 qualification. It neither captures/stores a
step pointer nor performs another native call. The previous standalone entry
guard is replaced by the access-layer guard, so future converted code cannot
accidentally acquire this borrow before the check.

## Why privatization is a later migration

Access control on the existing step field applies to all consumers at once.
Making it private after converting one callback would break the remaining
source groups inventoried in CP80. This checkpoint therefore converts one whole
callsite file and enforces its raw-access allowlist. DB/readers, viewer/display,
operation writers and save/read-only consumers need their own assigned batches
before the field's privacy switch.

Other public fields and already-held native pointers remain legacy surfaces.
Their mutation does not clear the existing model-owned parked barrier or
authorize reader continuation. This patch does not enable a lower-query reader
body, cursor retirement or public geometry completion.

## Focused tests and compile gates

The existing viewer-box target remains39 runtime Testkit cases. Added coverage:

- A nonpending control uses the actual installed dimension callback, then invokes
  withStep once. The callback confirms it received that control's actual step
  and the expected dimension, returning bool; its invocation count is exactly1.
- The actual parked directory-query fixture invokes withStep with a callback
  that would set a test flag. The same unsupported refusal occurs, the flag
  remains false, and the existing whole-step/table/timer/body invariants remain
  unchanged. The real retained dimension callback is separately refused too.
- Requires-expression static assertions accept void/bool callbacks and reject
  DB_STEP_DATA*/DB_STEP_DATA&/bool& results. These compile gates do not execute
  or manufacture a native model.
- Native DB_STEP_DATA's established size0x3408 is asserted, alongside its existing
  complete offset assertions from the unchanged layout header. GuiModel::step
  remains exactly unique_ptr<DB_STEP_DATA>, and GuiModel remains nonpolymorphic.
- Public C API version remains1; pointer-type assertions cover all six current
  exported declarations (version, codepage, GUI unit, installation folder and
  both conversion encodings), including path/message pointer types and size_t.

The worker checked source structure and include resolution only. The compiler
has **not** evaluated these new static_assert/constraint gates yet; root must
build the target. No passing-runtime-test claim is made.

## Layout and ABI release gates

Measured source proof: removing only the added forward/friend declarations from
the new engine.hpp bytes restores the exact CP80 SHA-256
`a63af54f21807744f80a9660d6ba0169fb0d450522413c7a7d2be5fab0f797b1`.
Every data declaration, member type/order and access label is unchanged. There
is no new object storage or vtable. Numeric GuiModel sizeof/align/offset values
were not measured by this worker; no invented layout constants were added.

Public C header SHA-256 remains
`34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`;
src/dll/abi.cpp remains
`6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848`.
Both were compared byte-for-byte by hash against the CP80 baseline. The current
CMake configuration links m2m_core statically and defines the DLL's C exports
in abi.cpp; no exported GuiModel signature is introduced.

Root's release gate is: compile all affected consumers/native layout assertions,
run viewer_box plus the existing C ABI consumer/version tests, compare the selected
CP80 and candidate DLL export names/ordinals/forwarders (RVA changes are expected),
then run the full source-stable suite/archive. The unchanged C declarations and
new function-type assertions are compile gates, not a substitute for the binary
export comparison. A future field-privacy or storage change needs a separate
actual layout/consumer review.

## Static preflight and frozen release

Static checks passed: no `.step`/`->step` field access remains in converted
class_slots.cpp; the helper's model gate precedes its borrow;39 unique Testkit
cases/main/supported macros and all new quoted includes resolve; no C assert or
REQUIRE_EQ; whitespace clean; header reverse-diff and C ABI source hashes match.
The only raw step field in the new access seam is the explicitly allowed
model.step.get() after its guard. No repository-wide zero-raw-access claim is made.

Paths below are relative to source/apps/moproj2mst. SHA-256:

| Released file | SHA-256 |
|---|---|
| include/m2m/engine.hpp | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| src/engine/gui_model_access.hpp | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| src/engine/class_slots.cpp | c641d180385c467052a635e0fec337580a904d89d46a8dc684661ddf55b5869c |
| tests/mdt/test_viewer_box.cpp | 128755279c3e6c5bac972a03b012f01e32617e3f7a2e55db2b917c7e3f6c71ed |

Target `m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`. Root owns review/build/
runtime and ABI validation. All four source/test files and this report are
released frozen; token measurement is unavailable.
