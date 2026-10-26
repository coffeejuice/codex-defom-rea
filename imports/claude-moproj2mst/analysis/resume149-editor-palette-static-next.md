# CP149: what the selected palette return cannot prove

No source-ready actual editor constructor or Save increment was found. This pass narrows the unresolved branch to an exact raw32 condition and checks whether the retained **post-constructor** palette can resolve it. It cannot: both branches merge into the same destination-word/ref tail. Preserve the selected Session's palette and public Save guards. Do not repeat CP146's query inventory or add another isolated owner helper.

Only this report was written. Work was limited to current candidate source, retained immutable native records and a bounded local DUMPBIN read. No source/test/CMake edit, build/test, GUI/app control, original execution, REA session operation or mutable REA access occurred. The GUI release gate was respected. The original executable is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected.

## The common tail erases branch evidence from the wrapper

The complete default constructor `[6504abb0,6504ac26)` was read with DUMPBIN14.44.35228.0 against the retained original QtGui4_64.dll, SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. This was file disassembly, not target execution. The narrow branch/tail is:

| Native instructions | Established effect |
| --- | --- |
| abcb | Store getter data D in destination P. |
| abce / abd4 / abd6 | Call imported `QBasicAtomicInt::deref`; test AL; JNE ac0d. |
| abd8..ac08 | On false, load temporary D; if nonnull invoke the vector destructor for60 elements, stride8, QBrush destructor6511cbc0, then delete D; restore RDI. |
| ac0d | **Both paths** load RCX from the already-stored `*P`. |
| ac10 | **Both paths** write uint32[P+8]=0. |
| ac17..ac25 | **Both paths** call imported ref on that pointer, ignore the returned bool, and return P. |

The tail does not clear P, replace it with a fresh allocation or return a branch flag. In particular, the zero branch's native sequence proceeds from deletion toward the same pointer-based retain; a reconstruction must not turn that into a safe replacement or infer that the deletion branch was impossible because execution later returned. This report does not execute or endorse that hypothetical invalid-lifetime profile.

Consequently editor004's private-return `{object,data,word8=0}` is compatible with the common tail's stores but is not evidence of the earlier AL result. A normal return, unchanged pointer, word0, later font construction or later palette wrapper read does not prove the nonzero-deref branch. Observing destination word0 earlier would also be wrong: its native store occurs after the possible destruction branch.

## Exact arithmetic domain, without a fabricated count

Let N be the **actual input of the native deref atomic**, not a nearby unsynchronized sample. QtCore67001ac0 uses raw32 decrement/SETNE. Its destruction predicate is exactly `N == 1`. It is not a positive-ref invariant or a host-shared-count test:

| N at deref | Raw result | Branch | Nonzero-tail destination ref, if otherwise qualified |
| --- | --- | --- | --- |
| 0 | ffffffff | nonzero | wraps to0; returned false is ignored |
| 1 | 0 | destruction | do not skip cleanup or claim a live destination |
| 2 | 1 | nonzero | becomes2 |
| ffffffff | fffffffe | nonzero | becomesffffffff |

For a same-owner, interference-free sequence, getter+1 followed by temporary-1 cancels and the later destination+1 produces the final net+1. This algebra is **conditional**, not evidence that source ref bits were initially positive, that they did not wrap, or that no other mutation occurred. The native ref caller itself does not reject a false return.

An actual AL receipt can therefore resolve the branch with less information than an exact ref count: AL=false establishes N=1; AL=true establishes only N!=1. It cannot justify seeding the candidate count with2 or reconstructing a precise current raw word. The current PaletteNativeRefState deliberately requires a known current word before releasing its held token. If only AL were available, an outcome-only/symbolic model would need a separately reviewed contract; do not force that receipt through `observeCurrent` with an invented value. Such a model alone would still not join the selected source owner.

## Current owner frontier has not moved

The current `editor_base.cpp` still has the historical complete-value projection and `PrivatePaletteUnavailable` guard. The selected factory supplies neither a qualified post-Open default palette nor an actual application getter token. `splitview_placeholder.cpp` retains the distinct SplitView partial pointer/token and refuses before temporary release, even when an isolated ref primitive has known bits. These are separate receivers and native occurrences.

