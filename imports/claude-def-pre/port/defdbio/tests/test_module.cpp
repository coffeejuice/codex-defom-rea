// Module smoke test: the library builds with the fidelity flags and links.
#include <defdbio/module.hpp>
#include <testkit/check.hpp>

#include <string>

TEST_CASE(module_links) { CHECK_EQ(std::string(def::dbio::module_name()), std::string("defdbio")); }

int main() { return testkit::run_all(); }
