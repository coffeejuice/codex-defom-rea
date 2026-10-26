# CP80: fresh manager constructor's null equality skip

The smallest integrated successor is the **actual candidate ModelManager constructor's owner-assignment/fresh-current equality-skip prefix**. The original's reset call is not a second clear or unconditional pointer store: its guard constructor already wrote null, and cbc920 compares that null with requested null and returns without calling Qt. This branch is source-closed. It can produce an honest candidate constructor-prefix receipt on the existing Session manager while native QObject construction, concrete Mo class, main+50 publication and public Close remain guarded.

The actual main+50 factory handoff is larger. CP77 statically closes its identities, but its executable producer requires the preceding native constructor chain and actual main ownership. CP78's Open-read receipt cannot supply that factory completion. CP79's parked GuiModel barrier also cannot be retired by a manager-constructor or owner-read receipt.

This report began as read-only design. Root then authorized the exact source lane after CP79 freeze; the implementation receipt below records the resulting three exclusive code/test changes. No build/test execution/GUI/original execution/mutable REA activity occurred. Evidence method: current source, retained CP77/78/79 captures and bounded offline PE/DUMPBIN/import analysis. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## New source closure: constructor reset does no write on this branch

The original manager base constructor c781d0 constructs owning list+18, constructs current guard+20 via15d170, stores owner+10 atc7822c, then callsc782b0 atc78235. Its guard constructor and initializer/reset helper do:

```text
15d17a  [currentGuard] = 0
15d186  return
c782be  rcx = manager+20
c782c2  rdx = 0
c782c4  callcbc920
cbc938  compare [currentGuard], requestedPointer
cbc93b  equal ->cbc94d
cbc947  unequal only: call [141e058b0]
cbc94d  return currentGuard address
```

PE imports identify141e058b0 as `QMetaObject::changeGuard(QObject**,QObject*)`. On the fresh constructor's unchanged default-null guard, the equal branch invokes no changeGuard, stores no pointer and destroys no model. Do not implement that exact branch by calling a general clear, selecting null or issuing a blanket reset. A raw E8 search found c78235 as the only direct c782b0 call candidate in this module; its constructor call is independently disassembled. This is bounded direct-call evidence, not complete indirect coverage.

Native ordering remains list/current construction -> owner assignment -> fresh-current comparison/skip -> derived Mo class045449 -> returned manager -> main+50 store01344c. Neither QObject/list/guard physical construction nor the unequal changeGuard branch is closed by this small prefix. Constructor-default null cannot be reused as a current-value witness after Open/selection or across Close callbacks.

## Actual candidate producer and minimal source task

Current `ModelManager` already belongs to the actual Session. Its constructor initializes owner_ in the initializer list, then obtains the heap lease in the body; C++ list/current members default-construct in declaration order. CP78 privately records that existing manager/backlink at Open without native readiness. Do not allocate a replacement manager on Open or convert that receipt into a factory-store observation.

The proposed implementation is limited to `include/m2m/engine/model_manager.hpp`, `src/engine/model_manager.cpp` and a new `tests/mdt/test_model_manager_constructor_prefix.cpp`, with a separate receipt. No Session, CP78 helper, CP79 engine/viewer, C ABI or public Close edit is necessary: the Session already constructs this actual class, and its existing Open-read receipt resolves the same manager.

In that actual constructor:

1. Keep private C++ storage safely initialized but move the **meaningful owner assignment into the constructor body**, after list/current storage has default-constructed. A private initial owner placeholder must not be exposed as an original native null-owner event. There is no new native QObject/parent/readiness assertion.
2. Assign the supplied candidate owner at the source-corresponding owner-store point. Record only candidate constructor provenance, using an opaque owner token if historical identity is exposed; do not add an escaped raw Session/manager pointer accessor.
3. At the following fresh-current point, verify that the actual private weak-current storage is still its default empty owner/control block, not merely `currentModel()==nullptr` or `expired()`. Those latter predicates can hide a stale nondefault guard. Compare against a default weak owner if needed. The constructor's source profile is exact fresh storage, not a general reset API.
4. For that default-null profile, record the **no-write equality skip**. Do not call current_.reset(), ModelManager::clear(), selectModel(nullptr), GuiModel::reset, viewerClear or any Qt provider. An unexpectedly nondefault fresh guard stops at the unavailable branch rather than being silently cleared.
5. Retain an immutable candidate constructor-prefix result on this actual manager. It can report owner assignment followed by fresh-null equality skip, with native QObject/base/concrete class/factory/main-window/wrapper/Close completion all false. Bind the adapter heap lease after this prefix; it is a separate adapter retention effect.

The record is a producer of actual **candidate** constructor facts. It is not a native constructor cursor or a captured current original object. Do not expose a generic public method that can rewrite the record or let an already populated manager reenter the fresh profile. The C++ constructor must return before a canonical shared owner can be attached; do not construct `shared_ptr(this)` inside it. Existing ModelManager owner/current/list public behavior should remain unchanged.

This source task makes the original constructor branch explicit on the same actual owner used by Session/Open, rather than adding another detached observation. It still stops before045449's native concrete class and01344c's main-field handoff. CP77 observations may later consume a real producer/capture, but the new candidate prefix must not automatically call them to self-certify the missing stages.

## Integration with the CP79 barrier

CP79 parks a lower-version geometry reader on the **actual GuiModel** and guards reset/clear/retry before effects. Current `GuiModel::reset()` calls `requireGeometrySyncAvailable()` before inspecting step or invoking viewerClear. The barrier's lifetime is not the ModelManager's current guard and its completion is not implied by any empty list, constructor-default null or CP78 receipt.

