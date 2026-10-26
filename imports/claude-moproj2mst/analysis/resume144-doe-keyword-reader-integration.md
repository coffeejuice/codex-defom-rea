# CP144: bind the constructor prefix to the reader's named Variable

CP143 closes the constructor/append owner stage needed for a bounded actual-reader partial effect. It can be integrated at the current QString/value abstraction through a narrow decoded scalar/TLOC profile, **using the original row name to select its actual Header owner**, then stopping before the first SetOprHasDoe invocation. Calling it on the reader's existing last-record pointer would be wrong for duplicate-name rows. It cannot report a normal native Keyword read, dispatch completion or public Save success.

This report is the only file written. No source/test/build, GUI, original process or mutable REA action occurred. It uses retained immutable original evidence and current frozen source. It does not claim a new runtime codec, receiver or native QString data-owner observation. Literal/numeric/value projection at this boundary must remain explicitly distinguished from native Qt ref/allocation/codec parity.

## Native ownership and selection sequence

The original GUI hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Controlling retained evidence includes the full90ee20 byte-qualified reader in the CP50 body/annotated capture, CP51's corrected notification argument order, CP93's name lookup capture, and CP141's native constructor/owner chain. The older unqualified `resume50-doe-variable-body.md` predates that recovered body and is historical, not the current evidence limit; use `resume50-doe-variable-body-pe.md` instead.

The actual manager constructor installs vtable1421671b8. Relevant byte-verified slots are +118=**derived0d25a0**, +128=91a950, +130=91d970, +138=91aa50, +180=91b5d0 and +1a0=91b950. The base DefDoeModel's +118=91a5b0 is not the public manager dispatch target.

| Native phase | Exact owner/effect |
| --- | --- |
| 90f0ac..90f0ef | Name/sample create helper may return an existing named record, but the reader ignores that pointer and obtains its last manager record. |
| 90f225 /90f352 /90f3f6..408 | Optional ID/dimension writes and queued LinkTo source use that last record. Earlier Description also uses it. |
| 90f5bf | Exact direct Keyword enters after those retained effects. |
| 90f637..90fa48 | Collect name, unitType, lowercased parmType, parameter, parameterName and nominalValue in order. Function/enum flags remain locals. |
| Keyword child loop | Collect text wrappers and ordered KwOperation/KwEntity IDs. Collected text is not yet committed to a keyword. |
| 9108d0 | Convert keyword name to local8 and resolve registry ID with921e90; TLOC's table value is8. |
| 91095a | Call actual manager vt+118 with **original Variable row name**, keyword ID, parameterName, unitType, reader-local dimension, nominalValue, operations and entities. |
| derived0d25a0 ->base91a5b0 | Check name existence, resolve name index and select its record, then call816b40 at91a6fb. |
| 816b8f /816ce1 /816d24 | Store supplied dimension, construct keyword, append to the selected Variable's canonical+30 owner; then union operation IDs into+38. |
| 0d2995 loop | For each supplied operation, derived manager invokes the global notifier with `slotSetOprHasDoe(int operationId, bool true)`. |
| 9109c8 /910a1b | Only after all manager notifications return, re-fetch the named Variable's keyword count, subtract1, and fetch that keyword. |
| 910a38 onward | Apply selector/type flags, nominal text, scalar/enum/function branches and later reader effects. |

Name existence and name-index helpers compare case-sensitive QString content in manager list order. Name-index returns the first matching index. CP93 already qualifies that value-level behavior. The candidate should select through the coherent `Study::vars` order and resolve the matching concrete Header owner, not use Header-owner vector order as an independent manager order.

The reader-local dimension is initialized3; an absent dimension attribute skips the earlier last-record setter but does not change that local3. Keyword construction receives this local value. Do not pass the selected Header's current dimension as a substitute.

### Duplicate-name consequence

For existing Variables A,B followed by a source row named A:

1. Existing create helper does not allocate/select/invalidate another Variable.
2. Reader header/LinkTo/Description effects target last record B.
3. Keyword manager dispatch selects A by original row name and appends/unions on A.
4. After notification return, keyword retrieval again uses A's name.

