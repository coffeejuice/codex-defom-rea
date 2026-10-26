# CP131 independent review: prepared Type3 row projections

The new private append helpers match the retained successful-call input order and row-field stores for the bounded value-only projection. They preserve append order and return the just-appended index (`size - 1`). They do not implement native parsing or native container semantics, and the existing `NUM_CON`/`REGIONS` dispatcher guards remain in place.

## `NUM_CON` mapping and return

The retained parser reads row fields in order as field 1 int, field 2 int, field 3 double, and field 4 int; it classifies field 1, constructs a QString, then calls the virtual append method with `(field1, classifier, field3, QString, field2, field4)`. The helper signature follows that call order. Its stored projection maps classifier to `value00`, field 1 to `value04`, field 2 to `value08`, field 4 to `value0c`, field 3 to `value18`, and QString to `value20`, matching the observed temporary-row writes. The tests use distinguishable values and two rows, so they check both the argument remapping and encounter order. Returned indices 0 and 1 match the native method's size-minus-one result; parser evidence says the caller ignores those return values.

## `REGIONS` mapping and guards

The observed `REGIONS` call passes integer zero plus a derived QString. The helper appends `{0, value}` and returns the same size-minus-one index. Two distinct input strings and sequential indices are tested. This represents only already-derived values; the regex, row conversion, QString derivation, file parser, native shared-data members, and allocation/cleanup behavior remain unimplemented. `NUM_CON` and `REGIONS` still return `RowBodyNotImplemented` from the tokenized scalar dispatcher, so this addition alone does not enable row parsing or the public Type3 path.

The append helpers rely on the stated caller precondition that the resulting count fits the positive int32 domain. Without that precondition, converting `vector::size()` to int32 would not reproduce a general native large-count or allocation-failure contract. Native temporary construction/destruction and exception interleavings are also explicitly excluded.

## Evidence and assessment

The implementation report binds the projection to the original executable SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`, the retained parser assembly, vtable data, and bounded helper windows in `resume131-type3-next.md`. Those windows support the successful-call argument/store order and return computation. The unresolved global QRegExp and tokenization prevent validating the values produced from `.OPT` input; the report does not claim that coverage.

No discrepancy found in the value projections or guarded scope. No build or tests were run during this independent review; no original runtime differential is claimed.
