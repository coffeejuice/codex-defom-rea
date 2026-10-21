"""Verify retained original post-keyword-import finalization without executing either program."""
from collections import Counter
from copy import deepcopy
import json
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_keyword_import_observations import held_out_scan
from verify_mesh_array_observations import descriptor_map
from verify_mesh_refresh_observations import NODE_BUFFERS, ELEMENT_BUFFERS, SLOTS, bytes_values
from verify_root_layout_observations import header_dimension

DIRECTORY = ROOT / "analysis/oracle/import-finalization-evaluation"
CAPTURE = ROOT / "analysis/import-finalization-runtime-evaluation.json"
MANIFEST = ROOT / "analysis/import-finalization-evaluation-cases.json"
FREEZE = ROOT / "analysis/import-finalization-candidate-freeze.json"
IDS = {"NSTART": (36, 1), "GEOTYP": (28, 1), "STNOUT": (34, 1), "OBJNAM": (144, 2),
       "OBJTYP": (145, 2), "RZ": (159, 2), "ELMCON": (173, 2)}


def provenance(evidence, run, manifest, freeze):
    check(run["status"] == "completed" and run["exit_code"] == 0
          and Path(run["cwd"]) == DIRECTORY, "run completion/directory")
    check(Path(run["executable"]) == ROOT / "source/DEF_PRE_64.exe"
          and digest(Path(run["executable"])) == run["executable_sha256"] == EXE_SHA,
          "original identity")
    verify_files(run, DIRECTORY)
    check(sum((DIRECTORY / n).stat().st_size for n in ("stdout.bin", "stderr.bin"))
          < run["max_output_bytes"], "output bound")
    order = [Path(c["path"]).name for c in manifest["cases"]]
    check(order == run["ordered_imports"] == [x["name"] for x in run["inputs"]]
          and len(order) == len(set(order)) > 0, "fixture order")
    inputs = {}
    for case, item in zip(manifest["cases"], run["inputs"]):
        source, copy = ROOT / case["path"], DIRECTORY / item["name"]
        check(Path(item["source"]) == source and Path(item["copy"]) == copy, "fixture paths")
        check(digest(source) == digest(copy) == case["sha256"] == item["sha256_before"]
              == item["sha256_after"] == item["source_sha256_after"], "fixture hashes")
        data = source.read_bytes().replace(b"\r\n", b"\n")
        check(len(data) < 4096 and data.isascii() and b"\0" not in data and b"\x1a" not in data,
              "fixture stream scope")
        inputs[item["name"]] = data
    cap = evidence["normalized_result"]
    script, gate = DIRECTORY.with_name(DIRECTORY.name + "-probe.js"), DIRECTORY.with_suffix(".ready")
    template = ROOT / "tools/observe_import_finalization.js"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate
          and manifest["probe"]["path"] == "tools/observe_import_finalization.js"
          and digest(template) == manifest["probe"]["sha256"], "probe binding")
    check(script.read_text(encoding="utf-8-sig") ==
          template.read_text(encoding="utf-8-sig").replace("__READY_GATE__", gate.as_posix()),
          "probe differs from passive template")
    pid = read(DIRECTORY.with_suffix(".pid.json"))
    check(pid["pid"] == run["pid"] and pid["executable_sha256"] == EXE_SHA
          and pid["executable"] == run["executable"] and pid["cwd"] == run["cwd"]
          and pid["ready_gate"] == run["ready_gate"], "harness PID binding")
    check(pid["inputs"] == [{k: x[k] for k in ("name", "source", "copy", "sha256_before")}
                             for x in run["inputs"]], "harness fixture binding")
    expected_stdin = "\n2\n" + "".join("1\n" + name + "\n\n" for name in order) + "E\nE\nY\n"
    check(run["stdin"] == expected_stdin, "harness ordered input protocol")
    check(len(freeze["files"]) == 21 and len({f["path"] for f in freeze["files"]}) == 21,
          "freeze inventory")
    for item in freeze["files"]:
        check(digest(ROOT / item["archive"]) == item["sha256"], "frozen candidate archive")
    return inputs


from verify_configuration_import_observations import line_integers


