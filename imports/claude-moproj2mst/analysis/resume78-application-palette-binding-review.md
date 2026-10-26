# Runtime app_pal join review

No blocking mismatch was found in the released **runtime registry join** against `resume78-splitview-palette-compose.md`. It retains one same-runtime owner, distinguishes unknown/null/present state and rejects conflicting observations without publishing replacement state. It does not enable a QApplication getter, private/public palette copy, placeholder continuation or widget counter effect. Those limits are necessary to the current acceptance boundary.

Only this review report was written. No source, build, executable test, GUI, mutable REA or original execution changed. Source hashes below identify this inspected snapshot; this is not a test/build freeze.

## Same-runtime owner and alias

`view_manager.hpp:305-313` adds a slot observation carrying qualification, pinned module SHA, optional slot RVA, optional presence, palette-data alias and wrapper word. It does not accept an externally constructed QApplicationPaletteOwnerState or a foreign palette shared_ptr.

`WidgetRuntimeState::Values` owns one `applicationPaletteOwner` (`view_manager.cpp:54`). `observeApplicationPaletteSlot` (`:300-336`) first requires a shared runtime owner, an existing values registry, exact runtime qualification, the pinned QtGui hash and `slotRva=0x9c9690`. The native module origin is the fixed original QtGui binary specified by the composition report, not inferred from a class cache or a widget palette value.

A present alias is resolved only through **this runtime's** `values_->palettes` map (`:310-316`). No new palette-data graph is allocated and no externally supplied control block is accepted. If the runtime already has its shared default data owner, `sameOwner` verifies both pointer and control block before admission. Equal numeric aliases in different runtime registries remain different actual data owners, as the two-runtime test checks.

The owner helper is created internally at most once, with the runtime-specific module/slot qualification. Existing owner and wrapper identities are reused on repeated matching observations. The wrapper's data is the canonical registry shared_ptr, and its group/resolve word is the explicit observed word. The global wrapper word is independent of per-widget word0 and need not equal the runtime default value's wrapper word. A present word0 is accepted because optional presence is checked separately from numeric zero.

The qualification/module inputs are admission receipts supplied by the caller, not authentication that a live original module was inspected. The method cannot turn receipt text into native constructor execution or lifetime evidence. Its ownership guarantee is the internal runtime registry/control-block relationship.

## Unknown, null and conflict behavior

| Observation | Released behavior |
| --- | --- |
| Unknown without payload | Create/reuse the diagnostic owner with Unknown state; no QPalette wrapper/value/read/provider effect. A later unknown observation does not downgrade already known state. |
| Explicit null without payload | Record KnownNull once; repeated null is idempotent. No GlobalColor2 constructor/provider, cache store or output copy is executed. |
| Present | Require positive registered alias and explicit wrapper word, reuse the same data owner and observe its retained wrapper. |
| Unknown/null carrying alias or word | Reject before owner publication, including explicit numeric0 payload. |
| Missing/wrong module, RVA, runtime qualification or registry | Reject before owner allocation/publication. |
| Null-to-present, present-to-null or conflicting present word/data | Reject, retaining the prior owner, wrapper and value. Native replacement/lazy initialization needs a separate source transition, not relaxed observation merging. |

The envelope/alias/default-data checks precede helper creation. For an existing helper, `observeNullSlot` and `observePresentSlot` reject these conflicts without mutation; the runtime additionally checks existing wrapper data with `sameOwner` before calling the older helper's pointer-equality value check (`view_manager.cpp:326-333`). Thus the helper's standalone permissive construction surface is not exposed as a foreign-owner join here.

KnownNull cannot currently advance through the runtime API into initialized output. That preserves the composition plan: the retained runtime default value is not a QPalette(GlobalColor2) constructor, and production provider absence cannot become success. No classCachePresent/classes value is read by this join to infer app_pal presence. When no runtime default value exists, the join can record a present wrapper from a registered alias without manufacturing a runtime default.

## No public/private copy effects

The new methods neither call `QApplicationPaletteOwnerState::read` nor resume the placeholder. The exposed owner is `shared_ptr<const QApplicationPaletteOwnerState>` (`view_manager.hpp:489-491`, `view_manager.cpp:338-341`), so the ordinary returned diagnostic API has no mutable read/provider operation. A retained const owner may preserve historical wrapper/data after runtime destruction but does not keep the runtime alive or authorize native calls.

There is no production observation/consumption call site in this snapshot; the new API calls appear in the focused tests. Existing startup default/class palettes do not automatically produce an app_pal owner. The join changes no WidgetState palette, private-construction input, font, locale, QObject/thread counter, QWidget current/maximum counter or class cache. A C++ observation wrapper allocation is not presented as a native QPalette allocation effect.

This matches the intended first split of CP78. The next consumer still must validate the actual live canonical same-runtime placeholder at PrivatePalette, retain exactly one getter output for that same call, and preserve getter wrapper data/word. Before writing the private destination, it must close or guard the native pointer-store/temporary-deref/possible-destruction/word0/destination-ref sequence. This join does none of those operations and cannot claim that shared_ptr ownership models native Qt atomic refs.

In particular, this review does not certify composition against existing per-widget private input observations. The current join checks the runtime's default **data owner**; a future consumer must also reconcile the particular widget's actual inputs/produced receipt and reject contradictions at its own boundary. A receiver lookup/read API with runtime-lifetime validation is still missing. These are remaining consumer obligations, not reasons to fabricate private copy effects in the registry join.

## Focused test review

`test_widget_application_palette_binding.cpp` contains11 Release-active Testkit cases and a `testkit::run_all()` entry. The source explicitly labels these as registry observations; no getter/private-copy action is invoked. Cases cover:

- default/class cache not inferring an owner; explicit unknown and null without lazy initialization;
- present data sharing by pointer/control block, exact wrapper word, idempotent owner/wrapper identity and unchanged per-widget word;
- preflight rejection for qualification/module/RVA and missing/zero/absent alias or missing word;
- illegal unknown/null payloads and retained state through null/present/data/word conflicts;
- no substitute default data owner, no default creation when absent and independent equal aliases in two runtimes;
- absent values registry/unshared runtime rejection, plus a retained diagnostic owner not retaining runtime lifetime or claiming native read/counter effects.

These tests directly exercise the intended observer state space. No focused case authenticates an original capture, a live placeholder consumer or native Qt ref/release effects, and none should be interpreted that way. No tests were compiled or run in this lane. Root retains grouped/full source-stable validation responsibility.

## Review hashes

Composition report SHA-256: `0b772aa972d720780ad9f78529ab94bbfa9230adf9a2b71fdb2f62e0d58d4893`.

| Source under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine/view_manager.hpp` | `e69f43da56b543c5fa5ca91f6660d43361d3724a4eb0b58b3e5b0541594e3615` |
| `src/engine/view_manager.cpp` | `0ab08f3cb4ac4a8dc24a9469153cdf518c5f8e8654072dc4080641b464e2ffb3` |
| `tests/mdt/test_widget_application_palette_binding.cpp` | `e1db62bee6b3254bd1ccd8be5a944edcf7cf6c6a3fc015b6d9fb559a68ea62bc` |
| `include/m2m/engine/application_palette_owner.hpp` | `ac6badf7155d8c1c7b1cfd816719e3685d5b3e65dbb2731f51f41a346b858e58` |
| `src/engine/application_palette_owner.cpp` | `58d6355b88150698b0af87128e8994f3595fa2c4d0a8db995aec1875d7851348` |

Token measurements are unavailable.