The current `appendBoundedHeaderVariable` local `variable` is B in this case. Passing it directly to CP143 would incorrectly append on B. A fresh first row hides this defect, so an integration test must include A,B,A.

## Current candidate seam

The actual lazy path already supplies the correct owner: `readLazyStudy` keeps its existing Study, reads DoeInputTable.XML through the bound StudyIo and calls `readInputTableFromSource(bytes,study,InputTableSource{input})` (`src/doe/activation.cpp:41`). The named-source wrapper owns filename context; it is unrelated to choosing a Keyword receiver. `doe_study.cpp` dispatches exact direct DoeVariable rows to `appendBoundedHeaderVariable` with one per-container deferred-link queue.

The latter currently retains header, earlier Description, fresh sample-invalid flags and queued links, then throws at exact Keyword entry (`src/doe/variable_record.cpp:177..181`). CP143 can advance this partial state without changing activation, Study, Session or parser signatures. Its canonical Item now retains the real typed record under the actual Header owner when the new later guard unwinds. No retained ScalarVariablePrefixRecord clone, Study/shared_ptr cycle, parallel keyword list or incomplete derived deletion is needed.

The current CP143 helper requires coherent Header owners/aliases, no alternate retained/scalar/geometry owner domains and an empty canonical Item list. Those admission bounds still apply at the Keyword encounter. It accepts explicit decoded inputs and performs no registry, parser or manager selection itself. A reader shim must supply those duties rather than treat the helper as evidence that they ran.

## Smallest bounded public partial-prefix proposal

Implement a decoder/manager shim that always ends in an explicit reconstruction guard; do not turn this into a successful parser path. A suitable shape is:

```cpp
[[noreturn]] void readBoundedKeywordConstructorPrefix(
    Study&, const qt::String& originalVariableRowName,
    int readerLocalDimension, const xml::Node& keyword);
```

The proposed first admission profile should be the already-retained B121 scalar/TLOC values, rather than general Qt numeric conversion:

- exact TLOC keyword name; unitType literal0; parmType exact lowercase double; parameter literal-1; parameterName absent (its default null value); nominalValue literal0.1;
- only the retained NominalData/string and KwOperations/KwOperation shape, with one exact operation ID1; no KwEntities, UpperData, LowerData, EnumData or function-specific attributes/children;
- retain the native first-child rule for NominalData: do not silently skip a raw/text first child and select a later string element. Collected nominal text remains a local, never a committed field before notification;
- source row is otherwise admitted by current Header parsing, manager mode0, actual coherent Header owners and exact row-name lookup. The named target's canonical Item list must be empty. The original variable name need not equal Transfer Time merely to perform the already-qualified content lookup; no new name normalization is introduced.

This is a narrow reconstruction admission profile, not native input validation. Reject any profile outside it at the existing Keyword encounter, retaining earlier header/Description effects but creating no Keyword. Existing malformed conversions and optional-header bounds stay intact. An initial implementation may require the exact retained nominal text to further bound the profile. Do not reuse the older scalar-prefix decoder wholesale: it owns a different Variable, interprets more shapes through host converters and has its own retention semantics.

On that admitted value profile, resolve the original row name to the actual first named Header alias. Call CP143 with ID8, parsed/default parameterName, unit0, reader-local dimension3, nominal0.1, operations[1] and empty entities. Then refuse **before** the first derived-manager notifier invocation. The record retains constructor-null nominal/upper/lower text, +80/+84 selectors remain constructor-1, function/enum/bound flags remain false and data slots remain null. The collected nominal text is not copied into it merely because the later value would be known.

Do not call the existing Session notifier just to move the guard: its unsuppressed prefix resets flag90 before its population refusal, and its supported/test bindings do not establish the original registered receiver occurrence. The proposed first cut intentionally has no notifier attempt, flag reset, key/map mutation or delivery. If a later root assignment advances into dispatch, it must supply the actual Session/StudyIo notifier identity and preserve every reached partial effect.

