#include "utils/Logs.h"
#include <boost/type_index.hpp>

namespace coral
{
    template <typename Type>
    class Singleton 
    {
    public:       
        // Deleted functions                                                     
        Singleton(const Singleton&) = delete;                                                        
        Singleton& operator=(const Singleton&) = delete;                                             
        Singleton(Singleton&&) = delete;                                                             
        Singleton& operator=(Singleton&&) = delete;  

        // Create the singleton
        template <typename Type, typename... Args>
        static void create(Args&&... args)
        {                                                     
            assert(!instance);                                                                      
            instance = new Type(std::forward<Args>(args)...);       
            coral::Logs(coral::success) << boost::typeindex::type_id_with_cvr<Type>().pretty_name() << " created";                    
        }

        // Destroy the singleton
        template <typename Type>
        static void destroy()
        {                              
            delete instance;  
            instance = nullptr;                                                   
            coral::Logs(coral::success) << boost::typeindex::type_id_with_cvr<Type>().pretty_name() << " destroyed";                              
        }
    
        // Return the instance
        static Type& get() 
        {
            return *instance;
        }

    protected:
        Singleton() = default;  

    private:
        inline static Type* instance = nullptr;
    };
}