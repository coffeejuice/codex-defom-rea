# Checkpoint 66: DBLIST globals across native Close and Open

## Decision

No safe per-Session admission value follows from the Close/Open evidence.
The process-type DWORD at `0x143d766b8 + 0x0c` is recalculated by the native
application's mode-0 state refresh; it is not reset to zero as a new
`ConversionSession` is created. The `+0x1c` DBLIST admission DWORD begins at
zero in a newly loaded PE, but the helper that writes `1` has no retained
qualified caller and no Close/Open reset is established. A Session opened in
an already-running process therefore cannot infer either field from its own
identity. Keep the public B127 DBLIST/ReportList refusal.

## `+0x0c`: process type is recalculated, not cleared

The retained direct body of `FUN_140013d40` reads the current global process
type through `FUN_1402cb7a0(0x143d766b8)`, performs its mode-0 handling, then
stores a selected value through `FUN_1402cb780(0x143d766b8, &value)`. The
mode-0 outcomes established by `wave3-normal-close-guard-contract.json` are:

* if there is no current Project, store process type `5`;
* if a current Project exists and its native-open-success byte at `+0x55` is
  true, store `5`;
* if a current Project exists but that byte is false, store `1`.

The normal native Open registration path calls
`FUN_140013d40(main, 0, 0)` from `FUN_14002a0e0`. The successful native
`.moproj` load path sets the Project byte at `+0x55` from the native reader's
success result before the post-Open registration. Thus the qualified
successful Open route refreshes process type to `5`; it does not preserve a
fresh-image zero. The warning-mode leaf reads this same `+0x0c` value, so
value `5` suppresses only the DBLIST missing-path dialog. It does not change
the failed result or undo earlier DBNAME registration effects.

The retained normal-Close trace also shows a post-manager-clear call to
`FUN_14002a0e0`, which reaches the same mode-0 refresh before the later
false-busy setter and final status reporting. If the current-Project lookup
at that moment is null, the refresh stores `5`; if a Project remains current,
the two Project-dependent branches above apply. The retained trace does not
authorize replacing that lookup with an assumption about a newly created
candidate Session. In either case, there is no unconditional `+0x0c = 0`
reset in the qualified Close tail. The subsequent native Open performs its
own state refresh and may store `5` or `1` according to the loaded Project.

This field is the original application's process type, separate from
settings `MoInputMode`. It has no CP64 binding to
`SessionImportContext::actualSession` or `SessionNotificationState`.

## `+0x1c`: fresh-image zero is not a Close/Open contract

The receiver-relative helper bodies show that:

* `FUN_1402cbec0` admits DBLIST exactly when the DWORD at receiver `+0x1c`
  equals zero;
* its sibling `FUN_1402cbf00` returns true when that DWORD is nonzero;
* `FUN_1402cbf30` writes the constant `1` to receiver `+0x1c`.

The original PE static scan covered all `.text` raw bytes (`RVA 0x1000`, raw
offset `0x400`, raw size `0x1e01400`) for direct `E8` call and `E9` jump
references to `0x1402cbf30`; it found none. A raw-file search for the exact
64-bit code pointer also found none. This is a bounded static negative result,
not proof that no indirect/dynamically formed caller exists. No retained
Close/Open capture identifies a caller of this setter, a zeroing setter for
`+0x1c`, or a condition that changes it back to zero. The setter's receiver
therefore cannot be tied to a specific lifecycle event from available
evidence.

The PE section map independently establishes initial storage only: global
RVA `0x3d766b8` falls in the virtual zero-fill tail of `.data`, `0x2b8` bytes
beyond its raw-data extent. On initial image load, both `+0x0c` and `+0x1c`
are zero. This does not guarantee the `+0x1c` value after startup, Close, or a
later Open in that process. Nor does it make the initial zero a safe
candidate-side constant for a new Session within a long-lived process.

## Candidate boundary

CP64 `actualSession` verifies the candidate's existing
`StudyIo -> ModelManager -> ownerSession()` identity. That object is scoped to
one conversion and cannot stand in for the original global application
manager whose process type and `+0x1c` gate survive individual project/session
transitions. Candidate `SessionNotificationState::suppressed` is not either
native DWORD. There is no source-supported owner or setter path that can
provide their occurrence-specific values on public B127 Open.

To reconsider DBLIST admission, next evidence must recover the actual
`0x1402cbf30` caller/receiver and all relevant `+0x1c` writes or establish a
complete application-scope state provider. For `+0x0c`, a compatible
application-state input must follow the Project/process-type refresh contract
and preserve its separate role as warning-only suppression. Until then no
candidate default, including zero for a newly opened Session, is justified.

## Evidence and qualification

| Artifact | SHA-256 |
|---|---|
| Original PE `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `analysis/resume65-dblist-global-owner.asm` — bounded getter/setter bodies | `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e` |
| `analysis/wave3-closure-next-140013d40.asm` — process-type read, selection and stores | `d581ce0d292e03e3433cdcf8bd2e477bf75b7cdf0d93e1538803f090c7139d10` |
| `analysis/wave3-boolean-external-callback-14002a0e0.asm` — Open registration call | `e3e74166115892e04fe23160955278cf3101abacb6d818cd41746bca94c43f63` |
| `analysis/resume62-close-global-reset.md` — Close path ordering and limits | `da0a6d265eb4c8e0d3e0581816a0ffa720bc294fc64ef7c8f0ffbf5e85941f29` |
| `analysis/wave3-normal-close-guard-contract.json` — process-type lifecycle | `2c3c855180354f53810702b5e1cabfae0e9365e6b61f5f86902394e419a1bde5` |

This was offline static PE/evidence review only. No source, build, test, GUI,
application, or mutable REA database was changed or run.
