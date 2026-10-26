# Native-opacity early return after completed image finish

Source is frozen for root grouped validation. The next source-qualified branch
is winUpdateIsOpaque's raw120=true AND window-flags bit11 clear return. It uses
the existing actual windowFlags_ field; no new runtime observation or synthetic
flag input is needed. The changed opaque bit remains committed before this call,
while unknown inputs and the later platform branch remain guarded.

Owner: editor300. Only editor_base.hpp/.cpp, test_editor_base.cpp and this report
were edited. No other source, CMake/plan, build/CTest, original execution, GUI or
mutable REA changes. Tests were authored/migrated and statically checked only;
no compiler/test result is claimed. Token usage is unavailable. Source is held
frozen pending root release.

## Receiver and live field proof

Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Read retained bounded captures:

- wave3-closure-next-6505e780.asm (QWidgetPrivate constructor);
- wave3-closure-next-6506e4d0.asm (QWidgetPrivate::init);
- wave3-closure-next-6505c8b0.asm (parentless flag adjustment);
- wave3-editor-parentless-opacity-65097940.asm (winUpdateIsOpaque);
- CP66 image-finish/caller-order qualification.

PE exports identify6506e4d0 as QWidgetPrivate::init and65097940 as
QWidgetPrivate::winUpdateIsOpaque. The exact owner/field chain is:

1. init saves its actual QWidgetPrivate receiver in RDI and reads the SAME
   QWidget q_ptr from private+8 into RSI at6506e4f9.
2.6506e554 computes private+148;6506e55b assigns that to QWidget+20.
  6506e577 stores the incoming window flags at private+154. Consequently
   QWidgetData+0c equals QWidgetPrivate+154, the current windowFlags_ projection.
3. Parentless6506e6d6 passes THAT flags address to6505c8b0. For the admitted
   fixed flags0/no-parent constructor profile, it chooses type1 and ORs0800f000,
   producing0800f001. Bit11(mask0800) is clear. This is the existing source-derived
   WindowFlagsAdjusted transition, not a startup value or a new assumption.
4.6506e74d commits changed opaque bit17, then6506e753 passes SAME private RDI
   as RCX to winUpdateIsOpaque at6506e756.
5.6509794a reads its private receiver's q_ptr;65097951 reads that widget's
   data pointer;65097955 tests window status from data+0c. After raw120's getter,
  6509796c separately tests bit11 in private+154: the SAME live flags field.

The implementation reads live windowFlags_, rather than constant-folding a
result from0800f001. The constructor qualification explains why this narrow
return is reachable in current bounded tests. No Qt enum name is assigned to
bit11 without separate source evidence.

## Native branch order and retained guards

winUpdateIsOpaque executes:

`window check -> raw120 attribute read -> if true, flags bit11 read -> clear bit
returns at650979cf; set bit continues at65097976`.

Known raw120=false returns before the bit11 test. Nonwindow returns before the
raw120 getter. Unknown raw120 cannot skip its reached qualification just because
both potential routes might ultimately return under a clear later flag.

For a set bit11, the native continuation next reads private+1d8 bit17. Opaque=true
jumps to650979a5 and uses the real HWND from QWidgetData+0; nonopaque has an
additional global659cae68 condition. The retained IAT map resolves65699ac0 to
GetWindowLongW and65699ac8 to SetWindowLongW; index is-20, with style bit19
set/cleared on the native routes. No HWND/profile, getter result/failure or platform
write behavior is newly qualified here. Keep this continuation unsupported.

## Implementation

OpacityDecision remains unchanged: finish result is published only after actual
I1 destruction, palette opacity is compared once, changed bit17 is stored, then
OpacityEffect begins the actual helper. Equal opaque bit still skips this helper
and all new reads.

