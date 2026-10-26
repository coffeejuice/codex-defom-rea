# CP171 numeric/encoding/path frontier after CP162 and CP165

## Decision

There is no new bounded original parser decision to implement in the reviewed
numeric/encoding/path lane. The next defensible step is an original-runtime
witness for a **new, exact one-value `SW2SP.DAT` token** outside the finite
CP84 oracle. Candidate source already routes the reachable one-value sidecar
through the Qt 4 base-0 parser, and its known comma-prefix cases are present.
Until one new token produces a documented caller-visible distinction, adding a
parser rule would be extrapolation rather than recovery.

Keep the ShapeOptim path boundary closed at its existing supported owner
profile. The positive rooted `DoeStudies/<study>/Control/Opt/` attempt is
already integrated before the later ROI geometry-name guard. Its false
`QDir::mkpath` branch still stops at the unrecovered modal-delivery boundary.
Do not treat that path behavior as a fresh end-to-end Open result.

## Integrated reachability and duplicate check

- CP84's source-qualified reader is the actual one-value `SW2SP.DAT` route:
  original `FUN_1410da690` requests one integer, ignores QTextStream status,
  and returns after successful file open. The candidate dispatches this
  filename/count profile to `detail::parseQt4Base0Int` in
  `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`.
- CP84 already bounds case-folded hexadecimal, binary/octal/decimal selection,
  signed values, prefix failure, overflow low-word behavior, and the comma
  delimiter result. The current adapter test at
  `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp` covers
  `1,234`, `2,345`, comma suffix, and the filename gate. Thus neither of the
  formerly proposed comma-prefix increments is a new task.
- CP162 SPRING2's four explicit UTF-16/32 BOM forms and exact original BMP
  decimal digit category are already implemented in the private companion
  reader, with malformed/non-BMP/unsupported-count profiles still guarded.
  Reopening that cut would duplicate CP162.
- CP164/CP122 ShapeOptim `studyFolder` evidence and tests already cover the
  rooted path, call timing, successful native directory creation, and
  persistence across the later geometry guard. False-mkpath modal completion
  remains intentionally guarded.

## Exact next witness required

Choose one concrete ASCII byte sequence not already in the CP84 oracle and
capture it at the **same original one-value SW2SP reader occurrence**. The
record must bind:

1. Original GUI and QtCore image hashes, plus the actual `SW2SP.DAT` call path
   and requested count of one.
2. Fresh QTextStream state, exact file bytes, and the selected `getInteger`
   branch.
3. Extracted integer, conversion status, and exact unread QChar/cursor position
   when the getter returns.
4. The caller's final destination value and successful-open return behavior,
   confirming that status and trailing bytes do not change the one-value result.

Only if this observation identifies a new result should the next parser change
add that first discriminating case under the existing one-value SW2SP route.
Do not infer arbitrary comma grouping, a second extraction, multi-value
SW2SP, a different numeric locale/options profile, or SPRING flag behavior.
No particular unobserved token is being proposed as accepted.

## Why encoding is not an alternative source-ready cut

The CP162 evidence establishes only explicit well-formed UTF-16/32 BOM decoding
and a BMP decimal-digit path in SPRING2. The older CP117 qualification says
that BOM-less decoding depends on process `codecForLocale()` startup state;
UTF-8 non-ASCII and ACP bytes are not source-qualified by the BOM captures.
Accordingly, the exact witness for any such extension is a same-process
original reader observation that binds startup language/codec identity, raw
bytes, decoded QChars, extraction result/status, unread cursor, and caller
result. Do not turn the four-BOM evidence into a BOM-less or UTF-8 rule.

## Verification boundary

This audit read the CP162 digit evidence/implementation/source review, CP165
numeric frontier, CP84 SW2SP oracle, CP164 ShapeOptim path note, CP117 codec
qualification, and the current reader/test call sites. No tests, build,
original runtime, GUI/app-control, live REA, or mutable state were accessed.
Existing test receipts are evidence of prior coverage only; this audit did not
rerun them. This report is the only write.
