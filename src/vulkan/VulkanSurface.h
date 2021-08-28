//
// Created by qaze on 02.08.2021.
//

#ifndef VULKAN_VULKANSURFACE_H
#define VULKAN_VULKANSURFACE_H

#include <vulkan.h>

namespace Vulkan {
    class VulkanSurface {
    private:
        explicit VulkanSurface(VkInstance instance);

        const char *const CLASS_NAME = "VulkanClass";

        VkInstance instance;

        HINSTANCE windowInstance;

        HWND window;

        VkSurfaceKHR surface;

        HINSTANCE registerClass();

        HWND createWindow();

    public:
        ~VulkanSurface();

        friend class VulkanInstance;
    };
}

#endif //VULKAN_VULKANSURFACE_H
