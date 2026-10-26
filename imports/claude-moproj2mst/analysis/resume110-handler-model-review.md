# CP110 invalid-parameter model and worker consumer review

## Review result

The model is correctly bounded as a pure value model, and its worker integration does not change the old worker wire/status contract. Unknown and Custom remain nonterminal guards; only the explicit FreshDefault worker profile enters dispatch. The Default branch models the prefix through the debug flag clear, then stops at `CaptureContextUnimplemented`. Neither the model nor the worker claims that native capture, unwind, filter or default-handler termination ran.

One diagnostic precision issue should be corrected or explicitly accepted: the model carries `InvalidParameterQualification::StaticDefaultConditional`, but the worker ledger serializes the hardcoded text `handler_model_qualification=source-model-only` for every model, including Unknown fallback and static-default dispatch. The worker’s expected `ledgerText` repeats the same generic string, so the test enforces that loss of the model’s precise qualification. This label is not a false native-execution claim, but it omits the static-default/no-later-replacement condition from the diagnostic that accompanies the candidate endpoint. Prefer serializing the value’s qualification name (and asserting `StaticDefaultConditional` for the fresh-default case and source-only for Unknown), or document the generic label as deliberate. It is not a blocker to the branch/protocol behavior.

## Branch and ordering checks

`modelFirstNullFilePrintf` starts with exactly `FirstNullFileChecked`, then modeled errno22, before any handler branch. This aligns with the source-qualified boundary: first null FILE check, then errno storage write22, then handler decode/dispatch. The candidate dispatch helper sets host `errno` from the validated request immediately before invoking the private backend; the worker validates `request.crtErrno == 22` and `errno == 22` before creating the model. The model itself never writes host errno. Its repeat-evaluation test checks all evidence classes with entry errno 0, EDOM and ERANGE and requires errno unchanged.

- **Unknown or invalid enum:** branch stays Unknown; only null-FILE-check and errno-written modeled effects; next is `DecodeHandlerUnavailable`; there is no modeled debug store or predicted process status. Invalid numeric enum values fall back to this same record.
- **SameOriginalModuleDecodedCustom:** models errno before decoded-custom selection, then `CustomDispatchPending` / `InvokeCustomHandlerUnimplemented`; it has no default flag clear or predicted status. The test marks this as a hypothetical pure input, not a witness obtained by the test.
- **StaticFreshDefaultAssumption:** preserves the static assumption as a distinct evidence and qualification value, models decoded-null and debug-store-to-`0x1450540e8` value0, and stops at `CaptureContextUnimplemented`. `predictedProcessStatus=0xC0000417` is explicitly conditional, not observed.
- **SameOriginalModuleDecodedNull:** remains distinct from the static assumption, but has the same bounded default prefix and capture guard. Its test explicitly says the input is hypothetical and does not establish a same-run original witness.

The pure tests cover the four branches, ordering, invalid evidence fallback and repeat purity. The model source has no native/OS calls or host errno assignment. `DefaultPrefixComplete` is a model-stage name only; the comments and report clearly say it is not a claim that the post-prefix prelude ran.

## Worker and protocol compatibility

At actual remesh dispatch the worker checks FreshDefault profile, exact site `0x1400abcf0`, source proof, expected status, errno22, ordinal1 and live errno22 before setting its model to `StaticFreshDefaultAssumption`. Unknown and Custom cases do not dispatch; they retain Returned `M2M_E_UNSUPPORTED`/OS0 with the existing guard message. Thus the model does not widen the admitted profile selection.

The worker still writes the existing v2 Boundary only after the validated model is recorded. The Boundary layout/status, profile/proof integer, nonce, SaveSnapshot, fixed candidate endpoint, BackendFailure tags, failClosed path and parked-writer behavior are unchanged. The denied case remains Started+Boundary with child status `0xC0000417` and no Returned frame; publication failure and endpoint-return remain backend failures, not terminal success. Model `predictedProcessStatus` is recorded under a distinct diagnostic key; it is not used to classify supervisor process disposition. The actual candidate endpoint still skips the native default-handler prelude, as stated in the qualification.

The implementation receipt records frozen hashes for the new model header/source/test and the worker consumer. I independently rehashed the four live files and they match that receipt: model header `5d3bc7db…ecfa9760`, model source `f494e9b3…50afdbee`, model test `6cbeabf3…96e1b228`, worker test `ab20892d…e7d605`. This is static source verification only; no build or test was run in this review.

## Limits

The enums for `SameOriginalModuleDecodedNull/Custom` are callable inputs to a pure function and the type itself cannot prove a corresponding retained module/run witness exists. The comments and tests correctly keep current calls hypothetical; future consumers must not treat the enum value alone as witness evidence. The implementation establishes no actual original handler selection, same-run decoded pointer, enabled-remesh original occurrence, CONTEXT capture, `RtlVirtualUnwind`, filter/debugger calls, native termination or native Close. The worker’s observed `0xC0000417` remains evidence only for its candidate fixed endpoint after a valid v2 Boundary.

No source, build, test, GUI, original-app or mutable REA action was performed.
