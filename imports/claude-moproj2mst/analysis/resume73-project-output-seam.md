# CP73 private project-output writer seam

The writer now accepts a private per-call output factory through `m2m::moproj::detail::saveProjectFileNativeWithFactory`. Ordinary `saveProjectFileNative` retains its stack `mst::FileDevice`, with no factory object or adapter allocation on the default route. Core conversion routing is deliberately absent from this change and remains root-owned.

All implementation paths below are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/source/apps/moproj2mst`. The assigned files are released and frozen for root integration:

| File | Lines | SHA-256 |
|---|---:|---|
| src/moproj/project_output_device.hpp | 31 | 586132f1b480112b5b3d4152b35aca508114fe65a93e786d0b11d2ba8cbec669 |
| src/moproj/write.cpp | 632 | 70ed65c4c2581ccbafaf123ab70d1fefd0d4747701fd72b17cf6ce8ab1f96717 |
| tests/moproj/test_project_output_device.cpp | 174 | 8eb2ae13ab0cf60d3509a601129f13debf738b67841f46a2e198468934e47c2c |

The installed `include/m2m/moproj_write.hpp` is byte-identical to the CP72 archive, SHA-256 `c78629ef0c232809026a66afcf1fcc74c3bf4b93391ed36cfc11cb0e42c42069`. No public header, convert.cpp, convert_details.hpp, C ABI, shared test, CMake, alternative optimization copy, GUI or mutable REA was edited. No configure, compiler or test process ran.

## Preserved writer order

`prepareNativeProjectOutput` assigns the external XML journal observation before the null/empty-path check, then applies the same suffix mutation. Both native entries use it before opening output.

`finishNativeProjectOutput` checks actual opened state before DOM mutation, invokes the unchanged `saveProjectFileBody`, then queries `error()==0`. The body still creates Stream, serializes UTF-8, closes the device while Stream exists, and destroys Stream before the error query. Exceptions still propagate; neither entry catches them or rolls back preceding effects.

The default entry constructs the same stack FileDevice after the prelude. The private entry invokes its borrowed factory at that exact output-open position and owns the returned device only for that call. A non-null failed-open device returns false. Null is a private infrastructure configuration error (`std::logic_error`), not a new native failed-open policy. No input reader, property writer, MST device, settings object, global, environment variable or Session gets a fault control.

The factory contract has only `open(path)` and an OutputDevice extension with `error() const`. Existing OutputDevice write/flush/close/opened/aboutToClose behavior remains unchanged.

## Dedicated test and its limits

CMake's existing test discovery gives target `m2m_moproj_project_output_device` and CTest name `m2m.moproj.project_output_device`. It contains exactly one case, `project_output_declared_postclose_error_controls_writer_result`, with two fresh disposable copies of `133_B_DBSTEP_BOOL3_before` beneath configured M2M_TEST_SCRATCH:

1. Control delegates the real FileDevice post-close status and requires true with real error zero.
2. Declared error 7 returns that status only from the private adapter's post-close error query and requires ordinary false; the underlying real device still completes its normal writes and close.

Each run opens/activates the actual copied project and uses the existing guiViews implementation. There is no GUI launch. The test adapter owns a real FileDevice and forwards its aboutToClose callback to the outer Stream callback only when the real device emits it. Close delegates directly, without pre-flushing, early notification, or a fabricated initial-flush event. Teardown closes idempotently and detaches the lower callback before destruction. Per-call observations live outside the device, so assertions never inspect a destroyed adapter.

Active assertions require one factory call at the exact normalized project path, successful real open, positive serialized bytes, one callback while open, ordered open/close-start/callback/close-end/error query, one query after close, and adapter destruction before inspection. Both runs compare the physical file to the final serialized DOM and require no recorded/replayed native project write in the external observation.

This tests the writer's status-to-Boolean rule with preserved delegated close ordering. It does not exercise outer property/MST continuation, final M2M_E_WRITE mapping, a physically caused persistent flush error, original fault residue or fresh-original acceptance. Root's forthcoming private core services and separate integrated case are needed for the continuation claim.

## Static preflight completed

Compared against `build/wave3-checkpoint72-verified/source.zip` and its receipt: source tree `1ab0ff16933b42355e4297bf8fcdd94ad831273a19b5e8f0aca2d6ab8c15b72f`; archive SHA-256 `7b817a38d65600f98469e11d950cca6b208211217df56b731e49cbef2a72cd1a`.

All new quoted includes resolve relative to their file or the target's existing include directory. The dedicated test has one Testkit main and one TEST_CASE, uses Release-active REQUIRE/CHECK/CHECK_EQ only, and contains no C assert, REQUIRE_EQ, skip or unsupported-result fallback. The shared serializer body is text-identical to the archive after CRLF normalization; its normalized text SHA-256 is `3fb7cc641e87f64739e66270a66c248f8cd60fb2204003dd96c890379f019085`. Writer CRLF formatting was preserved.

These are static checks, not a compilation or runtime pass. Root should configure to discover the new target, then group it with `m2m_s_save_io`, `m2m_mdt_file_device` and `m2m_doe_type2_native_open_guard` plus the core integration test once available. Preserve the complete source-stable launcher and immutable source/artifact/test freeze before acceptance. This report releases the three assigned files; no further source edits are planned until root reports results.
