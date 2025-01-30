cmake_minimum_required(VERSION 3.16)
project(OccViewer)

# Set vcpkg and toolchain configuration first
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

# Enable automatic handling of Qt dependencies
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set paths
set(Qt6_DIR "C:/Qt/6.8.1/mingw_64/lib/cmake/Qt6")
set(OpenCASCADE_DIR "C:/vcpkg/packages/opencascade_x64-windows/share/opencascade")
set(Freetype_DIR "C:/vcpkg/installed/x64-windows/share/freetype")
set(ZLIB_ROOT "C:/vcpkg/installed/x64-windows")

set(CMAKE_PREFIX_PATH 
    "C:/Qt/6.8.1/mingw_64"
    ${OpenCASCADE_DIR}
    ${Freetype_DIR}
    ${ZLIB_ROOT}
    ${CMAKE_PREFIX_PATH}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies in correct order
find_package(ZLIB REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found. Please check your Qt installation.")
endif()
find_package(OpenCASCADE REQUIRED)

# Debug output
message(STATUS "ZLIB_INCLUDE_DIRS: ${ZLIB_INCLUDE_DIRS}")
message(STATUS "ZLIB_LIBRARIES: ${ZLIB_LIBRARIES}")
message(STATUS "Freetype_DIR: ${Freetype_DIR}")
message(STATUS "Freetype_LIBRARIES: ${Freetype_LIBRARIES}")
message(STATUS "OpenCASCADE_INCLUDE_DIR: ${OpenCASCADE_INCLUDE_DIR}")
message(STATUS "OpenCASCADE_LIBRARY_DIR: ${OpenCASCADE_LIBRARY_DIR}")

# Add your executable
add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    occview.cpp
    mainwindow.h
    occview.h
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCASCADE_INCLUDE_DIR}
    ${Freetype_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

# Add platform-specific definitions
add_definitions(
    -DWNT
    -D_CRT_SECURE_NO_WARNINGS
    -DHAVE_WOK_CONFIG_H
    -DHAVE_CONFIG_H
    -DWIN32
    -D_WINDOWS
    -DCSFDB
    -DNDEBUG
)

# Add OpenCASCADE library directory to search path
link_directories(${OpenCASCADE_LIBRARY_DIR})

# Link libraries in correct dependency order
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets

    # Foundation & Base
    TKernel
    TKMath
    
    # Modeling Data
    TKG2d
    TKG3d
    TKGeomBase
    TKBRep
    
    # Modeling Algorithms
    TKGeomAlgo
    TKTopAlgo
    TKShHealing
    TKMesh
    TKPrim
    TKBool
    TKBO
    TKFillet
    TKOffset
    
    # Visualization (in dependency order)
    TKService
    TKGraphic3d
    TKAspect
    TKHLR
    TKMeshVS
    TKV3d
    TKOpenGl
    
    # Application Framework & Visualization
    TKCDF
    TKCAF
    TKLCAF
    TKBin
    TKXml
    TKXSBase
    
    # System Dependencies
    opengl32
    gdi32
    user32
    advapi32
)

# Add post-build command to copy DLLs
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCASCADE_LIBRARY_DIR}/TK*.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
