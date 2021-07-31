//
// Created by qaze on 27.07.2021.
//

#ifndef VULKAN_VULKANLOGICALDEVICE_H
#define VULKAN_VULKANLOGICALDEVICE_H


#include <vulkan.h>
#include <vector>
#include <optional>
#include <map>
#include "QueueType.h"
#include "extensions/VulkanDeviceExtensions.h"

namespace VulkanCookbook {
    class VulkanLogicalDevice {
    private:
        VkDevice device;

        std::map<QueueType, uint32_t> queues;

        VulkanDeviceExtensions extensions;

        explicit VulkanLogicalDevice(std::vector<VkPhysicalDevice> &physicalDevices,
                                     std::vector<char const *> const &desired_extensions);

        static std::optional<std::map<QueueType, uint32_t>> obtainDeviceQueues(VkPhysicalDevice physicalDevice);

        static std::optional<VkPhysicalDeviceFeatures> obtainDeviceFeatures(VkPhysicalDevice physicalDevice);

        static std::optional<uint32_t> getIndexOfQueueWithDesiredCapabilities(
                std::vector<VkQueueFamilyProperties> &queueFamilies, VkQueueFlags desiredCapabilities);

        static std::optional<VkDevice> createLogicalDevice(VkPhysicalDevice physicalDevice, std::map<QueueType,
                uint32_t> &queueMap, VkPhysicalDeviceFeatures desiredFeatures, std::vector<char const *> const &desiredExtensions);

        static std::vector<VkQueueFamilyProperties> getQueueFamiliesProperties(VkPhysicalDevice physical_device);

        void loadDeviceFunctions();

    public:
        ~VulkanLogicalDevice();

        friend class VulkanInstance;
    };
}


#endif //VULKAN_VULKANLOGICALDEVICE_H
