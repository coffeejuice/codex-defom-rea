# CP251 MultiBlow control implementation

## Change

Added `GuiModel::initializeMultiBlowLoadControl()` as the owner command for the MultiBlow load prologue's finite sim-control writes. It calls `simCtl()` once, returns when no control exists, then writes `dttype.u58 = 0` followed by `nstep = 10000`. The command is not `noexcept`, preserving the geometry barrier's refusal behavior.

`multiBlowLoadPrologue` retains its established order: base load prologue, TRANS2, TRANS1, model lookup/null return, then the owner command. Its separate raw writes and local `simCtl()` guard are removed. There is no C ABI change.

The real class-prologue test seeds DTTYPE and NSTEP with nondefault values before dispatch and checks the resulting state and existing base/TRANS effects. The viewer-box tests call the command directly with geometry parked, and directly with an absent step; refusal and no-recreation are asserted.

## Qualification

Retained native `evidence/gui_mo/s1/mdt/L4/slots.asm` shows accessor `0x140c7b2d0`, DTTYPE setter call at `0x140601fec`, then NSTEP store at `0x140601ff6`. The same function's retained `d0.c` shows the base/TRANS/model/control sequence. This establishes source order, not a newly captured runtime trace.

The MultiBlow test reclassifies operation 4 from corpus 42; it is reconstructed fixture coverage and is not an original MultiBlow corpus occurrence. Historical Oracle B saved files remain outcome witnesses; they do not observe the transient prologue ordering. The final-state unit test alone cannot dynamically prove write order. Later KEY loading may overwrite DTTYPE, so the test observes immediately after prologue.

## Verification

- `python admin/check_new_cpp_bindings.py tests/mdt/test_multiblow_load.cpp tests/mdt/test_viewer_box.cpp`: passed.
- `python admin/build_candidate.py --label cp251-multiblow-control-focused --target m2m_mdt_multiblow_load --target m2m_mdt_viewer_box`: configure and build passed; source fingerprint stable at `642837cd0751b79dc38caa0514b1bf8a5d000063e8b79c6a2d765337e5004bd4`.
- `ctest --test-dir build/candidate --output-on-failure -R "^m2m\\.mdt\\.(multiblow_load|viewer_box)$"`: 2/2 passed (`m2m.mdt.multiblow_load`, `m2m.mdt.viewer_box`).
- `git diff --check`: passed.

Focused executable SHA-256:

- `build/candidate/bin/m2m_mdt_multiblow_load.exe`: `cd82ff65f157396b5d1ef13de089b898797c0749e2b6279be2234aeae88e6e5c`
- `build/candidate/bin/m2m_mdt_viewer_box.exe`: `a19847f793d4718cfe93f7da3b5fc33b57c3df8c74388b3e29d4c41c6625bf17`

Source SHA-256:

- `include/m2m/engine.hpp`: `c859aeff39b327728d4441fb5770dffe0e27c1a14b4b9f79739a7c60e43f70b8`
- `src/engine/gui_model.cpp`: `150cba31b80b101ed4265765a270760fb3e494994a0a917b93aaf085c44bbf90`
- `src/engine/multiblow_load.cpp`: `586e55b1338f6662c4ada767198404b353eff38826c3286f36e4949c481c9bcc`
- `tests/mdt/test_multiblow_load.cpp`: `054077e44f133e533c0e0a30d97e5c8df684dd0ed0f6d17e9ac606921bd45f42`
- `tests/mdt/test_viewer_box.cpp`: `59f0776cd4eebbc929712b33ba8379722381fe424d53a17c9362d59dd403f524`

`engine.hpp` and `test_viewer_box.cpp` already had concurrent edits at task start; this change preserved them. This is a focused implementation result, not a full-suite run, source-stable freeze, or candidate acceptance.

## Review repair receipt

The independent implementation review requested an unrelated control-field preservation assertion in `multiblow_prologue_and_class_data`. The test now seeds `sc->stpinc = 2718` before actual class-prologue dispatch and asserts it remains 2718 immediately afterward. This field is independent of the base TNOW, TRANS2/TRANS1, DTTYPE, and NSTEP effects under test.

Repair verification:

- `python admin/check_new_cpp_bindings.py tests/mdt/test_multiblow_load.cpp`: passed.
- `python admin/build_candidate.py --label cp251-multiblow-control-repair --target m2m_mdt_multiblow_load`: configure/build passed; before/after source fingerprint `a4704e8cfac417519b6e0776f09ea25a832894c3bf30b250a7c3b65c435345fe`, stable.
- `ctest --test-dir build/candidate --output-on-failure -R "^m2m\\.mdt\\.multiblow_load$"`: passed (1/1).
- `git diff --check`: passed.
- Rebuilt executable SHA-256: `4623d3efc696faf4c647df3bdd28b649987761e0d49762b85265dc9a5ca86d16`.
- Updated `tests/mdt/test_multiblow_load.cpp` SHA-256: `06f802892a7ac5f4bb27dc6478008848a6f6018adb7c6fe61da4ded21379b65a`.
