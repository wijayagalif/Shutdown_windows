#include <iostream>
#include <windows.h>

class SystemController {
public:
    void shutdown() {
        // Token privileges adjustment to enable shutdown
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;

        // Get a token for this process
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
            std::cerr << "OpenProcessToken failed" << std::endl;
            return;
        }

        // Get the LUID for the shutdown privilege
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

        tkp.PrivilegeCount = 1;  // one privilege to set
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        // Get the shutdown privilege for this process
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

        if (GetLastError() != ERROR_SUCCESS) {
            std::cerr << "AdjustTokenPrivileges failed" << std::endl;
            return;
        }

        // Shut down the system and force all applications to close
        if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER)) {
            std::cerr << "ExitWindowsEx failed" << std::endl;
            return;
        }
    }
};

int main() {
    SystemController controller;
    controller.shutdown();
    return 0;
}
