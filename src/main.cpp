#include <Windows.h>
#include <iostream>
#include <vulkan.h>
#include "vector"
#include "vulkan/VulkanLibrary.h"
#include "VulkanFunctions.h"

namespace VulkanCookbook {

    std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

#define LIBRARY_TYPE HMODULE

    bool CheckAvailableInstanceExtensions(std::vector<VkExtensionProperties> &available_extensions) {
        uint32_t extensions_count = 0;
        VkResult result = VK_SUCCESS;

        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr);
        if ((result != VK_SUCCESS) ||
            (extensions_count == 0)) {
            std::cout << "Could not get the number of instance extensions." << std::endl;
            return false;
        }

        available_extensions.resize(extensions_count);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, available_extensions.data());
        if ((result != VK_SUCCESS) ||
            (extensions_count == 0)) {
            std::cout << "Could not enumerate instance extensions." << std::endl;
            return false;
        }

        return true;
    }

    bool IsExtensionSupported(std::vector<VkExtensionProperties> const &available_extensions,
                              char const *const extension) {
        for (auto &available_extension : available_extensions) {
            if (strstr(available_extension.extensionName, extension)) {
                return true;
            }
        }
        return false;
    }

    bool CreateVulkanInstance(std::vector<char const *> const &desired_extensions,
                              char const *const application_name,
                              VkInstance &instance) {
        std::vector<VkExtensionProperties> available_extensions;
        if (!CheckAvailableInstanceExtensions(available_extensions)) {
            return false;
        }

        for (auto &extension : desired_extensions) {
            if (!IsExtensionSupported(available_extensions, extension)) {
                std::cout << "Extension named '" << extension << "' is not supported by an Instance object."
                          << std::endl;
                return false;
            }
        }

        VkApplicationInfo application_info = {
                VK_STRUCTURE_TYPE_APPLICATION_INFO,                 // VkStructureType           sType
                nullptr,                                            // const void              * pNext
                application_name,                                   // const char              * pApplicationName
                VK_MAKE_VERSION(1, 0, 0),                         // uint32_t                  applicationVersion
                "Vulkan Cookbook",                                  // const char              * pEngineName
                VK_MAKE_VERSION(1, 0, 0),                         // uint32_t                  engineVersion
                VK_MAKE_VERSION(1, 0, 0)                          // uint32_t                  apiVersion
        };

        VkInstanceCreateInfo instance_create_info = {
                VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,             // VkStructureType           sType
                nullptr,                                            // const void              * pNext
                0,                                                  // VkInstanceCreateFlags     flags
                &application_info,                                  // const VkApplicationInfo * pApplicationInfo
                0,                                                  // uint32_t                  enabledLayerCount
                nullptr,                                            // const char * const      * ppEnabledLayerNames
                static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                  enabledExtensionCount
                desired_extensions.data()                           // const char * const      * ppEnabledExtensionNames
        };

        VkResult result = vkCreateInstance(&instance_create_info, nullptr, &instance);
        if ((result != VK_SUCCESS) ||
            (instance == VK_NULL_HANDLE)) {
            std::cout << "Could not create Vulkan instance." << std::endl;
            return false;
        }

        return true;
    }

    bool LoadInstanceLevelFunctions(VkInstance instance,
                                    std::vector<char const *> const &enabled_extensions) {
        // Load core Vulkan API instance-level functions
#define INSTANCE_LEVEL_VULKAN_FUNCTION(name)                                  \
    name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );                \
    if( name == nullptr ) {                                                     \
      std::cout << "Could not load instance-level Vulkan function named: "      \
        #name << std::endl;                                                     \
      return false;                                                             \
    }

        // Load instance-level functions from enabled extensions
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)        \
    for( auto & enabled_extension : enabled_extensions ) {                      \
      if( std::string( enabled_extension ) == std::string( extension ) ) {      \
        name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );            \
        if( name == nullptr ) {                                                 \
          std::cout << "Could not load instance-level Vulkan function named: "  \
            #name << std::endl;                                                 \
          return false;                                                         \
        }                                                                       \
      }                                                                         \
    }

