// Qt 4.8 path functions on Windows (QDir, QFileInfo, QFileSystemEntry) and DEF_GUI_MO's own
// path helpers (packet P05).
//
// The Qt part is ported from the QtCore4_64.dll that ships with DEFORM v11.0.2 (read from a copy,
// image base 0x67000000); each function carries `// @qt 0x670...` with its address there. The
// DEF_GUI_MO helpers carry `// @src 0x14...`. Null and empty strings are kept apart exactly as the
// Qt code does, because callers append and compare these results.
//
// One deliberate difference: QDir::setCurrent changes the process working directory in the GUI.
// Here the current directory is per thread (setCurrent/currentPath below), so conversions on
// different threads do not interfere. With no setCurrent on a thread it is the process directory.
#include "m2m/qt4path.hpp"

#include "m2m/longpath.hpp"

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <vector>

namespace m2m::qt {
namespace {

constexpr char16_t kSlash = u'/';

// ---- QString members used here (Qt 4.8 rules; the shared ones are P04's) --------------------------

// @qt 0x6706b320  QString(const QChar*, int): a size of 0 or less gives an empty, non-null string.
String fromRange(const std::u16string& s, int pos, int n)
{
    if (n < 1) return String(std::u16string());
    return String(s.substr(static_cast<size_t>(pos), static_cast<size_t>(n)));
}

// @qt 0x67073fa0  QString::mid(position, n): null when the string is null or position >= size.
String mid(const String& s, int position, int n = -1)
{
    const int size = s.size();
    if (s.isNull() || size <= position) return String();
    if (n < 0) n = size - position;
    if (position < 0) {
        n += position;
        position = 0;
    }
    if (size < position + n) n = size - position;
    if (position == 0 && n == size) return s;
    return fromRange(s.u16(), position, n);
}

// @qt 0x67073ef0  QString::left(n): the whole string unless 0 <= n < size.
String left(const String& s, int n)
{
    if (n < s.size() && n >= 0) return fromRange(s.u16(), 0, n);
    return s;
}

// QString(QChar)
String ofChar(char16_t c) { return String(std::u16string(1, c)); }

// QString::replace(QChar before, QChar after, Qt::CaseSensitive)
String replaced(const String& s, char16_t before, char16_t after)
{
    if (s.u16().find(before) == std::u16string::npos) return s;
    std::u16string t = s.u16();
    std::replace(t.begin(), t.end(), before, after);
    return String(std::move(t));
}

// QString::toLower. Qt uses its own Unicode tables; this uses the Windows ones (LCMapStringEx,
// invariant locale, linguistic casing off), which agree for every letter a path is likely to hold.
std::u16string lower(const std::u16string& s)
{
    if (s.empty()) return s;
    std::u16string out(s.size(), u'\0');
    const int n = LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, reinterpret_cast<LPCWSTR>(s.data()),
                                static_cast<int>(s.size()), reinterpret_cast<LPWSTR>(out.data()),
                                static_cast<int>(out.size()), nullptr, nullptr, 0);
    if (n != static_cast<int>(s.size())) return s;
    return out;
}

// QChar::toUpper (same note as lower()).
char16_t upper(char16_t c)
{
    wchar_t in = static_cast<wchar_t>(c), out = in;
    if (LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_UPPERCASE, &in, 1, &out, 1, nullptr, nullptr, 0) != 1) return c;
    return static_cast<char16_t>(out);
}

// QChar::isLetter
bool isLetter(char16_t c)
{
    WORD type = 0;
    const wchar_t w = static_cast<wchar_t>(c);
    if (!GetStringTypeW(CT_CTYPE1, &w, 1, &type)) return false;
    return (type & C1_ALPHA) != 0;
}

// QString::startsWith(QLatin1String, Qt::CaseSensitive)
bool startsWithCs(const String& s, std::u16string_view what)
{
    return s.u16().size() >= what.size() && std::u16string_view(s.u16()).substr(0, what.size()) == what;
}

// QString::split(QChar('/'), QString::SkipEmptyParts, Qt::CaseSensitive)
std::vector<String> splitSkipEmpty(const String& s, char16_t sep)
{
    std::vector<String> parts;
    const std::u16string& t = s.u16();
    size_t start = 0;
    for (;;) {
        const size_t end = t.find(sep, start);
        const size_t stop = end == std::u16string::npos ? t.size() : end;
        if (stop > start) parts.push_back(String(t.substr(start, stop - start)));
        if (end == std::u16string::npos) break;
        start = end + 1;
    }
    return parts;
}

