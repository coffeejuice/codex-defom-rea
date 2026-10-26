# CP211: outer loader typed-child cast and candidate class mapping

The outer loader casts `SfModelBase*` to `SfMoModel*`. Exact PE RTTI and constructor bytes justify mapping the candidate's existing immutable `DbHandle::Kind::Mo` to that native class **as a partial class projection**. A separate candidate typed-child gate can therefore be integrated before Project initialization without waiting for a runtime witness. This closes a static type-mapping question; it does not establish that the original factory or cast succeeded in the CP207/209 failure occurrence, or implement native construction.

## Evidence boundary

Read-only analysis of copied original DEFORM v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe`, recomputed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred image base `0x140000000`. Only this report was written. No source, test, build, fixture, original process, GUI, deployment, mutable REA database or shared plan was changed. The reverse-engineer-anything workflow was applied to retained qualified evidence and independent section-mapped PE inspection. No native session was opened.

Qualifications read: `AGENTS.md`, `TAKEOVER.md`, `RESEARCH_CHARTER.md`, `admin/import-receipt.json`, bounded current `analysis/takeover-state.json`, CP210 factory-gap/loader-null-static/C++ mapping, CP63 DBLIST constructor, CP114 main-manager constructor lifetime, retained constructor/factory instruction captures. Earlier decompilation names remain interpretations; PE RTTI strings below are actual bytes. New instruction checks used DUMPBIN 14.44.35228.0 on bounded ranges; the trailing partial instruction at `14001d0e9` was not used. Section mapping, descriptors, COLs and hierarchy records were independently decoded with Python standard-library `struct` from the copied PE, with no target execution or external libraries.

## Fixed descriptor and hierarchy proof

The two operands at loader call RVA `1d0b8` are actual MSVC type descriptors:

| Type descriptor VA | File offset | Name at descriptor+0x10 |
| --- | --- | --- |
| `143569448` | `3567448` | `.?AVSfModelBase@@` |
| `143569470` | `3567470` | `.?AVSfMoModel@@` |

Both descriptors begin with qword `142f03398` and a zero spare qword. The call instructions pass RCX=the same returned child, EDX=0, R8=`143569448`, R9=`143569470`, and the fifth argument `[rsp+20]=0`. The return at `1d0bd` is stored separately at `[rsp+48]`; zero branches to `1d3e7` at `1d0c8`. Only nonzero is passed to `44a90` at `1d0d3`. This is the pointer-style RTTI downcast shape, distinct from the prior child-null check and subsequent Project getter/construction-null check. The descriptor identity does not depend on guessing class names from port comments.

Mo constructor `140043fe0` first calls `140cda460` on the same allocation, then installs primary vftable `142112878` at `14004400c..140044013`, calls `140044090`, and returns the same allocation at `140044021`. Its vftable[-1] is complete-object locator `1430d5508`:

`signature=1, offset=0, cdOffset=0, typeDescriptorRVA=3569470, hierarchyRVA=30d5530, selfRVA=30d5508`.

The hierarchy at `1430d5530` contains `signature=0, attributes=0, numBaseClasses=3, baseArrayRVA=30d5548`. The three base descriptors decode as:

| Base descriptor VA | Type descriptor | Contained bases | PMD (mdisp,pdisp,vdisp) | Attributes |
| --- | --- | --- | --- | --- |
| `1430d5568` | `143569470` SfMoModel | 2 | `(0,-1,0)` | `40` |
| `1430d5590` | `143569448` SfModelBase | 1 | `(0,-1,0)` | `40` |
| `1430d40b8` | `143565748` QObject | 0 | `(0,-1,0)` | `40` |

The `0x40` metadata bit accompanies a hierarchy reference; none of these descriptors carries an inaccessible/ambiguous-base flag. The hierarchy and PMDs describe a single zero-offset nonvirtual chain. For an intact fully constructed object with this vftable, the selected base-to-Mo conversion has no derived/base address adjustment. No reinterpretation of a Project backlink is involved.

## Factory return class

The native manager vftable `142115f68` has COL `1430d55e8`, identifying `.?AVSfMoModelManager@@` at descriptor `143569420`. Its hierarchy is the corresponding zero-offset chain `SfMoModelManager -> SfModelManager` (`1435693f8`) `-> QObject`. Slot `+0x68` contains raw qword bytes `9054044001000000`, i.e. function `140045490`.

CP114 qualifies the original main-constructor chain and derived manager-table installation at `045449`; this newly decoded table independently confirms the class name. Factory `45490` requests `0x19f0` storage, calls `43fe0(storage,null)` when allocation is nonzero, and returns its constructor result. Thus normal completed construction through this specific manager table produces an `SfMoModel`, returned through the base interface. The expected cast succeeds and preserves pointer identity under that conditional static profile.

This is not a proof that every possible runtime manager uses this table, that intervening calls preserve the vftable, or that the original occurrence reached/completed this factory. `c79390` calls the path setter on the returned child before its own return; its null-allocation case must not be converted into a graceful outer null result. Full QObject/DB-reader construction, path/global-palette behavior and signal effects remain qualified by CP63 and CP114. The new metadata does not erase those dependencies.

## Minimal source-ready seam

Current `engine.hpp` explicitly labels `DbHandle` a partial `SfModelBase/SfMoModel` projection, has immutable `Kind { Base, Mo }`, and stores the Mo Project backlink separately. `prepareProjectModel` creates `Kind::Mo`, adopts its slots, sets the path, assigns/appends the model, registers the two connections, then publishes `p.dataDb`. `commitProjectModelConstruction` checks Kind::Mo only **after** `initProject` in `convert.cpp`; that later invariant is not the loader's earlier cast.

The exact small change justified by this audit is:

1. Add a distinct internal projection operation with pointer semantics, for example `DbHandle* outerLoaderMoChild(DbHandle*) noexcept`: null input returns null; nonnull Base returns null; nonnull Mo returns the identical pointer. Name/comment it as the candidate class projection of `1d0b8`, never native RTTI execution. Its only type premise is the immutable Kind established at candidate construction; it must not inspect `moProject`, loaded/read status, path existence, model ID, current selection or Project slots to decide the cast.
2. Invoke it in `prepareProjectModel` **after** `registerModelSignals` returns and **before** publishing the typed child to `p.dataDb`; this is before `initProject`/`commitProjectModelConstruction`. If a dedicated stage receipt is added by another lane, attach the successful gate to that same child identity at this boundary. Do not move path, ID, append or connection work after the gate.
3. The existing candidate factory always creates Mo, so this call should succeed. An unexpected candidate mismatch is an internal unsupported/invariant result, not a new public emulation of original loader-null failure. Keep the helper's null/Base outcomes testable internally without adding a public factory injector, claiming native failure reachability, or changing allocation-failure handling. Retain the existing post-construction ownership check as a separate condition.

This is sufficient to integrate the *typed-child decision projection* without original runtime observation. It is insufficient to assert that the entire outer-loader factory/cast stage, its native allocation/constructor effects, or the original failure's causal predicate has been reconstructed. In particular, do not expose a boolean named nativeFactorySucceeded based on this gate or use it to admit other unresolved public lifecycle paths.

Useful focused checks should exercise the distinction and ordering, not manufacture an original oracle: a Base child with even a populated candidate Project backlink must still fail the internal projection; a Mo child with a null Project backlink and unread/missing-path state must pass with identical identity; null must have no side effects. A session-level check should observe that registration is complete and Project construction/backlink/selection is still incomplete at the gate, then verify the same child is published and later receives the backlink. Where the test harness supports an internal continuation observation, verify that a failed typed outcome does not run Project initialization; do not add a public failure injection to achieve this. These are candidate contract tests, not runtime evidence. Use actual Testkit `CHECK_EQ`/`REQUIRE` and `testkit::run_all()`; there is no `REQUIRE_EQ`.

## Reproducible compact byte receipt

Ranges are VAs, end-exclusive. SHA-256 is over raw PE bytes in each selected range, not formatted DUMPBIN text.

| Range | File offset | SHA-256 |
| --- | --- | --- |
| `143569448..143569490` descriptors | `3567448` | `0de1b15d2b9b8bad5a47f7183544dcfacb96def55854fa242ee9dad63479206d` |
| `142112870..142112880` Mo COL pointer/table start | `2111070` | `46cb2871ada15d9ca72803d553298e60427629baeba0b8649006c2a3d9c1a1ef` |
| `1430d5508..1430d55e0` Mo RTTI | `30d3d08` | `9246dc6a74323529c4104ad2c4a85ee00eb6fae206bb392a27c3fa922bb4123e` |
| `142115f60..142115fd8` manager COL pointer through factory slot | `2114760` | `0ae9bef8c5a30cb3845ab3d8a4b5ae62b2eb1fa3a4b00dcae1c01b958b724985` |
| `1430d55e8..1430d56c0` manager RTTI | `30d3de8` | `6c0099a3306f1e4f10cd4c227c1971c8b372c977ec6780349a5ea6619c8b1794` |
| `14001d085..14001d0e9` factory/cast/Project call sequence | `1c485` | `dc26c95b3c0c7d93de8fee9e9ec7ea40a17929bff3a8790e33ae8b12e2bf51a7` |
| `140043fe0..14004402b` Mo constructor | `433e0` | `2e6e292ebfda9eebdd264f6e6c1d119085801fc4f324c9dd04907d25a94a5163` |
| `140045490..1400454ee` concrete child factory | `44890` | `440e9fcb3842e679167ed20deedd09deb22c36069e107e91647828b764efbe75` |

The runtime discriminator remains CP210's same-invocation return observations at `1d08a` and `1d0bd` (plus preceding duplicate lookup and following Project result/acceptance). Static type closure narrows the expected successful profile; it does not identify which predicate caused the captured original outer result to be null.
