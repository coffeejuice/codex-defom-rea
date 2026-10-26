# Next editor300 increment: returned gradient-stop value

The next bounded branch is style15/17 with an explicitly known empty source stop
vector. It is source-qualified, but it is not yet implementable solely inside
the editor files: the shared runtime must first represent the actual source
stop-data owner and the returned vector value. Returning `true` from an editor
`emptyStops` flag would omit the original list construction and ownership effects.

Owner: `editor300`. This report is the only write in this task. Candidate48
editor source remains frozen; no build, original execution, GUI control, or
mutable REA was used. Token measurement is unavailable.

## Evidence and qualifications

The original is `evidence/original-binaries/QtGui4_64.dll`, SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

- `analysis/resume48-editor-next.json`, SHA-256
  `a1cea2207375b91912c2edd81d0f634b4e2a013a17553143d48e82b2acf086a5`, retains
  immutable PE captures for `QGradient::stops6511b390`, QColor global-color
  construction `65120fc0`, and `setRgb6511ed10`, including tool/binary hashes,
  bounds, raw disassembly, and literal reads. Unwind regions are not assumed to
  cover exception funclets or an entire logical function.
- `analysis/resume48-gradient-state-contract.md`, SHA-256
  `9f17ba559fab76b54b079ecbfa7dc22ca828dd9f0344fe6b5aca6de6f7871fba`, adds
  independently checked vector copy/detach/append and ref/deref/free evidence.
  Its proposed state is a contract, not implemented or runtime-tested state.
- `analysis/wave3-editor-parentless-opacity-6511c090.asm` establishes the ordered
  stop-alpha scan and release on both outcomes. The QColor alpha leaf
  `analysis/wave3-editor-wrapper-palette-6511e6d0.asm` reads exactly color byte5.
- Probe004 observed a solid brush only. It supplies no gradient observations,
  source stop alias, count, flags, or empty-list default for production.

## Exact empty-source branch

The existing editor cursor has already consumed brush alpha before its guarded
style dispatch. For style15/17, `6511aba0` takes the style-not16 false return;
`6511c201` then passes the embedded gradient at brushData+70 to `stops`.

`6511b3b0..b3b8` reads the source stop owner at gradient+8 and its signed count
at owner+8. **Exactly zero** selects default construction. Unknown count cannot
be called empty, and an unobserved negative count must not be normalized to zero.
The zero branch does not consume the source capacity, source flags, source
payload, or a retained source-vector reference. Those inputs may remain unknown;
the default local value's own flags and references are the reached dependencies.

The default path uses a new local list value. It calls append `6511ae60` first
with position0 and GlobalColor2, then with position1 and GlobalColor3. Literal
reads establish `FF000000` and `FFFFFFFF`; the color constructor passes alpha255
to `6511ed10`, which writes alpha255*257 =65535 to the QColor word at+4. The
consumed byte at+5 is therefore255 for both stops. No RGB interpretation or
padding initialization is needed to establish those alpha values.

The returned value retains the constructed owner, applies the source flags-bit0
share/copy rule, and releases the intermediate local value. The source gradient's
pointer/count stays unchanged. `isOpaque` scans returned stops in order and
releases that returned value before returning true. This allocation/copy/release
sequence precedes editor opaque-bit comparison and the native-opacity helper.

## Smallest implementation and ownership split

The shared owner should first supply a tested gradient-stop value helper using
the existing runtime-interned brush. Preserve separate source and returned
vector-value references; the runtime alias registry is not a native Qt reference.
The gradient-state contract specifies count/capacity/flags bits0+1 and ordered
position bits plus QColor14 meaningful bytes, without pair padding. An editor
alpha array or an observed opacity boolean is not an equivalent representation.

After that helper is available, the editor-owned increment can:

1. Enter a `GradientStops` cursor only for style15/17, after the retained brush
   alpha read. Continue using the same actual brush owner.
2. Suspend at missing gradient/source/count input. For a known zero count, call
   the shared helper and retain its actual returned value/cursor through pending
   allocation or copy dependencies. Do not replace the original empty source.
3. Scan and release the returned value, then reuse the existing opacity-decision
   and native-helper cursor. Keep16/24 and public6504abb0 guarded.

The same helper can later admit nonempty shareable and nonshareable vectors;
those branches need explicit stop payload/flags and source-preserving copy
semantics. Starting with known empty data is a smaller editor admission, not
permission to omit the helper's vector mechanics.

Focused checks should cover both15/17 and brush alpha0/255; missing source/count;
unknown versus exactly zero count; source alias/count unchanged; returned
positions0/1 with alpha255; intermediate and returned reference release; equal
and changed opacity; raw120 native-helper suspension; and no replay of alpha,
list construction, scan, or release on retry. The public constructor must remain
refused. These are proposed checks, not executed tests.
Include zero source count with unknown source capacity/flags/payload, so the
shared helper does not add reads that the original default branch skips.

## Settings-fill alternative

A bounded source review compared retained General `140eb9630` and StepCount
`140ebb110` with current `fillGeneral`, `fillStepCount`, setup handoff, and
`resume46-editor-settings-audit.md`. It did not identify a smaller proven,
unimplemented value assignment within the editor-owned files. The snapshot
helpers already cover title/name clipping, MO mesh-number handling, process
flags, heat-method initial selection, and StepCount overrides under explicit
profiles. This is not a complete settings audit or a parity claim.

Adding settings-fill controls directly now would require binding actual native
control construction, callback/translator state and effective parent enablement.
The existing page-state projection cannot certify those native effects. The
first-grid cursor also still stops at layout construction. No GUI capture is
needed for the shared static gradient helper; that is the more precisely bounded
next implementation dependency.

The `gradient_state` worker confirmed its current implementation assignment is
style24 image metadata only; it is not implementing the shared gradient API.
The orchestrator must assign that distinct shared write set before editor
gradient admission. Candidate48 `editor_base.cpp` remains SHA-256
`7ee9165d16dd0351024bead83e219c9a4c9fb1590a1bdbc955033a39c190f2bd`.
