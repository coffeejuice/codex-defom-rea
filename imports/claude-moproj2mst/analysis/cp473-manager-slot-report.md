# CP473 DOE manager `+0x1c0` slot and method

## Result

For the normal `SfMoDoeManager` dynamic type, vtable slot `+0x1c0` resolves to `0x14091bcb0`. The method constructs and returns a temporary list of DOE variables whose link field at `+0x40` is null. Before testing each link field, it writes that variable's encounter index into `+0x18`; this mutation applies to linked and unlinked variables alike. The visible method does not modify the source manager list's membership or order.

The Type3 `.OPT` writer uses this returned QList for its size, then emits the count and a fixed numbered sequence. The writer does not retrieve or serialize the returned list's individual entries. Its writer call order and this manager method's side effect are distinct: the method can reindex variables even though `.OPT` output consumes only the list size.

## Byte binding

The input is the retained original `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The verifier maps VAs through PE sections and takes the full method bound from the PE x64 exception directory.

| Evidence | Result |
| --- | --- |
| Vtable base | `0x1421671b8` (`SfMoDoeManager`) |
| Slot VA (`base + 0x1c0`) | `0x142167378` |
| Slot file offset | `0x2165b78` |
| Raw qword | `b0 bc 91 40 01 00 00 00` |
| Resolved target | `0x14091bcb0` |
| Method range | `[0x14091bcb0, 0x14091bd9d)` |
| Method size | 237 bytes; 57 decoded instructions |
| Raw method SHA-256 | `f287cbd6ec06628ca3324c8bcfeba867dd7cd2766e59e7264c49bb9423483ad9` |
| Byte mismatches | 0; every addressed dumpbin instruction byte matches the PE |

The disassembler is Microsoft `dumpbin` 14.44.35228.0 using `/DISASM /RANGE`. Reproduce the check with:

```powershell
python analysis/cp473-manager-slot-verify.py
```

The script writes the bounded disassembly and machine-readable receipt beside itself. The disassembly SHA-256 and all capture metadata are recorded in `cp473-manager-slot-verification.json`.

## Visible method sequence

The entry receives `this` in `RCX` and caller-provided hidden QList return storage in `RDX`.

1. Copy the manager's list at `manager+0x38` into a local source QList.
2. Iterate its members in increasing index order from zero to the copied list size.
3. Fetch each variable and store the current index to `variable+0x18`.
4. Read the variable's `+0x40` link pointer. A nonnull pointer skips the result append; a null pointer is appended to the temporary result QList.
5. Copy-assign the result QList into the hidden return storage, then destroy the local list temporaries.

Instruction anchors in the verified range: list copy at `0x14091bcd4`; size at `0x14091bd03`; indexed retrieval at `0x14091bd17`; reindex store at `0x14091bd28`; link getter at `0x14091bd3c`; nonnull skip at `0x14091bd44`; append at `0x14091bd5c`; result assignment at `0x14091bd68`.

The manager's list is copied before iteration. The returned result is a separate temporary QList, populated only for null-link variables. This establishes method-level intent and visible call order; it does not establish Qt QList's internal reference counts, copy-on-write detach details, helper failure behavior, or allocation-failure cleanup. Those helper implementations were not included in this bounded capture. The report is qualified to the `SfMoDoeManager` table installed by constructor `0x1400d1180`, not arbitrary manager types.

## Provenance and limits

The retained inventory in `evidence/gui_mo/s1/s1-05/spec.md` around lines 750–755 lists the `+0x1c0` method alongside neighboring manager slots. Constructor `0x1400d1180` installs vtable `0x1421671b8` in `evidence/gui_mo/s1/mdt/DOE/d3.c`; the qword target and full method are independently byte-bound here. The CP465 Type3 writer report establishes that its manager `+0x1c0` call feeds the count/number sequence.

No original process, GUI, mutable REA database, source code, build, or test was used or changed. This static analysis does not prove behavior after a fault or helper failure, nor does it resolve broader manager/variable ownership and lifetime semantics.
