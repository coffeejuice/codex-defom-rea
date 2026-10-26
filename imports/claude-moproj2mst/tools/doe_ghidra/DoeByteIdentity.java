//@category DOE.StaticEvidence
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressRangeIterator;
import ghidra.program.model.listing.Function;
import com.google.gson.GsonBuilder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.*;

public class DoeByteIdentity extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (!"fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc".equals(currentProgram.getExecutableSHA256()))
            throw new IllegalStateException("target mismatch");
        Map<String,Object> result = new LinkedHashMap<>();
        result.put("schema", "wave2-doe-ghidra-memory-identity-v1");
        result.put("executable_sha256", currentProgram.getExecutableSHA256());
        List<Object> functions = new ArrayList<>();
        for (int i=1; i<args.length; i++) {
            Function function = getFunctionAt(toAddr(args[i]));
            if (function == null) throw new IllegalStateException("missing function " + args[i]);
            Map<String,Object> f = new LinkedHashMap<>();
            f.put("entry", function.getEntryPoint().toString());
            List<Object> ranges = new ArrayList<>();
            AddressRangeIterator iterator = function.getBody().getAddressRanges();
            while (iterator.hasNext()) {
                AddressRange range = iterator.next();
                long length = range.getLength();
                if (length > 8*1024*1024) throw new IllegalStateException("range too large");
                byte[] data = new byte[(int)length];
                if (currentProgram.getMemory().getBytes(range.getMinAddress(), data) != length)
                    throw new IllegalStateException("incomplete memory read");
                Map<String,Object> r = new LinkedHashMap<>();
                r.put("start", range.getMinAddress().toString());
                r.put("length", length);
                r.put("sha256", HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(data)));
                ranges.add(r);
            }
            f.put("ranges", ranges);
            functions.add(f);
        }
        result.put("functions", functions);
        result.put("qualification", "Function body ranges from a private Ghidra snapshot; compare each digest with explicit original PE file bytes. Matching code bytes does not prove inferred types or decompilation semantics.");
        Files.writeString(Path.of(args[0]), new GsonBuilder().setPrettyPrinting().create().toJson(result)+"\n", StandardCharsets.UTF_8);
        println("DOE_BYTE_IDENTITY functions=" + functions.size());
    }
}
