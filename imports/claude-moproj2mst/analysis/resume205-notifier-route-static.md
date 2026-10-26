# CP205 static: postOpen notifier and callback route

## Finding

The successful same-run crash capture is consistent with a synchronous callback dispatched by the `postOpen` `2signalRefreshMainView()` notification. The captured stack places callback-side application frames under Qt's `QMetaObject::invokeMethod`, then returns through the notifier and `postOpen` call sites. Its `postOpen` continuation is still on the stack, so this is an in-progress Open path, not evidence that Open completed.

This is a static-to-runtime correlation, not proof of which registered receiver/method was selected or that the notifier returned. The retained stack does not identify the exact slot, receiver object, or callback effects.

## Source identity and method

Target: copied shipped `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Independent read-only disassembly used Microsoft DUMPBIN 14.44.35228.0 on the copied PE. The crash facts are from `resume205-original-crash-capture-qualification.md` and the same-run raw summary in `resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json`; the target was installed v11.0.2, but the copied PE's version metadata was not separately queried here.

## Instruction path

1. In `postOpen` at preferred VA `0x14002a0e0`, `0x14002a719` loads the notifier singleton address `0x143d76700` into RCX, `0x14002a712` loads key address `0x142100170` into RDX, and `0x14002a720` calls notifier/dispatcher `0x1402cf3c0`. Reading the PE's mapped `.rdata` bytes at `0x142100170` yields the NUL-terminated key `2signalRefreshMainView()`.
2. In the dispatcher `0x1402cf3c0`, `0x1402cf3f1` reads suppression byte `+0x141`; a nonzero value branches to `0x1402cf72e`. The unsuppressed path snapshots the registered receiver list: `0x1402cf454` calls list lookup `0x1402d03f0`, `0x1402cf461` copies its result to a stack-local list, and `0x1402cf47a` calls `0x1400004ae0` to obtain the copied list size. The size-return continuation is `0x1402cf47f`.
3. The registration loop skips dead/null receivers on its branch to `0x1402cf6e3`. For a live receiver, the dispatcher prepares invocation arguments; `0x1402cf6bd` sets `R8D=1` (Qt `DirectConnection`), `0x1402cf6c6` calls the imported `QMetaObject::invokeMethod`, and `0x1402cf6cc` stores its Boolean result. It does not branch on that stored result before continuing cleanup at `0x1402cf6d3`. Thus dispatcher return alone does not establish slot completion.
4. If `invokeMethod` synchronously enters a receiver method, its application frames are nested beneath Qt's meta-invocation frames. The capture's app return RVAs `0xdafad`, `0xeaa7a`, and `0xeb1ec` are on that callback side of the stack. This bounded investigation does not assign them slot names or analyze the separately assigned `0xa7377` / `0xa7510` owner route.
5. Dispatcher cleanup reaches `0x1402cf724`, destroys the local snapshot, and returns at `0x1402cf737`. In `postOpen`, `0x14002a725` is the continuation immediately after the notifier call at `0x14002a720`; if dispatch returns, `postOpen` continues its later work.

## Reading the captured return addresses

The backtrace entries are return addresses, not function entry addresses and not the call instructions themselves. This distinction gives the following direct mapping:

| Captured app return RVA | Static callsite immediately before it | What it establishes |
|---|---|---|
| `0x2cf6cc` | `0x2cf6c6`: indirect call through IAT `0x141e05658` to `QMetaObject::invokeMethod` | The notifier's invocation call was active on the unwound path. It does not identify the selected method. |
| `0x2a725` | `0x2a720`: direct call to dispatcher `0x1402cf3c0` | The `postOpen` RefreshMainView notification was the enclosing dispatcher call. |
| `0x1cae` | `0x1ca9`: direct call from CLI main to `postOpen` `0x14002a0e0` | The outer CLI frame was waiting for `postOpen` to return. This return address does **not** mean the return had happened. |

The sequence `0x1c94` (loader return) → call at `0x1ca9` → `postOpen` → notification at `0x2a720` occurs before the `postOpen` return continuation `0x1cae`. The independent CLI static-semantics review places selected startup-branch completion later at `0x1e4e`. Therefore the crash is before completion of the CLI Open/postOpen path. It is not enough to claim the project loader failed, nor enough to claim the project Open completed: the captured stack shows execution inside a later `postOpen` notification, while the exact project-state transition and callback target remain unresolved.

## Evidence limits

- The same-run capture qualifies PID, first AV and accurate backtrace. Its crash is at QtCore `QListData::size` RVA `0x32c0`, read address `0x58`; see `resume205-original-crash-capture-qualification.md` for capture identity and runtime limitations.
- Static disassembly establishes the notification key, list-snapshot/invocation sequence, branch conditions and call continuations. It does not establish the live suppression byte, receiver snapshot contents, selected method, receiver identity, or normal return from any callback.
- App callback frames plus nested Qt meta-invocation frames support synchronous receiver execution on this stack. They do not by themselves prove successful slot return or the meaning of the app frames.
- This report makes no claim about the separately scoped `0xa7377` / `0xa7510` owner path, and no source, test, build, runtime capture, or GUI-control action was performed.

## Supporting retained material

- Fresh bounded DUMPBIN output was read directly from the copied PE for `postOpen` `0x14002a6d0..0x14002a735` and dispatcher `0x1402cf3c0..0x1402cf738`.
- `resume64-boolean-event-route-1402cf3c0.asm` retains a prior full dispatcher disassembly of this same copied target; `resume204-crash-candidate-slice.md` records the startup notifier/list-snapshot candidate, with its static-versus-runtime qualifications.
- `resume203-cli-open-static-semantics.md` distinguishes loader return `0x1c94`, `postOpen` return continuation `0x1cae`, and selected CLI branch completion `0x1e4e`.
