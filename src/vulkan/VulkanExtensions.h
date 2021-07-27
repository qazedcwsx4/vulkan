//
// Created by qaze on 26.07.2021.
//

#ifndef VULKAN_VULKANEXTENSIONS_H
#define VULKAN_VULKANEXTENSIONS_H

#include <vector>
#include <vulkan.h>
#include <string>

namespace VulkanCookbook {
    class VulkanExtensions {
    private:
        std::vector<VkExtensionProperties> availableExtensions;

        VulkanExtensions();

        static std::vector<VkExtensionProperties> getInstanceExtensions();

        void loadExtensionFunctions(const std::vector<const char *> &enabledExtensions, VkInstance &instance);

    public:
        bool isExtensionSupported(const char *extension);

        bool areExtensionsSupported(const std::vector<const char *> &desired_extensions);

        friend class VulkanInstance;
    };
}

#endif //VULKAN_VULKANEXTENSIONS_H
