# CP213: exact Qt4 QString snapshot layout

The retained and installed QtCore4_64.dll agree on an eight-byte QString object containing a Data pointer. The Data header holds refcount at +0, capacity at +4, length at +8, and the UTF-16 data pointer at +0x10. This is sufficient to implement a bounded read-only Frida snapshot at the existing outer and acceptance entry hooks. It does not prove that any pointer encountered at runtime belongs to a live heap allocation or that a snapshot is atomic.

Only this report, its synthetic fixture/receipt, and separately named bounded disassemblies/receipts were written. No original process, GUI/app-control session, mutable REA database, source/build, fixture project, deployment, or shared state was changed. Applied the reverse-engineer-anything skill's separation of original copied-PE facts, inference and runtime evidence; no REA session was acquired. Read subtree AGENTS, CP213 encoded-open plan, CP212 decoder report and bounded assembly, retained QtCore export table, CP81 source-owner/Latin-1/allocator evidence, CP98 resize, and CP120 realloc evidence. Earlier reports are leads; the direct getter instructions below are the layout proof.

## Exact artifact and byte proof

The selected binary is `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, 3,250,688 bytes. Independently hashing installed `C:\Program Files\SFTC\DEFORM\v11.0.2\QtCore4_64.dll` gives the same hash. Installed file version is **4.8.4.0** (ProductVersion absent). PE is AMD64, preferred image base `0x67000000`, timestamp `0x517180cf`, SizeOfImage `0x31f000`. Preferred addresses below are static image addresses; loaded RVAs must be added to the independently qualified module base.

Fresh read-only `admin/capture_pe_ranges.py` captures are retained in `resume213-qstring-layout-static.json` and `resume213-qstring-raw-static.json`. DUMPBIN is version 14.44.35228.0. Its bounded leaf spans include adjacent padding/instructions after RET; only the entry-through-RET instructions establish getter behavior.

| QtCore export / RVA | Entry-through-RET bytes | Meaning |
|---|---|---|
| QString::length/size / `fec0` | `48 8b 01 8b 40 08 c3` | load Data from `[RCX]`; return 32-bit `[Data+8]` |
| QString::capacity / `2e90` | `48 8b 01 8b 40 04 c3` | load Data; return 32-bit `[Data+4]` |
| QString::unicode/constData / `47940` | `48 8b 01 48 8b 40 10 c3` | load Data; return pointer `[Data+0x10]` |

Export names/RVAs come from retained `wave2-save-diffs-qtcore-exports.txt`; the fresh capture bytes directly establish those accesses. They correct a CP81 prose sentence that reverses the +4/+8 length/capacity labels. CP81's equal-length/capacity constructor values could not disambiguate them; the getters do.

The retained `resume81-doe-qstring-latin1-tail-6706bfa0.asm` allocates `2*n+0x20`, writes refcount 1 at +0, equal capacity/length at +4/+8, points +0x10 at Data+0x1a, and writes a 16-bit terminator at Data+0x1a+2*n. Fresh QString destructor `resume213-qstring-raw-67020c70.asm` decrements the 32-bit `[Data]` and calls the deallocator only on zero, establishing the ref field. CP98 resize writes size at +8 and a terminator through inline Data+0x1a. Fresh setUnicode (`74730`) calls resize and copies exactly `2*n` bytes through `[Data+0x10]`.

Fresh fromRawData (`72190`) proves that the character pointer can instead refer to an external buffer. It allocates 0x20 bytes for Data, sets ref 1, size/capacity to supplied n, stores the external pointer at +0x10, and does **not** terminate the external buffer. For null supplied pointer it instead uses Data+0x1a and n=0. Thus neither `chars == Data+0x1a` nor an accessible NUL after the logical string is a universal QString validity condition. For CP213's decoder→setUnicode path, inline storage is the expected static route, subject to actual snapshot evidence.

## Snapshot contract for the runner

Use these target-specific fields, all little-endian:

| Location | Type / role |
|---|---|
| object +0 | 64-bit Data pointer |
| Data +0 | signed 32-bit refcount |
| Data +4 | signed 32-bit capacity in UTF-16 units |
| Data +8 | signed 32-bit length in UTF-16 units |
| Data +0xc..0xf | padding: no semantic claim |
| Data +0x10 | 64-bit character pointer |
| Data +0x18..0x19 | flag storage; constructor clears low five bits, preserves others |
| Data +0x1a | first inline QChar if character pointer equals this address |

No target calls are necessary. Before interpreting any object, qualify the loaded QtCore module's canonical installed path, unique module identity, x64 image metadata/size, and the exact three getter byte ranges above, with the installed file hash pinned by the host. Hashing the disk file plus a few loaded bytes is a bounded loaded-image check, not proof that every loaded byte is unchanged. Record this distinction. Do not equate the preferred image base with the loaded base or require ASLR to be absent. Do not patch or invoke the getters.

Read and record the object pointer, eight object bytes, Data pointer, and 26 header bytes; extract the fields without converting 64-bit pointers to JS Number. Reject null object/header/character pointer, unreadable or partial ranges, negative length/capacity, capacity below length, or arithmetic overflow. A positive refcount is a conservative CP213 policy gate supported by the observed constructor paths, not a claim that every possible Qt object with other refcount values is invalid. Preserve the actual ref value on failure. Optionally reject unaligned object/header pointers conservatively; require two-byte character alignment for this trial and identify it as a policy gate.

Use length cap **128 units**. For length above the cap, return explicit `length-cap`/incomplete with `truncated=true` and no success value; never silently read only a prefix. For length 0, a successfully read header and valid pointer policy permit an empty units array, but this cannot satisfy CP213's expected 34-unit path gate. Reject unreadable state with null units/text, never an empty success string.

Compute `byteCount=2*length` only after the integer/128 cap. Check each full range's start/end with NativePointer/UInt64 comparisons so no pointer truncation or wrap occurs. Require every crossed page/range to be readable with the documented Process range API, then attempt the exact-length read in try/catch; mapping prechecks do not eliminate unmap/protection races. Verify returned ArrayBuffer length exactly equals byteCount. A zero-length string does not require a data-buffer read. Do not use NUL-terminated readUtf16String scanning.

Decode exact bytes with little-endian pairs into the authoritative array of 16-bit units. Preserve embedded zeros, paired surrogates and unpaired surrogates; display text can use String.fromCharCode over at most 128 units. Units/raw hex and length carry equality, since display rendering may replace or hide characters. Compare the full 34-unit expected path at both outer entry and acceptance entry, not merely display text or suffix.

After copying, reread the same object slot and same Data header and compare to the original bytes (or at least every semantic field). Full 26-byte comparison is conservative and may reject harmless padding changes. Fail explicitly on any changed pointer/header or read failure. Refcount may change legitimately in concurrent code; conservative rejection preserves uncertainty. Agreement does not detect an ABA mutation or in-place content mutation without header changes. A second content read can detect an observed difference but still cannot prove atomicity; do not claim a coherent locked snapshot.

If the runner observes the terminator, read exactly two additional bytes separately under the same guards, report its value/read status, and keep its failure separate from exact-length content. External data has no terminator guarantee. Report storage kind (`inline` or `external`), structural checks, map/read status and recheck result; never label the allocation heap-valid. Header fields and readable pages do not establish heap membership, liveness, allocator extent or generation. Do not read capacity*2 bytes, scan heaps, call malloc/Qt helpers, or assume a stale readable block is live.

Outer entry uses the existing raw QString argument from the independently reviewed six-hook protocol. Acceptance entry must retain raw RCX/RDX and establish RDX equals that exact outer object; the layout reader cannot supply call-frame attribution. Retain snapshot failure independently of scalar return results and observed Open completeness.

## Harmless fixture validation

`python analysis/resume213-qstring-layout-fixture.py` passed **17 synthetic-memory cases**: exact 34-unit path, embedded zero with paired/unpaired surrogates, valid empty, external buffer without terminator, negative size, capacity below size, length 129, zero/negative ref policy failures, null/odd/overflow data pointers, unreadable/partial content, null object, changed header and changed object pointer. Output and fixture hash are retained in `resume213-qstring-layout-fixture-receipt.json`.

This is an independently implemented Python model of the proposed reading contract. It validates layout arithmetic and desired failure semantics against constructed bytes; it does not test Frida APIs, generated runner code, live Qt objects, original behavior, hook neutrality, or module loading. The runner owner still needs actual-code mock tests and harmless native fixture execution under CP213's technical review. No original execution is released by this report.

## Clarification: refcount and capacity policy for the 34-unit target

Additional exact-DLL captures answer the proposed Qt-version exceptions. `resume213-qstring-policy-shared-data.json` retains the on-disk 32-byte shared-null header at RVA `2f0b30` and shared-empty header at `2f0b50`: **both have refcount 1, capacity 0 and length 0**, with preferred character pointers to self+0x1a. Those are initial PE data, not observed live refcounts. Fresh default constructor `resume213-qstring-policy-67003030.asm` stores the shared-null address then unconditionally increments its refcount. The fresh destructor capture decrements every Data refcount and frees on zero, with no `-1` static-sentinel exemption. Thus importing an immortal `ref=-1` convention from another Qt release is unsupported for this exact Qt4.8.4 image.

The external-storage capacity exception is also unsupported by the examined original constructors: fromRawData `72190` writes supplied n to **both capacity and size**. Fresh `resume213-qstring-policy-67072220.asm` additionally proves setRawData's unique-data reuse path writes n to +8 at `72334` and to +4 at `7233a`; its other path calls fromRawData. Both can represent nonempty external strings, but neither produces capacity 0 with nonzero size. This is a bounded examination of these paths; it is not a global proof that no hand-built/corrupted/transient state could ever have other field values.

For CP213's canonical input, main's decoder constructs a default QString and calls setUnicode with **34**, which calls resize, obtains inline capacity sufficient for 34 units, writes logical size 34, copies 68 bytes, and returns a reference-counted copy. Positive refcount and `capacity >= size == 34` are therefore justified conservative expectations at the outer entry, and at acceptance entry if the same value has remained intact. No exact positive refcount should be imposed because aliases change it. `ref <= 0` or `capacity < size` should be recorded as **unexpected-for-qualified-path/incomplete snapshot**, retaining fields, rather than accepted as a valid attributed path or declared a universal Qt corruption diagnosis. The generic data reader can still preserve raw fields when rejecting attribution. Refcount 0/-1 and external capacity-0 nonempty cases are useful negative-policy fixtures, but are not established valid-path fixtures for this binary. Map readability cannot override these path expectations or prove allocation liveness.

Read sibling `resume213-qstring-review.md` after it became available. Its independent getter/hash/layout PASS agrees with these offsets and its ephemeral actual-agent probes reject ref 0/-1 and capacity below 34. Its generated-reader HOLD concerns identity, exact range lengths, diagnostics and mapped-range coverage for the specific reviewed hashes; those findings remain the runner/reviewer owners' responsibility. This clarification does not override that HOLD or claim approval of a later runner revision.
