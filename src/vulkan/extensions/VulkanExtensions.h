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
    protected:
        std::vector<VkExtensionProperties> availableExtensions;

    public:
        bool isExtensionSupported(const char *extension);

        bool areExtensionsSupported(const std::vector<const char *> &desiredExtensions);

        virtual void loadExtensionFunctions(const std::vector<const char *> &enabledExtensions) = 0;
    };
}

#endif //VULKAN_VULKANEXTENSIONS_H
