# CP73 B121 TLOC isolated writer prefix

## Result

Added a candidate-only serializer for the exact retained B121 one-variable `Transfer Time` / `TLOC` shape and a raw-byte read→write→read regression. The serializer is a separate lower-level helper; it is not called by `writeInputTable`, `saveInputTable`, or public Type 2 conversion. Both public nonempty-variable refusal guards remain unchanged.

The retained source is `evidence/gui_mo/oracle_b/121_B_DOE_VAR/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`, 707 bytes, SHA-256 `18ec0c482730c82ade12e91771ca67e53e4604573032624b99f3f4829a86b069`; it has 24 CRLF lines. The new test's golden string reproduces this exact element layout and line ending conversion. Retained original source qualifies the element and child sequence, but does **not** qualify its final attribute order from setter-call order: the native variable setter sequence differs from B121's emitted `numSamples, varId, dimension, name`. The candidate helper therefore emits explicit fixture-specific byte order instead of routing through the generic XML `AttrOrder` projection. This is justified only as a bounded raw golden regression for this one validated record, not as a general Qt ordering or native call-order rule. Nominal text is exactly `TLOC` + four spaces + tab + `0.1`. It emits the nested `KwOperations` and top-level `Operations` relationships separately.

The exact-byte assertion is a **candidate-only fixture regression** against retained B121 bytes. No fresh original behavior or public Save parity is asserted. Because no build/test was permitted in this task, no candidate output bytes/hash or passing result exists yet; the equality claim remains an executable expectation, not an observed pass.

## Owned frozen files and SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/mst/doe_model.hpp` | `1706ae4db922b1e6cda879f62494f21f996132c2470530abb90996c6d3a40429` |
| `source/apps/moproj2mst/include/m2m/doe_study.hpp` | `5fcae896737ec00df6cec4e2d01ca4d76562df74c72bb7b0d1132deef7b4cc99` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `64bfdbe34401a7a7db30df5eb18471939b2e6a4663952c752f09ac8831812c76` |
| `source/apps/moproj2mst/tests/doe/test_b121_tloc_model_prefix.cpp` | `29e86131da5c6bb02c91e7ba1f7d3c36c9473f401ad1e8f11f1ab1df0d5fb04f` |

The new test file is included by the existing `tests/*` and `test_*.cpp` CMake glob. Seven active cases now cover fields/maps, model query, exact raw retained schema bytes and re-read, mapping mismatch rejection before mutation, nonfixture header rejection before effects, public reader refusal, and public writer refusal.

## Boundary and preflight

- `writeInputTable` still refuses any nonempty `Study::vars`; `saveInputTable` still refuses before it. No public route calls the new helper.
- The helper is constrained to the B121 header defaults, one variable, one scalar keyword, and operation id 1; it rejects nonfixture headers, regions, or constraints before planning output. It does not write `temp.KWD`, PAR tables, or other DOE files. Retained source shows the direct nominal getter is a QString copy; `temp.KWD` belongs to a distinct conditional dimension-reconciliation path, not direct XML serialization.
- `git diff --check` completed without whitespace diagnostics. CMake discovery and test case count were checked statically.
- No build or test executable was run. Compile/runtime correctness and the raw byte assertion are pending root's grouped build/test.
- No GUI/app control or mutable REA action occurred.
