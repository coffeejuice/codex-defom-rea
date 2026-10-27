# CP310 typed API and CP311 strict framing

CP310 adds an isolated Win64 read/cleanup adapter with explicit ctypes signatures and normalized BOOL-plus-output results for handle flags, job accounting/limits, process/job membership, PID, creation FILETIME, image path, duplication, waits and closes. Local and remote handle types reject the all-ones invalid sentinel and values beyond pointer width. Its live creation entry raises HOLD and it is not wired to CP309.

CP311 adds a pure byte-framing and OFFER→ACK→RECEIPT model. It bounds a four-byte little-endian length, total bytes, strict UTF-8 JSON, duplicate keys, schema, nonce and decimal handle values. Malformed input and repeated frames enter sticky HOLD; the model never grants resume authority. It has no pipe or independent worker identity binding.

Independent reviews found and corrected CP310 output-length and handle-bound cases, plus CP311 non-sticky decode failure, oversized decimal, nonfinite number and surrogate handling. Their final fake-only reviews passed. Root ran eight CP310 tests, eleven CP311 tests and the combined 123-test CP303–CP311 suite in both import orders. Source/log hashes and unpassed gates are in `analysis/cp310-311-fake-validation.json`. No worker, child, original or GUI process was run.

CP312 next resolves the native WORKER/CHILD job topology from official Windows documentation and a bounded harmless experiment design. Subsequent work must bind real worker endpoints, exact offered-handle identity, external ACK receipt, protected file-to-image evidence and exact cleanup. The compiled converter baseline remains CP301: 392 CTests and 373 source-bound m2m tests, six logged gaps, one diagnostic difference and five historical Save differences.
