//
// Created by qaze on 31.07.2021.
//

#ifndef VULKAN_VULKANINSTANCEEXTENSIONS_H
#define VULKAN_VULKANINSTANCEEXTENSIONS_H

#include "VulkanExtensions.h"

namespace Vulkan {
    class VulkanInstanceExtensions : public VulkanExtensions {
    private:
        VkInstance instance;

        static std::vector<VkExtensionProperties> getInstanceExtensions();

    public:
        //TODO handle it with smart pointers
        VulkanInstanceExtensions() = default;

        void loadExtensionFunctions(const std::vector<const char *> &enabledExtensions) override;

        explicit VulkanInstanceExtensions(VkInstance instance);
    };
}


#endif //VULKAN_VULKANINSTANCEEXTENSIONS_H
