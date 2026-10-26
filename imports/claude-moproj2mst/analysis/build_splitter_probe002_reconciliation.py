"""Compact source-qualified view of immutable parent-owned probe002 evidence."""
import hashlib
import json
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def binding(name):
    p = ROOT / name
    return {"path": name, "sha256": hashlib.sha256(p.read_bytes()).hexdigest()}


raw = json.loads((ROOT / "build/original-splitter-probe-002/capture-evidence.json").read_text())
evidence = raw["structuredContent"]
messages = [m.get("payload", {}) for m in evidence["normalized_result"]["messages"]]
states = {m["stateId"]: m["data"] for m in messages if m.get("kind") == "state"}
observations = [m for m in messages if m.get("kind") == "observation"]
roles = {m["object"]: m["role"] for m in messages if m.get("kind") == "role"}
focus = []
comparisons = []
for m in observations:
    d = m["data"]
    if m["stage"].startswith("focus_"):
        links = d["links"]
        child = links["child"]
        focus.append({"ordinal": m["ordinal"], "stage": m["stage"],
                      "receiver": roles[child["object"]],
                      "oldWindow": roles.get(d.get("oldWindow"), d.get("oldWindow")),
                      "currentWindow": roles.get(d["currentWindowWalk"].get("window")),
                      "walkComplete": d["currentWindowWalk"]["complete"],
                      "sameWindowNoOp": d.get("sameWindowNoOp"),
                      "childNext": roles.get(child["next"], "untracked"),
                      "childPrevious": roles.get(child["previous"], "untracked"),
                      "childFocusChildNull": child["focusChild"] == "0x0",
                      "windowPrevious": roles.get(links["currentWindow"]["previous"], "untracked"),
                      "entryPredecessor": links["entryPredecessor"]["trackedRole"],
                      "entryPredecessorNext": roles.get(links["entryPredecessor"]["next"], "untracked")})
    if "native_equality" in m["stage"]:
        comparisons.append({"ordinal": m["ordinal"], "kind": m["stage"], "context": d["context"],
                            "nativeReturnAL": d["nativeReturnAL"],
                            "sameData": d["left"]["data"] == d["right"]["data"],
                            "leftMask": d["left"].get("mask"), "rightMask": d["right"].get("mask")})

result = {
    "status": "Independent bounded reconciliation; no worker launch, GUI control or build",
    "evidence_id": evidence["evidence_id"],
    "bindings": [binding(p) for p in [
        "build/original-splitter-probe-002/capture-evidence.json",
        "build/original-splitter-probe-002/probe-reviewed-v2.js",
        "analysis/wave3-original-splitter-probe-002-receipt.json",
        "analysis/wave3-close-splitter-probe-002-focus-review.json",
        "analysis/wave3-close-splitter-post-value-parent-contract.json",
        "analysis/wave3-closure-next-close-splitter-parent-font-preparation-static.json",
        "analysis/wave3-closure-next-close-splitter-font-global-getters-static.json"]],
    "counts": dict(Counter(m.get("kind") for m in messages)),
    "completion": next(m for m in messages if m.get("kind") == "capture_complete_paused"),
    "codepages": next(m for m in messages if m.get("kind") == "codepages"),
    "initial_parent_profile": {"stateId": 1, "windowFlags": states[1]["widgetData"]["windowFlags"],
                               "attributesLow": states[1]["widgetData"]["attributesLow"],
                               "extended": states[1]["extended"], "extra": states[1]["extra"],
                               "sameThreadAsManagerAndSplitter": states[1]["threadData"] == states[2]["threadData"] == states[4]["threadData"],
                               "splitterHookNull": states[4]["hookData"] == "0x0"},
    "initial_globals": {"stateId": 3, **states[3]},
    "focus": focus,
    "comparisons": comparisons,
    "grid": [{"ordinal": m["ordinal"], "stage": m["stage"], "data": m["data"]}
             for m in observations if m["stage"].startswith("grid_")],
    "source_conclusions": [
        "First splitter focus context has null focusChild and self next/previous. Native ring partition therefore iterates zero times; its entry predecessor is the same actual manager at final splice. Four native writes match ord4/5.",
        "Only MainView.previous=manager and manager.next=MainView are initially bound. Earlier QTabWidget ring nodes remain unreconstructed; no complete ring inferred.",
        "All five new widgets have self-rings at focus entry. Last splitter reparent to manager has old/new window MainView and native early return; entry predecessor was not consumed by a splice.",
        "All18 native equality calls returntrue, same raw data and zero font/palette masks. This closes final comparison outcomes, not all preceding application-font/palette getter/cache effects.",
        "First receiver and parent selected style pointers are null, both widget stylesheets empty, and application stylesheet empty. Conditional inheritStyle no-persistent-effect branch is source-bound only once earlier value preparation is resolved.",
        "Actual grid factory returns QWidgetItemV2 bound to same splitter; grid box owns that item with raw bounds0,0,0,0. Final item list has one entry; cachedGrid identity remains unchanged. This observation does not itself implement earlier parenting/constructor callbacks."
    ],
    "implementation_boundary": "Advance actual initial parent/focus ownership and restore attribute0x2a, retaining ParentFontPreparationPending before application font lookup/cache work. No completed SplitView, placeholder population, name/orientation, notification aliases or public Close parity claimed.",
    "limitations": [
        "No event/filter trace was repeated in probe002. Probe001 callback populations remain independently phase-qualified.",
        "Parent walk is bounded32 and entry predecessor explicitly pre-state. Only first self-ring branch has source proof no partition changes that predecessor.",
        "Grid/layout/font/value snapshots have finite original script bounds; completion reports no truncation/errors for this capture.",
        "Process cleanup and loaded-file hash verification belong to root's receipt; no debugger cleanup is represented as GUI release."
    ]
}
target = ROOT / "analysis/wave3-close-splitter-probe-002-reconciliation.json"
assert not target.exists()
target.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"path": str(target.relative_to(ROOT)),
                  "sha256": hashlib.sha256(target.read_bytes()).hexdigest(),
                  "bytes": target.stat().st_size, "focus_records": len(focus),
                  "native_comparisons": len(comparisons)}))
