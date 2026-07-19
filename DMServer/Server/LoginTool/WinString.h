// WinString.h - 字符串转换工具（替代 CString）
// 项目字符集为 MultiByte，Win32 API 以 ANSI 形式调用
// 内部统一使用 std::string（GBK/ANSI 编码）
#pragma once
#include "stdafx.h"

namespace winstr {

// UTF-8 → ANSI (GBK) 窄字符串
inline std::string Utf8ToAnsi(const std::string& utf8)
{
	if (utf8.empty()) return "";
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	if (wlen <= 0) return utf8;
	std::wstring wide(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wide[0], wlen);

	int alen = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, NULL, 0, NULL, NULL);
	if (alen <= 0) return utf8;
	std::string ansi(alen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &ansi[0], alen, NULL, NULL);
	// 去除末尾 '\0'
	if (!ansi.empty() && ansi.back() == '\0') ansi.pop_back();
	return ansi;
}

// ANSI (GBK) → UTF-8
inline std::string AnsiToUtf8(const std::string& ansi)
{
	if (ansi.empty()) return "";
	int wlen = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, NULL, 0);
	if (wlen <= 0) return ansi;
	std::wstring wide(wlen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, &wide[0], wlen);

	int ulen = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, NULL, 0, NULL, NULL);
	if (ulen <= 0) return ansi;
	std::string utf8(ulen, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &utf8[0], ulen, NULL, NULL);
	if (!utf8.empty() && utf8.back() == '\0') utf8.pop_back();
	return utf8;
}

// 窄字符串 → 宽字符串（用于需要 Unicode API 的场景）
inline std::wstring ToWide(const std::string& narrow)
{
	if (narrow.empty()) return L"";
	int len = MultiByteToWideChar(CP_ACP, 0, narrow.c_str(), -1, NULL, 0);
	std::wstring wide(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, narrow.c_str(), -1, &wide[0], len);
	if (!wide.empty() && wide.back() == L'\0') wide.pop_back();
	return wide;
}

// 宽字符串 → 窄字符串
inline std::string ToNarrow(const std::wstring& wide)
{
	if (wide.empty()) return "";
	int len = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, NULL, 0, NULL, NULL);
	std::string narrow(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &narrow[0], len, NULL, NULL);
	if (!narrow.empty() && narrow.back() == '\0') narrow.pop_back();
	return narrow;
}

// C 风格字符串安全拷贝
template<size_t N>
inline void SafeCopy(char (&dst)[N], const std::string& src)
{
	size_t len = src.size() < (N - 1) ? src.size() : (N - 1);
	memcpy(dst, src.c_str(), len);
	dst[len] = '\0';
}

// 格式化字符串（替代 CString::Format）
template<typename... Args>
inline std::string Format(const char* fmt, Args... args)
{
	int len = snprintf(nullptr, 0, fmt, args...);
	if (len <= 0) return "";
	std::string result(len + 1, '\0');
	snprintf(&result[0], result.size(), fmt, args...);
	if (!result.empty() && result.back() == '\0') result.pop_back();
	return result;
}

// 格式化字符串（宽字符版本，用于 SetWindowText 等）
template<typename... Args>
inline std::wstring FormatW(const wchar_t* fmt, Args... args)
{
	int len = _snwprintf(nullptr, 0, fmt, args...);
	if (len <= 0) return L"";
	std::wstring result(len + 1, L'\0');
	_snwprintf(&result[0], result.size(), fmt, args...);
	if (!result.empty() && result.back() == L'\0') result.pop_back();
	return result;
}

} // namespace winstr
