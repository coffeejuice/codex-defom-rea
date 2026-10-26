# CP131 independent review: masked nonfinite node-count conversion

## Result

No blocking correctness issue found in the proposed narrow candidate change. The native target-node path performs a signed 32-bit `CVTTSD2SI`, then calls the qualified nonwrapping spin control. For a nonfinite target node count, checking the actual current MXCSR invalid-operation mask before executing the same intrinsic is a defensible conditional profile. It does not qualify the selected original process's MXCSR or enable public editor admission.

## Evidence checked

The controlling original binary is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained instruction range `[140ecf261,140ecf2ba)` is bound in CP131 at SHA-256 `79e20cb88c3f68216df58c775a5f4b5bbf2c7c917222f7d238ccadb7910917dd`. At `140ecf27f`, bytes `F2 0F 2C 94 08 C8 00 00 00` convert the target `double` to signed 32-bit; `140ecf29e` then calls `QSpinBox::setValue`. The conversion result is not written back to the temporary during load. The prior-field writeback later reads the spin value and stores it at `140ecef7f`.

The retained QSpinBox numeric profile is nonwrapping, min 0, max 1,000,000,000, with signed integer comparison and bounded `setValue`. Thus the masked-invalid integer-indefinite result `INT_MIN` clamps to zero. Candidate `integer(double)` is `_mm_cvttsd_si32(_mm_set_sd(value))`, the same SSE2 signed-32 truncation operation. C++20 is configured for this target, and existing code already uses `std::bit_cast`.

The CP131 report cites Intel's SDM for the ISA contract: nonfinite/out-of-range conversion produces the integer-indefinite value when invalid is masked; invalid-unmasked conversion invokes exception handling; MXCSR IM is bit 7. This is instruction semantics only. No selected native application's current MXCSR or original exception-handler outcome is present in retained evidence.

## Review findings and limits

- The proposed bitwise exponent test (`bits & 0x7ff0000000000000`) safely classifies quiet NaN, signaling NaN, and either infinity without floating comparison or `isfinite` on a signaling NaN. Requiring IM bit 7 before calling the existing intrinsic avoids deliberately executing an unmasked-invalid conversion.
- For admitted masked cases, retaining the same intrinsic and spin clamp is the source-matched route. The conversion's normal sticky invalid-status effect should remain; production must not alter MXCSR or clear its flags. The report's proposed tests restore the complete entry MXCSR before framework assertions, which avoids leaking test environment changes.
- On load, the raw target node-count bits remain in the temporary and model. A later callback that commits the visible spin value is the first point where zero replaces those bits. This also preserves the programmatic move followed by Save ordering: that Save copies the raw target record after loading spin zero; a later commit is a separate effect.
- When IM is clear, refusing before target-control and stored-index changes is a conservative candidate stop, not a claim about the original's successful return. The previous row may already have been committed, so `PreviousCommitted` is the accurate report boundary.
- This change should stay scoped to target `nodeCount`. Nonfinite interference, ratio, tolerance, old controls, other FP exception masks/status behavior, and arbitrary 2D paths remain outside this qualification. Existing finite overflow behavior should remain independently tested.

No actionable test or source change was identified by this read-only review. The CP131 proposal is a conditional candidate-only expansion; it does not establish native MXCSR state or fresh original parity. No source, test, build, GUI, original execution, or mutable REA action occurred in this review.
