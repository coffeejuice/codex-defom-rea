//@category DOE.StaticEvidence
import ghidra.app.script.GhidraScript;
import com.google.gson.GsonBuilder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.*;

public class DoeTextIdentity extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (!"fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc".equals(currentProgram.getExecutableSHA256()))
            throw new IllegalStateException("target mismatch");
        // Caller selects file-backed PE executable ranges explicitly. No function-body inference.
        List<Object> ranges = new ArrayList<>();
        for (int i=1; i<args.length; i+=2) {
            int length = Integer.parseInt(args[i+1]);
            if (length <= 0 || length > 128*1024*1024) throw new IllegalArgumentException("length bound");
            byte[] data = new byte[length];
            if (currentProgram.getMemory().getBytes(toAddr(args[i]), data) != length)
                throw new IllegalStateException("incomplete read");
            Map<String,Object> r = new LinkedHashMap<>();
            r.put("start", args[i]);
            r.put("length", length);
            r.put("sha256", HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(data)));
            ranges.add(r);
        }
        Map<String,Object> result = new LinkedHashMap<>();
        result.put("schema", "wave2-doe-ghidra-text-identity-v1");
        result.put("executable_sha256", currentProgram.getExecutableSHA256());
        result.put("ranges", ranges);
        result.put("qualification", "Explicit memory ranges; verify against original PE bytes. Imported function address sets may contain only the entry byte and are not complete decompilation coverage.");
        Files.writeString(Path.of(args[0]), new GsonBuilder().setPrettyPrinting().create().toJson(result)+"\n", StandardCharsets.UTF_8);
        println("DOE_TEXT_IDENTITY ranges="+ranges.size());
    }
}
