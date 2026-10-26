// m2m/longpath.hpp: path form for Win32 file calls, so that paths longer than MAX_PATH work
// (owner: P01, G1). Port addition: the original GUI goes through Qt 4.8, which fails on them.
//
// Only the name handed to the OS changes. The path strings the converter computes and writes into
// the MST are never built from this form.
#pragma once

#include <string>

namespace m2m {

// The name to pass to CreateFileW, GetFileAttributesW, MoveFileExW, DeleteFileW or a std::fstream:
// `path` itself when its full path is shorter than 248 characters (the CreateDirectoryW limit, so
// short paths behave exactly as before), else the full path (GetFullPathNameW: separators made
// native, "." and ".." resolved, relative names against the process directory) with the \\?\
// prefix, or \\?\UNC\ for a \\server\share path. A name that already has \\?\ or \\.\ is kept.
std::wstring win32Path(const std::wstring& path);
std::wstring win32Path(const std::u16string& path);

// GetFullPathNameW with a buffer that grows to fit (no MAX_PATH limit); empty on failure.
std::wstring fullPathName(const std::wstring& path);

}  // namespace m2m
