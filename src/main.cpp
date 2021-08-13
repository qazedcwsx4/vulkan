#include <Windows.h>
#include <iostream>
#include <vulkan.h>
#include "vector"
#include "vulkan/VulkanLibrary.h"
#include "VulkanFunctions.h"

using namespace VulkanCookbook;

int main() {
    const std::vector<const char*> extensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
    auto library = VulkanLibrary();
    auto instance = library.getInstance("XD", extensions);
    auto surface = instance.getSurface();
    auto device = instance.getLogicalDevice({}, surface);

    std::cout << "uwu";
}
