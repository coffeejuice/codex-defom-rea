"""Build source-derived GRNDAT input candidates; never executes an importer."""
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "analysis/chat-a-grndat-fixtures"
CONTRACT = ROOT / "analysis/chat-a-grndat-contract.json"
C = json.loads(CONTRACT.read_text())
W = [t["width"] for t in C["selector1"]["tables"]]
OFFSETS = [t["offset"] for t in C["selector1"]["tables"]]
LISTS = [t["offset"] for t in C["selector2"]["list_order"]]
assert W == [4, 6, 7, 6, 7, 6, 9, 6, 3]
assert LISTS == ["0x16f8", "0x16e0", "0x16c8", "0x1710", "0x1728"]
FILES = []
CASES = []

def digest(b):
    return hashlib.sha256(b).hexdigest()

class Input:
    def __init__(self):
        self.text = ""
        self.spans = []

    def add(self, stage, text, read="line", assertion=None):
        text.encode("ascii")
        start = len(self.text)
        self.text += text
        row = {
            "stage": stage, "byte_start": start, "byte_end_exclusive": len(self.text),
            "line_1_based": self.text[:start].count("\n") + 1, "reader_kind": read,
            "expected_input_bytes_hex": text.encode().hex(),
        }
        if assertion:
            row["assertion"] = assertion
        if read == "stream":
            row["tokens"] = [
                {"lexeme": m.group(), "start": start+m.start(), "end": start+m.end()}
                for m in re.finditer(r"\S+", text)
            ]
            row["cursor_note"] = (
                "For successful direct scans, cursor follows the assigned token, before trailing "
                "whitespace. The next nested line helper may first consume the remaining LF."
            )
        else:
            row["cursor_note"] = "A successful fgets consumes through this span end; bounds are lexical source offsets."
        self.spans.append(row)

    def missing(self, stage):
        self.add(stage, "", "EOF", "Expected read sees EOF at this exact file offset.")

    def outer(self, selector, secondary=0, material=20):
        self.add("material_and_outer_header", f"GRNDAT {material} {selector} {secondary}\n")

def copy_input(target, src, prefix):
    for span in src.spans:
        text = src.text[span["byte_start"]:span["byte_end_exclusive"]]
        target.add(prefix+"."+span["stage"], text, span["reader_kind"], span.get("assertion"))

def table(inp, stage, selector=0, value=401, count=1, second_count=1, end=None):
    if end == "header_eof":
        inp.missing(stage+".header")
        return
    if selector == 0:
        inp.add(stage+".header", f"0 {value}\n", assertion="Only rank/constant change; no typed reset or payload call.")
    elif selector in (1, 2):
        inp.add(stage+".header", f"{selector} {count}\n")
        if count > 0:
            for i in range(count):
                inp.add(stage+f".point{i}", f"{value+i} {value+10+i}\n", "stream")
    elif selector == 3:
        inp.add(stage+".header", f"3 {count} {second_count}\n")
        axes = [value+i for i in range(count)] + [value+10+i for i in range(second_count)]
        inp.add(stage+".axes", " ".join(map(str, axes))+"\n", "stream")
        vals = [value+100+i for i in range(count*second_count)]
        if end == "scratch_eof":
            inp.add(stage+".scratch_prefix", " ".join(map(str, vals[:1]))+" ", "stream")
            inp.missing(stage+".scratch_remaining")
        else:
            inp.add(stage+".dependent", " ".join(map(str, vals))+"\n", "stream")
    else:
        inp.add(stage+".header", f"{selector}\n")

def record(inp, stage, marker=100, embedded=False, count=2,
           scalar="23", real="24.5", tables=None, stop=None, inline_override=None):
    n = min(count, 12) if count > 0 else 0
    values = [str(marker+i)+".25" for i in range(n)]
    if inline_override is not None:
        values = inline_override
    if stop == "count_eof":
        inp.missing(stage+".count")
        return False
    if stop == "inline_eof":
        inp.add(stage+".count_inline_prefix", "3 "+values[0]+" ", "stream")
        inp.missing(stage+".inline_remaining")
        return False
    header = " ".join([str(count), *values, scalar, real])+"\n"
    if stop == "pair_eof":
        header = " ".join([str(count), *values, scalar])+" "
        inp.add(stage+".count_inline_scalar_prefix", header, "stream",
                f"One pair conversion stores int+0x70={scalar}; double+0x68 retains its pre-action value.")
        inp.missing(stage+".table_header")
        return False
    inp.add(stage+".count_inline_scalar_pair", header, "stream",
            f"Inline tail beyond count is retained. Scalar pair writes int+0x70={scalar} then double+0x68={real} without zeroing.")
    indices = [2] if embedded else [0, 1, 2]
    for i in indices:
        if stop == f"table{i}_eof":
            inp.missing(stage+f".table{i}.header")
            return False
        spec = (tables or {}).get(i, {})
        table(inp, stage+f".table{i}", value=marker+30+i, **spec)
        if spec.get("end") in ("scratch_eof", "header_eof"):
            return False
    return True

