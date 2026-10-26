"""NEW disabled CP215 observer from frozen CP214; no original execution."""
from pathlib import Path
import hashlib
R=Path(__file__).resolve().parent
b=(R/'original_postopen_frida_probe.mjs').read_bytes()
assert hashlib.sha256(b).hexdigest()=='54ce58f2cc0ca7451d37689ef490af98497137734d0f7a64be89c8cde2bba902'
s=b.decode().replace('\r\n','\n').replace('p214a','p215a').replace('00700032003100340061','00700032003100350061')
s=s.replace('const HOOKS = [...OUTER_HOOKS,...POST_HOOKS];', "const VIEW_HOOKS = [{name:'viewbind',rva:0x38380,bytes:'8854241048894c24084881ecc8000000',callerRva:0x2aaad},{name:'viewaccess',rva:0x12fa0,bytes:'48894c24084883ec28',callerRva:0x383e1},{name:'modelbind',rva:0x170430,bytes:'488954241048894c2408',callerRva:0x3862b}];\nconst HOOKS = [...OUTER_HOOKS,...POST_HOOKS,...VIEW_HOOKS];")
s=s.replace("'postopen-return'];", "'postopen-return','viewbind-enter','viewcurrent-enter','viewcurrent-return','viewaccess-enter','viewaccess-return','modelbind-enter','modelbind-return','viewbind-return'];",1)
s=s.replace('const EVENT_CAP_TOTAL = 112;', 'const EVENT_CAP_TOTAL = 176;')
s=s.replace('const postHooks = hooks.slice(6);','const postHooks = hooks.slice(6,9);')
s=s.replace('const rawReturn = text(retval), lowByte = retval.toUInt32() & 255;', 'const rawReturn = text(retval), lowByte = retval.toUInt32() & 255;')
s=s.replace('this.selectedCall = { frame, returnAddress:', 'this.selectedCall = { frame, rawRCX:text(this.context.rcx), returnAddress:')
s=s.replace("{pathWitness: qstring(ptr(frame.pathQString))} : {})", "{pathWitness: qstring(ptr(frame.pathQString)),parserWitness: projectByte(ptr(call.rawRCX))} : {})")
s=s.replace('    postStateInvalid,postSerial', '    postStateInvalid,postSerial')
s=s.replace('return {postStateInvalid,postSerial,postRejected:', 'return {viewStateInvalid,viewSerial,viewRejected:{...viewRejected},postStateInvalid,postSerial,postRejected:')
s=s.replace('      this.postCall=null;\n      try {', "      this.postCall=null;this.viewCurrentCall=null;\n      try {\n        if(hook.name==='current' && this.returnAddress.equals(app.base.add(0x383b1))) { viewCurrentEnter.call(this); return; }")
s=s.replace('try {const call=this.postCall;if(!call)return;', 'try {if(this.viewCurrentCall){viewCurrentLeave.call(this,retval);return;} const call=this.postCall;if(!call)return;')
s=s.replace('  // Installation gate runs', (R/'cp215_view_xml_agent.inc.js').read_text()+'\n  // Installation gate runs')
s=s.replace("hook.name === 'activation'", "(hook.name === 'activation' || hook.name === 'viewbind')")
s=s.replace("hook.name !== 'activation'", "(hook.name !== 'activation' && hook.name !== 'viewbind')")
s=s.replace('function qualifyObservation(messages,identity,filetime,ready)', 'function qualifyPredecessorObservation(messages,identity,filetime,ready)')
s=s.replace('function cleanupComplete(', (R/'cp215_view_xml_host.inc.js').read_text()+'\nfunction cleanupComplete(',1)
s=s.replace('r.postOpenObservationComplete === true', 'r.postOpenObservationComplete === true && r.predecessorChainQualified === true && r.viewObservationComplete === true && r.parserStateQualified === true')
s=s.replace("schema: 'original-postopen-prepared-v1'", "schema: 'original-view-xml-prepared-v1'").replace('resume214-original-postopen-','resume215-original-view-xml-').replace('interceptorHooks: 9','interceptorHooks: 12')
s=s.replace('      if (enforceShortPaths)', "      if (st.isFile()) requireThat(st.nlink === 1, `Hardlinked fixture entry rejected: ${item}`);\n      if (enforceShortPaths)")
s=s.replace('OUTER_HOOKS, POST_HOOKS, HOOKS,', 'OUTER_HOOKS, POST_HOOKS, VIEW_HOOKS, HOOKS,')
(R/'original_view_xml_frida_probe.mjs').write_text(s)
print('Generated disabled CP215 observer')
