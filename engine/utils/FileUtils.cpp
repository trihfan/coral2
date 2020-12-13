#include <fstream>
#include "FileUtils.h"
#include "utils/Logs.h"

using namespace coral;

std::filesystem::path FileUtils::getAbsolutePath(const std::filesystem::path& path)
{
    return path;
}

std::string FileUtils::readAll(const std::filesystem::path& path)
{
    std::wifstream file(path);
    if (!file.is_open())
    {
        Logs(warning) << "can't open " << path;
        return "";
    }

    return "";
}