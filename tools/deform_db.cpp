#include "deform/database.hpp"

#include <bit>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

namespace {
template<class T>
std::string fingerprint(const std::vector<T>& values) {
    std::uint64_t hash = 14695981039346656037ULL;
    for (const auto value : values) {
        using bits = std::conditional_t<sizeof(T) == 4, std::uint32_t, std::uint64_t>;
        auto raw = std::bit_cast<bits>(value);
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            hash ^= (raw >> (8 * i)) & 255;
            hash *= 1099511628211ULL;
        }
    }
    std::ostringstream out;
    out << std::hex << std::setfill('0') << std::setw(16) << hash;
    return out.str();
}
void header(std::ostream& out, const deform::database& db) {
    const auto& h = db.header();
    out << "DB " << std::setw(4) << h.version << "\n*\n";
    out << "DB Version     : [" << std::setw(8) << h.version << "]\n";
    out << "ReleaseDate    : [" << h.release_date << "]\n";
    out << "InitialSimDate : [" << h.initial_date << "]\n";
    out << "CurrentSimDate : [" << h.current_date << "]\n";
    out << "SimType        : [" << std::setw(8) << h.simulation_type() << "]\n";
    out << "NoRecords      : [" << std::setw(8) << h.record_count() << "]\n";
    out << "NoStoredSteps  : [" << std::setw(8) << db.stored_steps().size() << "]\n";
}
void simulation_steps(std::ostream& out, const deform::database& db) {
    struct operation { int dimension{}, first{}, last{}; };
    std::map<int, operation> operations;
    for (const auto i : db.stored_steps()) {
        const auto& r = db.records()[i];
        const auto op = r.words[5];
        if (op < 1) continue;
        if (!operations.contains(op)) operations.emplace(op, operation{r.dimension(), r.number(), 0});
        else operations[op].last = r.number();
    }
    out << std::setw(7) << operations.size() << ' ' << std::setw(7) << 4 << '\n';
    for (const auto& [id, op] : operations) {
        out << std::setw(7) << id << ' ' << std::setw(7) << op.dimension << '\n';
        out << "Operation " << id << '\n';
        out << std::setw(7) << op.first << '\n' << std::setw(7) << op.last << '\n';
    }
}
}

int main(int argc, char** argv) {
    std::string mode, input, output;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::cout << "deform_db -m {header|stepList|lastStepNo|simStepList|verify} -d file.DB -o result\n"
                         "verify decodes every stored step and reports array sizes and FNV-1a fingerprints.\n";
            return 0;
        }
        if ((arg == "-m" || arg == "-d" || arg == "-o") && i + 1 < argc) {
            auto& destination = arg == "-m" ? mode : arg == "-d" ? input : output;
            destination = argv[++i];
        } else {
            std::cerr << "Unknown or incomplete option: " << arg << '\n';
            return 2;
        }
    }
    if (input.empty() || mode.empty() || output.empty()) {
        std::cerr << "Specify -m, -d, and -o; see -h.\n";
        return 2;
    }
    if (mode != "header" && mode != "stepList" && mode != "lastStepNo" &&
        mode != "simStepList" && mode != "verify") {
        std::cerr << "Unsupported query mode\n";
        return 2;
    }
    std::cout << "\n*** DEF_PRE.EXE ***\n";
    try {
        deform::database db(input);
        std::ofstream out(output);
        if (!out) throw std::runtime_error("Cannot create result file");
        const auto steps = db.stored_steps();
        if (mode == "header") header(out, db);
        else if (mode == "stepList") {
            out << std::setw(7) << steps.size() << '\n';
            for (const auto i : steps) out << std::setw(7) << db.records()[i].number() << '\n';
        } else if (mode == "lastStepNo") {
            out << std::setw(7) << (steps.empty() ? 0 : db.records()[steps.back()].number()) << '\n';
        } else if (mode == "simStepList") simulation_steps(out, db);
        else {
            out << "{\"file_bytes\":" << db.file_size() << ",\"records\":" << db.records().size()
                << ",\"steps\":[\n";
            bool first = true;
            for (const auto i : steps) {
                const auto step = db.load_step(i);
                if (!first) out << ",\n";
                first = false;
                out << "{\"record\":" << i + 1 << ",\"step\":" << step.record.number()
                    << ",\"header_count\":" << step.header.size()
                    << ",\"integer_count\":" << step.integers.size()
                    << ",\"real_count\":" << step.reals.size()
                    << ",\"header_fnv\":\"" << fingerprint(step.header)
                    << "\",\"integer_fnv\":\"" << fingerprint(step.integers)
                    << "\",\"real_fnv\":\"" << fingerprint(step.reals) << "\"}";
            }
            out << "\n]}\n";
        }
        return out ? 0 : 1;
    } catch (const std::exception& e) {
        if (mode == "verify") {
            std::cerr << e.what() << '\n';
            return 1;
        }
        // Only this invalid-input subset is runtime verified against the
        // original. Other read failures need separate behavior characterization.
        if (mode == "header") {
            std::ofstream out(output);
            out << "Not a valid DEFORM database!\n";
        }
        return 0;
    }
}
