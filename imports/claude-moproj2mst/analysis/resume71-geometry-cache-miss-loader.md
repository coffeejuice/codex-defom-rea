# CP71 cache-miss timer prefix and DB-loader partial effects

The next bounded cache-miss prefix can reuse the **actual existing node-state
StepTimer**: start("Start"), then checkpoint("ReadSingleAArrayFromDB",nullptr),
and stop before entering the DB loader. This preserves two real timer samples
and named checkpoints after CP69's COW/table effects, without enabling a successful
cache-miss getter. DB loading and model-object copies remain guarded.

The loader is not read-only preparation. It can change the actual model version
before an early gate return, allocate/publish an A body, and mark an array cached
without reading stored data when its length is zero. Do not make it transactional
or infer public success from a null companion DB.

Owner: `gradient_state`. Only this report was written. No source/build/test,
GUI or mutable REA action was performed. Bounded direct PE reads were retained
as hashes here, with no new sidecar. Token measurement is unavailable.

## Frozen source and evidence identity

CP70 viewer SHA-256:
`14b6668cd18a0d20ee2fa83c3b026e0a10213468bdf1c38152c3a8adbb5c9354`;
viewer-test SHA-256:
`a354b1f22f5db225b5e3c2ea57e875eb086b2e42a4a7cbc469a1de9cf5d2240d`.
Current cache0 returns pending after real COW/table selection and before timer,
loader or object-copy effects. CP70 cached normal return remains separate.

Original GUI target SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Target/tool paths remain the retained DEF_GUI_MO_64.exe and CP70 VS dumpbin,
tool SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Commands used `/disasm:bytes /range:START,(END-1)` on the absolute PE.

| Complete normal function/leaf | Bounds, end exclusive | Native-byte SHA-256 | Stdout SHA-256 |
|---|---|---|---|
| DB loader | `140828060..1408283d5` | `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796` | `74c9cfcba3b88ecbabba31c79b57b5efed63c25fb1e9a4594d49ffcf045c580f` |
| Directory query | `140825440..140825532` | `4525b4f95fe70043fd1c70d20acced398afb237fa0b8331a12d0da131cd00ac3` | `8e2dbca3f54429e674fb228dc096850b927d64e478e3e307102651615245afb5` |
| Model version setter | `140825400..140825419` | `4fed3c566735c55f1ee0b69e7b5a69ec269243e8cb5aa16df3ca49d81c1beb07` | `29262921da0c8168a521951963bbd0758a6c329a69e04c42721493929b750952` |
| Double allocator | `1409c83b0..1409c8408` | `2ca64c1da768dd96662135394af3e2d8621a13a257c78ab42d2ebb046ea00ef4` | `3422dfb5408607dc1e32fbae13ed30f7077b56d4f0739a091718c71312941a3b` |
| Timer start | `140832340..14083236f` | `b95eadaf0da18648f36a299657ca240864e02c8b065e407708bdf72224b9d4be` | `4bcd80ec9477567225bb0e8ed5fe983c19f7d6afe87ff4057256c3176e7820d1` |
| Timer checkpoint | `140832370..14083242f` | `4eab6c32d8813886e4dfa2ddd8dc39d0f24f4a8b55c4f1267c3c6b8ff7b18dc6` | `b773d2825ef2166ad25972cf410ea397197900f3d251c5a043ef1bc6a44dfd1b` |
| Timer reset leaf | `1408324a0..1408324b5` | `b6842557e998c9dc6afbdc6515f9404ff61ca87793e8471ab18250028f61b520` | `1a1ca1759d8feee0d9c6d981a98415edc63d9dcc3f795fdcff8ceb0b8168b132` |
| Timer clock conversion | `1408324c0..14083250e` | `7aee061c434ad52f2596aa43fad739bebd6f88be869c51394fd49d18addeafec` | `e5ba04e19b740fce4e8aa9c2e651146cb1f14f575f915fc1f5a4805ed43c0434` |
| Node-timer initializer | `141d32af0..141d32b22` | `dc5ed4d5417bf3652f61ec18d7ba63e12db51c7b61adc142797aa6a95ddc2087` | `97d9977abb506efd437cf7ab9dca94c474fab41303910dca467d4e2a2d5b42ba` |
| Arrays-timer initializer | `141d32a70..141d32aa2` | `1431c5c529589adebeecd19996510ade77ab348a42e751552314711e25fb228a` | `e00d5fa63c3a6ba0ded61f1010f1492e5668a5596c8c8d270edc0ee67a2051f2` |
| CRT clock body | `141475498..1414754d6` | `f73c2647aaf39428e6a2f784476787345bc179b8ec87a980214582e7bb741647` | `8fba893773103cd0e9a305433589031c9bfa55b447f092c139092f1f3e81f574` |

