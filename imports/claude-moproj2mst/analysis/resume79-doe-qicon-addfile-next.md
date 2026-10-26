# Checkpoint 79: the next addFile ownership boundary

The next actual mutation after CP78's shell/request prefix is QFileInfo's separate private allocation and reference initialization. Its constructor already enters filesystem/resource engine selection before addFile obtains the suffix or asks an icon plugin factory. The fixed `.png` resource name does not close those engine owners. In this Windows build the file-entry paths are QStrings; they must not be represented as QByteArray storage.

The smallest independent, fully bounded next state slice is admitted nonnull allocation of the0xe0-byte QFileInfo private payload and its initial32-bit reference store0, then a guard before its actual QString path-copy ownership. A useful later extension can reach the file-engine handler boundary only after those QString/shared-null owners are bound. Neither prefix may publish the outer QFileInfo as constructed or change QIcon.d before its own native source phase.

This lane wrote only uniquely prefixed immutable PE selections/receipts and this report. No candidate source/build/test/GUI/app/mutable REA changed, and no CP78 source was edited during its validation/freeze.

## Three distinct value/owner layers

1. **CP78 QIcon value:** eight-byte d slot, null after default constructor. It is a heap value object, not QFileInfo, QObject or a plugin engine. Its fixed addFile request has QSize(-1,-1), mode0/state1.
2. **QFileInfo temporary:** a separate wrapper on addFile's stack and a0xe0-byte private allocation. Its filename/native-path strings, cached indices, refcount and file-engine ownership must be constructed before suffix lookup. This is not the QIconPrivate32-byte allocation described in CP78.
3. **Icon plugin/private engine:** later global QFactoryLoader owners return plugin interfaces/created engines; selected results become QIconPrivate and engine state. Only after that does addFile invoke engine virtual+0x28. The engine's later image decoder/byte arrays/resources are another layer.

The original DLL/PE import names and shared-null export addresses, not candidate field names, establish these distinctions.

## QFileInfo constructor and publication order

GUI addFile `0x650a2590` uses imported QFileInfo(QString) at `0x650a25e4`, resolved to QtCore `0x670b9a00`. The constructor:

- Allocates0xe0 bytes at `0x670b9a1f`.
- On nonnull calls private constructor `0x670b8030` at `0x670b9a34` with the actual path.
- Only after that returns does `0x670b9a3d` store the private pointer in the QFileInfo wrapper, then `0x670b9a45` increments private reference from0 to1. The initial private reference is not1, and the wrapper must not be published early.
- Allocation-null constructs wrapper.d=null; the subsequent suffix path dereferences it. This is an unsupported fault/null branch, not an ordinary successful empty suffix/file result.

Private constructor `0x670b8030` starts with reference0 at `0x670b8052`, then calls QDir::fromNativeSeparators at `0x670b8059`. It constructs file-entry storage at private+8 via `0x670eed10`, releases its temporary normalized path, zeros the metadata words at private+0x20/+0x24, then calls `0x670f0080` at `0x670b808e`. **The private engine pointer at+0x50 is stored only after that engine-selection call returns.** Cached arrays/flags at+0x58 and beyond are initialized later. Do not zero the entire uninitialized payload or expose those unexecuted fields as known.

The fixed DOE resource path contains no backslash. QDir::fromNativeSeparators `0x670ad700` takes its no-replacement QString copy branch, with an actual shared-data reference increment. General backslash input has its own detach/replace path. File-entry constructor calls the same separator helper again; this is not a filesystem canonicalization or file-existence result.

## QString versus QByteArray

`0x670eed10` constructs:

- file-entry+0: normalized **QString** filename;
- file-entry+8: pointer to **QString::shared_null0x672f0b30**, with its reference increment;
- signed16-bit cached values at+0x10/+0x12=-2, and+0x14=0.

The immutable QtCore export table distinguishes `QString::shared_null` RVA0x2f0b30 from `QByteArray::shared_null` RVA0x2f0900. The native-path field here is not a byte array. The normalized input temporary release leaves the filename's own QString lease; it does not drop all references to the original resource string.

