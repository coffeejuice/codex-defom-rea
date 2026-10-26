# Platform opacity branch: HWND owner remains unqualified

Preserve OpacityPlatform/NativeOpacityUnrecovered. The native style decision is
closed statically, but its actual receiver HWND and platform Get/Set return/effect
state are not represented or qualified. No source/test change is justified by
this evidence. In particular, do not substitute component identity, a display-DC
alias or a synthetic/null handle and then claim the native call returned.

Owner: editor300. This report is the only write. Existing source, bounded native
captures and direct original PE imports/bytes were read; no source/build/CTest,
original execution, GUI or mutable REA. No tests were authored/run. Token usage
is unavailable. CP67 source hashes are unchanged and remain frozen.

## Exact native receiver and decision

Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Read wave3-editor-parentless-opacity-65097940.asm, retained constructor/init
captures and resume67-editor-opacity-tail.md. The native code receives the SAME
QWidgetPrivate as the caller that already compared/stored private+1d8 bit17:

-6509794a: QWidget q_ptr = [private+8].
-65097951: QWidgetData = [QWidget+20], established as private+148 by init.
-65097955: test windowFlags bit0; false returns.
-65097963: query SAME QWidget's raw120; false returns.
-6509796c: test private+154/windowFlags bit11; clear returns.
-65097976: read SAME private+1d8 opaque bit17.

Only the final set-bit branch is under investigation here. Its decision table:

| Opaque bit17 | Module qword659cae68 | Native path |
|---|---|---|
| true | Not read | Clear OS style bit19 |
| false | zero | Clear OS style bit19 |
| false | nonzero | Set OS style bit19 |

For the completed selected-null texture, the actual finish result is true and
bit17 was committed before this helper. Thus its selected platform path is
CLEAR, and659cae68 is not a prerequisite on this path. That separate module global
must not be incorrectly cited as the first missing input for this specific case.
The nonopaque alternative would additionally require its actual runtime value;
its physical meaning is not invented here.

Both style paths do the following:

1. Reload SAME widget's data pointer at6509798a or650979a5.
2. Read actual native HWND from data+0 into saved RBX at65097993 or650979ae.
3. Pass that saved handle and index-20 to imported GetWindowLongW at65097999 or
   650979b4. Its32-bit EAX return is the OLD style input.
4. Set or clear EAX bit19 (mask00080000), preserving every other low32 bit.
5. Pass SAME saved HWND, SAME index-20 and new32-bit style to SetWindowLongW at
  650979c9. No widget-data handle reread occurs between these calls.
6. Return normally through650979cf..d9; Set's result is not locally checked.

Direct PE import resolution confirms:

| IAT address | DLL | Symbol |
|---|---|---|
| 65699ac0 | USER32.dll | GetWindowLongW |
| 65699ac8 | USER32.dll | SetWindowLongW |

This is a32-bit LONG style operation, not a pointer-valued replacement API or a
copy of Qt's windowFlags word. The bit numbers are different domains: raw120 is
a Qt attribute, bit11 is a Qt flags eligibility test, bit17 is Qt private opacity,
and bit19 is modified in the imported OS style return. None supplies an HWND.

## Failure and partial-effect order already established

The original has no local null-HWND early return before either import. A known
null handle would STILL reach those calls; it cannot be collapsed into helper
success by this reconstruction. Source initialization at6506e569 stores native
data+0=0 before later native creation. That is not evidence for the hypothetical
raw120=true/bit11-set platform call's current handle, API return or failure state.

There is no branch on Get's zero return, no GetLastError query, no branch on Set's
return and no rollback. The native caller always computes the bit operation from
the returned EAX and attempts Set when Get returns normally. Zero cannot be
reclassified as an automatic C++ exception or safe skip. Future reconstruction
must preserve this ordering even for qualified failing API outcomes, without
adding safer error propagation or transactional restoration. Actual OS side effects
and failure/last-error behavior are not inferred from these instructions alone.

Widget opaque bit17 has ALREADY changed before any of these platform effects.
That store must remain committed while the branch is guarded. OpacityPlatform
also retains the already consumed raw120/bit11 path; later earlier-field changes
must not restart those reads and bypass the reached platform continuation.

## Exact missing owner and present evidence limits

