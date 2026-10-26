# Checkpoint 66: ReportList signal-to-getter bridge remains incomplete

## Known static pieces

The ReportList handler packages a QString selector `SfTaskReportGenerate` and a `SfTaskBase *` output slot, then enters `FUN_1402cf740` with global dispatch state `0x143d76700` and method literal `2signalGetTaskBase()`. The original Qt import identity for the final indirect call in that wrapper is `QMetaObject::invokeMethod` (CP57 import receipt); the wrapper's return byte is not the returned task pointer. The handler later reads the generic output slot and, if non-null, calls `+0x218` on that pointer.

Separately, `SfMoMainWindow` meta record 17 maps `slotGetTaskBase(QString)` to `FUN_14003c630`. The getter returns the stored pointer under the key from a lazily obtained container at `this+0x4a0`, writing it through Qt's return argument. This path is established in [resume65-gettaskbase-dispatch.md](resume65-gettaskbase-dispatch.md) and its bounded captures.

## What the wrapper establishes about the actual target

The new bounded wrapper slice [resume66-gettaskbase-wrapper-dispatch.asm](resume66-gettaskbase-wrapper-dispatch.asm), SHA-256 `6F5CA442AB4EA0331C5CD5EEE37B005B52D63590A942E79F484122423E740167`, shows that `FUN_1402cf740`:

1. Reads suppression byte `global+0x141`; nonzero takes the early exit.
2. Resolves state through an object member at `global+0x88` and helpers including `FUN_1402d03f0`.
3. Unwraps an object-like value from a prepared record using `FUN_140c1aef0`, builds the generic argument pointer array, and calls the Qt invokeMethod import with connection value `1`.

So `0x143d76700` is dispatcher state, not proven to be the QObject receiver itself. The QObject passed to invokeMethod is resolved through the wrapper's record/registry path. The bounded resolver capture [resume66-signal-registry-resolver.asm](resume66-signal-registry-resolver.asm), SHA-256 `4FAFBF1884B3BC92AA971DF11B9FED4E61AF2B4034F341766DE106FE3529065E`, shows `FUN_1402d03f0` obtaining an internal object/table view, searching with a supplied key, and returning a pointer derived from that entry. The helper that retrieves the entry and the registry value's meaning remain unresolved; the slice does not name the receiver class or method implementation.

The constructor connection at `0x140011fa7` registers the global signal spelling `2signalGetTaskBase()` against receiver `param_1` with slot spelling `1slotGetTaskBase()` (bounded capture `resume64-mainwindow-gettaskbase-connect.asm`). Both spellings are no-argument signatures. The handler's generic payload includes a QString key and a typed `SfTaskBase *` return. The `SfMoMainWindow` metadata separately has the QString overload returning `SfTaskBase *`. Existing evidence does not show the dispatcher selecting that QString overload, converting the no-argument registration into it, or invoking it on the same `SfMoMainWindow` object. Do not equate the matching names with a closed bridge.

## Map insertion and dynamic class

The mapped getter proves only that a `SfTaskBase *`-typed result is loaded from an entry keyed by the requested QString. It does not inspect the entry object's vtable. Meta-call case 18 maps `slotCreateTaskBase(QString)` to `0x1400db573`, which calls `FUN_14003c6b0`; the inspected slice shows that this helper obtains the same `+0x4a0` container and delegates to another container helper. It does not yet prove which constructor/factory inserts the object for key `SfTaskReportGenerate`, or what vtable that value has. Therefore the static ReportGenerate vtable entry at `+0x218` remains a candidate, not an established active ReportList target.

## Exact unresolved target plan

To close the bridge without runtime evidence, the next bounded static work is:

1. Trace `FUN_1402cfbe0`/its registration record for the `signalGetTaskBase()` → `slotGetTaskBase()` connection and compare the stored receiver/method key to the registry lookup used by `FUN_1402d03f0`.
2. Resolve `FUN_1402d03f0`'s direct entry-value helper and show the exact QObject/method pair selected for this emitted signal.
3. Trace the selected method's meta-call signature. If it is the no-argument slot, establish where the QString key is consumed; if it is `slotGetTaskBase(QString)`, establish the overload-selection route.
4. Follow the creator/insertion path (`FUN_14003c6b0` and direct map/factory callees) only far enough to bind `SfTaskReportGenerate` to a concrete constructed object's vptr.

Until those links are established, do not claim that ReportList receives the result of `SfMoMainWindow::slotGetTaskBase(QString)` or that the result is dynamically `SfTaskReportGenerate`. Keep public DOE activation guarded. Static PE evidence only; no source/build/GUI/runtime/mutable REA changes occurred. Original PE SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
