#include <stdio.h>
#include <shlobj_core.h>

/*
To update the Folders customization (Explorer can be somewhat lazy when it comes to update these changes: it uses the IconCache.db storage when not otherwise instructed) we PInvoke SHGetSetFolderCustomSettings.
This Shell API creates or updates the desktop.ini file, located inside each customized folder. This file references the Icon used as the Folder Custom Icon.
This function allows a swift update of the folder new aspect. The update is, in this case, immediate. No need to delete the icons cache or restart Explorer.

C#: https://stackoverflow.com/questions/51799742/is-it-possible-to-create-a-program-that-can-redirect-folder-icon-directory-depen
*/

// refreshFolderIcon "폴더 경로"

int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf("도움말 출력\n");
        return 0;
    }

    char* folderPath = argv[1];
    char* param = argv[2];

    printf("args: %s, %s\n", folderPath, param);

    return folderSetting();
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

int folderSetting() {

    const char* iconPath = "C:\\Program Files (x86)\\Common Files\\TortoiseOverlays\\icons\\XPStyle\\ModifiedIcon.ico";
    const char* folderPath = "C:\\Users\\pdi10\\Desktop\\callDll\\새로운 프로젝트1";
    const int iconIndex = 0;

    // %systemroot%\System32\shell32.dll
    HMODULE shell32_dll = LoadLibrary(L"Shell32.dll");

    SHFOLDERCUSTOMSETTINGS fcs = { 0 };
    fcs.dwSize = sizeof(SHFOLDERCUSTOMSETTINGS);
    fcs.dwMask = FCSM_ICONFILE;
    fcs.pszIconFile = iconPath;
    fcs.cchIconFile = 0;
    fcs.iIconIndex = iconIndex;

    const HRESULT result = SHGetSetFolderCustomSettings(&fcs, folderPath, FCS_FORCEWRITE);

    FreeLibrary(shell32_dll);

    // printf("dwSize: %d\n", fcs.dwSize);
    printf("result: %d\n", result);
    return 0;
}
