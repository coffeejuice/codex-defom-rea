# CP105 per-call actual InputTable filename context

Actual file-reading callers now hand their exact successful-read filename value into an internal InputTable parser context. The legacy public bytes-only API remains source-less, and the existing pre-QFileInfo Geometry refusal is unchanged. This is an architectural prerequisite; it does not admit Geometry or establish native QString/QFileInfo ownership.

The named `src/doe/study_io.cpp` did not exist. Before edits root approved the actual exclusive files: `src/doe/activation.cpp`, `src/doe/doe_study.cpp`, `include/m2m/doe_study.hpp`, new `src/doe/input_table_reader.hpp`, and new `tests/doe/test_input_filename_context.cpp`, all under `source/apps/moproj2mst`. CP104's verified archive receipt existed before source writes. Root explicitly excluded a new Error subtype/observer merely to expose this context, and none was added.

## Source qualification and exact handoff

The controlling independent evidence is `resume104-doe-input-filename-owner-audit.md`, SHA-256 `EF9C25A8C40F186A952C1E3723038A9079868F9DD49B5C5BD71E99B8652094A5`, together with `resume104-doe-geometry-entry-plan.md` and its immutable caller receipt. Native outer0x14090e080 uses the same filename QString for its QFile and copy-passes it to0x14090ee20 at0x14090e467/0x14090e48d. Geometry later constructs QFileInfo from that argument. It is not a precomputed QFileInfo or a path reconstructed from the geometry payload.

The new internal `InputTableSource` contains an owned String filename value. `readInputTableFromSource(bytes, study, source)` receives that value by value and keeps it live through normal parsing or exception unwinding. Its address is passed only into the private `parseInputTable` frame. The source pointer explicitly distinguishes presence from absence; an empty filename value is not used as an absence sentinel. No pointer/context is stored on Study, Session or global state, and none escapes that call.

`readLazyStudy` passes `InputTableSource{input}` only after the existing `StudyIo::readFile(input, bytes)` succeeds. This is exactly its read path, without reconstruction, normalization, basename extraction or a second read. Existing `loadInputTable` likewise passes `InputTableSource{path}` after its actual `readAll(path, bytes)` succeeds. Failed opens retain their previous error/continuation behavior and never invoke the parser with an invented source.

The public `readInputTable(const std::string&, Study&)` signature is unchanged and delegates to the same private parser with no source. In-memory callers do not receive a guessed project path. The existing parser body and all state/effect/refusal order are shared; no Error type/message or C ABI changed. The public header comment now describes the source-less bytes entry instead of its obsolete blanket variable-refusal wording.

The private parse frame deliberately marks source as currently unconsumed: CP104 Geometry still refuses before QFileInfo. The value is available within the intended lexical/lifetime boundary for a future qualified implementation, but no path result, native wrapper, refcount, shared-null data identity, file-engine/cache state or geometry capability is inferred from it. `variable_record.cpp` was untouched and retained CP104 SHA-256 `7FCBE23AF509A3A94CA5555AAE181A370880A68AA3617C55AA3C3552621FD96D`.

## Validation and its limits

Three focused tests in new `test_input_filename_context.cpp` check meaningful invariants without adding a diagnostic error/observer:

- A named source, including an explicitly present empty filename, cannot bypass Geometry and produces the same unsupported text/state as the bytes-only reader.
- The public bytes function signature and normal/malformed parsing results remain; a supplied filename does not cause an additional file read or replace the bytes input.
- The real lazy read uses its exact expected StudyIo path, retains the same Geometry partial prefix and reaches no later sample writes. Fresh sample invalidation still occurs.

The tests observe the existing caller read ledger and parser effects/refusal, **not** the filename value inside the unconsumed parser parameter. Exact handoff/provenance/lifetime is source-level qualification only. No runtime native QString/FI ownership or path transformation is asserted. No synthetic new Error subtype, context history or fabricated successful Geometry result was added to make an otherwise invisible prerequisite observable.

Testkit/include preflight passed with supported assertions/main and resolvable quoted includes, including the relative private header path. Direct trailing-whitespace/final-newline scans passed for all five exclusive files, including untracked header/test. Callsite inspection confirms both actual file readers use the named source and the public bytes wrapper passes no source. No worker compiler/build/CTest, GUI/native process or mutable analysis database was used.

Target `m2m_doe_input_filename_context`, CTest `m2m.doe.input_filename_context`, is discovered by existing CMake globs. Root owns executable validation, with existing activation/Geometry-entry regressions retaining the observable refusal contract.

## Source freeze receipt

Hashes were sent to root and independent review immediately after static preflight. All paths below are under `source/apps/moproj2mst`; files remain held through root validation/archive.

| File | SHA-256 |
| --- | --- |
| src/doe/activation.cpp | EED08F06790D4651EA08D8159C8595DAA64A12BB363867EFCBB080218DD4E2D2 |
| src/doe/doe_study.cpp | 8493C19C02DAE30AD57E3F22D1225DD8CE67E7C0697EFB74D57D03CF7AB703DB |
| include/m2m/doe_study.hpp | 60533C094F3C376D2BA9071F4269E7CB106825F79844102C3F723A49F92E3E0C |
| src/doe/input_table_reader.hpp | E365CFDD1015F514D21EA8ACD5CA24AFB28FF21950FD25D9E88232C231C5F8CD |
| tests/doe/test_input_filename_context.cpp | 6E21F189A41A884DC175C90796301ACCA35FBFC1C6CC44B951E63DB615C42D8D |

Next dependency is binding this actual filename occurrence to a qualified QString/shared-null/QFileInfo private/file-engine/path sequence, then actual geometry payload/append/file effects. The current context does not supply that admission. Keyword temporary/codec/record ownership, QLocale globals and public Save reconciliation/refresh remain separate guards.
