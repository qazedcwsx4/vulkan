//
// Created by qaze on 18.07.2021.
//

#ifndef VULKAN_VULKANLIBRARY_H
#define VULKAN_VULKANLIBRARY_H

#define LIBRARY_TYPE HMODULE

#include <Windows.h>
#include <vulkan.h>
#include "VulkanInstance.h"
#include <vector>
#include <string>

namespace VulkanCookbook {
    class VulkanLibrary {
    private:
        LIBRARY_TYPE vulkanLibrary;

        void loadExportedFunctions();

        void loadGlobalFunctions();

    public:
        VulkanLibrary();

        ~VulkanLibrary();

        VulkanInstance getInstance(const char *application_name, std::vector<char const *> const &desiredExtensions);
    };
}

#endif //VULKAN_VULKANLIBRARY_H
