#include <stdio.h>
#include <string>
#include <cstring>  // for wcscpy_s, wcscat_s
#include <cstdlib>  // for _countof
#include <shlobj_core.h>

#include <codecvt>
#include <iostream>

#include <locale>
#include <clocale>
#include <vector>
#include "convert_string.h"

using namespace std;

/*
To update the Folders customization (Explorer can be somewhat lazy when it comes to update these changes: it uses the IconCache.db storage when not otherwise instructed) we PInvoke SHGetSetFolderCustomSettings.
This Shell API creates or updates the desktop.ini file, located inside each customized folder. This file references the Icon used as the Folder Custom Icon.
This function allows a swift update of the folder new aspect. The update is, in this case, immediate. No need to delete the icons cache or restart Explorer.

C#: https://stackoverflow.com/questions/51799742/is-it-possible-to-create-a-program-that-can-redirect-folder-icon-directory-depen
*/

// refreshFolderIcon "폴더 경로" "아이콘 경로" "폴더 Info(tip)"

wchar_t* toUnicode(const char* multibyte) {
    wchar_t unicode[_MAX_FNAME] = { 0, };


    int nLen = MultiByteToWideChar(CP_ACP, 0, multibyte, strlen(multibyte), NULL, NULL);
    MultiByteToWideChar(CP_ACP, 0, multibyte, strlen(multibyte), unicode, nLen);

    //MultiByteToWideChar(CP_ACP, 0, multibyte, strlen(multibyte) + 1, unicode, strlen(multibyte) + 1);

    return unicode;
}

//int folderSetting(char* folder, char* icon, char* info) {
int folderSetting1(string folder, string icon, string info) {

    //string strMulti = info;
    //int nLen = MultiByteToWideChar(CP_ACP, 0, &strMulti[0], strMulti.size(), NULL, NULL);
    //wstring strUni(nLen, 0);
    //MultiByteToWideChar(CP_ACP, 0, &strMulti[0], strMulti.size(), &strUni[0], nLen);

    //printf("info2: %s\n", strMulti);

    // LP: 포인터. C: const. W: unicode, T: 멀티바이트. STR: char.

    /*
    wstring folder_temp = s2w(folder);
    PCWSTR folderPath = folder_temp.c_str();

    wstring icon_temp = s2w(icon);
    LPCWSTR iconPath = icon_temp.c_str();

    wstring info_temp = s2w(info);
    LPCWSTR infoTip = info_temp.c_str();
    */

    /*
    wchar_t _folderPath[_MAX_FNAME] = { 0, };
    //wcscpy_s(_folderPath, _MAX_FNAME, L"C:\\Users\\pdi10\\Desktop\\callDll\\새로운 프로젝트2");
    wcscpy_s(_folderPath, _MAX_FNAME, toUnicode(folder.c_str()));
    PCWSTR folderPath = _folderPath;

    wchar_t _iconPath[_MAX_FNAME] = { 0, };
    //wcscpy_s(_iconPath, _MAX_FNAME, L".jjproj\\projectFolder.ico");
    //wcscpy_s(_iconPath, _MAX_FNAME, L"C:\\Program Files (x86)\\Common Files\\TortoiseOverlays\\icons\\XPStyle\\ModifiedIcon.ico");
    wcscpy_s(_iconPath, _MAX_FNAME, toUnicode(icon.c_str()));
    LPWSTR iconPath = _iconPath;

    wchar_t _info[_MAX_FNAME] = { 0, };
    wcscpy_s(_info, _MAX_FNAME, toUnicode(info.c_str()));
    LPWSTR infoTip = _info;
    
    wprintf(L"folder: %s\n", folderPath);
    wprintf(L"icon: %s\n", iconPath);
    printf("info: %s\n", infoTip);
    */








    wstring _folderPath = L"";
    convert_ansi_to_unicode_string(_folderPath, folder.c_str(), folder.size());
    PCWSTR folderPath = _folderPath.c_str();
    wcout << "PCWSTR is : " << folderPath << endl;


    wstring _icon = L"";
    convert_ansi_to_unicode_string(_icon, icon.c_str(), icon.size());
    //std::wcout << "unicode is : " << _icon.c_str() << std::endl;
    // LPCWSTR --> LPWSTR
    //LPCWSTR iconPath = _iconPath.c_str();
    wchar_t _iconPath[_MAX_FNAME];
    wcscpy_s(_iconPath, _countof(_iconPath), _icon.c_str());
    LPWSTR iconPath = _iconPath;
    wcout << "LPWSTR is : " << iconPath << endl;

    wstring _info = L"";
    convert_ansi_to_unicode_string(_info, info.c_str(), info.size());
    //std::wcout << "unicode is : " << _info.c_str() << std::endl;
    // LPCWSTR --> LPWSTR
    wchar_t _infoTip[1024];
    wcscpy_s(_infoTip, _info.c_str());
    LPWSTR infoTip = _infoTip;
    wcout << "LPWSTR is : " << infoTip << endl;










    const int iconIndex = 0;

    // %systemroot%\System32\shell32.dll
    //HMODULE shell32_dll = LoadLibrary(L"Shell32.dll");

    SHFOLDERCUSTOMSETTINGS fcs = { 0 };
    fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
    fcs.dwMask = FCSM_ICONFILE | FCSM_INFOTIP;
    fcs.pszIconFile = iconPath;
    fcs.cchIconFile = 0;
    fcs.iIconIndex = iconIndex;
    fcs.pszInfoTip = infoTip;
    fcs.cchInfoTip = 0;

    const HRESULT result = SHGetSetFolderCustomSettings(&fcs, folderPath, FCS_FORCEWRITE);

    //FreeLibrary(shell32_dll);

    printf("C++ result: %d\n", result);
    return 0;
}

