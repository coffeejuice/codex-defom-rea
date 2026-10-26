# CP148: the application codec producer cannot bootstrap its own late value

The smallest authoritative event is an actual read or write of QtCore's C-string codec field in the selected process occurrence. Application language selection alone supplies neither event. The recovered setup has an earlier C-string conversion while constructing its plugin path, before its named-codec lookup and setter. Its failed lookup also enters a warning dialog rather than storing null. CP147's production ProcessCodecOwner must therefore remain Unknown until a qualified source execution or occurrence-bound field observation supplies the missing input.

This report changes no source. It uses retained captures and bounded read-only DUMPBIN decoding of hash-verified immutable PE files. No GUI, running original, REA/database operation, compiler, executable test or build was used. CP147 files remain held for root validation.

## Actual setup order and the earlier read

The controlling prior reports are `resume83-doe-codec-producer-next.md`, `resume146-qstring-process-owner.md` and `resume147-qstring-process-owner-implementation.md`. Preferred addresses below are evidence addresses, not observed loaded addresses.

Application construction calls codec setup 0x1400024f0 at 0x140002227, after QApplication construction, helper 0x1407ffc10 and callback registration. Translator setup follows. Main-window construction occurs later. This process setup is not a Project/Study event, and the reconstructed converter does not currently execute this application constructor.

Setup reads raw language cache 0x143d79718 at 0x14000251a. Values 1/2/3/4/7 select Shift-JIS/Big5/KOI8-R/EUC-KR/GBK; other values leave the local codec-name pointer null. Before testing that pointer, setup performs these normal-path operations:

1. Helper 0x1402c8210 called at 0x1400025ad returns true in its captured body. It initializes a local byte to zero and returns its negation; no nested call or global input appears in those 43 bytes.
2. The true branch obtains QCoreApplication::applicationDirPath through IAT 0x141e04458. At 0x1400025f0 it calls string join helper 0x140002110 with suffix `/plugins_64` at 0x1420ee0f0. The alternate `/plugins_32` branch exists but is not selected by that recovered helper result.
3. Join helper copies the directory QString at 0x14000213e, calls QString::fromAscii at 0x140002155 through IAT 0x141e04430 with the suffix and length -1, appends the resulting QString at 0x140002174, and releases temporaries. Setup passes the joined path to QCoreApplication::addLibraryPath at 0x140002615 through IAT 0x141e04460.
4. QtCore QString::fromAscii export 0x67074300 calls fromAscii_helper 0x67074120 at 0x6707431a and publishes the returned data into its destination at 0x6707431f. The helper compares the actual C-string field at 0x67074126, before choosing null allocation versus nonnull decoding. This is a pre-configuration read of the old field, not a read of the language-selected codec.

This closes a meaningful static edge absent from the earlier language-only admission proposal. Even a fully known selected language cannot initialize this setup by assuming its eventual codec result: setup's plugin-path QString already needs the old codec/data owner. The applicationDirPath producer and earlier QApplication/startup calls remain additional unclosed inputs. The normal-path result does not establish that any particular retained run reached these calls or returned successfully.

## Lookup, setter result and liveness are separate facts

At 0x1400026af the selected name is tested. A null name skips the lookup/setter block. Otherwise codecForName(char const*) is called at 0x1400026c0. Its returned pointer is stored at stack+0x40 and tested. On nonnull return, that same pointer is passed in order to setCodecForLocale, setCodecForTr and setCodecForCStrings at 0x1400026d8, 0x1400026e3 and 0x1400026ee.

The final QtCore setter 0x67177590 is exactly a raw pointer store into 0x672f3e10 followed by RET. It does not retain a codec, initialize a QString refcount, validate a vtable or establish future liveness. Translation setter 0x67177580 similarly writes the separate translation field 0x672f4440. Locale setter 0x6714bc20 has locking and additional calls; the three globals and their effects must not be merged. Reaching the final store requires the preceding calls to have returned, rather than merely observing a prior nonnull lookup result.

A null lookup takes the warning branch beginning 0x1400026f9. It constructs translated warning text and calls QMessageBox::warning at 0x140002809. No direct C-string null store occurs on that branch. The warning is an additional UI/event boundary; a no-setter branch cannot certify preservation of a historical field value through its nested calls. Likewise an unsupported language means no direct setter in this routine, not that the field is currently null.

The char-pointer lookup export 0x670c1fb0 constructs a QByteArray and calls the QByteArray overload 0x6714cb20. The latter obtains the codec registry/cache, checks cached/registered entries, invokes codec name/alias virtual methods, and has fallback helper 0x6714be20. This bounded trace does not close the fallback or prove a particular named codec exists, which object is returned, or its decoder lifetime. The newly added plugin library path also precedes this lookup. A language enum or a presumed builtin codec is not a substitute for the returned object identity.

CP146 separates that pointer from the registry owner: QTextCodec construction registers objects; destruction removes registry/cache entries; module cleanup first clears the registry pointer and later performs deleting virtual calls. The C-string setter itself owns no such object. Registry destruction does not justify inventing a C-string null store, and the same address reused after object death would not preserve a codec generation. Project Close is not codec/module cleanup.

## What can update production knowledge