Nonleaf bounds were checked against PE unwind regions through normal RET;
version-setter/reset are complete leaves through RET. No exception-funclet
coverage or full body-copy callee recovery is asserted. CP70 retains the full
reader and outer which0 hashes that contain this call sequence.

## The former diagnostic objects are typed timers

Native initializer `141d32af0` constructs global143DBE000 with the literal
"_DB_STEP_DATA::ReadObjNodeStateVariableFromDB()" and flag0. This maps to source
`g_timer_read_obj_node_state_variable`. Initializer `141d32a70` constructs
143DCDAE0 with "_DB_STEP_DATA::ReadArraysFromDB" and flag1, mapping to
`g_timer_read_arrays`. These names/flags and constructor/reset layout match the
existing source globals; no new timer or assumed default history is needed.

Source qualifications:

- `include/defmodel/sdctl/step_timer.hpp` SHA-256
  `8a3fceb49f24342f1ff83b5b8a8ec211eb041bc366889c9c9c17411da454dcc9`.
- `src/sdctl/step_timer.cpp` SHA-256
  `f72dd675db7b7028afe957628497708e67d6b215eba32edbf643d96634afb136`.

After cache0, reader `14082d895` calls timer start with "Start", then `d8ab`
calls checkpoint with "ReadSingleAArrayFromDB" and sub=null. Start resets only
count at+5398, then records its checkpoint. Each checkpoint requires nonnull
label and signed count<100, samples the clock, writes low32 time at+4ee8+4*count,
copies the NUL-terminated label into+00c8+200*count, stores sub at+5078+8*count
and increments count. After start/first checkpoint the count is2. Name, flag,
old slots2..99 and other timer objects are not cleared. The flag does not suppress
these writes. Existing StepTimer methods preserve this order/layout; the short
literal labels fit their200-byte slots.

The native clock wrapper calls `141475498`, sign-extends its32-bit result,
divides/multiplies double by the retained1000.0 constant at1421D94E0, and truncates
to int64 before the checkpoint narrows low32. PE imports bind clock's IAT
141E03390 to KERNEL32.GetSystemTimeAsFileTime; its CRT body subtracts baseline
global1446FFB90 and scales to milliseconds. Existing StepTimer::clock_ms uses
the real host CRT std::clock and the same double conversion. Runtime clock
samples/epoch are external inputs, not constants or reconstructed zero values.
Reusing that existing clock profile is a semantic millisecond-clock adaptation;
exact cross-process tick equality or identical CRT epoch is not established.
If raw native-clock epoch parity is required, that remains a separate dependency.

After DB loader returns, reader `d8d6` records
"Assign the variable to the object data" on the **different** actual
g_timer_read_arrays, preserving its prior history/count. That later checkpoint
is outside the minimum pre-loader prefix. Do not reset that timer as a substitute
for reading its actual state.

## DB loader: ordered mutations and early exits

Let S be the actual step. The loader140828060 follows this order:

1. Read S+3390 companion DB. Null jumps to epilogue before directory/body/cache
   mutations. This is only this callee's early return, not full-reader success.
2. Call directory query140825440. It reads companion+ad8 record body at
   actual S+740 step index times0x28, and copies record fields+8/+c/+10/+20
   into stack-local outputs. No record-count/allocated-body bound proof was
   supplied in this lane; actual companion owner/body/index qualification is
   required before integrating it.
3. Query compares record+20 version with current S.step_version, and if different
   writes the record version through140825400 to S+1b64. This model mutation
   occurs **before** loader rechecks14082bf10 at828167. A newly old version can
   make that gate return true and exit while retaining the version change.
4. With gate still false, require id in0..279. Then S+1b68 and S+1b6c (MAR/A
   header lengths) must both be nonzero. Otherwise the native error-output path
   reports a read-before-step-header error and returns; no synthetic exception,
   header allocation or version rollback is justified.
5. Recheck actual S.u26c4[id]; nonzero exits. Otherwise, if S+1b78 A pointer is
   null, pass wrapped32(S.u2b24+2) to1409c83b0 and immediately publish its returned
   pointer at S+1b78. No loader-side allocation-result check precedes later steps.