// [C++] string ↔ wstring 변환
// https://stormpy.tistory.com/288
// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
string w2s(const wstring& var){
    static locale loc("");
    auto& facet = use_facet<codecvt<wchar_t, char, mbstate_t>>(loc);
    return wstring_convert<remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

wstring s2w(const string& var)
{
    static locale loc("");
    auto& facet = use_facet<codecvt<wchar_t, char, mbstate_t>>(loc);
    return wstring_convert<remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(var);
}


wstring StringToWideString(const string& str) {
    if (str.empty()) return wstring();

    size_t pos;
    size_t begin = 0;
    wstring ret;

    int size = 0;
    pos = str.find(static_cast<char>(0), begin);
    while (pos != std::string::npos) {
        std::string segment = std::string(&str[begin], pos - begin);
        std::wstring converted = std::wstring(segment.size() + 1, 0);
        size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.length());
        converted.resize(size);
        ret.append(converted);
        ret.append({ 0 });
        begin = pos + 1;
        pos = str.find(static_cast<char>(0), begin);
    }
    if (begin < str.length()) {
        std::string segment = std::string(&str[begin], str.length() - begin);
        std::wstring converted = std::wstring(segment.size() + 1, 0);
        size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, segment.c_str(), segment.size(), &converted[0], converted.length());
        converted.resize(size);
        ret.append(converted);
    }
    return ret;
}

int strTest(string info) {

    // https://stormpy.tistory.com/288
    //string info = "한글 한글";
    


    printf("info: %s\n", info.c_str());
    cout << "info : " << info << endl;


    wstring unicode = L"";
    convert_ansi_to_unicode_string(unicode, info.c_str(), info.size());
    std::wcout << "unicode is : " << unicode.c_str() << std::endl;

    LPCWSTR lwsresult = unicode.c_str();
    wcout << "LPCWSTR is : " << lwsresult << endl;

    wstring unicode2 = L"";
    convert_ansi_to_unicode_string(unicode2, info.c_str(), info.size());
    PCWSTR wsresult = unicode2.c_str();
    wcout << "PCWSTR is : " << wsresult << endl;

    /*
    wstring wstemp = s2w(info);
    LPCWSTR wsresult = wstemp.c_str();
    wprintf(L"LPCWSTR: %s\n", wsresult);
    wcout << "LPCWSTR is : " << wsresult << endl;


    string stemp = w2s(wsresult);
    printf("stemp: %s\n", stemp);
    cout << "stemp is : " << stemp << endl;
    //*/


    //wstring temp = wstring(info.begin(), info.end());
    //LPCWSTR wideString = temp.c_str();
    //cout << "wideString is : " << wideString << endl;

    //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    //const wstring ws = convert.from_bytes(info);


    //std::wstring wide(L"한글 Wide");
    //setlocale(LC_ALL, "");

    //const wstring ws = L"한글 Wide";
    //const wstring ws = StringToWideString(info);
    //LPCWSTR lpws = ws.c_str();
    //cout << "lpws is : " << ws.c_str() << endl;

    /*
    const locale locale("");
    typedef codecvt<wchar_t, char, std::mbstate_t> converter_type;
    const converter_type& converter = use_facet<converter_type>(locale);
    vector<char> to(ws.length() * converter.max_length());
    mbstate_t state;
    const wchar_t* from_next;
    char* to_next;
    const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
    if (result == converter_type::ok or result == converter_type::noconv) {
        const std::string s(&to[0], to_next);
        std::cout << "std::string =     " << s << std::endl;
    }
    */




    //std::string str(wide.begin(), wide.end());
    //std::cout << str << std::endl;

    return 0;
}



















