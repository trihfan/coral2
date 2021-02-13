#ifdef __APPLE__
#include <experimental/memory_resource>
namespace std
{
    namespace pmr = experimental::pmr;
}
#else
#include <memory_resource>
#endif