def selector1(marker=1000, counts=None, selectors=None, header_fail=None,
              payload_fail=None, prefix_values=0, final="17.25 -18.5\n"):
    x = Input()
    x.outer(1)
    counts = counts or [1]*9
    selectors = selectors or [1]*9
    for j in range(9):
        if header_fail == j:
            x.missing(f"table{j+1}.header")
            return x
        x.add(f"table{j+1}.header", f"{selectors[j]} {counts[j]}\n",
              assertion=f"Reset {OFFSETS[j]} only after this header succeeds; raw count={counts[j]}.")
    for j in range(9):
        for i in range(max(0, counts[j])):
            vals = [str(marker+j*100+i*20+k) for k in range(W[j]+1)]
            if payload_fail == j:
                if prefix_values:
                    x.add(f"table{j+1}.payload_prefix", " ".join(vals[:prefix_values])+" ", "stream")
                x.missing(f"table{j+1}.payload_remaining")
                return x
            x.add(f"table{j+1}.row{i}", " ".join(vals)+"\n", "stream",
                  f"Axis then {W[j]} dependent values, row-major; positive-count final line remainder discarded.")
    if final:
        x.add("final_pair", final, assertion="Writes M+0x14c8 then M+0x14c0; both zeroed before reading.")
    else:
        x.missing("final_pair")
    return x

def selector2(marker=2000, counts=None, embedded_count=2, embedded_tables=None,
              failure=None, count_lines=None, scalar="23", real="24.5"):
    x = Input()
    x.outer(2, marker+9)
    stop = failure[1] if failure and failure[0] == "embedded" else None
    if not record(x, "embedded", marker, True, embedded_count, scalar=scalar, real=real,
                  tables=embedded_tables, stop=stop):
        return x
    counts = counts or [1]*5
    for j in range(5):
        if count_lines and j in count_lines:
            line = count_lines[j]
            if line is None:
                x.missing(f"list{j}.count")
                return x
            x.add(f"list{j}.count", line, assertion=f"Clear list{LISTS[j]} first; count-reader status is ignored.")
        else:
            x.add(f"list{j}.count", str(counts[j])+"\n")
        for i in range(max(0, counts[j])):
            stop = failure[2] if failure and failure[0] == j and failure[1] == i else None
            if not record(x, f"list{j}.node{i}", marker+100*(j+1)+10*i,
                          scalar=scalar, real=real, stop=stop):
                return x
    return x

def scalar5(line="1 2 3 4 5 6\n", material=20):
    x=Input()
    x.outer(5, material=material)
    if line:
        x.add("six_coefficients", line, assertion="Six live outputs are zeroed first, then assigned in ascending offset order.")
    else:
        x.missing("six_coefficients")
    return x

def save(rel, inp, role="case"):
    p=OUT/rel
    p.parent.mkdir(parents=True, exist_ok=True)
    b=inp.text.encode("ascii")
    p.write_bytes(b)
    FILES.append({"path":p.relative_to(ROOT).as_posix(), "sha256":digest(b), "bytes":len(b),
                  "role":role, "encoding":"ASCII", "line_endings":"LF", "final_LF":b.endswith(b"\n")})
    for span in inp.spans:
        assert b[span["byte_start"]:span["byte_end_exclusive"]].hex()==span["expected_input_bytes_hex"]
    return p.relative_to(ROOT).as_posix()

