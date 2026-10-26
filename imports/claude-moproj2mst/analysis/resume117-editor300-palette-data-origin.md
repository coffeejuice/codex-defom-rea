# Selected palette data origin and the first atomic barrier

The data-pointer producer is statically closed through the getter copy and private pointer store. The selected historical private-return pointer therefore has a source-qualified getter-copy origin, even though its source wrapper was not captured directly. This does **not** bind the actual candidate Session to that owner or recover the atomic immediately before deref. Keep public `6504abb0` guarded. No source implementation is recommended from this evidence alone.

This report is the only file written. Investigation used bounded offline PE disassembly, retained reports/capture script and current candidate source. No original execution, GUI control, mutable REA database, build or test execution occurred.

## Exact data path

Let `O` be the wrapper selected by the application getter, `D=*(O)` its palette data, `T` the getter output temporary, and `P` the private destination wrapper. These are different objects. `R=uint32(D[0])` is a native atomic; it is not a wrapper word or a host owner count.

| Preferred QtGui address | Effect and ordering |
| --- | --- |
| `6504abb9/abbe` | Default private constructor passes its local `T=&[rsp+20]` to getter `65016f70`. |
| `65016f8a` | Getter loads application wrapper slot `659c9690` (RVA `9c9690`). This is a value read, not just a symbol identity. |
| `65016f94` | A nonnull loaded wrapper goes directly to the copy join. Null takes allocation/construction/publication first. |
| `65016fc0..fc6` | Pass `O` as copy source RDX, original caller output `T` as RCX, and call `6504a8b0`. |
| `6504a8bd/a8c3` | Load `D=*(O)` once, then store that pointer into `T`. No palette-data clone/detach occurs here. |
| `6504a8c6` | Native atomic retain on that same `D`; its bool return is ignored. Wrapper-word copying follows. |
| `6504a8f4`, then `65016fcb` | Copy returns its output wrapper; getter returns its original `T` address. |
| `6504abc3` | Load RCX=`*(T)` for temporary deref. |
| `6504abc8` | Load RDX=`*(RAX)`, with getter return RAX=`T`. Thus this is the same `D`. |
| `6504abcb` | Store `D` into `P`. The destination has not yet retained it. |
| `6504abce` | Deref temporary ownership on `D`; first unresolved numeric/branch input. |

There is no intervening replacement of `T` or `P` in this normal synchronous instruction path. CP116 establishes that temporary wrapper word `W` is not consumed by this private slice. It is unnecessary to know `W` to describe the data-pointer store and pending deref; assigning `W=0` to a general full-value getter would still be false.

The retained selected editor004 observation 10 sees `P=0x50ae000`, private `0x50ade90`, data `0x3e3edb0`, word 0, after this private constructor returned. CP90's frame gates connect it to selected editor `0x383e780`, Operation `0x37bdbe0`, thread 30444 and the same postOpen Project. Static provenance therefore identifies that returned data as the value copied by this getter invocation, assuming this source-qualified call path. This is stronger than merely knowing that a private palette exists. It remains an inference through the verified body, rather than a direct observation of `O`, `T` or their atomic at the earlier edge. It cannot be transplanted into another process or used as a candidate runtime control block.

## Null is a separate producer, not a selected fact

Getter null branch `65016f96..16fb9` allocates a 16-byte wrapper, calls `QPalette(GlobalColor)` at `65016fb0` with argument 2, and publishes its result into the slot before copying. Wrapper-allocation null is explicitly distinct and still reaches the copy join; it is not a successful initialized palette fallback.

Constructor `6504bf80` allocates `0x1f0` bytes at `6504bf9e`, calls data constructor `65049990` on a nonnull allocation at `6504bfb0`, stores its result into the wrapper at `6504bfb9`, zeros the wrapper word at `6504bfbc`, constructs QColor at `6504bfca`, and calls palette initialization `6504b7c0` at `6504bfd7`. The data constructor supplies atomic initial value 1 at `65049996/999e` before its brush construction and serial fields. Initialization and later sharing remain intervening work; allocation count 1 is not a selected current-count receipt.

Neither editor004's selected output nor the PE slot's initial storage chooses the null/present branch at the later postOpen getter. Startup initialization, later cached assignments and releases cannot be omitted. This bounded investigation did not attempt to recover all of `6504b7c0` or the application's complete palette mutation history. A fresh initialized-data path would require those remaining effects, as well as an actual null-slot occurrence.

## Retain and deref do not close the current count by cancellation

Native QtCore leaves `67001aa0` and `67001ac0` perform LOCK ADD of raw uint32 `+1` and `-1`, respectively, then SETNE AL. In an interval with no other mutations, a pre-copy word `r` becomes `r+1` at getter retain, then `r` at temporary deref, all modulo `2^32`. The deref result is zero exactly when its own input is 1. Source arithmetic does not establish selected `r`, and other threads can mutate the same data between these operations.

