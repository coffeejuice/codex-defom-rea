// Bounded standalone export from a read-only, privately copied Ghidra project.
//@category DOE.StaticEvidence
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import com.google.gson.GsonBuilder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class DoeExport extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) throw new IllegalArgumentException("output and addresses required");
        if (!"fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc".equals(currentProgram.getExecutableSHA256()))
            throw new IllegalStateException("original executable identity mismatch");
        Map<String,Object> result = new LinkedHashMap<>();
        result.put("schema", "wave2-doe-standalone-decompile-v1");
        result.put("program", currentProgram.getName());
        result.put("executable_sha256", currentProgram.getExecutableSHA256());
        result.put("qualification", "Read-only private snapshot. Static decompilation and database references; no original runtime or shared REA session. Indirect calls may remain unresolved.");
        List<Object> records = new ArrayList<>();
        DecompInterface decompiler = new DecompInterface();
        if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
        try {
            for (int i=1; i<args.length; i++) {
                Address address = toAddr(args[i]);
                Function function = getFunctionAt(address);
                Map<String,Object> record = new LinkedHashMap<>();
                record.put("requested_address", args[i]);
                if (function == null) {
                    record.put("status", "no-function-at-address");
                } else {
                    record.put("entry", function.getEntryPoint().toString());
                    record.put("name", function.getName());
                    DecompileResults dc = decompiler.decompileFunction(function, 90, monitor);
                    record.put("completed", dc.decompileCompleted());
                    record.put("error", dc.getErrorMessage());
                    if (dc.getDecompiledFunction() != null) record.put("c", dc.getDecompiledFunction().getC());
                    List<Object> refs = new ArrayList<>();
                    for (Reference ref : getReferencesTo(address)) {
                        Map<String,Object> item = new LinkedHashMap<>();
                        item.put("from", ref.getFromAddress().toString());
                        item.put("type", ref.getReferenceType().toString());
                        Function owner = getFunctionContaining(ref.getFromAddress());
                        item.put("owner", owner == null ? null : owner.getEntryPoint().toString());
                        refs.add(item);
                    }
                    record.put("references_to_entry", refs);
                    List<String> called = new ArrayList<>();
                    for (Function f : function.getCalledFunctions(monitor)) called.add(f.getEntryPoint().toString());
                    Collections.sort(called);
                    record.put("known_callees", called);
                }
                records.add(record);
                println("DOE_EXPORT " + args[i] + " " + record.getOrDefault("completed", false));
            }
        } finally {
            decompiler.dispose();
        }
        result.put("functions", records);
        Files.writeString(Path.of(args[0]), new GsonBuilder().setPrettyPrinting().create().toJson(result)+"\n", StandardCharsets.UTF_8);
    }
}
