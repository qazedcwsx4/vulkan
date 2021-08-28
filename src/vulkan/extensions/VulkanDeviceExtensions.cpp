//
// Created by qaze on 31.07.2021.
//

#include <vector>
#include "VulkanDeviceExtensions.h"
#include "../../VulkanFunctions.h"
#include "../doubleCall.h"

namespace Vulkan {
    VulkanDeviceExtensions::VulkanDeviceExtensions(VkPhysicalDevice physicalDevice, VkDevice device) :
            physicalDevice(physicalDevice), device(device) {
        availableExtensions = std::move(getDeviceExtensions());
    }

    std::vector<VkExtensionProperties> VulkanDeviceExtensions::getDeviceExtensions() {
        return doubleCall<VkExtensionProperties>(vkEnumerateDeviceExtensionProperties, physicalDevice, nullptr);
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