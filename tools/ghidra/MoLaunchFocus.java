import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.nio.file.*;

// Read-only binary import, no whole-program analyzers. Saves bounded selected routines.
public class MoLaunchFocus extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();
  DecompInterface di=new DecompInterface();di.openProgram(currentProgram);
  StringBuilder out=new StringBuilder("SOURCE: "+currentProgram.getExecutablePath()+"\nPROFILE: no auto-analysis; manually disassembled entry points; names/types inferred\n");
  for(int i=1;i<args.length;i++){
   Address address=toAddr(args[i]);disassemble(address);
   Function f=getFunctionAt(address);if(f==null)f=createFunction(address,null);
   out.append("\nADDRESS ").append(args[i]).append("\n");
   if(f==null){out.append("NO FUNCTION\n");continue;}
   DecompileResults result=di.decompileFunction(f,45,monitor);
   out.append(result.decompileCompleted()?result.getDecompiledFunction().getC():result.getErrorMessage()).append("\n");
  }
  di.dispose();Files.writeString(Path.of(args[0]),out.toString());
 }
}
