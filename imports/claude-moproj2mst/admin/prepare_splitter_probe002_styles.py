"""Add source-proven passive style observations to a separately frozen probe."""
import hashlib
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
source = root / 'build/original-splitter-probe-002/probe-reviewed.js'
target = source.with_name('probe-reviewed-v2.js')
assert hashlib.sha256(source.read_bytes()).hexdigest() == '7d8a1a7d0da678f164a1ce01fb016e470040ff5dc6f98cf2f6e555282ddc6f53'
assert not target.exists()
text = source.read_text(encoding='utf-8')
anchor = 'function extraFields(d) {'
assert anchor in text
text = text.replace(anchor, '''function stringExtent(address) {
  const data = pp(address);
  return { address: str(address), data: str(data),
    length: data.isNull() ? null : data.add(8).readS32(),
    qualification: 'Shipped Qt4 QString data+8, proven by QString::isEmpty; no text bytes read.' };
}
''' + anchor, 1)
text = text.replace("if (e.isNull()) return { extra: str(e), nativeChildren: false, coverage: 'absent extra' };",
                    "if (e.isNull()) return { extra: str(e), nativeChildren: false, style: null, stylesheet: { absentExtra: true, length: 0 }, coverage: 'absent extra' };")
text = text.replace("proxy: str(pp(e.add(0x10))), cursor: str(pp(e.add(0x18))), topExtra: str(t),",
                    "proxy: str(pp(e.add(0x10))), cursor: str(pp(e.add(0x18))), style: str(pp(e.add(0x20))), stylesheet: stringExtent(e.add(0x38)), topExtra: str(t),")
text = text.replace("applicationType: g.add(0x9c9670).readS32(), allWidgets:",
                    "applicationStylesheet: stringExtent(g.add(0x9c9738)), applicationType: g.add(0x9c9670).readS32(), allWidgets:")
target.write_text(text, encoding='utf-8', newline='\n')
receipt = dict(status='Prepared, not launched', source=str(source.relative_to(root)),
               source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
               reviewed=str(target.relative_to(root)), reviewed_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),
               source_contracts=['analysis/wave3-closure-next-close-splitter-style-empty-leaf-static.json',
                                 'analysis/wave3-closure-next-close-splitter-parent-style-getters-static.json'],
               changes=['Passive widget/application QString shared-data pointer and length at data+8',
                        'Passive parent/style extra+20 pointer'],
               qualification='No additional native call, hook, full text, or style identity inference.')
(root / 'analysis/wave3-close-splitter-probe-002-reviewed-v2.json').write_text(json.dumps(receipt, indent=2)+'\n', encoding='utf-8')
print(json.dumps(receipt))