Even a nonnull application wrapper supplies no numeric bound in these native leaves. For example, absent intervening mutation, raw input 0 reaches temporary-deref zero/destruction, while raw `0xffffffff` wraps to 0 at getter retain and back to `0xffffffff` at deref. The getter retain's false return is ignored. Do not replace the byte-level contract with a positivity rule, sentinel exemption, or an assumed nonzero deref.

At `6504abd4/abd6`, AL nonzero skips destruction. AL zero enters destruction of 60 QBrush elements from `D+8` and data deletion (`6504abfb/ac03`). Only afterward does the common tail write private word 0 (`6504ac10`) and retain destination data (`6504ac17`, bool ignored). A returned pointer or later readable brush does not provide a recorded at-edge atomic or safely authorize skipping the zero branch. A zero result must remain committed destruction-pending state, rather than being repaired by the later destination retain.

## Smallest data-only contract and missing admission

With an actual current getter producer bound to the same live runtime and selected private constructor, the smallest evidence-closed prefix would be:

1. Retain one native getter temporary on the producer's canonical data. An unknown numeric word can remain unknown, while the actual retain occurrence/token is recorded once.
2. Retain that same data identity in `P` as a partial pointer-only slot with destination word unknown. Preserve this mutation on refusal or retry.
3. Stop immediately before temporary deref until an exact current-sequence atomic or an appropriately qualified native outcome is available. Do not write private word 0, retain the destination, or proceed to font construction.

This is already the scoped shape of isolated CP81: `view_manager.cpp:344` admits a getter only through its actual registered runtime/receiver/wrapper/data owner; `splitview_placeholder.cpp:160..170` stores the same data partial slot and returns `PaletteRefUnavailable` or `PaletteTemporaryDerefUnrecovered`. Repeating that helper would not bind Editor300.

Actual `ConversionSession::beginEditor300Factory` still supplies the genuine Project/Operation/GuiModel and widget runtime but labels the postOpen private-value aliases as unbridged. `editor_base.cpp:77` returns `PrivatePaletteUnavailable`; `loader_l3.cpp:675` retains the `6504abb0` public refusal. Neither a historical native address nor another runtime's startup alias satisfies this owner preflight. General getter/full-value admission also still requires `W`; a partial data-only admission would need its own reviewed producer contract rather than weakening that API.

For a future bounded selected-frame observation, `W` can be omitted from the minimal consumer receipt: capture actual copy entry source `O`, `D=*(O)` and output `T` in the selected default-constructor frame; then at `6504abce` require `*(T)==*(P)==RCX==D` and record the current raw uint32 at `D`. Account for the actual native deref result/concurrent mutations under CP102's thread/race qualification. Copy entry directly identifies the getter's selected wrapper without separately requiring a global-slot snapshot; recording the slot remains useful to distinguish its cache branch. Native addresses must then be mapped to the candidate's actual live data owner by a separately reviewed admission. This lane neither acquires that observation nor creates such an owner.

## Qualifications

Reverified original QtGui SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` and QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Bounded direct PE byte ranges below have exclusive ends:

| Range | SHA-256 |
| --- | --- |
| QtGui `65016f70..65016fd4` getter | `c25e3f1aa850624ddd48c4f9523c292a873f9573fc1c338462fc6c86a8035b25` |
| QtGui `6504a8b0..6504a8fd` copy | `a0c82e2e8037dc55d2034c9b82ad3ae911f3c4af2e740438ecfbe4e9970cc03b` |
| QtGui `6504abb0..6504ac26` default constructor | `704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399` |
| QtGui `6504bf80..6504bfea` lazy constructor | `3cf5e6195328c734ca31032f6140742b4fac53cc20232dc66c1c225ed912907a` |
| QtGui `65049990..650499a4` data atomic initializer call | `a3d8d8222f5fc62d7f0bd754281123721027673fbe3bc62f1b357cf5b214d9fe` |
| QtCore `67001aa0..67001aa8` retain | `7b57a50acb800ca3e7d326b624b14349c569ea06f18e594273c33a92fe1256de` |
| QtCore `67001ac0..67001ac8` deref | `06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6` |

Retained frame/output qualification: CP90, CP98 and CP102 reports; editor004 `probe.js:138` reads wrapper object/data/word only, and `:363` records the selected private return. Its globals helper `:161` does not read the app_pal slot. No new native data/count measurement was found or produced. CP116 supplies the narrower word-consumption proof. The present conclusion is a closed static pointer path plus an unchanged runtime owner/atomic barrier.
