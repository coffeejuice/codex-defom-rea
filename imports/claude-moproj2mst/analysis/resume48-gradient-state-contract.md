# Shared gradient stop ownership before editor opacity

The next gradient increment must represent the brush's embedded gradient and its
stop-vector owner in the shared widget runtime. A list of alpha bytes stored only
in `EditorBaseConstruction` would lose the source copy/detach decision. This
contract permits an internal shared-state increment; it does not admit public
mode 300 or establish a completed QWidget constructor.

Owner: `gradient_state`. Exclusive output: this file. Read-only original PE
inspection and source review only; no source edits, builds, original execution,
GUI control, or mutable REA. The `editor300` lane was informed and retains
gradient guards for candidate 48. Its constant-false style closure is independent.
Tokens unavailable; no speed or token-saving claim.

## Evidence and qualification

The oracle is retained `evidence/original-binaries/QtGui4_64.dll`, SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
The native implementation is the evidence; general Qt documentation was not used
to fill unknown fields. All addresses below use its preferred image base.

Read these existing qualifications with the contract:

- `analysis/wave3-editor-parentless-opacity-static.json` and the corresponding
  `wave3-editor-parentless-opacity-6511c090.asm`: full captured opacity region,
  including style dispatch, stop scan, and release paths.
- `analysis/resume46-editor300.json`: bounded `6511aba0..6511abb6` entry and
  `6511acd7..6511ace2` false-return slices. These establish the style-not-16
  exit, not all radial-gradient behavior.
- `analysis/resume48-editor-next.json`: retained `QGradient::stops` capture
  `6511b390..6511b500`, byte hash
  `0ecea367fef00421474747790ffd85fc8e3362bba35f4330377b18483a7542a2`;
  QColor global-color constructor/setRgb captures and literal reads. The capture
  explicitly identifies unwind-region bounds, not proof of an entire logical
  procedure across exception funclets.
- `analysis/wave3-editor-probe004-palette-binding-contract.json`: actual runtime
  palette/brush identity qualification and observed **solid** brush. That probe
  did not observe any gradient and cannot seed one.
- `analysis/wave3-editor-wrapper-palette-6511e6d0.asm`: QColor alpha leaf reads
  exactly byte `color+5`; no color-spec conversion or inferred RGB interpretation.

Additional independent read-only PE inspection on 2026-10-24 used dumpbin
`C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`,
tool SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
The new helper qualifications can be reproduced with `/disasm:bytes` and these
inclusive ranges against that same DLL:

| Range | PE unwind end, exclusive | Native byte SHA-256 |
|---|---|---|
| `/range:0x6511a860,0x6511aa25` | `6511aa26` | `ea1074ee442a45f3b34c812f9a221f3b888ca73fa24e3198a8e9c5b4a8ce14ed` |
| `/range:0x6511ae60,0x6511af77` | `6511af78` | `eebca89f2b89371d13dd139af7c9ea65bb331caa5517980efae9e41a4b850624` |

These hashes were recomputed from the file-backed PE bytes, independently of the
disassembly text. The interpreted normal paths end at RET and include all direct
branch targets used below. Allocator failure/exception cleanup was not validated.
No new sidecar evidence files were created because file ownership is exclusive.

PE import-table resolution, independently read by IAT address:

| IAT address | Imported QtCore4_64.dll symbol |
|---|---|
| `656968c0` | `QBasicAtomicInt::ref()` |
| `65696978` | `QBasicAtomicInt::deref()` |
| `656968f8` | `QVectorData::free(QVectorData*, int)` |
| `65696a70` | `QBasicAtomicInt::operator!=(int) const` |
| `65696b50` | `QBasicAtomicInt::operator==(int) const` |
| `65696b78` | `QVectorData::allocate(int, int)` |
| `65696bc0` | `QBasicAtomicInt::operator=(int)` |

## Exact reached state and decisions

Let `B` be QBrush data and `D` the stored QVector data owner. Offsets are native
layout evidence, not a proposed C++ ABI:

