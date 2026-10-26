# Checkpoint 54 candidate-only full-project connector check

## Purpose and boundary

Run the retained complete `120_B_DOE` project through the candidate's project
reader and explicitly activate the sub-project that contains its Cogging to
DoeOutput request. This would establish that the corrected registry flags and
parser admit the edge in the actual project XML without launching the GUI or
calling Save. It is a candidate-only integration check; it cannot establish
original parity.

Do not use `m2m::convert` / `convertToBytes` as the sole check: the current driver
calls `activateSubProject(p, 0, ctx)` (`src/core/convert.cpp`), while the edge is
in Sub_Project index 1. The dedicated test should use the existing internal
reader seam and explicitly activate index 1.

## Input and scratch binding

- Read-only source fixture: `${M2M_ORACLE_B}/120_B_DOE/COG_P1_B1.moproj`, whose
  retained SHA-256 is
  `60ADDC2722E85F92A088FA07F77E8A966928AB5B934F161DF87DCF6A417ED30E`.
  The imported fixture is under `evidence/gui_mo/oracle_b/120_B_DOE`.
- Copy the complete case folder, including `COG_P1_B1.DB`, all listed project
  dependencies, and `DoeStudies`, beneath that test's configured
  `M2M_TEST_SCRATCH` directory. Keep the copied input tree pristine and use its
  `.moproj` as the reader argument. Never make the retained corpus path writable.
- Record a relative-path/size/SHA-256 manifest for the copied input before and
  after the check. The check performs no Save and should leave all copied input
  bytes and directory entries unchanged. Its artifacts are the test log and a
  compact graph/status receipt in scratch; there should be no generated MST,
  rewritten MOPROJ, or DOE output file.

## Callable sequence

This is a C++ test using existing interfaces, not a CLI Save invocation:

1. Construct `m2m::Project p` and `m2m::Ctx ctx`. Derive the sibling DB path
   with `m2m::app::changeSuffix(projectFile, m2m::qt::L(".DB"))`; call
   `m2m::initProject(p, m2m::qt::absolutePath(dbPath),
   m2m::qt::completeBaseName(dbPath))` and `m2m::makeRegistry(p)`.
2. Set `p.doeIo = m2m::doe::makeRecordingStudyIo()` before parsing the study
   sub-project. Leave `ctx.session == nullptr`; do not construct a GUI/session
   object, set native file-write flags, or invoke any save/operation loader.
3. Call `m2m::openProject(p, projectFile, ctx)`. Assert its accepted-suffix
   return is true and `p.openedOk` is true; retain any `ctx.warnings` and
   `ctx.originalDiagnostics`. Assert there are at least two sub-projects.
4. Call `m2m::activateSubProject(p, 1, ctx)` explicitly. Catch and report an
   `m2m::Error` with both code and message as a failed check, rather than
   converting an unexpected refusal into a passing result. Assert `activeSub ==
   1`, `subs[1]->loaded`, and `!p.loadingSub`.
5. Find the `FlowItem`s in `p.subs[1]->flow.items` by `opId`, not by assuming
   flow vector order. Assert operation id 3 has class `SfOprCogging`, operation
   id 6 has class `SfOprDoeOutput`, and the links are reciprocal:
   `cogging->next == doeOutput` and `doeOutput->prev == cogging`. Also inspect
   the original request in `p.dom` (DiagramItem index 2 requests target index
   5) so the receipt ties the accepted relation back to its XML indices.
6. Read the `p.registry` entries for both class keys and assert the exact flags
   `0x1a` and `0x20097`. Record each item's document index, operation id, class,
   requested target, registry flag word, and resulting predecessor/successor
   operation id (or null). This compact row makes request, type source, and
   actual parser result auditable together.

`openProject` calls `openProjectDb` and reads/walks the project file; the copied
database is therefore part of the required complete input. A `Ctx` without a
session skips the public driver’s remesh/session work. Recording DOE I/O is
needed because activating a Type 2 sub-project also visits its `SfDoeStudy`
element. Confirm the before/after tree manifest before describing this path as
non-mutating. If the reader or DOE recording seam refuses before flow parsing,
preserve the precise error as an integration result; do not bypass it by
synthetically constructing only the two flow items and call that a full-project
check.

## Assertions and reportable result

The pass condition is specifically:

- accepted project suffix and successful project walk (`true`, `p.openedOk`);
- second sub-project activation completed (`loaded`, selected index 1, no
  in-progress load);
- the source request from XML indices 2 to 5 maps to operation ids 3 to 6;
- the Cogging and DoeOutput registry flags are the evidence-derived values;
- the resulting graph has the reciprocal 3 -> 6 links; and
- all bytes/paths in the disposable input tree match its pre-check manifest.

Report parse/activation status, error code/message if any, warnings/diagnostics,
all asserted graph fields, candidate source-tree and test executable hashes,
fixture/project hash, and input-tree manifest match. Do not report Save or Close
status: this bounded test does not call those operations. `openProject` alone
does not dispatch `openActiveOperation` for either edge endpoint.

## What this establishes and what it cannot

Passing proves candidate reader + real registry + actual Sub_Project 1 XML
activation preserve this requested edge in a complete retained project. It
provides a stronger candidate integration check than the two-item P03 fixture,
and is orthogonal to the original `0x140074f50` probe's mocked callbacks.

It does not prove original Open activates Sub_Project 1, that the native
application's resulting graph is identical, that DoeOutput operation loading
or Save succeeds, or that `.MST`/side-file/Close bytes and states match. The
retained native probe does not parse a project or use real connector types; the
static initializer/decompile evidence and P03 test establish the candidate's
source contract only. Original acceptance still needs a fresh original run on
its own pristine disposable copy and a matched candidate run with graph, status,
Save effects, and Close/reset receipts. Existing historical `B:120_B_DOE`
comparison (Checkpoint 51: seven missing entries) is not a substitute for that
fresh paired capture and is not diagnostic of this connector edge by itself.

Design note only. No source, build, fixture, GUI, or REA database was modified.
