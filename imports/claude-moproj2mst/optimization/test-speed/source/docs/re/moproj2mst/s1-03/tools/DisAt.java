// Args: outFile addr:len ... ; disassembles linearly from addr for len bytes (disassembling as needed)
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
public class DisAt extends GhidraScript {
    public void run() throws Exception {
        String[] a = getScriptArgs();
        PrintWriter out = new PrintWriter(new FileWriter(a[0]));
        for (int i = 1; i < a.length; i++) {
            String[] p = a[i].split(":");
            Address s = toAddr(p[0]); long n = Long.parseLong(p[1], 16);
            Address e = s.add(n);
            out.println("// ===== " + s);
            Address cur = s;
            while (cur.compareTo(e) < 0) {
                Instruction ins = getInstructionAt(cur);
                if (ins == null) { new DisassembleCommand(cur, null, false).applyTo(currentProgram, monitor); ins = getInstructionAt(cur); }
                if (ins == null) { out.println(cur + "  ??"); cur = cur.add(1); continue; }
                out.println(ins.getAddress() + "  " + ins.toString());
                cur = ins.getMaxAddress().add(1);
            }
        }
        out.close();
    }
}