// ---- QFileSystemEntry (Windows), built from an internal path ('/' separators) ---------------------

struct Entry {
    String filePath;
    mutable short lastSeparator = -2;       // -2: not computed yet
    mutable short firstDotInFileName = -2;  // -2: not computed yet; relative to lastSeparator
    mutable short lastDotInFileName = 0;    // relative to firstDotInFileName

    explicit Entry(String p) : filePath(std::move(p)) {}

    char16_t at(int i) const { return filePath.at(i); }
    int size() const { return filePath.size(); }

    // @qt 0x670ef1e0  findLastSeparator
    void findLastSeparator() const
    {
        if (lastSeparator != -2) return;
        lastSeparator = -1;
        for (int i = size() - 1; i >= 0; --i) {
            if (at(i) == kSlash) {
                lastSeparator = static_cast<short>(i);
                break;
            }
        }
    }

    // @qt 0x670ef260  findFileNameSeparators
    void findFileNameSeparators() const
    {
        if (firstDotInFileName != -2) return;
        int firstDot = -1;
        int lastDot = -1;
        int lastSep = lastSeparator;
        int stop;
        if (lastSep < 0) {
            lastSep = -1;
            stop = 0;
        } else {
            stop = lastSep;
        }
        int i = size() - 1;
        for (; i >= stop; --i) {
            if (at(i) == u'.') {
                firstDot = lastDot = i;
                break;
            } else if (at(i) == kSlash) {
                lastSep = i;
                break;
            }
        }
        if (lastSep != i) {
            for (--i; i >= stop; --i) {
                if (at(i) == u'.') {
                    firstDot = i;
                } else if (at(i) == kSlash) {
                    lastSep = i;
                    break;
                }
            }
        }
        lastSeparator = static_cast<short>(lastSep);
        firstDotInFileName = static_cast<short>(firstDot == -1 ? -1 : firstDot - std::max(0, lastSep));
        if (lastDot == -1) lastDotInFileName = -1;
        else if (firstDot == lastDot) lastDotInFileName = 0;
        else lastDotInFileName = static_cast<short>(lastDot - firstDot);
    }

    bool driveOnly() const { return lastSeparator == -1 && size() >= 2 && at(1) == u':'; }

    // @qt 0x670ef440  fileName
    String fileName() const
    {
        findLastSeparator();
        if (driveOnly()) return mid(filePath, 2);
        return mid(filePath, lastSeparator + 1);
    }

    // @qt 0x670ef4b0  path
    String path() const
    {
        findLastSeparator();
        if (lastSeparator == -1) {
            if (size() >= 2 && at(1) == u':') return left(filePath, 2);
            return ofChar(u'.');
        }
        if (lastSeparator == 0) return ofChar(kSlash);
        if (lastSeparator == 2 && at(1) == u':') return left(filePath, lastSeparator + 1);
        return left(filePath, lastSeparator);
    }

    // @qt 0x670ef570  baseName
    String baseName() const
    {
        findFileNameSeparators();
        int length = -1;
        if (firstDotInFileName >= 0) {
            length = firstDotInFileName;
            if (lastSeparator != -1) --length;
        }
        if (driveOnly()) return mid(filePath, 2, length - 2);
        return mid(filePath, lastSeparator + 1, length);
    }

    // @qt 0x670ef600  completeBaseName
    String completeBaseName() const
    {
        findFileNameSeparators();
        int length = -1;
        if (firstDotInFileName >= 0) {
            length = firstDotInFileName + lastDotInFileName;
            if (lastSeparator != -1) --length;
        }
        if (driveOnly()) return mid(filePath, 2, length - 2);
        return mid(filePath, lastSeparator + 1, length);
    }

    // @qt 0x670ef690  suffix
    String suffix() const
    {
        findFileNameSeparators();
        if (lastDotInFileName == -1) return String();
        return mid(filePath, std::max<int>(0, lastSeparator) + firstDotInFileName + lastDotInFileName + 1);
    }

    // @qt 0x670ef080  isRelative
    bool isRelative() const
    {
        if (size() == 0) return true;
        if (at(0) == kSlash) return false;
        if (size() >= 2 && at(1) == u':') return false;
        return true;
    }

