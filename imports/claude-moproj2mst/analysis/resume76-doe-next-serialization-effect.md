# CP76: next DOE serialization effect after the isolated InputTable writers

## Finding and recommendation

The next exact file-level effect visible in both retained B121 and B122 post-action folders is the **mode-0 sample table output**: current `DEF_DOE.PAR`, followed by fixed `Samples_LHS.par`. Each pair is byte-identical within its case. This serialization is independent of scalar `SetOprHasDoe` delivery once a `Study::SampleState` has already been qualified. The smallest source-closed candidate is a fixture-backed test of the existing sample-table formatter/immediate sequence using the retained B121 and B122 mode-0 rows, checking output bytes directly and keeping all project/public Save integration guarded.

This is a detached serializer proof, not permission to append sample writes immediately after either new B121/B122 InputTable helper in public Save. Original Type2 order places `DoeTableRefresh` between InputTable XML and sample outputs. The refresh owner/delivery boundary is still unqualified in the candidate; skipping or reordering it would not preserve the native Save sequence. Existing `saveType2SampleSequenceImmediate` already encodes current-table first, then fixed modes 0, 1, 2, with ordinary open-failure continuation. The remaining independent precision step is to compare that known formatting against actual B121/B122 sample fixtures rather than only synthetic sample values.

## Retained file evidence

| Case | File | Length | SHA-256 |
|---|---|---:|---|
| B121 | `DoeStudies/DoeStudy1/Control/DEF_DOE.PAR` | 1,396 | `1B6887361A11FB31DDD728AB10C4C35F6405A5399A849E4F6D2B4285D0C50791` |
| B121 | `DoeStudies/DoeStudy1/Control/Samples_LHS.par` | 1,396 | same as B121 current PAR |
| B122 | `DoeStudies/DoeStudy1/Control/DEF_DOE.PAR` | 1,389 | `770D49EAEDED318B08B88EEC289FF26F133BF874F098A795E7E757A61F20D56F` |
| B122 | `DoeStudies/DoeStudy1/Control/Samples_LHS.par` | 1,389 | same as B122 current PAR |

Each has mode header `0`, 100 row records, and 101 CRLFs including the header. B121 and B122 sample values differ, so the equality is per-case between current mode 0 and LHS, not across cases. Neither historical folder has `Samples_FF.par` or `Samples_USR.par`; do not invent those files or infer their validity flags from omission alone. B122's other DOE artifacts (DEF_DOE.INI, parser INI, DS/MST.TEMP and GEO files) have distinct producers/interactive history and do not form one Save serialization batch.

## Source-qualified ordering and boundaries

`analysis/resume63-doe-public-save-order.md` qualifies the Type2 writer arm at native addresses: `0x1400d3200` InputTable at `0x140099770`; current samples through `0x1400d75a0` at `0x1400997b2`; `0x1400d7790` fixed mode 0 at `0x140099a77`, mode 1 at `0x140099d7c`, mode 2 at `0x14009a081`; then common DbName suffix. The InputTable function's `DoeTableRefresh` signal is between XML serialization and current/PAR outputs. These calls are in-order and their ordinary Boolean returns do not abort later phases.

`analysis/resume64-doe-sample-save-binding.md` and `analysis/resume71-doe-sequence-static-review.md` qualify the candidate serializer: header, one-based row numbers, trailing tab per value, CRLF; valid empty tables still emit a header; invalid tables skip only their own writer; PAR Boolean means open success and does not convert post-open write/close failures to false. Current source uses `qt::formatDouble` at six significant digits. Therefore a fixture test should compare exact bytes to detect locale/precision differences, and if it fails, stop at that numeric-format discrepancy rather than normalize the fixture.

`SetOprHasDoe` is a different unresolved signal in B121's variable **reader** (`0x14090ee20` path). It does not participate in formatting an already populated `SampleState`. But a public B121 project load must still pass that variable parse/delivery boundary before later sample-file loading can establish the `Study`; the detached test can source sample rows directly from fixture input without claiming this lifecycle path is recovered.

## Suggested next test-only/source-closed increment

Add a focused DOE test that reads the actual B121 and B122 `DEF_DOE.PAR` and `Samples_LHS.par` bytes in binary mode from configured `M2M_ORACLE_B`, uses the existing sample parser to construct mode-0 rows, and checks `writeSampleTable(0, rows, syntheticPath, FileEffects)` against the retained LHS bytes. Assert the current and fixed paths are distinct planned writes with each exact payload; do not write the oracle fixture. If using `saveType2SampleSequenceImmediate`, supply a fixture-built Study and fake StudyIo, then assert mode-0 sequence/path order and skip modes 1/2 only when those flags have independently been set in the fixture model. A single-fixture data reconstruction is not evidence of sample regeneration, random design generation, or general numeric locale behavior.

Do not wire this helper into public `subProjectElement` until the actual ordered `DoeTableRefresh` owner is executable and the same selected Study/I/O identity survives that handoff. Also keep B121's unresolved `SetOprHasDoe` reader delivery, B122's geometry file ownership/copy, public variable pruning/index finalization, XML post-close result, Project DOM replacement, and Type3 `.OPT` arm guarded. The new B121/B122 isolated XML writers only plan one XML artifact apiece and cannot stand in for those steps.

## Review scope

I compared retained fixture file sets/hashes, `resume63-doe-public-save-order.md`, `resume64-doe-sample-save-binding.md`, `resume71-doe-sequence-static-review.md`, current sample serializers, and the new isolated InputTable writer scope. Read-only only: no source/build/test/GUI/mutable REA action was performed. This note is the only written file.
