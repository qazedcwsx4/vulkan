cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
project(vulkan)

if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "debug")
endif ()

if (NOT DEFINED VK_USE_PLATFORM)
    if (WIN32)
        set(VK_USE_PLATFORM "WIN32")
    elseif (UNIX)
        set(VK_USE_PLATFORM "XCB")
    endif ()
endif ()

if (VK_USE_PLATFORM STREQUAL "VK_USE_PLATFORM_WIN32_KHR")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MT")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(PLATFORM_LIBRARY "")
elseif (VK_USE_PLATFORM STREQUAL "XCB")
    add_definitions(-std=c++11)
    set(PLATFORM_LIBRARY dl xcb)
elseif (VK_USE_PLATFORM STREQUAL "XLIB")
    add_definitions(-std=c++11)
    set(PLATFORM_LIBRARY dl X11)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "debug")
    add_definitions(-D_DEBUG)
endif ()

add_definitions(-DVK_NO_PROTOTYPES)
add_definitions(-DVK_USE_PLATFORM_${VK_USE_PLATFORM}_KHR)

file(GLOB EXTERNAL_HEADER_FILES "External/*.h")
source_group("External" FILES ${EXTERNAL_HEADER_FILES})

file(GLOB_RECURSE HEADER_FILES "src/*.h" "src/*.inl")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(vulkan ${EXTERNAL_HEADER_FILES} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(vulkan ${PLATFORM_LIBRARY})
target_include_directories(vulkan PUBLIC "External")
