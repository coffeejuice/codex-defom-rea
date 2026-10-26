# CP129 independent review: Type3 optimizer scalar prefix

## Result

For already-tokenized lines in its admitted scalar subset, the helper matches the retained constructor defaults and key-to-field mapping. It preserves encounter-order overwrites, uses the existing numeric conversion functions without a success flag, maps missing/value-one DB_PURGE to false and other parsed integer values to true, and leaves skipped/unknown keys unchanged. The helper is appropriately not a native Type3 object or parser. Its incomplete-line and row-body outcomes are candidate guards; they must not be described as native parse rejection.

## Defaults and field stores

The retained original evidence is summarized in `analysis/resume128-type3-constructor-parser.md`, bound to `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The constructor initializes scalar offsets as follows: `+0x0c=1`, `+0x10=1`, `+0x30=0x3fd`, `+0x34=-1`, `+0x38=200`, `+0x3c=0`, `+0x40=0.001`, and `+0x48=false`. The value helper initializes exactly these scalar values. It explicitly omits the native QString at `+0x28`, vptr/base state, and the nontrivial shared-data members at `+0x18/+0x20`; therefore it correctly avoids claiming constructor or list ownership parity.

Native parser evidence maps OPR_NO token 1 to `+0x38`, KRG_MAX token 1 to `+0x0c`, TYP_OBJ tokens 1/2/3 to `+0x30/+0x10/+0x34`, EPS_OBJ token 1 to `+0x40`, and DB_PURGE token 1 to `+0x48`. The candidate implements these same mappings. Repeated scalar keys overwrite previous values in encounter order, matching the per-line store model. It evaluates the three TYP_OBJ conversions into locals before assigning the three fields; this keeps the candidate state unchanged when its line-size guard fires. Native OOM/exception interleavings between conversions and stores are not qualified by this pure value helper.

## Skip, terminal, and row boundaries

The original parser recognizes and skips DEF_OPT, TYP_OPT, GRD_TYP and DOE_PAR; the candidate returns Ignored without changing scalar state. Unknown top-level keys also proceed to the next line in the native loop and remain unchanged in the candidate helper. ENDDATA is different: original parsing terminates its line loop at that key, while the value-only helper returns Ignored. The implementation receipt explicitly leaves parser-loop termination out of scope. Any future parser caller must special-case ENDDATA before continuing; the helper result alone does not encode the native stop condition.

NUM_CON and REGIONS are real row-bearing keys in the native parser. The candidate returns RowBodyNotImplemented and does not mutate state for them, which is an explicit guard rather than an implementation of their native effects. The real parser may already have applied earlier scalar lines or rows before reaching a later row; this helper neither models that file/device loop nor supplies rollback semantics.

For recognized scalar keys, the candidate returns RowBodyNotImplemented if required tokens are missing. The native evidence shows indexed token extraction and conversions with null success pointers but does not establish a native unsupported result for short/malformed split rows. These guards safely keep incomplete token vectors outside this prefix; they are not claims that the original refuses such files.

## Numeric limits

The original calls QString::toInt/toDouble without a success pointer. Candidate calls use `qt::toInt`/`qt::toDouble` with the same null-ok shape, and tests cover canonical ASCII values. This is suitable for the qualified token subset. Numeric parsing remains coupled to the existing candidate conversion implementation and its locale profile; this change does not independently qualify arbitrary encodings, locale initialization, malformed numeric tokens, overflow/underflow details, or the unknown QRegExp used to split native lines.

DB_PURGE behavior is source-specific: missing token writes false; with a token, it writes false only when decimal `toInt` equals 1, otherwise true. The candidate's `fields.size() >= 2 && qt::toInt(fields[1]) != 1` reproduces that branch for the accepted ASCII input. An invalid token's exact native conversion result is not established here, so do not generalize the “otherwise true” claim beyond values whose conversion result is known.

## Review boundary

This is a dormant private, already-tokenized scalar state helper. No QFile open, line read, QRegExp split, ENDDATA loop control, Type3 owner/list allocation, parser return, ShapeOptim activation, GEO import or public Type3 support is supplied. Root should treat row/short-line results as explicit candidate refusals and keep the public guard. The source-owner receipt reports static preflight only; I ran no build, tests, GUI, original process or mutable REA action.

Reviewed against `analysis/resume129-type3-scalar-implementation.md` and frozen hashes:

- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.hpp`: `07631bd99653e8ba4af73b8272cc6036c5ae8039a91504bc919706520f317f54`
- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.cpp`: `07949c11202ef449d66dce609609d6ad64c17a8b6060b3a97d49c8096af15b3c`
- `source/apps/moproj2mst/tests/doe/test_type3_optimizer_scalar_prefix.cpp`: `054b08c370d0170cfc66c8c33b124fcbce025d02c878727ad21b8c7c389054a7`
