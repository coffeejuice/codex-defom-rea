"""Create a distinct reviewed script; never rewrite the frozen worker probe."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
source = ROOT / 'build/original-splitter-probe-002/probe.js'
target = source.with_name('probe-reviewed.js')
assert hashlib.sha256(source.read_bytes()).hexdigest() == '086b383d3589107f8a5d0f57a31533693122d2b0d3067f145624f6ae7dc5e1b2'
assert not target.exists()
text = source.read_text(encoding='utf-8')
text = text.replace('roles: 7, states: 80, records: 160', 'roles: 7, states: 160, records: 160')
anchor = "const SCRATCH = "
text = text.replace(anchor, "// Check the expected image before any process-local setup.\nProcess.getModuleByName('DEF_GUI_MO_64.exe');\n" + anchor, 1)
old = '''function focusSnapshot(r, predecessor) {
  return { child: read('focus-child', () => focusLinks(r.object)),
    mainView: read('focus-main', () => focusLinks(mainView)),
    predecessor: read('focus-predecessor', () => focusLinks(predecessor)) };
}'''
new = '''function currentWindow(o) {
  const chain = [], seen = new Set();
  for (let i = 0; i < 32; ++i) {
    if (!o || o.isNull()) return { chain, complete: false, issue: 'null receiver' };
    if (seen.has(str(o))) return { chain, complete: false, issue: 'parent cycle' };
    seen.add(str(o));
    const d = priv(o), data = pp(o.add(0x20));
    if (d.isNull() || data.isNull()) return { chain, complete: false, issue: 'null initialized private/data' };
    const parent = pp(d.add(0x10)), flags = data.add(12).readU32();
    chain.push({ object: str(o), private: str(d), parent: str(parent), windowFlags: flags });
    if ((flags & 1) || parent.isNull()) return { chain, complete: true, window: str(o),
      reason: (flags & 1) ? 'window flag' : 'no parent' };
    o = parent;
  }
  return { chain, complete: false, issue: 'parent depth limit 32' };
}
function focusSnapshot(r, window, predecessor) {
  return { child: read('focus-child', () => focusLinks(r.object)),
    currentWindow: read('focus-window', () => focusLinks(window)),
    entryPredecessor: read('focus-entry-predecessor', () => focusLinks(predecessor)),
    qualification: 'Entry predecessor is pre-state; native partition may change it before the final splice load.' };
}'''
assert old in text
text = text.replace(old, new, 1)
old = '''        this.r = r; this.predecessor = pp(priv(mainView).add(0x98));
        record('focus_splice_enter', { oldWindow: str(cp(args[1])), links: read('focus-before', () => focusSnapshot(r, this.predecessor)) }); },
      onLeave() { if (this.r) record('focus_splice_return', { links: read('focus-after', () => focusSnapshot(this.r, this.predecessor)) }); }'''
new = '''        this.r = r; this.oldWindow = cp(args[1]);
        this.walk = read('focus-window-walk', () => currentWindow(r.object));
        this.window = this.walk.complete ? ptr(this.walk.window) : null;
        this.predecessor = this.window ? pp(priv(this.window).add(0x98)) : null;
        record('focus_splice_enter', { oldWindow: str(this.oldWindow), currentWindowWalk: this.walk,
          sameWindowNoOp: this.window ? same(this.oldWindow, this.window) : null,
          links: read('focus-before', () => focusSnapshot(r, this.window, this.predecessor)) }); },
      onLeave() { if (this.r) record('focus_splice_return', {
        currentWindowWalk: read('focus-return-window-walk', () => currentWindow(this.r.object)),
        links: read('focus-after', () => focusSnapshot(this.r, this.window, this.predecessor)) }); }'''
assert old in text
text = text.replace(old, new, 1)
target.write_text(text, encoding='utf-8', newline='\n')
receipt = dict(status='Prepared, not launched', source=str(source.relative_to(ROOT)),
               source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
               reviewed=str(target.relative_to(ROOT)), reviewed_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),
               correction_contract='analysis/wave3-close-splitter-probe-002-focus-review.json',
               changes=['Expected image check before process-local cwd setup', 'State cap 160',
                        'Bounded actual parent/window walk and same-window no-op observation',
                        'Actual entry-window predecessor; explicitly not a final splice operand claim'],
               qualification='No extra native calls or hooks. Exact consumed predecessor after ring partition remains unobserved.')
(ROOT / 'analysis/wave3-close-splitter-probe-002-reviewed.json').write_text(json.dumps(receipt, indent=2)+'\n', encoding='utf-8')
print(json.dumps(receipt))