The proposed constructor has no child/GuiModel objects to destroy and calls no geometry path. Preserve CP79's guards untouched. A future nonempty Close must still stop at actual child/model cleanup when that barrier is parked; dropping adapter storage or selecting a different current model is not original reader/reset completion. Diagnostic constructor records remain historical after model selection and cannot authorize barrier retirement.

## Focused test and remaining guard

New focused target: `m2m_mdt_model_manager_constructor_prefix`.

- Construct the actual Session and obtain its actual manager. Inspect candidate owner assignment/fresh-null equality-skip provenance before Open. All native factory/readiness flags remain false; no second manager exists.
- Call actual prepareProjectModel and use CP78's receipt to resolve that same manager. Its constructor result remains historical; ordinary one-child/two-registration/moProject-null/no-current behavior is unchanged.
- Perform Project construction/selection, then demonstrate that the constructor prefix remains recorded without asserting current is still null. Current selection and subsequent adapter clear must not re-run/rewrite the fresh constructor result or become a factory handoff.
- Cover standalone null-owner ModelManager as a separate candidate constructor profile; it does not establish a native null main or wrapper-safe factory failure.
- Preserve ordinary existing manager tests and the CP78 revoked/foreign/opaque-identity cases. Reuse CP79's existing real parked-model regression rather than injecting a private fake barrier into the new constructor test.

The equal native branch performs no externally visible mutation; do not invent a callback/counter solely to make a test observe a fictitious reset effect. Validate actual owner/current behavior and honest immutable provenance. The unexpected nondefault constructor branch need not be forced through an artificial private-state bypass. Root should run the new target with existing `m2m_mdt_model_manager`, `m2m_mdt_session_model_manager_read` and affected CP79 viewer tests when building. Six tests were subsequently implemented as recorded below; none were executed by this worker.

Remaining guard: no native045449 concrete class or01344c main+50 publication is established, so01d510/0133a0 native wrapper remains unqualified. CP78's preexisting rejected-suffix ordering limit also remains: a candidate constructor/read record is not proof the original accepted Open reached its corresponding instruction. Later main+3c0/013d40, fresh status/main-private/cache owners, modal schedules, nonempty destruction and native lifecycle acceptance stay separate.

## Evidence receipt

New bounded disassembly is retained inline above; no raw file was added. DUMPBIN14.44.35228.0 from MSVC14.44.35207 and PE import resolution supplied the branch/provider names. The guard constructor is a decoded leaf with no inspected unwind entry; padding is excluded. Code regions are end exclusive.

| Region | Code SHA-256 |
| --- | --- |
| 14015d170..14015d187 | `240d5de2be7e3366cee23fbe7271de4a8845db7a190188908f6a2e6570faf0ab` |
| 140c782b0..140c782ce | `f41abe6e674a81fb7f030384883440e03a0d0bc04f60efafa1cc66b614c60752` |
| 140cbc920..140cbc957 | `fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8` |

CP77 binds the enclosing base/derived/factory and RTTI hashes. The new evidence narrows the constructor reset to the equality skip; it does not close unequal guard mutation or any transitive native constructor/callback.

## CP80 implementation and release

The actual ModelManager constructor now uses only a private initial owner placeholder until all C++ members have default-constructed. Its body assigns the supplied owner, records an opaque candidate owner token, compares the private weak-current ownership against a default weak owner and records the no-write equality skip. Adapter heap retention follows. The constructor never calls reset/clear/select/currentModel/expired/changeGuard/GuiModel/viewer operations. An unexpected nondefault weak control block refuses instead of being cleared.

`constructorPrefixReceipt()` returns an immutable by-value `ModelManagerConstructorPrefixReceipt`. Only ModelManager constructs/updates its private producer fields. The receipt contains candidate origin, owner-assignment/equality-skip history and opaque integer owner identity; it exposes no historical raw pointer or live-object admission method. Native QObject/constructor/concrete Mo class/main+50/wrapper/readiness/Close flags remain false. Selection and clear do not rewrite that history or interpret it as current-null proof.

Six new Release-active Testkit cases cover standalone null-owner construction; the actual Session-owned manager before Open; the same actual manager through CP78 read/child registration; history after Project selection; ordinary selection/adapter clear preserving the constructor record; and an opaque copied history surviving actual Session/manager destruction without authorizing anything. Existing Session, CP78 helper, CP79 barrier and public Close/ABI behavior were not edited.

Static preflight passed: all quoted includes resolve, Testkit entry point and supported CHECK/REQUIRE/TEST_CASE macros exist, no C assert/NDEBUG checks or trailing whitespace occur, and `git diff --check` returned zero. Comment-stripped constructor inspection confirms owner assignment -> strict default weak ownership comparison -> equality-skip record -> heap lease, with no reset/clear/Qt callback. No build/CTest/test executable was run. Root owns grouped validation.

```text
cmake --build <combined-build-directory> --config Release --target m2m_mdt_model_manager_constructor_prefix
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.model_manager_constructor_prefix$ --output-on-failure
```

Only these three code/test files and this report changed in the source lane. Source ownership is released and files held frozen for root's build/archive:

| File relative to source/apps/moproj2mst | SHA-256 |
| --- | --- |
| include/m2m/engine/model_manager.hpp | `e9c6633f330a23772ec0f3fc948640262661daed8aa23cfec64aeb47761b0fe8` |
| src/engine/model_manager.cpp | `76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4` |
| tests/mdt/test_model_manager_constructor_prefix.cpp | `70bea282e9374ef68ab2bc35a291e513af23d6353bfef274807c062842cf628c` |

The implementation closes the bounded candidate producer/equality-skip branch only. Root validation and native lifecycle acceptance remain pending; the larger native constructor/main-field handoff and CP79 pending geometry reset guards remain unchanged.
