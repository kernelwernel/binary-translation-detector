#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS 1
#define LINUX 0
#define APPLE 0
#elif (defined(__linux__))
#define WINDOWS 0
#define LINUX 1
#define APPLE 0
#elif (defined(__APPLE__) || defined(__APPLE_CPP__) || defined(__MACH__) || defined(__DARWIN))
#define WINDOWS 0
#define LINUX 0
#define APPLE 1
#else
#define WINDOWS 0
#define LINUX 0
#define APPLE 0
#endif

#define u8 uint8_t
#define u32 uint32_t
#define CPU_MANUFACTURER 0x40000000

#if (LINUX) 
#include <cpuid.h>
#elif (WINDOWS)
#include <windows.h>
#include <intrin.h>
#endif


u8 get_windows_version() {
#if (WINDOWS)
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
        return osvi.dwMajorVersion;
    }
#endif

    return 0;
}


bool get_emulation_status_microsoft() {
#if (WINDOWS)
    USHORT process_machine = 0
    USHORT native_machine = 0;

    if (IsWoW64Process2(GetCurrentProcess(), &process_machine, &native_machine)) {
        bool is_emulated = (
            (native_machine == IMAGE_FILE_MACHINE_ARM64) &&
            (
                (process_machine == IMAGE_FILE_MACHINE_AMD64) || // not to be misread as "ARM64"
                (process_machine == IMAGE_FILE_MACHINE_I386) 
            )
        );

        return (is_emulated);
    }
#endif

    return false;
}



bool detect_apple_rosetta() {
    char manufacturer[13];
    manufacturer[12] = '\0';

    u32 cpu_info[4] = {0};
    
#if (WINDOWS)
    __cpuid((int32_t*)cpu_info, CPU_MANUFACTURER);
#elif (LINUX || APPLE)
    __cpuid_count(CPU_MANUFACTURER, 0, cpu_info[0], cpu_info[1], cpu_info[2], cpu_info[3]);
#endif

    *((u32*)manufacturer) = cpu_info[1];
    *((u32*)(manufacturer + 4)) = cpu_info[3];
    *((u32*)(manufacturer + 8)) = cpu_info[2];

    return (strcmp(manufacturer, "VirtualApple") == 0);
}



bool detect_microsoft_prism() {
#if (!WINDOWS)
    return false;
#else
    if (get_windows_version() == 11) {
        return get_emulation_status_microsoft();
    }

    return false;
#endif
}


bool detect_microsoft_x86_to_arm() {
#if (!WINDOWS)
    return false;
#else
    if (get_windows_version() == 10) {
        return get_emulation_status_microsoft();
    }

    return false;
#endif
}



int main() {
    if (detect_apple_rosetta()) {
        printf("Apple Rosetta translator detected\n");
    } else if (detect_microsoft_prism()) {
        printf("Microsoft Prism translator detected\n");
    } else if (detect_microsoft_x86_to_arm()) {
        printf("Microsoft x86-to-ARM (XTA) translator detected\n");
    } else {
        printf("No tranlator found\n");
    }

    return 0;
}