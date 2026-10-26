"""Prepare fresh short-path validation fixtures; never launches DEFORM or overwrites a trial."""
import argparse
import hashlib
import json
import re
import shutil
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = Path(r'C:\PROBLEM\TEST_REA\COG_P1_B1')
RING = Path(r'C:\Program Files\SFTC\DEFORM\v11.0.2\2D\2D_Examples\EN\Forging\RING_EN.KEY')
NAME = 'COG_P1_B1'


def digest(p):
    with p.open('rb') as f:
        return hashlib.file_digest(f, 'sha256').hexdigest()


def manifest(directory):
    return {p.relative_to(directory).as_posix(): dict(size=p.stat().st_size, sha256=digest(p))
            for p in sorted(directory.rglob('*')) if p.is_file()}


def one_operation(directory, apply=False):
    project = directory/f'{NAME}.moproj'
    tree = ET.parse(project)
    operations = tree.getroot().find('Operation_List')
    first = operations[0]
    assert first.findtext('Id') == '1' and first.findtext('Type') == 'SfOprHeatTransfer'
    assert all(o.get('Remesh') == '0' for o in first.findall('Objects/Object'))
    for operation in list(operations)[1:]:
        operations.remove(operation)
    operations.set('Number', '1')
    sub = tree.getroot().find('Sub_Project')
    sub.set('Number', '1')
    for diagram in list(sub.findall('DiagramItem')):
        if diagram.get('Operation_Id') != '1':
            sub.remove(diagram)
            continue
        for connections in diagram.findall('Connectors/Connections'):
            for item in list(connections):
                connections.remove(item)
            connections.set('Number', '0')
    master = directory/f'{NAME}.MST'
    text = master.read_text(encoding='latin-1')
    markers = list(re.finditer(r'^SIMULATION\s+(\d+)\s*$', text, re.M))
    assert len(markers) >= 2 and [int(m.group(1)) for m in markers] == list(range(1,len(markers)+1))
    master_text = text[:markers[1].start()]
    controls = (directory/'Operations/Task00001/SimCtrl.KEY').read_text(encoding='latin-1')
    assert re.search(r'^NSTEP\s+1\s*$', controls, re.M)
    assert re.search(r'^DTMAX\s+1\.0000000000E-001', controls, re.M)
    config = directory/'JobGroupSetupConfigureFile.txt'
    text = config.read_text(encoding='latin-1')
    for key, value in [('nSimultaneousSimulations','1'), ('nCoreForEachSimulation','1'),
                       ('ComputerNum','0'), ('JobRunMode','E_RUN_ON_LOCALLY')]:
        text, n = re.subn(r'^'+key+r'=.*$', key+'='+value, text, flags=re.M)
        assert n == 1, key
    if not apply:
        return dict(operations=1, steps=1, workers=1, source_simulation_blocks=len(markers))
    project.write_bytes(b'<!DOCTYPE ProjectFile>\n'+ET.tostring(tree.getroot(), encoding='utf-8'))
    master.write_text(master_text, encoding='latin-1', newline='\r\n')
    config.write_text(text, encoding='latin-1', newline='\r\n')
    # Preserve old MPI/run controls as evidence outside the runnable fixture.
    excluded = []
    for name in ['DEF_MPIenv.DAT', 'DEF_MPIp4penv.DAT', 'DefMpi.tmp', 'DEF_ARM_CTL.DAT', 'DEF_MULTI.INI',
                 'REMESH.SCR', 'DEF_PRE.TDB', 'DEF_PRE3.TDB', 'COG_SIM_INPUT.INI']:
        p = directory/name
        if p.exists():
            evidence = ROOT/'build/gui-validation/prepared-fixture-history'/directory.parent.name/directory.name/name
            evidence.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(p, evidence)
            assert digest(p) == digest(evidence)
            p.unlink()  # Exact file inside a newly created, resolved trial only.
            excluded.append(name)
    return dict(operations=1, steps=1, workers=1, deadline_seconds=120,
                remeshing=False, removed_stale_controls=excluded,
                qualification='Format-aware preparation; GUI loading and fresh generation still required before solver execution.')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--root', type=Path, default=Path(r'C:\PROBLEM\R2'))
    ap.add_argument('--apply', action='store_true')
    a = ap.parse_args()
    dest = a.root.resolve()
    assert dest.parent == Path(r'C:\PROBLEM').resolve() and re.fullmatch(r'R\d+', dest.name)
    assert not dest.exists(), 'Use a new empty trial root'
    source_manifest = manifest(SOURCE)
    one_operation(SOURCE)  # Validate all planned edits without modifying the source.
    projected = [str(dest/case/rel) for case in ['O3','C3','OS','CS'] for rel in source_manifest]
    projected += [str(dest/case/'RING.KEY') for case in ['O2','C2']]
    assert max(map(len, projected)) < 100
    report = dict(root=str(dest), source=str(SOURCE), source_manifest=source_manifest,
                  longest_existing_path=max(map(len,projected)),
                  reserved_generated_path=str(dest/'CS'/'SfOprConvert2Dto3D_TEMP_STEP.KEY'),
                  copies={}, applied=a.apply,
                  acceptance='Prepared inputs only; no GUI/backend/controller pass.')
    assert len(report['reserved_generated_path']) < 100
    if a.apply:
        dest.mkdir()
        for case in ['O3','C3','OS','CS']:
            target = dest/case
            shutil.copytree(SOURCE,target)
            assert manifest(target) == source_manifest
            changes = one_operation(target, apply=True) if case.endswith('S') else None
            report['copies'][case] = dict(path=str(target), manifest=manifest(target), controller=changes)
        for case in ['O2','C2']:
            target = dest/case
            target.mkdir()
            shutil.copy2(RING,target/'RING.KEY')
            assert digest(RING) == digest(target/'RING.KEY')
            report['copies'][case] = dict(path=str(target), manifest=manifest(target),
                dimension_evidence='DEFORM-2D header, GEOTYP 1; GUI verification pending')
        assert manifest(SOURCE) == source_manifest
        assert max(len(str(p)) for p in dest.rglob('*')) < 100
        (ROOT/'analysis/gui-validation-fixtures.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ['source_manifest','copies']},indent=2))


if __name__ == '__main__':
    main()
