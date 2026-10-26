# CP159 Stopping cache-miss entry and first DB owner boundary

The first source-ready continuation after CP158's qualified cache-zero branch is the actual node-reader timer: `start("Start")`, then `checkpoint("ReadSingleAArrayFromDB", nullptr)`, followed by refusal at DB-loader entry. This is a reached native state change, not a positive cache-miss getter. A nonnull companion DB immediately needs a different ownership proof from the selected Stopping main object. Do not enter its directory query using an opaque DB pointer.

This report reconciles a stale witness gap: CP71 already established the same original reader's timer/loader/query behavior; CP73, CP131/132 and CP142 further investigated geometry integrations. Their native function evidence applies because Stopping reaches the identical original coordinate getter/which0 reader. Their viewer-specific owner, barrier, allocation-receipt and retirement implementations do **not** automatically apply to Stopping. CP158's cached-hit source work is not repeated or changed here.

Only this report and six bounded ASM captures/two capture receipts were written. No source/test/build/fixture, original executable, mutable DB, GUI, live REA session or installed application was changed or run. The retained original is **DEF_GUI_MO_64 v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; this is not a reconstructed DEF_PRE instance. No original-runtime parity or full reader/exception behavior is claimed. Token measurement is unavailable.

## Immutable evidence binding

The retained PE `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed and decoded with VS DUMPBIN 14.44.35228.0, using `admin/capture_pe_ranges.py` and `/disasm:bytes /range:START,(END-1)`. Tool SHA-256 remains `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Captures are `analysis/resume159-editor-cachemiss-{ENTRY}.asm` for reader/loader, and `analysis/resume159-editor-cachemiss-prefix-{ENTRY}.asm` for the other four rows. Corresponding `*-static.json` receipts retain exact commands and text digests. Native bytes were independently sliced through the PE section mapping; all bounds below are end-exclusive.

| Function/range | Native-byte SHA-256 | Capture-text SHA-256 |
| --- | --- | --- |
| reader `[14082d7d0,14082dc8b)` | `a3bc02528ce5ce18e576c5f938d82fc8b081616504437a8ca980001ce74dd6e4` | `de5cdda35ab04b415ea6d094f9d2915f7b33d6deb5511bca70586c233502cd1f` |
| loader `[140828060,1408283d5)` | `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796` | `74c9cfcba3b88ecbabba31c79b57b5efed63c25fb1e9a4594d49ffcf045c580f` |
| directory query `[140825440,140825532)` | `4525b4f95fe70043fd1c70d20acced398afb237fa0b8331a12d0da131cd00ac3` | `8e2dbca3f54429e674fb228dc096850b927d64e478e3e307102651615245afb5` |
| timer start `[140832340,14083236f)` | `b95eadaf0da18648f36a299657ca240864e02c8b065e407708bdf72224b9d4be` | `4bcd80ec9477567225bb0e8ed5fe983c19f7d6afe87ff4057256c3176e7820d1` |
| checkpoint `[140832370,14083242f)` | `4eab6c32d8813886e4dfa2ddd8dc39d0f24f4a8b55c4f1267c3c6b8ff7b18dc6` | `b773d2825ef2166ad25972cf410ea397197900f3d251c5a043ef1bc6a44dfd1b` |
| double allocator `[1409c83b0,1409c8408)` | `2ca64c1da768dd96662135394af3e2d8621a13a257c78ab42d2ebb046ea00ef4` | `3422dfb5408607dc1e32fbae13ed30f7077b56d4f0739a091718c71312941a3b` |

These are complete normal PE unwind regions through RET, not proof of all logical exception funclets. Every native and text hash agrees with the CP70/71 retained evidence. No new disassembler database was opened. Additional directly rehashed slices used with CP71's decoded qualification:

| Range | Native-byte SHA-256 |
| --- | --- |
| timer reset `[1408324a0,1408324b5)` | `b6842557e998c9dc6afbdc6515f9404ff61ca87793e8471ab18250028f61b520` |
| clock wrapper `[1408324c0,14083250e)` | `7aee061c434ad52f2596aa43fad739bebd6f88be869c51394fd49d18addeafec` |
| node timer initializer `[141d32af0,141d32b22)` | `dc5ed4d5417bf3652f61ec18d7ba63e12db51c7b61adc142797aa6a95ddc2087` |
| arrays timer initializer `[141d32a70,141d32aa2)` | `1431c5c529589adebeecd19996510ade77ab348a42e751552314711e25fb228a` |
| version setter `[140825400,140825419)` | `4fed3c566735c55f1ee0b69e7b5a69ec269243e8cb5aa16df3ca49d81c1beb07` |
| A allocation/cache shortcut `[1408281fb,140828275)` | `286c53802c1db4aacf4fa6489fa9656c89577c20e3a037b246b0142f79b804f3` |

