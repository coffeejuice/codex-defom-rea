#include "recovered/keyword_catalog.hpp"
#include "recovered/keyword_import.hpp"
#include "recovered/keyword_reader.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace deform::recovered;

namespace {
std::string unhex(const std::string& text) {
    if (text == "-") return {};
    if (text.size() % 2) throw std::runtime_error("odd hex input");
    std::string result;
    const auto digit = [](char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        throw std::runtime_error("invalid hex input");
    };
    for (std::size_t i = 0; i < text.size(); i += 2)
        result.push_back(static_cast<char>(digit(text[i]) * 16 + digit(text[i + 1])));
    return result;
}
void quoted(std::string_view text) {
    constexpr char digits[] = "0123456789abcdef";
    std::cout << '"';
    for (unsigned char c : text) {
        if (c == '"' || c == '\\') std::cout << '\\' << static_cast<char>(c);
        else if (c < 32 || c >= 127)
            std::cout << "\\u00" << digits[c >> 4] << digits[c & 15];
        else std::cout << static_cast<char>(c);
    }
    std::cout << '"';
}
void metadata(const RootMetadata& m) {
    std::cout << "{\"title\":"; quoted(m.title);
    std::cout << ",\"operation_name\":"; quoted(m.operation_name);
    std::cout << ",\"simulation_name\":"; quoted(m.simulation_name);
    std::cout << ",\"cursim\":[" << m.cursim[0] << ',' << m.cursim[1]
              << ',' << m.cursim[2] << "]}";
}
void roots(const ObjectModel& primary, const ObjectModel& linked) {
    std::cout << "{\"primary\":"; metadata(primary.metadata());
    std::cout << ",\"linked\":"; metadata(linked.metadata());
    std::cout << '}';
}
void dimensions(const ObjectModel& primary, const ObjectModel& linked) {
    std::cout << "{\"primary\":" << primary.controls().dimension
              << ",\"linked\":" << linked.controls().dimension << '}';
}
}

int main() {
    try {
        ObjectModel primary(initialized_root_controls(3, 20));
        ObjectModel linked(initialized_root_controls(2, 20));
        primary.link_root(&linked);
        KeywordImporter primary_import(primary, keyword_catalog);
        KeywordImporter linked_import(linked, keyword_catalog);
        std::string command;
        while (std::cin >> command) {
            if (command == "import") {
                std::string target, encoded;
                std::cin >> target >> encoded;
                if (target != "primary" && target != "linked")
                    throw std::runtime_error("invalid root");
                auto& importer = target == "primary" ? primary_import : linked_import;
                std::ostringstream observations;
                auto* saved = std::cout.rdbuf(observations.rdbuf());
                bool first = true;
                importer.set_handler_observer([&](const KeywordEvent& e, const ObjectModel&) {
                    if (!first) std::cout << ',';
                    first = false;
                    std::cout << "{\"token\":"; quoted(e.token);
                    std::cout << ",\"offset\":" << e.offset << ",\"cursor\":" << e.cursor
                              << ",\"status\":" << *e.handler_status << ",\"metadata\":";
                    roots(primary, linked);
                    std::cout << ",\"dimensions\":"; dimensions(primary, linked);
                    std::cout << '}';
                });
                const auto r = importer.import_translated(unhex(encoded), false);
                importer.set_handler_observer({});
                std::cout.rdbuf(saved);
                std::cout << "{\"outcome\":" << static_cast<int>(r.outcome)
                          << ",\"status\":" << r.status << ",\"cursor\":" << r.cursor
                          << ",\"eof\":" << (r.reached_eof ? "true" : "false")
                          << ",\"handlers\":[" << observations.str() << "],\"metadata\":";
                roots(primary, linked);
                std::cout << ",\"dimensions\":"; dimensions(primary, linked);
                std::cout << ",\"events\":[";
                bool first_event = true;
                for (const auto& e : r.events) {
                    if (!first_event) std::cout << ',';
                    first_event = false;
                    std::cout << "{\"token\":"; quoted(e.token);
                    std::cout << ",\"offset\":" << e.offset << ",\"cursor\":" << e.cursor
                              << ",\"unknown_warning\":" << (e.unknown_warning ? "true" : "false") << '}';
                }
                std::cout << "]}\n";
            } else if (command == "helper") {
                std::string keyword, encoded, previous;
                std::size_t offset;
                std::cin >> keyword >> offset >> encoded >> previous;
                const auto bytes = unhex(encoded);
                KeywordReader reader(bytes, offset);
                int status, conversions = -1;
                if (keyword == "CURSIM") {
                    const auto r = reader.integer_triple();
                    status = r.status; conversions = r.conversions;
                    std::cout << "{\"values\":[" << r.values[0] << ',' << r.values[1] << ',' << r.values[2] << ']';
                } else {
                    const auto r = keyword == "OPRNAM" ? reader.material_name(unhex(previous))
                                                       : reader.name(unhex(previous));
                    status = r.status;
                    std::cout << "{\"text\":"; quoted(r.bytes);
                }
                std::cout << ",\"status\":" << status << ",\"conversions\":" << conversions
                          << ",\"cursor\":" << reader.cursor() << ",\"lines\":[";
                bool first = true;
                for (const auto& line : reader.line_reads()) {
                    if (!first) std::cout << ',';
                    first = false;
                    std::cout << "{\"offset\":" << line.offset << ",\"cursor\":" << line.cursor
                              << ",\"status\":" << line.status << ",\"bytes\":";
                    quoted(line.bytes); std::cout << '}';
                }
                std::cout << "]}\n";
            } else throw std::runtime_error("unknown command");
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