    // @qt 0x670ef110  isAbsolute
    bool isAbsolute() const
    {
        if (size() >= 3 && isLetter(at(0)) && at(1) == u':' && at(2) == kSlash) return true;
        return size() >= 2 && at(0) == kSlash && at(1) == kSlash;
    }

    // @qt 0x670eefa0  isClean
    bool isClean() const
    {
        int dots = 0;
        bool dotok = true;
        bool slashok = true;
        for (char16_t c : filePath.u16()) {
            if (c == kSlash) {
                if (dots == 1 || dots == 2) return false;
                if (!slashok) return false;
                dots = 0;
                dotok = true;
                slashok = false;
            } else if (dotok) {
                slashok = true;
                if (c == u'.') {
                    ++dots;
                    if (dots > 2) dotok = false;
                } else {
                    dots = 0;
                    dotok = false;
                }
            }
        }
        return dots != 1 && dots != 2;
    }
};

// @qt 0x670eedd0  resolveFilePath of an entry made from a native path
String fromNativeEntryPath(const String& native)
{
    String p = fromNativeSeparators(native);
    if (startsWithCs(p, u"//?/UNC/")) p = String(p.u16().substr(0, 2) + p.u16().substr(8));
    if (startsWithCs(p, u"//?/")) p = String(p.u16().substr(4));
    return p;
}

thread_local bool tCurrentSet = false;
thread_local String tCurrent;   // internal form ('/' separators)

// @qt 0x670ff490  QFileSystemEngine::currentPath (GetCurrentDirectoryW, drive letter upper-cased)
String processCurrentPath()
{
    String ret;
    wchar_t buf[MAX_PATH];
    DWORD n = GetCurrentDirectoryW(MAX_PATH, buf);
    if (n != 0) {
        if (n <= MAX_PATH) {
            ret = String(std::u16string(reinterpret_cast<const char16_t*>(buf)));
        } else {
            std::vector<wchar_t> big(n);
            // The binary passes MAX_PATH as the size here too, so a long directory fails the same way.
            if (GetCurrentDirectoryW(MAX_PATH, big.data()) != 0)
                ret = String(std::u16string(reinterpret_cast<const char16_t*>(big.data())));
        }
    }
    if (ret.size() > 1 && ret.at(1) == u':') {
        std::u16string t = ret.u16();
        t[0] = upper(t[0]);
        ret = String(std::move(t));
    }
    return fromNativeEntryPath(ret);
}

// @qt 0x670ffbf0  nativeAbsoluteFilePath (GetFullPathNameW; a trailing space is put back)
String nativeAbsoluteFilePath(const String& path)
{
    String absPath;
    const std::u16string& p = path.u16();
    DWORD bufSize = std::max<DWORD>(MAX_PATH, static_cast<DWORD>(p.size() + 1));
    std::vector<wchar_t> buf(bufSize);
    DWORD ret = GetFullPathNameW(reinterpret_cast<LPCWSTR>(p.c_str()), bufSize, buf.data(), nullptr);
    if (ret > bufSize) {
        bufSize = ret;
        buf.assign(bufSize, L'\0');
        ret = GetFullPathNameW(reinterpret_cast<LPCWSTR>(p.c_str()), bufSize, buf.data(), nullptr);
    }
    if (ret != 0) absPath = String(std::u16string(reinterpret_cast<const char16_t*>(buf.data())));
    if (!p.empty() && p.back() == u' ') absPath += u' ';
    return absPath;
}

// @qt 0x67101990  QFileSystemEngine::absoluteName(entry).filePath()
String absoluteName(const Entry& e)
{
    String ret;
    if (!e.isRelative()) {
        if (e.isAbsolute() && e.isClean()) ret = e.filePath;
        else ret = fromNativeSeparators(nativeAbsoluteFilePath(e.filePath));
    } else {
        ret = cleanPath(currentPath() + kSlash + e.filePath);
    }
    // Force an upper-case drive letter. On an empty result the binary grows the string to one
    // character and stores the upper-cased terminator, i.e. U+0000.
    if (ret.size() == 0 || ret.at(0) != kSlash) {
        std::u16string t = ret.u16();
        if (t.empty()) t.push_back(u'\0');
        else t[0] = upper(t[0]);
        ret = String(std::move(t));
    }
    return ret;
}

// @qt 0x670b8030  QFileInfoPrivate(const QString&): the entry holds fromNativeSeparators(file).
Entry fileInfo(const String& file) { return Entry(fromNativeSeparators(file)); }

