# CP456 CopyMirror capacity-return continuation

The smallest closed private increment is the gate-true, parsed-parent-zero branch when the actual View owner extent is **exactly 128**. The allocator returns zero before searching inactive records or reading the builder-local bounds. The builder then inserts that zero into the first empty owner link and returns normally. Its caller proceeds with the fixed input-row writes. This is a capacity failure of record allocation, not an error return from the enclosing builder or importer.

## Evidence identity and limits

The immutable imported `evidence/original-binaries/DEF_GUI_MO_64.exe` was read as a PE file only. Its SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The ten bounded dumpbin captures are identified by `cp456-mirror-capacity-static.json`. `cp456-mirror-capacity-byte-verification.json` maps all captured instruction bytes, including continuation lines, through the PE section table: **769 addressed instructions, 3,808 compared bytes, 3,808 distinct bytes, zero mismatches**. Dumpbin banner version is 14.44.35228.0. Unwind ranges are used where present; explicit 96-byte leaf candidates include padding and, for two captures, following-function prefixes. The verification receipt separately records each inspected logical body through its RET; no following-function bytes support these contracts.

Read inputs were CP445 producers/dependencies and their source qualifications, CP447 builder range and byte-verification qualifications, CP448 integration report/validation, retained `evidence/gui_mo/s1/mdt/L5/d1.c` and `d2.c`, and the current private owner/importer source. Historical Ghidra snapshots remain qualified as non-transaction-consistent; the relevant instructions here have a fresh direct PE byte binding. The reverse-engineer-anything skill informed the static-evidence qualification. This task used the explicitly requested local dumpbin route and did not acquire a shared mutable REA session. No original executable was launched, no GUI control was acquired, and no source, build, plan, state, fixture, or deployment file was changed.

## Exact bound contracts

| Function and inspected body | Relevant behavior |
| --- | --- |
| `141249730..141249a7b` | At `141249753` compare owner `+0x8468` with `0x80`; `14124975d` branches away only when unequal. Equal: `xor eax,eax` then jump to epilogue `141249a74`. No call, owner/record store, slot scan or bounds read occurs on this branch. It is equality, not `>=128`. |
| `141249420..141249430` | Returns the owner `+0x8468` extent. |
| `1412494c0..141249515` | Returns null for id <=0 or id > extent; otherwise returns owner+(id-1)*`0x108`. No active-byte check. Thus returned id zero resolves to null without even calling the extent getter. |
| `14124a010..14124a05d` | If owner link `+0x846c==-1`, assign supplied id; otherwise if `+0x8470==-1`, assign supplied id. There is no positive-id check or duplicate check. Both occupied: no change. The return is `rep ret`. |
| `141247d90..141247ddd` | The same first-then-second `-1` policy for record links `+0xfc/+0x100`. This body is bound for completeness; the selected parent-zero/id-zero path never calls it. |
| `141249f80..14124a008` | Parent zero calls root insertion `14124a010` with returned id. Nonzero parent attempts lookup and inserts into its record if found. Then always looks up returned id, and if found inserts the parent into that record. Parent=0,id=0 therefore inserts root zero, then lookup(0) returns null and there is no record-link store. |
| `14004e810..14004e828` | Assigns the supplied dword to record `+0x104`. |
| `141247ed0..141247f1c` | Stores supplied active byte to `+0xf9`; when the resulting byte is zero clears record links `+0xfc/+0x100` to -1. A nonzero active byte preserves links. |
| `140e23a00..140e23d75` | Builder, including capacity continuation below. |
| `140e23490..140e239ee` | Caller, including its fixed-row stores and deferred flag pass below. |

## Parent-zero capacity continuation and partial order

The builder still resolves Settings and tests flag 1 AND (flag 2 OR flag 3), obtains the owner, constructs the local reflection matrix, and handles the parsed parent before allocation. Existing CP447 matrix evidence remains the basis for matrix arithmetic and its exceptional-value qualifications. At parent zero it sets the local mode byte zero and skips nonzero-parent matrix composition.

