# CP534 graph lifetime correction and conditional `.OPT` unwind

CP532/533 review found two concrete private-model lifetime defects. Raw
Object2d reciprocal peer pointers could dangle when only one FlowItem was
destroyed; a retained notification progress object could accept a newly
allocated row at the same address. FlowItem destruction now removes both
input/output reciprocal edges before its port subobjects die. Its diagnostic
shared lifetime identity is not serialized or part of the m2m C ABI;
progress retains a weak identity and rejects a destroyed row even if an
address is reused. The focused test destroys source and target separately,
checks surviving peers are empty and rejects progress after row destruction.
This lifetime hook is inline because the P11 source cross-check compiles a
namespace-renamed FlowItem double without the original model object file.

An independent immutable-PE audit closed a conditional private `.OPT` writer
state-4 C++ unwind prefix. The separate pure projection requires explicitly
admitted C++ unwind, a retained exception identity and distinct prepared
stream/file/path identities. It publishes successor state before entering
each cleanup action: state4→1 QTextStream, state1→0 QFile, state0→−1 by-value
path QString. A supplied unavailable cleanup return retains the reached
prefix and stops before later actions. Returned actions do not produce a
writer false result or caller continuation. No real destructor, flush,
file close, physical output or public Type3 Save is invoked or admitted.

The original GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`;
QtCore SHA-256 is
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
The `.OPT` writer FuncInfo at `0x1427ba1a8` has 17 cleanup states, zero
local try blocks and 38 IP transitions; its state-4 cleanup chain is
4→1→0→−1. The independent audit verified 27 EXE code spans (2,261 bytes)
and four relevant Qt spans (774 bytes) against the mapped binaries, with
zero gaps or mismatches. Raw FuncInfo hash:
`17db065f92ca875ef4a2466b8d84022a730251a8c5436e555a97fbc67c636c24`;
unwind-map hash:
`62593bcd1ed672c89034e5560d1c2e3444ab5b570ed30e16469341dc3162ffb0`.

The first CP534 full build was invalidated by three independent worker files
appearing during the build and also exposed a namespace-renamed P11 linker
failure. Preserve that failed receipt. The corrected combined build binds
both independent increments to one source hash and two focused targets;
neither changes the public graph or `.OPT` guards. Fresh original
Open/Save/Close comparison remains required.
