# CP91 matched DoeVariable owner and header-only cut

A fresh mode0 manager can read an actual header-only DoeVariable to normal completion without keyword notification, QFileInfo, QWidget or codec consumers. This is an independently recovered object/pointer-list owner, distinct from CP90's copied Constraint value record. Root confirmed CP90 freeze and assigned the bounded DOE source cut after this read-only investigation.

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256 was verified as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded DUMPBIN14.44.35228.0 captures are retained in `resume91-doe-variable-owner-static.json` and `resume91-doe-variable-lifetime2-static.json`; each receipt binds commands, immutable binary hash, unwind bounds and capture hashes. No native execution, GUI, mutable database, build or test occurred. One initial capture stopped at a non-entry0x140919c70 after recording the constructor; its duplicate constructor artifact is historical, not a separate behavior claim. The completed lifetime2 receipt uses the actual destructor entry0x140919cb0.

## Original path and independent identities

CP50's completed `0x14090ee20` reader body is bound by `resume50-doe-variable-body-pe-receipt.json`:15345 bytes, code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08`. Its annotations and retained complete decompile establish exact direct DoeVariable dispatch, name/numSamples attributes, optional varId/dimension/LinkTo, independent direct Description/Keyword/Geometry tests, and final reindex/deferred links. CP91 reread the relevant instructions rather than inferring a reader from Constraint similarity.

The SfMoDoeManager vtable at0x1421671b8 independently reads as +f8=0x140919de0, +130=0x14091d970, +178=0x1400d7f00, +180=0x14091b5d0, +1a0=0x14091b950, +1d8=0x1400d7f20, +1e0=0x14091c7a0, +1e8=0x14091da70. Constructor0x1400d1180 installs that table after base0x140919b80.

For a matched row,0x14090f0ac calls vt+f8(name,numSamples,false). The caller ignores its returned object and separately reads count-1, then vt+1a0 to obtain the last variable at0x14090f0ef. This matters for duplicate names: helper returns an existing object, but following reader setters still target the last variable. A bounded fresh owner avoids this unresolved broader admission, without silently deduplicating.

## Record constructor, append and persistent order

0x140919de0 first calls same-name lookup vt+130. On its new path it requests0x48 bytes, invokes0x140816900(numSamples), then appends the **object pointer** to manager+38 through0x14172d7e0 at0x140919ec7. The unshared append builds an8-byte pointer node and copies it into a QList slot.0x140de9b40's selected branch copies8 bytes; no variable-copy constructor or new0x48 copy allocation is invoked there. This is not CP90's0x28 record-by-value append.

| Native record offset | Constructor state0x140816900 |
| --- | --- |
| +0/+8 | Default QString name and description |
| +10 | numSamples argument |
| +14 | int0 |
| +18 | index-1 |
| +1c | number0 |
| +20 | geometry type100 |
| +24 | dimension3 |
| +28/+30/+38 | Empty geometry/keyword/operation lists |
| +40 | Null link |

All0x48 represented fields have source initialization; no Constraint+10 analogy is used. Actual Qt shared-null/ref ownership is not asserted by the existing String value seam.

After append, helper assigns name through0x140dfbd00. It invokes next-number query vt+1e8 **even when its false flag skips the number setter**. On the one-fresh-variable profile the constructor number0 gives candidate1; no number store follows that false flag. Then name-index lookup vt+180 stores selected index at manager+74, then all three invalid bytes+70..72 become1. Only manager mode2 calls0x14091d110 design acquisition. Mode0 skips it. Base ctor0x140919b80 independently initializes mode0, selectedIndex-1 and all three invalid flags true.

After helper return the reader's positive varId setter0x1409ff590 writes record+1c, and dimension setter0x1400d8040 writes+24 when present. Empty LinkTo queues nothing. A header-only row has no child payload effect. With no links, normal node exhaustion calls reindex vt+1e0 and returns true; the outer reader then continues later siblings and its separate ID finalizer. No SetOprHasDoe call exists without entering the Keyword branch. Description, if later implemented, calls0x14010f200 only for nonempty text and assigns+8, but it is unnecessary for the smallest cut.

## Owner death and exception bounds

Base manager destructor0x140919cb0 traverses its variable pointer list from begin to end, skips null pointers and calls0x14091db70(flag1). That wrapper calls variable destructor0x1408169d0, then raw delete0x1414dfc68. Variable destructor walks/deletes keyword pointers, clears geometry through0x140817d10, then releases operation/keyword/geometry list wrappers and QString description/name. On the admitted empty-list record, none of the keyword/geometry payload deletion or keyword synchronization branch is reached. Manager clear vt+108=0x14091a2e0 likewise deletes pointed records in forward snapshot order, then clears its actual pointer list and invalidates sample modes. This is explicit record ownership; QList pointer-node release alone is not object destruction.

CP91 should store a concrete derived record in a concrete unique_ptr owner on Study and borrow its Var alias in Model.vars. Var has no virtual destructor: never place that derived record in vector<unique_ptr<Var>>. No Study/Session back edge or shared_ptr cycle is needed. Existing retained B121 prefix/state remains distinct.

The host owner must be secured before exposing the borrowed alias. Append to the observable vars list before name/selection/invalid/header writes, retaining earlier mutations if any later host operation fails. Any failed current alias append must remove only its uncommitted host owner. This is host lifetime safety, not proof of native allocator/COW/reference failure parity. No injected native-null allocation or artificial rollback schedule is qualified. Do not add native cleanup/reset semantics to Study beyond concrete host ownership.

## Smallest production cut and write set

Admit one fresh mode0 **header-only** exact direct DoeVariable with nonempty name, all name/numSamples/varId/dimension attributes present, canonical positive int32 samples/ID and exact dimension3; LinkTo absent or empty; no child nodes. These are reconstruction bounds, not native input validation. Refuse preexisting variables/other projection owners/retained prefix and later matched rows; do not substitute a lowest-free-ID or duplicate-name policy. Keep every other profile's existing matched-variable guard before append. Unknown direct children of DoeVariables keep their previous skip behavior.

Proposed exclusive source files, approved by root: new `include/m2m/doe_variable_record.hpp`, new `src/doe/variable_record.cpp`, existing `include/m2m/doe_study.hpp`, existing `src/doe/doe_study.cpp`, new `tests/doe/test_variable_record.cpp`. Expose sampleCount/f14/dimension/description with concrete type; use the inherited Var fields for name/index/number/type/lists/link and actual Study selectedVariableIndex initialized-1. This is a model value owner, not a native0x48 ABI image. No shared Session/parser/writer/CMake/API edit is required.

Focused tests must cover fields/selected state/invalid flags, same concrete borrowed identity, canonical/missing/overflow/mode/link/child guards before append, existing-owner refusal, first record retained before later matched refusal without reindex, later top-level guard retaining successful reindex but skipping outer IDs, and actual Session-owned lazy read/sample continuation. Save on that same owner must still refuse at nonempty-vars dimension reconciliation **before XML open**, unlike CP90's positive-constraint empty-vars path. Project DOM and record identity remain; no refresh/delivery is claimed.

Root owns build/CTest. Expected target `m2m_doe_variable_record`, CTest `m2m.doe.variable_record`. A header-only synthetic fixture proves the bounded production state transition; it does not establish B121 scalar parsing, historical oracle equality, full C ABI Type2 selection or public Save completion. Next dependencies remain Keyword/Geometry/deferred links, native allocation/refcounts, and actual public Save reconciliation/refresh owners.