## Exact reached order and first DB/body read

Let `S` be the same actual live step already checked after Stopping's real key0 dispatch; `N=O+8a0` is the selected main object's actual node set. Retain CP158's canonical variable/type, actual module table, signed index-bound and allocation/COW qualifications. The first added branch begins only after actual `S.u26c4[11]==0` is established, after table selection. No detached node set or substitute step is allowed.

1. At `14082d895`, call `140832340` on process global `143DBE000`, with literal `1427A6E6C` = `Start`. It calls reset, then checkpoint. CP71 initializer/layout qualification identifies this as existing `g_timer_read_obj_node_state_variable` (constructor flag false).
2. At `14082d8ab`, call checkpoint on that same global, literal `1427A6E78` = `ReadSingleAArrayFromDB`, sub null. The two entry samples set count to 2. Reset only changes count; it does not clear prior name/flag/padding or unused slots. Checkpoint samples the real clock, writes time at `+4ee8+4*i`, copies label at `+c8+200*i`, writes null sub at `+5078+8*i`, then increments count at `+5398`. Flag does not suppress writes. The strings were independently read directly from this PE.
3. At `14082d8c0`, call loader `140828060(S,&id)`. Its first external owner read is `S+3390` at `14082808b`. Null jumps directly to `1408283bd` and RET, without query/version/A/cache mutation.
4. With nonnull companion `D`, loader initializes stack outputs and calls `140825440(S,D,S.u740,&locals...)` at `14082814e`. The explicit step argument is saved but not used to select the record. Query checks D for null, then reads actual signed `S.u740`, multiplies by `0x28`, loads directory body `D+ad8` at `14082547c`, and first dereferences `directory + index*0x28 + 8` at `140825488`. There is no index-count, null-directory or extent check before that body read in this function.
5. Query reads the same record offsets `+8,+c,+10,+20` into caller locals (MAR/A header sizes and version under the retained source mapping). It compares the record version with current S version; mismatch calls setter `140825400`, writing actual `S+1b64`, then returns AL=true. This version effect precedes the loader's high-version predicate at `140828167`; a record can change which later path runs. Do not preflight that predicate using only the old version or roll the mutation back.

**First unresolved Stopping owner boundary:** the nonnull companion must be the actual `S+3390` owner with a current, physically bounded `+ad8` directory allocation and actual signed `S+740` record index. Stopping's main-object/node ownership does not establish that relationship. The viewer lane's Project DB receipt is separate evidence and may only be reused after an explicit current owner binding; a generic `GuiModel.db`, fake record or nonnull pointer is insufficient. This investigation stops admission before this first unresolved body access.

## Complete normal loader exits and remaining reader continuation

The complete decoded loader gives these exits, all joining epilogue `1408283bd`:

| Reached condition/site | Surviving effects before return |
| --- | --- |
| companion null, `8280a5` | No loader state writes |
| query false, `82815a` | Query only returns false on null companion; caller's preceding nonnull check normally excludes it |
| GUI predicate true, `828173` | Directory query/version mutation retained |
| signed id outside 0..279, `828195` | Query/version retained |
| either header length `S+1b68` or `S+1b6c` zero, `8281bc..1d4` | Query/version plus native diagnostic path; diagnostic implementation not newly closed here |
| cache became nonzero, `8281f6` | Prior query/version retained; no A allocation |
| selected stored length `S.u2b28[id]==0`, `828265..270` | Allocate/publish A first when absent, then set cache[id]=1 |
| nonzero stored length, `828275..3b2` | Prepare companion path/conversion, call `140b38d57` with actual S header/MAR/A owners, then set cache[id]=1 on normal return without testing its result |

For the A producer, null `S+1b78` calls `1409c83b0(wrapped32(S.u2b24+2))` at `82821e`, then publishes its return immediately at `82822e`, before the zero-length cache shortcut. Native helper special-cases count==0; otherwise it sign-extends count and calls an allocator with element size 8. A null result calls `140b2f920`; only a normal diagnostic return leads to returning stored null. Allocation diagnostic and nonempty decode ownership/control flow are outside this cut. There is no loader-side returned-pointer test or transactional rollback. Do not use nominal A length as a proved allocation extent.

