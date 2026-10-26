# CP163 FRCROT: actual reader owner and temp-file integration edge

The original has an **Open-time function-Keyword callsite** earlier than the Save reconciliation discussed in CP142/145/149. Reader `14090ee20` calls the three-text temp-file converter at **14091111d** when `parmType`, after lowercase conversion, equals `function`. This caller derives its handler selector from **KwEntities**, while the later Save reconciliation caller derives its selector from **KwOperations**. Those paths must not share an assumed selector producer or output-commit gate.

Disposition: a bounded **actual contextual-reader owner prefix** is source-ready for an exact FRCROT/function/empty-operations profile, ending before function conversion at `140910b6f`. Full file/table-helper integration is still blocked by the actual file/stream/scanner/scratch-owner occurrence below. Do not add another detached projection or call the private table helper with fabricated scanner results in production.

Only analysis files were written. No source/test/build, GUI/app control, original process, live REA retry or mutable database action occurred. Original v11.0.2 `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded ranges, source hashes, IAT names, literal bytes and retained capture hashes are in `analysis/resume163-frcrot-integration-evidence.json`; its reproduction script has the same stem with `.py`. No reconstructed executable or deployment is discussed.

## Earliest lifecycle caller and real Keyword owner

The current actual candidate path is `SubProject`'s Type2 `SfDoeStudy` -> contextual `readLazyStudy` -> actual `DoeInputTable.XML` read -> `readInputTableFromSource` -> named Header owner -> `readBoundedKeywordConstructorPrefix`. Its admitted names remain TLOC only. CP159/161's strict empty-operation TLOC branch commits nominal text to the actual named canonical record, then throws at `140910abf` before upper/lower text probes. TLOC's scalar `function=false` cannot exercise the file converter.

The retained complete original reader, independently byte-bound by CP50 and rechecked here, establishes:

1. `14090f7db..14090f835` sets function=true only for lowercase `parmType="function"`; `funcEntryNo` is parsed separately at `14090fa83..14090fb56`.
2. XML KwOperation IDs append to local list `rsp+120`. KwEntity IDs append to **different** local list `rsp+138` at `1409104e0`. Both are passed to the actual manager append at `14091095a`.
3. The manager's nonempty **operation** loop performs SetOprHasDoe before returning. A strict empty `KwOperations` source node yields the zero-iteration branch already qualified in CP159. The entity list does not make this operation loop nonempty.
4. On normal return, the reader re-fetches the original Variable name through virtual+128/count and +138/count-minus-one (`1409109c8/140910a1b`), then sets parameter/function/enum and nominal/upper/lower text on that record. This is the actual named owner, not a borrowed constructor pointer or the outer last-header target.
5. At `140910b6f`, function=false skips to `1409111c4`. Function=true initializes three null output slots, then builds a vector from the **entity** list. No entities yields empty; one entity yields `[entity0,1]`; two or more yield only `[entity0,entity1]` (`140910bb1..140910c58`). This is not a copy of the full entity list and is not the operation vector.
6. After normalizing the three local texts (remove CR, replace LF with CRLF), the vector is materialized by `140917800` at `14091101a`, and the original registry-derived keyword ID, parameter-name bytes, reader dimension and nominal/upper/lower texts are passed to `140916530` at `14091111d`.
7. The reader stores the returned EAX but commits data only when **all three output pointers are nonnull** (`140911146..140911165`). It then calls nominal, lower, upper setter targets in the observed address order `1409210d0`, `1409213f0`, `1409212f0`. Do not replace this with Save reconciliation's gate.

The separate `1400d2b60` Save reconciliation uses the current operation model, changes the Variable dimension before taking its first Keyword, copies that record's `KwOperations` into the vector, and calls the same converter at `1400d30d2`. Its upper/lower pointers are mandatory while nominal is optional. That function is called before the `1400d3200` InputTable writer and from other retained action paths. Direct XML serialization neither proves a temp-file effect nor invokes this entire conditional path unconditionally.

## A concrete source-ready actual-owner successor

The smallest useful real-reader increment is an exact **synthetic exploratory input profile**, derived from these visible branches, not claimed as a captured original fixture:

- Type2 contextual lazy reader, fresh mode0 Study and coherent concrete Header aliases; preserve the existing empty named Item-list requirement.
- Keyword name `FRCROT` (original registry ID276), `unitType="0"`, `parmType="function"`, `parameter="-1"`, `nominalValue="0"`, `funcEntryNo="0"`, and absent parameterName. Treat these as a narrow value projection, with the existing registry/Qt native ref/codec qualification limits; do not infer native string data owners.
- Exact NominalData/UpperData/LowerData first-child `string` values, each fixed single-line seven-bit ASCII without CR/LF. For example `FRCROT 1 1 0 5`, `FRCROT 1 1 0 6`, `FRCROT 1 1 0 4` distinguish the three owned texts. These are source inputs, **not claimed successful original parser cases**.
- Strict raw-child-empty KwOperations and one exact `KwEntity id="1"`. The native zero-operation producer determines that no notifier is reached; do not set a suppression flag or manufacture a callback return. A later function vector would be `[1,1]`, with positive first element selecting the indexed-pair handler arm. Empty/nonpositive selector shapes must not be inferred safe: handler3's alternate arm reads more vector elements without bounds checks.

Extend the actual `keyword_reader_prefix.cpp` profile/branch, reusing `appendFreshKeywordConstructorPrefix` and the existing canonical owner. Append ID276 with operations empty/entities `[1]`, re-resolve the original name, set parameter=-1, function=true/enum=false and all three text values, then **throw at 140910b6f before entering function conversion**. Keep the method noreturn and preserve all other profiles. The three setters `140921980`, `140921ae0`, `140921c40` assign offsets+10/+18/+20, remove CR and replace LF with CRLF; the single-line profile makes the represented text unchanged. They do not populate the null DYNARY data slots.

This advances a real source-file-selected Keyword owner and all three inputs. It does not integrate `applyFrcrotTablePrefix` yet, return parser success, touch temp.KWD, finalize variables/links, read later samples or enable Save. Tests should use actual `ConversionSession`/Type2 lazy read and recording StudyIo, include A,B,A named-owner selection, retain the header/Description-before-Keyword distinction, assert all three record texts/function/entities, and prove no notifier, temp write, later sample or deferred-link completion. Unknown/nonempty/raw-operation profiles must preserve their old guard and earlier effects.

Thus CP145/149's blanket “FRCROT requires notifier delivery first” is too broad. It remains true for nonempty-operation inputs. A source-derived empty operation list genuinely bypasses that notifier, independently of entities. The new actual-owner prefix is implementable without inventing a natural callback return; stronger native shared-ref/codec/allocation or observed original equality claims remain excluded.

## Actual file owner and why the table helper cannot be wired yet

`140916530` constructs a stack QFile from the literal **relative `temp.KWD`**, then opens it using flags composed from2 and0x10. Helpers `140004a60`, `140031680` and `1400318d0` prove bitwise OR -> **0x12, WriteOnly|Text**. IAT entries prove QFile construction/open, QTextStream construction/output, QFile close and destructors. It writes the supplied nominal QString through QTextStream, closes QFile, constructs a dimensioned `0x3408` scratch model, then opens the same literal path through `14146fcd8("temp.KWD","r")`.

The first `%39s` scan return at `1409166b1` is ignored. Registry group3 selects `140b667a0` at `140916756`. Only parser status0 reaches the upper write phase; upper status0 reaches lower. Handler3/output deep copies happen after all three parses pass. Three FILE pointers and scratch models remain nested through later phases and are cleaned up in reverse order; this is not three independent temporary files. There is no explicit temp-file removal call in this captured body.

The inactive candidate `keywordValues` is not this owner contract: it plans `projectFolder + separator + temp.KWD`, comments WriteOnly without Text, creates a separate fixed host temporary filename, uses host `std::fscanf`, closes/removes each phase's temporary immediately, discards `ids` and refuses FRCROT's nondefault handler3 before the first planned write. No production caller reaches it. Removing that guard or forwarding a canonical record to it would change path, mode, lifetime, parsing and selector behavior.

The actual relative path resolves against the process directory at each file call; the native function has no projectFolder argument. The same-occurrence current-directory/device owner, QTextStream output codec/state and resulting bytes, C text-mode reopen/translation, scanner file position/status and temporary-model/pair allocations must be bound before claiming file-to-helper parity. A no-CR/LF ASCII source value reduces newline cases but does not qualify an unknown active QTextStream codec or make a host parser the native scanner.

## Scanner return producer and remaining occurrence

CP161's prepared results are now tied to a concrete native producer chain:

`140af73f0` -> varargs wrapper **141472ac8** -> file-validation/locking wrapper **14147295c** -> callback **141480128** selected explicitly at `141472ae5`.

The validation wrapper can itself return -1 for invalid stream/format or incompatible file state; -1 is not proof of EOF. On its normal path, `141472aa3` calls that callback with the actual FILE, format, null locale argument and varargs destinations, copies EAX to EDI, unlocks the stream, and returns the saved result. The callback body is retained, but this audit does not claim a complete recovered float/locale grammar from it. A particular -1/0/1 observation must be produced by the actual reached FILE/format/cursor and destination occurrence, not selected from a test vector after reading a string with a different host CRT. CP161 correctly uses the generic status1 category for -1.

Even a qualified scanner return sequence would still need the group3 forward-pair identity/constructor owner, successful table buffer allocation, feature-sensitive reverse copy, handler3 selector and the `140898f40` copied-output lifetime before public admission. The source-ready owner prefix above stops before every one of these effects. For nonempty operations the first additional blocker remains the invocation-bound Qt codec/data publication for SetOprHasDoe key construction, then actual map/snapshot/receiver return and named re-fetch, as precisely recorded in CP146; neither this empty branch nor a private table projection resolves it.

## Selected immutable byte receipts

Ranges are end-exclusive. Full raw/capture hashes, file offsets and the current candidate source identities are retained in the JSON receipt.

| Range | SHA-256 |
| --- | --- |
| `14090f7db..14090f835`, function discriminator | `ba5e671522141b44a107a260186fe36f607e80ce85667fe4bc230a7faec57fe7` |
| `140910900..140910a67`, actual arguments/re-fetch/flags | `01489910532b394b7692e9ee5e36a212fa76fe51bf3a0f86e1083fdebc2dc74e` |
| `140910a67..140910b6f`, three owned-text setters | `7e581ec92eea0e054c81094c4878204d4ded633fb281936f77c57c36d4bb4a27` |
| `140910b6f..140910c58`, entity selector | `47fb4f7e6e424ea4c298dac46c846fede0fccd906a4880fca0c1c014ee1dd807` |
| `14091100a..1409111c4`, Open conversion/commit gate | `c16f4951665d2388b83525adf3422236d61ad91ab4cac77b302bb15bc7d5274b` |
| `140916530..140917055`, complete temp converter region | `43aef4d26f957c36cc8858d3b46e67eb54d62b82eaf324fd1047ee153fcc7110` |
| `1400d2b60..1400d31f3`, Save reconciliation | `4ba0640f63710b92171f3431d2b72268c5f38e22ac5d11f4ed24a4fa888bf8d3` |
| `1400318d0..140031904`, flags OR | `8e37b4fe90bfd056d2194ef6ad1216cfac5150f5ad73090f321d8488d92bc2b5` |
| `14147295c..141472ac6`, file-validation/callback wrapper | `d3654dc4ab46e6e9f28c5a50b7716c11edfdf443c5bfb7540bd18011b3fbd8de` |
| `141480128..1414812a8`, scanner callback | `89c881b26060543e1896944b90faf83c29f89e51e183e4b6f8a171015dfdfd0e` |
| `140921980..140921ad3`, nominal setter | `73afd7455b09c76d7c47ad11c2a9bf0c05a299ccb96a3a000e6f4ab5d5a502d9` |
| `140921ae0..140921c33`, upper setter | `2fff3128ade0202296be62563e85d31db77525c0c077aef49121e326f9c0f370` |
| `140921c40..140921d93`, lower setter | `a19c3db10d4d24882eebbe8c1e5675d75963670a43b02df2768699c504a35250` |

No active FRCROT success or original scanner outcome is claimed. The advancement is a conditional actual-owner source plan plus a precise, original-file-bound integration blocker; the frozen CP161 table helper and existing public guards remain unchanged.
