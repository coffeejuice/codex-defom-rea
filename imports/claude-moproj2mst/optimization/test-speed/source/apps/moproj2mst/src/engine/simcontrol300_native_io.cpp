// Native scalar sidecars: bounded KnownC/ASCII input profile. See
// analysis/wave2-save-diffs-300-native-reader-contract.json for qualifications.
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include <algorithm>
#include <cstdio>
#include <functional>
#include <windows.h>

namespace m2m::engine::simcontrol300 {
namespace {
bool asciiSpace(unsigned char c) { return (c >= 9 && c <= 13) || c == 32; }

// Original1407fbc10 takes narrow literal names, calls fopen("rt"), fclose,
// then remove -> DeleteFileA. Absolute Unicode/extended-path substitution is
// not established by that source. This adapter's first directory profile is
// explicitly narrower; original ASCII relative names under a Unicode CWD
// remain a recovery dependency, not a claim that the original rejects them.
std::string ordinaryNarrowPath(const qt::String& path, std::size_t reserve = 0)
{
    const auto& text = path.u16();
    if (text.size() + reserve >= MAX_PATH)
        throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar path outside ordinary MAX_PATH profile");
    std::string narrow;
    for (const auto c : text) {
        if (c < 32 || c >= 128)
            throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar path outside ASCII profile");
        narrow.push_back(c == u'\\' ? '/' : static_cast<char>(c));
    }
    if (narrow.starts_with("//?/") || narrow.starts_with("//./"))
        throw Error(M2M_E_UNSUPPORTED, "editor300 extended/device path remains unsupported");
    return narrow;
}

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
        ordinaryNarrowPath(directory); // Check namespace before cleanPath too.
        const auto ordinary = ordinaryNarrowPath(directory_, 1 + 14); // Slash plus STRAIN_DST.DAT.
        const bool driveAbsolute = ordinary.size() >= 3 &&
            ((ordinary[0] >= 'A' && ordinary[0] <= 'Z') || (ordinary[0] >= 'a' && ordinary[0] <= 'z')) &&
            ordinary[1] == ':' && ordinary[2] == '/';
        const auto serverEnd = ordinary.find('/', 2);
        const bool uncAbsolute = ordinary.starts_with("//") && serverEnd != std::string::npos &&
            serverEnd > 2 && serverEnd + 1 < ordinary.size();
        if (!driveAbsolute && !uncAbsolute)
            throw Error(M2M_E_ARG, "editor300 sidecar directory must be fully absolute");
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
        const auto native = ordinaryNarrowPath(resolve(path));
        std::FILE* file = std::fopen(native.c_str(), "rt");
        if (!file) return true;
        std::fclose(file);
        return std::remove(native.c_str()) == 0;
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