// @qt 0x670b0080  QDirPrivate::setPath (with the QDirPrivate ctor's "." for an empty path, 0x670b0cb0)
Entry dirEntry(const String& path)
{
    String p = fromNativeSeparators(path.isEmpty() ? L(".") : path);
    const int n = p.size();
    if (n > 1 && p.at(n - 1) == kSlash) {
        const bool driveRoot = n == 3 && p.at(1) == u':' && isLetter(p.at(0));
        if (!driveRoot) p = left(p, n - 1);
    }
    return Entry(p);
}

// @qt 0x670ad5c0  QDir::absolutePath = 0x670accb0 resolveAbsoluteEntry, then filePath
String dirAbsolutePath(const String& dir)
{
    const Entry d = dirEntry(dir);
    if (d.filePath.isEmpty()) return String();
    if (!d.isRelative() && d.isClean()) return d.filePath;
    return cleanPath(absoluteName(d));
}

// @qt 0x670aba90  driveSpec: the letter test in the binary is always true, only ':' is checked.
String driveSpec(const String& path)
{
    if (path.size() < 2) return String();
    if (path.at(1) != u':') return String();
    return mid(path, 0, 2);
}

}  // namespace

// @qt 0x670ac620  QDir::cleanPath
String cleanPath(const String& path)
{
    if (path.isEmpty()) return path;
    const String name = replaced(path, kSep, kSlash);
    const std::u16string& p = name.u16();
    const int len = static_cast<int>(p.size());
    std::u16string out(static_cast<size_t>(len), u'\0');
    // p[len] is the terminating null of the QString data; reads there see 0.
    auto P = [&](int i) -> char16_t { return i < len ? p[static_cast<size_t>(i)] : u'\0'; };

    int used = 0, levels = 0;
    for (int i = 0, last = -1, iwrite = 0; i < len; ++i) {
        if (P(i) == kSlash) {
            while (i < len - 1 && P(i + 1) == kSlash) {
                if (!i) break;   // keep a UNC "//" prefix
                i++;
            }
            bool eaten = false;
            if (i < len - 1 && P(i + 1) == u'.') {
                int dotcount = 1;
                if (i < len - 2 && P(i + 2) == u'.') dotcount++;
                if (i == len - dotcount - 1) {
                    if (dotcount == 1) {
                        break;
                    } else if (levels) {
                        if (last == -1) {
                            for (int i2 = iwrite - 1; i2 >= 0; i2--) {
                                if (out[static_cast<size_t>(i2)] == kSlash) {
                                    last = i2;
                                    break;
                                }
                            }
                        }
                        used -= iwrite - last - 1;
                        break;
                    }
                } else if (P(i + dotcount + 1) == kSlash) {
                    if (dotcount == 2 && levels) {
                        if (last == -1 || iwrite - last == 1) {
                            for (int i2 = (last == -1) ? (iwrite - 1) : (last - 1); i2 >= 0; i2--) {
                                if (out[static_cast<size_t>(i2)] == kSlash) {
                                    eaten = true;
                                    last = i2;
                                    break;
                                }
                            }
                        } else {
                            eaten = true;
                        }
                        if (eaten) {
                            levels--;
                            used -= iwrite - last;
                            iwrite = last;
                            last = -1;
                        }
                    } else if (dotcount == 2 && i > 0 && P(i - 1) != u'.') {
                        eaten = true;
                        used -= iwrite - std::max(0, last);
                        iwrite = std::max(0, last);
                        last = -1;
                        ++i;
                    } else if (dotcount == 1) {
                        eaten = true;
                    }
                    if (eaten) i += dotcount;
                } else {
                    levels++;
                }
            } else if (last != -1 && iwrite - last == 1) {
                eaten = (iwrite > 2);   // Windows: keep "C:/" and "//"
                last = -1;
            } else if (last != -1 && i == len - 1) {
                eaten = true;
            } else {
                levels++;
            }
            if (!eaten) last = iwrite;
            else continue;
        } else if (!i && P(i) == u'.') {
            int dotcount = 1;
            if (len >= 1 && P(1) == u'.') dotcount++;
            if (len >= dotcount && P(dotcount) == kSlash) {
                if (dotcount == 1) {
                    i++;
                    while (i + 1 < len - 1 && P(i + 1) == kSlash) i++;
                    continue;
                }
            }
        }
        out[static_cast<size_t>(iwrite++)] = P(i);
        used++;
    }

    String ret = used == len ? name : fromRange(out, 0, used);
    // Strip a trailing slash except for roots ("/" and "C:/").
    if (ret.size() > 1 && ret.at(ret.size() - 1) == kSlash) {
        if (!(ret.size() == 3 && ret.at(1) == u':')) ret = left(ret, ret.size() - 1);
    }
    return ret;
}

