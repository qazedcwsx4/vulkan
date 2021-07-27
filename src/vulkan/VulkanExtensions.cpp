//
// Created by qaze on 26.07.2021.
//

#include "VulkanExtensions.h"
#include <utility>
#include <algorithm>
#include "../VulkanFunctions.h"

namespace VulkanCookbook {
    VulkanExtensions::VulkanExtensions() {
        availableExtensions = std::move(getInstanceExtensions());
    }

    std::vector<VkExtensionProperties> VulkanExtensions::getInstanceExtensions() {
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

    bool VulkanExtensions::isExtensionSupported(const char *const extension) {
        for (auto &available_extension : availableExtensions) {
            if (strstr(available_extension.extensionName, extension)) {
                return true;
            }
        }
        return false;
    }

    bool VulkanExtensions::areExtensionsSupported(const std::vector<const char *> &desired_extensions) {
        for (auto extension: desired_extensions) {
            if (!isExtensionSupported(extension)) return false;
        }
        return true;
    }


    void VulkanExtensions::loadExtensionFunctions(const std::vector<const char *> &enabledExtensions, VkInstance &instance) {
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)              \
    for(auto & enabledExtension : enabledExtensions) {                              \
        if(std::string(enabledExtension) == std::string(extension)) {               \
            name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);              \
            if(name == nullptr) {                                                   \
                throw std::exception("Could not load Vulkan extension functions");  \
            }                                                                       \
          }                                                                         \
        }

#include "../ListOfVulkanFunctions.inl"
    }
}