GROUP_SECTIONS={
    "T01":["/material_layout","/ownership_and_initialization","/entry_and_status"],
    "T02":["/entry_and_status"], "T03":["/selector1"], "T04":["/selector1"],
    "T05":["/selector1"], "T06":["/selector1/final_pair"], "T07":["/record_reader"],
    "T08":["/selector2"], "T09":["/selector2","/ownership_and_initialization"],
    "T10":["/selector2","/record_reader"], "T11":["/selector2"],
    "T12":["/nested_record_table_reader"], "T13":["/nested_record_table_reader"],
    "T14":["/nested_record_table_reader/payload_rank2"], "T15":["/selector5"],
    "T16":["/other_selectors"], "T17":["/selector4_preserved","/other_selectors"],
    "T18":["/ownership_and_initialization"], "T19":["/grain_feature_gate"],
}
def add(name, inp, groups, expected_status, assertions, setup=None, cohort="discovery",
        refresh=None, positions=None, boundary="complete_handler_return", lifecycle=None,
        scalar_pair=None, record_count=None):
    rel=f"{cohort}/{name}.KEY"
    path=save(rel, inp)
    sections=sorted({sec for g in groups for sec in GROUP_SECTIONS[g]})
    CASES.append({
        "id":name, "cohort":cohort, "test_groups":groups, "input":path,
        "setup_imports":setup or [], "setup_execution":"In listed order, separate imports in the same root/process.",
        "precondition":"Fresh retained initialized root; root+0x864 observed0. Use listed setup files; never transplant native output into expected state.",
        "expected":{"source":"contract-derived, never candidate/original output", "boundary":boundary,
                    "handler_status":expected_status, "postbranch_refresh0":refresh if refresh is not None else expected_status==0,
                    "assertions":assertions, "lifecycle":lifecycle},
        "stream":{"coordinate_system":"Zero-based raw ASCII/LF file bytes; these are derived lexical positions, not observed ftell values.",
                  "file_length":len(inp.text), "expected_handler_return_position":positions if positions is not None else len(inp.text),
                  "outer_header_cursor_checks":[
                      {"material_token_end_and_413ee0_entry":m.end(),
                       "413ee0_line_end_or_eof":inp.text.find("\n",m.end())+1 if "\n" in inp.text[m.end():] else len(inp.text),
                       "qualification":"After a successful material-ID fscanf, before the outer-header fgets."}
                      for m in re.finditer(r"(?m)^GRNDAT[ \t]+-?\d+",inp.text)],
                  "cursor_qualification":"Capture per-helper/native logical and physical positions. Direct scalar scans stop before trailing whitespace; fgets consumes its line. EOF-boundary equals file length.",
                  "stages":inp.spans},
        "source_refs":[{"path":"analysis/chat-a-grndat-contract.json","json_pointer":sec} for sec in sections],
        "execution_status":"NOT_RUN", "runtime_oracle":None,
    })
    if scalar_pair is not None:
        CASES[-1]["expected"]["embedded_scalar_pair"] = scalar_pair
    if record_count is not None:
        CASES[-1]["expected"]["embedded_count"] = {
            "declared":record_count, "stored":min(record_count,12),
            "inline_conversions":max(0,min(record_count,12)),
            "clamp_diagnostic_present":record_count>12,
            "source_pointer":"/record_reader/steps/2",
            "evidence_ids":C["record_reader"]["sources"][2:4],
        }

def pair_expected(before, after, conversions):
    return {"before":{"int_0x70":before[0],"double_0x68":before[1]},
            "after":{"int_0x70":after[0],"double_0x68":after[1]},
            "pair_scan_conversions":conversions,
            "source_pointer":"/record_reader/steps/4",
            "evidence_ids":C["record_reader"]["sources"][:2]}

# Known setup grammar is retained in the committed nested-material/FSTRES fixtures.
init3=Input(); init3.add("dimension","* DEFORM-3D\n")
init2=Input(); init2.add("dimension","* DEFORM-2D\n")
P3=save("setup/initialize_3d.KEY",init3,"setup")
P2=save("setup/initialize_2d.KEY",init2,"setup")
seed=Input()
copy_input(seed,selector1(1000),"seed1")
copy_input(seed,selector2(2000,scalar="7",real="8.5"),"seed2")
copy_input(seed,scalar5("51 52 53 54 55 56\n"),"seed5")
seed.add("selector4","GRNDAT 20 4 0\n0 64.5\n")
seed.add("negative_seed","GRNDAT 20 -77 0\n")
SEED=save("setup/seed_all_branches.KEY",seed,"setup")
BASE=[P3,SEED]
heldseed=Input()
copy_input(heldseed,selector2(6200,counts=[2,0,1,0,2],scalar="47",real="48.5"),"hseed2")
copy_input(heldseed,selector1(7100,counts=[0,1,2,0,1,0,2,1,0]),"hseed1")
copy_input(heldseed,scalar5("91 92 93 94 95 96\n"),"hseed5")
HSEED=save("setup/held_out_seed.KEY",heldseed,"reserved_setup")

add("fresh_3d",scalar5(),["T01","T15"],0,
    ["Material20 is absent before action; constructor values inspected before selector writes.",
     "Creation refresh1 precedes outer header; six inputs stored exactly."],[P3])
