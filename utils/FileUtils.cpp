#include "FileUtils.h"
#include "Logs.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace coral;

std::filesystem::path FileUtils::getAbsolutePath(const std::filesystem::path& path)
{
    return path;
}

std::string FileUtils::readAll(const std::filesystem::path& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        Logs(warning) << "can't open " << path;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

#if defined(_WIN32) || defined(_WIN64)
#include <string>
#include <windows.h>

std::string FileUtils::getAppDirectory()
{
    char result[MAX_PATH];
    return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}

#elif defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>

std::string FileUtils::getAppDirectory()
{
    char buf[PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    if (_NSGetExecutablePath(buf, &bufsize) == 0)
    {
        return std::filesystem::path(std::string(buf)).parent_path();
    }
    return "";
}

#else
#include <limits.h>
#include <string>
#include <unistd.h>

std::string FileUtils::getAppDirectory()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}
#endif