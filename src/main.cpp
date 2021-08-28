#include <Windows.h>
#include <iostream>
#include <vulkan.h>
#include "vector"
#include "vulkan/VulkanLibrary.h"
#include "VulkanFunctions.h"

using namespace Vulkan;

int main() {
    try {
        const std::vector<const char *> extensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
        auto library = VulkanLibrary();
        auto instance = library.getInstance("XD", extensions);
        auto surface = instance.getSurface();
        auto device = instance.getLogicalDevice({VK_KHR_SWAPCHAIN_EXTENSION_NAME}, surface);
    } catch (std::exception &e) {
        std::cerr << "encountered exception\n" << e.what();
        exit(1);
    }

    std::cout << "uwu";
}
