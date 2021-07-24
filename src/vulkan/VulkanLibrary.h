//
// Created by qaze on 18.07.2021.
//

#ifndef VULKAN_VULKANLIBRARY_H
#define VULKAN_VULKANLIBRARY_H

#define LIBRARY_TYPE HMODULE

#include <Windows.h>
#include <vulkan.h>
#include "../VulkanFunctions.h"
namespace VulkanCookbook {
    class VulkanLibrary {
    private:
        LIBRARY_TYPE vulkanLibrary;

        void loadExportedFunctions();

        void loadGlobalFunctions();

    public:
        VulkanLibrary();

        ~VulkanLibrary();
    };
}

#endif //VULKAN_VULKANLIBRARY_H