The current ConversionSession differs from the CP142 archive only in process-codec-owner constructor plumbing. Its selected-editor factory/qualification/expected palette issue is unchanged. This provides no application palette slot, selected data alias, getter token or atomic input. CP147's process string owner cannot substitute for QtGui's palette-data owner.

`palette_native_ref.cpp` already supplies raw32 wrap, once-only held/released tokens and committed destruction-pending. Unknown current bits leave a temporary held. A committed zero leaves it released with destruction pending; host abandonment has no native decrement. Reimplementing this primitive or adding a symbolic app_pal registration would leave the actual selected factory at the same guard.

No new selected capture was introduced by this source-only codec work. CP143's selected editor004 capture remains the relevant bounded occurrence: twelve selected-D references, all wrapper snapshots. Its data pointer is a historical observation label, not the canonical control block in a candidate Session. Neither Splitter003's different-process data refs nor current C++ ownership supplies the missing join.

## Exact missing witness and next decision

The irreducible runtime witness has two independent parts:

1. **Selected data provenance:** actual selected Project/Operation/editor/private invocation, its getter-selected source O, output temporary T and destination P share the same D/allocation generation. Map that occurrence to the actual Session/runtime canonical data owner; do not use startup aliases or a different placeholder receiver.
2. **Consumed branch input/outcome:** either an actual native temporary-deref AL result for that same frame/D, or the actual atomic input N with supported execution-exclusion/mutation ordering. A sampled pre-call word without ordering and a post-constructor wrapper cannot settle it. To use the existing numeric primitive, additionally establish current-sequence raw bits rather than merely N!=1; a late word observation already includes getter retain and must not replay it.

This is a witness requirement, not another REA query plan or permission to acquire a process. Static disassembly has now closed the branch and convergence facts. Further static callers/default-value searches cannot select an occurrence's AL or canonical allocation generation.

After that witness, the smallest useful actual integration is still one retained getter result/token and one destination partial pointer store on EditorBase, with coherent Session pending-stage admission. It must stop at unknown refs or destruction pending. Nonzero can authorize only the native word0/destination-ref tail after its own input qualification. Font/thread/counter/setup ownership and actual Save delivery remain later dependencies; no `PaletteConstructed` or public Open→Save claim follows from this arithmetic.

No source write set is requested for CP149. Keep public6504abb0 and140eb5180 refusals and the private-page completion qualification.

## Static receipt

Half-open original QtGui range hashes recomputed from immutable bytes:

| Range | SHA-256 |
| --- | --- |
| 6504abd4..6504abd8, test/JNE | `96f8c5f339fa97db0972093835674028c3ebb333eee47dacd982413e40b55780` |
| 6504abd8..6504ac0d, destruction leg | `758d5c1faea2505977543c2edafd81bc611814a80580c0456543e3e319a6b8d1` |
| 6504ac0d..6504ac26, common tail | `a7a68a62747a91342eb79c50ae4404b008c931e99243c783d5b4f251a4cf4420` |

Retained import map confirms IAT65696978=`?deref@QBasicAtomicInt@@QEAA_NXZ`, IAT656968c0=`?ref@QBasicAtomicInt@@QEAA_NXZ`. The existing tail static receipt is `wave3-closure-next-close-splitter-palette-default-tail-static.json`; no new raw capture file was written.

Read-time candidate SHA-256 under source/apps/moproj2mst/:

- src/engine/editor_base.cpp: `20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817`.
- src/engine/conversion_session.cpp: `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5`.
- src/engine/palette_native_ref.cpp: `188ef36821488a00a31956b44642a59476ae6a5b68eae1f97e8e1e5098be64c5`.
- src/engine/splitview_placeholder.cpp: `560c4fbd58c6d5fe15447d18929b0e276121cf0e19bf56c3bb2dc9f627038128`.

This decision adds the common-tail/post-return ambiguity and exact N==1 predicate to CP143's owner frontier; it supplies no new native runtime values or candidate test result.