int folderSetting(wstring _folder, wstring _icon, wstring _info) {
    /*
    wcout << "_folder is : " << _folder << endl;
    wcout << "_icon is : " << _icon << endl;
    wcout << "_info is : " << _info << endl;
    */

    PCWSTR folderPath = _folder.c_str();
    wcout << "PCWSTR is : " << folderPath << endl;

    // LPCWSTR --> LPWSTR
    wchar_t iconPath[_MAX_FNAME];
    wcscpy_s(iconPath, _icon.c_str());
    wcout << "LPWSTR is : " << iconPath << endl;

    // LPCWSTR --> LPWSTR
    wchar_t infoTip[1024];
    wcscpy_s(infoTip, _info.c_str());
    wcout << "LPWSTR is : " << infoTip << endl;

    const int iconIndex = 0;

    // %systemroot%\System32\shell32.dll
    //HMODULE shell32_dll = LoadLibrary(L"Shell32.dll");

    SHFOLDERCUSTOMSETTINGS fcs = { 0 };
    fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
    fcs.dwMask = FCSM_ICONFILE | FCSM_INFOTIP;
    fcs.pszIconFile = iconPath;
    fcs.cchIconFile = 0;
    fcs.iIconIndex = iconIndex;
    fcs.pszInfoTip = infoTip;
    fcs.cchInfoTip = 0;

    const HRESULT result = SHGetSetFolderCustomSettings(&fcs, folderPath, FCS_FORCEWRITE);

    //FreeLibrary(shell32_dll);
    return result;
}

int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf("도움말 출력\n");
        return 0;
    }
    
    // 콘솔창에 한글 깨짐 보정
    //wcout.imbue(std::locale("korean"));
    wcout.imbue(std::locale(""));

    wstring _folder = L"";
    wstring _icon = L"";
    wstring _info = L"";
    if (argc >= 2) convert_ansi_to_unicode_string(_folder, argv[1], strlen(argv[1]));
    if (argc >= 3) convert_ansi_to_unicode_string(_icon, argv[2], strlen(argv[2]));
    if (argc >= 4) convert_ansi_to_unicode_string(_info, argv[3], strlen(argv[3]));

    return folderSetting(_folder, _icon, _info);


    //*
    string folder = "";
    string icon = "";
    string info = "";
    
    // const char* 전달 (문자열을 char 배열로 변환)
    if (argc >= 2) folder = argv[1];
    if (argc >= 3) icon = argv[2];
    if (argc >= 4) info = argv[3];

    //printf("folder: %s\n", folder);
    //printf("icon: %s\n", icon);
    //printf("strMulti: %s\n", info);
    //*/


    // char* 전달
    return folderSetting1(folder, icon, info);
    // const char* 전달 (문자열을 char 배열로 변환)
    //return folderSetting(&folder[0], &icon[0], &info[0]);
}


// https://stackoverflow.com/questions/16945892/custom-folder-icons-with-desktop-ini-instant-refreshing
// https://learn.microsoft.com/ko-kr/windows/win32/api/shlobj_core/nf-shlobj_core-shgetsetfoldercustomsettings
// SHFOLDERCUSTOMSETTINGS fcs = {0};
// fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
// fcs.dwMask = FCSM_ICONFILE;
// fcs.pszIconFile = iconPath;
// fcs.cchIconFile = 0;
// fcs.iIconIndex = iconIndex;
// SHGetSetFolderCustomSettings(&fcs, folderPath, FCS_FORCEWRITE);
