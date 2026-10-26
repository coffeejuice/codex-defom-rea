// Implementation proposal only; not in source/ and not built. Apply after the
// coordinator releases the current source boundary. See native-reader-contract.
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/longpath.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include <algorithm>
#include <cstdio>
#include <functional>

namespace m2m::engine::simcontrol300 {
namespace {
bool asciiSpace(unsigned char c) { return (c >= 9 && c <= 13) || c == 32; }

// Original getReal's C-locale ASCII classes and transition table,6722c880.
int realClass(unsigned char c)
{
    if (c >= '0' && c <= '9') return 2;
    if (c == '+' || c == '-') return 1;
    if (c == '.') return 3;
    if (c == 'e' || c == 'E') return 4;
    if (c == 'i' || c == 'I') return 5;
    if (c == 'n' || c == 'N') return 6;
    if (c == 'f' || c == 'F') return 7;
    if (c == 'a' || c == 'A') return 8;
    if (c == 't' || c == 'T') return 9;
    return 0;
}

double readReal(const std::string& bytes, std::size_t& cursor)
{
    static constexpr unsigned char transitions[13][10] = {
        {0,1,2,3,0,10,8,0,0,0}, {0,0,2,3,0,10,8,0,0,0},
        {13,13,2,3,5,0,0,0,0,0}, {0,0,4,0,0,0,0,0,0,0},
        {13,13,4,13,5,0,0,0,0,0}, {0,6,7,0,0,0,0,0,0,0},
        {0,0,7,0,0,0,0,0,0,0}, {13,13,7,13,13,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,9,0}, {0,0,0,0,0,0,12,0,0,0},
        {0,0,0,0,0,0,11,0,0,0}, {0,0,0,0,0,0,0,12,0,0},
        {13,0,0,0,0,0,0,0,0,0}};
    while (cursor < bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    int state = 0;
    std::string numeric;
    while (cursor < bytes.size()) {
        const auto c = static_cast<unsigned char>(bytes[cursor++]);
        state = transitions[state][realClass(c)];
        if (state == 0 || state == 13 || numeric.size() > 123) {
            --cursor; // original ungetChar
            if (numeric.size() > 123)
                while (cursor < bytes.size() && bytes[cursor] >= '0' && bytes[cursor] <= '9') ++cursor;
            break;
        }
        numeric.push_back(static_cast<char>(c));
    }
    return numeric.empty() ? 0.0 : qt::toDouble(qt::String::latin1(numeric));
}

int readDecimalInt(const std::string& bytes, std::size_t& cursor)
{
    while (cursor < bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    if (cursor == bytes.size()) return 0;
    const auto start = cursor;
    if (bytes[cursor] == '+' || bytes[cursor] == '-') ++cursor;
    const auto digits = cursor;
    while (cursor < bytes.size() && bytes[cursor] >= '0' && bytes[cursor] <= '9') ++cursor;
    if (cursor == digits || (bytes[digits] == '0' && cursor - digits > 1) ||
        (cursor < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[cursor]))))
        throw Error(M2M_E_UNSUPPORTED, "editor300 integer input outside recovered decimal profile");
    bool valid = false;
    const int value = qt::toInt(qt::String::latin1(std::string_view(bytes).substr(start, cursor - start)), &valid);
    if (!valid) throw Error(M2M_E_UNSUPPORTED, "editor300 integer input outside recovered int32 range");
    return value;
}

class NativeExtra3DIo final : public Extra3DIo {
public:
    NativeExtra3DIo(qt::String directory, unsigned codepage, std::function<void()> warning)
        : Extra3DIo(codepage), directory_(qt::cleanPath(directory)), warning_(std::move(warning)),
          reader_(doe::makeNativeStudyIo())
    {
        if (directory_.isEmpty() || qt::isRelativePath(directory_))
            throw Error(M2M_E_ARG, "editor300 sidecar directory must be absolute");
    }
    bool readDoubles(const qt::String& path, std::span<double> values) override
    {
        std::fill(values.begin(), values.end(), 0.0);
        auto file = openRead(path);
        if (!file) return false;
        if (values.empty()) return true; // original count0 opens but does not read
        const auto bytes = asciiBytes(*file);
        std::size_t cursor = 0;
        for (double& value : values) {
            if (cursor == bytes.size()) break;
            value = readReal(bytes, cursor);
        }
        return true;
    }
    bool readInts(const qt::String& path, std::span<int> values) override
    {
        std::fill(values.begin(), values.end(), 0);
        auto file = openRead(path);
        if (!file) return false;
        if (values.empty()) return true;
        const auto bytes = asciiBytes(*file);
        std::size_t cursor = 0;
        for (int& value : values) {
            if (cursor == bytes.size()) break;
            value = readDecimalInt(bytes, cursor);
        }
        return true;
    }
    bool readSprings(std::vector<SpringRecord>& output) override
    {
        output.clear();
        std::array<int, 1> count{};
        if (!readInts(qt::L("SPRING.INI"), count) || count[0] <= 0) return false;
        throw Error(M2M_E_UNSUPPORTED, "editor300 populated spring reader/editor remains unsupported");
    }
    bool removeIfReadable(const qt::String& path) override
    {
        const auto native = win32Path(resolve(path).u16());
        std::FILE* file = _wfopen(native.c_str(), L"rt");
        if (!file) return true;
        std::fclose(file);
        return _wremove(native.c_str()) == 0;
    }
    void nbcRemoveWarning() override { if (warning_) warning_(); }
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String& path) override
    {
        return std::make_unique<mst::FileDevice>(resolve(path));
    }
private:
    qt::String resolve(const qt::String& name) const
    {
        return qt::cleanPath(directory_ + qt::kSep + name);
    }
    std::unique_ptr<doe::StudyReadFile> openRead(const qt::String& name)
    {
        const auto path = resolve(name);
        return qt::fileExists(path) ? reader_->openReadFile(path) : nullptr;
    }
    std::string asciiBytes(doe::StudyReadFile& file)
    {
        auto bytes = file.readAll();
        if (std::any_of(bytes.begin(), bytes.end(), [](unsigned char c) { return c >= 128; }))
            throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar decoder outside recovered ASCII profile");
        return bytes;
    }
    qt::String directory_;
    std::function<void()> warning_;
    std::shared_ptr<doe::StudyIo> reader_;
};
} // namespace

std::unique_ptr<Extra3DIo> makeNativeExtra3DIo(const qt::String& directory,
    unsigned codepage, std::function<void()> warning)
{
    return std::make_unique<NativeExtra3DIo>(directory, codepage, std::move(warning));
}
} // namespace m2m::engine::simcontrol300
