# Checkpoint 57: isolated SimCtrl.PPV writer payload

## Source qualification

The original target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The complete retained writer decompile is `analysis/resume48-ppv-writer-receipt.txt`, bound to `evidence/gui_mo/out_s/s1.c` SHA-256 `e4e78f5ef1c94d103e50466632dffc363dc6389cbb9e1ff9bba10ea4abe75f27`; the receipt itself records SHA-256 `dcd1e9a5c130954ed782425f19f81b6f2c22008bd7c9abfe9da9445f9de3c453`. No runtime or REA action was used.

That body qualifies the writer decision and output sequence:

1. It calls `0x140307410(listLocal)` first. A true result returns false before constructing/opening the QFile. This helper's meaning is not assigned here; the serializer accepts the observed predicate result as an opaque boolean.
2. Otherwise it opens the passed path with `QIODevice` flags `(2, 0x10)` (WriteOnly | Text). Open failure returns false without payload writes.
3. After successful open, it writes count `local.size()` plus `shared.size()` whenever the shared-list pointer exists, even if the flags pointer is null.
4. Shared rows are emitted first, but only if both shared-list and flags pointers exist. For each row it writes seven tab-separated values: zero-based row index, `true`, two ints, two doubles, and the indexed flag as integer. Then local rows are emitted with indices restarting at zero and values index, `false`, two ints, two doubles, literal `0`.
5. Each line uses `QTextStream::endl`. The Windows QFile Text flag converts line breaks to CRLF. The stream uses the retained default Qt formatting: integer C-locale decimal, double SmartNotation/precision 6, no stream-setting override. The tested finite values produce ASCII decimal points and no grouping.
6. The QFile is closed and the body returns true without checking QTextStream status, QFile close status, or write results. This helper models successful-open payload bytes and that return decision, not device failure/partial-write byte accounting.

The exact record field order is retained in `analysis/resume48-ppv-writer-contract.md`; the native record layout is two ints at `+0/+4` and two doubles at `+8/+0x10`. The seventh emitted shared field comes from `flags[i]`; the local seventh field is integer zero. Do not reinterpret the reader's separate exactly-seven-token index-5 behavior as the writer's field order.

## Isolated implementation and tests

Added `include/m2m/engine/simctrl_ppv_writer.hpp` and `src/engine/simctrl_ppv_writer.cpp`. `writePpvPayload` models the opaque initial predicate, open success, successful payload and return result, but has no Project/Operation/QFile access and is not wired into `save_current.cpp`. It uses the existing `mst::Stream` numeric/string helpers and applies the target Windows Text-mode LF-to-CRLF conversion to the generated ASCII payload.

Added `tests/mdt/test_simctrl_ppv_writer.cpp`. Its byte-exact finite-value assertion checks shared-before-local ordering, seven columns, restarted local index, tab delimiters, and CRLF. Other cases pin the source-visible count/emission asymmetry when flags are absent, the opaque predicate's short circuit, open failure, and the permitted zero-count line. The new sources are covered by the project's existing `GLOB_RECURSE` source and `tests/*/test_*.cpp` discovery; CMake was not edited. The test has not been built or run; root owns validation.

## Unknown or intentionally excluded branches

- Meaning of `0x140307410` is opaque. This helper preserves its observed true/false branch effect without labeling the predicate empty/nonempty.
- The function's file path construction/routing and ordering relative to SimCtrl.KEY are outside this payload helper; `save_current.cpp` remains guarded and unchanged.
- If both shared and flags are non-null, native indexes `flags[i]` without a shown length check. The isolated API documents the caller precondition that flags cover the shared list; mismatched native state has no qualified behavior here.
- List sizes/indexes are native signed ints; extreme counts/overflow are not tested or qualified.
- Post-open write/flush/close failures and partial bytes are not simulated. Native ignores those results and reports true after the visible close path; actual partial-file behavior needs separate QFile/device evidence.
- The golden test covers finite doubles in the ordinary precision-6 profile. NaN/Inf spelling, extreme exponents, and any locale/codec outside the retained Windows Qt defaults are not established by these cases.
- No reader behavior, public Open/Save integration, path encoding profile, or original runtime outcome is inferred from the isolated serializer.

No shared CMake, public Save code, GUI, original process, or mutable REA state was touched. No build or test was run; the three owned files are frozen for root validation.
