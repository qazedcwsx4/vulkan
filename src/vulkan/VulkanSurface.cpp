//
// Created by qaze on 02.08.2021.
//

#include "VulkanSurface.h"
#include "../VulkanFunctions.h"
#include <Windows.h>
#include <exception>

namespace VulkanCookbook {
    namespace {
        enum UserMessage {
            USER_MESSAGE_RESIZE = WM_USER + 1,
            USER_MESSAGE_QUIT,
            USER_MESSAGE_MOUSE_CLICK,
            USER_MESSAGE_MOUSE_MOVE,
            USER_MESSAGE_MOUSE_WHEEL
        };
    }

    LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_LBUTTONDOWN:
                PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 0, 1);
                break;
            case WM_LBUTTONUP:
                PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 0, 0);
                break;
            case WM_RBUTTONDOWN:
                PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 1, 1);
                break;
            case WM_RBUTTONUP:
                PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 1, 0);
                break;
            case WM_MOUSEMOVE:
                PostMessage(hWnd, USER_MESSAGE_MOUSE_MOVE, LOWORD(lParam), HIWORD(lParam));
                break;
            case WM_MOUSEWHEEL:
                PostMessage(hWnd, USER_MESSAGE_MOUSE_WHEEL, HIWORD(wParam), 0);
                break;
            case WM_SIZE:
            case WM_EXITSIZEMOVE:
                PostMessage(hWnd, USER_MESSAGE_RESIZE, wParam, lParam);
                break;
            case WM_KEYDOWN:
                if (VK_ESCAPE == wParam) {
                    PostMessage(hWnd, USER_MESSAGE_QUIT, wParam, lParam);
                }
                break;
            case WM_CLOSE:
                PostMessage(hWnd, USER_MESSAGE_QUIT, wParam, lParam);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    VulkanSurface::VulkanSurface(VkInstance instance) : instance(instance) {
        windowInstance = registerClass();
        window = createWindow();

        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
                VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                nullptr,
                0,
                windowInstance,
                window
        };

        VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo,
                                                  nullptr, &surface);
        if (result != VK_SUCCESS || surface == VK_NULL_HANDLE) {
            throw std::exception("Could not create surface");
        }
    }

    HINSTANCE VulkanSurface::registerClass() {
        auto windowInstance = GetModuleHandle(nullptr);

        WNDCLASSEX windowClass = {
                sizeof(WNDCLASSEX),
                CS_HREDRAW | CS_VREDRAW,
                WindowProcedure,
                0,
                0,
                windowInstance,
                nullptr,
                LoadCursor(nullptr, IDC_ARROW),
                (HBRUSH) (COLOR_WINDOW + 1),
                nullptr,
                CLASS_NAME,
                nullptr
        };

        if (!RegisterClassEx(&windowClass)) {
            throw std::exception("Could not register window class");
        }

        return windowInstance;
    }

    HWND VulkanSurface::createWindow() {
        HWND window = CreateWindow(CLASS_NAME, "VulkanCookbook", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 720, nullptr,
                                   nullptr, windowInstance, nullptr);

        if (!window) {
            throw std::exception("Could not create window");
        }

        return window;
    }

    VulkanSurface::~VulkanSurface() {
        if (window) {
            DestroyWindow(window);
        }

        if (windowInstance) {
            UnregisterClass(CLASS_NAME, windowInstance);
        }

        if (surface) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
            surface = VK_NULL_HANDLE;
        }
    }
}