add("fresh_2d",selector2(2300,counts=[0]*5),["T01","T09"],0,
    ["Fresh embedded flag1 skips tables0/1; five lists empty; same GRNDAT data path in2D."],[P2])

for name,body,status in [
    ("outer_eof","GRNDAT 20",2),("outer_blank","GRNDAT 20\n",2),
    ("outer_bad","GRNDAT 20 BAD\n",0),("outer_selector_only","GRNDAT 20 5\n1 2 3 4 5 6\n",0),
    ("outer_commas","GRNDAT 20 5,0\n1,2,3,4,5,6\n",0)]:
    x=Input();x.add("outer_edge",body)
    add(name,x,["T02"],status,
        ["Header failure preserves old17e8 and skips refresh0; zero conversions instead selects0.",
         "One selector conversion zeroes missing secondary; comma normalization belongs to outer/coefficient line helpers."],BASE)

add("nine_tables_order",selector1(3000),["T03"],0,
    ["All nine headers/reset events precede first payload; distinguish every table/column by marker.",
     "Widths [4,6,7,6,7,6,9,6,3]; final14c8=17.25,14c0=-18.5."],BASE)
add("nine_mixed_subselectors",selector1(4100,counts=[0,1,2,0,1,0,2,1,0],
    selectors=[0,2,0,3,9,3,2,3,3]),["T03"],0,
    ["Flag1078 becomes0; unsupported9 retains old107c=1;1080/1084=1.",
     "Subselector0 retains reset axis3; allowed3 maps33; flags never gate payload."],BASE)
for j in range(9):
    add(f"header_{j+1}_eof",selector1(counts=[2]*9,header_fail=j),["T04"],2,
        [f"Earlier tables0..{j-1} reset with raw count2; table{j} and later retain seeded count1 and marker payload.",
         "Selector17e8=1 survives; no payload has begun; no refresh0."],BASE,refresh=False)
    for n,label in [(0,"axis"),(2,"prefix")]:
        add(f"payload_{j+1}_{label}_eof",selector1(5000,payload_fail=j,prefix_values=n),["T05"],2,
            ["All nine tables already reset; previous payloads complete; later tables have null buffers until reached.",
             f"Current table{j+1} allocated/zeroed: {n} scalar values assigned from generated row; remaining values0.",
             "No refresh0; return2."],BASE,refresh=False)
for name,line,status in [("full","61.25 -62.5\n",0),("partial","-0\n",0),
    ("bad","BAD\n",0),("blank","\n",2),("eof","",2)]:
    add("pair_"+name,selector1(counts=[0]*9,final=line),["T06"],status,
        ["Both pair outputs zero before read; first destination14c8,second14c0.",
         "All table counts0; refresh0 always after final pair even status2."],BASE,refresh=True)

for n in [-1,0,1,12,13]:
    add("record_count_"+str(n).replace("-","minus"),selector2(8100,counts=[0]*5,embedded_count=n),["T07"],0,
        [f"Record count={min(n,12)}; read only max(0,min(count,12)) inline values; tail retains seeded values.",
         f"Declared count {n}; stored count {min(n,12)}; inline conversions {max(0,min(n,12))}; clamp diagnostic present={n>12}.",
         "Successful reused embedded pair replaces seeded7/8.5 with23/24.5; both fields must change.",
         "Embedded tables0/1 unchanged; third table constant marker8132."],BASE,
         scalar_pair=pair_expected((7,8.5),(23,24.5),2),record_count=n)
for stage in ["count_eof","inline_eof","pair_eof","table2_eof"]:
    add("embedded_"+stage,selector2(8200,failure=("embedded",stage),
        scalar="19" if stage=="pair_eof" else "23"),["T07","T08"],2,
        ["Secondary14d0 stored before record read; partial inline/scalar writes retained.",
         "No list has yet cleared; all five seeded lists preserved.",
         "For pair_eof: int+0x70 changes7->19, double+0x68 retains8.5, pair scan returns1; following nested-header EOF yields handler2 with no list clear or refresh0."
         if stage=="pair_eof" else "Failure stage is recorded in the lexical spans."],BASE,refresh=False,
         scalar_pair=pair_expected((7,8.5),(19,8.5),1) if stage=="pair_eof" else None)
add("lists_mixed",selector2(9000,counts=[2,0,1,2,1]),["T09"],0,
    ["Import list order16f8,16e0,16c8,1710,1728; exact lengths2,0,1,2,1.",
     "Appended flags0 and all three table constants differ; every next/tail link valid; padding ignored."],BASE)
