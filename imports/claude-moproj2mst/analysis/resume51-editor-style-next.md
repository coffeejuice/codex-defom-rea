# Next brush branch after empty linear/conical gradients

The smallest new style16 decision consumes one packed focal-radius word. For a
finite decoded magnitude greater than1e-12, the original immediately classifies
the radial gradient as extended, and `isOpaque` returns false without reading
coordinates, radius or stops. This is a narrower successor than reconstructing
all radial geometry. Style24 already has a complete isolated image helper; its
next dependency is actual brush/runtime binding, described independently in
`resume51-style24-editor-hook.md`.

Owner: `editor300`. This file is the only write. No source, build, original
execution, GUI control or mutable REA changes. Token measurement unavailable;
"smallest" describes fields/control flow, not measured engineering speed.

## Immutable evidence

Original QtGui DLL SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Tool: Visual Studio dumpbin at
`C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`,
SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.

Existing `resume48-editor-next.json` captures the `6511aba0..6511ac1e` unwind
region. New read-only `/disasm:bytes /range:0x6511ac1e,0x6511ace1` inspection
closes its normal continuation through both returns. The whole logical normal
interval `6511aba0..6511ace2` has native-byte SHA-256
`0dd662a2d4e47f1af246a7e57b5a9a282df5f9c342d1c96f4f950ae3062cc6e4`.
Its first unwind-region endpoint alone would not have established the full helper.

| Native interval, end exclusive | Native-byte SHA-256 |
|---|---|
| `6511abb6..6511ac14` first field/early return | `86bc93af2df8ac200b26b2c9541405351e4809e4507436a906e6a421215a21c0` |
| `6511ac14..6511ace2` coordinates/return | `b2ea659a436280bf7d02b97ebfe2de40c2007adb21f84571c37f2a00b91dd80d` |
| `6511a370..6511a384` exported focalRadius leaf | `c553dd6db829985b4724534f9a01ed2f8a65319059af7d8d336c88e7c001ae6c` |

Export-map names were read from the retained PE-derived export map. Additional
bounded disassembly confirmed `QRadialGradient::focalRadius6511a370` reads+38,
masks low3bits, reinterprets float32 and converts to double. `radius` and
`centerRadius6511a350` read double+30. `center6511a3c0` reads+10/+18;
`focalPoint6511a2e0` reads+20/+28. The partial center-getter slice was used only
to identify those loads, not to claim an entire getter capture.

## Exact style16 decisions

The outer `6511c090` first consumes brush QColor alpha for every entered nonzero
brush style. It then calls `isExtendedRadialGradient6511aba0`. True causes
`6511c0da..c0dc` to return **nonopaque**, before the stop-vector getter. False
continues to the gradient-stop route. Brush alpha does not determine this result.

Let G be the embedded gradient at actual brush data B+70:

| Read | Meaning established by native getters/helper |
|---|---|
| G+38 uint32, therefore B+a8 | packed focal-radius bits; low3bits ignored |
| G+10/+18 doubles | center x/y |
| G+20/+28 doubles | focal x/y |
| G+30 double | center radius |

First, `6511abd0..ac07` computes:

`masked = rawWord & 0xfffffff8; focal = double(bit_cast<float>(masked))`.

It compares focal against0, flips the double sign bit when negative or unordered,
then compares the result with the constant epsilon. Immutable literal reads:

| Address | File bytes | Value/use |
|---|---|---|
| `65699fb0` | `0000000000000000` | double0 |
| `65699fc0` | `00000000000000800000000000000080` | two-lane sign XOR mask |
| `656a7d70` | `11ea2d819997713d` | double1e-12 |

For finite inputs this is absolute magnitude. A magnitude **greater** than the
double epsilon reaches `6511ac09` true. Ordered magnitude<=epsilon reaches
coordinate processing at `6511ac14`; it is not a false/ordinary-gradient result.

That continuation constructs QPointF(center), QPointF(focal), then their
difference. Imported calls resolve to QPointF constructor and x/y leaves. It
computes separate double multiplies and then `dxSquared+dySquared`, compares that
with separately squared radius, and returns true only for ordered strict greater.
Do not replace this with `hypot`, FMA, a tolerance or center-distance normalization.
Native `COMISD` unordered flags make NaN handling different between the first
epsilon comparison and the final strict-greater comparison. Floating exception,
rounding/denormal policy and nonfinite intermediates are not newly qualified here.

## Narrow style16 implementation proposal

First bind only an optional **raw uint32** focal-radius word to the actual
runtime-interned style16 brush, under the same live-widget/private-input
qualification and whole-batch conflict checks as gradient inputs. Do not accept
an observed `extended`/`opaque` flag or a decimal double that loses the packed
float rounding. No new stop-data owner is needed for this scalar.

An isolated finite-word decision can produce three states: unknown/nonfinite
guard; finite early-extended true; or geometry-continuation pending. Test masked
low3bits, positive/negative large values, +/-0, finite values below/above the
epsilon bucket, infinity/NaN guards and retry without repeated field/alpha reads.
Raw exponent-bit classification can preserve nonfinite guards before conversion.

Then add a retained `RadialFocalRadius` editor cursor after `BrushAlphaConsumed`.
For the proven early-extended result set the existing `opacityDesired_` false and
enter `OpacityDecision`; never allocate a stop getter or require stop/shared-null
inputs. Unknown word and finite-small geometry continuation retain precise
guards. Keep nonfinite/geometry paths guarded until their arithmetic profile and
actual fields are represented. The public wrapper/setup gate remains unchanged.

Later, a recovered geometry-false result may reuse `GradientStops` for known-empty
stops; the shared gradient binding currently admits only15/17 and must not be
widened to16 before that predicate is represented.

## Style24 readiness and wrapper dependencies

Read `resume51-style24-editor-hook.md` (SHA-256
`3b2f76cede76bd7a20b1f6fc688ca9194ecbe0627f3f361d5027939c113144d8`),
`resume50-style24-runtime-binding.md` and `resume48-brush-style24-contract.md`.
The existing image helper already destroys its temporary QImage before exposing
opacity, so a future editor `TextureImage` cursor can go directly to
`OpacityDecision`; it must not add another image copy/release or a gradient scan.

The smallest image route has pixmapSelected=false, imageData known null,
cachedPixmapPresent=false and paintingWord0. It returns opaque=true without
format or color-table reads. Nonnull image metadata routes are also implemented
in the isolated helper, but actual same-runtime ID aliases and brush-owned
payload links are still missing. Unknown/null are distinct; the brush alpha
prefix remains consumed even though it does not decide the texture result.

Both a narrow radial field hook and the texture hook require shared
`view_manager.hpp/.cpp` observation work, so those implementations are not
independent write sets. Isolated radial-decision recovery can proceed separately
while the texture owner prepares its atomic image observation helper. No
production radial or texture inputs follow from probe004's solid brush.
