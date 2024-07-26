cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(VulkanTest VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Manually find GLM
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
if(NOT GLM_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find GLM")
endif()

# Include directories
include_directories(${GLFW_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS} ${GLM_INCLUDE_DIR})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${Vulkan_LIBRARIES} dl pthread X11 Xxf86vm Xrandr Xi)

# Configure Debug Option
option(ENABLE_DEBUG "Enable debugging" ON)
if(ENABLE_DEBUG)
  message("Debug mode enabled")
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
else()
  message("Debug mode disabled")
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

