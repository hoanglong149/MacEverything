#pragma once
#include <string>

namespace me {
/// Case-insensitive lowercasing: ASCII fast-path + CoreFoundation Unicode fallback.
std::string toLower(const std::string& s);
/// Canonical normalization (NFC/NFD) for Unicode path comparison.
/// macOS stores filenames in either form; normalize before comparing.
std::string normalizeNFC(const std::string& s);
std::string normalizeNFD(const std::string& s);
}
