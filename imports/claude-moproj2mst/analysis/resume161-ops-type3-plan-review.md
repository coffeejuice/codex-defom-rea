# CP161 independent review: CP160 FRCROT table-suffix plan

## Disposition

The proposed **private prepared-observation table projection** is source-ready with one narrow contract clarification before implementation: treat scanner outcomes outside `{-1, 0, 1}` as candidate-unsupported, and make the already-zeroed buffer assumption explicit in the API/precondition. The original caller checks only `return == -1`; for the one-conversion `%Flf` calls, `0` leaves that destination untouched and any non-`-1` result continues. The source projection must not call a host scanner, parse text, claim locale/CRT equivalence, or attach to a public FRCROT/Type3 route.

The source receipt binds the table-reader path to the original immutable `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The plan's raw `table_reader` bytes (`0x140AF73F0..0x140AF77FA`) hash to `a9a7c9faf99726022edd96b06e427fed146093d41162bff2b557a30f91cc25a9`; the five `%Flf` byte strings are individually recorded in `resume160-ops-type3-evidence.json`.

## Scanner return and mutation review

At each loop site in `analysis/resume160-frcrot-table-140af73f0.asm`, the call to `0x141472AC8` is followed by `cmp eax, 0FFFFFFFFh`. Only equality branches to `eax=1` and returns table status 1. Every other return reaches the next loop/field. This exactly supports:

- `-1`: stop immediately with table status 1; do not scan the paired Y slot after an X failure.
- `0`: do not treat as failure; the just-zeroed destination remains unchanged, and continue.
- `1`: the single conversion assigned the `double`; that slot was written by the scanner before the caller continues.

The `%Flf` format addresses in the receipt decode to one conversion each. The formatted-scan wrapper `0x141472AC8` forwards the format and varargs to its shared scanner implementation. The plan correctly makes observations inputs and does not assert how arbitrary text or locale reaches these outcomes. A good API should reject synthetic codes outside the three values above rather than accidentally model them as success merely because the native branch tests only `-1`.

The FRCROT dimension-one loop is row-major: for each `i`, it scans `axis[0][i]` at `0x140AF757F`, then the value column at `0x140AF7608`, then advances to the next row. With the FRCROT table's single value column this is exactly `X0,Y0,X1,Y1,...`. The immediate stores are observable on failure: an X failure stops before that row's Y; a Y failure retains the just-scanned X and all prior completed slots. Unvisited slots remain zero only because the successful-allocation path calls the byte-fill helper before scanning. The proposed tests should assert both EOF branches at later X and Y positions and the return-0 continue behavior, not only final table contents.

## Mode, count, and boundary checks

The FRCROT mode path shown at `0x140B67701..0x140B67843` parses the mode/value before touching the forward table. A mode-parse error exits with group status 2 before the `+0x240` table reset. After successful mode parse it resets template `0x16`, then:

- Mode 0 writes dimension 0 and the parsed double constant; the table reader's early test at `0x140AF742B..449` returns 0 when dimension is zero and value-column count is at most one. It reaches no allocation, scan or trailing line read.
- Mode 1 writes dimension 1, code 1, and the prepared integer conversion result into the count field.
- Mode 2 writes dimension 1, code 4, and the prepared integer conversion result into the count field.
- Other mode values leave the common reset state (dimension zero, one value column, zero axis entries); the same early return rules out table scan failure on that path.

For dimension 1, `0x14089B840` releases old buffers, calls the count clamp leaf `0x14089B970`, ensures at least one value column, allocates/zeroes the selected arrays, and the reader scans. Negative prepared counts clamp to zero. `0x14089BC90` returns the aggregate table count; for the stated one-axis/one-column profile it equals the normalized count. A positive count reaches one trailing `0x1414733AC(buffer, 0x400, file)` call whose return is ignored. Count zero reaches no scans and no trailing line read.

The `0..7` profile is a valid **candidate restriction**, not a native maximum. It bounds each eight-byte buffer to at most 56 bytes and stays inside the fully captured small byte-fill path. The plan properly excludes allocator-null/large-fill behavior. Preserve metadata `+0x54/+0x60` across reset per wrapper `0x14089B9E0`, but do not expose fabricated pointers or claim native allocation ownership.

## Entry owner and side-effect order

The relevant FRCROT table owner at this call is the table region at `parser/context + 0x240`: the caller resets it at `0x140B67739` and passes that same address to `0x140AF73F0` at `0x140B67832`. This is distinct from an earlier parser branch's table at `+0x90`; the implementation must model only the FRCROT `+0x240` route and must not accidentally reuse that earlier branch. In the callee, `RCX` is the input file pointer, `RDX` is the table pointer, and the third argument in `R8D` is zero at the FRCROT call; the reader uses file plus table and does not use that third argument in the captured body.

A table-read status of 1 is stored by the FRCROT arm and does **not** bypass the group-3 common tail. The common tail reaches reverse identity/direction handling and the wrapper `0x1408E5D60`; only later does `0x140B68F67` map nonzero saved table status to group result 2. The proposed helper is therefore correctly limited to forward table state/status: it must not undo table writes on failure, skip the later common tail, claim reverse payload, or model mirror data. CP160's separate wrapper analysis correctly leaves the delegated mirror copy and its feature predicate unresolved.

## Recommended isolated source/test boundary

A private helper over an explicitly supplied reset table value and ordered scan-observation vector is a suitable increment. Keep it separate from `keywordValues`, `variable_record.cpp`, the FRCROT input-table reader, and all public operation dispatch. Its tests can verify mode `0`/unknown early-return outcomes, modes `1/2`, negative-to-zero count normalization, row-major destination order, code validation, zero/unassigned slots, failure status and retained prefixes, and the final ignored line-read boundary.

Before coding, make the following preconditions explicit:

1. table template/reset and recognized mode writes have already completed normally;
2. count conversion is already supplied as a prepared result (do not implement `cvttsd2si` here);
3. allocation and zero-fill succeeded for the bounded profile;
4. each observation corresponds to exactly the next original `%Flf` call and uses only `-1`, `0`, or `1`;
5. the final line-read is represented as a reached/ignored boundary, not an injected return that changes native table status.

These are candidate-only helper conditions. They do not establish the parser context's native allocation/lifetime, a complete Keyword owner, actual nominal/upper/lower FRCROT values, active `keywordValues` callsite, or public Save. Keep public Type3 and FRCROT gates unchanged.

## Qualification

Read-only plan review against `analysis/resume160-ops-type3-next.md`, its exact JSON receipt and retained `resume160-frcrot-table-140af73f0.asm`, helper captures, FRCROT callsite in `resume160-frcrot-group3-140b667a0.asm`, and prior CP144/149 FRCROT owner boundaries. No source/test edits, build, GUI, original process, or mutable REA action occurred.
