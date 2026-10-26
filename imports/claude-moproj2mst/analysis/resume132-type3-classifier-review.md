# CP132 independent review: Type3 row-tag classifier

The private classifier matches the recovered `DefOptModel +0xa0` value map. For the 93 input tags from `0x3f2` through `0x44e`, the explicit sparse ranges and singleton cases implement results 0, 1, and 2; every other in-window entry returns -1. The test enumerates all 93 entries against the embedded byte-derived oracle. Separate checks cover `INT32_MIN`, negative values, the immediate lower/upper neighbors, and `INT32_MAX`.

## Out-of-range semantics

The native body initializes the result to -1, subtracts `0x3f2` in 32-bit arithmetic, and uses an unsigned `ja` against `0x5c` before table dispatch. For tags below `0x3f2`, subtraction wraps to an unsigned value above `0x5c`, so the default -1 is retained. For tags above `0x44e`, the nonwrapped difference is greater than `0x5c`, also retaining -1. The candidate avoids reproducing that arithmetic directly and uses signed comparisons only against small positive in-range constants; its sparse comparisons therefore return -1 on both sides without signed overflow. The test’s minimum/maximum and boundary witnesses agree with this behavior.

The reported raw table is 93 bytes at VA `0x14091f134` (file offset `0x91e534`), with table class 3 landing on the default path. The implementation report says its local static check compared all 93 test oracle values with the original PE bytes. This independent review relies on those retained source-qualified artifacts and did not repeat the PE extraction.

## Scope and guards

This helper computes only a value; it has no state writes, virtual dispatch, parser reads, row allocation, or Qt cleanup. The `.OPT` `NUM_CON` parser remains `RowBodyNotImplemented`, and public Type3/ShapeOptim support remains guarded. The prepared-row append API remains a separate already-converted-value seam and does not call this classifier implicitly. Thus the classifier increment does not close the unresolved regex/token conversion or row ownership boundaries.

No discrepancy found in the map or out-of-range result. Validation is static only: no build or tests were run, and the original runtime was not launched. The implementation report’s PE-byte/source qualification supports the finite pure helper but does not establish complete row parsing or native object behavior.
