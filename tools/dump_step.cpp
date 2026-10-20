#include "deform/database.hpp"

#include <fstream>
#include <iostream>
#include <string>

template<class T>
void write(const std::string& path, const std::vector<T>& values) {
    std::ofstream out(path, std::ios::binary);
    out.write(reinterpret_cast<const char*>(values.data()),
              static_cast<std::streamsize>(values.size() * sizeof(T)));
    if (!out) throw std::runtime_error("Cannot write " + path);
}
int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: deform_dump database.DB step-number output-prefix\n"
                     "Writes host-order header, int32 and float64 array buffers.\n";
        return 2;
    }
    try {
        deform::database db(argv[1]);
        const auto step = db.load_step(db.find_step(std::stoi(argv[2])));
        const std::string prefix(argv[3]);
        write(prefix + ".header.bin", step.header);
        write(prefix + ".integers.bin", step.integers);
        write(prefix + ".reals.bin", step.reals);
        std::cout << step.header.size() << ' ' << step.integers.size() << ' '
                  << step.reals.size() << '\n';
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
