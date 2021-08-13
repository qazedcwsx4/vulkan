//
// Created by qaze on 21.07.2021.
//

#include <vector>
#include "VulkanInstance.h"
#include "../VulkanFunctions.h"

namespace VulkanCookbook {

    VulkanInstance::VulkanInstance(const char *applicationName, std::vector<char const *> const &desiredExtensions) :
            instance(createInstance(applicationName, desiredExtensions)),
            extensions(VulkanInstanceExtensions(instance)) {

        if (!extensions.areExtensionsSupported(desiredExtensions)) {
            throw std::exception("Not all availableExtensions are supported");
        }

        loadInstanceFunctions();
        extensions.loadExtensionFunctions(desiredExtensions);
    }

    VkInstance
    VulkanInstance::createInstance(const char *applicationName, std::vector<char const *> const &desiredExtensions) {
        VkInstance instance;

        VkApplicationInfo application_info = {
                VK_STRUCTURE_TYPE_APPLICATION_INFO,
                nullptr,
                applicationName,
                VK_MAKE_VERSION(1, 0, 0),
                "Vulkan Cookbook",
                VK_MAKE_VERSION(1, 0, 0),
                VK_MAKE_VERSION(1, 0, 0)
        };

        VkInstanceCreateInfo instance_create_info = {
                VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                nullptr,
                0,
                &application_info,
                0,
                nullptr,
                static_cast<uint32_t>(desiredExtensions.size()),
                desiredExtensions.data()
        };

        VkResult result = vkCreateInstance(&instance_create_info, nullptr, &instance);
        if (result != VK_SUCCESS || instance == VK_NULL_HANDLE) {
            throw std::exception("Was not able to create vulkan instance");
        }

        return instance;
    }

    void VulkanInstance::loadInstanceFunctions() {
#define INSTANCE_LEVEL_VULKAN_FUNCTION(name)                                        \
        name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);                  \
        if(name == nullptr) {                                                       \
          throw std::exception("Could not load instance-level Vulkan functions");   \
        }

#include "../ListOfVulkanFunctions.inl"
    }

    VulkanInstance::~VulkanInstance() {
        if (instance) {
            vkDestroyInstance(instance, nullptr);
            instance = VK_NULL_HANDLE;
        }
    }

    VulkanLogicalDevice VulkanInstance::getLogicalDevice(const std::vector<const char *> &desiredExtensions, VulkanSurface &surface) {
        std::vector<VkPhysicalDevice> availableDevices = enumeratePhysicalDevices();
        return VulkanLogicalDevice(availableDevices, desiredExtensions, surface.surface);
    }

    std::vector<VkPhysicalDevice> VulkanInstance::enumeratePhysicalDevices() {
        uint32_t devicesCount = 0;
        std::vector<VkPhysicalDevice> availableDevices;

        VkResult result = vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr);
        if (result != VK_SUCCESS || devicesCount == 0) {
            throw std::exception("Could not get the number of available physical devices");
        }

        availableDevices.resize(devicesCount);
        result = vkEnumeratePhysicalDevices(instance, &devicesCount, availableDevices.data());
        if (result != VK_SUCCESS || devicesCount == 0) {
            throw std::exception("Could not enumerate physical devices");
        }
        return availableDevices;
    }

    VulkanSurface VulkanInstance::getSurface() {
        return VulkanSurface(instance);
    }
}