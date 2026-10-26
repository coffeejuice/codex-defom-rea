# CP128 Type3 optimizer constructor and .OPT parser analysis

## Conclusion

The CP127 disassembly plus original PE import/vtable data closes a concrete constructor scalar prefix and much of the .OPT parser control flow. The constructor initializes a 0x50-byte object with an embedded QString, two nontrivial shared-data members, and known scalar fields. The parser opens a file, processes top-level keys in file order, and appends row records into the two embedded members. This is enough to specify a private scalar-state prefix and an exact future parser seam. It is not enough to create a source-equivalent complete optimizer owner: row helper contracts and the static split-regex initializer remain open. Keep Type3 ShapeOptim activation and populated GEO guards.

## Constructor and subordinate calls

The constructor at 0x140917ff0 is the derived constructor for the object allocated into SUB+0x28 by 0x1400b5470:

1. Call base initialization 0x14091dfc0.
2. Set vptr to 0x1427bceb8.
3. Default-construct the embedded QString at this+0x28 through QtCore IAT slot 0x141e044c0, then clear it through 0x141e045a8.
4. Write this+8 = 2.
5. Call initializer 0x1409180b0.

The base constructor 0x14091dfc0 sets base vptr 0x1427bec38, initializes embedded members +0x18 and +0x20 using temporary/copy helpers, then writes +8=0, +0x0c=1, +0x10=1. Derived initialization first sets +0x0c=0, clears the QString, then sets the following values:

| Offset | Initial value |
|---|---:|
| +0x30 | 0x3fd |
| +0x34 | 0xffffffff (-1) |
| +0x0c | 1 (after temporary zero) |
| +0x10 | 1 |
| +0x38 | 200 (0xc8) |
| +0x48 | byte 0 |
| +0x3c | 0 |
| +0x40 | 0.001 (bits 0x3f50624dd2f1a9fc from 0x1421b5050) |

It then calls 0x140919000(this+0x18) and 0x140919050(this+0x20). Each initializes a temporary through 0x140018f9e0, invokes a copy helper, and destroys the temporary. The copy helpers call verified QtCore QBasicAtomicInt::ref/deref entries and replace internal pointers, invoking cleanup when the old reference reaches zero. These members are not proven empty POD lists; their element ownership contracts remain unresolved.

Teardown is reverse ordered: derived destructor 0x140918060 destroys the QString at +0x28; base destructor 0x14091e040 restores base vptr 0x1427bec38, destroys +0x20 through 0x140919180, then +0x18 through 0x1409190a0. This closes the call order but not all payload cleanup semantics.

The vtable confirms scalar fields: +0x120 stores +0x30; +0x130 stores +0x34; +0x140 stores +0x38; +0x148 reads +0x40; +0x150 writes +0x40; +0x158 reads +0x48; +0x160 writes +0x48. Direct setters 0x14091e090 and 0x14091e0d0 write +0x0c and +0x10. Vtable +0xa0 resolves to classifier 0x14091f0b0; it subtracts 0x3f2, returns fallback for values above 0x5c, and otherwise uses a lookup/jump table. It classifies a row tag; it does not establish the entire row contract.

## .OPT parser: conditions and effects

I resolved parser IAT slots against the immutable PE's QtCore import table. At 0x140908630, the second argument is checked first. Null optimizer returns false before constructing QFile. Otherwise the function constructs QFile from the path argument and calls QFile::open with a mode value built from immediates 1 and 0x10. Open failure builds the retained “Failed to open file, %1” text and returns false; no optimizer mutation precedes successful open.

After successful open, the parser calls optimizer vtable +0x160 with true, constructs QTextStream on the file, and reads lines until atEnd. It splits each nonempty line with QString::split using a static QRegExp object at 0x143dd9a28, compares token zero, and handles keys in file order. The QRegExp's initialization and pattern are absent from retained evidence, so the exact delimiter remains unknown.

| Key | Observed effect |
|---|---|
| DEF_OPT, TYP_OPT, GRD_TYP | Recognized and skipped. |
| OPR_NO | Token 1 toInt(ok=null, base 10), vtable +0x140 stores +0x38. |
| KRG_MAX | Token 1 toInt(ok=null, base 10), 0x14091e090 stores +0x0c. |
| TYP_OBJ | Tokens 1, 2, 3 toInt; store respectively at +0x30, +0x10, +0x34. |
| NUM_CON | Token 1 is row count. For each available following row, split fields, convert selected values to int/double, classify one tag through +0xa0, then vtable +0x8 appends a row to member +0x18. EOF exits the block without undoing earlier rows. Record field meanings and malformed-row behavior remain unclosed. |
| DOE_PAR | Recognized and skipped. |
| REGIONS | Token 1 is row count. Each following row becomes a string-list value appended to member +0x20 through vtable +0xa8. EOF exits without undoing earlier rows. Exact split pattern and row schema remain open. |
| EPS_OBJ | Token 1 toDouble(ok=null), vtable +0x150 stores +0x40. |
| DB_PURGE | If token 1 exists, toInt(ok=null, base 10); store false when result equals 1, otherwise true. Missing token stores false. vtable +0x160 writes +0x48. |
| ENDDATA | Terminates the loop and reaches file close/cleanup. |

Unknown top-level tags reach line cleanup and the next-line loop rather than a visible error. Numeric conversion calls pass null ok pointers. The parser has no entry-time clear or transaction for its row members. Each append occurs immediately, so later EOF/exit retains prior rows.

The return local at stack +0x4c is set to zero and has no other store or address escape in the complete disassembly; both return paths load it. The parser therefore returns false on the observed paths even when it opens and parses. The retained caller stores but does not branch on the result. A false return must not be treated as proof that parsing had no effects.

The first line is separately read, a trimmed temporary produced and destroyed, and toInt(base 10) stored in an otherwise unused local before the main atEnd loop. Its semantic purpose is unknown; do not infer it is a version field.

## First source-closed private prefix

The smallest bounded increment is a private scalar-state value, not a complete SUB+0x28 owner. It can encode the exact constructor defaults for +0x0c, +0x10, +0x30, +0x34, +0x38, +0x3c, +0x40, +0x48, and an initially empty +0x28 QString. Pure tokenized-field operations can reproduce the resolved OPR_NO, KRG_MAX, TYP_OBJ, EPS_OBJ, and DB_PURGE scalar writes, preserving conversion order and the observed DB_PURGE boolean mapping.

Keep actual .OPT tokenization and NUM_CON/REGIONS rows guarded. The static QRegExp at 0x143dd9a28 lacks its initializer/pattern, and +0x18/+0x20 row records use shared-data helper chains whose layouts, copies and cleanup are not closed. A scalar prefix must not claim native allocation/refcount parity, parser equivalence, listF0 equivalence, active ShapeOptim load, or GEO effects. If a complete owner is required, those are the first blockers.

## Evidence qualification

Original binary SHA-256: FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC. Reused bounded CP127 dumpbin captures and performed read-only inspection of the PE import table, vtable data at 0x1427bceb8, static strings at 0x1427ba0c0..0x1427ba150, and direct helper instructions. Dumpbin is Visual Studio Build Tools MSVC 14.44.35207. No Ghidra import retry, mutable REA analysis, source edit, build, test, GUI action, or original execution was performed. Public Type3 activation and ShapeOptim GEO import remain guarded.
