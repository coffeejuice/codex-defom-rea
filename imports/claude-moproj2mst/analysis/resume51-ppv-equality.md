# Resume 51: PPV lookup equality and matched-index evidence

## Question and target

Question: does the original lookup at `0x140d4bab0` compare the complete
two-integer/two-double point, and what index does `0x140d4b8c0` return for a
match?

Target selected for static analysis:
`evidence/original-binaries/DEF_GUI_MO_64.exe`, PE image base `0x140000000`,
SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
This matches the target identity already recorded in
`analysis/resume50-ppv-lookup-contract.md` and
`analysis/resume50-ppv-pattern.md`.

## Bounded-query result

I attempted Ghidra pseudocode for only `0x140d4bab0` and `0x140d4b8c0` from
that selected PE. The provider did not reach its default analysis-ready state:
the request ended with `provider_timeout`, reporting the Ghidra startup deadline
of 330000 ms while importing/analyzing the 67,611,136-byte target. The returned
diagnostics identify the same target SHA and describe incomplete analysis; no
function pseudocode or procedure evidence was returned. I did not retry, alter
the timeout/configuration, or annotate the database. The ephemeral binary
session was closed after the failed request.

A second bounded fallback request used the configured REA `inspect_with_objdump`
provider for disassembly of that exact local PE path. It returned
`capability_unavailable` / `unsupported_provider`, reason
`ERR_REA_WINDOWS_NATIVE` (“Unable to run configured command”). Thus there is no
disassembly receipt from this fallback either. This is an observed provider
availability failure, not evidence about the functions' behavior.

## Evidence that remains established

The retained decompile `evidence/gui_mo/s1/mdt/L0/A/f0.c` (SHA-256
`a873ed9deb32842d55f45bc51f7f8166766979dfbd2c1dbe2d99c2aab99e80ed`) shows
`0x140d4af00` constructing a point with integer fields at offsets `+0`/`+4`
and doubles at `+8`/`+0x10`, then calling `0x140d4bab0`. If that helper returns
an empty result, the caller appends and returns the new last index. Otherwise,
it calls `0x140d4b8c0` and returns that helper's integer result; it does not
append on this branch. The evidence therefore establishes lookup-before-append
and index reuse, conditional on whatever predicate the lookup applies.

The retained reader receipt `analysis/resume48-ppv-reader-receipt.txt` shows
shared rows passing that returned index to `0x140d4bb70`. The retained
`0x140d4bb70` body in `f0.c` calls `QListData::insert` at the supplied index
and writes the flag into the inserted slot. So a repeated shared row resolving
to an existing point causes another insertion at that point index; it is not
an indexed flag replacement. Subsequent flag positions shift. This insertion
fact does not require knowing the equality predicate.

The previous bounded raw-PE inspection recorded function extents
`0x140d4bab0..0x140d4bb61` and `0x140d4b8c0..0x140d4b9e0` for this same
hash-verified PE. Extent and raw bytes alone do not establish a semantic
field-comparison contract without a reliable instruction decode and call
interpretation.

## Disposition

| Claim | Status |
|---|---|
| Point layout is two `int` fields followed by two `double` fields | **Settled** by retained `0x140d4af00` decompile. |
| Lookup miss appends and returns last index | **Settled** by retained `0x140d4af00` decompile. |
| Lookup hit avoids append and uses `0x140d4b8c0` result | **Settled** by retained caller decompile. |
| Equality compares all four fields, and its double comparison mode | **Unknown**; requested callee body unavailable. |
| `0x140d4b8c0` is the exact first/equal item's index under duplicate matches | **Unknown**; requested callee body unavailable. |
| Shared-row flags replace or insert | **Settled: insert**, from retained `0x140d4bb70` decompile. |

Smallest next query: run a bounded local disassembler that can map the PE's
file-backed `.text` bytes for only the two known function ranges, or provide a
retained qualified decompile/assembly receipt for those exact addresses. Until
then, implementation must model the native lookup as an opaque deduplication
predicate and must not claim full-record equality or exact duplicate-index
selection. No source/build/GUI/mutable REA change was made.
