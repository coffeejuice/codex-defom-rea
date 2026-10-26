// Lists every reference into [start,end) with the referencing function and the offset into the range.
// Args: outFile startHex endHex [recSize]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class RangeRefs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        PrintWriter out = new PrintWriter(new FileWriter(a[0]));
        Address s = toAddr(a[1]), e = toAddr(a[2]);
        long rec = a.length > 3 ? Long.parseLong(a[3]) : 0;
        ReferenceManager rm = currentProgram.getReferenceManager();
        for (Address x = s; x.compareTo(e) < 0; x = x.add(1)) {
            for (Reference r : rm.getReferencesTo(x)) {
                Address from = r.getFromAddress();
                Function f = getFunctionContaining(from);
                long d = x.subtract(s);
                out.println(x + "\t" + (rec > 0 ? (d / rec) + "+0x" + Long.toHexString(d % rec) : "") + "\t" + from + "\t"
                        + (f == null ? "-" : f.getName() + "@" + f.getEntryPoint()) + "\t" + r.getReferenceType());
            }
        }
        out.close();
    }
}
