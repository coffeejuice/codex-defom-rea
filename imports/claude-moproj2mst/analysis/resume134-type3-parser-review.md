# CP134 Type3 `.OPT` parser boundary review

CP132–CP133 close the line delimiter for a supplied BMP `QString`, and CP130 distinguishes ENDDATA as a terminal tokenized result. Together with the scalar dispatcher, these support a small in-memory sequence seam over already separated lines. They do not yet support enabling the QFile/QTextStream `.OPT` parser or ShapeOptim Type3 loading.

## Established native sequence

The retained original parser body is bound to `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC` in `analysis/resume127-type3-opt-parser.asm` and summarized in `resume128-type3-constructor-parser.md`:

1. A null optimizer returns false before QFile construction. Otherwise QFile is opened; open failure reports the retained failure text and returns false without optimizer mutation.
2. After a successful open, the parser writes the DB_PURGE/purge property true, constructs QTextStream, reads one line separately, trims it and converts it to an otherwise unused integer.
3. It then loops while `!atEnd()`, reads a line, splits with the static QRegExp `\\s+` and `SkipEmptyParts`, and dispatches keys in file order.
4. Scalar fields are written as encountered. ENDDATA terminates the line loop and reaches common close/cleanup at `0x140909400`. The return local is zero on observed paths, so the parser returns false even after successful open and mutations; its caller stores but does not branch on that return.
5. NUM_CON/REGIONS append to the optimizer’s shared-data row members immediately. EOF or later termination does not roll back prior scalar writes or appends.

CP132 recovers the regex construction/options and split mode. CP133’s pure tokenizer matches the full retained BMP whitespace set and SkipEmptyParts. CP130’s `Terminate` outcome represents ENDDATA without field writes. These are compatible prefixes, not a file parser.

## Smallest safe integrated prefix

The smallest useful next integration is a private in-memory line-sequence driver that accepts an ordered collection of already supplied `qt::String` lines, tokenizes each with the CP133 helper, applies only supported scalar/skip keys with the CP129 dispatcher, stops on `Terminate`, and returns an explicit guarded outcome on `RowBodyNotImplemented` or surrogate input. It must preserve earlier scalar mutations when a later row key or unsupported line is reached, matching the native nontransactional order. Empty token vectors should remain no-ops. Tests should cover scalar-overwrite order, skipped/unknown lines, ENDDATA stopping before following lines, and a guarded row after an earlier scalar mutation.

This would integrate only the in-memory tokenization and scalar-prefix pieces. It must not call itself a file parser, claim native parse success/status, construct a Type3 owner, or route public ShapeOptim/Type3 input. The source return-false behavior belongs to the native file parser boundary and should not be inferred from an in-memory helper return enum.

## Missing evidence before file-level integration

A source-equivalent file parser still needs a bounded implementation/qualification of QFile open modes and errors, QTextStream device/codec behavior, `readLine`/`atEnd` ordering (including empty file and terminal newline), the separate first-line trim/toInt step, read errors/close and cleanup outcomes, and exact token comparison/admission. The tokenizer refuses surrogate-containing lines; this is an explicit candidate safety boundary, not recovered native rejection.

More importantly, row-body paths are not closed. Although CP131 projects successful prepared append calls and CP132 maps the classifier, the parser’s QString derivation/formatting, malformed or short row behavior, native shared-data allocation/copy/exception cleanup, and `+0x18/+0x20` member ownership remain unknown. The global QRegExp lifetime and initializer are now known but do not solve those row contracts. Earlier successful scalar changes/rows persist if a later read or row fails, so a future parser cannot preflight the whole file transactionally without changing native ordering.

No discrepancy found among the recovered regex, tokenizer, scalar dispatcher, and ENDDATA control cuts. No source/build/test/GUI/original-process/REA action was performed in this review. Public Type3 and ShapeOptim activation should remain guarded pending the file and row boundaries above.
