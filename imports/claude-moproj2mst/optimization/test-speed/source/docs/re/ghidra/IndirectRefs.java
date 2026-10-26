// Indirect-call sweep for the batch ledger.
// Writes three files into outDir:
//   ptrtables.tsv  runs of consecutive 8-byte pointers to function entries in non-executable memory:
//                  start, count, symbol at start (vftable namespace), symbol at start-8 (RTTI locator), targets
//   addrtaken.tsv  per function, non-call references it makes to function entries (F:), to pointer
//                  runs (T:start), to defined data items holding function pointers (D:start) and to
//                  any other non-executable address (R:addr)
//   dataitems.tsv  those data items: start, length, name, function pointers inside
// Args: outDir
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class IndirectRefs extends GhidraScript {
    FunctionManager fm;
    Memory mem;

    Function fnAtPtr(Address slot) {
        try {
            long v = mem.getLong(slot);
            Address t = toAddr(v);
            if (!mem.contains(t)) return null;
            return fm.getFunctionAt(t);
        } catch (Exception e) { return null; }
    }

    String sym(Address a) {
        Symbol s = currentProgram.getSymbolTable().getPrimarySymbol(a);
        if (s == null) return "";
        return s.getName(true).replace('\t', ' ');
    }

    @Override
    public void run() throws Exception {
        String dir = getScriptArgs()[0];
        fm = currentProgram.getFunctionManager();
        mem = currentProgram.getMemory();
        ghidra.program.model.listing.Listing lst = currentProgram.getListing();

        TreeMap<Long, long[]> runs = new TreeMap<>();   // start -> {count}
        PrintWriter pt = new PrintWriter(new FileWriter(dir + "/ptrtables.tsv"));
        for (MemoryBlock b : mem.getBlocks()) {
            if (b.isExecute() || !b.isInitialized()) continue;
            Address a = b.getStart();
            long off = (8 - (a.getOffset() & 7)) & 7;
            a = a.add(off);
            Address start = null;
            List<String> tg = new ArrayList<>();
            while (a.add(7).compareTo(b.getEnd()) <= 0) {
                Function f = fnAtPtr(a);
                if (f != null) {
                    if (start == null) start = a;
                    tg.add(f.getEntryPoint().toString());
                } else if (start != null) {
                    runs.put(start.getOffset(), new long[]{tg.size()});
                    pt.println(start + "\t" + tg.size() + "\t" + sym(start) + "\t" + sym(start.subtract(8)) + "\t" + String.join(",", tg));
                    start = null;
                    tg.clear();
                }
                a = a.add(8);
            }
            if (start != null) {
                runs.put(start.getOffset(), new long[]{tg.size()});
                pt.println(start + "\t" + tg.size() + "\t" + sym(start) + "\t" + sym(start.subtract(8)) + "\t" + String.join(",", tg));
            }
        }
        pt.close();

        Map<Address, String> items = new LinkedHashMap<>();
        PrintWriter at = new PrintWriter(new FileWriter(dir + "/addrtaken.tsv"));
        ReferenceManager rm = currentProgram.getReferenceManager();
        for (Function f : fm.getFunctions(true)) {
            if (monitor.isCancelled()) break;
            LinkedHashSet<String> out = new LinkedHashSet<>();
            for (Address a : f.getBody().getAddresses(true)) {
                for (Reference r : rm.getReferencesFrom(a)) {
                    if (r.getReferenceType().isCall() || r.getReferenceType().isJump()
                            || r.getReferenceType().isFlow()) continue;
                    Address t = r.getToAddress();
                    if (!t.isMemoryAddress() || !mem.contains(t)) continue;
                    Function g = fm.getFunctionAt(t);
                    if (g != null) {
                        if (g != f) out.add("F:" + t);
                        continue;
                    }
                    MemoryBlock b = mem.getBlock(t);
                    if (b == null || b.isExecute()) continue;
                    Map.Entry<Long, long[]> e = runs.floorEntry(t.getOffset());
                    if (e != null && t.getOffset() < e.getKey() + 8 * e.getValue()[0]) {
                        out.add("T:" + toAddr(e.getKey()));
                        continue;
                    }
                    Data d = lst.getDataContaining(t);
                    if (d != null && d.getLength() > 8 && d.getLength() <= 0x40000) {
                        Address ds = d.getMinAddress();
                        if (!items.containsKey(ds)) {
                            List<String> tg = new ArrayList<>();
                            long s0 = (8 - (ds.getOffset() & 7)) & 7;
                            for (long k = s0; k + 8 <= d.getLength(); k += 8) {
                                Function h = fnAtPtr(ds.add(k));
                                if (h != null) tg.add(h.getEntryPoint().toString());
                            }
                            items.put(ds, d.getLength() + "\t" + sym(ds) + "\t" + String.join(",", tg));
                        }
                        if (!items.get(ds).endsWith("\t")) { out.add("D:" + ds); continue; }
                    }
                    out.add("R:" + t);
                }
            }
            // Indirect calls: "call [reg+disp]" gives V:disp (a vtable slot offset); "call reg" gives V:r.
            for (Instruction ins : lst.getInstructions(f.getBody(), true)) {
                ghidra.program.model.symbol.FlowType ft = ins.getFlowType();
                if (!ft.isComputed() || !(ft.isCall() || (ft.isJump() && ins.getMnemonicString().equals("JMP")
                        && ins.getOperandType(0) != 0 && rm.getReferencesFrom(ins.getAddress()).length == 0)))
                    continue;
                Object[] op = ins.getOpObjects(0);
                boolean reg = false; long disp = -1;
                for (Object o : op) {
                    if (o instanceof ghidra.program.model.lang.Register) reg = true;
                    else if (o instanceof ghidra.program.model.scalar.Scalar) disp = ((ghidra.program.model.scalar.Scalar) o).getUnsignedValue();
                }
                int ot = ins.getOperandType(0);
                boolean memop = (ot & ghidra.program.model.lang.OperandType.DYNAMIC) != 0
                        || (ot & ghidra.program.model.lang.OperandType.INDIRECT) != 0;
                if (reg && memop) out.add("V:" + Long.toHexString(disp < 0 ? 0 : disp));
                else if (reg) out.add("V:r");
            }
            if (!out.isEmpty()) at.println(f.getEntryPoint() + "\t" + String.join(",", out));
        }
        at.close();
        PrintWriter di = new PrintWriter(new FileWriter(dir + "/dataitems.tsv"));
        for (Map.Entry<Address, String> e : items.entrySet())
            if (!e.getValue().endsWith("\t")) di.println(e.getKey() + "\t" + e.getValue());
        di.close();
    }
}
