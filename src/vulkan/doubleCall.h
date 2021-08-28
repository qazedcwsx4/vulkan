//
// Created by qaze on 15.08.2021.
//

#ifndef VULKAN_DOUBLECALL_H
#define VULKAN_DOUBLECALL_H

#include <vector>
#include <functional>

namespace Vulkan {
    template<typename T, typename ... Args1, typename ... Args2>
    std::vector<T> doubleCall(void (*fun)(Args1 ...), Args2 ... args) {
        uint32_t size = 0;
        std::bind(fun, args..., &size, nullptr)();
        if (size == 0) {
            throw std::exception("vulkan call failed");
        }
        std::vector<T> vector(size);
        std::bind(fun, args..., &size, vector.data())();
        return vector;
    }

    template<typename T, typename ... Args1, typename ... Args2>
    std::vector<T> doubleCall(VkResult (*fun)(Args1 ...), Args2 ... args) {
        uint32_t size = 0;
        VkResult result;
        result = std::bind<VkResult>(fun, args..., &size, nullptr)();
        if (result != VK_SUCCESS || size == 0) {
            throw std::exception("vulkan call failed");
        }
        std::vector<T> vector(size);
        result = std::bind<VkResult>(fun, args..., &size, vector.data())();
        if (result != VK_SUCCESS) {
            throw std::exception("vulkan call failed");
        }
        return vector;
    }
}

#endif //VULKAN_DOUBLECALL_H