"""Generate NEW actual-code CP215 tests; old test artifacts remain frozen."""
from pathlib import Path
R=Path(__file__).resolve().parent
s=(R/'test_cp214_postopen_frida_fixture.mjs').read_text().replace('original_postopen_frida_probe.mjs','original_view_xml_frida_probe.mjs').replace('p214a','p215a')
s=s[:s.index('function prepared(')]
s=s.replace("h.name==='activation'", "(h.name==='activation'||h.name==='viewbind')")
s=s.replace('let memory, patches, options, headerReads;', 'let memory, patches, options, headerReads, parserReads;')
s=s.replace('  if(this.n===HEADER',"  if(this.n===0x155n && ++parserReads===2 && options.parserRace) memory.set(0x155n,0);\n  if(this.n===HEADER")
s=s.replace('options=o;headerReads=0;', 'options=o;headerReads=0;parserReads=0;')
s=s.replace(' number(OBJECT,HEADER);', " put(0x155n,[o.parserByte??1]);number(0xc00000n,0xb00000n);number(0xb001b8n,o.slotTarget??BASE+0x170430n);\n number(OBJECT,HEADER);")
s += (R/'cp215_view_xml_mock_cases.inc.js').read_text()
(R/'test_cp215_view_xml_frida_fixture.mjs').write_text(s)
s=(R/'test_cp214_postopen_command_cleanup.mjs').read_text().replace('original_postopen_frida_probe.mjs','original_view_xml_frida_probe.mjs')
(R/'test_cp215_view_xml_command_cleanup.mjs').write_text(s)
s=(R/'test_cp214_postopen_native_fixture.mjs').read_text().replace('original_postopen_frida_probe.mjs','original_view_xml_frida_probe.mjs').replace('cp214p02','cp215v01').replace('resume214-native-postopen-','resume215-native-view-xml-').replace('nine prologue','twelve prologue')
# Runtime callback RVAs for secondary selected caller and dynamic slot target are synthetic-only substitutions.
s=s.replace("  fs.writeFileSync(path.join(OUT,`agent-${mode}-${preResumeReject}.js`),agent);", "  agent=agent.replaceAll('0x383b1',String(meta.exports.viewcurrent_cont)).replaceAll('0x170430',String(meta.hooks.find(h=>h.name==='modelbind').rva));\n  fs.writeFileSync(path.join(OUT,`agent-${mode}-${preResumeReject}.js`),agent);")
start=s.index(' row.passed=');end=s.index('\n fs.writeFileSync',start)
s=s[:start]+" row.passed=clean&&(preResumeReject?row.resumed==null&&row.cleanup.exit_code===0xe201:mode===6?row.cleanup.exit_code===42:mode===9?row.endpoint?.reason==='deadline'&&row.cleanup.exit_code===0xe201&&events.some(e=>e.event==='modelbind-enter')&&!events.some(e=>e.event==='modelbind-return'):row.endpoint?.reason==='postopen-return'&&row.cleanup.exit_code===0xe201&&events.at(-1)?.event==='postopen-return'&&row.witnesses.length===4&&row.witnesses.every(w=>w.witness.ok===true&&w.witness.length===34));"+s[end:]
s=s.replace(" if(mode===8)assert.ok(current?.rawReturn==='0x0'&&!a);", " const status=events.find(e=>e.event==='acceptance-return')?.parserWitness, vb=events.find(e=>e.event==='viewbind-enter'), bc=events.find(e=>e.event==='viewcurrent-return'), bind=events.find(e=>e.event==='modelbind-enter'), bound=events.find(e=>e.event==='modelbind-return');\n row.viewWitnesses={status,viewEntry:vb,viewCurrent:bc,bindEntry:bind,bindReturn:bound};\n if(!preResumeReject&&mode!==6)assert.ok(status?.ok===true&&status.rawByte===1);\n if(mode===5&&!preResumeReject)assert.ok(vb?.rawDL===1&&bc?.rawReturn===post.project&&bind?.receiverSlot?.ok===true&&bind.receiverSlot.matchesConcreteTarget===true&&bind.rawRDX!=='0x0'&&bound?.normalReturn===true&&BigInt(vb.entryRsp)-BigInt(bind.entryRsp)===0xd0n);\n if(mode===8)assert.ok(vb&&!bind);")
s=s.replace("post?.project==='0x100'", "post?.project===events.find(e=>e.event==='outer-return')?.rawReturn")
s=s.replace("current?.rawReturn==='0x100'", "current?.rawReturn===post.project").replace("a?.rawRCX==='0x100'", "a?.rawRCX===post.project")
(R/'test_cp215_view_xml_native_fixture.mjs').write_text(s)
