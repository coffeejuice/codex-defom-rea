"""Verify retained original scalar time controls and post-import finalization without executing either program."""
from collections import Counter
from copy import deepcopy
import json
import argparse
from pathlib import Path
import re
import struct

from capture_oracle import digest
from verify_boundary_sort_observations import ROOT, EXE_SHA, binding, check, read, verify_files
from verify_keyword_import_observations import held_out_scan
from verify_configuration_import_observations import line_integers
from verify_mesh_array_observations import descriptor_map
from verify_mesh_refresh_observations import NODE_BUFFERS, ELEMENT_BUFFERS, SLOTS, bytes_values
from verify_root_layout_observations import header_dimension

DIRECTORY = ROOT / "analysis/oracle/time-controls-evaluation"
CAPTURE = ROOT / "analysis/time-controls-runtime-evaluation.json"
MANIFEST = ROOT / "analysis/time-controls-evaluation-cases.json"
FREEZE = ROOT / "analysis/time-controls-candidate-freeze.json"
HELD_OUT = False
IDS = {"DSMAX": (48, 1), "DTMAX": (50, 1), "GEOTYP":(28,1), "STNOUT":(34,1),
       "OBJNAM":(144,2), "OBJTYP":(145,2), "RZ":(159,2), "ELMCON":(173,2)}


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
          and len(order) == len(set(order)) == (13 if HELD_OUT else 16), "fixture order")
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
    template = ROOT / "tools/observe_time_controls.js"
    check(Path(cap["source_path"]) == script and digest(script) == cap["source_sha256"]
          and Path(run["ready_gate"]) == gate
          and manifest["probe"]["path"] == "tools/observe_time_controls.js"
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
    check(len(freeze["files"]) == 23 and len({f["path"] for f in freeze["files"]}) == 23,
          "freeze inventory")
    for item in freeze["files"]:
        check(digest(ROOT / item["archive"]) == item["sha256"], "frozen candidate archive")
    return inputs



FLOAT_KEYS=("step_increment_0x930","time_step_0x920","time_step_secondary_0x928")
VIEW_KEYS=("finalization_controls","time_controls_hex","import_flag_0x2f88",
           "import_flag_0x2f89","nstart_option_0x1740","material_count")
ZERO="0000000000000000"
NEG_ZERO="0000000000000080"


def bounded_decimal_scan(line,count):
    """Independent retained-grammar check, limited to these finite fixture decimals."""
    data=line.replace(b",",b" ")
    cursor,conversions,values=0,0,[0.0]*count
    for i in range(count):
        cursor+=len(data[cursor:])-len(data[cursor:].lstrip(b" \t\r\n\v\f"))
        match=re.match(rb"[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)",data[cursor:])
        if not match:
            # The native first conversion can consume a sign/point and reach
            # input exhaustion. A following LF remains a matching failure.
            prefix=re.match(rb"[+-]?\.?",data[cursor:]).group()
            cursor+=len(prefix)
            if conversions==0 and cursor==len(data):conversions=-1
            break
        significand=match.group()
        cursor+=len(significand)
        exponent=re.match(rb"[eE][+-]?[0-9]*",data[cursor:])
        word=significand
        if exponent:
            piece=exponent.group();cursor+=len(piece)
            if re.search(rb"[0-9]",piece):word+=piece
        values[i]=float(word)
        check(abs(values[i])!=float("inf"),"finite fixture numeric domain")
        conversions+=1
    return conversions,values,data


def stream_position(s,data):
    check(type(s["cursor"]) is int and type(s["count"]) is int
          and 0<=s["cursor"]<=4096 and -1<=s["count"]<=4096,"reader stream bounds")
    if s["buffer_hex"]=="":
        check(s["cursor"]==s["count"]==0,"EOF buffer reset")
        return len(data)
    check(s["buffer_hex"]==data.hex() and s["cursor"]+s["count"]==len(data),"reader buffer identity")
    return s["cursor"]


def verify_real(e,data,offset,previous_scan,keyword):
    count=1 if keyword==48 else 2
    check(e["kind"]==("real1" if count==1 else "real2") and e["before"]==previous_scan["after"],
          "real helper identity/stream continuity")
    check(stream_position(e["before"],data)==offset,"real helper input offset")
    end=data.find(b"\n",offset)
    end=min(len(data),offset+1023,end+1 if end>=0 else len(data))
    line=data[offset:end]
    eof=offset==len(data)
    check(e["lines"]==[{"limit":1024,"eof":eof,"hex":None if eof else line.hex()}],
          "real helper exact line/EOF")
    expected_after={"cursor":end,"count":len(data)-end,"buffer_hex":data.hex()}
    if eof or (end==len(data) and not line.endswith(b"\n")):
        expected_after={"cursor":0,"count":0,"buffer_hex":""}
    check(e["after"]==expected_after and stream_position(e["after"],data)==end,
          "real helper after-line cursor/reset")
    if eof:
        conversions,values=-1,[0.0]*count
        check(e["scans"]==[],"physical EOF must not scan")
    else:
        conversions,values,scan_input=bounded_decimal_scan(line,count)
        check(e["scans"]==[{"input_hex":scan_input.hex(),"format":" ".join(["%Flf"]*count),
                            "conversions":conversions}],"real scanner bytes/format/conversions")
    raw=[struct.pack("<d",x).hex() for x in values]
    check(e["status"]==int(conversions==-1) and e["values"]==values and e["output_hex"]==raw,
          "real helper status/zeroing/partial raw output")
    return end,{"file":e["file"],"sequence":e["sequence"],"kind":e["kind"],"offset":offset,
        "line_hex":line.hex(),"physical_eof":eof,"scanner_conversions":None if eof else conversions,
        "status":e["status"],"values":e["values"],"output_hex":raw}


def check_view(s):
    check(s["material_count"]==0 and s["nstart_option_0x1740"]==0
          and s["import_flag_0x2f88"]==s["import_flag_0x2f89"]==0,"selected root scope")
    c=s["finalization_controls"]
    check(set(c)==set(FLOAT_KEYS)|{"step_number_0x8b0","saved_step_0x744","control_0x93c","mode_0x990"},
          "control inventory")
    check(c["step_number_0x8b0"]==c["saved_step_0x744"]==-1 and c["control_0x93c"]==0
          and type(c["mode_0x990"])is int and c["mode_0x990"]in(0,1),"unmodified step/table guard scope")
    check(set(s["time_controls_hex"])==set(FLOAT_KEYS),"raw scalar inventory")
    for k in FLOAT_KEYS:
        raw=bytes.fromhex(s["time_controls_hex"][k])
        check(len(raw)==8 and struct.unpack("<d",raw)[0]==c[k],"scalar binary64/raw value")


def check_state(s,linked=False):
    check_view(s)
    check(int(s["address"],16)>0 and s["dimension"] in (2,3)
          and s["object_count"]==len(s["objects"])<=3,"root/dimension scope")
    check(s["alternate_enabled"]==s["suppress_boundaries"]==0
          and len(s["components"])==79 and all(type(x)is int and x>=0 for x in s["components"])
          and len(s["output_flags"])==7 and all(type(x)is int for x in s["output_flags"]),"root profile shape")
    check(s["node8_default"]==(20 if s["process_units_raw"]==0 else 68),"unit context")
    if linked:check(s["linked_state"]is None and not s["objects"],"linked recursion/empty scope")
    elif s["linked_state"]is not None:
        check(s["linked_state"]["address"]!=s["address"],"linked root alias")
        check_state(s["linked_state"],True)


def view(s):
    return {"root":s["address"],**{k:deepcopy(s[k])for k in VIEW_KEYS}}


def event_view(e):
    return {"root":e["root"],**{k:e[k]for k in VIEW_KEYS}}


def verify(evidence,run,manifest,inputs):
    cap,params=evidence["normalized_result"],evidence["parameters"]
    pid=run["pid"]
    check(type(pid)is int and pid>0 and cap["pid"]==params["pid"]==pid
          and cap["target"]==f"DEF_PRE_64.exe (pid {pid})","PID/target")
    check(cap["session_id"]==params["session_id"] and cap["state"]=="detached"
          and cap["mode"]==params["mode"]=="attach"
          and cap["device_id"]==params["device_id"]=="local"
          and cap["source_kind"]=="file" and cap["messages_truncated"]is False
          and cap["cleanup_error"]is None,"complete capture lifecycle")
    messages,scripts=cap["messages"],cap["scripts"]
    check(len(messages)>1 and len(scripts)==1 and all(m["type"]=="send"
          and m["script_id"]==scripts[0]["script_id"]for m in messages),"message inventory")
    hook=messages[0]["payload"]
    check(hook["operation"]=="hooks-installed" and hook["pid"]==pid and int(hook["base"],16)>0,
          "hook identity")
    events=[m["payload"]for m in messages[1:]]
    check([e["sequence"]for e in events]==list(range(1,len(events)+1)),"sequence completeness")
    active=last_state=token=last_scan=row=None
    imports,rows,handlers,stages=[],[],[],[]
    mappings=descriptor_map(read(ROOT/"analysis/mesh-array-contract.json"))
    allocations,identities,dimensions,generations={},{},{},{}
    counts,transitions,units=Counter(),[],set()
    allocation_serial=0
    configuration=[]
    final_serial=0
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
            for d, row in zip(ds, mappings):
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
        for x,y,row in zip(before["descriptors"], after["descriptors"], mappings):
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
        op=event["operation"]
        if op=="import-before":
            check(active is None and len(imports)<len(inputs) and event["file"]==list(inputs)[len(imports)],
                  "import order/nesting")
            if last_state is not None:check(event["state"]==last_state,"between-import state continuity")
            else:
                for initial in (event["state"],event["state"]["linked_state"]):
                    if initial is not None:
                        check(all(initial["time_controls_hex"][k]==ZERO for k in FLOAT_KEYS)
                              and initial["finalization_controls"]["mode_0x990"]==1,
                              "constructor scalar baseline")
            active={"file":event["file"],"status":0,"reset":0,"scans":0,"handlers":0,
                    "before_controls":event["state"]["finalization_controls"],"header_seen":False}
            pos,seen,token,last_scan,row=0,False,None,None,None
            selected,pending=False,[]
            strict_next_scan=False
        else:
            check(active is not None and event["file"]==active["file"],"event outside import")
            if active["reset"]<2:
                check(op==["flags-reset-before","flags-reset-after"][active["reset"]],"wrapper reset order")
            if "final_after"in active:check(op=="import-after","event after finalizer")
            elif "final_before"in active:
                check(op in("stage-before","stage-after","object-stage-before","object-stage-after","finalize-after"),"unexpected finalizer event")
        if op=="flags-reset-before":
            check(event_view(event)==view(last_state),"reset entry state")
            active["reset"]=1
        elif op=="flags-reset-after":
            check(event_view(event)==view(last_state),"reset effects in zero-flag scope")
            active["reset"]=2
        elif op=="stream-scan":
            check((token is None or event["format"]=="%i") and row is None
                  and "final_before"not in active,"scan/handler pairing")
            if last_scan is not None and token is None and (not HELD_OUT or strict_next_scan):
                check(stream_position(event["before"],inputs[event["file"]])==pos,"stream gap after helper")
            pos,_=held_out_scan(event,inputs[event["file"]],pos,seen)
            seen,last_scan=True,event
            active["scans"]+=1
        elif op=="keyword-resolved":
            check(token is None and last_scan["format"]=="%39s" and last_scan["value"]==event["token"]
                  and IDS.get(event["token"])==(event["id"],event["category"]),"keyword mapping/pairing")
            token=dict(event,offset=pos)
            selected,pending=False,[]
        elif op=="real-row":
            check(token is not None and token["id"] in (48,50) and row is None,"real helper pairing")
            pos,row=verify_real(event,inputs[event["file"]],token["offset"],last_scan,token["id"])
            rows.append(row)
        elif op=="object-selected":
            check(token is not None and token["category"]==2 and not selected
                  and last_scan["format"]=="%i" and last_scan["value"]==event["requested"]
                  and event["found"] is True,"object selector pairing")
            selected=True
        elif op in ("allocation-free","allocation-update"):
            names=NODE_BUFFERS if event["kind"]=="nodes" else ELEMENT_BUFFERS
            check(event["kind"]in("nodes","elements") and set(event["metadata"])==set(names),
                  "allocation inventory")
            if op=="allocation-update":
                allocation_serial+=1
                check(token is not None and event["serial"]==allocation_serial
                      and event["mode"]in("replacement","refresh"),"allocation serial/context")
                n=event["requested_count"]
                check(type(n)is int and n>=0 and event["changed_buffers"]==(names if n>0 else [])
                      and event["selector"]==(-1 if event["mode"]=="replacement" else None),
                      "allocation selection")
                pending.append(event)
            for name,b in event["metadata"].items():
                buffer(b,name,event["object"])
                previous=allocations.get(b["slot"])
                if op=="allocation-free":check(b["extent_source"]=="null-pointer","free left live buffer")
                elif event["requested_count"]<=0:
                    check(previous is None and b["extent_source"]=="null-pointer" or b==previous,
                          "zero-count allocation changed state")
                elif event["mode"]=="replacement":
                    check(b["known_prefix"]==b["allocated_items"] and not any(bytes.fromhex(b["known_hex"])),
                          "replacement allocation not initialized zero")
                else:
                    known=min(previous["known_prefix"] if previous else 0,b["allocated_items"])
                    check(b["known_prefix"]==known and b["known_hex"]==
                          (previous["known_hex"]if previous else "")[:known*(8 if b["integers"]else 16)],
                          "preserving realloc known prefix")
                if op=="allocation-update" and event["requested_count"]>0 and b["extent_source"]!="null-pointer":
                    check(b["allocation_serial"]==allocation_serial and b["extent_source"]==
                          ("observed-calloc-allocator" if event["mode"]=="replacement" else
                           "observed-preserving-realloc"),"allocation metadata provenance")
                allocations[b["slot"]]=deepcopy(b)
        elif op=="handler-after":
            check(token is not None and (event["id"],event["category"])==(token["id"],token["category"])
                  and selected==(token["category"]==2),"handler pairing")
            scalar=event["id"]in(48,50)
            expected=deepcopy(last_state)
            c,h=expected["finalization_controls"],expected["time_controls_hex"]
            if scalar:
                check(row is not None and event["status"]==2*row["status"] and not pending,
                      "scalar helper/status pairing")
                if event["id"]==48:
                    c[FLOAT_KEYS[0]]=row["values"][0];h[FLOAT_KEYS[0]]=row["output_hex"][0]
                    positive=c[FLOAT_KEYS[0]]>0
                    if positive:c[FLOAT_KEYS[1]]=0;h[FLOAT_KEYS[1]]=ZERO
                else:
                    for k,v,bits in zip(FLOAT_KEYS[1:],row["values"],row["output_hex"]):c[k]=v;h[k]=bits
                    positive=c[FLOAT_KEYS[1]]>0
                    if positive:c[FLOAT_KEYS[0]]=0;h[FLOAT_KEYS[0]]=ZERO
                if positive and c["control_0x93c"]==0:c["mode_0x990"]=int(c[FLOAT_KEYS[0]]<=0)
                check(event["state"]==expected,"immediate positive-only scalar effects/preservation")
                handlers.append({"file":event["file"],"sequence":event["sequence"],"id":event["id"],
                    "status":event["status"],"positive_branch":positive,"controls":c,"raw":h})
            else:
                check(HELD_OUT and row is None,"non-scalar fixture scope")
                actual=event["state"]
                headers=header_dimension(inputs[event["file"]][:token["offset"]])
                if headers and not active["header_seen"]:
                    active["header_seen"]=True
                    if headers[0]!=last_state["dimension"]:
                        c.update({k:0.0 for k in FLOAT_KEYS});c["mode_0x990"]=1
                        c["step_number_0x8b0"]=c["saved_step_0x744"]=-1;c["control_0x93c"]=0
                        h.update({k:ZERO for k in FLOAT_KEYS})
                check(actual["finalization_controls"]==c and actual["time_controls_hex"]==h,
                      "non-scalar time-control preservation/reset")
                if token["category"]==1:
                    old=last_state;same_dim=old["dimension"]==actual["dimension"]
                    flags=list(old["output_flags"])if same_dim else[0,0,0,0,1,0,0]
                    mode=old["geometry_mode"]if same_dim else 1
                    check(event["id"]in(28,34) and actual["dimension"]==2,"configuration fixture scope")
                    values,status,end=line_integers(inputs[event["file"]],token["offset"],
                                                   1 if event["id"]==28 else 9)
                    if event["id"]==28:mode=values[0]
                    else:flags=values[:7]
                    check(event["status"]==status and actual["geometry_mode"]==mode
                          and actual["output_flags"]==flags,"configuration reader mutation/status")
                    pos=end
                    eligible=[o for o in old["objects"]if o["type"]!=7]if same_dim else[]
                    check([(x["object"],x["kind"],x["mode"])for x in pending]==
                          [(o["address"],k,"refresh")for o in eligible for k in("nodes","elements")],
                          "configuration refresh order/type7 skip")
                    if same_dim:
                        check(len(old["objects"])==len(actual["objects"]),"configuration changed object count")
                        for a,b in zip(old["objects"],actual["objects"]):
                            unchanged={k:v for k,v in a.items()if k not in
                                ("components","buffer_metadata","initialized_prefixes","direct",
                                 "coordinates","connectivity","descriptors")}
                            check(all(b[k]==v for k,v in unchanged.items()),"refresh unrelated object mutation")
                            if a["type"]==7:
                                a=deepcopy(a);a["components"]=b["components"]
                                check(a==b,"inactive object refreshed")
                            else:descriptor_refresh(a,b)
                    configuration.append({"sequence":event["sequence"],"id":event["id"],
                        "status":status,"visited_ids":[o["id"]for o in eligible],
                        "skipped_ids":[o["id"]for o in old["objects"]if o["type"]==7]if same_dim else[]})
                else:check(event["status"]==0,"selected mesh setup handler failure")
                for allocation in pending:
                    obj=next(o for o in actual["objects"]if o["address"]==allocation["object"])
                    kind=allocation["kind"]
                    check(allocation["requested_count"]==obj["node_allocation_count"if kind=="nodes"else"element_allocation_count"]
                          and allocation["requested_arity"]==(None if kind=="nodes"else obj["allocation_arity"]),
                          "allocation uses container counts/arity")
                    if allocation["requested_count"]>0:
                        for name,b in allocation["metadata"].items():
                            width=allocation["requested_arity"]if name=="connectivity"else actual["components"][0 if name=="coordinates"else int(name[6:])]
                            check(b["allocated_items"]==allocation["requested_count"]*width,"allocation extent versus table")
                            if allocation["mode"]=="refresh":check(obj["buffer_metadata"][name]==b,"refresh metadata mismatch")
            if event["status"]:active["status"]=event["status"]
            active["handlers"]+=1
            strict_next_scan=scalar or token["category"]==1
            token=row=None;pending=[]
        elif op=="finalize-before":
            check(token is None and row is None and "final_before"not in active
                  and last_scan["conversions"]==-1 and pos==len(inputs[event["file"]])
                  and event["state"]==last_state,"finalizer timing/EOF/state")
            final_serial+=1
            check(event["finalizer_serial"]==final_serial,"finalizer serial")
            active["final_before"]=deepcopy(last_state)
            active["stage_index"]=0;active["stage_active"]=None;active["stage_view"]=view(last_state)
        elif op=="stage-before":
            index=active["stage_index"]
            check(active["stage_active"]is None and index<3
                  and event["stage"]==["control","materials","objects"][index]
                  and event["finalizer_serial"]==final_serial
                  and event_view(event)==active["stage_view"],"stage order/entry state")
            inventory=[{k:o[k]for k in("address","id","type","flag_0x3990")}
                       for o in active["final_before"]["objects"]]
            check(event["option"]==(0 if index==2 else None)
                  and event["objects"]==(inventory if index==2 else None),"stage option/inventory")
            if index==2:
                active["visits"]=[o for o in inventory if o["type"]!=7]
                active["visit_index"]=0;active["visit_pending"]=None
            active["stage_active"]=event["stage"]
        elif op in("object-stage-before","object-stage-after"):
            check(active.get("stage_active")=="objects" and event["finalizer_serial"]==final_serial
                  and event["root"]==active["stage_view"]["root"] and event["option"]==0,
                  "object stage nesting/option")
            item={"address":event["object"],**{k:event[k]for k in("id","type","flag_0x3990")}}
            check(active["visit_index"]<len(active["visits"])
                  and item==active["visits"][active["visit_index"]]and item["flag_0x3990"]==0,
                  "eligible object visit identity/guard")
            if op=="object-stage-before":
                check(active["visit_pending"]is None,"overlapping object visits")
                active["visit_pending"]=item
            else:
                check(active["visit_pending"]==item,"object visit pairing")
                active["visit_pending"]=None;active["visit_index"]+=1
        elif op=="stage-after":
            index=active["stage_index"]
            check(active["stage_active"]==event["stage"] and event["finalizer_serial"]==final_serial
                  and event["option"]==(0 if index==2 else None)
                  and event["objects"]==([{k:o[k]for k in("address","id","type","flag_0x3990")}
                     for o in active["final_before"]["objects"]]if index==2 else None),"stage exit pairing")
            if index==2:
                check(active["visit_pending"]is None and active["visit_index"]==len(active["visits"]),
                      "missing eligible object visit")
            expected=deepcopy(active["stage_view"])
            c=expected["finalization_controls"]
            if index==0 and c["control_0x93c"]==0:c["mode_0x990"]=int(c[FLOAT_KEYS[0]]<=0)
            check(event_view(event)==expected,"finalizer stage effect/raw scalar preservation")
            stages.append({"file":event["file"],"stage":event["stage"],"sequence":event["sequence"],
                "controls":c,"raw":expected["time_controls_hex"]})
            active["stage_view"]=expected;active["stage_index"]+=1;active["stage_active"]=None
        elif op=="finalize-after":
            check(active.get("stage_index")==3 and active["stage_active"]is None
                  and "final_after"not in active and event["finalizer_serial"]==final_serial,
                  "finalizer completion")
            expected=deepcopy(active["final_before"])
            expected["finalization_controls"]=deepcopy(active["stage_view"]["finalization_controls"])
            check(event["state"]==expected,"finalizer result/unrelated state mutation")
            active["final_after"]=deepcopy(expected)
        elif op=="import-after":
            check("final_after"in active and event["state"]==active["final_after"]
                  and event["status"]==active["status"],"wrapper final state/status aggregation")
            imports.append({"file":event["file"],"status":event["status"],"scans":active["scans"],
                "handlers":active["handlers"],"before_controls":active["before_controls"],
                "before_finalizer":active["final_before"]["finalization_controls"],
                "after_controls":event["state"]["finalization_controls"],"raw":event["state"]["time_controls_hex"]})
            active=None
        else:check(op=="import-before","unknown event/probe error")
        if "state"in event:
            check_state(event["state"])
            state(event["state"],event)
            last_state=deepcopy(event["state"])
        elif "finalization_controls"in event:check_view(event)
    check(active is None and token is None and row is None and len(imports)==len(inputs),"incomplete imports")
    return {"imports":imports,"real_rows":rows,"handlers":handlers,"stages":stages,
        "operation_counts":dict(Counter(e["operation"]for e in events)),
        "messages_including_hook":len(messages),"sequenced_events":len(events),"configuration_handlers":configuration,
        "allocation_updates":allocation_serial,"mesh_counts":dict(counts),"dimension_transitions":transitions}


def main():
    global DIRECTORY,CAPTURE,MANIFEST,FREEZE,HELD_OUT
    parser=argparse.ArgumentParser();parser.add_argument("--held-out",action="store_true")
    HELD_OUT=parser.parse_args().held_out
    if HELD_OUT:
        DIRECTORY=ROOT/"analysis/oracle/time-controls-held-out"
        CAPTURE=ROOT/"analysis/time-controls-runtime-held-out.json"
        MANIFEST=ROOT/"analysis/time-controls-held-out-cases.json"
        FREEZE=ROOT/"analysis/time-controls-corrected-freeze.json"
    evidence,run,manifest,freeze=read(CAPTURE),read(DIRECTORY/"run.json"),read(MANIFEST),read(FREEZE)
    inputs=provenance(evidence,run,manifest,freeze)
    result=verify(evidence,run,manifest,inputs)
    mutations=[]

    def pick(e,op,predicate=lambda x:True):
        return next(m["payload"]for m in e["normalized_result"]["messages"]
                    if m["payload"]["operation"]==op and predicate(m["payload"]))

    def reject(label,fn,prov=False):
        e,r,m,f=deepcopy(evidence),deepcopy(run),deepcopy(manifest),deepcopy(freeze)
        fn(e,r,m,f)
        try:
            provenance(e,r,m,f) if prov else verify(e,r,m,inputs)
        except (ValueError,KeyError,TypeError,IndexError) as error:
            mutations.append({"mutation":label,"rejected":True,"reason":str(error)})
            return
        raise ValueError("tampering escaped: "+label)

    if not HELD_OUT:
        reject("PID",lambda e,r,m,f:r.update(pid=1))
        reject("missing event",lambda e,r,m,f:e["normalized_result"]["messages"].pop(10))
        reject("sequence gap",lambda e,r,m,f:pick(e,"real-row").update(sequence=999))
        reject("token buffer corruption",lambda e,r,m,f:pick(e,"stream-scan")["after"].update(buffer_hex="00"))
        reject("helper kind",lambda e,r,m,f:pick(e,"real-row").update(kind="real2"))
        reject("helper line bytes",lambda e,r,m,f:pick(e,"real-row")["lines"][0].update(hex="20310a"))
        reject("helper line bound",lambda e,r,m,f:pick(e,"real-row")["lines"][0].update(limit=2048))
        reject("physical EOF reported as empty line",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["lines"][0]["eof"])["lines"][0].update(eof=False,hex=""))
        reject("comma replacement omitted",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["lines"][0]["hex"] is not None and "2c"in p["lines"][0]["hex"])
            ["scans"][0].update(input_hex="202c202c0a"))
        reject("scanner format",lambda e,r,m,f:pick(e,"real-row")["scans"][0].update(format="%lf"))
        reject("EOF sign accepted",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["file"]=="10_real1_plus_eof.KEY")["scans"][0].update(conversions=0))
        reject("newline sign rejected",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["file"]=="07_failed_signs_newline.KEY")["scans"][0].update(conversions=-1))
        reject("partial pair counted as full",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["kind"]=="real2" and p["scans"] and p["scans"][0]["conversions"]==1)
            ["scans"][0].update(conversions=2))
        reject("reader status",lambda e,r,m,f:pick(e,"real-row",lambda p:p["status"]==1).update(status=0))
        reject("negative-zero output lost",lambda e,r,m,f:pick(e,"real-row",
            lambda p:NEG_ZERO in p["output_hex"])["output_hex"].__setitem__(0,ZERO))
        def corrupt_unassigned(e,r,m,f):
            p=pick(e,"real-row",lambda p:p["kind"]=="real2" and p["scans"]
                   and p["scans"][0]["conversions"]==1)
            p["values"][1]=7;p["output_hex"][1]=struct.pack("<d",7).hex()
        reject("unassigned second output retained nonzero with consistent raw bytes",corrupt_unassigned)
        reject("helper EOF cursor not reset",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["file"]=="10_real1_plus_eof.KEY")["after"].update(cursor=7))
        reject("handler status hides helper error",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["status"]==2).update(status=0))
        def lose_secondary(e,r,m,f):
            p=pick(e,"handler-after",lambda p:p["id"]==48
                and p["state"]["finalization_controls"][FLOAT_KEYS[0]]==3)
            p["state"]["finalization_controls"][FLOAT_KEYS[2]]=0
            p["state"]["time_controls_hex"][FLOAT_KEYS[2]]=ZERO
        reject("positive DSMAX incorrectly clears secondary 928",lose_secondary)
        def keep_other_primary(e,r,m,f):
            p=pick(e,"handler-after",lambda p:p["id"]==50)
            p["state"]["finalization_controls"][FLOAT_KEYS[0]]=2.5
            p["state"]["time_controls_hex"][FLOAT_KEYS[0]]=struct.pack("<d",2.5).hex()
        reject("positive DTMAX fails to clear930",keep_other_primary)
        reject("nonpositive scalar normalizes mode too early",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["id"]==48 and p["state"]["finalization_controls"][FLOAT_KEYS[0]]==-2)
            ["state"]["finalization_controls"].update(mode_0x990=1))
        reject("unrelated step field mutation",lambda e,r,m,f:pick(e,"handler-after")
            ["state"]["finalization_controls"].update(saved_step_0x744=0))
        reject("root negative-zero bits lost",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["file"]=="06_negative_zero.KEY")["state"]["time_controls_hex"].update(step_increment_0x930=ZERO))
        reject("reset changed scalar",lambda e,r,m,f:pick(e,"flags-reset-after")
            ["finalization_controls"].update(time_step_0x920=1))
        reject("stage order",lambda e,r,m,f:pick(e,"stage-before").update(stage="materials"))
        reject("stage root",lambda e,r,m,f:pick(e,"stage-before").update(root="0x1"))
        reject("control stage normalization omitted",lambda e,r,m,f:pick(e,"stage-after",
            lambda p:p["stage"]=="control")["finalization_controls"].update(mode_0x990=0))
        reject("empty material stage changed raw scalar",lambda e,r,m,f:pick(e,"stage-after",
            lambda p:p["stage"]=="materials")["time_controls_hex"].update(time_step_secondary_0x928=ZERO))
        reject("invented eligible object",lambda e,r,m,f:pick(e,"stage-before",
            lambda p:p["stage"]=="objects").update(objects=[{"id":1}]))
        def remove_stage(e,r,m,f):
            messages=e["normalized_result"]["messages"]
            i=next(i for i,m in enumerate(messages)if m["payload"]["operation"]=="stage-after")
            messages.pop(i)
            for i,m in enumerate(messages[1:],1):m["payload"]["sequence"]=i
        reject("missing stage exit with repaired sequence",remove_stage)
        reject("finalizer changes raw negative zero",lambda e,r,m,f:pick(e,"finalize-after",
            lambda p:p["file"]=="06_negative_zero.KEY")["state"]["time_controls_hex"].update(time_step_0x920=ZERO))
        reject("wrapper discards remembered error",lambda e,r,m,f:pick(e,"import-after",
            lambda p:p["status"]==2).update(status=0))
        reject("fixture hash",lambda e,r,m,f:m["cases"][0].update(sha256="0"*64),True)
        reject("executable hash",lambda e,r,m,f:r.update(executable_sha256="0"*64),True)
        reject("probe hash",lambda e,r,m,f:e["normalized_result"].update(source_sha256="0"*64),True)
        reject("harness order",lambda e,r,m,f:r["ordered_imports"].reverse(),True)
        reject("transcript hash",lambda e,r,m,f:r["files"][0].update(sha256="0"*64),True)
        reject("freeze archive hash",lambda e,r,m,f:f["files"][0].update(sha256="0"*64),True)
    else:
        reject("PID",lambda e,r,m,f:r.update(pid=1))
        reject("missing event",lambda e,r,m,f:e["normalized_result"]["messages"].pop(10))
        reject("sequence gap",lambda e,r,m,f:pick(e,"real-row").update(sequence=999))
        reject("token buffer",lambda e,r,m,f:pick(e,"stream-scan")["after"].update(buffer_hex="00"))
        reject("helper input bytes",lambda e,r,m,f:pick(e,"real-row")["lines"][0].update(hex="20310a"))
        reject("helper bound",lambda e,r,m,f:pick(e,"real-row")["lines"][0].update(limit=2048))
        reject("scanner format",lambda e,r,m,f:pick(e,"real-row")["scans"][0].update(format="%lf"))
        reject("sign-dot EOF accepted",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["file"]=="03_plus_dot_eof.KEY")["scans"][0].update(conversions=0))
        reject("delimited sign-dot rejected",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["file"]=="02_failed_items_delimited.KEY")["scans"][0].update(conversions=-1))
        reject("partial EOF pair failed",lambda e,r,m,f:pick(e,"real-row",
            lambda p:p["file"]=="08_partial_minus_eof.KEY")["scans"][0].update(conversions=-1))
        reject("helper status",lambda e,r,m,f:pick(e,"real-row",lambda p:p["status"]==1).update(status=0))
        reject("negative-zero helper lost",lambda e,r,m,f:pick(e,"real-row",
            lambda p:NEG_ZERO in p["output_hex"])["output_hex"].__setitem__(0,ZERO))
        def corrupt_partial(e,r,m,f):
            p=pick(e,"real-row",lambda p:p["kind"]=="real2"and p["scans"][0]["conversions"]==1)
            p["values"][1]=7;p["output_hex"][1]=struct.pack("<d",7).hex()
        reject("consistent partial-output corruption",corrupt_partial)
        reject("handler suppresses read error",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["status"]==2).update(status=0))
        def erase_secondary(e,r,m,f):
            p=pick(e,"handler-after",lambda p:p["id"]==48 and
                   p["state"]["finalization_controls"][FLOAT_KEYS[0]]>0 and
                   p["state"]["finalization_controls"][FLOAT_KEYS[2]]!=0)
            p["state"]["finalization_controls"][FLOAT_KEYS[2]]=0
            p["state"]["time_controls_hex"][FLOAT_KEYS[2]]=ZERO
        reject("DSMAX clears preserved secondary",erase_secondary)
        reject("early nonpositive normalization",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["id"]==48 and p["state"]["finalization_controls"][FLOAT_KEYS[0]]<=0
            and p["state"]["finalization_controls"]["mode_0x990"]==0)
            ["state"]["finalization_controls"].update(mode_0x990=1))
        reject("wrong stage order",lambda e,r,m,f:pick(e,"stage-before").update(stage="materials"))
        reject("wrong stage root",lambda e,r,m,f:pick(e,"stage-before").update(root="0x1"))
        reject("finalizer mode",lambda e,r,m,f:pick(e,"stage-after",
            lambda p:p["stage"]=="control")["finalization_controls"].update(mode_0x990=9))
        reject("wrapper status",lambda e,r,m,f:pick(e,"import-after",lambda p:p["status"]==2).update(status=0))
        reject("allocation extent",lambda e,r,m,f:pick(e,"allocation-update")
            ["metadata"]["coordinates"].update(allocated_items=999))
        reject("allocation container count",lambda e,r,m,f:pick(e,"allocation-update").update(requested_count=99))
        reject("configuration mutation",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["id"]==28)["state"].update(geometry_mode=99))
        reject("refresh descriptor history",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["id"]==34 and bool(p["state"]["objects"]))
            ["state"]["objects"][0]["descriptors"][0].update(old_count=99))
        reject("descriptor registration",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:bool(p["state"]["objects"]))["state"]["objects"][0]["descriptors"][0].update(slot_offset=1))
        def corrupt_unknown(e,r,m,f):
            p=pick(e,"allocation-update",lambda p:any(b["unknown_items"]for b in p["metadata"].values()))
            b=next(b for b in p["metadata"].values()if b["unknown_items"])
            b["values"][b["known_prefix"]]=0
        reject("unknown direct tail asserted zero",corrupt_unknown)
        def corrupt_growth(e,r,m,f):
            p=pick(e,"handler-after",lambda p:p["id"]==34 and bool(p["state"]["objects"]))
            d=next(d for d in p["state"]["objects"][0]["descriptors"]if d["length"]>0)
            d["values"][-1]=7;d["values_hex"]=d["values_hex"][:-16]+struct.pack("<d",7).hex()
        reject("consistent descriptor growth corruption",corrupt_growth)
        reject("type7 finalizer visit",lambda e,r,m,f:pick(e,"object-stage-before").update(id=2,type=7))
        def remove_object_visit(e,r,m,f):
            messages=e["normalized_result"]["messages"]
            for op in("object-stage-before","object-stage-after"):
                i=next(i for i,x in enumerate(messages)if x["payload"]["operation"]==op)
                messages.pop(i)
            for i,x in enumerate(messages[1:],1):x["payload"]["sequence"]=i
        reject("missing object visit pair with repaired sequence",remove_object_visit)
        reject("object finalizer option",lambda e,r,m,f:pick(e,"object-stage-before").update(option=1))
        reject("geometry guard",lambda e,r,m,f:pick(e,"object-stage-after").update(flag_0x3990=1))
        reject("scalar mutation of mesh",lambda e,r,m,f:pick(e,"handler-after",
            lambda p:p["id"]==48 and bool(p["state"]["objects"]))["state"]["objects"][0].update(mesh_flag=0))
        reject("finalizer changes raw zero sign",lambda e,r,m,f:pick(e,"finalize-after",
            lambda p:NEG_ZERO in p["state"]["time_controls_hex"].values())
            ["state"]["time_controls_hex"].update(time_step_secondary_0x928=ZERO))
        reject("fixture hash",lambda e,r,m,f:m["cases"][0].update(sha256="0"*64),True)
        reject("executable hash",lambda e,r,m,f:r.update(executable_sha256="0"*64),True)
        reject("probe hash",lambda e,r,m,f:e["normalized_result"].update(source_sha256="0"*64),True)
        reject("harness order",lambda e,r,m,f:r["ordered_imports"].reverse(),True)
        reject("transcript hash",lambda e,r,m,f:r["files"][0].update(sha256="0"*64),True)
        reject("freeze archive hash",lambda e,r,m,f:f["files"][0].update(sha256="0"*64),True)
    current=[{"path":f["path"],"matches":(ROOT/f["path"]).is_file() and digest(ROOT/f["path"])==f["sha256"]}
             for f in freeze["files"]]
    negatives=[{"file":r["file"],"sequence":r["sequence"],"output_hex":r["output_hex"]}
               for r in result["real_rows"]if NEG_ZERO in r["output_hex"]]
    borders=[r for r in result["real_rows"]if "signs"in r["file"]or r["file"].startswith(("10_","11_","12_","13_","14_","15_"))]
    deps=("tools/capture_oracle.py","tools/verify_boundary_sort_observations.py",
          "tools/verify_keyword_import_observations.py","analysis/time-controls-static-audit.json",
          "analysis/mesh-refresh-controls-pseudocode.json","analysis/keyword-catalog.json",
          "analysis/time-controls-0x140414990.json","analysis/time-controls-0x140414a40.json",
          "analysis/model-0x1401ee510.json","analysis/model-0x1401ee5a0.json",
          "analysis/finalization-0x1401f4390.json","analysis/finalization-0x1401d7f40.json",
          "tools/verify_configuration_import_observations.py","tools/verify_mesh_array_observations.py",
          "tools/verify_mesh_refresh_observations.py","tools/verify_root_layout_observations.py",
          "analysis/mesh-array-contract.json","analysis/configuration-refresh-contract.json")
    report={"status":"pass","scope":__doc__,"capture":binding(CAPTURE),
        "evidence_id":evidence["evidence_id"],"pid":run["pid"],"run":binding(DIRECTORY/"run.json"),
        "fixtures":binding(MANIFEST),"probe":binding(DIRECTORY.with_name(DIRECTORY.name+"-probe.js")),
        "probe_template":binding(ROOT/"tools/observe_time_controls.js"),
        "pid_record":binding(DIRECTORY.with_suffix(".pid.json")),
        "harness_source":binding(ROOT/"tools/capture_keywords.py"),
        "verifier":binding(Path(__file__)),"dependencies":[binding(ROOT/p)for p in deps],
        "candidate_freeze":{"manifest":binding(FREEZE),"declared_created_utc":freeze["created_utc"],
            "verified_archives":len(current),"current_matches":current},
        **result,"negative_zero_observations":negatives,"first_conversion_border_observations":borders,
        "tampering_checks":mutations,
        "findings":[
            "All 16 fixture files, source/copy hashes, target, harness PID/input/output records, passive probe template and 23 frozen archives verified.",
            "The 39 real-helper calls each have one bounded fgets record. Their scan input exactly matches the line after comma replacement; physical EOF has no sscanf call.",
            "Zero/unassigned outputs, partial values, conversion counts, statuses and binary64 bytes agree with the bounded independent fixture parser. A failed read does not retain its old destination.",
            "Positive DSMAX clears only primary 920 and preserves secondary 928. Positive DTMAX clears 930 even when its second conversion is missing. Nonpositive values do not trigger mutual clearing or immediate mode normalization.",
            "All 16 finalizers run control, materials and objects in order, including after import status 2. The control stage normalizes 990 from 930; later empty stages preserve it.",
            "Negative zero is retained as 0000000000000080 in all three scalar fields and through finalization. JSON numbers alone cannot establish this property.",
            "Lone first-field +,-,. at physical EOF return scanner -1/helper 1; the same tokens followed by LF return scanner 0/helper 0. A pair containing 2 then + at EOF returns scanner 1/helper 0.",
            "The initial reader review's first-conversion EOF concern is confirmed by original evidence. This verifier neither executes nor assumes a corrected replacement."],
        "limitations":[
            "No original or replacement program execution. Only the independent Python evidence verifier runs.",
            "Sixteen small ASCII fixtures, dimension 3 primary/empty linked dimension 2, no objects or materials, option 1740=0 and guard 93c=0. No populated mesh or geometry pass coverage in this run.",
            "Destination zeroing before fgets is established statically. Probe return snapshots corroborate zero outputs on failure/unassigned fields but do not hook the exact zero-store instructions.",
            "The bounded decimal interpreter validates these finite numbers only. General rounding, overflow, underflow, locale and nonfinite values are outside scope.",
            "No DTTYPE, nonzero 93c pinned-mode branch, dimension reset or disabled finalization is exercised here.",
            "Raw scalar preservation and complete emitted snapshot equality do not prove unobserved model fields unchanged.",
            "The first-field EOF distinction is an observed discovery. This report does not relabel it as a prediction made before capture.",
            "Frozen archives are verified; current source/binary matches are informational. Declared chronology is not a trusted timestamp, and no launch-time harness source digest was retained."]}
    if HELD_OUT:
        report["first_conversion_border_observations"]=[r for r in result["real_rows"]
            if r["file"]!="13_populated_planar.KEY"]
        report["findings"]=[
            "All 13 bound held-out files, target/probe/PID/transcript records and 23 corrected frozen archives verified.",
            "The 26 exact helper calls validate sign-dot exhaustion, invalid trailing characters, partial pairs, incomplete exponents and signed-zero raw bytes against independent fixture interpretation.",
            "Every scalar mutation follows positive-only mutual clearing and preserves the secondary DTMAX field when required; enabled finalizers normalize mode only in the control stage.",
            "The final file changes to 2D, creates a populated planar mesh, an inactive type7 placeholder and an empty active object, then executes GEOTYP/STNOUT refreshes before scalar imports.",
            "All allocation extents, descriptor bindings/history/zero-growth, known byte prefixes and masked unknown tails are checked. Type7 objects are skipped by refresh and finalization; both eligible objects are visited by the finalizer.",
            "Scalar handlers and finalizers preserve the full captured planar object, descriptor, direct-buffer and boundary state except their established scalar fields."]
        report["limitations"]=[
            "No original or replacement program execution. Independent evidence verification only; no replacement-generated expected values.",
            "The first 12 files use empty primary and linked roots. The final compound case has one planar mesh, one inactive placeholder and one empty active object; broader topology/model coverage is not implied.",
            "Empty materials, zero geometry gate, option1740=0 and guard93c=0. DTTYPE, pinned mode and scientific geometry/material algorithms remain untested.",
            "The verifier checks selected mesh setup structure and original allocation/refresh invariants. It does not recompute boundary topology or every object handler value from fixture bytes.",
            "Unknown direct growth tails are null-masked and never inferred zero. Known prefix preservation and descriptor zero-growth are separate contracts.",
            "The bounded decimal interpreter validates these finite fixture values, not general rounding, range, locale or nonfinite conversion behavior.",
            "Exact store-instruction timing of pre-read zeroing is static evidence; returned zero outputs corroborate it but are not a hook on those writes.",
            "Corrected freeze archives are verified; declared chronology is metadata rather than a trusted timestamp. Current source matches are informational.",
            "No launch-time harness source digest was retained. Current harness binding is recorded alongside retained PID/input/output evidence."]
    output=ROOT/("analysis/time-controls-held-out-observations.json" if HELD_OUT else
                 "analysis/time-controls-observations.json")
    output.write_text(json.dumps(report,indent=2)+"\n",encoding="utf-8")
    print(f"PASS: {len(result['imports'])} imports; {len(result['real_rows'])} real helpers; {len(mutations)} tamper checks")


if __name__=="__main__":
    main()
