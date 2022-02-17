//
// Created by qaze on 15.08.2021.
//

#ifndef VULKAN_VULKANSWAPCHAIN_H
#define VULKAN_VULKANSWAPCHAIN_H


#include <vulkan.h>
#include <vector>

namespace Vulkan {
    class VulkanSwapchain {
    public:
        explicit VulkanSwapchain(VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR presentMode, VkSurfaceCapabilitiesKHR capabilities);

    private:
        static uint32_t determineSwapchainImageCount(VkSurfaceCapabilitiesKHR &capabilities);

        static VkExtent2D determineSwapchainImageSize(VkSurfaceCapabilitiesKHR &capabilities);

        static VkImageUsageFlags determineSwapchainUsageScenario(VkSurfaceCapabilitiesKHR &capabilities);

        static VkSurfaceTransformFlagBitsKHR determineSwapchainImageTransformation(VkSurfaceCapabilitiesKHR &capabilities);

        static VkSurfaceFormatKHR determineSwapchainImageFormat(std::vector<VkSurfaceFormatKHR> &capabilities);
    };
}


#endif //VULKAN_VULKANSWAPCHAIN_H
