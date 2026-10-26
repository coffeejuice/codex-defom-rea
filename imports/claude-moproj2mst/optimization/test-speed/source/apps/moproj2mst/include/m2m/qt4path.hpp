// m2m/qt4path.hpp: Qt 4.8 path functions on Windows and DEF_GUI_MO's own path helpers
// (seam; owner P05). Rules: s1-02 1.4, s1-01 1 and 4. File-system queries are read-only.
#pragma once

#include "m2m/qt4.hpp"

namespace m2m::qt {

// QDir::separator() on Windows.
inline constexpr char16_t kSep = u'\\';

// QDir::cleanPath (Qt 4.8, Windows): '\\' -> '/', collapse "//", "./", "../", drop a trailing '/'.
// Null or empty input is returned unchanged.
String cleanPath(const String& p);
// QDir::convertSeparators: '/' -> '\\'.
String convertSeparators(const String& p);
// QDir::fromNativeSeparators: '\\' -> '/'.
String fromNativeSeparators(const String& p);
// QDir(dirAbs).relativeFilePath(p) (Qt 4.8, Windows rules: drive and components case-insensitive).
String relativeFilePath(const String& dir, const String& p);
// QFileInfo(p).fileName() / completeBaseName() / baseName() / suffix() / path() / absolutePath()
String fileName(const String& p);
String completeBaseName(const String& p);
String baseName(const String& p);
String suffix(const String& p);
String path(const String& p);
String absolutePath(const String& p);
// QFile::exists(p) (read-only query of the real file system).
bool fileExists(const String& p);
// QDir::isRelativePath(p)
bool isRelativePath(const String& p);

// QDir::currentPath / QDir::setCurrent, per thread (the GUI changes the process directory; the
// DLL keeps one per thread so parallel conversions do not interfere). Without setCurrent on a
// thread, currentPath is the process directory. setCurrent fails (false) if p is not a directory.
String currentPath();
bool setCurrent(const String& p);
// Sets the thread's current directory for its lifetime and restores the previous one after.
class CurrentDirScope {
public:
    explicit CurrentDirScope(const String& dir);
    ~CurrentDirScope();
    CurrentDirScope(const CurrentDirScope&) = delete;
    CurrentDirScope& operator=(const CurrentDirScope&) = delete;
    bool ok() const { return ok_; }

private:
    bool hadPrev_;
    String prev_;
    bool ok_ = false;
};

}  // namespace m2m::qt

namespace m2m::app {
using qt::String;

// @src 0x140818b50  endsWithSuffix(s, ext): strip one leading '.' of ext, s.endsWith("." + ext, ci)
bool endsWithSuffix(const String& s, const String& ext);
// @src 0x140818e20  ext without one leading '.', null if empty
String stripDot(const String& ext);
// @src 0x140818d50  changeSuffix(s, ext) = 0x1408192e0(path(s), completeBaseName(s), ext)
String changeSuffix(const String& s, const String& ext);
// @src 0x1408192e0  cleanPath(dir + sep + completeBaseName(base) + "." + stripDot(ext))
String joinBaseSuffix(const String& dir, const String& base, const String& ext);
// @src 0x140818bf0  name + "." + stripDot(ext) unless name already ends with it (0x140818b50)
String appendSuffix(const String& name, const String& ext);
// @src 0x140819590  QDir(QFileInfo(db).absolutePath()).relativeFilePath(p): REL(p) of s1-02 1.4
// (0x140819590 itself is QFileInfo(db).absolutePath(), i.e. qt::absolutePath)
String relToDb(const String& dbPath, const String& p);

}  // namespace m2m::app