| Field | Source instruction/effect |
|---|---|
| QBrush value `+0` -> `B` | `6511c0ad`, `6511c0c1` |
| `B+4`: signed 32-bit style | `6511c0c4`, `6511c0e4..c0fc` |
| `B+8`: QColor; `B+0d`: alpha high byte | `6511c0b0..c0be`, alpha leaf |
| `B+70`: embedded QGradient | `6511c201` |
| gradient `+8` -> `D`, therefore `B+78` -> `D` | `6511b3b0` |
| `D+0`: atomic ref count | imported ref/deref/compare calls receive `D` |
| `D+4`: capacity, `D+8`: signed count | `6511b495..b499`, `6511b4e4..b4e8` |
| `D+0c`: flags; bit 0 controls shareable return | `6511b48a..b49c`, `6511b4dd..b4ef` |
| `D+10+24*i`: stop position, 8 bytes | `6511a97e..a981`, `6511aeaa..aeb6` |
| `D+18+24*i`: QColor, 14 meaningful bytes copied | `6511a984..a996`, `6511aeb9..aec7` |
| `D+1d+24*i`: consumed alpha byte | `6511c228..c232` plus alpha leaf |

Every entered `isOpaque` first consumes brush alpha and compares it with 255
before examining style. For style 15 or 17, `6511aba0` immediately takes its
style-not-16 exit to `6511acd7` and returns false. Thus it does not inspect radial
geometry. `6511c0e4..c0fc` selects the gradient route, and `6511c201..c20d`
passes the embedded `B+70` gradient to `QGradient::stops`.

`QGradient::stops(6511b390)` returns a vector **value** through its output pointer:

1. `6511b3b0..b3b8` reads the embedded source vector pointer and count. Exactly
   zero count takes the default-building branch; it does not change the gradient's
   stored pointer or count. Unknown count must remain unknown.
2. A nonzero count uses `6511b4d1..b4d4`: copy the source pointer into the returned
   value and increment its atomic ref count. If flags bit 0 is set, return that
   same owner. This is a temporary additional value reference to source data.
3. If bit 0 is clear, `6511b4e4..b4ef` invokes helper `6511a860` with source
   count/capacity. Because step 2 retained a reference, the helper sees a shared
   owner under valid ordinary ownership. `6511a8c2..a8d4` therefore allocates at
   `6511a8db..a938`; new count starts at 0, ref count is assigned 1, capacity is
   copied, bit 0 is set and bit 1 is preserved. `6511a93d..a99d` copies records
   in source order, then `6511a9da..aa17` releases the old temporary reference
   and installs the new pointer in the returned value. The brush's source owner
   remains the same. Position bits and meaningful QColor bytes survive the copy;
   pair padding is not a reconstructed value.
4. Zero source count builds a local list from imported QVector shared-null data,
   then appends two stops using `6511ae60`: position 0 with GlobalColor 2 (black),
   position 1 with GlobalColor 3 (white). The constructor/setRgb evidence gives
   alpha 65535, hence consumed alpha byte 255, for both. The result retains the
   local list owner, applies the same flags-bit-0 copy rule if needed, and releases
   the intermediate local value at `6511b4a2..b4c6`. This is a new returned
   default list, not a persistent rewrite of an empty source list.

The append helper checks ref-count inequality to 1 and required capacity at
`6511ae82..ae9f`, invokes growth/detach when necessary at `6511aed0..af1b`,
copies the same position/QColor fields, then increments count at `6511af5d`.

After the return, `6511c21b..c246` scans the returned vector's stop QColor alpha
bytes in order, stopping at the first value other than 255. It releases that
temporary owner on either exit (`6511c24a..c28f`) and returns false for the first
nonfull alpha, true after the full scan. The original brush alpha comparison is
consumed but does not determine this gradient result. No stop-position sorting,
color interpolation, RGB conversion, or normalization is reached by this scan.
The generic signed-count `<=0` scan exit exists in the binary; do not silently
reinterpret an unobserved/malformed negative count as the valid empty list.

