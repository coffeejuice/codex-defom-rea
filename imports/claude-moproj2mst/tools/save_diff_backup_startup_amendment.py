"""Retain GUI startup-unit reconciliation after the original constructor difference."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-backup-startup-amendment.json"
if TARGET.exists():
    raise SystemExit("Immutable backup startup amendment already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


source = "source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp"
test = "source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp"
cpp, tests = [(ROOT / path).read_text() for path in (source, test)]
declarations = (ROOT / "source/apps/moproj2mst/include/m2m/p17.hpp").read_text()
checks = {
    "original_startup_seed_before_owner_copy": cpp.index("backup.unit = guiStartupUnit();")
        < cpp.index("sim_ctrl_set_owner(backup") < cpp.index("sim_ctrl_copy(&backup"),
    "removed_loaded_seed": "backup.unit = sc.unit;" not in cpp,
    "declaring_headers": '#include "m2m/p17.hpp"' in cpp and '#include "m2m/p17.hpp"' in tests,
    "scope_and_getter_declarations": "class GuiStartupUnitScope" in declarations and "int guiStartupUnit();" in declarations,
    "test_main": "int main() { return testkit::run_all(); }" in tests,
    "no_trailing_whitespace": all(line.rstrip() == line for value in (cpp, tests) for line in value.splitlines()),
}
if not all(checks.values()):
    raise SystemExit(f"Prebuild checks failed: {checks}")
receipt = {
    "status": "gui_startup_backup_amendment_frozen_for_root_build",
    "sources": [record(source), record(test)],
    "lineage": [record(path) for path in [
        "analysis/wave2-save-diffs-300-backup-unit-repair.json",
        "analysis/wave2-save-diffs-300-gui-unit-ctor-contract.json",
        "analysis/wave2-save-diffs-300-gui-unit-setter-qualification.json",
        "analysis/wave2-save-diffs-300-observed-composition-increment.json",
    ]],
    "root_interface_at_handoff": [record(path) for path in [
        "source/apps/moproj2mst/include/m2m/p17.hpp",
        "source/apps/moproj2mst/src/model/units.cpp",
        "source/port/defmodel/include/defmodel/step/db_step.hpp",
        "source/port/defmodel/src/step/sim_ctrl.cpp",
    ]],
    "original": "GUI1408d3bc0 stores global143d79724 toUNIT beforeowner/init. GUI backup constructor in140ecb340 uses that same global, then owner/init, then loadedSC field-awarecopy. Fresh global0 is loader-zero-filled, distinct from later loadedUNIT1/2; prior UI sessions can change global. No original heap-value assumption remains for GUIUNIT.",
    "amendment": "Replace backup.unit=sc.unit with backup.unit=guiStartupUnit() before owner/init, using root's per-call thread_local startup seam. Root's GUI-only native constructor seed now establishes the same value; explicit local seed also documents the original boundary. Preserve later sim_ctrl_copy in both directions and original omissions. No DEF_PRE source changes made by worker.",
    "test": "Existing lifecycle unit case now runs six startup/loaded pairs:startup0/1/2 xloaded1/2. Assert actualGuiModel constructorUNIT equals startup before loadedfixture setup, verify customloaded environment/snapshots survive Open and remembered text applies, and assert applying loadedUNIT neverchanges startupglobal. Scope restores prior startup state after each pair. No existing assertions weakened; lifecycle casecount remains7.",
    "checks": checks,
    "validation": "Cheap API/header/main/source-order/whitespace inspection only. No workerbuild/test/runtime/fixturewrites. Parent119 checkpoint predates current amendment/composition/rootGUIseed and is not acceptance evidence for them. Source is frozen immediately after receipt.",
    "limits": "Root owns shared GUI startup scope and native variant initialization; their policy/validation is separate. Public300 guard remains, remesh source not started, whole application recovery incomplete.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(), "checks": checks}))
