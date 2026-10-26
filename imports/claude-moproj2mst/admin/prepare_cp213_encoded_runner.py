"""Generate the separate HOLD observer from the retained disabled CP210 transport."""
from pathlib import Path
R=Path(__file__).resolve().parent
s=(R/'original_outer_loader_frida_probe.mjs').read_text()
s=s.replace("const CWD = 'C:\\\\PROBLEM\\\\p202a';", "const CWD = 'C:\\\\PROBLEM\\\\p213a';")
s=s.replace("const NATURAL_EXIT_MS = 5000;", "const OBSERVATION_MS = 5000;\nconst QSTRING_LAYOUT_REVIEWED = false;\nconst QT_PATH = path.join(path.dirname(EXE), 'QtCore4_64.dll');\nconst QT_SHA = 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160';\nconst CLI_ARG = '0043003a005c00500052004f0042004c0045004d005c00700032003100330061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a';")
s=s.replace("'acceptance-return', 'outer-return'", "'acceptance-enter', 'acceptance-return', 'outer-return'")
s=s.replace('const EVENT_CAP_TOTAL = 56;', 'const EVENT_CAP_TOTAL = 64;')
q=(R/'cp213_qstring_agent.inc.js').read_text()
s=s.replace('  const actualHooks = [];', q+'\n  const actualHooks = [];')
s=s.replace('registrationCallCompleted: true, patchGatePassed, patches: patchReceipts', 'qtLoadedIdentity: {path: qt.path, base:text(qt.base), size:qt.size, boundedMetadataAndGetterChecks:true}, registrationCallCompleted: true, patchGatePassed, patches: patchReceipts')
s=s.replace('    const pythonHash = fileSha(PYTHON);', "    const pythonHash = fileSha(PYTHON);\n    requireThat(process.version === 'v24.19.0' && fileSha(nodePath) === '3602f2bb1a10f2cbab4c36886218a33c1ab3db87290e73b033c46c77147d0237', 'Node identity mismatch');\n    requireThat(pythonHash === '4d6f5f81a4bca11191c4c7c6b43632694d0a4ce74e068619d8fdc161d469859a', 'Python identity mismatch');\n    requireThat(fileSha(fridaPackagePath) === '7efb59d2050b7b6304e1617f463c4eb7b2f7cb7e3e80e677976825a76ee31e8d', 'Frida package identity mismatch');")
s=s.replace("emit('outer-enter', { ...frame });", "emit('outer-enter', { ...frame, pathWitness: qstring(ptr(frame.pathQString)) });")
s=s.replace("emit('outer-return', { ...frame, rawReturn: text(retval), normalReturn: true });", "emit('outer-return', { ...frame, rawReturn: text(retval), normalReturn: true, pathWitness: qstring(ptr(frame.pathQString)), hookSnapshot: {boundedStateInvalid, sequence: sequence + 1, counts: {...counts, 'outer-return': counts['outer-return'] + 1}, overflow: {...overflow}, rejected: {...rejected}} });")
s=s.replace('        frame.pending = hook.name;', "        if (hook.name === 'acceptance') emit('acceptance-enter', {invocationId: frame.invocationId, main: frame.main, returnAddress: text(this.returnAddress), entryRsp: text(this.context.rsp), rawRCX: text(this.context.rcx), rawRDX: text(this.context.rdx), pathWitness: qstring(this.context.rdx)});\n        frame.pending = hook.name;")
s=s.replace('normalReturn: true });\n      } catch (error) { send({ kind: \'hook-error\', site: hook.name', "normalReturn: true, ...(hook.name === 'acceptance' ? {pathWitness: qstring(ptr(frame.pathQString))} : {}) });\n      } catch (error) { send({ kind: 'hook-error', site: hook.name")
begin=s.index('function completionPrerequisites(');end=s.index('\nasync function run()',begin)
s=s[:begin]+(R/'cp213_encoded_host.inc.js').read_text()+s[end:]
s=s.replace("  requireThat(HOOK_BYTES_REVIEWED === true,", "  requireThat(QSTRING_LAYOUT_REVIEWED === true, 'HOLD: independent QString layout and generated reader review required');\n  requireThat(HOOK_BYTES_REVIEWED === true,")
s=s.replace('`resume210-original-outer-loader-frida-${stamp}`', '`resume213-original-encoded-open-${stamp}`')
s=s.replace("schema: 'original-outer-loader-frida-probe-prepared-v1'", "schema: 'original-encoded-open-prepared-v1'")
s=s.replace("['-m', 'open', '-f', PROJECT]", "['-m', 'open', '-f', CLI_ARG]")
s=s.replace("[EXE, '-m', 'open', '-f', PROJECT]", "[EXE, '-m', 'open', '-f', CLI_ARG]")
s=s.replace("'--', '-m', 'open', '-f', PROJECT", "'--', '-m', 'open', '-f', CLI_ARG")
s=s.replace('naturalExitMs: NATURAL_EXIT_MS', 'observationMs: OBSERVATION_MS')
s=s.replace('    requireThat(canonical(EXE)', '    assertEncodedArgument(CLI_ARG, PROJECT);\n    requireThat(fileSha(QT_PATH) === QT_SHA, \'QtCore4 DLL identity mismatch\');\n    requireThat(canonical(EXE)')
s=s.replace('    result.runnerSha256 =', "    result.qtCoreIdentity = {path: QT_PATH, sha256: fileSha(QT_PATH)};\n    result.runnerSha256 =")
s=s.replace("  let helperTransportOverflow = false, helperTotalBytes = 0;", "  let helperTransportOverflow = false, helperTotalBytes = 0;\n  const endpoint = createEndpoint(OBSERVATION_MS);\n  ready.catch(() => {}); resumed.catch(() => {});")
s=s.replace("  ready.catch(() => {}); resumed.catch(() => {});", "") # declarations follow ready creation
s=s.replace("    stage('owned-helper-started');", "    ready.catch(() => {}); resumed.catch(() => {});\n    stage('owned-helper-started');")
s=s.replace("      if (message.type === 'send' && message.payload?.kind === 'install-readiness-failure')", "      endpoint.accept(message);\n      if (message.type === 'send' && message.payload?.kind === 'install-readiness-failure')")
s=s.replace("    sendCommand('resume');", "    endpoint.start();\n    sendCommand('resume');")
s=s.replace("    // Keep the command channel open. The finally block gives natural exit five seconds.", "    result.observationEndpoint = await endpoint.wait;\n    stage('observation-ended', result.observationEndpoint);\n    result.intentionalTerminationRequested = true;\n    commandChannel.terminateAndEnd();")
begin=s.index('      // The first AV message');end=s.index('      try { await bounded(helperStdoutEnded',begin)
s=s[:begin]+"""      endpoint.cancel();
      if (helper) { result.intentionalTerminationRequested = true; commandChannel.terminateAndEnd(); }
      try { await bounded(helperExit, 22000, 'owned cleanup and watchdog'); }
      catch (error) { result.failures.push({stage: 'helper-exit', message: String(error)}); }
"""+s[end:]
begin=s.index('    const hookEvents =');end=s.index("    try { if (result.fixtureAfter)",begin)
s=s[:begin]+"""    Object.assign(result, qualifyObservation(scriptMessages, childIdentity, creationFiletimeExact, result.handlerReady));
    result.cleanupVerified = result.cleanupReceipt?.exit_verified === true && result.cleanupReceipt?.handles_closed === true && result.cleanupReceipt?.wait_result === 0 && result.helperExit?.code === 0;
    result.processAbsent = result.windowsAbsent === true && result.fridaAbsent === true;
    try { const after = manifest(SOURCE); result.sourceAfterDigest = after.contentDigest; result.sourcePreserved = after.contentDigest === EXPECTED.fixtureManifest; }
    catch (error) { result.sourcePreserved = false; result.failures.push({stage:'source-after', message:String(error)}); }
    result.lifecycleAcceptance = {save: false, acceptedClose: false, complete: false};
    result.releaseBlocked = ['independent exact runner/generated agent review', 'QString layout/reader review', 'native ownership/endpoint review'];
"""+s[end:]
s=s.replace('    result.captureComplete = result.captureComplete && result.completionPrerequisitesMet;', "    result.cleanupComplete = cleanupComplete(result, helperError, helperBuffer, helperTransportOverflow);\n    result.captureComplete = result.completionPrerequisitesMet;\n    result.technicalStatus = 'HOLD_PENDING_INDEPENDENT_REVIEW';")
s=s.replace("run().catch(error", "if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) run().catch(error")
s=s.replace("  const sendCommand = command => { if (!helper?.stdin.destroyed) helper.stdin.write(JSON.stringify({ command }) + '\\n'); };", "  const commandChannel = createCommandChannel(() => helper?.stdin);\n  const sendCommand = command => commandChannel.send(command);")
s += '\nexport { injectedSource, patchReadinessQualified, assertEncodedArgument, qualifyObservation, completionPrerequisites, cleanupComplete, createEndpoint, createCommandChannel, HOOKS, EVENT_KINDS, EXE, PROJECT, CLI_ARG };\n'
(R/'original_encoded_open_frida_probe.mjs').write_text(s)
print('Generated disabled CP213 observer')