def verify(evidence, run, manifest, inputs, rows):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    pid = run["pid"]
    check(cap["pid"] == params["pid"] == pid and type(pid) is int and pid > 0
          and cap["target"] == f"DEF_PRE_64.exe (pid {pid})", "PID/target")
    check(cap["session_id"] == params["session_id"] and cap["state"] == "detached"
          and cap["mode"] == params["mode"] == "attach"
          and cap["device_id"] == params["device_id"] == "local"
          and cap["source_kind"] == "file", "capture lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
          "incomplete capture")
    messages, scripts = cap["messages"], cap["scripts"]
    check(len(messages)>1 and len(scripts)==1 and all(m["type"]=="send"
          and m["script_id"]==scripts[0]["script_id"] for m in messages), "message inventory")
    hook=messages[0]["payload"]
    check(hook["operation"]=="hooks-installed" and hook["pid"]==pid
          and int(hook["base"],16)>0,"hook identity")
    events=[m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events]==list(range(1,len(events)+1)),"sequence completeness")
    allocations, identities, dimensions, generations = {}, {}, {}, {}
    counts, transitions, units = Counter(), [], set()
    summaries, nstarts, stage_results, header_results = [], [], [], []
    active=token=last_scan=last_state=None
    serial=finalizer_serial=0
    pending=[]
    def buffer(b, name, address):
        check(int(b["slot"], 16) == int(address, 16) + SLOTS[name]
              and b["integers"] == (name != "coordinates"), "buffer slot/type")
        check(b["extent_source"] != "unknown-not-read", "unestablished allocation in complete capture")
        n, k = b["allocated_items"], b["known_prefix"]
        check(type(n) is int and type(k) is int and 0 <= k <= n <= 512
              and b["unknown_items"] == n-k and len(b["values"]) == n, "buffer known extent")
        check(b["values"][k:] == [None] * (n-k), "unknown direct tail not null-masked")
        check(len(bytes_values(b["known_hex"], b["values"][:k], b["integers"]))
              == k * (4 if b["integers"] else 8), "buffer initialized bytes")
        if b["extent_source"] == "null-pointer":
            check(int(b["pointer"], 16) == 0 and n == k == 0, "null buffer extent")
        else:
            check(int(b["pointer"], 16) > 0 and b["extent_source"] in
                  ("observed-calloc-allocator", "observed-preserving-realloc")
                  and type(b["allocation_serial"]) is int, "buffer allocation provenance")

    def state(s, event, linked=False):
        counts["root_snapshots"] += 1
        address, dim = s["address"], s["dimension"]
        check(int(address, 16) > 0 and dim in (2, 3), "root address/dimension")
        if address not in dimensions:
            dimensions[address], generations[address], identities[address] = dim, 0, {}
        elif dimensions[address] != dim:
            transitions.append({"sequence": event["sequence"], "linked": linked,
                                "from": dimensions[address], "to": dim})
            dimensions[address], generations[address], identities[address] = dim, generations[address]+1, {}
        check(len(s["components"]) == 79 and all(type(x) is int and x >= 0 for x in s["components"]),
              "component table bounds")
        check(s["suppress_boundaries"] == s["alternate_enabled"] == 0
              and len(s["output_flags"]) == 7 and all(type(x) is int for x in s["output_flags"]),
              "configuration snapshot scope")
        check(s["node8_default"] == (20 if s["process_units_raw"] == 0 else 68), "unit default")
        units.add(s["process_units_raw"])
        objects = s["objects"]
        check(s["object_count"] == len(objects) <= 8 and [o["id"] for o in objects]
              == list(range(1, len(objects)+1)), "object traversal")
        if linked:
            check(not objects and s["linked_state"] is None, "linked root scope")
        addresses = set()
        for o in objects:
            counts["object_snapshots"] += 1
            check(o["dimension"] == dim and o["components"] == s["components"], "object/root controls")
            check(all(type(o[k]) is int and o[k] >= 0 for k in
                      ("nodes", "elements", "node_allocation_count", "element_allocation_count", "allocation_arity")),
                  "object counts")
            check(o["nodes"] == o["node_allocation_count"] and o["elements"] == o["element_allocation_count"]
                  and o["arity"] in (4, 8) and o["allocation_arity"] ==
                  (o["arity"] if o["element_allocation_count"] else 0), "container/header counts")
            ds = o["descriptors"]
            check(len(ds) == len({d["id"] for d in ds}) == 53, "descriptor inventory")
            identity = (o["address"], tuple(d["address"] for d in ds))
            check(identities[address].setdefault(o["id"], identity) == identity, "generation identity")
            for p in (identity[0], *identity[1]):
                check(int(p, 16) > 0 and p not in addresses, "object/descriptor alias")
                addresses.add(p)
            for d, row in zip(ds, rows):
                counts["descriptor_snapshots"] += 1
                check(d["id"] == row["state_id"] and d["slot_offset"] ==
                      int(row["slot_relative_to_object"], 16) and d["vtable_rva"] ==
                      int(row["vtable_address"], 16)-0x140000000, "descriptor registration")
                check(all(type(d[k]) is int and d[k] >= 0 for k in
                          ("count", "width", "multiplicity", "old_count", "old_width", "old_multiplicity", "length")),
                      "descriptor bounds")
                check(d["length"] == d["count"]*d["width"]*d["multiplicity"] <= 512
                      and d["multiplicity"] == 1 and d["storage_references"] == 1
                      and d["bound_pointer"] == d["storage_pointer"], "descriptor shape/binding")
                check(len(bytes_values(d["values_hex"], d["values"] or [], False)) == d["length"]*8,
                      "descriptor byte extent")
                check(d["length"] == 0 or int(d["storage_pointer"], 16) > 0, "descriptor null payload")
                check(d["values"] is not None or int(d["storage_pointer"], 16) == 0, "descriptor null values")
                if not row["selector_minus_one_included"]:
                    check(d["count"] == 0, "post-only descriptor allocated")
            check(set(o["buffer_metadata"]) == set(SLOTS)
                  and set(o["direct"]) == {str(i) for i in range(18,24)} | {"61"}, "buffer inventory")
            for name, b in o["buffer_metadata"].items():
                buffer(b, name, o["address"])
                container_count=o["element_allocation_count"] if name in ELEMENT_BUFFERS else o["node_allocation_count"]
                width=o["allocation_arity"] if name=="connectivity" else s["components"][
                    0 if name=="coordinates" else int(name[6:])]
                check(b["allocated_items"]==container_count*width,"buffer extent versus container/table")
                key = name[6:] if name.startswith("direct") else name
                check(o["initialized_prefixes"][key] == b["known_prefix"], "prefix projection")
                check((o["direct"][key] if name.startswith("direct") else o[name]) == b["values"],
                      "buffer value projection")
                cached = allocations.get(b["slot"])
                if cached is not None:
                    check({k:v for k,v in b.items() if k not in ("values","known_hex")} ==
                          {k:v for k,v in cached.items() if k not in ("values","known_hex")},
                          "snapshot allocation metadata drift")
                else:
                    check(b["extent_source"] == "null-pointer", "untracked allocated snapshot")
                allocations[b["slot"]] = deepcopy(b)
                counts["unknown_items_in_snapshots"] += b["unknown_items"]
            total = 0
            for group in o["boundary_groups"]:
                if dim == 2:
                    check(group["faces"] == group["owners"] == [] and len(group["nodes"]) >= 2,
                          "planar boundary shape")
                    total += len(group["nodes"])-1
                else:
                    check(group["nodes"] == [] and len(group["faces"]) == 4*len(group["owners"]),
                          "spatial boundary shape")
                    total += len(group["owners"])
            check(total == o["boundary_total"], "boundary total")
        if s["linked_state"] is not None:
            check(not linked and s["linked_state"]["address"] != address
                  and s["linked_state"]["dimension"] == 5-dim, "linked dimension/order")
            state(s["linked_state"], event, True)


    def view(s):
        return {"root":s["address"], "finalization_controls":deepcopy(s["finalization_controls"]),
                **{k:s[k] for k in ("import_flag_0x2f88","import_flag_0x2f89",
                                    "nstart_option_0x1740","material_count")}}

    def event_view(e):
        return {k:e[k] for k in ("root","finalization_controls","import_flag_0x2f88",
                                 "import_flag_0x2f89","nstart_option_0x1740","material_count")}

    def controls_after_headers(s, offset):
        result=deepcopy(s)
        active["dimension_reset_now"]=False
        headers=header_dimension(inputs[active["file"]][:offset])
        if headers and not active["header_seen"]:
            active["header_seen"]=True
            if headers[0]!=s["dimension"]:
                active["dimension_reset"]=True
                active["dimension_reset_now"]=True
                result["finalization_controls"]={"step_number_0x8b0":-1,"saved_step_0x744":-1,
                    "step_increment_0x930":0,"control_0x93c":0,"mode_0x990":1}
                header_results.append({"file":active["file"],"from":s["dimension"],"to":headers[0],
                    "flag_before":s["import_flag_0x2f88"],"sequence":event["sequence"]})
        return result

    for event in events:
        op=event["operation"]
        if op=="import-before":
            check(active is None and len(summaries)<len(inputs)
                  and event["file"]==list(inputs)[len(summaries)],"import order/nesting")
            if last_state is not None: check(event["state"]==last_state,"state drift between imports")
            active={"file":event["file"],"before":deepcopy(event["state"]),"status":0,
                "handlers":[],"scans":0,"reset":0,"header_seen":False,"dimension_reset":False}
            pos,seen,token,last_scan,selected,pending=0,False,None,None,False,[]
        else:
            check(active is not None and event["file"]==active["file"],"event outside import")
            if active["reset"]==0:
                check(op=="flags-reset-before","missing wrapper entry reset")
            elif active["reset"]==1:
                check(op=="flags-reset-after","incomplete wrapper entry reset")
            if "final_after" in active:
                check(op=="import-after","event after finalizer return")
            elif "final_before" in active:
                check(op in ("stage-before","stage-after","object-stage-before",
                             "object-stage-after","finalize-after"),"unexpected event within finalizer")
        if op=="flags-reset-before":
            check(active["reset"]==0 and not seen and token is None
                  and event_view(event)==view(active["before"]),"wrapper pre-parse flag reset entry")
            active["reset_before"]=event_view(event)
            active["reset"]=1
        elif op=="flags-reset-after":
            check(active["reset"]==1,"flag reset pairing")
            expected=deepcopy(active["reset_before"])
            expected["import_flag_0x2f88"]=expected["import_flag_0x2f89"]=0
            check(event_view(event)==expected,"flag reset effects")
            last_state["import_flag_0x2f88"]=last_state["import_flag_0x2f89"]=0
            active["reset"]=2
        elif op=="stream-scan":
            check(active["reset"]==2 and "final_before" not in active,"scan before reset/after finalizer")
            check(token is None or event["format"]=="%i","unpaired handler before token")
            pos,detail=held_out_scan(event,inputs[event["file"]],pos,seen)
            seen,last_scan=True,event
            active["scans"]+=1
        elif op=="keyword-resolved":
            check(token is None and last_scan["format"]=="%39s"
                  and last_scan["value"]==event["token"],"keyword scan pairing")
            expected=IDS.get(event["token"],(305,0))
            check(expected==(event["id"],event["category"]),"keyword mapping")
            if expected==(305,0): continue
            token=dict(event,offset=pos)
            selected=False
        elif op=="object-selected":
            check(token is not None and token["category"]==2 and not selected
                  and last_scan["format"]=="%i" and last_scan["value"]==event["requested"]
                  and event["found"] is True,"object selector pairing")
            selected=True
        elif op in ("allocation-free","allocation-update"):
            names=NODE_BUFFERS if event["kind"]=="nodes" else ELEMENT_BUFFERS
            check(event["kind"] in ("nodes","elements") and set(event["metadata"])==set(names),
                  "allocation event inventory")
            if op=="allocation-update":
                serial+=1
                check(token is not None and event["serial"]==serial and event["mode"] in
                      ("replacement","refresh"),"allocation serial/context")
                n=event["requested_count"]
                check(type(n)is int and n>=0 and event["changed_buffers"]==(names if n>0 else [])
                      and event["selector"]==(-1 if event["mode"]=="replacement" else None),
                      "allocation selection")
                pending.append(event)
            for name,b in event["metadata"].items():
                buffer(b,name,event["object"])
                previous=allocations.get(b["slot"])
                if op=="allocation-free": check(b["extent_source"]=="null-pointer","free left live buffer")
                elif event["requested_count"]<=0:
                    check(previous is None and b["extent_source"]=="null-pointer" or b==previous,
                          "zero-count allocation changed state")
                elif event["mode"]=="replacement":
                    check(b["known_prefix"]==b["allocated_items"] and not any(bytes.fromhex(b["known_hex"])),
                          "replacement allocation not initialized zero")
                else:
                    known=min(previous["known_prefix"] if previous else 0,b["allocated_items"])
                    check(b["known_prefix"]==known and b["known_hex"]==
                          (previous["known_hex"] if previous else "")[:known*(8 if b["integers"] else 16)],
                          "preserving realloc known prefix")
                allocations[b["slot"]]=deepcopy(b)
        elif op=="handler-after":
            check(token is not None and (event["id"],event["category"])==(token["id"],token["category"])
                  and selected==(token["category"]==2),"handler pairing")
            s=event["state"]
            old=controls_after_headers(last_state,token["offset"])
            expected_controls=deepcopy(old["finalization_controls"])
            expected_flag=old["import_flag_0x2f88"]
            if event["id"]==36:
                check(not pending and s["nstart_option_0x1740"]==0,"NSTART mask/allocation scope")
                values,status,end=line_integers(inputs[event["file"]],token["offset"],1)
                check(-(2**31)<=values[0]<2**31,"fixture NSTART int32 scope")
                expected_controls["step_number_0x8b0"]=values[0]
                if status==0:expected_flag=1
                check(event["status"]==status,"NSTART byte-derived status")
                pos=end
                nstarts.append({"file":event["file"],"sequence":event["sequence"],"offset":token["offset"],
                    "consumed_hex":inputs[event["file"]][token["offset"]:end].hex(),
                    "status":status,"stored_step":values[0],"flag":expected_flag})
                if not active["dimension_reset_now"]:
                    unchanged=deepcopy(last_state)
                    unchanged["finalization_controls"]=expected_controls
                    unchanged["import_flag_0x2f88"]=expected_flag
                    check(s==unchanged,"NSTART unrelated state mutation")
            else:
                check(event["category"]==2,"unverified configuration keyword in finalization fixture")
            check(s["finalization_controls"]==expected_controls
                  and s["import_flag_0x2f88"]==expected_flag
                  and s["import_flag_0x2f89"]==old["import_flag_0x2f89"],
                  "handler control/flag mutation or dimension-reset persistence")
            if event["status"]:active["status"]=event["status"]
            active["handlers"].append({"id":event["id"],"status":event["status"]})
            token,pending=None,[]
        elif op=="finalize-before":
            check(token is None and active["reset"]==2 and "final_before" not in active
                  and last_scan["conversions"]==-1 and pos==len(inputs[event["file"]]),
                  "finalizer timing/EOF/pair")
            # A header-only file can reset between the last full snapshot and EOF.
            expected=controls_after_headers(last_state,len(inputs[event["file"]]))
            if event["state"]["dimension"]==last_state["dimension"]:
                check(event["state"]==last_state,"state mutation before finalizer")
            check(event["state"]["finalization_controls"]==expected["finalization_controls"]
                  and event["state"]["import_flag_0x2f88"]==expected["import_flag_0x2f88"]
                  and event["state"]["import_flag_0x2f89"]==expected["import_flag_0x2f89"],
                  "pre-finalizer controls/flag continuity")
            finalizer_serial+=1
            check(event["finalizer_serial"]==finalizer_serial,"finalizer serial")
            active["final_before"]=deepcopy(event["state"])
            active["stage_index"]=0
            active["stage_active"]=None
            active["stage_view"]=view(event["state"])
            check(active["stage_view"]["nstart_option_0x1740"]==0
                  and active["stage_view"]["material_count"]==0,"finalizer selected empty-material scope")
        elif op=="stage-before":
            check("final_before" in active and "final_after" not in active
                  and active["stage_active"] is None and active["stage_index"]<3
                  and event["stage"]==["control","materials","objects"][active["stage_index"]]
                  and event["finalizer_serial"]==finalizer_serial
                  and event_view(event)==active["stage_view"],"finalizer stage order/entry continuity")
            check(event["option"]==(0 if event["stage"]=="objects" else None),"stage option")
            active["stage_active"]=event["stage"]
            if event["stage"]=="objects":
                inventory=[{k:o[k] for k in ("address","id","type","flag_0x3990")}
                           for o in active["final_before"]["objects"]]
                check(event["objects"]==inventory,"object stage inventory")
                active["object_visits"]=[o for o in inventory if o["type"]!=7]
                active["object_index"]=0
                active["object_pending"]=None
            else:check(event["objects"] is None,"non-object stage inventory")
        elif op in ("object-stage-before","object-stage-after"):
            check(active.get("stage_active")=="objects" and event["finalizer_serial"]==finalizer_serial
                  and event["root"]==active["stage_view"]["root"] and event["option"]==0,
                  "object stage nesting/option")
            visits=active["object_visits"]
            check(active["object_index"]<len(visits),"extra object finalizer")
            item={"address":event["object"],**{k:event[k] for k in ("id","type","flag_0x3990")}}
            check(item==visits[active["object_index"]] and item["flag_0x3990"]==0,
                  "object visit order/type7 skip/geometry guard")
            if op=="object-stage-before":
                check(active["object_pending"] is None,"overlapping object stage")
                active["object_pending"]=item
            else:
                check(active["object_pending"]==item,"object stage pairing")
                active["object_pending"]=None
                active["object_index"]+=1
        elif op=="stage-after":
            check(active.get("stage_active")==event["stage"]
                  and event["finalizer_serial"]==finalizer_serial
                  and event["option"]==(0 if event["stage"]=="objects" else None),"stage exit pairing")
            expected=deepcopy(active["stage_view"])
            if event["stage"]=="control":
                c=expected["finalization_controls"]
                if c["control_0x93c"]==0:c["mode_0x990"]=int(c["step_increment_0x930"]<=0)
                check(event["objects"] is None,"control stage inventory")
            elif event["stage"]=="objects":
                check(active["object_pending"] is None and active["object_index"]==len(active["object_visits"]),
                      "missing eligible object finalizer")
                check(event["objects"]==[{k:o[k] for k in ("address","id","type","flag_0x3990")}
                      for o in active["final_before"]["objects"]],"object inventory changed during gated pass")
            else:check(event["objects"] is None,"material stage inventory")
            check(event_view(event)==expected,"stage effects or premature root step update")
            stage_results.append({"file":event["file"],"stage":event["stage"],
                "controls":event["finalization_controls"],"sequence":event["sequence"]})
            active["stage_view"]=expected
            active["stage_active"]=None
            active["stage_index"]+=1
        elif op=="finalize-after":
            check("final_before" in active and "final_after" not in active
                  and active["stage_index"]==3 and active["stage_active"] is None
                  and event["finalizer_serial"]==finalizer_serial,"finalizer completion")
            expected=deepcopy(active["final_before"])
            expected["finalization_controls"]=deepcopy(active["stage_view"]["finalization_controls"])
            if expected["import_flag_0x2f88"]:
                c=expected["finalization_controls"]
                if c["step_number_0x8b0"]>=0:c["step_number_0x8b0"]=-(c["step_number_0x8b0"]+1)
                c["saved_step_0x744"]=c["step_number_0x8b0"]
            check(event["state"]==expected,"finalizer tail effects/other tracked mutation")
            active["final_after"]=deepcopy(event["state"])
        elif op=="import-after":
            check("final_after" in active and event["state"]==active["final_after"]
                  and event["status"]==active["status"],"wrapper result/status aggregation")
            headers=header_dimension(inputs[event["file"]])
            check(event["state"]["dimension"]==(headers[0] if headers else active["before"]["dimension"]),
                  "header dimension rule")
            case=manifest["cases"][len(summaries)]
            s=event["state"]
            c=s["finalization_controls"]
            check(s["dimension"]==case["expected_dimension"]
                  and generations[s["address"]]==case["expected_generation"]
                  and event["status"]==case["expected_status"]
                  and c["step_number_0x8b0"]==case["expected_post_step"]
                  and c["saved_step_0x744"]==case["expected_post_saved_step"]
                  and s["import_flag_0x2f88"]==case["expected_post_import_flag_0x2f88"],
                  "predeclared fixture hypotheses")
            summaries.append({"file":event["file"],"status":event["status"],"scans":active["scans"],
                "handlers":active["handlers"],"before_controls":active["before"]["finalization_controls"],
                "before_flag":active["before"]["import_flag_0x2f88"],
                "after_controls":event["state"]["finalization_controls"],
                "after_flag":event["state"]["import_flag_0x2f88"],"dimension":event["state"]["dimension"],
                "visited_object_ids":[o["id"] for o in active["object_visits"]]})
            active=None
        else:check(op=="import-before","unknown event/probe error")
        if "state" in event:
            s=event["state"]
            check(s["material_count"]==0 and all(o["flag_0x3990"]==0 for o in s["objects"]),
                  "selected material/geometry scope")
            check(s["nstart_option_0x1740"]==0 and s["import_flag_0x2f88"] in (0,1)
                  and s["import_flag_0x2f89"]==0,"unmasked import flag scope")
            state(s,event)
            last_state=deepcopy(s)
    check(active is None and token is None and len(summaries)==len(inputs) and len(units)==1,
          "incomplete trace")
    return {"imports":summaries,"NSTART_handlers":nstarts,"stage_exits":stage_results,
        "dimension_header_observations":header_results,"dimension_transitions":transitions,
        "operation_counts":dict(Counter(e["operation"] for e in events)),"counts":dict(counts),
        "allocation_updates":serial,"process_units_raw":sorted(units)}


