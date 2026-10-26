# CP252 secondary-display no-step dimension correction

## Change

`displaySecond` now passes `b->dimensionOr(3)` into the secondary `GuiModel` constructor. The code still reuses an existing second before examining the main model, returns null if no main exists, then constructs, sets the backlink, optionally copies GEOTYP through nullable control access, and publishes/returns the owned second in the existing order.

The caller tests in `tests/mdt/test_display_selection.cpp` cover a live main with an absent step, requiring a dimension-3 secondary with its backlink and `DisplayModels` ownership set while the main remains step-less. Repeated calls reuse the same pointer. After destroying the separately owned main, the extant second is still reused; after dropping it, a call with no main returns null and does not recreate it. A separate table verifies live dimensions 2, 3, and present 0 are copied unchanged and the second is reused. These are reconstructed caller-level tests, not physical GUI or original-application reachability tests.

## Source qualification and limits

The original static path is documented in `resume252-next-geometry-leaf.md` and independently reviewed PASS in `resume252-next-geometry-leaf-review.md`. The identified original is `evidence/original-binaries/DEF_GUI_MO_64.exe`, version 11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The static full function range `0x140cf9990..0x140cf9ac8` has SHA-256 `635617aa3ec47b312986217bfea91ee855f820285531448a005b9a1b431218ec`; the nullable control getter range `0x140c7b2d0..0x140c7b301` has SHA-256 `2f5b78f5174075403916e79492d514c1eb0aab38b00ffd322bcef44b73bc2ef2`. This is disassembly/decompile evidence only: no original instruction was executed and no naturally reached no-step occurrence is established.

Under the documented stable-main, unparked and successful allocation/constructor prefix, original `displaySecond` obtains default dimension 3 for a missing main step, then its nullable main-control lookup skips GEOTYP copying. The static path supports normal completion, backlink setup and secondary publication for that profile. It does not prove fresh-project reachability, reentrancy behavior, or all exceptional paths. Original publication precedes backlink/control work; converter publication remains after those operations. This edit does not resolve that separate effect-order discrepancy or the converter's geometry admission behavior.

## Checks and artifact identity

- `python admin/check_new_cpp_bindings.py --project-root source/apps/moproj2mst tests/mdt/test_display_selection.cpp`: passed; Testkit entry point, includes and assertion usage preflight passed.
- `python admin/build_candidate.py --label cp252-second-default-focused --target m2m_mdt_display_selection`: passed. No `--then-full` was used. Receipt: `analysis/cp252-second-default-focused-build.json`; source fingerprint before and after build matched (`b589e9193d07c9e9adb4d9d0944bf7465d87e488c9db1f86f6b52f4d4ab3357c`, 3,563 files).
- `ctest --test-dir build/candidate -R '^m2m\.mdt\.display_selection$' --output-on-failure`: passed, 1/1.
- `git diff --check`: passed.

Focused test executable SHA-256: `ed2159b8cf1431e5b7553f3f4f6b6541f5b1ab8311707f0e7f2622b8d31d9355`.

The checkout already contained other concurrent edits to these same two source files before this leaf, including named-display dimension handling and its caller test. They remain outside this change. The focused build fingerprint covers the whole 3,563-file source tree as it stood during this build, not an isolated CP252 patch. Root should perform the source-stable integration run and checkpoint freeze after all source owners hold.
