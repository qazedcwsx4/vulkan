//
// Created by qaze on 21.07.2021.
//

#ifndef VULKAN_VULKANINSTANCE_H
#define VULKAN_VULKANINSTANCE_H


#include <vulkan.h>
#include <vector>
#include "extensions/VulkanExtensions.h"
#include "VulkanLogicalDevice.h"
#include "extensions/VulkanInstanceExtensions.h"
#include "VulkanSurface.h"

namespace VulkanCookbook {
    class VulkanInstance {
    private:
        VkInstance instance;

        VulkanInstanceExtensions extensions;

        VulkanInstance(const char *applicationName, std::vector<char const *> const &desiredExtensions);

        void loadInstanceFunctions();

        std::vector<VkPhysicalDevice> enumeratePhysicalDevices();

        static VkInstance
        createInstance(const char *applicationName, std::vector<char const *> const &desiredExtensions);

    public:
        ~VulkanInstance();

        VulkanLogicalDevice getLogicalDevice(const std::vector<const char *> &desiredExtensions, VulkanSurface &surface);

        VulkanSurface getSurface();

        friend class VulkanLibrary;
    };
}

#endif //VULKAN_VULKANINSTANCE_H
