cmake_minimum_required(VERSION 3.18)

# When built as part of the QHiveFrame project (via add_subdirectory),
# the library target 'QHiveFrame' is already defined — link directly.
# When built standalone, find_package(QHiveFrame) is required.

if(NOT TARGET QHiveFrame)
    project(ImageViewerExample LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
    find_package(QHiveFrame REQUIRED)

    set(QHF_LIB QHiveFrame::QHiveFrame)
else()
    set(QHF_LIB QHiveFrame)
endif()

add_executable(imageviewer
    main.cpp
    ImageViewerWorkspace.h   ImageViewerWorkspace.cpp
    ImageViewerEditor.h      ImageViewerEditor.cpp
    ImageViewerMode.h        ImageViewerMode.cpp
)

target_link_libraries(imageviewer PRIVATE ${QHF_LIB} Qt5::Widgets Qt5::Core Qt5::Gui)
