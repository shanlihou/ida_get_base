#include "pch.h"
namespace StackParser 
{
	const vector<string> explode(const string& s, string c)
	{
		string buff{ "" };
		vector<string> v;
		int cur = 0;
		while (1) {
			int pos = s.find(c, cur);
			if (pos != -1) {
				v.emplace_back(s.substr(cur, pos - cur));
				cur = pos + c.size();
			}
			else {
				v.emplace_back(s.substr(cur));
				break;
			}

		}

		return v;
	}


	DWORD getAddrByStr(const string& s)
	{
		DWORD ea;
		sscanf_s(s.c_str(), "%x", &ea);
		return ea;
	}

	string cs2s(const CString& s) {
		return string((CStringA)s);
	}

	CString s2cs(const string& s) {
		return CString(s.c_str());
	}

	string nospace(const char* s) {
		string ret;
		while (*s != '\0') {
			if (*s != ' ' && *s != '\t') {
				ret += *s;
			}
			s++;
		}
		return ret;
	}

	string hexBuf(char *s) {
		string ret;
		char _buf[10];
		while (*s != '\0') {
			sprintf_s(_buf, "%02x ", *s);
			ret += _buf;
			s++;
		}
		return ret;
	}
}