for j in range(5):
    for stage in ["count_eof","table0_eof","table1_eof","table2_eof"]:
        add(f"list{j}_failed_node_{stage}",selector2(10000,counts=[2]*5,failure=(j,1,stage)),["T10"],2,
            [f"List{j} cleared then has2 linked nodes, second partial; earlier lists length2; later lists retain seeded length1.",
             "Append precedes read. Inline count EOF leaves fresh failed node defaults; table failures retain prior fields."],BASE,refresh=False)
for j in range(5):
    add(f"list{j}_count_eof",selector2(11000,counts=[0]*5,count_lines={j:None}),["T11"],0,
        ["All five lists eventually clear: current and remaining count readers see EOF, ignored as count0.",
         "Earlier lists completed empty; return0 and refresh0; no appended nodes."],BASE)
for name,line in [("bad","BAD\n"),("negative","-4\n"),("spaces","   \n"),
                  ("two_lf","\n\n"),("one_lf_then_zero","\n0\n")]:
    add("count_"+name,selector2(12000,counts=[0]*5,count_lines={2:line}),["T11"],0,
        ["Count helper skips at most one exact LF; spaces are not skipped; failure/negative yields zero iterations.",
         "All five lists empty; count status not propagated; refresh0."],BASE)

for sel in range(4):
    spec={2:{"selector":sel,"count":2,"second_count":1}}
    add(f"record_table_selector{sel}",selector2(13000,counts=[0]*5,embedded_tables=spec),["T12"],0,
        ["Type96 and width1 retained; selectors/counts/rank follow table header with no typed reset.",
         "Initial tables0/1 remain untouched; rank2 axes read before temporary dependent values."],BASE)
rankseed=save("setup/embedded_rank2.KEY",selector2(14000,counts=[0]*5,
             embedded_tables={2:{"selector":3,"count":2,"second_count":1}},scalar="7",real="8.5"),"setup")
add("record_table_to_constant",selector2(15000,counts=[0]*5),["T12"],0,
    ["Third table becomes rank0 constant15032; old counts/axes/dependent pointers are retained, not freed.",
     "Do not compare padding or infer physical empty state from rank0."],[P3,rankseed])
for name,line in [("negative","-7\n"),("bad","BAD\n"),("spaces","   \n"),("two_lf","\n\n")]:
    x=Input();x.outer(2,55)
    x.add("embedded.count_inline_scalar_pair","0 23 24.5\n","stream")
    # A direct fscanf leaves LF; helper consumes it as its one skipped line.
    x.add("embedded.table2.header",line)
    for j in range(5):x.add(f"list{j}.count","0\n")
    add("record_header_"+name,x,["T13"],0,
        ["Nested header scan failure is ignored; selector stays-1 (or explicit negative); table retained.",
         "Only one LF may be skipped; extra LF can be consumed by subsequent count helper.",
         "No table payload read; five lists empty."],BASE)
# Unknown positive on initial rank0 table consumes no payload under shared-reader gate.
add("record_unknown_positive_rank0",selector2(16000,counts=[0]*5,embedded_tables={2:{"selector":9}}),
    ["T13"],0,["Selector9 performs no header writes; calls41a800 on old rank0/width1 table; no payload consumed."],BASE)
# Unknown positive on an already rank1 table must consume its old row shape.
x=Input();x.outer(2,66)
x.add("embedded.fields","0 23 24.5\n","stream");x.add("embedded.table2.header","9\n")
x.add("embedded.table2.old_shape_payload","17001 17002\n","stream")
for j in range(5):x.add(f"list{j}.count","0\n")
rank1=save("setup/embedded_rank1.KEY",selector2(17000,counts=[0]*5,
    embedded_tables={2:{"selector":1,"count":1}},scalar="7",real="8.5"),"setup")
add("record_unknown_positive_rank1",x,["T13"],0,
    ["Unknown9 retains rank1/count1/type96; reallocates and reads old shape axis17001/value17002."],[P3,rank1])
add("rank2_scratch_eof",selector2(18000,counts=[0]*5,
    embedded_tables={2:{"selector":3,"count":2,"second_count":1,"end":"scratch_eof"}}),
    ["T14"],2,["Axes complete; only scratch prefix written; dependent allocated zeros never committed.",
               "Native temporary leaks on EOF; bounded process isolation required in future capture.",
               "No list clear and no postbranch refresh0."],BASE,refresh=False)

for name,line,status in [("full","21 22 23 24 25 26\n",0),("partial","-0 2.25\n",0),
    ("bad","BAD\n",0),("blank","\n",2),("eof","",2),("comma","31,32,33,34,35,36\n",0)]:
    add("six_"+name,scalar5(line),["T15"],status,
        ["Zero all six before scan; assigned prefix exact, including negative zero; remainder+0.",
         "Other branch storage retained; refresh0 regardless of helper failure."],BASE,refresh=True)
