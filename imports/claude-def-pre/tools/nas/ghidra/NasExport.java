// Exports the current program for the NAS archive (docs/storage-plan.md 6.1): a .gzf and a JSON file of what people
// and scripts added (functions, non-default symbols, comments, bookmarks, data type names).
// Headless: analyzeHeadless <projdir> <project> -process <program> -noanalysis -readOnly
//           -scriptPath tools/nas/ghidra -postScript NasExport.java <outDir> [nogzf]
// @category NAS
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Iterator;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataType;
import ghidra.program.model.listing.Bookmark;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;

public class NasExport extends GhidraScript {

    private static String q(String s) {
        if (s == null) return "null";
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            switch (c) {
                case '"': b.append("\\\""); break;
                case '\\': b.append("\\\\"); break;
                case '\n': b.append("\\n"); break;
                case '\r': b.append("\\r"); break;
                case '\t': b.append("\\t"); break;
                default:
                    if (c < 0x20) b.append(String.format("\\u%04x", (int) c));
                    else b.append(c);
            }
        }
        return b.append('"').toString();
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File out = new File(args[0]);
        out.mkdirs();
        boolean gzf = !(args.length > 1 && args[1].equals("nogzf"));
        String name = currentProgram.getName();

        if (gzf) {
            File f = new File(out, name + ".gzf");
            // the packed form of the project file: what Ghidra's own "Export > Ghidra Zip File" writes
            currentProgram.getDomainFile().packFile(f, monitor);
            if (!f.isFile()) throw new RuntimeException("no gzf written: " + f);
            println("gzf " + f + " " + f.length());
        }

        File jf = new File(out, name + ".annotations.json");
        try (PrintWriter w = new PrintWriter(new FileWriter(jf, java.nio.charset.StandardCharsets.UTF_8))) {
            w.println("{");
            w.println(" \"program\": " + q(name) + ",");
            w.println(" \"sha256\": " + q(currentProgram.getExecutableSHA256()) + ",");
            w.println(" \"image_base\": " + q(currentProgram.getImageBase().toString()) + ",");
            w.println(" \"project\": " + q(state.getProject().getName()) + ",");

            w.println(" \"functions\": [");
            boolean first = true;
            for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
                if (!first) w.println(",");
                first = false;
                w.print("  {\"entry\": " + q(fn.getEntryPoint().toString()) + ", \"name\": " + q(fn.getName(true)) +
                        ", \"source\": " + q(fn.getSymbol().getSource().toString()) +
                        ", \"signature\": " + q(fn.getSignature().getPrototypeString()) +
                        ", \"body\": " + q(fn.getBody().getMinAddress() + "-" + fn.getBody().getMaxAddress()) + "}");
            }
            w.println("\n ],");

            w.println(" \"symbols\": [");
            first = true;
            Iterator<Symbol> it = currentProgram.getSymbolTable().getAllSymbols(true);
            while (it.hasNext()) {
                Symbol s = it.next();
                if (s.getSource() == SourceType.DEFAULT || s.isExternal()) continue;
                if (!first) w.println(",");
                first = false;
                w.print("  {\"address\": " + q(s.getAddress().toString()) + ", \"name\": " + q(s.getName(true)) +
                        ", \"type\": " + q(s.getSymbolType().toString()) + ", \"source\": " + q(s.getSource().toString()) +
                        "}");
            }
            w.println("\n ],");

            w.println(" \"comments\": [");
            first = true;
            Listing l = currentProgram.getListing();
            int[] kinds = { CodeUnit.EOL_COMMENT, CodeUnit.PRE_COMMENT, CodeUnit.POST_COMMENT, CodeUnit.PLATE_COMMENT,
                            CodeUnit.REPEATABLE_COMMENT };
            String[] names = { "eol", "pre", "post", "plate", "repeatable" };
            for (int k = 0; k < kinds.length; k++) {
                Iterator<Address> ai = l.getCommentAddressIterator(kinds[k], currentProgram.getMemory(), true);
                while (ai.hasNext()) {
                    Address a = ai.next();
                    if (!first) w.println(",");
                    first = false;
                    w.print("  {\"address\": " + q(a.toString()) + ", \"kind\": " + q(names[k]) + ", \"text\": " +
                            q(l.getComment(kinds[k], a)) + "}");
                }
            }
            w.println("\n ],");

            w.println(" \"bookmarks\": [");
            first = true;
            Iterator<Bookmark> bi = currentProgram.getBookmarkManager().getBookmarksIterator();
            while (bi.hasNext()) {
                Bookmark b = bi.next();
                if (b.getTypeString().equals("Analysis") || b.getTypeString().equals("Info")) continue;
                if (!first) w.println(",");
                first = false;
                w.print("  {\"address\": " + q(b.getAddress().toString()) + ", \"type\": " + q(b.getTypeString()) +
                        ", \"category\": " + q(b.getCategory()) + ", \"comment\": " + q(b.getComment()) + "}");
            }
            w.println("\n ],");

            w.println(" \"data_types\": [");
            first = true;
            Iterator<DataType> di = currentProgram.getDataTypeManager().getAllDataTypes();
            while (di.hasNext()) {
                DataType d = di.next();
                if (!first) w.println(",");
                first = false;
                w.print("  {\"path\": " + q(d.getPathName()) + ", \"length\": " + d.getLength() + "}");
            }
            w.println("\n ]");
            w.println("}");
        }
        println("annotations " + jf);
    }
}