#include "ListOfVulkanFunctions.inl"

        return true;
    }

    bool EnumerateAvailablePhysicalDevices(VkInstance instance,
                                           std::vector<VkPhysicalDevice> &available_devices) {
        uint32_t devices_count = 0;
        VkResult result = VK_SUCCESS;

        result = vkEnumeratePhysicalDevices(instance, &devices_count, nullptr);
        if ((result != VK_SUCCESS) ||
            (devices_count == 0)) {
            std::cout << "Could not get the number of available physical devices." << std::endl;
            return false;
        }

        available_devices.resize(devices_count);
        result = vkEnumeratePhysicalDevices(instance, &devices_count, available_devices.data());
        if ((result != VK_SUCCESS) ||
            (devices_count == 0)) {
            std::cout << "Could not enumerate physical devices." << std::endl;
            return false;
        }

        return true;
    }

    bool CheckAvailableDeviceExtensions(VkPhysicalDevice physical_device,
                                        std::vector<VkExtensionProperties> &available_extensions) {
        uint32_t extensions_count = 0;
        VkResult result = VK_SUCCESS;

        result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr);
        if ((result != VK_SUCCESS) ||
            (extensions_count == 0)) {
            std::cout << "Could not get the number of device extensions." << std::endl;
            return false;
        }

        available_extensions.resize(extensions_count);
        result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count,
                                                      available_extensions.data());
        if ((result != VK_SUCCESS) ||
            (extensions_count == 0)) {
            std::cout << "Could not enumerate device extensions." << std::endl;
            return false;
        }

        return true;
    }

    void GetFeaturesAndPropertiesOfPhysicalDevice(VkPhysicalDevice physical_device,
                                                  VkPhysicalDeviceFeatures &device_features,
                                                  VkPhysicalDeviceProperties &device_properties) {
        vkGetPhysicalDeviceFeatures(physical_device, &device_features);

        vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    }

    bool CheckAvailableQueueFamiliesAndTheirProperties(VkPhysicalDevice physical_device,
                                                       std::vector<VkQueueFamilyProperties> &queue_families) {
        uint32_t queue_families_count = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, nullptr);
        if (queue_families_count == 0) {
            std::cout << "Could not get the number of queue families." << std::endl;
            return false;
        }

        queue_families.resize(queue_families_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, queue_families.data());
        if (queue_families_count == 0) {
            std::cout << "Could not acquire properties of queue families." << std::endl;
            return false;
        }

        return true;
    }

    bool SelectIndexOfQueueFamilyWithDesiredCapabilities(VkPhysicalDevice physical_device,
                                                         VkQueueFlags desired_capabilities,
                                                         uint32_t &queue_family_index) {
        std::vector<VkQueueFamilyProperties> queue_families;
        if (!CheckAvailableQueueFamiliesAndTheirProperties(physical_device, queue_families)) {
            return false;
        }

        for (uint32_t index = 0; index < static_cast<uint32_t>(queue_families.size()); ++index) {
            if ((queue_families[index].queueCount > 0) &&
                ((queue_families[index].queueFlags & desired_capabilities) == desired_capabilities)) {
                queue_family_index = index;
                return true;
            }
        }
        return false;
    }

    struct QueueInfo {
        uint32_t FamilyIndex;
        std::vector<float> Priorities;
    };

    bool CreateLogicalDevice(VkPhysicalDevice physical_device,
                             std::vector<QueueInfo> queue_infos,
                             std::vector<char const *> const &desired_extensions,
                             VkPhysicalDeviceFeatures *desired_features,
                             VkDevice &logical_device) {
        std::vector<VkExtensionProperties> available_extensions;
        if (!CheckAvailableDeviceExtensions(physical_device, available_extensions)) {
            return false;
        }

        for (auto &extension : desired_extensions) {
            if (!IsExtensionSupported(available_extensions, extension)) {
                std::cout << "Extension named '" << extension << "' is not supported by a physical device."
                          << std::endl;
                return false;
            }
        }

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

        for (auto &info : queue_infos) {
            queue_create_infos.push_back({
                                                 VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType                  sType
                                                 nullptr,                                          // const void                     * pNext
                                                 0,                                                // VkDeviceQueueCreateFlags         flags
                                                 info.FamilyIndex,                                 // uint32_t                         queueFamilyIndex
                                                 static_cast<uint32_t>(info.Priorities.size()),    // uint32_t                         queueCount
                                                 info.Priorities.data()                            // const float                    * pQueuePriorities
                                         });
        };

        VkDeviceCreateInfo device_create_info = {
                VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // VkStructureType                  sType
                nullptr,                                            // const void                     * pNext
                0,                                                  // VkDeviceCreateFlags              flags
                static_cast<uint32_t>(queue_create_infos.size()),   // uint32_t                         queueCreateInfoCount
                queue_create_infos.data(),                          // const VkDeviceQueueCreateInfo  * pQueueCreateInfos
                static_cast<uint32_t>(validationLayers.size()),                                                  // uint32_t                         enabledLayerCount
                validationLayers.data(),                                            // const char * const             * ppEnabledLayerNames
                static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                         enabledExtensionCount
                desired_extensions.data(),                          // const char * const             * ppEnabledExtensionNames
                desired_features                                    // const VkPhysicalDeviceFeatures * pEnabledFeatures
        };

        VkResult result = vkCreateDevice(physical_device, &device_create_info, nullptr, &logical_device);
        if ((result != VK_SUCCESS) ||
            (logical_device == VK_NULL_HANDLE)) {
            std::cout << "Could not create logical device." << std::endl;
            return false;
        }

        return true;
    }

    bool LoadDeviceLevelFunctions(VkDevice logical_device,
                                  std::vector<char const *> const &enabled_extensions) {
        // Load core Vulkan API device-level functions
#define DEVICE_LEVEL_VULKAN_FUNCTION(name)                                    \
    name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );            \
    if( name == nullptr ) {                                                     \
      std::cout << "Could not load device-level Vulkan function named: "        \
        #name << std::endl;                                                     \
      return false;                                                             \
    }

        // Load device-level functions from enabled extensions
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)          \
    for( auto & enabled_extension : enabled_extensions ) {                      \
      if( std::string( enabled_extension ) == std::string( extension ) ) {      \
        name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );        \
        if( name == nullptr ) {                                                 \
          std::cout << "Could not load device-level Vulkan function named: "    \
            #name << std::endl;                                                 \
          return false;                                                         \
        }                                                                       \
      }                                                                         \
    }

