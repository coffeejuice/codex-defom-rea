# CP105 — DOE InputTable named-source handoff review

## Result

No blocker found in the frozen five-file handoff. It carries the actual successfully-read input path through one synchronous parser invocation, leaves the existing public bytes-only signature intact, and does not consume filename context or weaken the Geometry guard. This is a source-level handoff only; it does not reproduce native QString/QFileInfo ownership or prove a runtime native path identity.

## Findings

- `src/doe/activation.cpp::readLazyStudy` computes the InputTable path once, reads that path through the bound `StudyIo`, and only after successful read passes the same `input` value in `InputTableSource{input}`. A read failure still takes the existing last-error branch and never invokes the named-source parser. The later sample-file operations remain after the parser call and therefore preserve their prior continuation/refusal behavior.
- `src/doe/doe_study.cpp::loadInputTable` likewise computes one path, calls `readAll(path, bytes)`, returns false on failure, and passes that same path on successful read. No reconstructed project/geometry path is substituted.
- `src/doe/input_table_reader.hpp` is under `src/doe` and is not part of the installed include API. `include/m2m/doe_study.hpp` retains `bool readInputTable(const std::string&, Study&)`. The new `readInputTableFromSource` and `InputTableSource` are internal-only. The named function owns its source value by value; `parseInputTable` receives a pointer to that stack-owned value and returns synchronously, so the pointer cannot outlive the source. `parseInputTable` does not store or dereference it.
- Both entry points reach the same `parseInputTable` body. Geometry still raises the same unsupported error at the entry guard before QFileInfo/path construction, attribute conversion, file reads, or geometry-owner creation. No new Error subtype, status override, source-presence test, or alternate geometry branch was added.
- `test_input_filename_context.cpp` checks that named and bytes-only Geometry readers produce the same refusal while retaining the same reached header/Description prefix; it checks the public function-pointer signature, normal and malformed byte-result behavior, and the lazy caller's recorded read path/name prefix with no later sample I/O. It does not claim that an in-memory caller's supplied filename is authentic or that the candidate String shares native QString data/ref state.

## Snapshot hashes

- `src/doe/activation.cpp`: `EED08F06790D4651EA08D8159C8595DAA64A12BB363867EFCBB080218DD4E2D2`
- `src/doe/doe_study.cpp`: `8493C19C02DAE30AD57E3F22D1225DD8CE67E7C0697EFB74D57D03CF7AB703DB`
- `include/m2m/doe_study.hpp`: `60533C094F3C376D2BA9071F4269E7CB106825F79844102C3F723A49F92E3E0C`
- `src/doe/input_table_reader.hpp`: `E365CFDD1015F514D21EA8ACD5CA24AFB28FF21950FD25D9E88232C231C5F8CD`
- `tests/doe/test_input_filename_context.cpp`: `6E21F189A41A884DC175C90796301ACCA35FBFC1C6CC44B951E63DB615C42D8D`

No source or test changes were made during review; no build or tests were run.
