# CP142: Private Cogging coupled-reference increment

## Source-closed decision and effect order

The native branch is fully described once the caller supplies the result of `QFile::exists("MAN_COUPLE.DAT")`; it does not inspect file bytes. In `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §5 and the matching retained decompile, the boolean is sampled only on the `spring != 0` path, after object 4 is translated and its spring fields/direction are written. The conditional object edits occur later at two distinct points:

1. Object 5: copy object 4, copy inter-object data, rotate 180 degrees, optionally assign movement reference id 4/value 1.0, then call Router and ViewerSync.
2. Object 7: copy object 4, copy inter-object data, translate, write direction -1, rotate 180 degrees, optionally assign movement reference id 6/value 1.0, then call Router and ViewerSync.

Object 6 has no coupled-reference assignment. The existence result is not a global model property, and the assignments must not be moved before the respective copy/transform sequence: each copy would overwrite them. With `couple == false`, native code performs no conditional write at these points; any value inherited by the copy remains as copied. Null object 5/7 skips its assignment and following router call.

Current candidate code matches the ordering sites but always leaves `couple` false when the file is absent and refuses when its project-relative probe is true (`source/apps/moproj2mst/src/engine/cogging_save.cpp`, around the spring branch and object 5/7 blocks). CP141 leaves the public lookup-directory mapping unresolved; this receipt does not relax that guard.

## Smallest private helper and test

Keep filesystem lookup separate from movement mutation. The smallest helper is an internal `applyCoupledManipulatorReference(LLOBJ_DATA* object, bool couple, int referenceId)` that does nothing for false/null and, for true, writes exactly the 32-bit integer at object offset `0x22c0` and the 64-bit double `1.0` at `0x22c8`. Call it only at the two current native-order points with `(o5, couple, 4)` and `(o7, couple, 6)`. The boolean should be passed into the private placement path explicitly by any future caller; this helper must not resolve a path or read process CWD.

A focused unit test can construct zeroed `LLOBJ_DATA` records and check:

- true/object5 yields id 4 and exact double 1.0;
- true/object7 yields id 6 and exact double 1.0;
- false leaves sentinel bytes at both offsets unchanged;
- null is a no-op.

For integration confidence, retain the call placement after copy/rotate (object 5) and after copy/translate/direction/rotate (object 7), before the corresponding Router call. A larger `cogPlaceManipulators` fixture can then assert final object KEY fields, but it is not needed to validate this isolated value projection. Do not add a public file-present test until CP141's Save-time directory witness is resolved.

Evidence basis: native pseudocode and operand offsets in `spec_L1B.md` §5; current candidate effect order in `cogging_save.cpp` around lines 590–641. Read-only inspection only; no source/build/GUI/REA state was changed.
