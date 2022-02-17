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

namespace Vulkan {
    class VulkanLogicalDevice {
    private:
        VkPhysicalDevice physicalDevice;

        VkDevice device;

        std::map<QueueType, uint32_t> queues;

        VulkanDeviceExtensions extensions;

        explicit VulkanLogicalDevice(std::vector<VkPhysicalDevice> &physicalDevices,
                                     std::vector<char const *> const &desiredExtensions,
                                     VkSurfaceKHR surface);

        static std::optional<std::map<QueueType, uint32_t>>
        obtainDeviceQueues(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

        static std::optional<VkPhysicalDeviceFeatures> obtainDeviceFeatures(VkPhysicalDevice physicalDevice);

        static std::optional<uint32_t> getIndexOfQueueWithDesiredCapabilities(
                std::vector<VkQueueFamilyProperties> &queueFamilies, VkQueueFlags desiredCapabilities);

        static std::optional<VkDevice> createLogicalDevice(VkPhysicalDevice physicalDevice, std::map<QueueType,
                uint32_t> &queueMap, VkPhysicalDeviceFeatures desiredFeatures, std::vector<char const *> const &desiredExtensions);

        static std::vector<VkQueueFamilyProperties> getQueueFamiliesProperties(VkPhysicalDevice physicalDevice);

        static std::optional<uint32_t> getIndexOfQueueWithPresentationSurfaceSupport(
                std::vector<VkQueueFamilyProperties> &queueFamilies,
                VkPhysicalDevice physicalDevice,
                VkSurfaceKHR surface);

        void loadDeviceFunctions();

        VkPresentModeKHR determinePresentationMode(VkPresentModeKHR presentMode, VkSurfaceKHR surface);

        VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkSurfaceKHR surface);

        std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkSurfaceKHR surface);

    public:
        ~VulkanLogicalDevice();

        friend class VulkanInstance;
    };
}


#endif //VULKAN_VULKANLOGICALDEVICE_H
