// One helper stdin lifecycle. write(false) means queued with backpressure, not failure.
// Keep termination and end idempotent even during the gap before stream destruction.
function createCommandChannel(getStream) {
  let terminationSent = false, ended = false;
  function writable(stream) {
    return stream && !ended && !stream.destroyed && !stream.writableEnded && !stream.writableFinished && stream.writable !== false;
  }
  function send(command) {
    const stream = getStream();
    if (!writable(stream) || (command === 'terminate' && terminationSent)) return false;
    stream.write(JSON.stringify({command}) + '\n');
    if (command === 'terminate') terminationSent = true;
    return true;
  }
  return {send, terminateAndEnd() {
    const stream = getStream();
    send('terminate');
    if (writable(stream)) {ended = true; stream.end();}
    return {terminationSent, ended};
  }};
}
function assertEncodedArgument(token, project) {
  const fixed = '0043003a005c00500052004f0042004c0045004d005c00700032003100330061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a';
  requireThat(token === fixed && token.length === 136 && /^[0-9a-f]+$/.test(token) && token.length % 4 === 0, 'Exact canonical 136-hex argument required');
  const units = token.match(/.{4}/g).map(x => parseInt(x, 16));
  requireThat(units.length === 34 && String.fromCharCode(...units) === project, 'Encoded argument roundtrip mismatch');
}
function createEndpoint(ms, now = () => performance.now()) {
  let finish, timer, started = null, settled = false;
  const wait = new Promise(resolve => {finish = resolve;});
  const end = reason => {if (!settled) {settled = true; clearTimeout(timer); finish({reason, elapsedMs: started === null ? null : now() - started});}};
  return {wait, start() {if (started !== null) throw Error('Observation resumed twice'); started = now(); timer = setTimeout(() => end('deadline'), ms);},
    accept(message) {
      if (settled || started === null) return;
      if (now() - started >= ms) {end('deadline'); return;}
      if (message.type === 'error' || ['hook-error', 'exception-handler-error'].includes(message.payload?.kind)) end('error');
      else if (message.payload?.kind === 'first-av-core') end('first-av');
      else if (message.payload?.kind === 'hook' && message.payload.event === 'outer-return') end('outer-return');
    }, cancel() {clearTimeout(timer);}};
}
function qualifyObservation(messages, identity, filetime, ready) {
  const out = {openObservationComplete: false, encodedPathQualified: false, postOpenRoute: {observed: false, status:'not_instrumented', basis:'CP209 static main forwards outer RAX to 2a0e0 unconditionally; no added hook'}};
  try {
    const events = messages.filter(m => m.type === 'send' && m.payload?.kind === 'hook').map(m => m.payload);
    const returns = events.filter(e => e.event === 'outer-return'), entries = events.filter(e => e.event === 'outer-enter');
    requireThat(returns.length === 1 && entries.length === 1, 'Unique outer pair required');
    const entry = entries[0], returned = returns[0], snapshot = returned.hookSnapshot;
    requireThat(events.length <= EVENT_CAP_TOTAL && events.every((e,i) => e.sequence === i+1 && e.pid === identity.pid && e.processCreationFiletime === filetime && e.tid === identity.primary_tid && e.invocationId === entry.invocationId), 'Sequence/identity mismatch');
    requireThat(events[0] === entry && events.at(-1) === returned && returned.normalReturn === true && !returned.invalid && !returned.pending && snapshot?.boundedStateInvalid === false, 'Outer frame incomplete');
    requireThat(snapshot.sequence === events.length && EVENT_KINDS.every(k => snapshot.counts[k] === events.filter(e => e.event === k).length && snapshot.counts[k] < EVENT_CAP_PER_KIND && snapshot.overflow[k] === 0), 'Counter parity failed');
    requireThat(['noUniqueOwner','outerLeaveMismatch','order','overlap'].every(k => snapshot.rejected[k] === 0), 'Frame attribution rejected');
    requireThat(!messages.some(m => m.type === 'error' || ['hook-error','exception-handler-error','install-readiness-failure'].includes(m.payload?.kind)), 'Observer errors');
    const base = BigInt(ready.module.base);
    requireThat(BigInt(entry.returnAddress) === base + BigInt(HOOKS[0].callerRva) && BigInt(entry.pathQString) === BigInt(entry.main) + 0x18d8n && returned.main === entry.main && returned.pathQString === entry.pathQString && returned.entryRsp === entry.entryRsp && returned.returnAddress === entry.returnAddress, 'Outer identity mismatch');
    const stages = events.filter(e => e.event.endsWith('-return') && e.event !== 'outer-return');
    let failed = null;
    for (let i=0; i<stages.length; i++) {
      const e=stages[i], hook=HOOKS[i+1];
      requireThat(hook && e.event === hook.name+'-return' && e.normalReturn === true && e.main === entry.main && BigInt(e.returnAddress) === base + BigInt(hook.callerRva) && BigInt(e.entryRsp) === BigInt(entry.entryRsp)-0x1e0n && failed === null, 'Helper order/frame mismatch');
      const raw=BigInt(e.rawReturn), low=Number(raw & 255n);
      const failure=hook.name === 'duplicate' ? raw !== 0n : hook.name === 'acceptance' ? low === 0 : raw === 0n;
      requireThat(e.lowByte === low && e.failed === failure, 'Raw scalar disagreement');
      if(failure) failed=hook.name;
    }
    requireThat(stages.length > 0 && (failed !== null || stages.length === 5) && (failed === null ? BigInt(returned.rawReturn)!==0n : BigInt(returned.rawReturn)===0n), 'Incomplete prerequisite prefix');
    const accepts = events.filter(e=>e.event==='acceptance-enter');
    requireThat(accepts.length === (stages.length === 5 ? 1 : 0), 'Acceptance entry count mismatch');
    const witness = w => w?.ok === true && w.rawReadStatus === 'exact_length_stable_reread' && w.truncated === false && w.object === entry.pathQString && w.length === 34 && Array.isArray(w.units) && w.units.length === 34 && w.units.every((n,i)=>n===PROJECT.charCodeAt(i)) && w.text === PROJECT;
    requireThat(witness(entry.pathWitness) && witness(returned.pathWitness), 'Outer path witness incomplete');
    if(accepts.length) {
      const a=accepts[0], project=stages[3], acceptance=stages[4];
      requireThat(a.sequence === project.sequence+1 && acceptance.sequence === a.sequence+1 && a.rawRCX === project.rawReturn && a.rawRDX === entry.pathQString && a.main === entry.main && BigInt(a.returnAddress)===base+BigInt(HOOKS[5].callerRva) && BigInt(a.entryRsp)===BigInt(entry.entryRsp)-0x1e0n && witness(a.pathWitness) && witness(acceptance.pathWitness), 'Reader path/receiver attribution incomplete');
      out.encodedPathQualified=true;
    }
    out.outerObservationQualified=true; out.openObservationComplete=true; out.firstFailedPrerequisite=failed; out.rawOuterReturn=returned.rawReturn;
    out.postOpenRoute.inferredArm=BigInt(returned.rawReturn)===0n ? 'null_refresh_arm' : 'nonnull_arm';
  } catch(error) {out.qualificationError=String(error);}
  return out;
}
function cleanupComplete(r, error, buffer, overflow) {
  return r.cleanupVerified === true && r.processAbsent === true && r.helperStdoutClosed === true && error === null && buffer === '' && !overflow && r.helperStdinError == null && r.scriptUnloadSettled === true && r.sessionDetached === true && r.rawHelperReceiptsRetained === true && r.failures.length === 0;
}
function completionPrerequisites(r,error,buffer,overflow) {
  return r.patchReadinessQualified === true && r.installReadinessFailure == null && r.openObservationComplete === true && r.encodedPathQualified === true && cleanupComplete(r,error,buffer,overflow) && r.sourcePreserved === true && !!r.fixtureAfter && r.hostMessageOverflow == null && r.resumedReceipt?.previous_suspend_count === 1 && ['running','exited'].includes(r.resumedReceipt.state) && r.observationEndpoint?.reason === 'outer-return' && r.observationEndpoint.elapsedMs < OBSERVATION_MS && r.intentionalTerminationRequested === true && !(r.cleanupReceipt?.termination_attempts??[]).some(a=>a.source==='watchdog_deadline');
}
