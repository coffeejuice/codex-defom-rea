"""Prepare changed evaluation extents without modifying the frozen generator."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys

source=Path('tools/prepare_material_curves.py')
freeze=json.loads(Path('analysis/material-curves-candidate-freeze.json').read_bytes())
expected=next(v['sha256']for v in freeze['files']if Path(v['path'])==source)
assert hashlib.sha256(source.read_bytes()).hexdigest()==expected
text=source.read_text()
changes=[
    ('p1=payload(([10.+delta,20.+delta],[],[]),[1.5+delta,2.5+delta])',
     'p1=payload(([10.+delta,20.+delta,25.+delta],[],[]),[1.5+delta,2.5+delta,2.75+delta])'),
    ('d2=payload(([10.+delta,20.+delta],[.1+delta],[]),[.5+delta,.75+delta],rank=2)',
     'd2=payload(([10.+delta,20.+delta,25.+delta],[.1+delta],[]),[.5+delta,.75+delta,1.125+delta],rank=2)'),
    ("op('FPERV',[2,1,1,1],[p1,p2])", "op('FPERV',[3,1,1,1],[p1,p2])"),
    ("op('DIFCOE',[1,2,1,0,0],[d2])", "op('DIFCOE',[1,3,1,0,0],[d2])"),
    ("op('FPERV',[2.9,1.9,-2.5,1.1],[p1,payload()])",
     "op('FPERV',[3.9,1.9,-2.5,1.1],[p1,payload()])"),
]
for old,new in changes:
    assert text.count(old)==1,old
    text=text.replace(old,new)
target=Path('build/prepare-material-curves-evaluation.py')
target.write_text(text,encoding='utf-8')
subprocess.run([sys.executable,str(target),'evaluation'],check=True)
path=Path('analysis/material-curves-evaluation-inputs.json')
plan=json.loads(path.read_bytes())
plan['independent_extent_preparation']=dict(frozen_generator_sha256=expected,
    generated_source_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),changes=changes,
    qualification='Prepared after training PASS and before original/C++ evaluation. Three-row FPERV and DIFCOE inputs replace training two-row extents. Frozen candidate/generator remain unchanged.')
path.write_text(json.dumps(plan,indent=2)+'\n',encoding='utf-8')
