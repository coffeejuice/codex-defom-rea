"""Derive NEW actual-code tests; never run original processes."""
from pathlib import Path
R=Path(__file__).resolve().parent
s=(R/'test_encoded_open_frida_fixture.mjs').read_text().replace("'./original_encoded_open_frida_probe.mjs'","'./original_postopen_frida_probe.mjs'").replace('p213a','p214a')
s=s.replace("console.log(JSON.stringify({outcome:'PASS',scope:'actual_generated_agent_and_host_mock',originalExecuted:false,guiControlAcquired:false}));",'')
s=s.replace('import {injectedSource, HOOKS,','import {patchReadinessQualified, injectedSource, HOOKS,')
# Replace old qualification/endpoint tests with dedicated postOpen suite.
start=s.index('const qualify=f=>');s=s[:start]
s=s.replace("for(const h of HOOKS)","for(const h of HOOKS)")
s=s.replace(' return {messages,outer,stage', ' return {messages,hooks,call,outer,stage')
s=s.replace("'00100000'+h.bytes.slice(10)","'00100000'+(h.name==='activation'?'0f1f4000'+h.bytes.slice(18):h.bytes.slice(10))")
s=s.replace("h.name==='activation'?'0f1f4000'+h.bytes.slice(18):h.bytes.slice(10)","h.name==='activation'?(o.badPadding?'90909090':'0f1f4000')+(o.badSuffix?'cc'+h.bytes.slice(20):h.bytes.slice(18)):h.bytes.slice(10)")
s=s.replace('if(o.badPatch){','if(o.badPatch||o.badPadding||o.badSuffix){')
s += (R/'cp214_postopen_mock_cases.inc.js').read_text()
(R/'test_cp214_postopen_frida_fixture.mjs').write_text(s)
s=(R/'test_encoded_open_command_cleanup.mjs').read_text().replace('original_encoded_open_frida_probe.mjs','original_postopen_frida_probe.mjs')
(R/'test_cp214_postopen_command_cleanup.mjs').write_text(s)
s=(R/'test_encoded_open_native_fixture.mjs').read_text().replace("'./original_encoded_open_frida_probe.mjs'","'./original_postopen_frida_probe.mjs'")
s=s.replace('cp213e02','cp214p02').replace("'owned_suspended_process.py'","'cp214_owned_suspended_process.py'")
s=s.replace('8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d','0e5389f7dd3f734557d87725e8dad422a3daafc507b230f060fb900da5c14edf')
s=s.replace('resume213-native-encoded-','resume214-native-postopen-').replace('six prologue','nine prologue').replace("'--max-seconds','20'","'--max-seconds','30'")
s=s.replace("row.endpoint?.reason==='outer-return'", "row.endpoint?.reason==='postopen-return'")
start=s.index(' row.passed=');end=s.index('\n fs.writeFileSync',start)
s=s[:start]+" row.passed=clean&&(preResumeReject?row.resumed==null&&row.cleanup.exit_code===0xe201:mode===6?row.cleanup.exit_code===42:mode===9?row.endpoint?.reason==='deadline'&&row.cleanup.exit_code===0xe201&&events.some(e=>e.event==='activation-enter')&&!events.some(e=>e.event==='activation-return'):row.endpoint?.reason==='postopen-return'&&row.cleanup.exit_code===0xe201&&events.at(-1)?.event==='postopen-return'&&row.witnesses.length===4&&row.witnesses.every(w=>w.witness.ok===true&&w.witness.length===34));"+s[end:]
s=s.replace(' const clean=row.cleanup', " const post=events.find(e=>e.event==='postopen-enter'), current=events.find(e=>e.event==='current-return'), a=events.find(e=>e.event==='activation-enter'), ar=events.find(e=>e.event==='activation-return');\n row.nativeWitnesses={postDelivered:post?.project==='0x100'&&post.main===events[0]?.main,current:current?.rawReturn,activationRCX:a?.rawRCX,activationEDX:a?.rawEDX,activationNormal:ar?.normalReturn===true,stackDelta:a&&post?String(BigInt(post.entryRsp)-BigInt(a.entryRsp)):null};\n if(mode===5&&!preResumeReject)assert.ok(row.nativeWitnesses.postDelivered&&current?.rawReturn==='0x100'&&a?.rawRCX==='0x100'&&a?.rawEDX===0&&ar?.normalReturn===true&&row.nativeWitnesses.stackDelta==='1248');\n if(mode===8)assert.ok(current?.rawReturn==='0x0'&&!a);\n const clean=row.cleanup")
s=s.replace('try{await trial(5);await trial(4);await trial(6);await trial(7);await trial(5,true);','try{await trial(5);await trial(8);await trial(9);await trial(6);await trial(5,true);')
(R/'test_cp214_postopen_native_fixture.mjs').write_text(s)
