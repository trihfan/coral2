#include <fstream>
#include <sstream>
#include "FileUtils.h"
#include "Logs.h"

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