After **every normal loader return**, including null companion, reader `14082d8d6` appends `Assign the variable to the object data` (literal `1427A6E90`) to the different process global `143DCDAE0` = existing `g_timer_read_arrays`. Its prior count/history is retained; native signed count<100 appends, >=100 skips, negative can index before arrays and must remain a reconstruction guard rather than be repaired.

The reader then computes its ordered special-state selection and walks signed `i<S.u1378`. Each destination is the actual one-based main-object lookup `1408302a0(S,i+1)`, not the selected Stopping node set substituted for every object. It forms the per-object MAR address before null/type7/flag caller skips, then calls node `1408421d0` and/or element `1408450f0` assignment with S's MAR/A and tables. CP132/142 retain additional zero-storage/equal-width qualifications; no positive copy path is newly admitted here. After a fully completed loop it calls node-timer Stop at `14082dc7c`, then returns AL=true at `dc81`; the outer which0 caller ignores reader AL. A loader early exit therefore is **not** whole-reader completion or a no-op cache miss.

## Source-ready smallest implementation and tests

The current `stoppingCoordinates` combines invalid MAR/A bounds and cache0 into one refusal at lines 400–403. Split only those decisions: invalid bounds still refuse before timer access; nonzero cache still completes CP158 unchanged; valid cache0 calls the existing actual node timer's two entry methods, then returns the existing `CoordinateSync` failure before loader entry. No new continuation pointer, cached success, resume frame, public Save support, DB dereference, arrays checkpoint or object copy is required. Each fresh invocation repeats its reached native entry effects; it is not resuming a saved stack frame.

This inherits the documented existing `StepTimer::clock_ms()` semantic adaptation: host CRT `std::clock` with native-shaped conversion. CP71 identifies native CRT time as an external input and does not establish cross-process epoch/tick equality. Preserve real opaque samples; do not invent fixed timestamps or claim byte-identical native timing.

Required focused tests in the existing Stopping test target:

- Valid cache0, initial module dimensions 0/2/3: actual allocation/key0 COW/table selection precedes reset/checkpoints; one reached getter/dispatch; node timer count 2 with exact two labels/null subs. Preserve slot tails beyond written NULs, slots 2..99, name/flag/padding; arrays and other timers remain byte-identical. Use a forwarding real dispatcher observation hook to prove timer bytes are still old at dispatch.
- Cache1/-1 and invalid MAR index/signed A limit retain their current success/refusal and byte-exact unchanged timers. Foreign owner, wrong type/registry, parked model, missing selected object, no node and ordinary low-version/nonzero-flag paths must not acquire the new timer effect.
- Null **and opaque nonnull** companion DB controls both reach the same pre-loader refusal without dereferencing D or changing step version, MAR/A pointers/body, cache or header data. Preserve already reached widget/allocation/COW effects. Never let fixture destruction own the sentinel pointer; restore borrowed controls first.
- Repeated fresh cache0 invocations reset the node timer and write only slots 0/1 again; current dimension3 retains altered actual cells, COW stays unique after the first detach, and no arrays checkpoint/Stop is appended. Cached -> miss -> cached preserves this distinction.
- Keep private constructor failure stage InitialFill/Stopping and public completeness false; tests establish reconstructed private lifecycle behavior, not original-runtime or public Session acceptance. Root owns grouped build/testing and freeze.

Current source binding (read only): `simcontrol300.cpp` SHA-256 `3435741e29068aea9e336069f5388efe68a5056b1d54d70d626b6f0b385508d0`; `test_simcontrol300_stopping_cached.cpp` `cb2a84a88d16626e871c49e2c84b42ceaa8c837a01c58bed07474b1644c3d634`; `step_timer.cpp` `f72dd675db7b7028afe957628497708e67d6b215eba32edbf643d96634afb136`; `step_timer.hpp` `8a3fceb49f24342f1ff83b5b8a8ec211eb041bc366889c9c9c17411da454dcc9`. The smallest next evidence lane after this timer cut is current Stopping S-to-companion-directory ownership and bounds; null-companion post-loader arrays timer is separately source-closed under its actual count qualification, but does not complete assignment.
