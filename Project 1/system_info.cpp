#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>
#include <algorithm>

std::string executeCommandAndCaptureOutput(const std::string & command) {
    std::string result = "";
#ifdef _WIN32
    FILE * pipe = _popen(command.c_str(), "r");
#else
    FILE * pipe = popen(command.c_str(), "r");
#endif
    if (!pipe) {
        return "ERROR: Could not run command.";
    }
    char buffer[128];
    while(!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
           result += buffer;
        }
    }
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}

int main() {
    std::cout << "--- C++ Compiler and Standard Information ---" << std::endl;

#ifdef __cplusplus
    std::cout << "C++ Standard: ";
    if (__cplusplus == 199711L) std::cout << "C++98";
    else if (__cplusplus == 201103L) std::cout << "C++11";
    else if (__cplusplus == 201402L) std::cout << "C++14";
    else if (__cplusplus == 201703L) std::cout << "C++17";
    else if (__cplusplus == 202002L) std::cout << "C++20";
    else if (__cplusplus > 202002L) std::cout << "C++23 or later"; 
    else std::cout << "Unknown (" << __cplusplus << ")";
    std::cout << std::endl;
#endif

#ifdef _MSC_VER
    std::cout << "Compiler: Microsoft Visual C++ " << _MSC_VER << std::endl;
#elif __GNUC__
    std::cout << "Compiler: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
#elif __clang__
    std::cout << "Compiler: Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << std::endl;
#else
    std::cout << "Compiler: Unknown " << std::endl;
#endif

    std::cout << "\n--- Operating System Information ---" << std::endl;

#ifdef _WIN32
    std::cout << "OS Name: " << executeCommandAndCaptureOutput("wmic os get Caption /value | find \"=\"") << std::endl;
    std::cout << "OS Version: " << executeCommandAndCaptureOutput("wmic os get Version /value | find \"=\"") << std::endl;
    std::cout << "OS Architecture: " << executeCommandAndCaptureOutput("wmic os get OSArchitecture /value | find \"=\"") << std::endl;
    std::cout << "Hostname: " << executeCommandAndCaptureOutput("hostname") << std::endl;
    std::cout << "Username: " << executeCommandAndCaptureOutput("echo %USERNAME%") << std::endl;

    std::cout << "\n--- CPU Information ---" << std::endl;
    std::cout << "CPU Model: " << executeCommandAndCaptureOutput("wmic cpu get Name /value | find \"=\"") << std::endl;
    std::cout << "Cores: " << executeCommandAndCaptureOutput("wmic cpu get NumberOfCores /value | find \"=\"") << std::endl;
    std::cout << "Logical Processors: " << executeCommandAndCaptureOutput("wmic cpu get NumberOfLogicalProcessors /value | find \"=\"") << std::endl;

    std::cout << "\n--- Memory Information ---" << std::endl;
    std::cout << "Total Physical Memory: " << executeCommandAndCaptureOutput("wmic ComputerSystem get TotalPhysicalMemory /value | find \"=\"") << " bytes" << std::endl;

#else
    std::cout << "OS Info (uname): " << executeCommandAndCaptureOutput("uname -a") << std::endl;

    std::string os_release_info = executeCommandAndCaptureOutput("cat /etc/os-release 2>/dev/null | grep PRETTY_NAME | cut -d '=' -f2 | tr -d '\"'");
    if (!os_release_info.empty()) {
        std::cout << "Pretty OS Name: " << os_release_info << std::endl;
    }

    std::cout << "Hostname: " << executeCommandAndCaptureOutput("hostname") << std::endl;

    const char * user = getenv("USER");
    if (user) {
        std::cout << "Username: " << user << std::endl;
    } else {
        std::cout << "Username: Not found" << std::endl;
    }

    std::cout << "\n--- CPU Information ---" << std::endl;
    #ifdef __APPLE__
        std::cout << "CPU Model: " << executeCommandAndCaptureOutput("sysctl -n machdep.cpu.brand_string") << std::endl;
        std::cout << "Cores: " << executeCommandAndCaptureOutput("sysctl -n machdep.cpu.core_count") << std::endl;
    #else
        std::cout << "CPU Model: " << executeCommandAndCaptureOutput("grep 'model name' /proc/cpuinfo | head -n 1 | cut -d ':' -f2 | sed 's/^*//'") << std::endl;
        std::cout << "Cores: " << executeCommandAndCaptureOutput("grep 'cpu cores' /proc/cpuinfo | head -n 1 | cut -d ':' -f2 | sed 's/^*//'") << std::endl;
        std::cout << "Logical Processors: " << executeCommandAndCaptureOutput("grep '^processor' /proc/cpuinfo | wc -l") << std::endl;
    #endif

    std::cout << "\n--- Memory Information ---" << std::endl;
    #ifdef __APPLE__
        std::cout << "Total Physical Memory: " << executeCommandAndCaptureOutput("sysctl -n hw.memsize") << " bytes" << std::endl;
    #else
        std::cout << "Total Physical Memory: " << executeCommandAndCaptureOutput("grep MemTotal /proc/meminfo | awk '{print $2, $3}'") << std::endl;
    #endif

#endif

    std::cin.get();
    return 0;
}