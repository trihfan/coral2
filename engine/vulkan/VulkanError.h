#pragma once

#include "utils/Logs.h"

#ifndef USE_VALIDATION_LAYERS
#define	VERIFY(R) (R == VK_SUCCESS)
#else
#define VERIFY(R)                                                                                                  \
    [](VkResult result) {                                                                                          \
        switch (result)                                                                                            \
        {                                                                                                          \
        case VK_SUCCESS:                                                                                           \
            break;                                                                                                 \
        case VK_NOT_READY:                                                                                         \
            Logs(error) << "VK_NOT_READY in file " << __FILE__ << " at line " << __LINE__;                         \
            break;                                                                                                 \
        case VK_TIMEOUT:                                                                                           \
            Logs(error) << "VK_TIMEOUT in file " << __FILE__ << " at line " << __LINE__;                           \
            break;                                                                                                 \
        case VK_EVENT_SET:                                                                                         \
            Logs(error) << "VK_EVENT_SET in file " << __FILE__ << " at line " << __LINE__;                         \
            break;                                                                                                 \
        case VK_EVENT_RESET:                                                                                       \
            Logs(error) << "VK_EVENT_RESET in file " << __FILE__ << " at line " << __LINE__;                       \
            break;                                                                                                 \
        case VK_INCOMPLETE:                                                                                        \
            Logs(error) << "VK_INCOMPLETE in file " << __FILE__ << " at line " << __LINE__;                        \
            break;                                                                                                 \
        case VK_ERROR_OUT_OF_HOST_MEMORY:                                                                          \
            Logs(error) << "VK_ERROR_OUT_OF_HOST_MEMORY in file " << __FILE__ << " at line " << __LINE__;          \
            break;                                                                                                 \
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:                                                                        \
            Logs(error) << "VK_ERROR_OUT_OF_DEVICE_MEMORY in file " << __FILE__ << " at line " << __LINE__;        \
            break;                                                                                                 \
        case VK_ERROR_INITIALIZATION_FAILED:                                                                       \
            Logs(error) << "VK_ERROR_INITIALIZATION_FAILED in file " << __FILE__ << " at line " << __LINE__;       \
            break;                                                                                                 \
        case VK_ERROR_DEVICE_LOST:                                                                                 \
            Logs(error) << "VK_ERROR_DEVICE_LOST in file " << __FILE__ << " at line " << __LINE__;                 \
            break;                                                                                                 \
        case VK_ERROR_MEMORY_MAP_FAILED:                                                                           \
            Logs(error) << "VK_ERROR_MEMORY_MAP_FAILED in file " << __FILE__ << " at line " << __LINE__;           \
            break;                                                                                                 \
        case VK_ERROR_LAYER_NOT_PRESENT:                                                                           \
            Logs(error) << "VK_ERROR_LAYER_NOT_PRESENT in file " << __FILE__ << " at line " << __LINE__;           \
            break;                                                                                                 \
        case VK_ERROR_EXTENSION_NOT_PRESENT:                                                                       \
            Logs(error) << "VK_ERROR_EXTENSION_NOT_PRESENT in file " << __FILE__ << " at line " << __LINE__;       \
            break;                                                                                                 \
        case VK_ERROR_FEATURE_NOT_PRESENT:                                                                         \
            Logs(error) << "VK_ERROR_FEATURE_NOT_PRESENT in file " << __FILE__ << " at line " << __LINE__;         \
            break;                                                                                                 \
        case VK_ERROR_INCOMPATIBLE_DRIVER:                                                                         \
            Logs(error) << "VK_ERROR_INCOMPATIBLE_DRIVER in file " << __FILE__ << " at line " << __LINE__;         \
            break;                                                                                                 \
        case VK_ERROR_TOO_MANY_OBJECTS:                                                                            \
            Logs(error) << "VK_ERROR_TOO_MANY_OBJECTS in file " << __FILE__ << " at line " << __LINE__;            \
            break;                                                                                                 \
        case VK_ERROR_FORMAT_NOT_SUPPORTED:                                                                        \
            Logs(error) << "VK_ERROR_FORMAT_NOT_SUPPORTED in file " << __FILE__ << " at line " << __LINE__;        \
            break;                                                                                                 \
        case VK_ERROR_FRAGMENTED_POOL:                                                                             \
            Logs(error) << "VK_ERROR_FRAGMENTED_POOL in file " << __FILE__ << " at line " << __LINE__;             \
            break;                                                                                                 \
        case VK_ERROR_SURFACE_LOST_KHR:                                                                            \
            Logs(error) << "VK_ERROR_SURFACE_LOST_KHR in file " << __FILE__ << " at line " << __LINE__;            \
            break;                                                                                                 \
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:                                                                    \
            Logs(error) << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR in file " << __FILE__ << " at line " << __LINE__;    \
            break;                                                                                                 \
        case VK_SUBOPTIMAL_KHR:                                                                                    \
            Logs(error) << "VK_SUBOPTIMAL_KHR in file " << __FILE__ << " at line " << __LINE__;                    \
            break;                                                                                                 \
        case VK_ERROR_OUT_OF_DATE_KHR:                                                                             \
            Logs(error) << "VK_ERROR_OUT_OF_DATE_KHR in file " << __FILE__ << " at line " << __LINE__;             \
            break;                                                                                                 \
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:                                                                    \
            Logs(error) << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR in file " << __FILE__ << " at line " << __LINE__;    \
            break;                                                                                                 \
        case VK_ERROR_VALIDATION_FAILED_EXT:                                                                       \
            Logs(error) << "VK_ERROR_VALIDATION_FAILED_EXT in file " << __FILE__ << " at line " << __LINE__;       \
            break;                                                                                                 \
        case VK_ERROR_INVALID_SHADER_NV:                                                                           \
            Logs(error) << "VK_ERROR_INVALID_SHADER_NV in file " << __FILE__ << " at line " << __LINE__;           \
            break;                                                                                                 \
        case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:                                                                      \
            Logs(error) << "VK_ERROR_OUT_OF_POOL_MEMORY_KHR in file " << __FILE__ << " at line " << __LINE__;      \
            break;                                                                                                 \
        case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:                                                                 \
            Logs(error) << "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR in file " << __FILE__ << " at line " << __LINE__; \
            break;                                                                                                 \
        case VK_ERROR_NOT_PERMITTED_EXT:                                                                           \
            Logs(error) << "VK_ERROR_NOT_PERMITTED_EXT in file " << __FILE__ << " at line " << __LINE__;           \
            break;                                                                                                 \
        default:                                                                                                   \
            Logs(error) << "Unknow VkResult, method in file " << __FILE__ << " at line " << __LINE__;              \
            break;                                                                                                 \
        }                                                                                                          \
        return result == VK_SUCCESS;                                                                               \
    }(R)
#endif
