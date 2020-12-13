#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <filesystem>

namespace coral
{
    class FileUtils
    {
    public:
        static std::filesystem::path getAbsolutePath(const std::filesystem::path& path);
        static std::string readAll(const std::filesystem::path& path);
    };
}
#endif
