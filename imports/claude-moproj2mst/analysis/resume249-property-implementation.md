# CP249 heating-time unit migration — implementation evidence

## Change

Added `GuiModel::readPropertyHeatingTimeUnit(int&) const noexcept`. It returns false without changing the output when `step` is absent; otherwise it copies `step->sim_ctrl.unit` unchanged. The query has no geometry admission call and returns no native alias.

`propsSetHeatingTime` now refuses before lookup when the owner is null, otherwise calls `opModel(p, *pr.owner)` exactly once with the supplied project. A missing model or step preserves the existing `M2M_E_UNSUPPORTED` text. The unit is copied before `valueWithUnit(t, 0, unit)`, and the formatted value is assigned through `B(pr)` only afterwards. No loader or other reader was changed.

Tests cover direct values 1, 2 and -37, unchanged native unit and absent-step sentinel; successful provider-backed writes for 7.25 and 8.5 seconds, one lookup per call, block allocation and unchanged native unit; and a live unit query in the parked geometry viewer case. Existing null/refusal and explicit-control checks remain.

## Validation

- `python admin/build_candidate.py --label cp249-heating-unit-focused-r2 --target m2m_moproj_props_l2 --target m2m_mdt_viewer_box` — configure and both Release targets succeeded.
- `ctest --test-dir build/candidate -R "m2m\.(moproj\.props_l2|mdt\.viewer_box)$" --output-on-failure` — 2/2 passed. The props test exercised its configured oracle corpus: 95 operations, 900 values, zero mismatches, five existing class-registration skips.
- `python admin/check_test_source_preflight.py` and `python admin/check_new_cpp_bindings.py` on both changed tests — passed with no issues.
- `git diff --check` — passed.

The first focused CTest run failed only on a cumulative provider-count assertion that still expected five calls after the newly added two successful writes. The assertion was updated to account for those two calls; the focused build and complete focused CTest group were rerun successfully.

## Limits and provenance

This is focused implementation evidence, not a full source-stable launcher, freeze, or scientific acceptance. The independent PASS review qualifies the API design and retained historical decompile attribution for `0x1405fcb90`/`0x1408221e0`; no new original executable/runtime evidence was collected. The unguarded behavior is a current-source compatibility fact, not proof of original null behavior. The loader's later raw control use remains out of scope. No full build or freeze was run.

## SHA-256

Source and focused executables at the end of the successful focused check:

| Artifact | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `94BA1713683DF19C7CED45B0B447A668631E4A3F4D32EB26744140386C0E066E` |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | `D46C0C404DD38DF05353418A862638B71F9F3C11CFA5FE468825389F67D014C0` |
| `source/apps/moproj2mst/src/engine/props_sync.cpp` | `5B1A20DBE9AC280896EE61D03D1E08224B54A81904379221108ECB650D916D71` |
| `source/apps/moproj2mst/tests/moproj/test_props_l2.cpp` | `3D0E16DD194E4E8035BC8DD130DECD31541314E8253D5821577EEF8F2F8D962D` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `D5BA92E99459BEA366F4C0F1777EADF6C4F9CDD2FA4D5B2AF7DF594EC6271A5A` |
| `build/candidate/bin/m2m_moproj_props_l2.exe` | `DE65455EBFE36116CD73C3C602C4DA0A1E8A49CAB3D6774B7C792A29A461511F` |
| `build/candidate/bin/m2m_mdt_viewer_box.exe` | `D1C43662CCB0EE8BD742F35AABC1A6082D44FF95D4F8401AB4DF7092F0949365` |
