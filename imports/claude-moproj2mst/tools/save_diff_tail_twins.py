"""Compare original GUI/native instruction shapes; no binary loading or execution.

External absolute references are paired, not proved equivalent. Internal branch
locations are normalized to offsets. This is bounded structure evidence only.
"""
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
DUMPBIN = Path('C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe')
PAIRS = [
    ('nodal-resize',0x140854530,0x1408545a7,0x140180310,0x140180387),
    ('element-resize',0x140857a60,0x140857b12,0x140183840,0x1401838f2),
]
BINARIES = [ROOT / 'evidence/gui_mo/DEF_GUI_MO_64.exe', ROOT / 'evidence/original-binaries/DEF_PRE_64.exe']

def capture(binary, start, end, label):
    raw = subprocess.run([str(DUMPBIN), '/DISASM:NOBYTES', f'/RANGE:0x{start:x},0x{end:x}', str(binary)], check=True, capture_output=True, text=True).stdout
    path = ROOT / f'analysis/wave2-save-diffs-300-tail-twin-{label}.asm'
    path.write_text(raw, encoding='utf-8')
    rows = []
    for address, instruction in re.findall(r'^\s*([0-9A-F]{16}): (.+)$', raw, re.M):
        if int(address,16) >= end:
            continue
        rows.append(instruction.strip())
    externals = []
    def normalize(instruction):
        def address(match):
            value = int(match.group().rstrip('h'),16)
            if value < 0x140000000:
                return match.group()
            if start <= value < end:
                return f'local+{value-start:x}'
            externals.append(hex(value))
            return 'external'
        return re.sub(r'\b[0-9A-F]{16}h?\b', address, instruction)
    return [normalize(row) for row in rows], externals, {'path':str(path.relative_to(ROOT)), 'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}

results=[]
for label,gs,ge,ns,ne in PAIRS:
    g,grefs,gfile=capture(BINARIES[0],gs,ge,label+'-gui')
    n,nrefs,nfile=capture(BINARIES[1],ns,ne,label+'-native')
    results.append({'label':label,'gui':[hex(gs),hex(ge)],'native':[hex(ns),hex(ne)],'instruction_count':[len(g),len(n)],'normalized_instruction_shapes_equal':g==n,'first_difference':next(({'index':i,'gui':a,'native':b} for i,(a,b) in enumerate(zip(g,n)) if a!=b),None),'external_reference_pairs':list(dict.fromkeys(zip(grefs,nrefs))) if len(grefs)==len(nrefs) else {'gui':grefs,'native':nrefs},'captures':[gfile,nfile]})
output={'qualification':__doc__,'binaries':[{'path':str(p.relative_to(ROOT)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in BINARIES],'results':results}
path=ROOT/'analysis/wave2-save-diffs-300-tail-twin-comparison.json'
path.write_text(json.dumps(output,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'output':str(path),'results':[{k:v for k,v in r.items() if k not in ['external_reference_pairs','captures']} for r in results]}))

