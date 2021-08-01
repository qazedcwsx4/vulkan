//
// Created by qaze on 27.07.2021.
//

#include "VulkanLogicalDevice.h"
#include "../VulkanFunctions.h"
#include <set>

namespace VulkanCookbook {
    VulkanLogicalDevice::VulkanLogicalDevice(std::vector<VkPhysicalDevice> &physicalDevices,
                                             const std::vector<const char *> &desiredExtensions) {
        bool initialized = false;
        for (auto &physicalDevice : physicalDevices) {
            auto deviceFeatures = obtainDeviceFeatures(physicalDevice);
            if (!deviceFeatures.has_value()) continue;
            auto queueMap = obtainDeviceQueues(physicalDevice);
            if (!queueMap.has_value()) continue;

            auto device = createLogicalDevice(physicalDevice, queueMap.value(), deviceFeatures.value(), {});
            if (!device.has_value()) continue;

            VulkanDeviceExtensions extensions(physicalDevice, device.value());
            if (!extensions.areExtensionsSupported(desiredExtensions)) continue;

            VkDevice deviceValue = device.value();
            this->device = deviceValue;
            this->queues = std::move(queueMap.value());
            this->extensions = std::move(extensions);
            initialized = true;
            break;
        }
        if (!initialized) throw std::exception("Could not find suitable createdDevice");

        loadDeviceFunctions();
        extensions.loadExtensionFunctions(desiredExtensions);
    }

    std::optional<VkDevice> VulkanLogicalDevice::createLogicalDevice(VkPhysicalDevice physicalDevice,
                                                                     std::map<QueueType, uint32_t> &queueMap,
                                                                     VkPhysicalDeviceFeatures desiredFeatures,
                                                                     const std::vector<const char *> &desiredExtensions) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        std::set<uint32_t> queueIds;
        for (auto &queue : queueMap) {
            queueIds.emplace(queue.second);
        }

        for (auto &queue : queueIds) {
            const float priorities[] = {1.0f};
            queueCreateInfos.push_back({
                                               VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                               nullptr,
                                               0,
                                               queue,
                                               static_cast<uint32_t>(1),
                                               priorities
                                       });
        };

        VkDeviceCreateInfo device_create_info = {
                VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                nullptr,
                0,
                static_cast<uint32_t>(queueCreateInfos.size()),
                queueCreateInfos.data(),
                0,
                nullptr,
                static_cast<uint32_t>(desiredExtensions.size()),
                desiredExtensions.data(),
                &desiredFeatures
        };

        VkDevice device;
        VkResult result = vkCreateDevice(physicalDevice, &device_create_info, nullptr, &device);
        if (result != VK_SUCCESS || device == VK_NULL_HANDLE) {
            return std::nullopt;
        }

        return device;
    }

    std::optional<VkPhysicalDeviceFeatures> VulkanLogicalDevice::obtainDeviceFeatures(VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        if (!deviceFeatures.geometryShader) {
            return std::nullopt;
        } else {
            deviceFeatures = {};
            deviceFeatures.geometryShader = VK_TRUE;
        }
        return deviceFeatures;
    }

    std::optional<std::map<QueueType, uint32_t>> VulkanLogicalDevice::obtainDeviceQueues(
            VkPhysicalDevice physicalDevice) {

        auto queueFamiliesProperties = getQueueFamiliesProperties(physicalDevice);
        auto graphicsQueueIndex = getIndexOfQueueWithDesiredCapabilities(queueFamiliesProperties,
                                                                         VK_QUEUE_GRAPHICS_BIT);
        auto computeQueueIndex = getIndexOfQueueWithDesiredCapabilities(queueFamiliesProperties, VK_QUEUE_COMPUTE_BIT);

        if (!graphicsQueueIndex.has_value() || !computeQueueIndex.has_value()) return std::nullopt;

        return std::map<QueueType, uint32_t>{{QueueType::GRAPHICS, graphicsQueueIndex.value()},
                                             {QueueType::COMPUTE,  computeQueueIndex.value()}};
    }

    std::optional<uint32_t> VulkanLogicalDevice::getIndexOfQueueWithDesiredCapabilities(
            std::vector<VkQueueFamilyProperties> &queueFamilies, VkQueueFlags desiredCapabilities) {

        for (uint32_t index = 0; index < static_cast<uint32_t>(queueFamilies.size()); ++index) {
            if ((queueFamilies[index].queueCount > 0) &&
                ((queueFamilies[index].queueFlags & desiredCapabilities) == desiredCapabilities)) {
                return index;
                //return std::optional<uint32_t>(index);
            }
        }

        return std::nullopt;
    }

    std::vector<VkQueueFamilyProperties>
    VulkanLogicalDevice::getQueueFamiliesProperties(VkPhysicalDevice physical_device) {
        uint32_t queueFamiliesCount = 0;
        std::vector<VkQueueFamilyProperties> queueFamilies;

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamiliesCount, nullptr);
        if (queueFamiliesCount == 0) {
            throw std::exception("Could not get the number of queue families");
        }

        queueFamilies.resize(queueFamiliesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamiliesCount, queueFamilies.data());

        return queueFamilies;
    }

    void VulkanLogicalDevice::loadDeviceFunctions() {
#define DEVICE_LEVEL_VULKAN_FUNCTION(name)                                            \
        name = (PFN_##name)vkGetDeviceProcAddr(device, #name);                        \
        if((name) == nullptr) {                                                       \
          throw std::exception("Could not load device-level Vulkan function");        \
        }

#include "../ListOfVulkanFunctions.inl"
    }

    VulkanLogicalDevice::~VulkanLogicalDevice() {
        if (device) {
            vkDestroyDevice(device, nullptr);
            device = VK_NULL_HANDLE;
        }
    }
}