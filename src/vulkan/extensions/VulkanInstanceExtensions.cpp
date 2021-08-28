//
// Created by qaze on 31.07.2021.
//

#include "VulkanInstanceExtensions.h"
#include "../../VulkanFunctions.h"
#include "../doubleCall.h"

namespace Vulkan {
    VulkanInstanceExtensions::VulkanInstanceExtensions(VkInstance instance) : instance(instance) {
        availableExtensions = std::move(getInstanceExtensions());
    }

    std::vector<VkExtensionProperties> VulkanInstanceExtensions::getInstanceExtensions() {
        return doubleCall<VkExtensionProperties>(vkEnumerateInstanceExtensionProperties, nullptr);
    }

    void VulkanInstanceExtensions::loadExtensionFunctions(const std::vector<const char *> &enabledExtensions) {
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)              \
    for(auto & enabledExtension : enabledExtensions) {                              \
        if(std::string(enabledExtension) == std::string(extension)) {               \
            name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);              \
            if(name == nullptr) {                                                   \
                throw std::exception("Could not load Vulkan extension functions");  \
            }                                                                       \
          }                                                                         \
        }

#include "../../ListOfVulkanFunctions.inl"
    }
}