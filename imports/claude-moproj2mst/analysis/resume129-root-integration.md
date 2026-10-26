# CP129 canonical OPI cache integration

Root added a private `OperationInfoModelCache` member after the stable OPI
identity in `Operation` and exposed only a const accessor. It starts Unknown.
No native item model, default-null receipt, cleanup, or accepted Close action
was added. The private cache rejects a default-null commit on this canonical
instance; detached diagnostics retain their separate API.

The new out-of-line constructor required `operation_info_model_cache.cpp` in
standalone packet targets, the p08/p09 subprojects and the macro-renamed p11
textual cross-check. Root added those dependencies. The first preflight also
found a same-translation-unit anonymous helper name collision in p11; root
renamed the private helper to `unavailableItemModelCache` without changing
its refusal conditions. Failed and repaired preflight logs are retained at
`build/logs/resume129-preflight-build.log`,
`build/logs/resume129-preflight-repair-build.log` and
`build/logs/resume129-p11-repair-build.log`. The repaired focused tests and
full source-bound suite passed.

Final SHA-256: `model.hpp`
`BAF657F01683196D78A45CBB180273CFA8D10B57C0523110171FE4FE235E078D`;
main CMake `2D4E0D5EF2ADDCE9011DF45DB49304E127D20FFD929EE7177AC96916E1D89CB5`;
p08 CMake `4162ABCEAF19AC55452CBB927DDE6083BF53E3C5322791DFBA8090B1A2D0C85F`;
p09 CMake `F6EDCA8CAB6A2B9236A83E74EE0C000B9659CFFB371B7964DE6B2ADC57E62EED`;
p11 cross-check `0DA6511F62388A36CC9DDEC08B352577E54EA7AF9CAC434AF91DCA538E8D02A5`;
cache source `D316E3DEC25A4ED97A389654D2A8D9DE6D69A36858B2A512483CF48AB3305A06`.