6. If S.u2b28[id]==0, write1 to S.u26c4[id] at828265 and return. The A allocation
   therefore precedes this zero-stored-length shortcut; it is not rolled back.
7. A nonzero stored length continues through companion-path preparation and
   string conversion, then calls140b38d57 with model header/MAR/A regions and
   locals. That function is a substantial body, not an import thunk. Its DB-read
   behavior remains unqualified here. On normal return, loader unconditionally
   writes1 to S.u26c4[id] at8283b2 without testing the callee's result first.

Allocator1409c83b0 special-cases **n==0**, not n<=0. Other counts are sign-extended
and passed to its calloc-like callee with element size8. Null allocation invokes
diagnostic140b2f920; only if that diagnostic returns normally does the helper
return its stored null pointer. Do not invent overflow/negative-count safety,
throw/rollback behavior, or assume the diagnostic always returns. In particular,
the zero-count result can remain null and the subsequent zero-length shortcut
still marks cache1. These raw partial effects are qualifications for a later
bounded loader, not permission to enter allocation from the viewer now.

The directory/body owner is S.db (companion Cls_1401dbfe0), not GuiModel.db's
adapter handle merely because both names contain DB. Its directory storage and
actual step-index bounds must be supplied by the real companion owner; an opaque
nonnull pointer, fabricated header records or the geometry object's ownership
of M is insufficient.

## Model-object copy edges remain a separate dependency

After a loader early return or normal return, the outer reader still records
its arrays-timer checkpoint and loops to S.u1378 object count. It obtains each
destination through1408302a0(S,ordinal), skips null/kind7 and another object
predicate, then passes that object and S's MAR/A/dimension/address tables to
1408421d0 or1408450f0. With canonical3D tables and id0x0b/flag1, the ordinary
flag1 copy path calls1408421d0; altered actual table values can change its special
category branch. No destination can be replaced with optional geometry M by
assumption. Exact nested array writes/allocation/partial failure in these copy
callees are not yet closed.

Thus companion DB null, directory-query failure, missing headers, a newly old
version or the zero-length cache shortcut does not by itself complete the reader
or suppress its later object-copy loop. Cached early return in CP70 skips all
of these operations and must remain a distinct successful conditional branch.

## Smallest next implementation and tests

Viewer-local next increment, if assigned: only after bounded in-range/cache0 is
established after real COW/table selection, call the existing actual node timer's
start("Start") and checkpoint("ReadSingleAArrayFromDB",nullptr), then retain a
pending guard at loader entry. Preserve actual raw timer fields/history and real
clock samples. Do not call the DB loader, arrays timer, stop/report or object
copies, and do not enable public cache-miss success. No shared helper/CMake change
is needed for that prefix. If this stage requires byte-identical native clock
epochs rather than the existing source clock profile, retain the current guard
and assign the clock dependency explicitly instead of inventing timestamps.

An additional null-companion loader-return/post-arrays-timer prefix could be
scoped later, but still stops before object copies and must qualify that timer's
actual signed count/storage (negative count must not become a repaired value).
It should not be bundled with the directory/A-body allocation branch.

Required Release-active tests before the minimum prefix:

- Miss: real COW detaches once, actual table selection precedes node-timer reset,
  then count2, labels Start/ReadSingleAArrayFromDB, null subs; slots2..99, name,
  flag and all other timers remain unchanged. Preserve actual source owner/cache/
  version/body pointers and keep selected scene unsupported.
- Cached1/-1 completes the established CP70 path without changing this timer.
  Invalid header index/id limit, unknown owners, bad registry, missing coordinates,
  zero counts and other fast getter routes must not enter the miss timer prefix.
- Repeated fresh miss invocation resets count and records its two new samples,
  preserving unused slots and table current3 no-refill. Do not append a fabricated
  third checkpoint or expose a fictitious native resume frame. Test fixtures may
  snapshot/restore globals only for isolation; production retains partial effects.
- Null or opaque companion DB must not be dereferenced by this pre-loader prefix;
  no A allocation, version change, cache1 write or arrays-timer change is allowed.
  Future loader tests need actual record-owner/index evidence, zero-length cache
  shortcut ordering, returned-allocation failure contract and no invented rollback.

Use actual runtime clock values as opaque samples; do not require fixed values,
monotonicity across system-clock changes, or newly zeroed unused timer arrays.
Existing `test_sdctl_timer.cpp` covers class bookkeeping/name/flags but was not
run here and does not qualify full geometry integration. Root focused target
remains `m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`; native miss success and
loaded-project owner provenance/runtime parity remain unclaimed.
