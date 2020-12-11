#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

namespace coral
{
    class Filesystem
    {
    public:
        static std::string getPath(const std::string& path);
    };
}
#endif
