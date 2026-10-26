// defcmp: command line for the output comparator. See harness/defcmp/README.md.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "defcmp/defcmp.hpp"

using namespace defcmp;

namespace {

int usage() {
    std::fputs(
        "usage:\n"
        "  defcmp compare [options] EXPECTED ACTUAL   compare two files\n"
        "  defcmp dirs    [options] EXPECTED ACTUAL   compare two output directories\n"
        "  defcmp oracle  [options] ENTRY ACTUAL      compare an oracle cache entry with a port run\n"
        "  defcmp hash    [options] FILE...           print masked/normalised digests\n"
        "  defcmp masks   [--mask SPEC] FILE.DB       list the masked byte ranges\n"
        "options:\n"
        "  --kind key|db|transcript|bytes   default: from the extension\n"
        "  --mask SPEC          DB masks, default \"default\" (smi3,smi4,istart);\n"
        "                       e.g. +header-only, all,-smi2, none, +bytes:OFF:LEN\n"
        "  --expected-digest H  digest of the expected file (from 'hash'); the expected\n"
        "                       file is then only read when the digests differ\n"
        "  --root DIR=NAME      transcript: replace this directory prefix by NAME\n"
        "  --mask-regex RE      transcript: also mask matches of RE\n"
        "  --keep-dates, --keep-times, --keep-paths   transcript: switch a mask off\n"
        "exit code: 0 equal, 1 different, 2 error\n",
        stderr);
    return 2;
}

int print_rows(const std::vector<DirEntryResult>& rows) {
    int bad = 0, err = 0, skipped = 0;
    for (const auto& d : rows) {
        const char* tag = "ok";
        if (!d.result.error.empty()) tag = "ERR", ++err;
        else if (d.skipped) tag = "SKIP", ++skipped;
        else if (!d.ok()) tag = "DIFF", ++bad;
        std::printf("%-4s %-10s %s\n", tag, kind_name(d.kind), d.name.c_str());
        if (!d.ok()) std::printf("     %s\n", d.skipped ? d.result.where.c_str() : d.result.summary().c_str());
    }
    std::printf("%zu rows, %d different, %d skipped, %d errors\n", rows.size(), bad, skipped, err);
    return err ? 2 : (bad ? 1 : 0);
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) return usage();
    const std::string cmd = argv[1];
    Options opt;
    std::optional<Kind> kind;
    std::optional<Digest> exp_digest;
    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i) {
        const std::string a = argv[i];
        auto value = [&]() -> const char* {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "missing value for %s\n", a.c_str());
                std::exit(2);
            }
            return argv[++i];
        };
        if (a == "--kind") {
            kind = parse_kind(value());
            if (!kind) return usage();
        } else if (a == "--mask") {
            std::string err;
            auto m = DbMasks::parse(value(), &err);
            if (!m) {
                std::fprintf(stderr, "%s\n", err.c_str());
                return 2;
            }
            opt.db = *m;
        } else if (a == "--expected-digest") {
            exp_digest = parse_digest(value());
            if (!exp_digest) return usage();
        } else if (a == "--root") {
            const std::string v = value();
            const auto eq = v.rfind('=');
            if (eq == std::string::npos) return usage();
            opt.transcript.roots.emplace_back(v.substr(0, eq), v.substr(eq + 1));
        } else if (a == "--mask-regex") {
            try {
                opt.transcript.extra.emplace_back(value());
            } catch (const std::regex_error& e) {
                std::fprintf(stderr, "bad regex: %s\n", e.what());
                return 2;
            }
        } else if (a == "--keep-dates") opt.transcript.dates = false;
        else if (a == "--keep-times") opt.transcript.times = false;
        else if (a == "--keep-paths") opt.transcript.paths = false;
        else if (a.size() > 2 && a[0] == '-' && a[1] == '-') return usage();
        else args.push_back(a);
    }

    try {
        if (cmd == "compare") {
            if (args.size() != 2) return usage();
            const Kind k = kind ? *kind : kind_from_path(args[0]);
            const Result r = compare_files(k, args[0], args[1], opt, exp_digest);
            std::printf("%s: %s\n", kind_name(k), r.summary().c_str());
            return !r.error.empty() ? 2 : (r.equal ? 0 : 1);
        }
        if (cmd == "dirs" || cmd == "oracle") {
            if (args.size() != 2) return usage();
            const auto rows = cmd == "dirs" ? compare_dirs(args[0], args[1], opt) : compare_oracle_entry(args[0], args[1], opt);
            return print_rows(rows);
        }
        if (cmd == "hash") {
            if (args.empty()) return usage();
            for (const auto& f : args) {
                const Kind k = kind ? *kind : kind_from_path(f);
                const std::string masks = k == Kind::Db ? opt.db.canonical() : "-";
                std::printf("%s  %-10s %s  %s\n", digest_hex(digest_file(k, f, opt)).c_str(), kind_name(k), masks.c_str(), f.c_str());
            }
            return 0;
        }
        if (cmd == "masks") {
            if (args.size() != 1) return usage();
            for (const auto& r : db_mask_ranges(args[0], opt.db))
                std::printf("%12llu %4llu  (record %llu byte %llu)\n", static_cast<unsigned long long>(r.offset),
                            static_cast<unsigned long long>(r.length), static_cast<unsigned long long>(r.offset / 512 + 1),
                            static_cast<unsigned long long>(r.offset % 512));
            return 0;
        }
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 2;
    }
    return usage();
}
