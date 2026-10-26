#include <defrt/stdlib/cli_helpers.hpp>

#include <defrt/stdlib/crt_emulation.hpp>

#include <cstring>

namespace def::rt {

// @src 14001b4e0  -h help screen (each line is one fprintf to __iob_func() + 0x30)
void print_help()
{
    print_help(stdout);
}

// part of @src 14001b4e0
void print_help(std::FILE* f)
{
    std::fprintf(f, "\n");
    std::fprintf(f, "*** DEF_PRE.EXE - help ***\n");
    std::fprintf(f, "\n");
    std::fprintf(f, "\tUsage\n");
    std::fprintf(f, "\t  o. Interactive mode\n");
    std::fprintf(f, "\t        DEF_PRE.EXE\n");
    std::fprintf(f, "\t        - OR -  \n");
    std::fprintf(f, "\t  o. Batch mode\n");
    std::fprintf(f, "\t        DEF_PRE.EXE [-h]\n");
    std::fprintf(f, "\t                    [-m {header|stepList|lastStepNo|simStepList}]\n");
    std::fprintf(f, "\t                    [-d <db name>]\n");
    std::fprintf(f, "\t                    [-o <file name>]\n");
    std::fprintf(f, "\n");
    std::fprintf(f, "\tExample\n");
    std::fprintf(f, "\t  o. This help screen\n");
    std::fprintf(f, "\t        DEF_PRE.EXE -h\n");
    std::fprintf(f, "\t  o. Get header information\n");
    std::fprintf(f, "\t        DEF_PRE.EXE -m header -d a.db -o header.txt\n");
    std::fprintf(f, "\t  o. Get step number list\n");
    std::fprintf(f, "\t        DEF_PRE.EXE -m stepList -d a.db -o stepList.txt\n");
    std::fprintf(f, "\t  o. Get the last step number\n");
    std::fprintf(f, "\t        DEF_PRE.EXE -m lastStepNo -d a.db -o lastStepNo.txt\n");
    std::fprintf(f, "\t  o. Get the simulation step number list\n");
    std::fprintf(f, "\t        DEF_PRE.EXE -m simStepList -d a.db -o simStepList.txt\n");
    std::fprintf(f, "\n");
}

// @src 14001b700  strip one leading and one trailing double quote
void strip_quotes(char* s)
{
    char buf[1024]; // RSP+0x20 (acStack_419 + 1) up to the cookie at RSP+0x420
    const std::size_t len = std::strlen(s);
    if (len >= sizeof buf) {
        // 14001b712: the copy loop writes len + 1 bytes and reaches the security cookie;
        // __security_check_cookie at return fails.
        abnormal_exit(kStatusStackBufferOverrun);
    }
    std::memcpy(buf, s, len + 1);
    int end = static_cast<int>(len);
    if (buf[len - 1] == '"') {
        end = static_cast<int>(len) - 1;
    }
    const std::int64_t start = buf[0] == '"' ? 1 : 0;
    const std::int64_t stop = end;
    if (start < stop) {
        std::memcpy(s, buf + start, static_cast<std::size_t>(stop - start));
    }
    s[stop] = '\0';
}

// @src 14001c090  split a string with strtok into a pointer table
void split_tokens(char* s, const char* delim, char** table, std::int32_t* count)
{
    *count = 0;
    table[*count] = std::strtok(s, delim);
    while (table[*count] != nullptr) {
        *count = *count + 1;
        table[*count] = std::strtok(nullptr, delim);
    }
}

// @src 14001bfe0  first and last stored step of an operation
bool operation_steps(const std::int32_t* op, const void* records, const std::int32_t* nrec,
                     std::int32_t* dim, std::int32_t* first_step, std::int32_t* last_step)
{
    const auto field = [records](std::uint64_t off) {
        std::int32_t v;
        std::memcpy(&v, static_cast<const char*>(records) + off, sizeof v);
        return v;
    };
    bool first = true;
    std::uint64_t off = 0;
    for (std::int32_t i = 0; i < *nrec; ++i, off += kDirRecordSize) {
        if (field(off + 0x24) != 0 && field(off + 0x14) == *op) {
            if (first) {
                first = false;
                *dim = field(off + 0x1c);
                *first_step = field(off + 0x04);
            } else {
                *last_step = field(off + 0x04);
            }
        }
    }
    return !first;
}

} // namespace def::rt
