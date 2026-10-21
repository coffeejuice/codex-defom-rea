"""Verify retained original configuration imports without executing either program."""
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

DIRECTORY = ROOT / "analysis/oracle/configuration-import-evaluation"
CAPTURE = ROOT / "analysis/configuration-import-runtime-evaluation.json"
MANIFEST = ROOT / "analysis/configuration-import-evaluation-cases.json"
FREEZE = ROOT / "analysis/configuration-import-candidate-freeze.json"
IDS = {"GEOTYP": (28, 1), "STNOUT": (34, 1), "OBJNAM": (144, 2),
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
          and len(order) == len(set(order)) == 8, "fixture order")
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
    template = ROOT / "tools/observe_configuration_import.js"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate
          and manifest["probe"]["path"] == "tools/observe_configuration_import.js"
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


def line_integers(data, offset, count):
    """Bounded fixture interpretation of 413de0/414600; no candidate code."""
    if offset == len(data):
        return [0] * count, 2, offset
    end = data.find(b"\n", offset)
    end = len(data) if end < 0 else end + 1
    line = data[offset:end].replace(b",", b" ")
    check(len(line) < 1024, "configuration reader line bound")
    values, cursor = [0] * count, 0
    for i in range(count):
        cursor += len(line[cursor:]) - len(line[cursor:].lstrip(b" \t\r\n\v\f"))
        match = re.match(rb"[+-]?(?:0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)", line[cursor:])
        if not match:
            break
        word = match.group()
        unsigned = word.lstrip(b"+-")
        radix = 16 if unsigned.lower().startswith(b"0x") else 8 if unsigned.startswith(b"0") else 10
        values[i] = int(word, radix)
        cursor += len(word)
    # Original helper reports error for EOF/empty scan, not partial conversion.
    return values, (2 if not line.strip() else 0), end


