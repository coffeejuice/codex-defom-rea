#include "deform/object_summary.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string_view>

namespace {
void json_string(std::ostream& out, std::string_view value) {
    out << '"';
    for (const unsigned char c : value) {
        if (c == '"' || c == '\\') out << '\\' << c;
        else if (c < 32 || c >= 127)
            out << "\\u00" << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<unsigned>(c) << std::dec << std::setfill(' ');
        else out << c;
    }
    out << '"';
}
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: deform_objects database.DB step-number output.json\n";
        return 2;
    }
    try {
        const deform::database db(argv[1]);
        const auto step = db.load_step(db.find_step(std::stoi(argv[2])));
        const auto objects = deform::summarize_objects(step);
        std::ofstream out(argv[3]);
        if (!out) throw std::runtime_error("Cannot create object summary");
        out << "{\"step\":" << step.record.number() << ",\"dimension\":" << step.record.dimension()
            << ",\"version\":" << step.record.version() << ",\"objects\":[\n";
        for (std::size_t i = 0; i < objects.size(); ++i) {
            const auto& o = objects[i];
            if (i) out << ",\n";
            out << "{\"id\":" << o.id << ",\"active\":" << (o.active ? "true" : "false")
                << ",\"has_mesh\":" << (o.has_mesh ? "true" : "false")
                << ",\"raw_type\":" << o.raw_type << ",\"internal_type\":" << o.internal_type
                << ",\"node_count\":" << o.node_count << ",\"element_count\":" << o.element_count
                << ",\"name\":";
            json_string(out, o.name);
            out << ",\"integer_descriptor_index\":" << o.integer_descriptor_index
                << ",\"real_descriptor_index\":" << o.real_descriptor_index << '}';
        }
        out << "\n]}\n";
        if (!out) throw std::runtime_error("Cannot write object summary");
        std::cout << objects.size() << " objects\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
