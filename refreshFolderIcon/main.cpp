// refreshFolderIcon.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
//#include <string>
#include <cstring>  // for wcscpy_s, wcscat_s
//#include <cstdlib>  // for _countof
#include <iostream> // wcout

// 문자열 변환
#include "convert_string.h"

// SHGetSetFolderCustomSettings
#include <shlobj_core.h>

using namespace std;

/*
To update the Folders customization (Explorer can be somewhat lazy when it comes to update these changes: it uses the IconCache.db storage when not otherwise instructed) we PInvoke SHGetSetFolderCustomSettings.
This Shell API creates or updates the desktop.ini file, located inside each customized folder. This file references the Icon used as the Folder Custom Icon.
This function allows a swift update of the folder new aspect. The update is, in this case, immediate. No need to delete the icons cache or restart Explorer.

C#: https://stackoverflow.com/questions/51799742/is-it-possible-to-create-a-program-that-can-redirect-folder-icon-directory-depen
*/

// refreshFolderIcon "폴더 경로" "아이콘 경로" "폴더 Info(tip)"

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

    /*
    // https://stackoverflow.com/questions/16945892/custom-folder-icons-with-desktop-ini-instant-refreshing
    // https://learn.microsoft.com/ko-kr/windows/win32/api/shlobj_core/nf-shlobj_core-shgetsetfoldercustomsettings
     SHFOLDERCUSTOMSETTINGS fcs = {0};
     fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
     fcs.dwMask = FCSM_ICONFILE;
     fcs.pszIconFile = iconPath;
     fcs.cchIconFile = 0;
     fcs.iIconIndex = iconIndex;
     SHGetSetFolderCustomSettings(&fcs, folderPath, FCS_FORCEWRITE);
    */

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
        // refreshFolderIcon "폴더 경로" "아이콘 경로" "폴더 Info(tip)"
        printf("버전: %s\n", "v1.0.0");
        printf("사용법: %s\n", "refreshFolderIcon \"폴더 경로\" \"아이콘 경로\" \"폴더 Info(tip)\"");
        return 0;
    }
    
    // 콘솔창에 한글 깨짐 보정
    //wcout.imbue(std::locale("korean"));
    wcout.imbue(std::locale(""));

    // 명령행 인수 처리
    wstring _folder = L"";
    wstring _icon = L"";
    wstring _info = L"";
    if (argc >= 2) convert_ansi_to_unicode_string(_folder, argv[1], strlen(argv[1]));
    if (argc >= 3) convert_ansi_to_unicode_string(_icon, argv[2], strlen(argv[2]));
    if (argc >= 4) convert_ansi_to_unicode_string(_info, argv[3], strlen(argv[3]));

    return folderSetting(_folder, _icon, _info);
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