for val in [0,3,6,2147483647,-1,-7,-2147483648]:
    x=Input();x.outer(val)
    add("route_"+str(val).replace("-","minus"),x,["T16"],0,
        [f"17e8={-1 if val<0 else val};17ec="+(f"0x{(-val)&0xffffffff:08x}" if val<0 else "retains77"),
         "No payload read; refresh0; all other stored branches retained."],BASE)

for name,payload,status in [("constant","0 291.5\n",0),("header_eof","",2),
                           ("rank1","1 2\n1 201\n2 202\n",0)]:
    x=Input();x.outer(4)
    if payload:x.add("selector4_table",payload)
    else:x.missing("selector4_table")
    add("selector4_"+name,x,["T17"],status,
        ["Preserve established41b270 contract; unlike41c070 this wrapper resets type on successful header.",
         "Postbranch refresh0 on success/failure; other GRNDAT branches retained."],BASE,refresh=True)

# Documented dimension reset is a separate import, not an invented material-removal keyword.
reset=Input();reset.add("dimension_change","* DEFORM-2D\n");reset.outer(0)
add("dimension_reset_3d_to_2d",reset,["T18"],0,
    ["Destroy populated material20 during changed-dimension root reset, including five lists and nested tables.",
     "Then GRNDAT20 creates a fresh material with constructor defaults and selection0.",
     "Capture primary/linked root identities independently; arbitrary alternate-root keyword routing is excluded."],
    BASE,lifecycle="dimension-change teardown followed by fresh construction")

tet=Input();tet.add("mesh","* DEFORM-3D\nOBJTYP 1 1 0\nRZ 1 4\n1 0 0 0\n2 1 0 0\n3 0 1 0\n4 0 0 1\nELMCON 1 1 4\n1 1 2 3 4\n")
TET=save("setup/tetrahedron.KEY",tet,"setup")
plane=Input();plane.add("mesh","* DEFORM-2D V11.0 KEYWORD FILE\nOBJTYP 1 2 0\nRZ 1 3\n1 0 0\n2 1 0\n3 0 1\nELMCON 1 1\n1 1 2 3 1\n")
PLANE=save("setup/triangle.KEY",plane,"setup")
add("disabled_gate_populated_3d",selector1(22000,counts=[0]*9),["T19"],0,
    ["Observe gate864=0, never force it; feature53 false leaves output unwritten.",
     "Compare known mesh/descriptor fields before and after ordinary refresh; do not infer scientific finalization."],
    [TET,SEED])
add("disabled_gate_populated_2d",scalar5("41 42 43 44 45 46\n"),["T19"],0,
    ["Observe gate864=0; feature53 output is not a valid expected width.",
     "Planar setup uses retained four-slot triangle grammar (first node repeated); inspect known mesh/descriptor retention."],
    [PLANE,SEED])

# Reserved inputs are generated independently of candidate/original output; they are not yet frozen held-out evaluation.
for name,inp,groups,assertions in [
    ("h_nine_reordered_counts",selector1(31000,counts=[2,0,1,2,0,1,0,2,1],
        selectors=[2,1,2,3,0,1,9,3,2]),["T03","T19"],["Distinct count/subselector combinations and markers; preserve conditional flags as specified."]),
    ("h_lists_replacement",selector2(34000,counts=[0,2,0,1,2],scalar="59",real="60.5"),["T07","T09"],["List order fixed; lengths0,2,0,1,2; embedded/appended table-count distinction.","Successful reused embedded pair replaces reserved seed47/48.5 with59/60.5; both fields must change."]),
    ("h_negative_zero",scalar5("0 -0 0.125 -0.25 1048576 -32\n"),["T15"],["Bitwise finite values; sign of second zero preserved."]),
    ("h_header7_eof",selector1(header_fail=6,counts=[0,2,1,2,0,2,1,2,0],selectors=[2]*9),
        ["T04"],["Prior resets only; target/later old tables retained; distinct counts/selectors from discovery."]),
    ("h_last_list_eof",selector2(35000,counts=[0]*5,count_lines={4:None},scalar="59",real="60.5"),["T11"],["Last count EOF ignored; success and refresh0."]),
]:
    failed=name=="h_header7_eof"
    add(name,inp,groups,2 if failed else 0,assertions,[P2,HSEED],cohort="held_out_reserved",refresh=not failed,
        scalar_pair=pair_expected((47,48.5),(59,60.5),2) if name=="h_lists_replacement" else None)

