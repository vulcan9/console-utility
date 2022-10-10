// refreshFolderIcon.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <stdio.h>
//#include <string>
#include <cstring>  // for wcscpy_s, wcscat_s
//#include <cstdlib>  // for _countof
#include <iostream> // wcout

// ���ڿ� ��ȯ
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

// refreshFolderIcon "���� ���" "������ ���" "���� Info(tip)"

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
        // refreshFolderIcon "���� ���" "������ ���" "���� Info(tip)"
        printf("����: %s\n", "v1.0.0");
        printf("����: %s\n", "refreshFolderIcon \"���� ���\" \"������ ���\" \"���� Info(tip)\"");
        return 0;
    }
    
    // �ܼ�â�� �ѱ� ���� ����
    //wcout.imbue(std::locale("korean"));
    wcout.imbue(std::locale(""));

    // ����� �μ� ó��
    wstring _folder = L"";
    wstring _icon = L"";
    wstring _info = L"";
    if (argc >= 2) convert_ansi_to_unicode_string(_folder, argv[1], strlen(argv[1]));
    if (argc >= 3) convert_ansi_to_unicode_string(_icon, argv[2], strlen(argv[2]));
    if (argc >= 4) convert_ansi_to_unicode_string(_info, argv[3], strlen(argv[3]));

    return folderSetting(_folder, _icon, _info);
}

// ���α׷� ����: <Ctrl+F5> �Ǵ� [�����] > [��������� �ʰ� ����] �޴�
// ���α׷� �����: <F5> Ű �Ǵ� [�����] > [����� ����] �޴�

// ������ ���� ��: 
//   1. [�ַ�� Ž����] â�� ����Ͽ� ������ �߰�/�����մϴ�.
//   2. [�� Ž����] â�� ����Ͽ� �ҽ� ��� �����մϴ�.
//   3. [���] â�� ����Ͽ� ���� ��� �� ��Ÿ �޽����� Ȯ���մϴ�.
//   4. [���� ���] â�� ����Ͽ� ������ ���ϴ�.
//   5. [������Ʈ] > [�� �׸� �߰�]�� �̵��Ͽ� �� �ڵ� ������ ����ų�, [������Ʈ] > [���� �׸� �߰�]�� �̵��Ͽ� ���� �ڵ� ������ ������Ʈ�� �߰��մϴ�.
//   6. ���߿� �� ������Ʈ�� �ٽ� ������ [����] > [����] > [������Ʈ]�� �̵��ϰ� .sln ������ �����մϴ�.
