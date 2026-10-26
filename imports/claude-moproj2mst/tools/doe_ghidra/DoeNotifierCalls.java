// Bound instruction verification for one notifier registration entry point.
//@category DOE.StaticEvidence
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;

public class DoeNotifierCalls extends GhidraScript {
    private Address addr(String s) { return toAddr(s.startsWith("0x") ? s.substring(2) : s); }
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("output and candidate input required");
        if (!"fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc".equals(currentProgram.getExecutableSHA256()))
            throw new IllegalStateException("original executable identity mismatch");
        JsonObject input = JsonParser.parseString(Files.readString(Path.of(args[1]))).getAsJsonObject();
        JsonArray calls = input.getAsJsonArray("registration_direct_call_candidates");
        Map<String, JsonObject> regions = new LinkedHashMap<>();
        for (JsonElement e : calls) {
            JsonObject r = e.getAsJsonObject().getAsJsonObject("unwind_region");
            if (r != null) regions.put(r.get("begin").getAsString(), r);
        }
        Map<String, Object> output = new LinkedHashMap<>();
        output.put("schema", "doe-notifier-instruction-verification-v1");
        output.put("executable_sha256", currentProgram.getExecutableSHA256());
        output.put("qualification", "Private in-memory disassembly seeded only at supplied Windows unwind regions. Changes rolled back; headless readOnly prevents persistence. This is not whole-program dynamic registration proof.");
        List<Object> decoded = new ArrayList<>();
        List<Object> seeds = new ArrayList<>();
        int transaction = currentProgram.startTransaction("bounded notifier caller verification");
        try {
            for (JsonObject r : regions.values()) {
                Address begin = addr(r.get("begin").getAsString());
                Address end = addr(r.get("end_exclusive").getAsString()).subtract(1);
                DisassembleCommand command = new DisassembleCommand(begin, new AddressSet(begin, end), true);
                command.enableCodeAnalysis(false);
                boolean succeeded = command.applyTo(currentProgram, monitor);
                Map<String, Object> seed = new LinkedHashMap<>();
                seed.put("begin", begin.toString());
                seed.put("end_inclusive", end.toString());
                seed.put("command_succeeded", succeeded);
                seed.put("status", command.getStatusMsg());
                seeds.add(seed);
            }
            for (JsonElement e : calls) {
                JsonObject candidate = e.getAsJsonObject();
                Address call = addr(candidate.get("candidate_site").getAsString());
                Instruction instruction = getInstructionAt(call);
                Map<String, Object> record = new LinkedHashMap<>();
                record.put("site", call.toString());
                record.put("instruction", instruction == null ? null : instruction.toString());
                boolean verified = false;
                if (instruction != null && instruction.getFlowType().isCall()) {
                    for (Address flow : instruction.getFlows())
                        if (flow.equals(toAddr("1402cfbe0"))) verified = true;
                }
                record.put("verified_registration_call", verified);
                Instruction first = getInstructionAt(call.subtract(14));
                Instruction second = getInstructionAt(call.subtract(7));
                boolean shape = first != null && second != null && first.getLength() == 7 && second.getLength() == 7 &&
                    "LEA".equalsIgnoreCase(first.getMnemonicString()) && "LEA".equalsIgnoreCase(second.getMnemonicString()) &&
                    "RDX".equalsIgnoreCase(first.getDefaultOperandRepresentation(0)) &&
                    "RCX".equalsIgnoreCase(second.getDefaultOperandRepresentation(0));
                record.put("adjacent_rdx_rcx_lea_shape", shape);
                if (shape) {
                    record.put("rdx_instruction", first.toString());
                    record.put("rcx_instruction", second.toString());
                }
                decoded.add(record);
            }
            List<Object> refs = new ArrayList<>();
            for (Reference ref : getReferencesTo(toAddr("1402cfbe0"))) {
                Map<String,Object> row = new LinkedHashMap<>();
                row.put("from", ref.getFromAddress().toString());
                row.put("type", ref.getReferenceType().toString());
                refs.add(row);
            }
            output.put("references_after_bounded_disassembly", refs);
            output.put("regions", seeds);
            output.put("calls", decoded);
            Files.writeString(Path.of(args[0]), new GsonBuilder().setPrettyPrinting().create().toJson(output)+"\n");
        } finally {
            currentProgram.endTransaction(transaction, false);
        }
        println("DOE_NOTIFIER_CALLS " + decoded.size());
    }
}
