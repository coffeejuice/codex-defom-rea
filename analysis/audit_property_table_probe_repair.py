"""Audit bounded property probe continuation reachability without executing the target."""
import hashlib
import json
import re
import struct
from pathlib import Path


def bind(path):
    return {"path": str(path).replace("\\", "/"), "sha256": hashlib.sha256(Path(path).read_bytes()).hexdigest()}


def main():
    prior = Path("analysis/property-table-precapture-audit.json")
    assert bind(prior)["sha256"] == "dec734f8ec2450d05727d023192e5852aebc7cc2bfbadb14826d82d348356c09"
    audit = json.loads(Path("analysis/property-table-byte-audit.json").read_text())
    base = 0x140000000
    instructions = []
    paths = ["analysis/property-table-readers-dumpbin.txt", "analysis/property-table-material-dispatch-dumpbin.txt"]
    for path in paths:
        for line in Path(path).read_text(encoding="utf-8-sig").splitlines():
            m = re.match(r"^\s+([0-9A-F]{16}): ((?:[0-9A-F]{2} )+)\s*(\S+)(?:\s+(.*))?$", line)
            if m:
                instructions.append(dict(address=int(m[1],16), raw=bytes.fromhex(m[2]), mnemonic=m[3], operands=m[4] or ""))
            elif instructions and re.fullmatch(r"\s+(?:[0-9A-F]{2}\s*)+", line):
                instructions[-1]["raw"] += bytes.fromhex(line)
    pe = Path("source/DEF_PRE_64.exe").read_bytes()
    off = struct.unpack_from("<I", pe, 0x3c)[0]
    n = struct.unpack_from("<H", pe, off+6)[0]
    sec = off+24+struct.unpack_from("<H",pe,off+20)[0]
    sections = [struct.unpack_from("<IIII",pe,sec+i*40+8) for i in range(n)]
    def native(a, count):
        for _, rva, length, raw in sections:
            if base+rva <= a and a+count <= base+rva+length:
                return pe[raw+a-base-rva:raw+a-base-rva+count]
        raise AssertionError("unmapped bytes")
    for x in instructions:
        assert native(x["address"],len(x["raw"])) == x["raw"]
    hooks = [0x41ab19,0x41abab,0x41abc8,0x41b34f,0x41b379,0x41b651,0x385b7a]
    hooks += [int(c["return"],16)-base for c in audit["cases"]]
    results=[]
    for rva in hooks:
        address=base+rva
        previous=next(x for x in instructions if x["address"]+len(x["raw"])==address)
        assert previous["mnemonic"]=="call" and previous["raw"][0]==0xe8
        branches=[]
        for x in instructions:
            if x["mnemonic"].startswith("j") and re.fullmatch("[0-9A-F]{16}",x["operands"]) and int(x["operands"],16)==address:
                b=x["raw"]
                displacement=struct.unpack("<i",b[-4:])[0] if len(b)>2 else struct.unpack("<b",b[-1:])[0]
                assert x["address"]+len(b)+displacement==address
                branches.append({"address":hex(x["address"]),"bytes":b.hex(),"mnemonic":x["mnemonic"]})
        if rva==0x41abc8:
            assert [int(x["address"],16)-base for x in branches]==[0x41a89d,0x41a8b1,0x41a93c,0x41aa33]
        else:
            assert not branches,hex(rva)
        assert hex(address) not in audit["header_jump_table"]
        assert all(int(c["case_entry"],16)!=address for c in audit["cases"])
        results.append({"continuation":hex(address),"preceding_call":hex(previous["address"]),"call_bytes":previous["raw"].hex(),"direct_incoming_branches":branches})
    probe=Path("tools/observe_property_table_import.js").read_text()
    assert "at(0x41abc8),{onEnter(){const c=propertyDataInvocations.get(this.threadId);if(c?.temp)" in probe
    result={
        "status":"PASS_repaired_probe_bounded_training_capture_ready",
        "supersedes_readiness":bind(prior),
        "correction":"The prior readiness conclusion is withdrawn. It checked instruction alignment but missed four incoming branches at 41abc8. The historical audit is preserved unchanged. This is a probe audit error, not a candidate behavior discrepancy.",
        "native_observation_status":"Faulty-probe launch failed before process creation (PID null); no native observations or compatibility claims are based on it.",
        "repair":"41abc8 emits temporary-free only with an existing per-thread temporary frame. Rank0/rank1 and out-of-selected-rank bypass paths have no temporary and do not emit this event.",
        "reachability_review":[
            "41ab19 is reached after the temporary allocator; rank0/1 branch around it. Allocation precedes all staged scans, including zero-count staging.",
            "41abab follows staged memcpy only. Scan EOF branches to function exit and cannot reach commit/free. Successful zero-count staging still reaches memcpy/free.",
            "41abc8 additionally receives direct branches 41a89d,41a8b1,41a93c,41aa33; the new guard distinguishes these paths without reading a nonexistent temporary.",
            "Header scan/reset/data continuations are immediately after their calls, have no direct branch predecessors or selected jump-table targets, and retain frame-presence guards.",
            "All thirteen wrapper continuations and COARSE flag continuation have only the audited call fallthrough within the inspected dispatcher. Header EOF bypasses data and reset, and wrapper completion cleans its still-active header frame."
        ],
        "scope":"Bounded new property hooks only; existing unrelated hooks are not re-audited. This is static capture readiness, not runtime equivalence or proof that every possible indirect branch is absent.",
        "continuations":results,
        "bindings":[bind(p) for p in [__file__,"source/DEF_PRE_64.exe","analysis/property-table-byte-audit.json","analysis/property-table-static-contract.json","tools/observe_property_table_import.js"]+paths],
    }
    Path("analysis/property-table-probe-v2-repair-audit.json").write_text(json.dumps(result,indent=2)+"\n")
    print(result["status"],len(results),"continuations",bind("analysis/property-table-probe-v2-repair-audit.json")["sha256"])


if __name__=="__main__":
    main()
