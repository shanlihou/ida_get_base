#pragma once
#include <vector>
#include <string>
#include <windef.h>

using std::string;
using std::vector;
namespace StackParser {
    const vector<string> explode(const string& s, string c);
    DWORD getAddrByStr(const string& s);
    string cs2s(const CString& s);
    CString s2cs(const string& s);
    string nospace(const char* s);
    string hexBuf(char* s);
}