//
// Created by qaze on 31.07.2021.
//

#include <vector>
#include "VulkanDeviceExtensions.h"
#include "../../VulkanFunctions.h"

namespace VulkanCookbook {
    VulkanDeviceExtensions::VulkanDeviceExtensions(VkPhysicalDevice physicalDevice, VkDevice device) :
            physicalDevice(physicalDevice), device(device) {
        availableExtensions = std::move(getDeviceExtensions());
    }

    std::vector<VkExtensionProperties> VulkanDeviceExtensions::getDeviceExtensions() {
        uint32_t extensionsCount = 0;
        std::vector<VkExtensionProperties> availableExtensions;

        VkResult result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);
        if (result != VK_SUCCESS || extensionsCount == 0) {
            throw std::exception("Could not get the number of device extensions");
        }

        availableExtensions.resize(extensionsCount);
        result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount,
                                                      availableExtensions.data());
        if (result != VK_SUCCESS || extensionsCount == 0) {
            throw std::exception("Could not enumerate device availableExtensions");
        }

        return availableExtensions;
    }

    void VulkanDeviceExtensions::loadExtensionFunctions(const std::vector<const char *> &enabledExtensions) {
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)            \
    for(auto & enabledExtension :enabledExtensions) {                           \
      if(std::string(enabledExtension) == std::string(extension)) {             \
        name = (PFN_##name)vkGetDeviceProcAddr(device, #name);                  \
        if(name == nullptr) {                                                   \
          throw std::exception("Could not load device-level Vulkan function");  \
        }                                                                       \
      }                                                                         \
    }

#include "../../ListOfVulkanFunctions.inl"
    }
}