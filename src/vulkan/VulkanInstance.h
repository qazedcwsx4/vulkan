//
// Created by qaze on 21.07.2021.
//

#ifndef VULKAN_VULKANINSTANCE_H
#define VULKAN_VULKANINSTANCE_H


#include <vulkan.h>
#include <vector>
#include "VulkanExtensions.h"

namespace VulkanCookbook {
    class VulkanInstance {
    private:
        VkInstance instance;

        VulkanExtensions extensions;

        VulkanInstance(const char *application_name, std::vector<char const *> const &desired_extensions);

        void loadInstanceFunctions();
    public:
        ~VulkanInstance();

        friend class VulkanLibrary;
    };
}

#endif //VULKAN_VULKANINSTANCE_H
