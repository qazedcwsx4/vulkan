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

namespace VulkanCookbook {
    class VulkanInstance {
    private:
        VkInstance instance;

        VulkanInstanceExtensions extensions;

        VulkanInstance(const char *application_name, std::vector<char const *> const &desired_extensions);

        void loadInstanceFunctions();

        std::vector<VkPhysicalDevice> enumeratePhysicalDevices();

        static VkInstance
        createInstance(const char *application_name, std::vector<char const *> const &desired_extensions);

    public:
        ~VulkanInstance();

        VulkanLogicalDevice getLogicalDevice();

        friend class VulkanLibrary;
    };
}

#endif //VULKAN_VULKANINSTANCE_H
