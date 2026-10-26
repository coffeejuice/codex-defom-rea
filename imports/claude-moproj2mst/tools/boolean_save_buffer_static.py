"""Close the native-path QFile buffering contract with offline PE evidence."""
import hashlib
import json
from pathlib import Path
import subprocess
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import DUMPBIN, imports


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    out = ROOT/'analysis/wave3-boolean-save-buffer-contract.json'
    assert not out.exists()
    binary = ROOT/'evidence/original-binaries/QtCore4_64.dll'
    pe = PE(binary)
    assert sha(pe.data) == 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160'
    captures = []
    entries = [0x670b5c70,0x670b5b30,0x670b5340,0x670b58b0,0x670b5a20,
               0x670b5d80,0x670b6010,0x670ea8e0,0x670f8690,0x670f82b0,
               0x670f9420,0x670ea7e0,0x670f7ee0]
    leaf_ends = {0x670ea7e0:0x670ea7f2,0x670f7ee0:0x670f7eef}
    for entry in entries:
        bounds = pe.bounds(entry)
        end = leaf_ends.get(entry) or int(bounds['end_exclusive'],0)
        if bounds: assert int(bounds['begin'],0) == entry
        cmd = [str(DUMPBIN),'/disasm:bytes',f'/range:{hex(entry)},{hex(end-1)}',str(binary)]
        raw = subprocess.run(cmd,capture_output=True,check=True).stdout
        path = ROOT/f'analysis/wave3-boolean-save-buffer-{entry:x}.asm'
        assert not path.exists()
        path.write_bytes(raw)
        captures.append({'entry':hex(entry),'end_exclusive':hex(end),
                         'bounds_qualification':bounds['qualification'] if bounds else 'Leaf selected through exact return/tail jump; no unwind entry.',
                         'code_sha256':sha(pe.data[pe.off(entry):pe.off(end)]),
                         'artifact':str(path.relative_to(ROOT)),'artifact_sha256':sha(raw),'command':cmd})
    imp = imports(pe)
    assert imp[0x671a8170] == 'WriteFile'
    assert imp[0x671a83d0] == 'CloseHandle'
    files = ['source/apps/moproj2mst/include/m2m/mst/file_device.hpp',
             'source/apps/moproj2mst/src/mst/file_device.cpp',
             'source/apps/moproj2mst/src/engine/boolean_editor.cpp',
             'source/apps/moproj2mst/tests/mdt/test_file_device.cpp',
             'analysis/wave3-boolean-save-io-contract.json']
    result = {'schema':'wave3-boolean-save-buffer-v1','binary':str(binary.relative_to(ROOT)),
              'binary_sha256':sha(pe.data),'method':'Offline DUMPBIN/PE reads only. No target or original function execution, DLL load, UI, build, fault injection or shared REA.',
              'captures':captures,'imports':{'0x671a8170':imp[0x671a8170],'0x671a83d0':imp[0x671a83d0]},
              'contract':{
                  'basic_block':'QFilePrivate ctor0x670b5b30 calls ring ctor at0x670b5b8a with0x4000.',
                  'write_data':'0x670b6010: own mode2 buffers; pending+incoming>0x4000 flushes first and returns-1 if that flush fails. Incoming>0x4000 then bypasses buffer, otherwise reserve/append and return input size.',
                  'reserve':'0x670b5340: empty ring reserves max(basicBlock,newSize). Otherwise append if tail+newSize fits logical backing length. If not, resize same block when tail<length/2; otherwise shrink old block to tail and append max(basicBlock,newSize) block.',
                  'flush_one_segment':'0x670b5d80 calls engine.write on first contiguous segment and consumes a positive returned prefix. QFile::flush0x670b5e00 compares that result with total pending count captured before writing; mismatch returns false. It does not drain multiple segments in a loop.',
                  'native_write':'0x670f8690 for path handle: count starts at low32(input length), each WriteFile uses min(remaining,0x2000000), successful positive writes loop. Zero stops. Failure before any progress returns-1 and sets engine error2; failure after progress returns the positive prefix.',
                  'native_flush':'QFS flush leaf0x670ea7e0 ->0x670f7ee0. Path-open native handle has stdio pointer+0x70==0, so native flush returns true without FlushFileBuffers.',
                  'close':'QFile::close0x670b5f30 calls QFile::flush once independently of QTextStream status, invokes QIODevice close, clears its ring, and closes engine. Thus buffered remainder can be retried once at close; remaining bytes are discarded before native CloseHandle. A close error updates QFile error state, which Boolean helpers do not inspect.',
                  'truncate':'Path-open handle initially has offset0. QFS native open invokes setSize(0), then returns true without checking its result. FileDevice uses SetEndOfFile on that fresh handle and ignores its result.'},
              'candidate':{'sources_sha256':{s:sha((ROOT/s).read_bytes()) for s in files},
                  'test_target':'m2m_mdt_file_device','ctest':'m2m.mdt.file_device','test_cases':4,
                  'scope':'Native path-open WriteOnly only; not FILE*, CRT-descriptor, arbitrary QIODevice, sequential-device or Qt filesystem-plugin reconstruction.',
                  'validation':'Source/static checks only in worker; parent owns build/test. Native tests cover truncate, deferred small writes, overflow boundary, large direct path, close flush, failed opens, and deterministic LockFileEx-induced flush failure followed by unlock and close retry. Actual OS positive-short-write and close-failure injection remain untested.'},
              'adapter_qualifications':['Existing win32Path long-path extension is preserved; this does not claim original Qt long-path equivalence.',
                  'Original Qt error strings/enums remain private to its file objects; FileDevice exposes the OutputDevice result contract without inventing GUI messages.',
                  'Earlier Save stages still use the inherited deferred effects runner. This package closes native MST/Boolean device behavior, not all writer failure ordering.'],
              'reproducer':str(Path(__file__).relative_to(ROOT)),'reproducer_sha256':sha(Path(__file__).read_bytes())}
    out.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'receipt':str(out),'sha256':sha(out.read_bytes()),'captures':len(captures),'test_cases':4}))


if __name__ == '__main__':
    main()
