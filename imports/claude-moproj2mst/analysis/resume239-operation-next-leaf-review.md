# CP239 operation cache-presence leaf review

## PASS

The proposed four replacements are behavior-preserving. `Operation::hasEngineModel() const noexcept` directly returns `static_cast<bool>(engineModel)`, so it checks the stored pointer value without locking, copying, changing `use_count`, or calling lazy model creation. This matches each existing `if (operation.engineModel)` presence fence, including aliasing pointers with a null stored pointer. It is distinct from the free `m2m::hasEngineModel(const Operation&)` in `src/ops/express.cpp`, which checks class/project model availability and does not inspect this cache. The proposal correctly names the member query explicitly.

All four fences occur after the same live-OPI and legal-stage checks and prerequisite null/empty checks, and before the subsequent list/wrapper read. Replacing only the expression in place preserves the established order and refusal. The engine-cache boundary and blocked retry are correctly excluded: the former records a weak owner and address token, while the latter needs null, address, and control-block checks.

The cited cases exercise cache reappearance at each stage: object list, position list, recovery wrapper, and recovery flags. Each installs a cache after reaching its null/empty stage, asserts the existing cache-appearance refusal, and checks the progress cursor remains at the reached stage. The paired successful-prefix cases cover absent-cache progress and preservation of storage/current state. Existing identity and CP225 owner-comparison cases remain separate coverage. An accessor-mirroring test is not needed for a direct boolean conversion relocation.

The original behavior is appropriately qualified by the cited CP123/124/126/127 records: static original-image/decompile branch evidence and declared adapter fixtures, not a fresh accepted-Close witness. The proposed member API and `shared_ptr` representation are not attributed to the original. The leaf makes no claim about native cleanup completion, item-model state, QObject expiry, or accepted Close.

This is a source review only. The report explicitly says no tests ran; root remains responsible for focused and source-stable grouped validation.
