# CP159 strict-empty KwOperations session test plan

## Correction after call-chain verification

The original read-only plan below contains a mistaken statement that the
current `ConversionSession` project-open route always refuses before Keyword.
`variable_record.cpp` admits an exact explicit `DoeVariable` header (canonical
name/id, dimension 3, supported sample value) and then calls
`readBoundedKeywordConstructorPrefix` for a direct Keyword child. The first
attempt used an underspecified row. The implemented Type2 `readLazyStudy`
test uses an admitted row and reaches the CP159 strict-empty branch. This
does not by itself establish a completed public Open, Save or Close.

Keep the text below as the historical plan and use
`analysis/resume159-doe-lazy-test-implementation.md` and its independent source
review for the verified reachability claim.

## Decision

The strict parsed-empty case is reachable through the production Type2 lazy-study reader. It is not reachable through the existing actual `ConversionSession` project-open route: that route's `DoeInputTable.XML` parser currently recognizes `DoeVariable` rows only by calling `appendBoundedHeaderVariableFromSource`, and that helper refuses the existing variable payload before keyword parsing. Therefore the smallest honest integration test should exercise `readLazyStudy` with a real Type2 `DoeInputTable.XML` fixture, rather than claim a full project-open/session path. A session-owned `StudyIo` may be used if accessible, but is not necessary to assert the state and risks widening scope.

## Target and fixture

Extend `source/apps/moproj2mst/tests/mdt/test_doe_activation.cpp`, adjacent to `doe_lazy_unsupported_branch_preserves_prior_header_and_sample_effects`. Use `ScriptIo`'s in-memory file map and exact Type2 route: `readLazyStudy(study, *element(), studies(), io)`, with `io.files["DoeInputTable.XML"]` containing direct `DoeVariables/DoeVariable/Keyword` nodes satisfying `retainedProfile` exactly: TLOC attributes, `NominalData/string` text `TLOC    \t0.1`, and empty `KwOperations`. Set `study.samples.mode=0`; row dimension must resolve to 3 and its original name must match the keyword owner. No filesystem, GUI, mutable REA, build, or public-admission change is needed.

## Expected state at guard

Catch `Error` with `M2M_E_UNSUPPORTED` and the scalar nominal-text-prefix marker. Assert input read was the only I/O action (in particular no sample/PAR writes), canonical named `HeaderVariableRecord` owns one Keyword with `nominalText == TLOC    \t0.1`, parameter `-1`, false function/enum flags and empty operation list. Assert outer-row partial effects remain as produced before the keyword guard, and no later variable/PAR/DOM completion is claimed. This checks committed nominalText on the canonical Study and preserves the partial guard boundary.

## Why not ConversionSession today

The direct-reader test in `tests/doe/test_keyword_reader_empty_operations_prefix.cpp` constructs a synthetic Study and invokes the private helper, so it proves only the local commit. `test_doe_activation.cpp` already proves Type2 lazy reader I/O and partial effects, but its `<DoeVariable/>` example stops at the unsupported variable reader. `parseInputTable` routes exact `DoeVariable` rows through `appendBoundedHeaderVariableFromSource`; no current session entrypoint skips that admission and calls the private keyword helper. Do not weaken the public admission or bypass the actual Type2 caller merely to make a session test pass. A true session test requires a future supported variable-prefix integration seam.

## Evidence identities

The CP159 implementation receipt is `analysis/resume159-doe-empty-implementation.md`; it records direct test SHA-256 `A46F991FAF0C98E099A74C4FFBFE910F77D4C40405E876BD11A35B003FED6C2B` and reader implementation SHA-256 `8FB6412E44EE3720C6740ABA04A8E1B63FF0AFAC9026C11EDB55644F3899A1F4`. Local source inspected: `src/doe/keyword_reader_prefix.cpp`, `src/doe/doe_study.cpp`; existing Type2 fixture/test: `tests/mdt/test_doe_activation.cpp`; session coverage: `tests/mdt/test_conversion_session.cpp`.
