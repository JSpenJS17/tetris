#include <stdlib.h>
#include <string.h>

#if _WIN32
// idk which of these are actually needed
#include <Windows.h>
#include <Lmcons.h>
#include <io.h>
#include <conio.h>
#include <shlobj.h>
#include <Shlwapi.h>

#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
// for getpwuid
#include <pwd.h>
#include <unistd.h>
#endif 

char *get_username()
{
    static char login[64];
    char *l = NULL;
#ifdef _WIN32
    LPSTR mybuffer;
    DWORD size = UNLEN + 1;
    TCHAR buffer[UNLEN + 1];

    mybuffer = buffer;
    GetUserName(mybuffer, &size);
    l = mybuffer;

#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))

    struct passwd *pw;

    pw = getpwuid(getuid());

    l = pw->pw_name;
#endif

    if ((l == NULL) || (*l == '\0'))
        if ((l = getenv ("USERNAME")) == NULL)
            if ((l = getenv ("LOGNAME")) == NULL)
                if ((l = getenv ("USER")) == NULL)
                {
                    l = (char*)"nobody";
                }

    strncpy (login, l, 64);
    login[63] = '\0';

    return (login);
}
