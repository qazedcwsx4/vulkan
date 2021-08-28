//
// Created by qaze on 18.07.2021.
//

#include <exception>
#include "VulkanLibrary.h"
#include "../VulkanFunctions.h"

namespace Vulkan {
    VulkanLibrary::VulkanLibrary() {
#if defined _WIN32
        vulkanLibrary = LoadLibrary("vulkan-1.dll");
#elif defined __linux
        vulkanLibrary = dlopen("libvulkan.so.1", RTLD_NOW);
#endif
        if (vulkanLibrary == nullptr) throw std::exception("Could not connect with a Vulkan Runtime library");
        loadExportedFunctions();
        loadGlobalFunctions();
    }

    VulkanLibrary::~VulkanLibrary() {
#if defined _WIN32
        FreeLibrary(vulkanLibrary);
#elif defined __linux
        dlclose(vulkanLibrary);
#endif
        vulkanLibrary = nullptr;
    }

    void VulkanLibrary::loadExportedFunctions() {
#if defined _WIN32
#define LoadFunction GetProcAddress
#elif defined __linux
#define LoadFunction dlsym
#endif

#define EXPORTED_VULKAN_FUNCTION(name)                              \
    name = (PFN_##name)LoadFunction(vulkanLibrary, #name);         \
    if(name == nullptr) throw std::exception("Could not load exported Vulkan function");

#include "../ListOfVulkanFunctions.inl"
    }

    void VulkanLibrary::loadGlobalFunctions() {
#define GLOBAL_LEVEL_VULKAN_FUNCTION(name)                              \
    name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );           \
    if(name == nullptr) throw std::exception("Could not load global level Vulkan function");

#include "../ListOfVulkanFunctions.inl"
    }

    VulkanInstance VulkanLibrary::getInstance(
            const char *application_name,
            const std::vector<const char *> &desiredExtensions) {
        return VulkanInstance(application_name, desiredExtensions);
    }
}