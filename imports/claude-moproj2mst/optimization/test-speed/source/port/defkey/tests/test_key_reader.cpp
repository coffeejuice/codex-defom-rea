// ctest-labels: key-common
// Tier-1 tests of the KEY interpreter loop (1401b6520) and the writer pieces (1401c5a20,
// section banner, WriteChain). Expected behaviour is read off the decompiled loop.
#include "key_test_util.hpp"

#include <defkey/key_reader.hpp>
#include <defkey/key_writer.hpp>
#include <defkey/line_io.hpp>
#include <testkit/check.hpp>

#include <map>
#include <string>
#include <vector>

using namespace def::key;
using keytest::InFile;
using keytest::OutFile;

namespace {

struct Mock : KeyReaderHooks {
    bool sub = true;
    std::vector<int> model_dims, sub_dims;
    std::vector<std::string> calls; // "group:id:rest-of-line"
    std::map<int, int> status;      // id -> status to return
    bool has_sub_model() override { return sub; }
    void set_model_dimension(int d) override { model_dims.push_back(d); }
    void set_sub_model_dimension(int d) override { sub_dims.push_back(d); }
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        char buf[kLineSize] = {};
        std::fgets(buf, kLineSize, f);
        calls.push_back(std::to_string(group) + ":" + std::to_string(id) + ":" + buf);
        auto it = status.find(id);
        return it == status.end() ? 0 : it->second;
    }
};

} // namespace

TEST_CASE(dispatch_and_comments)
{
    InFile in("t_kr_1.txt", "*\n*  DEFORM-3D V11.0.2 KEYWORD FILE (Qt)\n*\nTITLE\nNSTEP 10\n\n  CNTACT 1 2\n");
    OutFile out("t_kr_1o.txt");
    Mock m;
    CHECK_EQ(read_key_stream(in.get(), m, out.get()), 0);
    REQUIRE(m.calls.size() == 3u);
    CHECK_EQ(m.calls[0], std::string("1:22:\n"));    // the handler starts on the rest of the line
    CHECK_EQ(m.calls[1], std::string("1:40: 10\n"));
    CHECK_EQ(m.calls[2], std::string("3:271: 1 2\n"));
    REQUIRE(m.model_dims.size() == 1u); // only the first matching comment sets the dimension
    CHECK_EQ(m.model_dims[0], 3);
    CHECK_EQ(m.sub_dims[0], 2);
    CHECK_EQ(out.text(), std::string(""));
}

TEST_CASE(dimension_forms)
{
    struct Case {
        const char* text;
        int model;
        int sub;
    };
    const Case cases[] = {{"* DEFORM 2D\n", 2, 3},       {"* DEFORM F3\n", 3, 2},
                          {"*DEFORM-F2 x\n", -1, -1},     // token "*DEFORM-F2": the rest is " x"
                          {"* DEFORM-MACH2\n", 2, 3},     {"* DEFORM-MICROSTRUCTURE2 V\n", 2, 3},
                          {"* DEFORM-4D\n", -1, -1},      {"* deform-3d\n", -1, -1}};
    int k = 0;
    for (const Case& c : cases) {
        InFile in(("t_kr_d" + std::to_string(k++) + ".txt").c_str(), c.text);
        Mock m;
        m.sub = false;
        CHECK_EQ(read_key_stream(in.get(), m, stdout), 0);
        if (c.model < 0) {
            CHECK(m.model_dims.empty());
        } else {
            REQUIRE(m.model_dims.size() == 1u);
            CHECK_EQ(m.model_dims[0], c.model);
        }
        CHECK(m.sub_dims.empty()); // no sub-model: not called
    }
}

TEST_CASE(errors_print_and_continue)
{
    InFile in("t_kr_2.txt", "TITLE\nNSTEP 1\nBOGUS rest\nSRFNRG 5\nCNTACT 1 2\nTITLE\n");
    OutFile out("t_kr_2o.txt");
    Mock m;
    m.status[40] = 2;  // NSTEP: Reading Error
    m.status[271] = 3; // CNTACT: Data Error
    // Last non-zero status is returned at EOF, after the whole file was read.
    CHECK_EQ(read_key_stream(in.get(), m, out.get()), 3);
    CHECK_EQ(m.calls.size(), 4u);
    CHECK_EQ(out.text(), std::string("ERROR: Keyword NSTEP Reading Error. \n"
                                     "WARNING: Keyword BOGUS not found. Skip  rest\n\n"
                                     "ERROR: Keyword CNTACT Data Error. \n"));
}

TEST_CASE(other_status_stops)
{
    InFile in("t_kr_3.txt", "TITLE\nNSTEP 1\nTITLE\n");
    Mock m;
    m.status[22] = 7;
    CHECK_EQ(read_key_stream(in.get(), m, stdout), 7);
    CHECK_EQ(m.calls.size(), 1u);
}

TEST_CASE(long_token_is_split)
{
    // "%39s" reads 39 characters; the remainder is the next token.
    const std::string name(45, 'A');
    InFile in("t_kr_4.txt", name + "\n");
    OutFile out("t_kr_4o.txt");
    Mock m;
    CHECK_EQ(read_key_stream(in.get(), m, out.get()), 0);
    CHECK_EQ(out.text(), "WARNING: Keyword " + std::string(39, 'A') + " not found. Skip " + std::string(6, 'A') +
                             "\n\n");
}

TEST_CASE(banners_and_chain)
{
    OutFile out("t_kw_1.txt");
    write_file_banner(out.get(), 0, "DEFORM-3D", "11.0.2");
    write_file_banner(out.get(), 2, "ignored", "11.0.2");
    WriteChain w(out.get());
    int ran = 0;
    w.section("Process Definition");
    w.step([&] { ++ran; std::fprintf(out.get(), "%-6s\n", keyword_label(22)); return 0; });
    w.step([&] { ++ran; return 4; });
    w.section("Not written");
    w.step([&] { ++ran; return 0; });
    CHECK_EQ(w.status(), 4);
    CHECK_EQ(ran, 2);
    CHECK_EQ(out.text(), std::string("*\n*  DEFORM-3D V11.0.2 KEYWORD FILE (Qt)\n*\n"
                                     "*\n*  DEFORM-2D V11.0.2 KEYWORD FILE (Qt)\n*\n"
                                     "*\n*  Process Definition\n*\nTITLE \n"));
}

int main() { return testkit::run_all(); }
