  // Qualified against exact QtCore getters; never calls into the target.
  const qt = Process.findModuleByName('QtCore4_64.dll');
  if (!qt || qt.path.toLowerCase() !== ${JSON.stringify(QT_PATH.toLowerCase())}) throw new Error('Loaded QtCore identity mismatch');
  const qtMatches = Process.enumerateModules().filter(m => m.name.toLowerCase() === 'qtcore4_64.dll');
  if (qtMatches.length !== 1 || !qtMatches[0].base.equals(qt.base) || qt.size !== 0x31f000) throw new Error('Loaded QtCore uniqueness/size mismatch');
  const qtPe = qt.base.add(qt.base.add(0x3c).readU32());
  if (qt.base.readU16() !== 0x5a4d || qtPe.readU32() !== 0x4550 || qtPe.add(4).readU16() !== 0x8664 || qtPe.add(8).readU32() !== 0x517180cf || qtPe.add(24).readU16() !== 0x20b || qtPe.add(80).readU32() !== 0x31f000) throw new Error('Loaded QtCore PE metadata mismatch');
  const hexBytes = bytes => Array.from(new Uint8Array(bytes), x => x.toString(16).padStart(2, '0')).join('');
  const qtGetters = [{rva:0xfec0,bytes:'488b018b4008c3'},{rva:0x2e90,bytes:'488b018b4004c3'},{rva:0x47940,bytes:'488b01488b4010c3'}];
  for (const getter of qtGetters) if (hexBytes(qt.base.add(getter.rva).readByteArray(getter.bytes.length/2)) !== getter.bytes) throw new Error('Loaded QtCore getter bytes mismatch');
  function readable(address, bytes) {
    if (!Number.isSafeInteger(bytes) || bytes < 0 || bytes > 256 || address.isNull()) throw new Error('invalid read bounds');
    const end = address.add(bytes);
    if (end.compare(address) < 0) throw new Error('pointer wrap');
    let cursor = address, ranges = 0;
    while (cursor.compare(end) < 0) {
      const range = Process.findRangeByAddress(cursor);
      if (++ranges > 256 || !range || !range.protection.includes('r')) throw new Error('unreadable range');
      const next = range.base.add(range.size);
      if (next.compare(cursor) <= 0) throw new Error('invalid mapped range');
      cursor = next.compare(end) < 0 ? next : end;
    }
    const buffer = address.readByteArray(bytes);
    if (!buffer || buffer.byteLength !== bytes) throw new Error('partial range read');
    return buffer;
  }
  function qstring(object) {
    const out = {object: text(object), ok: false, units:null, text:null, rawReadStatus: 'unread', recheckStatus:'not_reached', truncated: false, allocationValidity:'structural-and-readable-only'};
    try {
      const objectBuffer = readable(object, 8), objectBefore = hexBytes(objectBuffer);
      out.objectBytes = objectBefore;
      if (ptr(object).and(7).toUInt32() !== 0) throw new Error('unaligned QString object');
      const d = ptr('0x'+new DataView(objectBuffer).getBigUint64(0,true).toString(16)); out.header = text(d);
      if (d.and(7).toUInt32() !== 0) throw new Error('unaligned QString header');
      const headerBuffer = readable(d, 26), view = new DataView(headerBuffer);
      const before = hexBytes(headerBuffer); out.headerBytes = before;
      out.refcount = view.getInt32(0,true); out.capacity = view.getInt32(4,true); out.length = view.getInt32(8,true);
      out.data = '0x'+view.getBigUint64(16,true).toString(16);
      out.storageKind = ptr(out.data).equals(d.add(26)) ? 'inline' : 'external';
      out.flags = view.getUint16(24,true);
      if (ptr(out.data).isNull()) throw new Error('null QString data');
      if (ptr(out.data).and(1).toUInt32() !== 0) throw new Error('unaligned QString data');
      if (out.refcount <= 0 || out.capacity < 0 || out.capacity < out.length || out.length < 0 || out.length > 128) {out.truncated = out.length > 128; throw new Error('invalid or excessive QString length/refcount/capacity');}
      const bytes = out.length ? new Uint8Array(readable(ptr(out.data), out.length * 2)) : new Uint8Array(0);
      if (bytes.length !== out.length * 2) throw new Error('partial content read');
      out.units = [];
      for (let i = 0; i < bytes.length; i += 2) out.units.push(bytes[i] | bytes[i + 1] << 8);
      out.text = String.fromCharCode(...out.units);
      out.utf16leHex = hexBytes(bytes.buffer);
      out.headerAfter = hexBytes(readable(d, 26));
      out.objectAfter = hexBytes(readable(object, 8));
      out.recheckStatus = out.headerAfter === before && out.objectAfter === objectBefore ? 'matched' : 'mismatch';
      if (out.recheckStatus !== 'matched') throw new Error('QString changed during snapshot');
      out.ok = true; out.rawReadStatus = 'exact_length_stable_reread';
    } catch (error) {out.units=null; out.text=null; delete out.utf16leHex; out.error = String(error).slice(0, 180); out.rawReadStatus = 'failed';}
    return out;
  }
