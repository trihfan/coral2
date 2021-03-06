#pragma once
#include <vector>

namespace backend
{
    /**
     * @brief Texture type
     */
    enum class BackendResourceType
    {
        texture2d
    };

    /**
     * @brief Texture internal format
     */
    enum class BackendResourceFormat
    {
        r8,
        r8g8,
        r8g8b8,
        r8g8b8a8,
        depth24_pencil8
    };

    /**
     * @brief Creation parameters for the texture
     */
    struct BackendResourceParams
    {
        BackendResourceType type;
        BackendResourceFormat format;
        int width;
        int height;
        int samples = 1;
        unsigned char* data = nullptr; // optional data
    };

    /**
     * @brief The BackendResource class is an interface for a gpu resource
     */
    class BackendResource
    {
    public:
        BackendResource(const BackendResourceParams& params);
        virtual ~BackendResource() = default;

        virtual void bind(int index) = 0;

        const BackendResourceParams& getParams() const;

    protected:
        const backend::BackendResourceParams params;
    };
}
