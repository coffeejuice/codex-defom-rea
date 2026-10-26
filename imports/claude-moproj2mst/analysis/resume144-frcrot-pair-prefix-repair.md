# CP144 FRCROT pair-prefix test correction

The focused CTest failure was an incorrect expected list in the existing-reverse case. Its prepared input contained `(7,3)` but not the requested forward `(3,7)`. The native-derived contract and helper append the missing forward tuple before entering the mode/table and reverse tail. The test now expects the preserved original order followed by `(3,7)`; it continues to verify that the existing reverse tuple is not duplicated and the mirror-call boundary is reached. No production source changed.

Updated test SHA-256: `02901F6EC2CAC2FD3A899FEF8D558383C11D72C1647C402BD21692BE6609DFD1`.

Validation was static only: checked the exact input tuples, expected append order, event index for `ReverseFound`, and the three-file write set. No build/test was run in this repair task.