The icon-factory getter also does **not** construct a QByteArray for `/iconengines`: exact imports are QLatin1String(char const*) at QtGui IAT0x656969e0, then QString(QLatin1String const&) at0x65696c08. The factory IID is a borrowed constant char string. Any QByteArray conversion/codec/image-format cache needed inside later loader or engine descendants must have its own source-qualified byte-array data/ref/encoding ownership; it cannot be borrowed from these QString fields or assumed from a filename suffix. This report closes no general QByteArray engine payload.

## File engine selection occurs before suffix/plugin selection

`0x670f0080` copies the file-entry's two QString owners and cache words, clears an output engine pointer, then calls `0x670efbc0`. That function obtains the path and first calls `0x670a7dd0` at `0x670efc00`, storing its returned custom engine before interpreting resource/drive prefixes.

`0x670a7dd0` checks actual global handler state at0x672f3ed0. If enabled, it obtains/locks the actual registry owner and visits handlers in order, calling handler virtual+8(path) until the first nonnull engine. It does not return a default inferred from the source file's fixed path. A qualified disabled/empty handler branch returns null; the current recovery has not bound that actual global inventory/lifetime for this reached QFileInfo call.

Only when that first custom-handler attempt returns null does the code inspect path characters. For a colon at index0 (`:/...`), `0x670efcbe` allocates a16-byte resource engine and calls `0x670cc9a0`. It can otherwise take other prefix/search/native paths. The QFileInfo constructor uses the no-existence-check parameter, but that does not omit engine creation, registry interaction or ref effects. Do not jump directly to the resource engine because the request starts `:`; custom handlers precede it.

After a real private constructor return, QFileInfo::suffix `0x670b7c50` checks private+0xb4 and delegates to file-entry `0x670ef690` on its admitted path. That helper resolves cached file-position values and uses the actual QString slice. `.png` is a value fact for this literal, not evidence that preceding constructor/handler effects completed.

## Icon factory and fallback ownership remains later

Once suffix QString is obtained, addFile's existing branch proceeds as CP78 described:

1. Getter `0x650a1e80` for V2 factory owner global0x659cb678, IID `com.trolltech.Qt.QIconEngineFactoryInterfaceV2`.
2. If no actual created private state, getter `0x650a1d10` for legacy owner global0x659cb658, IID `com.trolltech.Qt.QIconEngineFactoryInterface`.
3. Each lazily constructs QFactoryLoader with QString `/iconengines`, case-sensitivity0; publishes through atomic compare/exchange, disposes losing allocations, and registers global cleanup once. These are process-global owners, not per-Study or per-file defaults.
4. QFactoryLoader.instance(suffix) returns a QObject; interface query and plugin create may return null independently. Null V2 can continue legacy; null legacy can continue default engine. A nonnull result changes the selected ownership path. No plugin result is proved from suffix `.png` alone.
5. QIconPrivate32-byte/global-counter/engine pointer installation, possible legacy ancillary ref owner, and final engine virtual+0x28 occur after these factory decisions. A successful plugin object is not itself the final QIcon/graphic-item owner. Ordinary void addFile has no public Boolean load-success check.

These captures qualify native branches but not the actual current global loader/plugin/handler state. A fake empty registry, supplied generic callback or filename-only representation cannot enable the public receiver route. Actual backend/graphic/old-icon ownership remains an independent later dependency.

## Failure/unwind separation

QFileInfo outer ctor FuncInfo has one state0 covering its private-constructor call (`0x670b9a29` until0x670b9a3d). Cleanup funclet `0x6717b5e0` frees its raw0xe0 allocation if that constructor unwinds. Its wrapper was not yet published.

Private constructor FuncInfo distinguishes normalized temporary and file-entry construction. At engine-selection call `0x670b808e`, state2's cleanup `0x6718a060` destroys file-entry storage at private+8 via `0x670a75d0`, then outer construction cleanup frees the raw private block. Reference/engine publication and later cache initialization have not completed. A source guard that stops there must not claim a live normally constructed QFileInfo; retained diagnostics may preserve the reached prefix, but are not proof that native raw allocation survives an actual constructor exception.

QFileInfo destructor is split across retained unwind entries: head `0x670b9bd0` checks/decrements the actual private ref; tail `0x670b9be2` calls private destruction/free on the last reference, then branches to the common epilogue. Capturing only the first18-byte entry would miss that cleanup. Neither entry alone is a complete standalone logical destructor, so this report uses their combined path without treating .pdata boundaries as whole-function proof.