For retry/nonempty-list profiles, keep the explicit reconstruction admission before CP143's dimension store. Native816b40 could instead dedupe and notify; that existing-record branch is outside this first integration. Do not claim retry refusal is original execution behavior. A same-name row whose named target is absent or incoherent remains guarded before the new append, not substituted with the last record.

### Value abstraction and native QString ownership limit

The proposal advances canonical **values and ownership at the existing model abstraction**. It does not assert that native QDom/QString/QList temporaries, default QLocale publication, toLower/toLocal8Bit codec selection, registry CString construction or their allocation failures have run in the candidate. This limitation already applies to the retained scalar prefix and current bounded Header/value readers; it must be explicit in the integration receipt.

If root requires full invocation-bound native QString/data/ref parity before this public partial-state cut, it is not closed: the first unrepresented Keyword-local phase begins at90f5bf, and the registry input at9108a1..9108d0 requires the actual toLocal8Bit result and data lifetime at that invocation. No codec default may be fabricated. In that stricter scope, keep the current public entry guard and expose only the explicitly decoded private CP143 API. The owner migration does not itself resolve a process codec. This report authorizes neither scope expansion nor a new runtime probe; source work awaits root's explicit value-prefix assignment.

## Unwind and Save ordering

After the proposed post-append guard, the same actual Study retains Variable/header and canonical keyword/operation-prefix mutations. Fresh Variable creation has already invalidated all three sample flags; tables/counts/mode retain their prior payloads. Earlier Description may have changed; later child effects do not run. The per-container queued links unwind without resolving, container reindex and outer Variable-ID finalization remain unreached, outer Operations is not processed, and lazy LHS/FF/USR writes/current-PAR read do not run. Study name and the completed XML read remain reached effects.

After a genuinely qualified notifier return, do not resume using CP143's borrowed result as the assumed current target. Native re-fetches keyword count-1 by the original row name. Reentrant callbacks can change collection state; the actual returned owner/liveness must be revalidated before selector/type/text writes. A successful/suppressed helper status alone does not certify this later lookup or parser completion.

Public nonempty Type2 Save still stops at variable dimension reconciliation before prune/XML. Adding an owned keyword changes the source-qualified private prune count criterion (the Variable is no longer empty), but it does not close the earlier global/current-MO/lazy operation-manager stage. XML output, DoeTableRefresh key/receiver return, cached-manager current-mode/fixed-mode PAR order and public completion remain guarded. No Save callback is supplied by the parser shim.

## Proposed source/test ownership after assignment

Worker: exclusive `src/doe/variable_record.cpp` for the exact Keyword call site; NEW `src/doe/keyword_reader_prefix.hpp/.cpp`; NEW `tests/doe/test_keyword_reader_prefix.cpp`; implementation receipt. A public profile-comment edit in `include/m2m/doe_variable_record.hpp` needs root assignment. No shared Study/Session/activation/emitter/Save/CMake interface change is necessary for a stop-before-notifier cut.

Focused actual lazy-reader tests must cover:

1. Fresh admitted row: same Study/Header ownership, constructor-only typed keyword, op union, selected state/fresh invalid flags, XML-read receipt, no later sample attempts and no notifier attempt.
2. A,B,A: header/Description and queued-link source effects on B, keyword/dimension/ops on named A, B's Item list untouched; no deferred-link resolution.
3. Earlier Description retained, later Description/Geometry/other children unreached after the new guard; known repeated/unknown Keyword profiles retain their earlier entry refusal.
4. Retry on the now-nonempty named target refuses without replacing its pointer or synthesizing dedupe/notification; malformed/foreign/legacy cases create no new keyword and preserve reached header effects.
5. Absent dimension uses reader-local3 for Keyword, not a previously changed named target's dimension. Existing typed owners/aliases remain canonical and no retained scalar-prefix owner is created.
6. Public Save remains refused before reconciliation/prune/XML; no notification return, text commit, success status or old DOM replacement is claimed.

No source or test was edited in CP144. CP143's private API remains the only implemented constructor entry until root assigns the bounded integration and its value/native ownership scope.
