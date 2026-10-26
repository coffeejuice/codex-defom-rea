# Checkpoint 65: DBLIST native global fields and Open lifecycle

## Findings

The two DBLIST global reads have different status:

| Field | Static owner/evidence | Fresh native process / successful Open | Candidate Session mapping |
|---|---|---|---|
| `0x143d766b8 + 0x0c` | The process-global application state object; `0x1402cb780` stores a caller-provided DWORD and `0x1402cb7a0` reads it. Retained Open/Close evidence identifies it as the application process type. | The global lies in the PE `.data` zero-fill tail initially. After a successful native project Open, the retained path selects process type `5` when the current Project's native-open success bit is set. DBLIST's later warning check suppresses only the warning when this field equals `5`. | Not a per-`ConversionSession` property. CP64's `actualSession` binding establishes candidate owner identity only; it does not carry the application process type or establish a candidate equivalent. |
| `0x143d766b8 + 0x1c` | `0x1402cbec0` admits DBLIST exactly when this DWORD is zero. A sibling predicate reads the same field; `0x1402cbf30` writes `1`. The field's domain meaning, full write/reset set, and the runtime owner that invokes the setter remain unresolved. | This DWORD is also in the PE `.data` zero-fill tail initially, so a newly loaded image starts it at zero. That is only the pre-execution image state. The retained Open path does not establish its value after application initialization or after opening a new project in an already-running application. | No retained source binding maps it to `ConversionSession`, `Project`, `SessionImportContext`, or `SessionNotificationState`. The CP64 raw Session pointer cannot represent a process-global field. |

The original `+0x0c` warning state is therefore now identified, correcting the narrower statement in `resume65-dblist-admission.md` that its meaning was unmapped. It is an application process type, not a Session notification flag. The caller-byte trace in that earlier report remains: the recovered DoeOutput route supplies zero as DBLIST argument 6, allowing evaluation of the separate `+0x1c` predicate.

## Storage and write evidence

The retained bounded PE capture `analysis/resume65-dblist-global-owner.asm` contains the direct bodies:

* `0x1402cb780`: read DWORD from its second argument and store it at receiver `+0x0c`.
* `0x1402cb7a0`: return DWORD at receiver `+0x0c`.
* `0x1402cbec0`: return true exactly when receiver `+0x1c` equals zero.
* `0x1402cbf00`: return true exactly when receiver `+0x1c` is nonzero.
* `0x1402cbf30`: store DWORD `1` at receiver `+0x1c`.

These are receiver-relative helpers. The callers pass `0x143d766b8` in the
already-retained process-type/Open and DBLIST call chains, tying the relevant
receiver to that application-global address. The setter body at `0x1402cbf30`
does not reveal its semantic name or establish its caller's lifecycle. No
zeroing write to `+0x1c` or call from the native Open path is qualified by
this evidence.

The original PE's `.data` section has virtual address `0x354c000`, virtual
size `0x1b09344`, raw size `0x82a400`, and raw pointer `0x354a000`. The global
RVA is `0x3d766b8`, offset `0x82a6b8` into `.data`, which is `0x2b8` bytes
beyond the section's raw data and within its virtual size. The loader
therefore zero-fills this tail; both DWORDs begin as zero in a newly loaded
image. This does not prove they remain zero through static initialization,
window startup, or subsequent project Open/Close transitions.

The retained `wave3-normal-close-guard-contract.json` closes the `+0x0c`
lifecycle on successful native Open: `FUN_14002a0e0` calls
`FUN_140013d40(main,0,0)`; that helper resolves the current Project and, when
the Project's native-open-success flag at `+0x55` is true, stores process
type `5` through `FUN_1402cb780` at its `+0x0c` field. The same receipt
qualifies that this application process type is distinct from
`MoInputMode`. Thus an original fresh successful Open can change the
initial-zero field to `5`; the warning path will then skip its dialog but
retain its false result and earlier manager effects. This is not evidence
that a candidate fresh `ConversionSession` should initialize a corresponding
field to `5`.

## B127 implication

The global address is application process state, while the CP64 pointer is
an engine conversion owner. A new candidate Session does not establish that
the original process-global `+0x1c` admission value is zero, nor does its
identity reproduce the `+0x0c` process type or native main-window QWidget.
For a fresh original process, the PE zero-fill gives an initial `+0x1c == 0`
and `+0x0c == 0`; successful native Open later sets `+0x0c` to 5 on the
qualified project-success route. For an already-running application, the
current `+0x1c` value cannot be derived from a newly opened Session.

Keep the public B127 DBLIST/ReportList refusal. To release it safely requires
the remaining `+0x1c` setter/reset caller and lifecycle, plus candidate-side
state owned at the same application scope (or an explicit runtime fact
provider with matching lifetime). The existing `actualSession` identity
binding is insufficient. No source or test changes were made.

## Bounded evidence and hashes

| Artifact | SHA-256 |
|---|---|
| Original `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| New bounded direct-body capture `analysis/resume65-dblist-global-owner.asm` | `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e` |
| Existing lifecycle qualification `analysis/wave3-normal-close-guard-contract.json` | `2c3c855180354f53810702b5e1cabfae0e9365e6b61f5f86902394e419a1bde5` |
| Caller/admission analysis `analysis/resume65-dblist-admission.md` | `e40f8e7eba7df191d5a1a28fd08966eaa2e2b67b82537361749a1bd0c9efe7a8` |

The new capture was produced with DUMPBIN 14.44.35228.0 from MSVC 14.44.35207
using only the two ranges recorded in its header. This was offline static
inspection; no source, build, test, GUI, project, or mutable REA database
was changed or run.
