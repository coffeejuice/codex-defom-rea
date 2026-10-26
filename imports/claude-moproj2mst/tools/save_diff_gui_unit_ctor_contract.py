"""Qualify GUI-only UNIT constructor initialization and the fresh P17 startup global."""
from pathlib import Path
import hashlib
import json
import struct

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-gui-unit-ctor-contract.json"
if TARGET.exists():
    raise SystemExit("Immutable GUI UNIT constructor contract already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


binary = ROOT / "evidence/gui_mo/DEF_GUI_MO_64.exe"
data = binary.read_bytes()
pe = struct.unpack_from("<I", data, 0x3c)[0]
opt = pe + 24
base = struct.unpack_from("<Q", data, opt + 24)[0]
sh = opt + struct.unpack_from("<H", data, pe + 20)[0]
va = 0x143d79724
region = None
for i in range(struct.unpack_from("<H", data, pe + 6)[0]):
    at = sh + i * 40
    vsize, rva, rawsize, rawpos = struct.unpack_from("<IIII", data, at + 8)
    if rva <= va - base < rva + vsize:
        delta = va - base - rva
        region = {"name": data[at:at + 8].rstrip(b"\0").decode(), "rva": hex(rva),
            "virtual_size": hex(vsize), "raw_size": hex(rawsize), "global_delta": hex(delta),
            "global_is_uninitialized_virtual_tail": delta >= rawsize,
            "global_four_bytes_stay_within_section": delta + 4 <= vsize}
        break
assert region and region["global_is_uninitialized_virtual_tail"] and region["global_four_bytes_stay_within_section"]
files = [
    "analysis/wave2-save-diffs-300-gui-sc-ctor.asm",
    "analysis/wave2-save-diffs-300-gui-sc-owner.asm",
    "analysis/wave2-save-diffs-300-gui-step-ctor.asm",
    "analysis/wave2-save-diffs-300-gui-unit-writer-hints.json",
    "analysis/wave2-save-diffs-300-gui-unit-startup-writes.asm",
    "analysis/wave2-save-diffs-300-gui-unit-settings-store.asm",
    "analysis/wave2-save-diffs-300-gui-unit-accessors.asm",
    "analysis/wave2-save-diffs-300-gui-unit-getter.asm",
    "analysis/wave2-save-diffs-300-gui-unit-radio-getter.asm",
    "source/apps/moproj2mst/src/model/units.cpp",
    "source/apps/moproj2mst/include/m2m/p17.hpp",
    "source/apps/moproj2mst/src/engine/gui_model.cpp",
    "source/port/defmodel/src/step/sim_ctrl.cpp",
    "source/port/defmodel/src/step/db_step_data.cpp",
    "analysis/wave2-save-diffs-300-adjacent-unit-initialization-gap.json",
    "analysis/wave2-save-diffs-300-backup-unit-repair.json",
]
receipt = {
    "status": "original_gui_ctor_unit_store_closed_before_owner_init_shared_root_fix_pending",
    "original_binary": {"path": binary.relative_to(ROOT).as_posix(), "sha256": hashlib.sha256(data).hexdigest()},
    "global": {"va": hex(va), "pe_region": region,
        "fresh_loaded_value": 0,
        "qualification": "Global lies in PE section virtual tail beyond raw bytes, initialized to zero by loader. Initial constant tool expected file backing and failed; no literal file dword value is claimed."},
    "evidence": [record(path) for path in files],
    "gui_order": [
        "GUI SCctor1408d3bc0 constructs curves/furnace, writes owner+bc0=null, then1408d3cac loads global143d79724 into EAX and1408d3cb2 writes UNIT this+fc beforereturn.",
        "GUI DB_STEP_DATA ctor140821bf0 calls SCctor at140821c3e, then owner/init1408d3cd0 at140821ccb. Owner setter writes+bc0 then calls GUIinit1408d3d10. Therefore original UNIT is defined before any environment init, independently of original heap contents.",
        "Inherited DEF_PRE port SimControl ctor sets only owner; conditional GUI variant seed is a genuine missing GUI/native-twin difference, not a justified change to DEF_PRE semantics.",
    ],
    "startup_mapping": {
        "getter": "1403b5580 loads exactglobal143d79724 and returns integer.",
        "setter": "1403b5560 stores suppliedinteger to sameglobal; no validation in this leaf.",
        "new_project": "14001c110(NewProject) at14001c27b stores1 for selectedunit1;294 stores2 for2;2a0 stores1 otherwise. It is not the ordinary fresh FileOpen path.",
        "dialog": "SfProjectNew slotOk140127960 stores140127910 result. Radio getter tests widget+90 and returns2 ifchecked,else1.",
        "p17": "Existing guiStartupUnit() in model/units.cpp returns0 and p17.hpp explicitly documents exactglobal143d79724, freshprocess0, priorNewProject session1/2, and separate C ABI override. This was already the property's constructor startup-unit seam; it can qualify a fresh GUI SCconstructor seed too.",
        "scope": "No complete arbitrary-session state claim; direct setter can hold other values. Fresh single-project Open has no priorNewProject unit writes. Loaded Project::unit/engine UNIT is a later independent state; do not substitute it as original constructor global without qualification.",
    },
    "backup_difference": "Original GUI backup in140ecb340 calls same SCconstructor, so seeds GUIglobal (fresh0), then owner/init, then copies loaded SC includingUNIT. Worker earlier seedbackup.unit=sc.unit defines native initialization but uses loaded1/2 beforeinit; this is an explicit transient adapter choice, not the exact original seed. Copy subsequently overwrites copied UNIT/environment fields; no blanket unobservable-equivalence claim for all omissions. Once root GUI-variant ctor is faithful, this local override should be reconciled at the owned lifecycle boundary.",
    "shared_fix_boundary": "Root owns shared port decision. Existing g_gui_engine_variant distinguishes GUI from DEF_PRE. Seed from an explicitly qualified per-call GUIstartup unit before owner/init on GUIvariant; preserve original DEF_PRE ctor semantics. Do not rely on postconstruction setters to remove earlierUB. Native flags/default-unit policy and tests remain root-owned.",
    "limits": "Read-only originalPE selections/sourcecomparison. No original runtime/privateexports/builds/testexecution/fixturemutation/sharedsourceedits. Raw fragment starts/ends are explicit; no all-writer exhaustiveness claim from byte-pattern scan alone.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
initial = ROOT / "analysis/wave2-save-diffs-300-gui-unit-initial-value.json"
initial.write_text(json.dumps({"status": "not_file_backed_pe_virtual_tail", "va": hex(va),
    "source_sha256": hashlib.sha256(data).hexdigest(), "pe_region": region,
    "fresh_loaded_value": 0, "supersedes": "Initial literal file-value extraction failed because this address is in virtual zero-fill tail."}, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(),
    "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(), "global_region": region}))
