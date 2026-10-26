import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import java.nio.file.*;

// Read-only bounded instruction evidence for known entries/call sites.
public class MoCallsiteFocus extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();
  StringBuilder out=new StringBuilder("SOURCE: "+currentProgram.getExecutablePath()+"\nPROFILE: no auto-analysis; disassemble known function entry\n");
  for(int i=1;i+2<args.length;i+=3){
   Address entry=toAddr(args[i]),start=toAddr(args[i+1]),end=toAddr(args[i+2]);
   disassemble(entry);
   out.append("\nENTRY ").append(entry).append(" RANGE ").append(start).append("..").append(end).append("\n");
   InstructionIterator it=currentProgram.getListing().getInstructions(start,true);
   int count=0;
   while(it.hasNext()&&count++<100){
    Instruction ins=it.next();if(ins.getAddress().compareTo(end)>0)break;
    out.append(ins.getAddress()).append(" ");
    for(byte b:ins.getBytes())out.append(String.format("%02x",b&255));
    out.append(" ").append(ins).append("\n");
   }
  }
  Files.writeString(Path.of(args[0]),out.toString());
 }
}
