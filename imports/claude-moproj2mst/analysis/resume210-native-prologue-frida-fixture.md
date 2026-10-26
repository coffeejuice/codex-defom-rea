# CP210 exact-prologue native Frida fixture — PASS within observed five-byte patch mode

Eight fresh disposable-child trials passed with exact copies of all six pinned original entry-prefix byte strings in valid synthetic x64 functions. The all-success run exercised every prologue and epilogue. Native checks established stack, selected register and return-value preservation before the deliberate AV. The handler-only baseline and all six hooked paths reached the same AV module RVA, operation and address, naturally exiting with `0xc0000005`. All eight owned children are verified exited, handles closed and absent through Windows and Frida checks.

**This qualifies the observed five-byte Frida entry patches in the synthetic image. It does not qualify an original fourteen-byte fallback, original shared RTTI semantics, original module placement or original fault-path neutrality.** Both original runner guards remain false; its SHA-256 remains `41c2cd035bd023c3728b1dc73bb8ef167d13ae644bd11f4b1ecfcfc61790351b`. No original or GUI was launched, attached or controlled, and no product executable was rebuilt/deployed.

## Fixture and exact byte shapes

New files are `admin/build_outer_loader_native_prologue_fixture.py` (SHA-256 `19dd07408f64cd9696f83b7f2f4d329a322fe2ca900442a3a173505053a70ee6`) and `admin/test_outer_loader_native_prologue_frida_fixture.mjs` (SHA-256 `d03dd71f0916d07108ac55d4263c2240945f49b1f3ec6b139e74363fba7d7b35`). Generated C/MASM, metadata, compiler commands and logs are retained in `scratch/cp210p01`. Synthetic `fixture.exe` SHA-256: `6f1ab2fa4bdf47a80ceda881f293e522333fae551a8c30a9e1d15600b473a83c`.

Each prefix is emitted as literal bytes, followed by a valid synthetic body and matching epilogue. An independent offline PE-section audit compared every prefix to the pinned original image SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. That image belongs to the original installation v11.0.2 DEF_GUI_MO_64.exe, whose retained version-resource reading is null; installation version is distinct from a version-resource value.

| Site | Copied complete prefix | Bytes |
|---|---|---:|
| outer `1ca30` | save RDX/RCX, allocate 1d8 | 17 |
| duplicate `c78c10` | save RDX/RCX, allocate b8 | 17 |
| factory `c79390` | save RDX/RCX, allocate 48 | 14 |
| RTTI entry shape `146f8fc` | save RBX/RSI, push RDI/R12/R13/R14/R15, allocate 60 | 23 |
| Project `44a90` | save RCX, allocate 88, store qword -2 at RSP+48 | 21 |
| acceptance `8a9b0` | save RDX/RCX, allocate 168 | 17 |

The synthetic RTTI-shaped function executes these instructions and restores registers/stack, but performs no runtime type conversion. It cannot reproduce unrelated callers, type metadata, exceptions or shared-runtime contention. The Project immediate store executes in valid local storage. No original function body is executed.

## Native execution and state checks

The fixed runner accepts no target or PID arguments, verifies the fixed synthetic hash and unchanged owned-helper hash, creates only a fresh helper-owned suspended process, and uses pinned Frida 17.23.2. It extracts the actual prepared runner's injected callback code, replacing only synthetic identity/module size and hook/caller RVAs/prefixes. Extra fixture diagnostics read code and decode visible branch targets. The baseline disables all six Interceptor attachments while retaining the same AV observer.

The native wrapper saves its caller's nonvolatile registers, installs distinct canaries in RBX, RSI, RDI, RBP and R12–R15, saves its RSP, and calls the outer with RCX=700000 and RDX=7018d8. After the outer returns it checks RSP, all eight canaries, RCX and RDX, then restores the original registers. A failed state check exits 77 before the deliberate AV. C separately checks the expected outer return (zero for each failed prerequisite, 100h for all-success). Thus each observed natural AV implies both sets of checks passed. The actual observer also requires the helper-entry stack relation and records raw normal returns.

Trials were: handler-only all-success baseline; duplicate, factory, cast, Project and acceptance failures; hooked all-success; and one intentional failure after hooks-ready/before-resume. All seven normally resumed children exited naturally with `0xc0000005` and no termination attempt. The injected child was never resumed, emitted no callbacks, and cleanup verified one owned termination with exit `0xe201`. Every trial retained textual creation FILETIME, primary TID, exact image/argv identity, raw helper JSONL, generated agent, raw Frida messages and independent absence receipts.

All normal AV cores identify `fixture.exe+10ae`, operation `write`, address `58`, and the exception observer's `return_false_continue_natural_exception` disposition. This is handler-only versus hooked first-AV parity inside this synthetic image, not CP205 parity and not a no-Frida baseline. The native child disables WER/error dialogs before the test; no GUI-control API is used.

## Observed patches and accessible branch evidence

The raw receipt directory is `analysis/resume210-native-prologue-frida-2026-10-26T12-21-32-663Z/`. Each `native-patch-evidence` message retains 64 bytes before and after each entry, decoded entry instructions, visible branch target addresses/64-byte contents, and synthetic caller-continuation RVAs. Frida was flushed before reading patches.

Every hooked entry used a five-byte `E9 rel32` patch. The untouched suffix of each copied prefix exactly matches the pre-hook bytes. Each visible target starts with `push qword ptr [rip-0e]`, then `jmp qword ptr [rip+2]`. The read-only audit decodes both the E9 target and the indirect-jump target from the retained bytes and records these in `retained-evidence-audit.json` (36 installed entry patches across six hooked trials). All-success is the run that executes all six.

The public `Interceptor.attach` API exposes a listener, not Frida's private onEnter/onLeave/invoke trampoline pointers. The visible entry deflector and its indirect destination are retained; the private invoke trampoline and its actual resume instruction were not exposed by the public API and were not guessed from opaque Gum internals. Entry+5 is recorded only as an expected displaced-window continuation, explicitly unobserved. Exact exported caller continuations and normal helper/outer returns are observed separately.

Because only the first five-byte MOV was overwritten at each synthetic site, this run demonstrates relocation of that instruction and correct subsequent execution of the remaining exact prologue. It does not demonstrate displacement/relocation of the RTTI pushes, large stack subtraction or Project immediate store under a longer patch. The full exact prefixes are present and execute correctly, but a fourteen-byte patch choice remains a distinct untested condition.

## Checks and limits

Syntax checks passed. The native run returned PASS for all eight trials. Read-only audit of retained receipts independently verified exact original-prefix identity, callback ordering/count snapshots, exact helper callers, expected failure flags, primary thread/creation identity, no rejected callbacks, no state overflow and no hook/script errors. No automatic rerun or original launch followed.

The approved escalated run cited the user's explicit bounded disposable child-test authorization (“Yes, run the child test”). It needed access to the pinned Frida package outside the sandbox. Tool-reported execution wall time was about 4.81 seconds; no comparative speed or token-saving claim follows.

Original release still requires independent review of the actual installed-image allocation/patch choice, original shared RTTI effects, and fault-path parity under the proposed six hooks. This receipt narrows the native fixture gap while preserving those limits.
