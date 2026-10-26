# CP64 Type2 Save node prefix implementation freeze

The new helper validates actual owners and appends the native study node to the supplied pending Sub_Project. It returns the precise next unavailable phase rather than performing I/O or claiming Type2 Save success. Root has connected this API to the shared moproj writer and retains an immediate M2M_E_UNSUPPORTED guard using the returned message.

Only three exclusive new helper/test files and this report were written in this implementation lane. Existing shared writer, interfaces, Session, model, I/O, CMake and public driver files are root-owned and were not edited by this lane. No compiler/build, CTest, executable tests, GUI or mutable REA occurred.

## Frozen API

```cpp
Type2SaveNodePrefixResult appendType2SaveNodePrefix(
    engine::ConversionSession&, Project&, SubProject&, xml::Node&);
```

Type2SaveNextBoundary has VariableDimensionReconciliation and XmlOutputDevice. Result fields are nextBoundary, appendedStudyNode(xml::NodePtr), and studyOwner(weak_ptr<Study>), plus guardMessage(). The exact messages are:

```text
Type2 study Save: InputTable variable dimension reconciliation remains unsupported
Type2 study Save: InputTable XML output device remains unsupported
```

Owner admission checks the live Session Project first and SUB membership before dereferencing that SUB, then type2, Project backlink, ownedDoeStudy, raw sub.doe alias, Study.sub backlink, existing Project StudyIo, actual Session ModelManager alias and its ownerSession. The current StudyIo interface exposes its manager as a raw alias; the actual shared manager owner comes from Session. This helper does not certify an otherwise inaccessible I/O-to-manager control block, native QObject completion or callback lifetime.

The caller supplies its cached source Save-frame SUB. The helper does not reselect activeSubProject or require currentModel, preserving the distinction between cached Study identity and later variable reconciliation's actual-current-Project lookup. It validates a pending element tagged Sub_Project and refuses a known already-installed direct Project DOM child, preventing mutation of the old SUB in the shared callsite. The caller still owns the pending-node source-frame provenance; this is not a general DOM-parent/foreign-document ownership API.

After validation it calls the existing source-qualified studyElement(*study), appends that node after existing pending children, and selects the next boundary based only on variable-list emptiness. It does not dereference variables or reconcile dimensions. Empty variables reaches the XML-output-device guard; nonempty variables reaches reconciliation first. Constraint serialization remains later than the currently unavailable XML open boundary and is not prevalidated here.

The result keeps a weak Study diagnostic lease and an ordinary value-node lease; it cannot keep the native Study alive. No study filename, directory, sample/PAR output, FileEffects write, notifier attempt/reset, current selection or old DOM replacement is performed. Type3 is not accepted by the helper, and root keeps its distinct public guard.

## Seven focused tests

- Actual owned name and node order; empty-vars XML stage; no I/O, notifier reset or current-model effects; old SUB untouched.
- Nonempty variable count returns reconciliation stage without variable/current-model access or mutation.
- A still-owned supplied cached SUB remains distinct when activeSub changes; no reselection occurs.
- Foreign Project/SUB, broken Study alias/backlink and Type3 refuse before append.
- Missing/unbound/foreign I/O-manager, wrong pending tag and already-installed old SUB refuse before append.
- Returned weak Study identity expires after actual Project release while the copied node remains inspectable.
- **Real shared recording moproj writer integration** using root's optional Session argument, actual Session/Project/Type2 SUB/Study/I/O and minimal DiagramViews. It reaches the exact XML guard after view construction; old SUB identity/name/children remain intact, earlier root Title update survives, and no study/main-file bytes or notification are emitted.

The final fixture intentionally selects Type2 SUB1 internally and calls the actual shared recording writer. It is not a claim of public C ABI Type2 selection, native project-file truncation, original GUI view parity, or complete Open-to-Save acceptance. A separate native writer fixture must cover real .moproj open/truncation before the same guard. The nonempty-variable fixture supplies only count/unavailable-entry state and never claims to have parsed native B121 variables.

## Static verification and root target

All 18 quoted includes resolve. The test has seven unique Testkit cases and main, supported CHECK/REQUIRE macros, no C assert, final newlines and no trailing whitespace. The SessionNotificationState definition is explicitly included for field assertions. Static API/call inspection confirms root's shared writer passes the Session pointer and uses result.guardMessage(); executable validation is pending.

Exact suggested root target: m2m_doe_type2_save_node_prefix; CTest name: m2m.doe.type2_save_node_prefix. Core must compile src/doe/type2_save_node_prefix.cpp; the test must link the actual shared moproj writer as well as the helper. Root owns Release preflight, grouped checks and archive freeze.

Next dependency is actual InputTable dimension/cleanup/device/status then key-specific DoeTableRefresh delivery, followed by conditional current/LHS/FF/USR outputs in native order. Source qualifiers and the file-error/PAR distinction remain in resume64-doe-type2-integration.md and resume64-doe-save-effects.md. Public B121/DBLIST, Type3 and unresolved Type2 output remain guarded.

Freeze UTC: 2026-10-24T22:06:51.265623+00:00.

| New helper/test | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/type2_save_node_prefix.hpp` | `187d96e0270d241144dc012603569e98d8a91ec3a0e46a1ed0162ec4f92061a5` |
| `source/apps/moproj2mst/src/doe/type2_save_node_prefix.cpp` | `a3b64864214915e0c775019e8b0bd4a10f6b35e62392b5908930183fe1f5dec3` |
| `source/apps/moproj2mst/tests/doe/test_type2_save_node_prefix.cpp` | `05073fb7f5046671a2771adbbeb94fd77bcb0716bf02a2c34d439d43f8270291` |

| Controlling evidence/current shared callsite snapshot | SHA-256 |
| --- | --- |
| `analysis/resume64-doe-type2-integration.md` | `70d215a2a672662751ebb69003cea63c56a28fec24772b1235e515ba57f3a4b4` |
| `analysis/resume64-doe-save-effects.md` | `68c8a8f29fa6d19631d3db7ad9a3d114dcf28f0c369285877c040a996380d18c` |
| `analysis/resume63-doe-public-save-order.md` | `354b25eb69b39d8aa0896fbe2ffd3d12d84f0f6de824599c7da1483ddf432335` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `988b9599de5d47426bef8fd34554b436636e0af2932ecbd1f43e1458629532e4` |
| `source/apps/moproj2mst/src/moproj/write.cpp` | `7e1d496af86ef5bc19971e97cf0b85c7230035320baaf315c6b7f02ba21832ca` |
| `source/apps/moproj2mst/include/m2m/moproj_write.hpp` | `e63805ac6e60b249d03bfb9e051c6aa10d4547186dc31ccc3bb2b82009958738` |
| `source/apps/moproj2mst/include/m2m/doe_io.hpp` | `3207aaa840a4774ac2323fcef675363c08eeca75c8baec2e9b84a7bceff69192` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `09feedab6ca1ac321deee04eecf5ff51bd6c1b2302c396ee4de0a3af5acfcebe` |