WidgetState currently models component/runtime identity, attributes, flags,
geometry, font/palette/locale, private opacity and staged constructor state. It
has no canonical per-widget HWND owner, native-window lifetime/generation registry,
GetWindowLong result state or SetWindowLong completion/failure effects. Source
search found no Get/Set implementation or HWND binding in the engine. Existing
splitter comments about a source-proven fresh handle0 are separate qualified
branches, not an editor platform-handle owner.

WidgetState::nativeIdentity() identifies the reconstructed component; it is not
an OS HWND. The displayDcAlias used by font/DPI work belongs to another native
resource domain and cannot be reused. PaintDeviceCleanupGlobals identifies the
QtGui cleanup symbol/counter, not a native window or OS style. Reusing any of
those numbers would create a synthetic handle rather than prove this receiver.

Retained probe004 palette contract explicitly observed raw120=false at Record23,
closing the helper before native-window calls. It does not capture this set-bit
path's HWND, Get result or Set effect. The current admitted constructor adjusts
flags to0800f001 (bit11 clear), and current tests that set raw120=true therefore
exercise CP67's clear-bit early return. No real bit11-set transition or original
active-null UI reachability is established. Do not fabricate a flags setter or
public constructor profile to manufacture such a call.

The FIRST missing execution edge for the selected opaque=true platform path is
therefore an actual receiver-scoped HWND owner for the read at650979ae, followed
by source-qualified platform read/write outcomes on that SAME saved owner. The
bit17/clear-style arithmetic alone is insufficient to implement the requested
platform branch. No source prefix or import effect was added in this increment;
the existing guard remains before65097976's next read.

## Future bounded contract, once evidence exists

A later assignment should first bind actual native window identity to the same
qualified live WidgetState/runtime, including process/window lifetime domain so
recycled numeric handles cannot be silently substituted. It needs an actual
QWidgetData+0 observation or recovered native-window creation ownership path.
The receiver's component pointer, title/window search result or alias coincidence
is not that proof. Unknown handle, explicitly null handle and a known actual owner
must stay distinct; no default nonnull owner may be created.

Only after that join can an isolated platform cursor preserve:

- Captured actual private opaque result and only the conditional659cae68 read;
- Exact HWND owner read once, borrowed without inventing ownership transfer;
- One GetWindowLongW result receipt and computed32-bit style bits;
- One SetWindowLongW attempt on SAME saved owner, including its real outcome;
- Normal return only after that call completes, keeping the previously committed
  widget opacity and reached import effects through a pending guard.

Tests then need actual qualified successful/failing return profiles, all untouched
style bits including bit31, no null-handle shortcut, conditional global-read order,
no rerun of Get after Set is pending, exact current receiver/owner identity and
lifetime/rebind rejection. Original results are not parameters named
platformOpacitySucceeded. No such fixture/backdoor or new test is introduced now.
Future no-GUI/headless platform modeling still requires source/evidence-qualified
native ownership/effects; this report does not authorize calls against user windows.

## Byte/source receipts and unchanged guard

Selected native ranges, end-exclusive, hashed directly from pinned original PE:

| Range | SHA-256 |
|---|---|
| 65097976..650979cf, style decision/import calls | 939c32ee027e9318aa53df329df8ed36bc2e0af476b11ed014b702eb093d6f6d |
| 6506e554..6506e57d, widget-data/initial handle/flags stores | dc53d91b0932efc2b3c50248d2aa9c80dfd18e172bae82f1629a1f47a8ad914a |

Current unchanged CP67 source hashes:

| File under source/apps/moproj2mst/ | SHA-256 |
|---|---|
| include/m2m/engine/editor_base.hpp | 73510e2d3cd0cd6dc2f87505c633c4683afb4ba9d323903d721cbbe6a2a1a66b |
| src/engine/editor_base.cpp | 20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817 |
| tests/mdt/test_editor_base.cpp | 998751ffdccb5fc582ba626f993c9d0a3b62bc74f8c38172443d9150011b484a |

The existing editor target remains73 cases; no new test result is claimed.
Preserve OpacityPlatform/NativeOpacityUnrecovered, public private-palette
PrivatePaletteUnavailable/not_ported6504abb0 and later QWidget/application-wrapper
refusals. The missing owner applies to this unsupported alternative; its necessity for the
selected fresh constructor has not been established because current flags take
the qualified clear-bit return. It supplies no permission to widen mode300 or
claim full fresh Open-Save-Close parity.
