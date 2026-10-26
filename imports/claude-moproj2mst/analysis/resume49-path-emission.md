# Checkpoint 49: B98 path reproduction and bounded emission contract

## Reproduction audit

The raw repro at `analysis/resume48-b98-raw-repro.md` records the existing checkpoint 47 CLI, its SHA-256, input-copy location, command, exit status, raw hashes, and line-level diff. The candidate MST is 30 bytes longer only because its one absolute path line contains a 95-byte current scratch prefix in place of the historical 65-byte case prefix. A unified diff shows no other changed lines. Prefix replacement equality was checked on the raw byte arrays, while both unmodified hashes are retained. This is a bounded diagnostic mapping, not raw equality and not a comparator change.

The source-level interpretation is independently supported by `analysis/wave2-save-diffs-diagnosis.json` and `evidence/gui_mo/s1/s1-05/spec.md`. The checkpoint47 historical result remains qualified: the original MST was saved under an earlier `deform-rea-work` root; no original executable was launched for checkpoint47 validation. Candidate reproduction proves current converter output for its scratch location, not that a fresh original Save under the same location has been observed.

## Source-qualified contract: Diestress only

For `SfOprDiestress` (`writeMst`, original VA `0x1402b5410`), the retained original spec says:

1. Resolve the operation's current subproject DB path with its directory (`1400b9e80(..., true)`). This is not the writer's `db` argument and not a path relative to the project file.
2. Copy that QString and replace every case-insensitive `.DB` occurrence with `.KEY`.
3. After the properties pointer check and simulation counter selection, emit the SIM header, then call `0x1401344a0` with that key path. This helper emits the literal `KFREAD    \t`, integer `1`, then the QString path and line terminator.
4. Emit GENDB using the original DB path afterward.

The port at `source/apps/moproj2mst/src/ops/diestress.cpp` follows this sequence (`opSubProjectDb`, `replaceAllCi`, `emitKfreadOne`, then `emitGendb`). B98 confirms the absolute path materialization for one historical and one candidate environment. This evidence licenses the path derivation and raw location-specific output for this Diestress operation. The sample line is ASCII; the B98 diff does not establish non-ASCII codec behavior.

The `0x1401344a0` path argument is streamed as QString. The original MST closure does not set a QTextStream codec; `source/docs/re/moproj2mst/s1-02/spec.md` describes codecForLocale output. Consequently the line's ASCII bytes are unaffected by ACP choice. Non-ASCII path bytes from this QString route would need original locale-codec/profile evidence. This is a different route from QString::toLocal8Bit followed by CRT fopen.

## How far the contract can generalize

Do not generalize `opSubProjectDb -> replace .DB -> emit absolute .KEY path` across all operations. The evidence here names one virtual writer and one helper. Other MST records may emit relative KEY names, paths from different model fields, or operation-specific literal forms; the retained s1-04 spec lists multiple KFREAD variants and distinct output shapes. The safe reusable statement is narrower: **when an original writer is shown to pass an absolute QString path to the same QString QTextStream helper, that helper preserves the path value as a line in the MST using the active QTextStream codec.** Each writer still needs evidence for how it constructs that path, whether it is absolute/relative, its suffix rules, and its surrounding ordering.

The existing `tests/s/test_save_folder.cpp::relocated()` is broader than B98 and intentionally recognizes several folder spellings and path components. Its existing rules must not be treated as evidence for an individual operation's path contract. For B98, retain the exact raw difference and, in a diagnostic view only, map the single evidenced absolute case prefix on the KFREAD path line. Do not map carried-over `<case>_before` paths or alter production output.

## Concrete follow-up

If this path family is used in another acceptance case, first bind that operation's original writer VA and path-construction source to the same absolute QString helper. Then capture candidate and original raw files before cleanup, keeping action/session and project root. Compare the exact path line and all remaining bytes independently; apply a case-specific prefix mapping only after confirming the field contains the generated current project path. Preserve original and candidate hashes and show raw plus mapped results. To extend from ASCII to non-ASCII, capture target-process ACP/Qt codec after startup and test a representable non-ASCII path; B98 cannot answer that question.

No source, test comparator, build, GUI, or mutable REA state was changed for this audit.
