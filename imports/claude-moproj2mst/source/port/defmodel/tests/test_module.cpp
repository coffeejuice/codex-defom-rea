// Module smoke test: the library builds with the fidelity flags and links.
#include <defmodel/module.hpp>
#include <testkit/check.hpp>

#include <string>

TEST_CASE(module_links) { CHECK_EQ(std::string(def::model::module_name()), std::string("defmodel")); }

int main() { return testkit::run_all(); }