// @qt 0x670ade90  QDir::convertSeparators = 0x670ad600 toNativeSeparators
String convertSeparators(const String& p) { return replaced(p, kSlash, kSep); }

// @qt 0x670ad700  QDir::fromNativeSeparators
String fromNativeSeparators(const String& p) { return replaced(p, kSep, kSlash); }

// @qt 0x670ae290  QDir(dir).relativeFilePath(fileName)
String relativeFilePath(const String& dirPath, const String& fileName)
{
    String dir = cleanPath(dirAbsolutePath(dirPath));
    String file = cleanPath(fileName);

    if (fileInfo(file).isRelative() || fileInfo(dir).isRelative()) return file;

    const String dirDrive = driveSpec(dir);
    String fileDrive = driveSpec(file);
    bool fileDriveMissing = false;
    if (fileDrive.isEmpty()) {
        fileDrive = dirDrive;
        fileDriveMissing = true;
    }
    if (lower(fileDrive.u16()) != lower(dirDrive.u16()) || (startsWithCs(file, u"//") && !startsWithCs(dir, u"//")))
        return file;

    dir = mid(dir, dirDrive.size());
    if (!fileDriveMissing) file = mid(file, fileDrive.size());
    // QString::remove(0, n) keeps an emptied string non-null; mid() above would make it null.
    if (dir.isNull()) dir = String(std::u16string());
    if (file.isNull()) file = String(std::u16string());

    String result;
    const std::vector<String> dirElts = splitSkipEmpty(dir, kSlash);
    const std::vector<String> fileElts = splitSkipEmpty(file, kSlash);
    const int nd = static_cast<int>(dirElts.size()), nf = static_cast<int>(fileElts.size());

    int i = 0;
    while (i < nd && i < nf && lower(dirElts[static_cast<size_t>(i)].u16()) == lower(fileElts[static_cast<size_t>(i)].u16()))
        ++i;
    for (int j = 0; j < nd - i; ++j) result += L("../");
    for (int j = i; j < nf; ++j) {
        result += fileElts[static_cast<size_t>(j)];
        if (j < nf - 1) result += kSlash;
    }
    return result;
}

// @qt 0x670b7b10  QFileInfo::fileName
String fileName(const String& p) { return fileInfo(p).fileName(); }
// @qt 0x670b7bb0  QFileInfo::completeBaseName
String completeBaseName(const String& p) { return fileInfo(p).completeBaseName(); }
// @qt 0x670b7b60  QFileInfo::baseName
String baseName(const String& p) { return fileInfo(p).baseName(); }
// @qt 0x670b7c50  QFileInfo::suffix
String suffix(const String& p) { return fileInfo(p).suffix(); }
// @qt 0x670b7a70  QFileInfo::path
String path(const String& p) { return fileInfo(p).path(); }

// @qt 0x670b8ab0  QFileInfo::absolutePath (0x670b8260 case AbsolutePathName)
String absolutePath(const String& p)
{
    const Entry e = fileInfo(p);
    if (e.filePath.isEmpty()) return L("");   // qWarning "Constructed with empty filename"
    const String r = Entry(absoluteName(e)).path();
    if (r.isNull()) return L("");
    return r;
}

// @qt 0x670b4220  QFile::exists(name) = QFileInfo(name).exists(). A relative name is resolved
// against this thread's current directory (see setCurrent).
bool fileExists(const String& p)
{
    const Entry e = fileInfo(p);
    if (e.filePath.isEmpty()) return false;
    const String native = convertSeparators(absoluteName(e));
    return GetFileAttributesW(win32Path(native.u16()).c_str()) != INVALID_FILE_ATTRIBUTES;   // long paths
}

// @qt 0x670ab980  QDir::isRelativePath = QFileInfo(p).isRelative()
bool isRelativePath(const String& p) { return fileInfo(p).isRelative(); }

