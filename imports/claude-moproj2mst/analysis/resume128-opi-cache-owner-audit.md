# CP128: canonical OPI+160 owner audit

A canonical Operation-owned **Unknown-only observation slot** can be added safely without claiming native nullness, provided it is private, exposed only as const, and has no production promotion/default-commit path. Its knowledge remains Unknown across every currently unrepresented callback. This closes a bounded owner contract, not a usable null-cleanup admission. Binding defaults to persistent KnownNull still needs callback/creation invalidation and must remain deferred.

Only this report was written. No source/model.hpp edits, builds/tests, GUI/original execution or mutable REA database. All native claims are static original-image/capture qualifications, not new live witnesses.

## Constructor, defaults and distinct lifecycle events

Immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses use the common140 prefix. Bounded DUMPBIN14.44.35228.0 and PE bytes show:

- SfOperationInfo constructor04fcd0 initializes QObject at04fcf3, engine guard+38 through15d170 at04fd12, lists/strings, then **guard+160 through15d170 at04fdd1**, before calling defaults050550 at04fddc.15d170 writes one qword zero at15d17a and returns; there is no allocation or Qt registration call in that wrapper.
- Defaults explicitly assigns+160 null throughCBC920 at05067b. On a freshly zeroed guard, CBC920's equality branch atCBC93b skips its mutation import. Constructor guard initialization and defaults reset are separate events, even though both yield null on this path.
- The0500b0 copy/reset helper also ends by assigning destination+160 null throughCBC920 at050423. Do not copy a source model identity into a new OPI merely because other fields are copied. Current Operation is noncopyable/nonmovable, so this additional native reset is not a new candidate entry point.
- Target destruction can independently null a registered QPointer. Later OPI payload cleanup reads the then-current pointer at05079e; the+160 member's guard removal at050478 and OPI's incoming-guard expiry at QObject destruction remain distinct. No standalone defaults snapshot proves those later states.

Native constructor initialization is now directly established rather than inferred from the defaults comment. This does **not** justify assigning KnownNull in every arbitrary modern Operation constructor: modern default construction is only a partial projection, and its subsequent admitted effects do not track the native model.

## Concrete creation and callback order

The retained054950 capture closes the local order more precisely than the current source comment:

1.Resolve class at054979; absent class returns. Class virtual+a0 at05499d controls the branch.
2.On true,0549b3 obtains its tree helper and0549ca calls0c3980 with this OPI **before reading+160**.
3.Read+160 at0549de. If null, allocate0x50 at0549f1, construct SfMoItemModel at054a08 when allocation is nonnull, then assign the actual guard at054a3b. A null allocation is assigned null; no invented safe error path is established.
4.Only after that assignment does class virtual+a8 at054a74 receive the current guarded model, followed by class virtual+278 at054aa6. Existing-nonnull paths also make later callbacks and repeated guard reads.
5.If virtual+a0 was false,054c09 calls class+a8 with model argument null. That direct body does not allocate+160, but the virtual recipient is still not a general no-mutation proof.

Native0548c0 chooses054950 at054925 when AppState process type is5; otherwise054931 calls054c80. The latter obtains class and engine model, then054cca calls054f40(create=true) before class+b0 at054ceb and notifier02ceae0.054f40(false) is noncreating; true+current-null constructs0c4890 and assigns+160 at054fc1. Therefore both initialization paths matter; a create=false call scan alone cannot bound writers.

The post-load call chain has earlier callback uncertainty:

- Parser08ccf0, after setting the operation flag at08dc99, fetches Props through051230 and calls its **virtual+c0 at08dcc3**, before returning success. Candidate `operation_rescue.cpp:108` calls `item->props->postLoad`. Base Props::postLoad is empty (nativecac130), but derived overrides exist. This audit does not classify every override as a writer or prove all preserve+160.
- Class base loader5ef430 resets the engine model, invokes class virtual+b8/78/1b8/c0 at5ef485/498/4a8/4bb, then calls054350 at5ef4c6.054350 fetches Props and invokes its **virtual+58 at05438e** when nonnull. Only then does5ef4da call0547b0.
-0547b0 first calls display refresh054820 at0547c2, then0548c0 at0547d1, followed by further UI/notifier work. Candidate postLoadViews starts with `displayRefreshScene(p, opModel(...))`, then objectTreeFill/objectTreeAdd. Its current source comment that the tree model “does not exist yet” is a historical first-open assumption, not a persistent lifetime invariant.

Consequently invalidating knowledge after objectTreeFill/Add would be too late. A future KnownNull integration needs a barrier before the first admitted unrepresented callback/creation effect, including parser Props postLoad and class/Props loader callbacks when reached. Invalidating before postLoadViews alone is insufficient. This audit identifies concrete barriers but does not close the exhaustive callback, signal-recipient, inline and indirect writer set. Unknown is an epistemic state; dropping KnownNull at such a barrier must not delete, reset or invent a native pointer change.

## Closed minimal owner contract

