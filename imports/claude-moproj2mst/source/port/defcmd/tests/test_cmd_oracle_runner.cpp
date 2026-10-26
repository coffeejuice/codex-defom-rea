// ctest-labels: p67-1 runner
// cmd_oracle_runner.hpp: JSON string decoding and the inputs of a cached case (any number of
// entries, source files or inline text as tools/oracle/def_pre_oracle.py stores them).
#include "cmd_oracle_runner.hpp"

#include <testkit/check.hpp>

TEST_CASE(json_strings_are_decoded)
{
    const std::string j = R"({"label": "A\/B", "text": "KFREAD 0\nX.KEY\r\n\t\"q\" \\ \u00e9"})";
    CHECK_EQ(cmdoracle::json_string(j, "label"), std::string("A/B"));
    CHECK_EQ(cmdoracle::json_string(j, "text"), std::string("KFREAD 0\nX.KEY\r\n\t\"q\" \\ \xe9"));
    CHECK_EQ(cmdoracle::json_string(j, "missing"), std::string());
}

TEST_CASE(case_inputs_take_any_number_of_files_and_inline_texts)
{
    const std::string j = R"({
 "label": "GEO/x",
 "inputs": [
  {"name": "SEED.KEY", "link": false, "source": "C:\\corpus\\SEED.KEY", "sha256": "aa"},
  {"name": "CMD.KEY", "link": false, "text": "KFREAD 0\nSEED.KEY\nGFREAD\nPART.GEO\nKFWRIT\nOUT.KEY\n", "sha256": "bb"},
  {"name": "PART.GEO", "link": false, "source": "tests/commands/GEO/part.GEO", "sha256": "cc"}
 ],
 "dirs": []
})";
    const std::vector<cmdoracle::CaseInput> in = cmdoracle::case_inputs(j);
    CHECK_EQ(static_cast<int>(in.size()), 3);
    CHECK_EQ(in[0].name, std::string("SEED.KEY"));
    CHECK_EQ(in[0].source, std::string("C:\\corpus\\SEED.KEY"));
    CHECK(!in[0].inline_text);
    CHECK_EQ(in[1].name, std::string("CMD.KEY"));
    CHECK(in[1].inline_text);
    CHECK_EQ(in[1].text, std::string("KFREAD 0\nSEED.KEY\nGFREAD\nPART.GEO\nKFWRIT\nOUT.KEY\n"));
    CHECK_EQ(in[2].source, std::string("tests/commands/GEO/part.GEO"));
}

TEST_CASE(diegeo_faces_compare_as_a_set)
{
    const std::string pts = "DIEGEO       1       1       3       2\n"
                            "       1  0.0  0.0  0.0\n"
                            "       2  1.0  0.0  0.0\n"
                            "       3  0.0  1.0  0.0\n";
    const std::string a = pts + "       1       1       2       3       1\n"
                                "       2       3       2       1       3\n";
    // same faces, other order and rotation, same orientation
    const std::string b = pts + "       1       2       1       3       2\n"
                                "       2       2       3       1       2\n";
    CHECK(cmdoracle::diegeo_equal_as_face_set(a, b));
    // the reversed orientation is a different face
    const std::string c = pts + "       1       1       3       2       1\n"
                                "       2       3       2       1       3\n";
    CHECK(!cmdoracle::diegeo_equal_as_face_set(a, c));
    // points must be equal byte for byte
    std::string d = a;
    d.replace(d.find("1.0"), 3, "1.5");
    CHECK(!cmdoracle::diegeo_equal_as_face_set(a, d));
}

TEST_CASE(start_time_is_read_from_the_metadata)
{
    CHECK_EQ(cmdoracle::entry_start_time(std::filesystem::temp_directory_path(),
                                         R"({"duration_s": 0.5, "start_time": 1792596820, "files": {}})"),
             static_cast<std::int64_t>(1792596820));
}

int main() { return testkit::run_all(); }

