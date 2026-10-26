# CP70 native Type2 observer regression

## Test change

Updated `source/apps/moproj2mst/tests/doe/test_type2_native_open_guard.cpp` to pass a caller-owned `engine::FileEffects` observation sink through the new final `saveProjectFileNative` parameter. The test now removes B120's retained `DoeInputTable.XML` from its disposable project copy before Save, preserving the original bytes/hash in memory. It catches the actual DOE table owner guard and verifies:

- the XML writer returned normally with `writerSucceeded=true`, no stream failure, and post-close file error 0;
- `StudyIo::xmlJournalOwner()`, the observer sink's `xmlJournal`, and the pre-call journal are the same owner;
- the single attempt reached `RefreshBoundaryReached`, with the expected ordered XML events and attempt ordinal 0;
- the physically regenerated XML matches the retained B120 bytes exactly: 249 bytes, SHA-256 `1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0`;
- the enclosing Save still stops at `DOE table owner: main-window +458 is unavailable`, and the observer sink contains no replayable writes, deletes, directory attempts, removals, or renames.

The expected event sequence follows the existing file/device ordering: open requested/result; close begins; buffered QTextStream write is requested/returned; flush; close completes; QFile error is observed; normal XML writer return; then the pre-dispatch refresh-boundary marker. The exact owner refusal establishes that actual receiver delivery remains beyond the test boundary. The native `.moproj` itself is still truncated before that refusal, as the existing test asserts.

## Static preflight and freeze

The test-only source hash is `377dc3205a2364887a72161a3d400cdc4b6c5898d99a2fc5e9d75d704c5bf83d`. The existing instrumentation files inspected for the new call contract were unchanged during this test edit:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/moproj/write.cpp` | `6cee2b06ce406e18c3a74643fe387b14c0434e22025debc5d734f55505d259d1` |
| `source/apps/moproj2mst/src/doe/io.cpp` | `b8c71b9a5be6fd13b444af8d998dbafa5a71a38e31adcc72894145d5655e87f1` |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `c086a5530b3828204083d1891bb3f4be555bd7b822cbda851fcfa66780f2fd59` |

Static review confirms the observation argument only receives the shared journal alias before native file work; the journal is mutated by the actual `StudyIo` device callbacks, not replayed through `FileEffects`. The source distinguishes `NormalWriterReturn` from the `RefreshBoundaryReached` event and calls `requireRefreshDelivery` after the latter. The test file has no added raw `assert`; all test predicates use Release-active Testkit checks. No build or test execution was performed in this lane; root owns validation.

## Scope

This is an internal candidate native-writer prefix comparison against the retained B120 after-state. It does not establish public `m2m_convert_w` Type2 selection, actual Qt timer/UI refresh behavior, later PAR/sample writers, final project serialization, or fresh-original lifecycle parity. B121/B122 variable and geometry content remain outside this empty-variable test.

No production source, build output, GUI, or mutable REA session was changed or exercised. The only source edit in this lane is the named test file; this report is the sole other file written.
