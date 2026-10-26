# CP255 SPRING combo labels owner migration

Implemented the reviewed finite owner boundary as `bool GuiModel::copyEditorSpringComboLabels(simcontrol300::SpringComboLabels&) const`. `engine.hpp` forward declares the owned record; the complete IO definition is included only by the implementation. The old lifecycle helper is removed. Its original algorithm is preserved: one unguarded count read, range refusal, formatted `0   - None`, numbered object lookups 1..count, kind-7 skip before name reads, bounded NUL scan, printable ASCII, signed decimal formatting and the 64-byte snprintf cap, then identical ordered catalogs. A local initialized record publishes by move only on success. Ordinary false results preserve all output fields; exceptions still propagate.

The existing Extra3D lambda remains deferred after scalar sidecar reads. It calls the owner operation, converts false to nullopt, and otherwise returns the owned labels. `fillExtra3D` and its spring read/actual-label-versus-legacy fallback policy are unchanged. No public native alias, observer, key or identity claim was introduced.

## Runtime checks

Added direct lifecycle cases for zero count and absent step (exact placeholder/catalog copies/index reset); dense 1/2/3 numbered traversal with explicit kinds and malformed kind-7 name skipped; owned strings retained after native mutation/destruction; missing numbered lookup after an accumulated eligible object; negative and over-4096 reported counts with allocation ownership retained and metadata restored; nonterminated 0x54 name, non-ASCII name, last-scanned-index NUL, length-64 formatted refusal, length-63 formatted success and empty-name success. Every ordinary refusal checks distinctive prior vectors and both indices unchanged. The existing ordered scalar-sidecar test remains intact and still passes IDs10/20. The valid positive-count parked viewer fixture now checks geometry refusal and unchanged output. Parked zero-count semantics are supported by source inspection, not a new runtime parked-zero fixture.

Focused targets confirmed from configured CTest registration: `m2m_mdt_simcontrol300_lifecycle`, `m2m_s_spring_page_io_roundtrip`, `m2m_mdt_viewer_box`.

- Requested grouped build `cp255-spring-labels-focused`: PASS. A subsequent line-ending-only correction preserves the lifecycle test's existing LF format. A repeat using the same label was refused by the launcher's immutable-receipt protection without running a build. Final matching grouped build: `analysis/cp255-spring-labels-focused-lf-build.json`, PASS and source stable. Neither build used `--then-full`.
- Focused CTests: initial sandboxed run passed lifecycle/viewer but SPRING IO failed creating its existing disposable C:\PROBLEM directories (24 fixture setup access errors). Authorized escalated rerun: **3/3 PASS**, `analysis/resume255-spring-labels-focused-ctest-elevated.log`. The original failure is retained in `analysis/resume255-spring-labels-focused-ctest.log`.
- Binding/include/Testkit preflight: PASS, `analysis/resume255-spring-labels-binding-preflight.json`.
- `git diff --check`: PASS.
- Final source fingerprint after tests: `376bcc8f157c9f4ed0612ae64a58931ce4f55de81a96891b2c1a08ed491d13e5` (3563 files), matching final build receipt. Source and selected executable hashes: `analysis/resume255-spring-labels-hashes.json`.

## Evidence and limits

Read CP254 next-leaf proposal and PASS review, CP120 placeholder reconciliation and actual-label lifecycle integration review. Retained original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; setup `0x140ec8b00` and formatter `0x140ec98e0` support label order, zero placeholder formatting and visible-text parsing. The 4096 count, NUL/ASCII and buffer caps remain reconstruction admission rules, not recovered native safe limits.

This source-preserving extraction adds no original symbol/interface recovery, fresh differential witness, physical QWidget/reentrant proof, broad ObjectKey/currentness closure, public editor/Save completion or cache1/accepted-Close witness. No original process, GUI-control session, full build, freeze, plan or shared recovery-state edit occurred. Full source-stable integration suite and freeze remain root work. No dmo.exe or reconstructed DEF_PRE_64 executable was built or deployed in this converter-only lane. Wall times are in retained build/test receipts; token measurement is unavailable.
