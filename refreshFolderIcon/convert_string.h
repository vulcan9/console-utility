#pragma once
#ifndef CONVERT_STRING_H
#define CONVERT_STRING_H

#include <Windows.h>
#include <string>

DWORD convert_ansi_to_unicode_string(
    __out std::wstring& unicode,
    __in const char* ansi,
    __in const size_t ansi_size
);

DWORD convert_unicode_to_ansi_string(
    __out std::string& ansi,
    __in const wchar_t* unicode,
    __in const size_t unicode_size
);

DWORD convert_unicode_to_utf8_string(
    __out std::string& utf8,
    __in const wchar_t* unicode,
    __in const size_t unicode_size
);

DWORD convert_utf8_to_unicode_string(
    __out std::wstring& unicode,
    __in const char* utf8,
    __in const size_t utf8_size
);

#endif