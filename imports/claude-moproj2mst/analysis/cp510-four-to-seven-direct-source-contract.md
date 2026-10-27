# CP510 selected four-to-seven direct placement

This checkpoint adds a static parser/model projection for an initially empty
chart with four to seven ordinary, nongroup, initially unlinked
`SfOprForming2D` items. It does not establish fresh native Open, Save, Close,
notification, rendering, or failure parity.

## Original source qualification

The immutable `DEF_GUI_MO_64.exe` SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
An independent DUMPBIN 14.44.35228.0 audit checked 21 bounded GUI spans,
16,177 contiguous bytes and 3,148 addressed instructions against mapped PE
bytes with zero mismatches. The original row lookup
`14014dfa0..14014e179` hashes to
`6101966ca147a3f060f7e92a6857f669a22898b3deb6e01157b4e0d478e43fd8`;
row-height rebuild `140157fe0..1401583ce` hashes to
`4800f06d904dfa13555f738873e8cf3bb6aa5f3bd976ab56a089f096d417d65c`.
The graph chain `140084ad0..140084ef4` hashes to
`332c034f751878c92b575d78954faebc97d13cd714bb39ed29e4975f92bba7a5`;
MST projection `1400b6630..1400b745d` hashes to
`9f2a04dcc677f82ec5e1b59815c7deef16574f77e422667466636da1cd17c01a`.
Decision, rounding, installer, relayout and graph body hashes match the CP507
receipt. QtCore `QRectF::moveTo` is an independently checked x/y-only leaf,
preserving parsed width and height bits.

## Selected contract

Snapshot all parsed RectFs, CellPos values and links before placement. Every
rectangle component is finite and every x is below 1332.5. The first item
has y below 67.5; each later item has y below 160. The first row lookup sees
an empty row table. Thereafter the populated row has minimum height 185, so
160 is the maximal common bound independent of item-specific graphics height.
Exact 160 is outside this common route, not proven universally invalid.

Each final CellPos is either explicit `(0,c)` with `0<=c<=6`, or missing/OR
sentinel (`row==-1 || col==-1`). A sentinel discards its other coordinate and
uses the native geometry-derived column: clamp x below -60 to zero, then
half-up round x/205. All snapshot target columns must be distinct and in 0..6.
Every installation is therefore into an empty token; an explicit-path local
direction value cannot affect the result.

Each final cell is `(0,c)` and its RectF is `(205*c,+0,w,h)` with parsed signed
width and height bits preserved. IDs and model-vector order remain unchanged.
The graph connects adjacent occupied columns lower to higher, reciprocally.
Empty columns break connections. MST order starts at the lowest occupied
column and follows its contiguous linked run.

## Remaining guards and validation

Explicit collisions, duplicate target columns, nonfinite or outside-envelope
geometry, other classes/groups, initial links or preseeded charts, and counts
outside four to seven remain outside this increment. CP507's three-item bounds
remain unchanged. Focused tests cover four through seven items, mixed explicit
and sentinel forms, signed/extreme dimensions, varied target order, graph gaps,
and exclusion controls. Full-suite and fixture results belong in the CP510
freeze receipt, not in this source contract.
