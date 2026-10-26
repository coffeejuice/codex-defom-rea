// Module smoke test: the library builds with the fidelity flags and links.
#include <defcmd/module.hpp>
#include <testkit/check.hpp>

#include <string>

TEST_CASE(module_links) { CHECK_EQ(std::string(def::cmd::module_name()), std::string("defcmd")); }

int main() { return testkit::run_all(); }