## Smallest shared representation and branch decision

Keep the existing runtime-interned `WidgetBrushDataState` as the QBrush-data owner
selected through `WidgetPaletteDataState::brushAt(group, role)`. Extend it with
an optional gradient value referring to a separately interned stop-data owner.
Do not create an editor-only brush, palette, gradient, or observed opacity result.

For this reached branch, the gradient value needs only its optional stop-data
reference: geometry, spread and other gradient fields are not read for styles
15/17. A stop-data owner needs a positive runtime-local alias, known/unknown
signed count, capacity, flags bits 0/1, and an ordered complete/unknown payload.
The payload record is the 8-bit-pattern bytes of `double` position plus the 14
meaningful QColor bytes. A packed modern representation can use `uint64_t`
position bits and `array<uint8_t,14>` color; derive alpha from color index 5.
This retains exactly the fields copied by the recovered detach, excludes
padding, and avoids a redundant potentially contradictory alpha field per stop.
The existing brush-level alpha remains distinct from stop alpha.

Use a separate explicit vector-value wrapper for source/returned values sharing
that owner. A same-source sharable return retains the alias; a nonsharable return
allocates a fresh owner, preserves payload/capacity/bit1, sets bit0 and releases
the temporary old reference; an empty source constructs a separate default owner.
If physical atomic count is needed as a tracked contract, record qualified
external references separately from these modeled value references. Never use
`shared_ptr::use_count()` as the Qt atomic count: runtime alias registries add
host references that the native vector does not have. Do not invent absolute
ref counts from a palette or brush alias.

Observation APIs must use existing runtime/live-owner qualification and monotonic
unknown-to-known/equal merging. Stop aliases are distinct from brush aliases;
equal content does not prove identical native owners. A source count of 0 is
different from unavailable payload. Conflicting aliases/counts/capacity/payload,
foreign-runtime links and expired observation receivers must be rejected before
mutating existing owners. Full known payload count must agree with known count.
No host success/opaque boolean is an input.

Decision for candidate 48: **retain style15/17 guards**. Shared state and a tested
copy/retain/release helper should land first. Source permits a later narrow
internal style15/17 branch once its ownership prerequisites are represented;
style16 radial geometry, style24 texture, native-window opacity, application
wrappers and public `6504abb0`/mode300 gates remain independent unresolved work.

## Focused prerequisites before admitting style15/17

- Same palette slots and brush aliases expose the same embedded source stop owner;
  distinct brush data can share stop data without being collapsed into one brush.
- Unknown source pointer/count/flags/payload blocks at its exact read, after the
  already-consumed brush alpha; retries do not repeat palette-group or QObject
  prefix effects. Never snapshot an unknown list as empty.
- Nonempty sharable returns keep the alias and add/drop one modeled vector-value
  reference. Nonsharable returns use a fresh alias, preserve ordered position
  bits/QColor14/capacity/bit1, set bit0, and leave source payload/owner unchanged.
- Empty source produces returned positions 0/1 with opaque black/white, consumes
  and releases the intermediate list and returned value, and leaves source empty.
- Both styles 15/17, brush alpha 0/255: all full stop alphas return true; first,
  middle and last nonfull stop alphas return false at the matching scan position.
  A qualified prefix nonfull alpha may terminate the scan before later stop
  alpha reads, but any reached nonsharable deep copy still requires its full
  copied payload before that scan can begin.
- Opacity mutation still precedes the native-window helper; changed/equal paths,
  raw120 guards, and retry cursors remain as existing source ordering requires.
- Observation rejection does not partially overwrite shared owners. Test wrong
  runtime, duplicate/conflicting aliases, incomplete payload and count mismatch.
- Public factory and mode300 remain guarded; no constructor-wrapper completion
  claim follows from passing these internal focused checks. Run the converter's
  required regression gate only under the orchestrator's next source/build freeze.

These are proposed test prerequisites, not executed tests. Fresh original gradient
runtime observation and allocation-failure parity remain unestablished.
