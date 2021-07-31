//
// Created by qaze on 31.07.2021.
//

#include "VulkanInstanceExtensions.h"
#include "../../VulkanFunctions.h"

namespace VulkanCookbook {
    VulkanInstanceExtensions::VulkanInstanceExtensions(VkInstance instance) : instance(instance) {
        availableExtensions = std::move(getInstanceExtensions());
    }

    std::vector<VkExtensionProperties> VulkanInstanceExtensions::getInstanceExtensions() {
        uint32_t extensions_count = 0;

        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr);
        if (result != VK_SUCCESS || extensions_count == 0) {
            throw std::exception("Could not get the number of instance extensions");
        }

        std::vector<VkExtensionProperties> extensions;
        extensions.resize(extensions_count);

        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, extensions.data());
        if (result != VK_SUCCESS || extensions_count == 0) {
            throw std::exception("Could not enumerate instance extensions");
        }

        return extensions;
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