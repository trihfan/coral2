#pragma once

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
        r8g8b8a8u,
        depth24_pencil8
    };

    /**
     * @brief Creation parameters for the texture
     */
    struct BackendResourceParams
    {
        BackendResourceType type = BackendResourceType::texture2d;
        BackendResourceFormat format = BackendResourceFormat::r8g8b8a8u;
        int samples = 1;
        int width;
        int height;
    };

    /**
     * @brief The BackendResource class is an interface for a gpu resource
     */
    class BackendResource
    {
    public:
        BackendResource(const BackendResourceParams& params);
        virtual ~BackendResource() = default;

        virtual void bind() = 0;

        const BackendResourceParams& getParams() const;

    protected:
        const backend::BackendResourceParams params;
    };
}