Keep this separate from CP78's enclosing QIcon: its shell has already returned from its own constructor; its caller has only resource QString cleanup around addFile and no completed-QIcon delete on this later failure. Cleaning up QFileInfo's partially constructed private state therefore does **not** reclaim/reset the enclosing QIcon shell.

Further QFileInfo/file-engine/loader descendants may have additional cleanup/exception behavior. No general all-or-nothing rollback or engine-decode failure result is inferred from the selected FuncInfo maps.

## Smallest source-closed successor and guard

The minimum independent mutation is a **partial QFileInfo private construction record**: admitted nonnull0xe0 allocation, reference0 store, then stop before `0x670b8059` (QDir::fromNativeSeparators and actual QString reference ownership). All other bytes remain unavailable; wrapper publication/ref1, engine+0x50, metadata cache fields, suffix, plugin result and QIcon.d remain unmodified. This is more precise than inventing a full QFileInfo or zero-filling its storage.

If implementation is desired after CP78 freeze, keep it in new isolated helper/test files, with a caller-owned diagnostic construction arena and explicit unreturned-constructor stage. It must distinguish adapter retained allocation diagnostics from native construction-failure raw-free semantics. It cannot expose a QFileInfoConstructed/addFile-returned token. A public call remains guarded at the existing addFile boundary until actual owner integration is assigned.

The next useful larger slice would model the actual input QString lease/no-backslash copy, file-entry QString shared-null lease and cached words, then stop before `0x670f0080` / first custom file-engine handler. That requires real shared-data reference owners, not a copied std::string or a QByteArray stand-in. The known fixed path makes the value branch simple but does not establish those owner mutations. Root should choose this larger slice only if its QString/runtime state interface can carry the proven references and cleanup order.

Focused contract tests for any successor must verify allocated-size/ref0 only, unavailable tail fields/outer wrapper, same original shell unchanged, explicit next guard, no invented suffix/plugin/default engine, and separate diagnostic retention versus simulated native constructor-unwind cleanup. Later QString tests must distinguish0x672f0b30 from byte-array shared null and preserve no-replacement reference semantics. No tests were authored/executed in this lane.

## Evidence receipt

Original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; QtGui SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. Existing original GUI hash and CP78 shell/import/unwind evidence remain unchanged.

New bounded dumpbin evidence:16 selections totaling65,321 output bytes. Views were limited to the QFileInfo/path/handler/factory and selected cleanup bodies; this is not a full Qt engine reconstruction. Compact receipts identify exact ranges/commands/capture hashes:

| Receipt | SHA-256 |
| --- | --- |
| resume79-doe-fileinfo-static.json | C049EE4828DA8E69A485DB8E1AD0BC711A316679ED3F0B6562637754D6B98C45 |
| resume79-doe-icon-factories-static.json | 89A8E3E3694DAE1AAC2DD5FD96277DE6E1CD5471EB89FC7443B88BCAEC39514E |
| resume79-doe-fileinfo-private-static.json | 244B5631149C9A35D2C6669D44B43B8F52FD32712BAE6DEF412D2D3B90332D3E |
| resume79-doe-fileentry-static.json | 599DC94C77EA6888C4C7A4C9E84DFF471F02F34DDFCE7A069F0A26DB5F5394C1 |
| resume79-doe-fileengine-static.json | 0AF80BCBB6C6043795FB84648682AED0FDC9CD79DE6332EF2DD136F98519F69A |
| resume79-doe-fileengine-handlers-static.json | 8E7061669501DBD063411ECB78BD642285E515E1E6ADFCB188547D19286220DC |
| resume79-doe-fileinfo-unwind-actions-static.json | D0DF4E779A0CAC1FA790B890147F903B825E8F386664DF44AB02CCA5C5BABE9C |
| resume79-doe-icon-imports.json | 222B047E8F0AD6179FF322C51E20DABFA0E946275F4AEF38C796BEAF017C6450 |
| resume79-doe-fileinfo-unwind.json | DE9FBE74C5AEE958B6E79E230ABE450CA434AFB976947FC8CC8A00096E5BA9B6 |

All artifacts are under analysis/. No source, executable test, public integration, runtime plugin/resource result or native failure parity claim follows from this report. CP78 source remains held for root's freeze.