Reserve these files only after root assigns source ownership: `source/apps/moproj2mst/include/m2m/model.hpp`, existing `include/m2m/engine/operation_info_model_cache.hpp`, `src/model/operation_info_model_cache.cpp`, and `tests/mdt/test_operation_info_model_cache.cpp` beneath that app directory. No operation_rescue, loader, postLoadViews, cleanup, Session or public Close edit is required for this first increment.

Proposed shape:

1.Operation privately embeds one OperationInfoModelCache (or a narrowly equivalent canonical observation type), initialized with `*this` **after operationInfo_**. OperationInfoIdentity must exist before the cache captures its guard. Existing noncopy/nonmove Operation semantics remain unchanged.
2.Expose a **const-only canonical accessor**. Do not expose a mutable reference, public setter, friend-to-arbitrary-caller promotion, receipt-forwarding method, or constructor that lets diagnostics replace the member. Existing standalone cache diagnostic constructors/commit API may remain compatible on their separate instances, but cannot mutate the canonical member.
3.The canonical member starts Unknown and remains Unknown throughout ordinary construction, initOperation/operationDefaults, Open, Save and partial cleanup. Do not wire commitNativeDefaultNull yet. Future source-owned events can add a separately reviewed promotion API; a nonblank string is not such authority.
4.Repeated reads name the same member and same OPI generation. Lookup still validates live identity and refuses Unknown. A separately committed KnownNull diagnostic for the same Operation cannot substitute for the canonical slot or advance cleanup. No native/model/selection allocation is added; a guard copy and empty metadata replace neither the actual QObject nor its physical guard layout.
5.No implicit member destructor may be called native+160 guard removal. The cache contains observation metadata only. Declare order so fallback OperationInfoIdentity expiry still occurs after represented members; any returned reference is bounded by its Operation lifetime. Escaped diagnostics must use a guard snapshot, not dereference a destroyed member.

This Unknown-only contract is closed under unrepresented callback effects because it makes no pointer-null/live assertion before or after them. It is useful as the single production-owned observation boundary, but must not be advertised as completed+160 construction, native destructor progress, or a Close success. A mutable canonical accessor using the current diagnostic commit API would invalidate this conclusion.

## Meaningful focused tests

Extend m2m_mdt_operation_info_model_cache with: constness of the accessor's result; stable same-member identity across repeated access; canonical Unknown after bare/default and actual initOperation paths; a same-OPI detached KnownNull fixture leaving canonical Unknown and refusing its lookup; foreign/explicitly expired OPI rejection; and a guard snapshot expiring after Operation destruction without retaining target allocation. Preserve existing diagnostic tests/API behavior and noncopy/move static assertions.

Do not add per-Operation make_shared, factory or lazy payload allocation. Check the actual constructor path before claiming allocation neutrality; existing Operation itself already allocates identity/recovery metadata. Use a bounded allocation probe if needed to distinguish those existing allocations from the added observation guard copy, without claiming that error formatting is allocation-free. Tests should retain Release-active Testkit checks/main/includes. No tests ran here.

Canonical KnownNull/default-event binding and the0507a6 cleanup skip remain a **later** task. They require a source-owned event API plus closed invalidation/guard coverage, or an actual current-value producer at the point of use. Public Close and all existing cleanup boundaries remain guarded.

## Reproducibility

End-exclusive original byte hashes:

| Range | SHA-256 |
| --- | --- |
|14004fcd0..14004fe0b|`8fd3a109eea0a9d60e475c1a6468caad8b961249aa2356484f75da4c2f2830fa`|
|14015d170..14015d187|`240d5de2be7e3366cee23fbe7271de4a8845db7a190188908f6a2e6570faf0ab`|
|140054950..140054c17|`4e73e67993a8faa93eeee5a997004a1f8b42e94ad1f62b9bf4755bdb5fedfdff`|
|1400548c0..14005493b|`5eeada4f399bf5c953b5cf330f6e9d293d1d96c58ee41e955596454e0db9f1e5`|
|1405ef430..1405ef4ee|`cf0d90a0b817aabe772a3ce68bb1a5157b952078542eb30108a931dd410662f8`|
|140054350..140054396|`c36fd14484fac7b9f3d2991a6e05afb3c1048e88d8b3915414d94f12ce162afa`|
|14008dc90..14008dcc9|`de40b63bb2ab85f300e6b0a6877bbddebb501f44abd7e8bcdf08d0cb3ce056b4`|

Retained054950/054c80/054f40 ASM hashes respectively: `7a5e356914f4561c813ee8d001dc7c1559ecdeb5207c82c672ec06c43b839d95`, `f8bbe67a841a0999fe09438ad04852da5cc6253e8f1f74daf6f9739a9a9202b7`, `15c617ce9f10ea6dcc9b65fdec22781718468d09e29ee79c5acf961ee694f931`. Current loader.cpp hash `746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c`; props_keys.cpp `aea4667878bcc35e8261ab295f0b981b73907430f912b33251546db10129c1e9`. Earlier CP128 report retains exact getter/default/cleanup hashes and current cache-source hashes. No reconstructed executable/deployment was inspected or changed.
