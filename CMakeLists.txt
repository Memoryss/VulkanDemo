cmake_minimum_required(VERSION 2.8.12)

project (VULKAN_DEMO)

set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/external)

find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)

add_subdirectory(src)