// @qt 0x670ac560  QDir::currentPath (this thread's directory, see setCurrent)
String currentPath()
{
    if (tCurrentSet) return tCurrent;
    return processCurrentPath();
}

bool setCurrent(const String& p)
{
    if (p.isEmpty()) return false;
    const Entry e = fileInfo(p);
    const String abs = absoluteName(e);
    const String native = convertSeparators(abs);
    const DWORD attr = GetFileAttributesW(win32Path(native.u16()).c_str());   // long paths
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) return false;
    // GetCurrentDirectoryW reports the directory GetFullPathNameW makes of it, without a trailing
    // separator except at a drive root.
    String full = nativeAbsoluteFilePath(native);
    std::u16string t = full.u16();
    if (t.size() > 3 && t.back() == u'\\') t.pop_back();
    if (t.size() > 1 && t[1] == u':') t[0] = upper(t[0]);
    tCurrent = fromNativeEntryPath(String(std::move(t)));
    tCurrentSet = true;
    return true;
}

CurrentDirScope::CurrentDirScope(const String& dir) : hadPrev_(tCurrentSet), prev_(tCurrent)
{
    ok_ = setCurrent(dir);
}

CurrentDirScope::~CurrentDirScope()
{
    tCurrentSet = hadPrev_;
    tCurrent = prev_;
}

}  // namespace m2m::qt

namespace m2m::app {
namespace {

// QChar('.') + s (0x1400ab0d0)
String dotPlus(const String& s) { return String(std::u16string(1, u'.')) + s; }

// QString::endsWith(s, Qt::CaseInsensitive), Qt 4.8 null/empty rules
bool endsWithCi(const String& s, const String& what)
{
    if (s.isNull()) return what.isNull();
    if (s.size() == 0) return what.size() == 0;
    const int pos = s.size() - what.size();
    if (pos < 0) return false;
    std::u16string a = s.u16().substr(static_cast<size_t>(pos));
    std::u16string b = what.u16();
    for (auto* t : {&a, &b}) {
        if (t->empty()) continue;
        std::u16string f(t->size(), u'\0');
        if (LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, reinterpret_cast<LPCWSTR>(t->data()),
                          static_cast<int>(t->size()), reinterpret_cast<LPWSTR>(f.data()),
                          static_cast<int>(f.size()), nullptr, nullptr, 0) == static_cast<int>(t->size()))
            *t = std::move(f);
    }
    return a == b;
}

}  // namespace

// @src 0x140818e20  ext without one leading '.' (case-sensitive startsWith); null if ext is empty
String stripDot(const String& ext)
{
    String e = ext;
    if (e.isEmpty()) return String();
    if (e.at(0) == u'.') e = String(e.u16().substr(1));   // QString::remove(0, 1)
    return e;
}

// @src 0x140818b50  s.endsWith("." + stripDot(ext), Qt::CaseInsensitive)
bool endsWithSuffix(const String& s, const String& ext)
{
    const String e = stripDot(ext);
    return endsWithCi(s, dotPlus(e));
}

// @src 0x140818d50  changeSuffix(s, ext) = 0x1408192e0(QFileInfo(s).path(), completeBaseName(s), stripDot(ext))
String changeSuffix(const String& s, const String& ext)
{
    const String dir = qt::path(s);
    const String base = qt::completeBaseName(s);   // 0x140819090
    const String e = stripDot(ext);
    return joinBaseSuffix(dir, base, e);
}

// @src 0x1408192e0  cleanPath(dir + sep + completeBaseName(base) + "." + stripDot(ext)). The base
// goes through completeBaseName again, so "a.b" given as base becomes "a".
String joinBaseSuffix(const String& dir, const String& base, const String& ext)
{
    const String b = qt::completeBaseName(base);   // 0x140819090
    const String e = stripDot(ext);
    return qt::cleanPath(dir + qt::kSep + b + u'.' + e);
}

// @src 0x140818bf0  name, or name + "." + stripDot(ext) unless name already ends with that suffix
String appendSuffix(const String& name, const String& ext)
{
    String r = name;
    const String e = stripDot(ext);
    if (!endsWithSuffix(name, e)) r = name + u'.' + e;
    return r;
}

// @src 0x140819590  QFileInfo(db).absolutePath(), used as QDir(...).relativeFilePath(p): REL(p)
String relToDb(const String& dbPath, const String& p)
{
    return qt::relativeFilePath(qt::absolutePath(dbPath), p);
}

}  // namespace m2m::app
