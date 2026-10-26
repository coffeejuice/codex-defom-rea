# CP209 converter integration and successor

**Later qualification (CP212):** This report's B98 CLI crash used a literal
Windows path for `-f`. Exact-PE analysis now shows that the original CLI
expects four hex digits per UTF-16 code unit; the 34-character literal was
outside that decoder's valid domain and caused an out-of-range final decode
iteration. The captured scalar/fault behavior remains evidence for that
malformed command, not for a valid B98 Open or a public converter mismatch.
See `analysis/resume212-acceptance-static.md` and
`analysis/resume212-cli-decoder-independent.md` before using these findings.

## Recovered original behavior

The owned v11.0.2 `DEF_GUI_MO_64.exe` run at
`analysis/resume207-original-function-owner-2026-10-26T11-51-11-398Z/`
completed with the CP205 first-fault path: QtCore4_64.dll RVA `32c0`, read
address/RCX `0x58`, and the same callback return chain. Independent review
qualified the exact `1d510` normal return of zero in fault-active `eaf60`
invocation 2. The child exited naturally with `0xC0000005`; exact handles
closed, Windows and Frida found its PID absent, the 27-file fixture was
unchanged, and the original-run guard was restored to false. The pre-store
field read does not prove a committed store or select a deeper null route.

Copied-PE branch analysis, independently byte-checked, shows main forwards
outer loader `1ca30`'s RAX directly to `postOpen` and the captured
`2signalRefreshMainView()` site at `2a720` belongs to the **null incoming
model** arm. This corrects the earlier hypothesis that the CLI crash occurred
after a successful Open in an unconditional refresh. The outer loader's exact
null predicate is still unknown. CP210 static analysis identifies duplicate
lookup, factory, RTTI, Project-result and suffix-admission decision sites;
the last is not explained by XML-read failure alone. No original Save or
accepted Close was observed.

## Source integration and validation

`currentProjectForManager(ModelManager&)` now exposes the existing current-Mo
Project resolution before active-subproject resolution. It preserves the
current-list membership check, null current/failed Mo result, missing lazy
Project guard, backlink guard, and existing type4 composition. No public ABI,
postOpen callback, B98-specific behavior, or Save ordering changed. Focused
Release tests passed 45 cases in two executables, and independent source
review passed. The complete source tree hash is
`6cf73dc1f94258ba53cbbfd5195f12366ab5dc0e5f357077f83bc45efb50010e`.

The grouped candidate build completed with stable source and cache hashes
(`analysis/cp209-project-resolver-build.json`). A first sandboxed suite run
failed 13 tests only when creating disposable `C:\PROBLEM` paths; retained
logs show access denied before behavior assertions. The rerun with the
required fixture access passed: 361 registered, 354 clean, six logged gaps,
one logged diagnostic difference, zero failed/skipped/incomplete, no fixture
changes, stable source/binary bindings, 17.782 seconds of CTest time. Logged
gaps and the five historical differences were inspected and match the CP201
baseline. The frozen candidate is `build/wave3-checkpoint209-verified/`, with
validation at `analysis/wave3-checkpoint209-validation.json`: historical 93
SAME/five DIFF. This is an implementation checkpoint, not completion.

## Selected successor

Identify the first failed outer-loader prerequisite in the same original
invocation, then map its manager/factory/model state to the candidate. A
child-only Windows debug-event fixture passed initial natural paths but
independent review held direct original adaptation for debugger-state,
breakpoint, exception, cleanup and identity gates. It is being hardened.
A default-disabled function-hook alternative is being prepared separately.
Only a reviewed, bounded, exact-owned capture with CP205 first-fault parity
can promote a branch claim. The public converter continues to preserve its
guards until a corresponding original lifecycle path is established.

CP208 independent audits reconciled editor300, active DOE, Boolean timing,
format, geometry, operation dispatch, graph, IO, Close, Cogging, dialog and
ownership lanes. They found no further evidence-backed source increment in
those lanes. Their individual `analysis/resume208-r*-next.md` reports give
the smallest missing witness rather than treating old audit entries as new
defects. `python admin/check_continuation.py` still reports 15 unfinished
workstreams and eight pending acceptance gates.

## Evidence

- `analysis/resume209-capture-independent-review.md`
- `analysis/resume209-headless-entry.md`
- `analysis/resume209-outer-loader-independent-review.md`
- `analysis/resume210-loader-null-static.md`
- `analysis/resume209-cpp-action.md`
- `analysis/resume209-source-review.md`
- `analysis/cp209-project-resolver-build.json`
- `build/test-runs/1793016233783848600/receipt.json`
- `analysis/wave3-checkpoint209-validation.json`
