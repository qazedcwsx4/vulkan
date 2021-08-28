//
// Created by qaze on 26.07.2021.
//

#include "VulkanExtensions.h"
#include <utility>
#include <algorithm>
#include "../../VulkanFunctions.h"

namespace Vulkan {
    bool VulkanExtensions::isExtensionSupported(const char *const extension) {
        for (auto &available_extension : availableExtensions) {
            if (strstr(available_extension.extensionName, extension)) {
                return true;
            }
        }
        return false;
    }

    bool VulkanExtensions::areExtensionsSupported(const std::vector<const char *> &desiredExtensions) {
        for (auto extension: desiredExtensions) {
            if (!isExtensionSupported(extension)) return false;
        }
        return true;
    }
}