#include <Windows.h>
#include <iostream>
#include <vulkan.h>
#include "vector"
#include "vulkan/VulkanLibrary.h"
#include "VulkanFunctions.h"

using namespace VulkanCookbook;

int main() {
    auto library = VulkanLibrary();
    auto instance = library.getInstance("XD", {});
    auto device = instance.getLogicalDevice();
    std::cout << "uwu";
}