def verify(evidence, run, manifest, inputs, rows):
    cap, params = evidence["normalized_result"], evidence["parameters"]
    check(cap["pid"] == params["pid"] == run["pid"] == 30952
          and cap["target"] == "DEF_PRE_64.exe (pid 30952)", "PID/target")
    check(cap["session_id"] == params["session_id"] and cap["state"] == "detached"
          and cap["mode"] == params["mode"] == "attach"
          and cap["device_id"] == params["device_id"] == "local"
          and cap["source_kind"] == "file", "capture lifecycle")
    check(cap["messages_truncated"] is False and cap["cleanup_error"] is None,
          "incomplete capture")
    messages, scripts = cap["messages"], cap["scripts"]
    check(len(messages) == 208 and len(scripts) == 1 and all(m["type"] == "send"
          and m["script_id"] == scripts[0]["script_id"] for m in messages), "message inventory")
    hook = messages[0]["payload"]
    check(hook["operation"] == "hooks-installed" and hook["pid"] == run["pid"]
          and int(hook["base"], 16) > 0, "hook identity")
    events = [m["payload"] for m in messages[1:]]
    check([e["sequence"] for e in events] == list(range(1, 208)), "sequence completeness")
    allocations, identities, dimensions, generations = {}, {}, {}, {}
    summaries, refreshes, transitions = [], [], []
    counts = Counter()
    serial = 0
    last_state = None
    active = token = last_scan = None
    pending, selected = [], False
    units = set()

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
        check(s["object_count"] == len(objects) <= 3 and [o["id"] for o in objects]
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

    def descriptor_refresh(before, after):
        for x,y,row in zip(before["descriptors"], after["descriptors"], rows):
            n = before["node_allocation_count"] if int(row["slot_relative_to_object"],16)<0x9e0 else before["element_allocation_count"]
            if n <= 0 or not row["selector_minus_one_included"]:
                check(x == y, "unselected descriptor mutated")
                continue
            check(y["count"] == n and y["width"] == after["components"][x["id"]]
                  and y["old_count"] == x["count"] and y["old_width"] == x["width"]
                  and y["old_multiplicity"] == (x["multiplicity"] if x["id"] in (38,43,45)
                                               else x["old_multiplicity"]), "refresh descriptor history")
            raw, size = bytes.fromhex(x["values_hex"]), y["length"]*8
            expected = bytearray(raw[:size] + b"\0"*max(0,size-len(raw)))
            if len(raw) != size and x["id"] == 46 and y["width"] == 9:
                for r in range(n):
                    for c in (0,4,8):
                        expected[(r*9+c)*8:(r*9+c+1)*8] = struct.pack("<d",1)
            check(y["values_hex"] == expected.hex(), "descriptor preserving resize/default refill")
            if size == len(raw):
                check(y["storage_pointer"] == x["storage_pointer"], "same-size descriptor moved")

    for event in events:
        op = event["operation"]
        if op == "import-before":
            check(active is None and len(summaries)<8 and event["file"] == list(inputs)[len(summaries)],
                  "import order/nesting")
            if last_state is not None:
                check(event["state"] == last_state, "state drift between imports")
            active = {"file":event["file"], "before":deepcopy(event["state"]), "handlers":[],
                      "scans":0, "status":0, "sequence_start":event["sequence"]}
            pos, seen, token, last_scan, selected, pending = 0, False, None, None, False, []
            no_consume_pending = False
        else:
            check(active is not None and event["file"] == active["file"], "event outside import")
        if op == "stream-scan":
            check(token is None or event["format"] == "%i", "unpaired handler before next token")
            if no_consume_pending:
                check(event["before"]["cursor"] == pos, "3D GEOTYP consumed remainder")
                no_consume_pending = False
            pos, detail = held_out_scan(event, inputs[event["file"]], pos, seen)
            seen, last_scan = True, event
            active["scans"] += 1
        elif op == "keyword-resolved":
            check(token is None and last_scan["format"] == "%39s"
                  and last_scan["value"] == event["token"]
                  and IDS.get(event["token"]) == (event["id"],event["category"]), "keyword dispatch")
            token = dict(event, offset=pos)
            pending, selected = [], False
        elif op == "object-selected":
            check(token is not None and token["category"] == 2 and not selected
                  and last_scan["format"] == "%i" and last_scan["value"] == event["requested"]
                  and event["found"] is True, "selector pairing")
            selected = True
        elif op in ("allocation-free","allocation-update"):
            names = NODE_BUFFERS if event["kind"] == "nodes" else ELEMENT_BUFFERS
            check(event["kind"] in ("nodes","elements") and set(event["metadata"]) == set(names),
                  "allocation event inventory")
            if op == "allocation-update":
                serial += 1
                check(token is not None and event["serial"] == serial and event["mode"] in
                      ("replacement","refresh"), "allocation serial/context")
                n = event["requested_count"]
                check(type(n) is int and n >= 0 and event["changed_buffers"] == (names if n>0 else [])
                      and event["selector"] == (-1 if event["mode"]=="replacement" else None),
                      "allocation selection")
                pending.append(event)
            for name,b in event["metadata"].items():
                buffer(b,name,event["object"])
                previous = allocations.get(b["slot"])
                if op == "allocation-free":
                    check(b["extent_source"] == "null-pointer", "free left live buffer")
                elif event["requested_count"] <= 0:
                    check(previous is None and b["extent_source"]=="null-pointer" or b==previous,
                          "zero-count allocation changed state")
                elif event["mode"] == "replacement":
                    check(b["known_prefix"] == b["allocated_items"] and not any(bytes.fromhex(b["known_hex"])),
                          "replacement allocation not initialized zero")
                else:
                    old_known = previous["known_prefix"] if previous else 0
                    known = min(old_known,b["allocated_items"])
                    check(b["known_prefix"] == known, "realloc known-prefix propagation")
                    check(b["known_hex"] == (previous["known_hex"] if previous else "")[:known*(8 if b["integers"] else 16)],
                          "realloc changed initialized prefix")
                if op == "allocation-update" and event["requested_count"]>0 and b["extent_source"]!="null-pointer":
                    check(b["allocation_serial"]==serial and b["extent_source"] ==
                          ("observed-calloc-allocator" if event["mode"]=="replacement" else
                           "observed-preserving-realloc"), "allocation serial/source")
                allocations[b["slot"]] = deepcopy(b)
        elif op == "handler-after":
            check(token is not None and (event["id"],event["category"]) == (token["id"],token["category"])
                  and selected == (token["category"]==2), "handler pairing")
            s = event["state"]
            if token["category"] == 1:
                old = last_state
                same_dim = old["dimension"] == s["dimension"]
                flags = list(old["output_flags"]) if same_dim else [0,0,0,0,1,0,0]
                mode = old["geometry_mode"] if same_dim else 1
                ignored = event["id"] == 28 and s["dimension"] == 3
                if ignored:
                    status, end = 0, token["offset"]
                    no_consume_pending = True
                else:
                    values,status,end = line_integers(inputs[event["file"]],token["offset"],
                                                     1 if event["id"]==28 else 9)
                    if event["id"]==28: mode=values[0]
                    else: flags=values[:7]
                check(event["status"]==status and s["geometry_mode"]==mode and s["output_flags"]==flags,
                      "configuration reader status/partial mutation")
                pos = end
                eligible = [o for o in old["objects"] if o["type"]!=7] if same_dim and not ignored else []
                check([(x["object"],x["kind"],x["mode"]) for x in pending] ==
                      [(o["address"],k,"refresh") for o in eligible for k in ("nodes","elements")],
                      "refresh object order/type7 skip")
                if same_dim:
                    check(len(old["objects"])==len(s["objects"]), "refresh object count changed")
                    for a,b in zip(old["objects"],s["objects"]):
                        unchanged = {k:v for k,v in a.items() if k not in
                                     ("components","buffer_metadata","initialized_prefixes","direct",
                                      "coordinates","connectivity","descriptors")}
                        check(all(b[k]==v for k,v in unchanged.items()), "refresh changed unrelated object field")
                        if ignored or a["type"]==7:
                            a=deepcopy(a);a["components"]=b["components"]
                            check(a==b,"ignored/type7 object changed")
                        else:
                            descriptor_refresh(a,b)
                for allocation in pending:
                    o=next(o for o in s["objects"] if o["address"]==allocation["object"])
                    check(allocation["requested_count"] == o["node_allocation_count" if
                          allocation["kind"]=="nodes" else "element_allocation_count"],
                          "refresh used wrong container count")
                    check(allocation["requested_arity"] == (None if allocation["kind"]=="nodes"
                          else o["allocation_arity"]), "refresh used selected rather than allocation arity")
                refreshes.append({"sequence":event["sequence"],"file":event["file"],"id":event["id"],
                                  "status":status,"geometry_mode":mode,"output_flags":flags,
                                  "no_consume":ignored,"resized_object_ids":[o["id"] for o in eligible],
                                  "skipped_type7":[o["id"] for o in old["objects"] if o["type"]==7] if same_dim else []})
            else:
                check(event["status"]==0, "unexpected selected object handler failure")
            for allocation in pending:
                o=next(o for o in s["objects"] if o["address"]==allocation["object"])
                for name,b in allocation["metadata"].items():
                    if allocation["requested_count"]<=0:continue
                    width = allocation["requested_arity"] if name=="connectivity" else s["components"][
                        0 if name=="coordinates" else int(name[6:])]
                    check(b["allocated_items"]==allocation["requested_count"]*width,
                          "allocated extent differs from root table")
                    check(o["buffer_metadata"][name]==b or
                          allocation["mode"]=="replacement", "refresh snapshot differs from allocation")
            if event["status"]:active["status"]=event["status"]
            active["handlers"].append({"id":event["id"],"status":event["status"],"sequence":event["sequence"]})
            token, pending = None, []
        elif op == "finalize-before":
            check(token is None and "final_before" not in active and last_scan["conversions"]==-1
                  and pos==len(inputs[event["file"]]) and event["state"]==last_state,
                  "finalizer before EOF/pair or intervening state drift")
            active["final_before"]=deepcopy(event["state"])
        elif op == "finalize-after":
            check("final_before"in active and "final_after"not in active
                  and event["state"]==active["final_before"], "tracked finalization changed")
            active["final_after"]=deepcopy(event["state"])
        elif op == "import-after":
            check("final_after"in active and event["state"]==active["final_after"]
                  and event["status"]==active["status"], "import status aggregation")
            case=manifest["cases"][len(summaries)]
            s=event["state"]
            header = header_dimension(inputs[event["file"]])
            check(s["dimension"] == (header[0] if header else active["before"]["dimension"]),
                  "root dimension differs from fixture header")
            check(s["dimension"]==case["expected_dimension"] and event["status"]==case["expected_status"]
                  and s["geometry_mode"]==case["expected_geometry_mode"]
                  and s["output_flags"]==case["expected_output_flags"], "predeclared scenario outcome")
            check(generations[s["address"]]==case["expected_generation"], "dimension generation")
            summaries.append({k:v for k,v in active.items() if k not in ("before","final_before","final_after")})
            summaries[-1].update(sequence_end=event["sequence"],dimension=s["dimension"])
            active=None
        else:
            check(op=="import-before", "unknown event/probe error")
        if "state"in event:
            state(event["state"],event)
            last_state=deepcopy(event["state"])
    check(active is None and token is None and len(summaries)==8 and len(units)==1, "incomplete trace")
    return {"imports":summaries,"configuration_handlers":refreshes,"dimension_transitions":transitions,
            "operation_counts":dict(Counter(e["operation"]for e in events)),
            "allocation_updates":serial,"counts":dict(counts),"process_units_raw":sorted(units)}


def main():
    evidence,run,manifest,freeze=read(CAPTURE),read(DIRECTORY/"run.json"),read(MANIFEST),read(FREEZE)
    inputs=provenance(evidence,run,manifest,freeze)
    rows=descriptor_map(read(ROOT/"analysis/mesh-array-contract.json"))
    result=verify(evidence,run,manifest,inputs,rows)
    mutations=[]
    def event(e,seq):return e["normalized_result"]["messages"][seq]["payload"]
    def reject(label,fn,prov=False):
        e,r,m,f=deepcopy(evidence),deepcopy(run),deepcopy(manifest),deepcopy(freeze)
        fn(e,r,m,f)
        try:
            provenance(e,r,m,f) if prov else verify(e,r,m,inputs,rows)
        except (ValueError,KeyError,TypeError,IndexError) as error:
            mutations.append({"mutation":label,"rejected":True,"reason":str(error)})
            return
        raise ValueError("tampering escaped: "+label)
    reject("PID",lambda e,r,m,f:r.update(pid=1))
    reject("missing event",lambda e,r,m,f:e["normalized_result"]["messages"].pop(20))
    reject("sequence gap",lambda e,r,m,f:event(e,4).update(sequence=99))
    reject("scan bytes",lambda e,r,m,f:event(e,3)["after"].update(buffer_hex="00"))
    reject("configuration status",lambda e,r,m,f:event(e,140).update(status=0))
    reject("partial flags",lambda e,r,m,f:event(e,140)["state"]["output_flags"].__setitem__(0,1))
    reject("type7 allocation visit",lambda e,r,m,f:event(e,46).update(object=event(e,48)["state"]["objects"][1]["address"]))
    reject("allocation known prefix",lambda e,r,m,f:event(e,44)["metadata"]["direct18"].update(known_prefix=0))
    reject("allocation extent",lambda e,r,m,f:event(e,44)["metadata"]["direct18"].update(allocated_items=999))
    reject("descriptor binding",lambda e,r,m,f:event(e,48)["state"]["objects"][0]["descriptors"][0].update(bound_pointer="0x1"))
    reject("descriptor history",lambda e,r,m,f:event(e,48)["state"]["objects"][0]["descriptors"][0].update(old_count=99))
    def corrupt_unknown_tail(e,r,m,f):
        for message in e["normalized_result"]["messages"]:
            p=message["payload"]
            if "metadata" not in p:
                continue
            for b in p["metadata"].values():
                if b["unknown_items"]:
                    b["values"][b["known_prefix"]]=0
                    return
        raise AssertionError("missing unknown-tail fixture")
    reject("unknown direct tail asserted zero",corrupt_unknown_tail)
    def corrupt_descriptor_bytes(e,r,m,f):
        d=next(d for d in event(e,48)["state"]["objects"][0]["descriptors"] if d["id"]==2)
        d["values"][-1]=7
        d["values_hex"]=d["values_hex"][:-16]+struct.pack("<d",7).hex()
    reject("consistent raw/value descriptor growth corruption",corrupt_descriptor_bytes)
    reject("3D ignored remainder consumed",lambda e,r,m,f:
           event(e,162)["before"].update(cursor=event(e,162)["before"]["cursor"]+1,
                                         count=event(e,162)["before"]["count"]-1))
    reject("generation hypothesis",lambda e,r,m,f:m["cases"][1].update(expected_generation=9))
    reject("linked dimension",lambda e,r,m,f:event(e,48)["state"]["linked_state"].update(dimension=2))
    reject("finalizer mutation",lambda e,r,m,f:event(e,72)["state"].update(geometry_mode=9))
    reject("fixture hash",lambda e,r,m,f:m["cases"][0].update(sha256="0"*64),True)
    reject("executable hash",lambda e,r,m,f:r.update(executable_sha256="0"*64),True)
    reject("probe hash",lambda e,r,m,f:e["normalized_result"].update(source_sha256="0"*64),True)
    reject("harness order",lambda e,r,m,f:r["ordered_imports"].reverse(),True)
    reject("transcript hash",lambda e,r,m,f:r["files"][0].update(sha256="0"*64),True)
    reject("freeze archive hash",lambda e,r,m,f:f["files"][0].update(sha256="0"*64),True)
    current=[{"path":f["path"],"matches":(ROOT/f["path"]).is_file() and digest(ROOT/f["path"])==f["sha256"]}
             for f in freeze["files"]]
    report={"status":"pass","scope":__doc__,"capture":binding(CAPTURE),"evidence_id":evidence["evidence_id"],
            "run":binding(DIRECTORY/"run.json"),"fixtures":binding(MANIFEST),
            "probe":binding(DIRECTORY.with_name(DIRECTORY.name + "-probe.js")),"probe_template":binding(ROOT/"tools/observe_configuration_import.js"),
            "harness_source":binding(ROOT/"tools/capture_keywords.py"),"pid_record":binding(DIRECTORY.with_suffix(".pid.json")),
            "dependencies":[binding(ROOT/p) for p in (
                "tools/capture_oracle.py","tools/verify_boundary_sort_observations.py",
                "tools/verify_keyword_import_observations.py","tools/verify_mesh_array_observations.py",
                "tools/verify_mesh_refresh_observations.py","tools/verify_root_layout_observations.py",
                "analysis/mesh-array-contract.json","analysis/configuration-refresh-contract.json")],
            "verifier":binding(Path(__file__)),"candidate_freeze":{"manifest":binding(FREEZE),
                "declared_created_utc":freeze["created_utc"],"verified_archives":len(current),"current_matches":current},
            **result,"tampering_checks":mutations,
            "limitations":["No original or replacement execution. Fixture hypotheses are checked; captured arrays remain the oracle.",
                "Eight short ASCII fixtures, empty materials, unit multiplicities, empty linked roots and fixed process unit context.",
                "Raw unknown growth tails are not read or asserted zero; known prefixes are propagated from observed allocations and frees.",
                "Descriptor resize predictions use prior original bytes and recovered allocator rules, not candidate output.",
                "Capture snapshots bound generation identity; no claim about unobserved transitions in arbitrary input streams.",
                "Freeze archive hashes are verified; declared chronology is metadata, not an independently trusted timestamp.",
                "Run/PID/input/transcript/probe bindings substantiate harness provenance; the run does not retain a launch-time harness source digest.",
                "Finalization equality concerns only emitted fields, not all scientific state."]}
    output=ROOT/"analysis/configuration-import-observations.json"
    output.write_text(json.dumps(report,indent=2)+"\n",encoding="utf-8")
    print(f"PASS: {len(result['imports'])} imports; {len(result['configuration_handlers'])} configuration handlers; {len(mutations)} tamper checks")


if __name__=="__main__":
    main()
