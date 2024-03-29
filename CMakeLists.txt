cmake_minimum_required(VERSION 3.5)

project(CppTimer VERSION 1.0.0 HOMEPAGE_URL https://github.com/Humminghead/CppTimer LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost)

message(NOTICE "Boost libraries was found. Version:" ${Boost_VERSION})

file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

set(THREAD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/thread/src/pthread/thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/thread/src/pthread/once.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/thread/src/future.cpp
  )

add_library(${PROJECT_NAME} STATIC ${CXX_FILES} ${THREAD_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/thread/include
)

# Define install path
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${PROJECT_NAME} )
set(CMAKE_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${PROJECT_NAME} )

# Print install path
message(VERBOSE "Install includes in: " ${INCLUDE_INSTALL_DIR})
message(VERBOSE "Install cmake file in: " ${CMAKE_INSTALL_DIR})

# Create configuration files
configure_package_config_file( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                               "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                               INSTALL_DESTINATION
                               ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
                               PATH_VARS INCLUDE_INSTALL_DIR CMAKE_INSTALL_DIR
                               )

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

# Install configuration files
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DIR})

# Install target
install(FILES ${H_FILES} DESTINATION ${INCLUDE_INSTALL_DIR})
install(TARGETS ${PROJECT_NAME} LIBRARY)
