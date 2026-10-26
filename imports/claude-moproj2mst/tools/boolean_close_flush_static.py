"""Retain shipped Qt close notification and QTextStream destructor flush proof."""
import hashlib
import json
from pathlib import Path
import subprocess
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import DUMPBIN


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    output = ROOT/'analysis/wave3-boolean-close-flush-contract.json'
    assert not output.exists()
    binary = ROOT/'evidence/original-binaries/QtCore4_64.dll'
    pe = PE(binary)
    assert sha(pe.data) == 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160'
    entries = [0x670c4b00,0x670c3a70,0x670c2b60,0x670c6dd0,0x670ba470,
               0x67171270,0x670c4f20,0x670c2030,0x670c2210,0x671315c0,0x6712ec70]
    captures = []
    for entry in entries:
        bounds = pe.bounds(entry)
        if entry == 0x67171270:
            end = 0x67171283
        else:
            assert int(bounds['begin'],0) == entry
            end = int(bounds['end_exclusive'],0)
        # The connect body is only needed through type3->type0 normalization.
        selected_end = 0x6713163a if entry == 0x671315c0 else end
        command = [str(DUMPBIN),'/disasm:bytes',f'/range:{hex(entry)},{hex(selected_end-1)}',str(binary)]
        raw = subprocess.run(command,capture_output=True,check=True).stdout
        artifact = ROOT/f'analysis/wave3-boolean-close-flush-{entry:x}.asm'
        assert not artifact.exists()
        artifact.write_bytes(raw)
        captures.append({'entry':hex(entry),'end_exclusive':hex(selected_end),
                         'bounds_qualification':bounds['qualification'] if bounds else 'Exact leaf tail-jump; no unwind entry.',
                         'selection_qualification':'Connect prefix only; remainder not used.' if selected_end != end else 'Complete selected unwind region or exact leaf.',
                         'code_sha256':sha(pe.data[pe.off(entry):pe.off(selected_end)]),
                         'artifact':str(artifact.relative_to(ROOT)),'artifact_sha256':sha(raw),'command':command})
    strings = {}
    for address in [0x6722c308,0x6722c318]:
        pos = pe.off(address)
        strings[hex(address)] = pe.data[pos:pe.data.find(b'\0',pos)].decode()
    assert strings['0x6722c308'] == '2aboutToClose()'
    assert strings['0x6722c318'] == '1flushStream()'
    assert pe.u64(pe.off(0x6722c288)+0x10) == 0x670c6dd0
    sources = [
        'source/apps/moproj2mst/include/m2m/mst/file_device.hpp',
        'source/apps/moproj2mst/src/mst/file_device.cpp',
        'source/apps/moproj2mst/tests/mdt/test_file_device.cpp',
        'source/apps/moproj2mst/include/m2m/mst/stream.hpp',
        'source/apps/moproj2mst/src/mst/stream.cpp',
        'analysis/wave3-boolean-save-buffer-contract.json']
    result = {'schema':'wave3-boolean-close-flush-v1','binary':str(binary.relative_to(ROOT)),
              'binary_sha256':sha(pe.data),'method':'Offline DUMPBIN/direct PE only; no target execution, DLL load, UI, builds or shared REA.',
              'captures':captures,'literals':strings,
              'confirmed_chain':[
                  {'site':'0x670c4b8a','behavior':'QTextStream(QIODevice*) connects device aboutToClose to embedded receiver flushStream; raw connection type3.'},
                  {'site':'0x670c3b12','behavior':'setDevice installs the same connection on its new nonnull device after resetting stream status/state.'},
                  {'site':'0x6713162c..0x67131633','behavior':'QObject::connect normalizes raw type3 to0 (r13d was zeroed at0x671315ee).'},
                  {'site':'0x670c2b95','behavior':'Private constructor installs embedded receiver vtable0x6722c288 at private+8.'},
                  {'site':'0x670c2264','behavior':'Private reset called by external-device constructor clears ownership byte+0x20; external QFile is not deleted by Stream destruction.'},
                  {'site':'0x670c6e04','behavior':'Receiver meta-call InvokeMetaMethod0 calls flushWriteBuffer0x670c2f50 via receiver+0x10=Stream and Stream+8=private.'},
                  {'site':'0x670ba491','behavior':'QIODevice::close emits aboutToClose before clearing openMode at0x670ba498.'},
                  {'site':'0x6717127e','behavior':'aboutToClose signal tail-calls QObject activation0x6712ec70 with QIODevice metaobject0x67256428 and local signal2.'},
                  {'site':'0x6712eef5..0x6712ef19','behavior':'Activation compares receiver thread id with current id; normalized type0 uses queued branch only when different, otherwise direct path0x6712f108. Relevant local QFile/receiver construction is same-thread.'},
                  {'site':'0x670c4f50..0x670c4f56','behavior':'~QTextStream checks pending write buffer length and calls flushWriteBuffer before private destruction when nonempty.'},
                  {'site':'0x670c20e1','behavior':'Private destructor tail-calls exported QObject::~QObject0x6712f870 on embedded receiver after releasing buffers. External-device ownership flag is false, so no QFile close/delete in this branch.'}],
              'correct_close_order':['QFile initial byte-buffer flush, including one retry after prior failure.',
                  'QIODevice aboutToClose while device still open; Stream flushes pending text through its existing sticky-status guard.',
                  'QIODevice clears openMode; QFile clears residual byte buffer and closes its native engine.'],
              'superseded_assumption':{'text':'Unflushed QTextStream text is lost merely because the writer calls QFile.close before QTextStream destruction.',
                  'disposition':'Wrong for the recovered same-thread QFile/QTextStream connection; pending successful-close text is flushed by aboutToClose. Earlier comments/receipts expressing that assumption are superseded by this receipt.',
                  'preserve':'Status3 still suppresses pending text during close notification/destructor. QFile can independently retry a byte-buffered earlier line.'},
              'candidate':{'production_owner_split':'Coordinator owns OutputDevice/Stream callback binding and destructor; worker owns FileDevice notification at exact close slot and five native-device test cases.',
                  'sources_sha256':{s:sha((ROOT/s).read_bytes()) for s in sources},
                  'validation':'Source frozen before coordinator build; worker did no build. Native close-tail/destructor and byte-lock failure/retry tests are present, execution pending coordinator.'},
              'qualification':'Same-thread connection/close/destructor scope only. General cross-thread queued lifecycle and arbitrary Qt device ownership are not reconstructed.',
              'reproducer':str(Path(__file__).relative_to(ROOT)),'reproducer_sha256':sha(Path(__file__).read_bytes())}
    output.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'receipt':str(output),'sha256':sha(output.read_bytes()),'captures':len(captures)}))


if __name__ == '__main__':
    main()
