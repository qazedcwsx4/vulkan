//
// Created by qaze on 15.08.2021.
//

#include "VulkanSwapchain.h"
#include "doubleCall.h"

namespace Vulkan {
    VulkanSwapchain::VulkanSwapchain(VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR presentMode,
                                     VkSurfaceCapabilitiesKHR capabilities) {

    }

    uint32_t VulkanSwapchain::determineSwapchainImageCount(VkSurfaceCapabilitiesKHR &capabilities) {
        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }
        return imageCount;
    }

    VkExtent2D VulkanSwapchain::determineSwapchainImageSize(VkSurfaceCapabilitiesKHR &capabilities) {
        VkExtent2D imageSize = {640, 480};
        if (0xFFFFFFFF == capabilities.currentExtent.width) {
            if (imageSize.width < capabilities.minImageExtent.width) {
                imageSize.width = capabilities.minImageExtent.width;
            } else if (imageSize.width > capabilities.maxImageExtent.width) {
                imageSize.width = capabilities.maxImageExtent.width;
            }

            if (imageSize.height < capabilities.minImageExtent.height) {
                imageSize.height = capabilities.minImageExtent.height;
            } else if (imageSize.height > capabilities.maxImageExtent.height) {
                imageSize.height = capabilities.maxImageExtent.height;
            }
        } else {
            imageSize = capabilities.currentExtent;
        }
        return imageSize;
    }

    VkImageUsageFlags VulkanSwapchain::determineSwapchainUsageScenario(VkSurfaceCapabilitiesKHR &capabilities) {
        return 0;
    }

    VkSurfaceTransformFlagBitsKHR
    VulkanSwapchain::determineSwapchainImageTransformation(VkSurfaceCapabilitiesKHR &capabilities) {
        return VK_SURFACE_TRANSFORM_FLAG_BITS_MAX_ENUM_KHR;
    }

    VkSurfaceFormatKHR VulkanSwapchain::determineSwapchainImageFormat(std::vector<VkSurfaceFormatKHR> &capabilities) {
        return VkSurfaceFormatKHR();
    }


}