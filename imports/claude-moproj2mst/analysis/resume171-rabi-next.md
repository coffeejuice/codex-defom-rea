# CP171 public C ABI Open/Save/Close coverage audit

## Decision

One small, source-determined ABI contract remains untested: **successful wide and UTF-8 message-buffer truncation must preserve complete Unicode characters, terminate within the supplied capacity, and produce equivalent text**. Add this as a focused extension of `tests/g0/test_abi.cpp`; no new original-runtime inference is required. It exercises the public conversion boundary on an input that deterministically returns `M2M_E_OPEN`, so it does not depend on a successful Open or on changing Open/Save/Close behavior.

CP170's frozen checkpoint remains 361 registered tests (354 clean, six logged gaps, one diagnostic difference, zero failures/skips/incomplete), with historical 93 SAME/five DIFF unchanged (`analysis/wave3-checkpoint170-validation.json`). CP170's root integration states the selected implementation work is private QObject `+0x58`; this audit does not treat that work as ABI or lifecycle coverage. The broader public Open→Save→Close acceptance is still incomplete and this proposal does not claim to close it.

## Contract evidence and existing gap

The public contract in `source/apps/moproj2mst/include/moproj2mst.h` says nonempty messages are NUL-terminated and truncated when they do not fit. `source/apps/moproj2mst/src/dll/abi.cpp` implements the conversion: `putMsgW` backs off a trailing UTF-16 high surrogate; `putMsgUtf8` backs off continuation bytes. Both therefore intentionally avoid splitting a Unicode scalar at the truncation edge.

Current `tests/g0/test_abi.cpp::message_truncation_keeps_nul` checks only that a 4-wide-character buffer and 5-byte UTF-8 buffer end in NUL for an ASCII missing-path diagnostic. It does not exercise either Unicode-boundary rule or establish that wide and UTF-8 entry points return the same diagnostic text after decoding. Existing ABI tests cover invalid UTF-8 input, flags, settings, and failure states, but not this successful diagnostic conversion/truncation contract.

## Smallest test proposal

Extend only `message_truncation_keeps_nul` or add a neighboring G0 case. Use a deterministic missing path whose diagnostic contains a supplementary-plane character (for example, an emoji in the filename), call `m2m_convert_w` and `m2m_convert_utf8` with capacities that end after the high-surrogate / first UTF-8 byte prefix, and assert:

1. both calls return `M2M_E_OPEN` and each buffer contains a NUL within its declared capacity;
2. the wide output does not end in an unpaired high surrogate;
3. the UTF-8 output is valid UTF-8 and does not end in an incomplete multibyte sequence;
4. decoding the complete-capacity wide and UTF-8 messages yields identical text, and both short outputs equal a prefix ending before the same Unicode scalar.

Keep the test independent of the original executable: these are adapter buffer semantics explicitly stated by the public header and implemented in `abi.cpp`. No build or test was run during this read-only audit.

## Scope and blocker

This covers an exposed adapter contract and is safe to add without fresh behavior inference. It does not create evidence for original native Open, Save, or Close semantics, and it does not justify relaxing any unsupported-operation guard. The exact CP170-derived blocker for full public lifecycle coverage remains the absence of a qualified same-occurrence original Open→Save→Close witness; the bounded CP170 non-GUI route audit found no fresh startup CLI route. GUI capture also remains subject to the repository release gate. This proposal is an independent, worthwhile ABI test only.

This report is read-only apart from its creation. No source, test, fixture, build, executable, GUI/app-control session, live REA state, or shared runtime state was changed or run.