# Ordering/retention across separate imports is explicit and independently hash-bound.
sequence = []
for name,inp in [
    ("h_sequence1",selector1(36000,counts=[0]*9,final="361.25 -362.5\n")),("h_sequence2",selector2(37000,counts=[0,1,0,0,1],scalar="59",real="60.5")),
    ("h_sequence4",None),("h_sequence5",scalar5("71 72 73 74 75 76\n")),
    ("h_sequence_negative",None),("h_sequence_positive",None)]:
    if inp is None:
        inp=Input();inp.outer(4 if name.endswith("4") else (-2147483648 if name.endswith("negative") else 3),83)
        if name.endswith("4"):inp.add("table","0 38.25\n")
    setup=[P3,HSEED,*sequence]
    add(name,inp,["T16","T17"],0,["Observe each handler before next import; untouched branch state persists.",
        "Only negative route updates17ec; later positive3 retains0x80000000." if name.endswith("positive") else
        "17ec assertion depends on prior sequence actions; do not reuse discovery seed77 blindly."],
        setup,cohort="held_out_reserved")
    sequence.append(CASES[-1]["input"])

UNSUPPORTED=[
    {"groups":["T14"],"case":"Matching failure in rank2 uninitialized scratch","reason":"Cannot assert deterministic bytes; no executable input supplied for this domain."},
    {"groups":["T02"],"case":"Malformed first material-ID scan","reason":"Uninitialized material-selection local; excluded."},
    {"groups":["T12"],"case":"Nonfinite/out-of-range floating-to-int, overflowing table products","reason":"No deterministic bounded compatibility assertion; excluded."},
    {"groups":["T18"],"case":"Arbitrary material-removal command and explicit root-destruction API","reason":"Contract supplies destructor chain but not a proven public command transcript; no invented command. Dimension-reset fixture covers a documented teardown route."},
    {"groups":["T19"],"case":"Enabled root864 and scientific feature29/53 widths","reason":"Unproved producer/consumer behavior; do not force gate enabled."},
    {"groups":["T01","T09","T10","T18"],"case":"OOM, constructor exceptions, corrupted lists, shared/borrowed buffers and nonnull cache","reason":"Normal returned-status contract does not establish these domains."},
]
plan={
    "schema_version":1, "status":"READY_FOR_COORDINATOR_STATIC_REVIEW_NOT_RUNTIME_EXECUTED",
    "base_commit":C["base_commit"], "target":C["target"],
    "contract":{"path":CONTRACT.relative_to(ROOT).as_posix(),"sha256":digest(CONTRACT.read_bytes())},
    "purpose":"Small source-derived GRNDAT keyword candidates, stage coordinates and mutation assertions; no implementation/original run.",
    "scope":"Contract-bound defined subset across T01..T19; exclusions and setup qualifications are explicit.",
    "harness":{
        "launches_per_case":"Future harness should start from a fresh initialized root/process per case, then perform setup_imports and action as separate imports in the same process.",
        "root":"Use retained constructor/linked-root profile, default gate864=0; observe rather than synthesize its value.",
        "capture":"Capture complete handler boundaries, helper return statuses, raw known field bytes, physical allocation ownership and logical/physical stream positions. Distinguish initial creation refresh1 from postbranch refresh0.",
        "error_scope":"handler_status is0/2 at0x140384a50, not whole application exit code/import finalization outcome.",
        "position_rules":"Preserve ASCII LF bytes exactly; Windows text translation must be recorded. Stage source offsets are not runtime observations or a promise about external ftell encoding.",
        "numeric_domain":"Finite representable decimal/integer inputs with bounded extents; compare known binary64 bits, not decimal JSON equality.",
        "markers":"Unique table/row/column values and distinct list/node table constants identify misordering; no added unknown sentinel keyword affects parsing.",
        "record_scalar_pair_markers":{"discovery_seed":[7,8.5],"discovery_complete_action":[23,24.5],"discovery_partial_action_integer":19,"reserved_seed":[47,48.5],"reserved_complete_action":[59,60.5],"qualification":"Source-derived inputs and expected stores; no runtime observation."},
        "lifecycle":"Do not compare pointer addresses across material destruction/recreation; compare lifetime/order and known semantic fields.",
    },
    "cohort_policy":{
        "discovery":"Available for fixture/harness development and contract falsification.",
        "held_out_reserved":"Separate files/seed/order/values, never executed. Must freeze candidate, fixtures, expectations and capture code before evaluation; reserved does not yet mean independently validated held-out.",
        "no_output_derivation":"Expectations come only from cited retained contracts/native evidence and lexical fixture construction; neither candidate nor original was run.",
    },
    "source_refs":[
        {"path":"analysis/chat-a-grndat-contract.json","sha256":digest(CONTRACT.read_bytes())},
        *[{"path":p,"sha256":digest((ROOT/p).read_bytes())}for p in
          ["analysis/dimension-import-contract.json","analysis/planar-boundary-contract.json",
           "tests/fixtures/planar_boundary/training/02_closed_triangle.KEY",
           "tests/fixtures/fstres/training_a/32_populated_refresh_and_new_zero_material.KEY"]],
        *[{"path":r["path"],"sha256":r["sha256"]} for r in C["source_bindings"]
          if r["path"] in ["analysis/chat-a-grndat-native-evidence.json","analysis/chat-a-grndat-byte-audit.json",
             "analysis/nested-material-contract.json","analysis/property-table-static-contract.json",
             "analysis/model-0x140413de0.json","analysis/model-0x140413ee0.json"]],
    ],
    "files":FILES, "cases":CASES, "unsupported_or_uncovered":UNSUPPORTED,
    "coverage":[{"test_group":g["id"],"contract_cases":g["cases"],
        "fixtures":[c["id"]for c in CASES if g["id"] in c["test_groups"]],
        "qualification":"Partial deterministic subset; see unsupported_or_uncovered and case-specific setup."
    } for g in C["test_matrix"]],
    "execution":{"original_runs":0,"candidate_runs":0,"compatibility_tests_passed":None,"native_session_opened":False},
}
assert all(row["fixtures"] for row in plan["coverage"])
filemap={f["path"]:f for f in FILES}
for case in CASES:
    assert all(p in filemap for p in case["setup_imports"])
    assert case["input"] not in case["setup_imports"]
    for ref in case["source_refs"]:
        obj=C
        for part in ref["json_pointer"].split("/")[1:]:obj=obj[part]
    assert case["stream"]["expected_handler_return_position"] <= filemap[case["input"]]["bytes"]
