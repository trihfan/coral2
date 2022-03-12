#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>
#include <string>

namespace coral
{
    class FileUtils
    {
    public:
        static std::filesystem::path getAbsolutePath(const std::filesystem::path& path);
        static std::string readAll(const std::filesystem::path& path);
        static std::string getAppDirectory();
    };
}
#endif
