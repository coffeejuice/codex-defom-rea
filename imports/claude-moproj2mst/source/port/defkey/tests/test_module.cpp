// Module smoke test: the library builds with the fidelity flags and links.
#include <defkey/module.hpp>
#include <testkit/check.hpp>

#include <string>

TEST_CASE(module_links) { CHECK_EQ(std::string(def::key::module_name()), std::string("defkey")); }

int main() { return testkit::run_all(); }
