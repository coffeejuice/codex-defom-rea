# CP147 source review: process codec owner wiring

## Result

The new `ProcessCodecOwner` itself is inert and does not admit any codec/key behavior: its constexpr default is Unknown, it has no mutator/read API, its destruction is trivial, and the Session stores only a borrowed pointer. The no-allocation and no-cleanup requirements are met by this type. The new notifier test confirms that an isolated Null codec profile does not admit the real notifier-key path.

I found one actionable mismatch with the stated legacy-unbound contract: `core/convert.cpp` constructs every `ConversionSession` with the process owner, including non-`M2M_FLAG_WRITE_SAVE` calls, while only `ctx.session` is conditionally null. Today no production consumer reads `processCodecOwner()` and those legacy reads remain source-less, so there is no newly admitted behavior. Still, the actual legacy-run Session is bound, contrary to CP146's requirement that legacy packet Sessions remain unavailable. Construct the owner-bearing Session only for the full Save context (or otherwise ensure non-Save Sessions remain unbound) before any occurrence consumer uses this getter.

The `run`-local owner also defines one owner per serialized conversion call, not necessarily one per OS process across multiple API calls. This is acceptable only while the run is explicitly the selected fresh-process reconstruction profile. If future source producers model a persistent in-process original, the owner must move to that process/run-driver boundary and be supplied consistently; it must not be silently reset on every `run` call.

## Checks

### Owner default and identity

`include/m2m/engine/process_codec_owner.hpp` is a fixed-value type: default constructor is `constexpr noexcept`, all facts default absent/Unknown, there is no heap/member owner of QString data, and the destructor is defaulted/trivial. `sourceBoundary()` always reports `CodecKnowledgeUnavailable`; `qtCoreModuleSha256` and `cStringsSymbolRva` are static source identity only. No method can store Null/NonNull or manufacture a module generation, codec pointer, or object generation.

`ConversionSession` keeps the legacy one-argument constructor, adds an explicit owner-reference overload, and stores a non-owning raw pointer with a lifetime comment (`include/m2m/engine/conversion_session.hpp:77-88,179-186`). Session destruction does not revoke, reset, or delete that owner. The tests cover explicit sharing across two Sessions, separation across two owner objects, the one-argument constructor's null binding, and survival through explicit Project release and Session destruction.

### Run lifetime and public behavior

`convert.cpp` holds the engine mutex, creates the no-throw owner before details reset and before entering the `try`, then constructs Session inside the try. This lexical order keeps the owner alive through Session destruction, catch conversion, and details publication (`src/core/convert.cpp:160-169,199-203,473-487`). Because the owner is trivially destructible and allocation-free, its pre-try placement adds no new allocation or owner-destructor failure path. The ABI is unchanged; no public argument or return type is added. A focused `convertDetailed` test verifies the empty-project E_ARG code/message and details surface.

### No false codec admission or cleanup

The production process owner remains Unknown for the entire test; isolated `QStringResourceProcessScope::setIsolatedCodecProfile(Null, ...)` mutates only that separate test object. The integrated test reaches the actual TLOC notifier, observes the existing unsuppressed flag reset and key-construction guard, and confirms no map-key/list attempt, no sample reads/writes, no process-codec knowledge change, and no QString source construction. No setter, decoded-data publication, native codec liveness, global cleanup, or native QString release is inferred.

### Coverage qualification

`process_codec_owner_sessions_share_only_explicit_process_identity` checks an unbound directly constructed Session, not a non-Save `run` Session. Yet `run` currently always calls `ConversionSession session(diagramEffects, processCodecOwner)` at line 203, and only assigns `ctx.session` conditionally at line 207. This means all internal conversion modes have a nonnull process owner, even though the legacy conversion route cannot presently pass that Session into the DOE source context. The minimal correction is conditional constructor selection on the same full-Save/profile condition; add a test at the run-construction seam if that seam can be tested without broadening the public ABI.

## Reviewed files and hashes

| File | SHA-256 |
|---|---|
| `include/m2m/engine/process_codec_owner.hpp` | `5C353B68088024B586E322BCAEDD055FBC220777B015E578C012270AFCDEA680` |
| `include/m2m/engine/conversion_session.hpp` | `BB11C4582745B706FEB3F502A65940F96DAE327C9E3783960D30AC618C154D93` |
| `src/engine/conversion_session.cpp` | `679F4C2195A4E664709EC946A67253C177E3B3957EF36BF253DCE53178EEF9A5` |
| `src/core/convert.cpp` | `EDB6879F57B0F846ECCB6086ACB0DC6D83EA9ACE5487E09006DF9D36410E8303` |
| `tests/doe/test_process_codec_owner.cpp` | `A350D76CE260C03FF9458B433734D1DBCE903F3CA85470D10B822AEE8DCB30DB` |

Read-only review only; no build, test run, GUI control, original execution, or REA mutation was performed.