1. `140e23cf3` calls `141249730`; extent 128 returns EAX=0 with owner extent, matrices, records, bounds and active bytes unchanged by the allocator. Inactive records do not permit reuse at extent 128.
2. `140e23cf8` saves id zero; `140e23d0c` calls lookup(0). `140e23d1c` takes the null branch to `140e23d38`, skipping `141247ba0` coordinate/helper work entirely. Thus the builder's still-unknown bounds and normalization-length dependencies are not consumed on this selected allocation path.
3. `140e23d4d` calls `141249f80(owner,&parent,&id)`. Parent=0,id=0 makes root links transition `{-1,-1}->{0,-1}->{0,0}` on successive calls; subsequent calls leave `{0,0}` unchanged. In general, only the first available -1 is replaced, and occupied values are preserved. No record link or payload is touched by this call on this path.
4. The builder executes its two retained temporary destructors (`14121c570` at `140e23d5b` and `140e23d69`) and returns at `140e23d75`. New exception/OOM/destructor failure semantics are not established by these selected bodies; this is the ordinary control-flow contract after normal upstream calls.
5. Caller `140e237f3` ignores the builder's return. `140e237f8..140e2380c` selects **input row i**, owner+i*`0x108`; it does not select allocation id zero or an inactive slot. It calls the `+0x104` setter at `140e23823`, then stores widened parsed binary32 values in order `+0xe0`, `+0xe8`, `+0xf0`, `+0xc0`, `+0xc8`, `+0xd0` at `140e2383c/858/874/890/8ac/8c8`. `+0xd8` is untouched. At `140e238e8` it sets active true; it then advances to the next row.
6. Only after every first-pass row does the second loop begin (`140e238f2` onward). It applies each parsed nonzero/zero flag via `141247ed0`; disabling clears that record's links, while enabling preserves them. Owner root links are not cleared by the active setter.

An initial owner `{-1,-1}` therefore receives `{0,-1}` before caller row 0 is written. If a later row hits an existing unsupported branch, retain those root/caller mutations and the first-pass active=true state; do not run the deferred parsed-flag pass. A later nonzero parent must retain the existing guard before capacity handling because the original composes that parent before calling the allocator. A later extent-below-128 allocation must retain the CP448 matrix/active partial mutation and bounds guard.

## Recommended private boundary and decisive tests

Replace only the existing parent-zero extent==128 unsupported branch with first-empty owner-link insertion of zero followed by a normal return from the enabled builder prefix. Keep the actual Settings reads, reflection construction, and nonzero-parent guard in their existing order. Let the existing caller perform its fixed-row stores and deferred flag loop. Keep public operation admission and positive serializer guards unchanged. Extents outside the reachable invariant 0..128 are not newly supported, and this equality check must not silently become a >= comparison.

A focused test can reach extent 128 using the existing retained-mutation prefix calls; it does not need a positive original Open. Assert (a) capacity before reuse even when a slot is inactive, (b) no record matrix/mode/bounds/extent mutation by the allocator branch, (c) root link fill with zero on first and second capacity calls and no third overwrite, (d) caller writes target fixed i rather than allocation id, preserving existing matrices/mode and `c0[3]`, (e) successful first-pass completion followed by parsed disabling clears record links but preserves owner links, and (f) a later nonzero-parent/bounds refusal retains earlier root and fixed-row writes and suppresses all final flags. Reachable nondefault record links are not established by these new private capacity paths; do not claim a test proves their preservation merely by observing default {-1,-1}.

The original caller contains no row<128 guard before its fixed record stores. For i>=128 this task does not authorize reproducing native memory corruption. Preserve the reconstruction's existing explicit private out-of-range refusal, **after** a normally returning capacity builder, retaining any root-link insertion that already happened. No fresh original runtime occurrence, original acceptance, successful public CopyMirror Open/Save, general bounds reconstruction, or full application completion is claimed.
