"""Generate NEW disabled CP214 runner from hash-frozen repaired CP213 transport."""
from pathlib import Path
import hashlib
R=Path(__file__).resolve().parent
b=(R/'original_encoded_open_frida_probe.mjs').read_bytes()
assert hashlib.sha256(b).hexdigest()=='bb88fb26717683c7c97627453b742d042ab0f1074388d54a98ed0772b7519318'
s=b.decode().replace('\r\n','\n')
s=s.replace('p213a','p214a').replace('00700032003100330061','00700032003100340061')
s=s.replace('const OBSERVATION_MS = 5000;', 'const OBSERVATION_MS = 12000;')
s=s.replace('const HELPER_MAX_SECONDS = 20;', 'const HELPER_MAX_SECONDS = 30;')
s=s.replace("'owned_suspended_process.py'", "'cp214_owned_suspended_process.py'")
helper=(R/'owned_suspended_process.py').read_bytes()
assert hashlib.sha256(helper).hexdigest()=='8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d'
h=helper.decode().replace('<= 20:', '<= 30:').replace('in (0, 20]', 'in (0, 30]')
(R/'cp214_owned_suspended_process.py').write_bytes(h.encode())
helper_hash=hashlib.sha256(h.encode()).hexdigest()
s=s.replace('8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d',helper_hash)
s=s.replace("const HOOKS = [", "const OUTER_HOOKS = [",1)
needle="const EVENT_KINDS = "
i=s.index(needle)
s=s[:i]+"const POST_HOOKS = [{name:'postopen',rva:0x2a0e0,bytes:'488954241048894c2400856574881ec88040000',callerRva:0x1cae},{name:'current',rva:0x45780,bytes:'48894c24084883ec48',callerRva:0x2aa72},{name:'activation',rva:0xa7250,bytes:'8954241048894c24084883ec38',callerRva:0x2aaa1}];\nconst HOOKS = [...OUTER_HOOKS,...POST_HOOKS];\n"+s[i:]
# Correct exact postOpen prefix (two MOVs, two pushes, SUB).
s=s.replace('488954241048894c2400856574881ec88040000','488954241048894c240856574881ec88040000')
s=s.replace("'outer-return'];", "'outer-return','postopen-enter','current-enter','current-return','activation-enter','activation-return','postopen-return'];",1)
s=s.replace('const EVENT_CAP_TOTAL = 64;', 'const EVENT_CAP_TOTAL = 112;')
s=s.replace('const steps = hooks.slice(1).map', 'let lastOuter = null;\n  const steps = hooks.slice(1,6).map')
s=s.replace('for (const hook of hooks.slice(1)) Interceptor.attach', 'for (const hook of hooks.slice(1,6)) Interceptor.attach')
s=s.replace("        stack.pop(); if (!stack.length)", "        lastOuter = {...frame, rawReturn:text(retval), outerSequence:sequence};\n        stack.pop(); if (!stack.length)")
s=s.replace('  // Installation gate runs', (R/'cp214_postopen_agent.inc.js').read_text()+'\n  // Installation gate runs')
s=s.replace('receipt.suffixUnchanged = receipt.after.slice(10) === hook.bytes.slice(10);', "receipt.displacedLength = hook.name === 'activation' ? 9 : 5;\n      receipt.padding = hook.name === 'activation' ? receipt.after.slice(10,18) : '';\n      receipt.suffixUnchanged = receipt.after.slice(receipt.displacedLength*2) === hook.bytes.slice(receipt.displacedLength*2);")
s=s.replace("receipt.redirectSize === 5 && receipt.suffixUnchanged;", "receipt.redirectSize === 5 && receipt.suffixUnchanged && (hook.name !== 'activation' || receipt.padding === '0f1f4000');")
s=s.replace("receipt.after.slice(10) !== hook.bytes.slice(10) ||", "receipt.displacedLength !== (hook.name === 'activation' ? 9 : 5) || (hook.name === 'activation' && (receipt.padding !== '0f1f4000' || receipt.after.slice(10,18) !== '0f1f4000')) || receipt.after.slice(hook.name === 'activation' ? 18 : 10) !== hook.bytes.slice(hook.name === 'activation' ? 18 : 10) ||")
s=s.replace('requires_e9_rel32_and_unchanged_full_prefix_suffix','requires_e9_rel32_strict5_or_activation_exact9_nop_window')
s=s.replace('function qualifyObservation(messages, identity, filetime, ready)', 'function qualifyOuterObservation(messages, identity, filetime, ready)')
# Prior outer qualification must compare only the first six hooks and bounded prefix.
start=s.index('function qualifyOuterObservation(');end=s.index('\nfunction cleanupComplete(',start)
outer=s[start:end].replace('HOOKS[','OUTER_HOOKS[')
s=s[:start]+outer+s[end:]
s=s.replace('function cleanupComplete(', (R/'cp214_postopen_host.inc.js').read_text()+'\nfunction cleanupComplete(',1)
s=s.replace("message.payload.event === 'outer-return'", "message.payload.event === 'postopen-return'")
s=s.replace("r.observationEndpoint?.reason === 'outer-return'", "r.observationEndpoint?.reason === 'postopen-return'")
s=s.replace("end('outer-return')", "end('postopen-return')")
s=s.replace("r.openObservationComplete === true && r.encodedPathQualified === true", "r.openObservationComplete === true && r.encodedPathQualified === true && r.postOpenObservationComplete === true")
s=s.replace("schema: 'original-encoded-open-prepared-v1'", "schema: 'original-postopen-prepared-v1'")
s=s.replace('resume213-original-encoded-open-', 'resume214-original-postopen-')
s=s.replace('interceptorHooks: 6', 'interceptorHooks: 9')
s=s.replace("    result.observationEndpoint = await endpoint.wait;", "    result.observationEndpoint = await Promise.race([endpoint.wait, helperExit.then(() => ({reason:'natural-exit', elapsedMs:null}))]);")
s=s.replace('createEndpoint, createCommandChannel, HOOKS,', 'createEndpoint, createCommandChannel, OUTER_HOOKS, POST_HOOKS, HOOKS,')
(R/'original_postopen_frida_probe.mjs').write_text(s)
print('Generated disabled CP214 runner; helper SHA256 '+helper_hash)
