# CP217 negative-ID candidate parser preflight

## Result

The candidate parser accepts and reads the copied negative-ID B98 project: `acceptedProject=true`, `openedOk=true`, no exception, and the complete copied file/directory manifest is unchanged across candidate Open. In particular, candidate Open did not create `Lubricants` or any other tree entry. This is a candidate-only result; it does not establish that the original accepts the variant, nor does it identify the original directory creator.

## Exact input and checks

The untouched source is `evidence/gui_mo/oracle_b/98_B_DS3F_before/Gear_blank.moproj`, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`, 10,932 bytes. Test code copied the full 27-file fixture to the configured CTest scratch tree, changed only the direct `Operation_List/Operation/Id` nodes `1 -> -1` and `2 -> -2`, then compared the complete resulting bytes against an independently constructed expected byte vector. This inserts exactly two ASCII minus bytes at original zero-based positions 1,945 and 4,217; the variant is 10,934 bytes with SHA-256 `33537d2a1fd412537f17f3a9f1381a35b0c8569a290d1aacab4c492dde2e4fcf`. Names, Name/Id/Type ordering, types, object IDs, diagram references, and all other bytes remain unchanged. Candidate `openedOk=true` shows its XML/project reader accepted the resulting file.

The test reports the complete post-Open tree delta and now asserts `acceptedProject`, `openedOk`, and exact before/after manifest equality. It does not assume those outcomes: any failure or tree change fails CTest. Its manifest includes files and directories, including empty directories. The observed delta is empty.

## Build and focused run

Built with the repository wrapper, which provides the MSVC environment:

```text
python admin/build_candidate.py --label cp217-negative-id-rerun3 --target m2m_s_b98_negative_id_preflight
```

The focused build exited 0. Then ran:

```text
ctest --test-dir build/candidate -R '^m2m.s.b98_negative_id_preflight$' -V
```

CTest passed 1/1 in 0.06 seconds. Runtime output:

```text
candidate_acceptedProject=true candidate_openedOk=true tree_unchanged=true
candidate_open_error=<none>
```

No `tree_delta` rows were emitted. The final test source is `source/apps/moproj2mst/tests/s/test_b98_negative_id_preflight.cpp` (SHA-256 `47b3a957e73b8b789a4e5059007d6bb5961b8a721a4f5b4b7b6b499b5b06bcc4`). The initial executable hash `648d68e2cf83bdd5745c03a5ad6d44efb8284dfd676a334090968672aecafe8b` predates the review-requested assertions. The current executable built by `cp217-negative-id-rerun3` is `build/candidate/bin/m2m_s_b98_negative_id_preflight.exe` (SHA-256 `2351142e6acac7b4859e4d3748eb970444864645d611e729b4c73814b2e9547e`).

## Interpretation limits

The candidate reaches its own successful `openProject` return and `openedOk` milestones with negative IDs, demonstrating that this candidate implementation does not reject this input during its Open path. These are not equivalent to the original application's internal milestones. The original still must be tested independently before any claim about original acceptance, full Open comparability, or causal directory effects. Its diagram entries still refer to operation IDs 1 and 2, which remains a known downstream validity risk. No original process, GUI, hook, or Save was used.
