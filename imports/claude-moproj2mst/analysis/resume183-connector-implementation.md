# CP183 connector implementation

Implemented only the retained QtCore interval `[0x131232,0x131235)` (`48 8b c3`, `mov rax,rbx`). `QObjectPrefix::returnPrivateIdentity` carries the existing opaque allocation token and `BeforePrivateEpilogue` marks the next instruction at RVA `0x131235`. This does not execute the epilogue, return to the caller, complete QObject construction, publish native addresses, or enable backlinks.

The internal diagnostic instruction stop now supports ordinals 0..5, default 5. Ordinals 0..4 preserve every CP182 stop. The shared CP182 guard path revalidates owner/root control blocks, allocation identity and sizes, module/version/caller provenance, sparse zero prefix, and exact unknown-origin EAX/private20 expressions before the move. The return token must be absent at every earlier cursor, and must equal the original allocation token at the terminal cursor. Reentry refuses without replay. The unknown symbolic expression remains retained unchanged.

Added four focused test cases: complete predecessor/terminal snapshot preservation and same/different-key reentry; nineteen malformed predecessor cases; missing/foreign terminal token refusal; and every CP182 rewind/resume stop. Existing default-session terminal assertions advance to CP183, diagnostic rewinds clear the return slot, and frame snapshot equality includes it. CP182 explicit ordinal-four tests retain their previous boundary. Invalid internal-stop coverage now uses six.

No build, test execution, GUI interaction, or REA database access was performed by this implementation worker. Integration validation remains the root worker's responsibility. Changes are limited to the four source/test files below and this report.

## Implementation file hashes

- `source/apps/moproj2mst/src/doe/task_factory.cpp`: `1f35a50476e65a906cd2931e7ad90ca8791fe35023653b42ee3f96cd6438b1ce`
- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`: `78876f1b91f05ef1c3a0afe361de0854509d14225c00448e35a0c41462eeb0fd`
- `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp`: `5b3fdf93ae055a3f19a38339efa808f52656a6155a30d85015a4fa9d7f5435aa`
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`: `fdc0d9d183fcec9b58276e3d86fd16f2c19eeb5373fbc1626e1a4c0538d79c20`

## First integration failure correction

The root worker's first focused CTest run reported repeated failures inside the CP170 corruption matrix. Its independently named `matrixFrame` diagnostic rewind retained the newly populated CP183 return slot, so the earlier premature-return guard correctly refused before the matrix reached its intended CP168/CP169 corruption guards. The fixture rewind now clears `returnPrivateIdentity`, flags EAX/private20 and private28 along with its earlier cursor. The original specific CP168/CP169 refusal assertions remain intact; no production guard was weakened. The implementation worker inspected `build/logs/resume183-dual-focused-ctest-01-03.log` and made this test-only correction; the root worker must rerun the test.
