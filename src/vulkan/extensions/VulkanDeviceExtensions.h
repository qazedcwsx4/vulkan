//
// Created by qaze on 31.07.2021.
//

#ifndef VULKAN_VULKANDEVICEEXTENSIONS_H
#define VULKAN_VULKANDEVICEEXTENSIONS_H

#include <vector>
#include "VulkanExtensions.h"

namespace Vulkan {
    class VulkanDeviceExtensions : public VulkanExtensions {
    private:
        VkPhysicalDevice physicalDevice;

        VkDevice device;

        std::vector<VkExtensionProperties> getDeviceExtensions();

    public:
        //TODO handle it with smart pointers
        VulkanDeviceExtensions() = default;

        explicit VulkanDeviceExtensions(VkPhysicalDevice physicalDevice, VkDevice device);

        void loadExtensionFunctions(const std::vector<const char *> &enabledExtensions) override;
    };
}

#endif //VULKAN_VULKANDEVICEEXTENSIONS_H
