# CP136 independent NUM_CON sequence review

No blocking discrepancy remains in the repaired private sequence source/test set. The initially observed CP135 regression fixture expected NUM_CON to remain guarded at its header; it now uses the still-guarded REGIONS key, preserving its earlier-effect/guard-index assertion. The header comment was also updated to describe the admitted NUM_CON profile. Both repairs are included in the final implementation receipt and hashes below.

## Original qualification and order

Independently read the immutable PE by its section mapping: original `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the 16 bytes at VA `1427b9f0f` are `005c732b0052756e00436f6e74726f6c`. The QString argument loaded at parser `140908d23` therefore points at a leading NUL, confirming empty content rather than an unresolved formatter. Candidate `qt::String(std::u16string{})` is explicitly non-null and empty under its actual constructor definition.

Retained parser ASM `analysis/resume127-type3-opt-parser.asm` shows count conversion at `140908bb8`, signed count comparison at `140908bea..140908bf1`, EOF check before each row at `140908bf7..140908c07`, then line read/split. It converts fields 1,2,3,4 as int/int/double/int at `140908c48..140908ce5`, classifies field1 through vtable+a0, constructs the empty QString and appends via vtable+8 at `140908d8e`. Completed appends precede the next count/EOF test, and count completion/EOF rejoins the top-level loop at `140908db4`. The implementation follows that encounter order for admitted inputs and retains completed rows on early input exhaustion. Signed nonpositive counts perform no row iterations, matching the native signed comparison.

The native append argument order remains field1, classifier, field3, empty QString, field2, field4, as projected by the existing prepared-row helper into classifier/value04/value18/value20/value08/value0c. The new tests check every projected value for a complete row and preserve source row order across early EOF. ENDDATA after the declared rows stops top-level processing before a later scalar mutation. A token encountered inside a declared row remains row input, rather than gaining top-level ENDDATA semantics.

## Candidate guards, ownership and tests

Short rows, failed/non-ASCII numeric conversions, nonfinite doubles, surrogate lines and vector-size bounds are explicit candidate admission guards. Native numeric calls pass null success flags, so these guards must not be presented as original rejection behavior. The receipt correctly preserves that distinction and does not assign native file-parser status to AllLinesConsumed. Earlier scalar writes/accepted appends survive a later guard; the malformed-row test checks this directly. The index points at the unapplied row, while REGIONS still stops at its own top-level header.

The sequence borrows its span/state synchronously and appends owned value rows; it does not retain a source-line pointer, construct a native optimizer or claim Qt container/refcount/EH ownership. Source allocation/exception/file-stream cleanup fidelity remains outside this value projection. Private call references remain confined to the implementation/header and focused tests. `ShapeOptimClass::loadClassData` still refuses type3's missing native optimizer owner. No file reader or public Type3/ShapeOptim path was enabled.

Two optional focused additions would strengthen the new test: assert `!row.value20.isNull()` alongside isEmpty, and cover zero/negative count followed by a scalar. They are nonblocking because the constructor definition and signed loop directly establish the stated behavior; no contrary source path was found. Root should run both sequence test executables, including the repaired CP135 regression, after compilation.

## Final source binding

Final hashes match the repaired `analysis/resume136-type3-numcon-sequence.md`, SHA-256 `5d534b1a0d09d83b6617a14396c5c494a07ff29134671f1c222c92d8fe0001e1`:

- Sequence header: `4552c72444172220d46f0fc6c6604ed264f4f4a2726cb129d98d11674884a4e2`.
- Sequence implementation: `b4305aff7678d0f082ed7602c2db6e9562d6969d53868934ce8977fd17d20e28`.
- Existing scalar-sequence test: `326d862981679be5fd9dff1497f6349ecf4238aed600f631ba0c2aa92c2c40d1`.
- New NUM_CON-sequence test: `db7d3df210098b6863da8021ac8b4a568f7a0d5240756dddbd3310f81a7e0d63`.

Only this report was written. No source/test edits, compilation/test execution, GUI/original process execution or mutable REA actions were performed by this reviewer.
