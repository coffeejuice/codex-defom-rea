# Focused regression design for B:120_B_DOE connector edge

## Assigned-file result

`source/apps/moproj2mst/tests/mdt/test_connector_type.cpp` is absent. Do not create it here: this item has no authority to edit shared CMake or register a new target. The existing focused parser test is `source/apps/moproj2mst/tests/p03/test_connector_contract.cpp`; its local `parse(Project&, vector<Item>)` helper constructs the real registry with `makeRegistry(p)`, creates a SubProject, parses XML, and returns the graph in `p.subs[0]->flow.items`.

## Minimal regression to integrate

Add a case to that existing test file using the real registry/parser and just the verified requested edge:

```cpp
Project p;
parse(p, {{"SfOprCogging", 3, 1}, {"SfOprDoeOutput", 6, -1}});
const auto& items = p.subs[0]->flow.items;
REQUIRE(items.size() == 2);
CHECK(items[0]->next == items[1].get());
CHECK(items[1]->prev == items[0].get());
```

This intentionally isolates admission from DOE serialization: operation ids 3 and 6 are taken from the retained complete project, while the helper's parsed two-item document makes the second endpoint index 1. The type combination is source-qualified: Cogging class-init flags `0x1a` give a `Control3d` output; DoeOutput flags `0x20097` give a combined input that accepts it. If desired, add reverse-direction DoeOutput→Cogging as a separate matrix fact only if the saved project requests it; the retained verified edge establishes only Cogging→DoeOutput.

## Fixture identity and limits

The complete-project source is `C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b\120_B_DOE\COG_P1_B1.moproj`, SHA-256 `60ADDC2722E85F92A088FA07F77E8A966928AB5B934F161DF87DCF6A417ED30E`. Its Sub_Project index 1 has item index 2 / operation id 3 / `SfOprCogging` requesting target index 5 / operation id 6 / `SfOprDoeOutput`. The complete file is a reference for class and operation IDs; the two-item test is a minimal synthetic parser fixture and does not claim complete-project parity.

This regression would prove converter registry initialization plus parser admission for the exact type direction. It would not prove that Open selects the second Sub_Project, that the original accepts the edge, or that DOE Save succeeds. Native original admission probe `0x140074f50` retains mocked type/owner callbacks; it is not end-to-end evidence. Current DoeOutput Save behavior has separate unsupported/qualified paths.

## Root-owned integration

Root may add the focused case to the existing P03 source-owned test file, then run `m2m.p03.connector_contract` and the integrated regression against the frozen current source. Record the final source tree hash and test log. A separate fresh original/candidate project comparison should preserve the real two-sub-project structure and capture the second sub-project graph, Open status, Save status/partial file effects, and Close/reset state; label any DOE Save refusal by stage.

Read-only design note only; no test or production source, CMake, build, project file, application, or REA database was changed.