assert len({c["id"] for c in CASES}) == len(CASES)
discovery_hashes={filemap[c["input"]]["sha256"] for c in CASES if c["cohort"]=="discovery"}
held_hashes={filemap[c["input"]]["sha256"] for c in CASES if c["cohort"]=="held_out_reserved"}
assert not discovery_hashes & held_hashes, "Reserved action bytes duplicate discovery"
by_input={c["input"]:c for c in CASES}
for case in CASES:
    if case["cohort"]=="held_out_reserved":
        assert all(by_input[p]["cohort"]=="held_out_reserved"for p in case["setup_imports"]if p in by_input)
for f in FILES:
    raw=(ROOT/f["path"]).read_bytes()
    assert len(raw)==f["bytes"] and digest(raw)==f["sha256"]
    assert b"\r" not in raw and len(raw)<=10000
plan_path=ROOT/"analysis/chat-a-grndat-fixture-plan.json"
plan_path.write_text(json.dumps(plan,indent=2)+"\n")
report={
    "status":"STATIC_PACKAGE_CHECKS_PASS_RUNTIME_NOT_RUN",
    "files":len(FILES),"cases":len(CASES),"discovery":sum(c["cohort"]=="discovery"for c in CASES),
    "held_out_reserved":sum(c["cohort"]=="held_out_reserved"for c in CASES),
    "total_fixture_bytes":sum(f["bytes"]for f in FILES),"max_fixture_bytes":max(f["bytes"]for f in FILES),
    "test_groups_with_candidates":len(plan["coverage"]),
    "checks":["JSON parses","Contract hash bound","All19 test groups mapped","Source JSON pointers resolve",
              "Every setup path resolves without self-reference","Fixture SHA256/size matches",
              "All stage slices match declared bytes","ASCII/LF only","Bounded fixture sizes","Unique case IDs",
              "Reserved action bytes disjoint from discovery","Reserved setup sequences reference no discovery cases"],
    "not_checked":["Native behavior","Candidate behavior","Runtime cursor encodings","Whole-import/finalization status"],
    "coordinator_gate":"Review semantic assertions and qualified setup before implementation/runtime capture.",
    "readiness_gaps":UNSUPPORTED,
    "plan_sha256":digest(plan_path.read_bytes()),"generator_sha256":digest(Path(__file__).read_bytes()),
    "original_runs":0,"candidate_runs":0,"native_session_opened":False,
}
(ROOT/"analysis/chat-a-grndat-fixture-readiness.json").write_text(json.dumps(report,indent=2)+"\n")
print(json.dumps({k:v for k,v in report.items() if k not in ["checks","readiness_gaps","not_checked"]},indent=2))