#include "ListOfVulkanFunctions.inl"

        return true;
    }

    void GetDeviceQueue(VkDevice logical_device, uint32_t queue_family_index, uint32_t queue_index, VkQueue &queue) {
        vkGetDeviceQueue(logical_device, queue_family_index, queue_index, &queue);
    }

    bool CreateLogicalDeviceWithGeometryShadersAndGraphicsAndComputeQueues(VkInstance instance,
                                                                           VkDevice &logical_device,
                                                                           VkQueue &graphics_queue,
                                                                           VkQueue &compute_queue) {
        std::vector<VkPhysicalDevice> physical_devices;
        EnumerateAvailablePhysicalDevices(instance, physical_devices);

        for (auto &physical_device : physical_devices) {
            VkPhysicalDeviceFeatures device_features;
            VkPhysicalDeviceProperties device_properties;
            GetFeaturesAndPropertiesOfPhysicalDevice(physical_device, device_features, device_properties);

            if (!device_features.geometryShader) {
                continue;
            } else {
                device_features = {};
                device_features.geometryShader = VK_TRUE;
            }

            uint32_t graphics_queue_family_index;
            if (!SelectIndexOfQueueFamilyWithDesiredCapabilities(physical_device, VK_QUEUE_GRAPHICS_BIT,
                                                                 graphics_queue_family_index)) {
                continue;
            }

            uint32_t compute_queue_family_index;
            if (!SelectIndexOfQueueFamilyWithDesiredCapabilities(physical_device, VK_QUEUE_COMPUTE_BIT,
                                                                 compute_queue_family_index)) {
                continue;
            }

            std::vector<QueueInfo> requested_queues = {{graphics_queue_family_index, {1.0f}}};
            if (graphics_queue_family_index != compute_queue_family_index) {
                requested_queues.push_back({compute_queue_family_index, {1.0f}});
            }

            if (!CreateLogicalDevice(physical_device, requested_queues, {}, &device_features, logical_device)) {
                continue;
            } else {
                if (!LoadDeviceLevelFunctions(logical_device, {})) {
                    return false;
                }
                GetDeviceQueue(logical_device, graphics_queue_family_index, 0, graphics_queue);
                GetDeviceQueue(logical_device, compute_queue_family_index, 0, compute_queue);
                return true;
            }
        }
        return false;
    }

    void DestroyLogicalDevice(VkDevice &logical_device) {
        if (logical_device) {
            vkDestroyDevice(logical_device, nullptr);
            logical_device = VK_NULL_HANDLE;
        }
    }

    void DestroyVulkanInstance(VkInstance &instance) {
        if (instance) {
            vkDestroyInstance(instance, nullptr);
            instance = VK_NULL_HANDLE;
        }
    }

    void ReleaseVulkanLoaderLibrary(LIBRARY_TYPE &vulkan_library) {
        if (nullptr != vulkan_library) {
#if defined _WIN32
            FreeLibrary(vulkan_library);
#elif defined __linux
            dlclose( vulkan_library );
#endif
            vulkan_library = nullptr;
        }
    }


}

using namespace VulkanCookbook;

int main() {
    auto xd = VulkanLibrary();
    std::cout << "uwu";
}
