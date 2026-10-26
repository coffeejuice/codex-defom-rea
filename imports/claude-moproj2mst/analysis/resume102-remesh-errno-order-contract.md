# CP102 remesh ordinary real-conversion errno ordering

A broader public-reader state transition is source-qualified without admitting
another numeric spelling: successful ordinary real conversion must preserve
the preceding native errno. Current `originalReal()` clears errno to detect a
new host range error, then leaves that temporary clear visible on success.
Native's successful finite/no-digits wrapper does not clear errno. Integer
overflow followed by finite filter/scaling records therefore loses ERANGE in
the candidate even though all model fields continue correctly.

Only this report was written. No source/test edits, build/test, GUI/original
execution or mutable REA. Source ownership remains pending CP101 release and
root's agreement to the exact pair below.

## Native state and ordered counterexample

The normal valid-token/C-runtime profile is the existing reader profile. Its
kind10 path calls `1414759b8 ->14147583c`; the latter does not clear errno on
entry. It builds locale/endptr state, invokes `141486ccc`, and selects its return:

- invalid/no-digits flags240h return zero without an errno store;
- overflow81h or underflow-to-zero bit100h reach141475973/5978 and set errno22h;
- ordinary result flags take141475980 and return payload directly, preserving
  the prior errno. No resetting store precedes this selection.

The fltin wrapper does not write errno; its normal converter uses temporary
integer/stack state. Invalid-argument/locale/TLS and allocation failures remain
outside this existing normal adapter profile. The errno accessor itself is
141474134: current CRT thread-data+10, with its known fallback if absent.

CP98 already establishes kind5 signed overflow setting ERANGE34 and retaining
the saturated shared local. Consider the public input sequence:

    INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 2147483648
    FILTER_DISTANCE 7.5
    SCALING_FACTOR_EACH_LAYER_INWARD 2.25
    DONE

Native method becomes5, filter7.5, scaling2.25 and errno remains34 before close.
Candidate currently gets the same fields but clears errno to0 twice. This is
an ordered shared C-runtime state difference on the existing actual Session
remesh reader, not a new page-helper profile. The successful close/outer API
may have its own effects; do not claim a final M2M status or arbitrary later
caller errno from this local proof.

## Minimal production contract

In the existing ordinary fallback only:

1. Save entry errno immediately before the temporary clear/host strtod call.
2. Keep the temporary zero so current-call ERANGE is detected independently
   of a prior ERANGE. Preserve the existing finite/range admission checks.
3. Only after those checks accept the value, restore entry errno and return it.

Restore after checks, not before: a new current-call range error must remain
an unsupported current conversion and must not be hidden by the saved state.
On an admitted finite result, restoration is unconditional; optional host-only
nonrange errors must not replace the source's untouched errno on this profile.

Do not change numeric values, fields, loop/cursor/close order or profile
admission. CP99/100 canonical range branches still set34 and return before
this fallback. CP101 prefix-zero branch still preserves errno directly.
Unqualified nonfinite/range cases keep their existing reached guard behavior.

Exact proposed exclusive files:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_errno_order.cpp`.

No header/CMake/old-test amendments are expected: current tests check values
and range errno, but none requires success to clear a prior error. Root owns
build and any acceptance matrix lane. No source write is requested before
the freeze and ownership agreement.

Focused cases: a preset EDOM persists through finite filter/scaling; CP98
integer ERANGE persists through subsequent finite records; the already-admitted
no-digits0 path preserves entry errno; a new unsupported host range error
overwrites prior EDOM and retains the prefix; and actual Session's
loadProjectRemesh dispatcher uses the same owner and preserves the error at
the reader's close/return boundary. Keep assertion scope explicit; do not rely
on errno after unrelated project/GUI operations or infer an ABI error mapping.

## Reader/Save audit reconciliation

The larger ordinary Save sequence already matches retained source:31 live-field
fprintf calls, ignored write/close results, then root-owned post-MST error
mapping. Disabled removal is unconditional and ignored. CP98's combined denied
remove/earlier writer-false matrix is a useful acceptance test, not a missing
production branch. Failed enabled fopen remains an invalid-parameter boundary;
the actual handler/default terminal outcome is not newly qualified here.
No Save ordering change or terminal handler emulation is proposed.

## PE receipt

Imported original EXE SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Verified DUMPBIN14.44.35228.0 SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Preferred-VA half-open ranges:

| Range | SHA-256 |
|---|---|
| `14147583c..1414759b6` | `03e0e1ba872019abe31f370b68e4c6f2355da8cb31e69f8c2f7b1ff4b634ab79` |
| `14147592a..14147599c` | `c713d3212435672fad4bcacb70e0135db4518c3fd22449b8a4f925a72a19cc68` |
| `141486ccc..141486d95` | `54916ad926e930ab4851f1ad09aaf3cc781a4d93eb200cf51b11c47c10f025f3` |
| `141474134..141474154` | `82fd11be9f0076f075cafa1b9e5c1466415a7f017d285cda92bb723d53800482` |

`wave3-remesh-save-lifecycle-contract.json` SHA-256
`dd596017d3cf2b0cde72a666ae7e4046080f2baf3816daa03cd6b066979d28c6`
and CP98 terminal matrix SHA-256
`09c911018153efd3e2d6b2a48ce3042fb136525961c6a4130b341340b300121c`
retain the independent Save qualification. No new raw artifact was written.