OpacityEffect validates known live flags, checks window status, then records the
known raw120 read once as NativeOpacityRaw120Read. Known false/nonwindow returns
through existing NativeOpacityHelperReturned and SharedTail. Known true advances
to new OpacityWindowFlags stage, which records the actual bit11 read once as
NativeOpacityFlag11Read. Clear returns normally; set retains new OpacityPlatform
with NativeOpacityUnrecovered. That stage is terminal for this increment, so a
later earlier-field change cannot re-read raw120 or bypass its already consumed
platform continuation. Unknown flags/raw120 retain their qualification guard.

Diagnostic optional values nativeOpacityRaw120Read()/nativeOpacityFlag11Read()
record consumed read results, not mutable source observations. They remain absent
when native control flow skipped the corresponding read. NativeOpacityHelperReturned
is recorded once before SharedTail. No renderer handle, OS call, new field setter
or native success flag was introduced.

The former guard for every raw120=true was conservative. Existing true/clear
profiles now reach the normal early return. This is an evidence-backed narrowing
of that guard, not authorization to execute the set-bit platform path. The actual
raw120=false default and all public/private-palette refusals remain unchanged.

## Focused cases and source checks

Target m2m.mdt.editor_base now has73 Release-active cases,809 CHECK and72 REQUIRE
conditions. Four prior raw120=true cases (ordinary texture, gradient, solid and
completed active-null texture) were migrated from the old overguard to the
source-qualified true/clear return. They assert:

- Image/gradient values already released/destroyed before native-helper return;
- Actual raw120=true receipt, bit11=false receipt and exactly one of each read;
- Changed bit17 precedes raw120, which precedes bit11, then helper return;
- Source/producer/consumer/global effects do not replay on SharedTail retries;
- Changing raw120 after helper return does not rewrite historical read receipts.

The unchanged-nonopaque radial case now explicitly asserts no raw120 or bit11
read, matching the caller's equal-bit skip. One new known-false case proves
raw120=false skips bit11 entirely, returns once and retains its false historical
receipt even if the live attribute later becomes true.

No test-only window-flags setter or fabricated set-bit constructor profile was
added. Current fixed constructor flags are clear, so set-bit platform and unknown
field guards are preserved and independently source-audited, not claimed as
executed coverage. Existing standalone helper/source lifecycle tests remain
unchanged. Static checks passed for whitespace/conflict markers, balanced
delimiters after comment/string stripping,73 unique cases, one main, no assert
and CHECK/REQUIRE argument scans. Setup remains Release-active through Testkit.
Root owns grouped build/test/archive validation; no build/CTest ran here.

## Native and source receipts

Native ranges below are end-exclusive, hashed independently from original PE
bytes; retained disassembly above contains them. No new raw capture was needed.

| Native interval | SHA-256 |
|---|---|
| 6505e8b7..6505e8c6, embedded data flags location | 71b8ca4b46012892879251a5cfc299d9885b50c992718dc00c5e2c527a7ebdf5 |
| 6506e554..6506e57d, widget data pointer/flags store | dc53d91b0932efc2b3c50248d2aa9c80dfd18e172bae82f1629a1f47a8ad914a |
| 6505c8b0..6505c970, flag adjustment through RET | 8ae18f28d9b074f790486c6496aa4761d26d8b52cb6f2398bf8a87a84784f484 |
| 65097940..650979da, native-opacity helper through RET | ff1c67468e44846bcd917cb85d59bfd1a7242b0fdeecf10a973ff606130d3fef |

Frozen paths under source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/editor_base.hpp | 73510e2d3cd0cd6dc2f87505c633c4683afb4ba9d323903d721cbbe6a2a1a66b |
| src/engine/editor_base.cpp | 20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817 |
| tests/mdt/test_editor_base.cpp | 998751ffdccb5fc582ba626f993c9d0a3b62bc74f8c38172443d9150011b484a |

Next dependency is root validation and the independent Widget tail/application
wrapper/private-value bridge. A future set-bit native-opacity branch needs
actual live HWND ownership and source-qualified Get/SetWindowLong effects/failure
handling; no such call is made now. Public Session PrivatePaletteUnavailable,
not_ported6504abb0 and later wrapper/setup refusal remain. This internal branch
establishes no new actual UI/source reachability or full Open-Save-Close parity.
