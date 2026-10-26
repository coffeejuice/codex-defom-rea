// P06: project file access for the data-file readers and C stdio text mode ("rt").
#include <filesystem>
#include <fstream>
#include <iterator>

#include "m2m/datafiles.hpp"
#include "m2m/longpath.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::datafiles {

bool readFileBytes(const Project& p, const String& path, std::string& bytes)
{
    if (path.isEmpty()) return false;
    auto it = p.overlay.find(qt::cleanPath(path));
    if (it != p.overlay.end()) {
        bytes = it->second;
        return true;
    }
    std::ifstream in(std::filesystem::path(win32Path(path.u16())), std::ios::binary);   // long paths
    if (!in) return false;
    bytes.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return true;
}

// The MSVC CRT in text mode: CR LF is read as LF (a lone CR stays), Ctrl-Z ends the file.
TextFile::TextFile(const std::string& rawBytes)
{
    data_.reserve(rawBytes.size());
    for (std::size_t i = 0; i < rawBytes.size(); ++i) {
        char c = rawBytes[i];
        if (c == '\x1a') break;
        if (c == '\r' && i + 1 < rawBytes.size() && rawBytes[i + 1] == '\n') continue;
        data_ += c;
    }
}

bool TextFile::open(const Project& p, const String& path, TextFile& f)
{
    // fopen takes the 8-bit form of the path (QString::latin1 / toLocal8Bit). A character the
    // conversion cannot carry becomes '?', which names no file.
    for (char16_t c : path.u16())
        if (c > 0xff) return false;
    std::string raw;
    if (!readFileBytes(p, path, raw)) return false;
    f = TextFile(raw);
    return true;
}

int TextFile::getc()
{
    requireMemory();
    if (pos_ >= data_.size()) return -1;
    return static_cast<unsigned char>(data_[pos_++]);
}

bool TextFile::fgets(std::string& buf, int n)
{
    if (lineReader_) return lineReader_(buf, n);
    if (pos_ >= data_.size()) return false;
    std::string out;
    while (static_cast<int>(out.size()) < n - 1 && pos_ < data_.size()) {
        char c = data_[pos_++];
        out += c;
        if (c == '\n') break;
    }
    buf = std::move(out);
    return true;
}

TextFile TextFile::fromLineReader(LineReader reader)
{
    if (!reader) throw Error(M2M_E_ARG, "TextFile requires a live line reader");
    TextFile result;
    result.lineReader_ = std::move(reader);
    return result;
}

void TextFile::requireMemory() const
{
    if (lineReader_)
        throw Error(M2M_E_UNSUPPORTED, "borrowed TextFile supports only fgets");
}
bool TextFile::eof() const { requireMemory(); return pos_ >= data_.size(); }
void TextFile::ungetc() { requireMemory(); if (pos_ > 0) --pos_; }
std::size_t TextFile::pos() const { requireMemory(); return pos_; }
const std::string& TextFile::data() const { requireMemory(); return data_; }

}  // namespace m2m::datafiles
