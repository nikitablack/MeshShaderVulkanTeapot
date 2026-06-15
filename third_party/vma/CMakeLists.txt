cmake_minimum_required(VERSION 3.25)

set(VMA_VERSION v3.3.0)
set(VMA_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/${VMA_VERSION}")

if(EXISTS "${VMA_SRC_DIR}/CMakeLists.txt")
    add_subdirectory("${VMA_SRC_DIR}")
else()
    message(STATUS "Downloading vma ${VMA_VERSION}")

    FetchContent_Declare(
        vma
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
        GIT_TAG ${VMA_VERSION}
        SOURCE_DIR ${VMA_SRC_DIR}
    )

    FetchContent_MakeAvailable(vma)
endif()