| Reached native event | Established fact | Still unavailable |
| --- | --- | --- |
| Setup entered, language read/name selected | Historical setup decision and selected name | Current C-string value, lookup result, data owner |
| No selected name, or codecForName returned null | This setup branch performs no direct C-string setter | Current nullness or preservation through nested calls/events |
| Actual 0x67177590 store with actual argument | Raw field value at that write occurrence; this setup's reached argument is nonnull | Decoder liveness, returned QString data, later field value |
| Actual 0x67074126 field read for the selected conversion | Null/nonnull branch choice at that occurrence | Nonnull decoder validity and unrelated later conversions |
| Nonnull branch's actual reload at 0x67074184 | Pointer used by that branch's subsequent decoder path | Codec owner/generation, virtual decoder return and produced data |

For a later source implementation, these must be events of the canonical process/module owner already borrowed by the actual Session. A production write/read event needs process/profile and loaded-module generation, source call occurrence, execution order and actual pointer value. A nonnull decoder additionally needs codec-object lifetime/generation. A generic public `setKnownNull`, a name-based codec factory, an isolated profile or a caller-provided historical pointer would bypass the missing evidence and is not proposed.

A store establishes knowledge at that store. Preserving it for a later notifier requires coverage of intervening writers/lifetime changes; an unqualified interval restores Unknown. A monotonically increasing host epoch cannot itself prove such coverage. A read tied to the actual QString occurrence can qualify that occurrence without proving all earlier startup writers. Once a null branch is actually selected, later field changes do not retroactively change it. Nonnull compare/reload are distinct source reads and cannot silently be collapsed across an unknown interval.

The actual selected CP145 context supplies checked borrowed Session/Project/SUB/StudyIo identity. Its bound CP147 owner supplies one inert process-profile identity, not an observed QtCore load or source read. After the notifier's unsuppressed flag90 reset, the SetOprHasDoe key still stops before QString construction. Suppression still returns from the notifier and stops before named-keyword re-fetch. Refresh and icon sites can borrow the same process history but need their own reached occurrence and data-publication proof; a successful earlier key does not allocate their data or prove receiver delivery.

## Smallest next action and implementation boundary

No production knowledge update is source-closed in the current reconstructed driver. CP147's immutable Unknown holder is correct. Adding another decision-only helper would record startup history without advancing actual admission, so this report proposes no source edit.

The narrowest missing original witness is a bounded read of QtCore base+RVA 0x2f3e10 at the selected QString helper's 0x67074126 branch, tied to the same process/module generation, thread, source caller and destination occurrence. For the actual notifier this means the key constructor reached after its flag90 clear; for the earlier setup it means the `/plugins_64` fromAscii call. A nonnull observation must include the pointer actually reloaded at 0x67074184 and live codec/vtable generation before decoder admission. This report initiates no runtime observation and does not claim permission to bypass root's GUI/REA serialization.

If pursuing static reconstruction instead, first close the pre-configuration plugin-path conversion and the app-owned QString inputs, then the actual registry/cache/fallback result at 0x6714cb20/0x6714be20 and its registered object lifetime. Only execution of that source path can supply its final setter event. Repeating scans of the language setter or exported codec store cannot resolve those invocation inputs.

When evidence exists, root must coordinate the canonical process-owner API and actual consumer event together. Minimum files are `include/m2m/engine/process_codec_owner.hpp` and the selected existing DOE constructor/notifier site; Session borrowing need not change. Tests should distinguish historical setter from later read, null lookup from null field, separate profiles/module/object generations, unknown intervals, the nonnull reload, Project Close without reset, and isolated profiles that cannot admit production. There is no current testable successful production value to invent.

## Compact static receipt

Immutable GUI SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Immutable QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Both hashes were rechecked in this lane. DUMPBIN read-only decoding used `/DISASM:NOBYTES /RANGE:0x<entry>,0x<last-byte>`; no analysis database was opened. These raw ranges end at the stated RET and exclude following padding.

| Preferred VA / bytes | SHA-256 |
| --- | --- |
| GUI 140002110 /166 | 66264b72ff033e1e059fd29eeb7cdf3239056c3dbc81a2cd09c198cd2a26e6db |
| GUI 1402c8210 /43 | 33f9c7e247342b6eb520050cc2a56e0ae34d97496ed15517a79403b2e682ace8 |
| QtCore 67074300 /43 | 829bfb1bedd8245313de9c83a3e006285d69d17ca711cdbb82d8514136e06fe5 |
| QtCore 670c1fb0 /70 | 73bd018a332878017d7962abca6ec90b7e7b83cdee29244162f905714f1119b4 |
| QtCore 6714cb20 /1441 | 6e98f7c865d61acbb7f314e121ea3cae00811c786ac1308c3645c78e221a7b60 |

Retained setup capture `resume81-doe-qstring-codec-init-1400024f0.asm` SHA-256 is `22f992ffc002c8ba9c3058c676d71cd6f87c6e9b0d6cd43580d1980212dcf994`. Locale and translation/store captures `wave3-editor-startup-codec-bindings-6714bc20.asm` and `wave3-editor-startup-codec-bindings-67177580.asm` have hashes `d5345ac7518bef4e9b6940a7cdc30c488050a954db5ba751f8bf93b90a76ab31` and `218bbe172731161a9479dce1b293ec09e5d60cc8fa35b5b84c629a7135d8084e`. CP146 records the branch/helper, registry and cleanup body receipts. No new raw capture file was required.