def main():
    evidence,run,manifest,freeze=read(CAPTURE),read(DIRECTORY/"run.json"),read(MANIFEST),read(FREEZE)
    inputs=provenance(evidence,run,manifest,freeze)
    rows=descriptor_map(read(ROOT/"analysis/mesh-array-contract.json"))
    result=verify(evidence,run,manifest,inputs,rows)
    mutations=[]

    def pick(e, op, predicate=lambda p: True):
        return next(m["payload"] for m in e["normalized_result"]["messages"]
                    if m["payload"]["operation"]==op and predicate(m["payload"]))

    def reject(label, fn, prov=False):
        e,r,m,f=deepcopy(evidence),deepcopy(run),deepcopy(manifest),deepcopy(freeze)
        fn(e,r,m,f)
        try:
            provenance(e,r,m,f) if prov else verify(e,r,m,inputs,rows)
        except (ValueError,KeyError,TypeError,IndexError) as error:
            mutations.append({"mutation":label,"rejected":True,"reason":str(error)})
            return
        raise ValueError("tampering escaped: "+label)

    reject("PID",lambda e,r,m,f:r.update(pid=1))
    reject("missing event",lambda e,r,m,f:e["normalized_result"]["messages"].pop(10))
    reject("sequence gap",lambda e,r,m,f:pick(e,"stream-scan").update(sequence=999))
    reject("scan bytes",lambda e,r,m,f:pick(e,"stream-scan")["after"].update(buffer_hex="00"))
    reject("pre-reset prior flag",lambda e,r,m,f:pick(e,"flags-reset-before",
        lambda p:p["import_flag_0x2f88"]==1).update(import_flag_0x2f88=0))
    reject("reset fails to clear flag",lambda e,r,m,f:pick(e,"flags-reset-after").update(import_flag_0x2f88=1))
    reject("reset changes step",lambda e,r,m,f:pick(e,"flags-reset-after")["finalization_controls"].update(step_number_0x8b0=17))
    reject("NSTART failed read accepted",lambda e,r,m,f:pick(e,"handler-after",
        lambda p:p["id"]==36 and p["status"]==2).update(status=0))
    reject("NSTART garbage treated as error",lambda e,r,m,f:pick(e,"handler-after",
        lambda p:p["file"]=="08_garbage.KEY").update(status=2))
    reject("NSTART failed read retained destination",lambda e,r,m,f:pick(e,"handler-after",
        lambda p:p["id"]==36 and p["status"]==2)["state"]["finalization_controls"].update(step_number_0x8b0=-7))
    reject("successful then failed read clears success flag",lambda e,r,m,f:pick(e,"handler-after",
        lambda p:p["file"]=="07_success_then_eof.KEY" and p["status"]==2)["state"].update(import_flag_0x2f88=0))
    reject("dimension reset clears import flag",lambda e,r,m,f:pick(e,"handler-after",
        lambda p:p["file"]=="10_dimension_reset.KEY" and p["id"]==144)["state"].update(import_flag_0x2f88=0))
    reject("stage order",lambda e,r,m,f:pick(e,"stage-before").update(stage="materials"))
    reject("stage root",lambda e,r,m,f:pick(e,"stage-before").update(root="0x1"))
    reject("stage serial",lambda e,r,m,f:pick(e,"stage-before").update(finalizer_serial=99))
    reject("control normalization result",lambda e,r,m,f:pick(e,"stage-after",
        lambda p:p["stage"]=="control")["finalization_controls"].update(mode_0x990=0))
    reject("premature step update in material stage",lambda e,r,m,f:pick(e,"stage-after",
        lambda p:p["stage"]=="materials")["finalization_controls"].update(step_number_0x8b0=-5))
    reject("object stage nonzero option",lambda e,r,m,f:pick(e,"object-stage-before").update(option=1))
    reject("type7 object visited",lambda e,r,m,f:pick(e,"object-stage-before").update(type=7))
    reject("wrong object visit identity",lambda e,r,m,f:pick(e,"object-stage-before").update(id=2))
    reject("geometry gate differs",lambda e,r,m,f:pick(e,"object-stage-after").update(flag_0x3990=1))

    def remove_visit(e,r,m,f):
        messages=e["normalized_result"]["messages"]
        for op in ("object-stage-before","object-stage-after"):
            index=next(i for i,x in enumerate(messages) if x["payload"]["operation"]==op)
            messages.pop(index)
        for i,x in enumerate(messages[1:],1):x["payload"]["sequence"]=i
    reject("missing visit pair with repaired sequence",remove_visit)
    reject("negative step transformed twice",lambda e,r,m,f:pick(e,"finalize-after",
        lambda p:p["file"]=="03_negative.KEY")["state"]["finalization_controls"].update(step_number_0x8b0=6))
    reject("saved step not updated",lambda e,r,m,f:pick(e,"finalize-after")["state"]["finalization_controls"].update(saved_step_0x744=-1))
    reject("finalizer clears import flag",lambda e,r,m,f:pick(e,"finalize-after")["state"].update(import_flag_0x2f88=0))
    reject("error return discarded",lambda e,r,m,f:pick(e,"import-after",
        lambda p:p["status"]==2).update(status=0))
    reject("descriptor slot mapping",lambda e,r,m,f:pick(e,"handler-after",
        lambda p:bool(p["state"]["objects"]))["state"]["objects"][0]["descriptors"][0].update(slot_offset=1))
    reject("finalizer mutates descriptor history",lambda e,r,m,f:pick(e,"finalize-after")
        ["state"]["objects"][0]["descriptors"][0].update(old_count=99))
    reject("predeclared fixture outcome",lambda e,r,m,f:m["cases"][0].update(expected_post_step=4))
    reject("fixture hash",lambda e,r,m,f:m["cases"][0].update(sha256="0"*64),True)
    reject("executable hash",lambda e,r,m,f:r.update(executable_sha256="0"*64),True)
    reject("probe hash",lambda e,r,m,f:e["normalized_result"].update(source_sha256="0"*64),True)
    reject("harness order",lambda e,r,m,f:r["ordered_imports"].reverse(),True)
    reject("transcript hash",lambda e,r,m,f:r["files"][0].update(sha256="0"*64),True)
    reject("frozen archive hash",lambda e,r,m,f:f["files"][0].update(sha256="0"*64),True)
    current=[{"path":f["path"],"matches":(ROOT/f["path"]).is_file() and digest(ROOT/f["path"])==f["sha256"]}
             for f in freeze["files"]]
    dependencies=(
        "tools/capture_oracle.py","tools/verify_boundary_sort_observations.py",
        "tools/verify_keyword_import_observations.py","tools/verify_mesh_array_observations.py",
        "tools/verify_mesh_refresh_observations.py","tools/verify_root_layout_observations.py",
        "tools/verify_configuration_import_observations.py","analysis/mesh-array-contract.json",
        "analysis/import-finalization-callsite-audit.json","analysis/keyword-reader.json",
        "analysis/keyword-parser.json","analysis/keyword-handlers-evidence.json",
        "analysis/finalization-0x1401d7f40.json","analysis/finalization-0x1401d7f10.json",
        "analysis/finalization-0x1401d6250.json","analysis/finalization-0x1401d63c0.json",
        "analysis/model-0x1401ee510.json","analysis/model-0x1401ee5a0.json",
        "analysis/model-0x140160d90.json","analysis/model-0x1402059a0.json")
    report={"status":"pass","scope":__doc__,"capture":binding(CAPTURE),
        "evidence_id":evidence["evidence_id"],"pid":run["pid"],"run":binding(DIRECTORY/"run.json"),
        "fixtures":binding(MANIFEST),"probe":binding(DIRECTORY.with_name(DIRECTORY.name+"-probe.js")),
        "probe_template":binding(ROOT/"tools/observe_import_finalization.js"),
        "harness_source":binding(ROOT/"tools/capture_keywords.py"),
        "pid_record":binding(DIRECTORY.with_suffix(".pid.json")),
        "dependencies":[binding(ROOT/p) for p in dependencies],"verifier":binding(Path(__file__)),
        "candidate_freeze":{"manifest":binding(FREEZE),"declared_created_utc":freeze["created_utc"],
            "verified_archives":len(current),"current_matches":current},
        **result,"tampering_checks":mutations,
        "findings":[
            "The enabled wrapper clears both captured import flags before scans, including when the previous successful import left 2f88 set.",
            "All imports reach control, materials and objects in that order; root step encoding/copy happens only after these stages.",
            "Active object visits follow the root inventory and omit type 7. All observed geometry guards are zero, so 2059a0 has an empty guarded body for this subset.",
            "NSTART blank/EOF failures zero the destination, preserve an earlier success flag in the same import and retain status 2 through later successful handlers.",
            "NSTART zero-conversion nonblank text succeeds with stored zero and flag 1; positive, zero, negative, INT_MAX and INT_MIN paths are observed.",
            "The observed dimension change after NSTART resets control step/saved fields to -1 while preserving root 2f88=1. This is a bounded runtime observation, not an exhaustive transitive reset proof.",
            "Across these finalizers the full emitted object, descriptor and direct-buffer state is preserved; finalizer scalar changes follow the retained rules."],
        "limitations":[
            "No original or replacement execution by this verifier; no replacement-generated expected states.",
            "Thirteen short ASCII fixtures, empty materials, unmasked NSTART, geometry gate 0, empty linked roots and one process unit context.",
            "Control normalization is observed only at step_increment=0, control=0 and mode=1. Positive increment, nonzero control and NaN branches remain untested.",
            "The probe does not hook fclose or filename clears; their ordering is static wrapper evidence. Disabled-finalization options, open failures and recursive imports are unobserved.",
            "NSTART lines are interpreted from bound fixture bytes using a bounded independent integer helper, not general native CRT equivalence. Object handler statuses are captured and aggregated, not independently reconstructed here.",
            "Snapshots validate 53 descriptor mappings, shape, values/raw bytes, identity and finalizer preservation. They do not independently predict all object-handler state transitions.",
            "No unknown direct tails occur here. Allocation knowledge is checked but arbitrary preserving growth behavior belongs to the separate refresh evaluation.",
            "Empty-material and zero-geometry gates avoid those payload algorithms; no complete scientific pipeline recovery claim.",
            "Freeze archives are hash-verified. Declared chronology is metadata, not a trusted timestamp; current source matches are informational.",
            "Run/PID/input/transcript/probe bindings verify retained provenance. The harness did not retain a launch-time harness-source digest."]}
    output=ROOT/"analysis/import-finalization-observations.json"
    output.write_text(json.dumps(report,indent=2)+"\n",encoding="utf-8")
    print(f"PASS: {len(result['imports'])} imports; {len(result['NSTART_handlers'])} NSTART handlers; {len(mutations)} tamper checks")


if __name__=="__main__":
    main()
