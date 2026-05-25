cmake_minimum_required(VERSION 3.18)

# Uni Demo — standalone example for QHiveFrame framework
# Demonstrates:
#   - Multiple workspaces with workspace tab switching
#   - Split editor layout (horizontal split with project tree + task detail)
#   - Multiple modes per editor (tree/list, view/edit, kanban)
#   - QHFCollapsibleSection, form widgets, kanban board
#
# Build:
#   cmake --preset mac-brew -DBUILD_EXAMPLES=ON
#   cmake --build --preset mac-brew --target uni-demo

set(TARGET uni-demo)

set(SOURCES
    main.cpp
    SplitDemoWorkspace.h    SplitDemoWorkspace.cpp
    KanbanDemoWorkspace.h        KanbanDemoWorkspace.cpp
    TreeEditor.h         TreeEditor.cpp
    DetailEditor.h                DetailEditor.cpp
    KanbanEditor.h              KanbanEditor.cpp
    TreeMode.h           TreeMode.cpp
    ListMode.h           ListMode.cpp
    DetailViewMode.h              DetailViewMode.cpp
    DetailEditMode.h              DetailEditMode.cpp
    KanbanMode.h                KanbanMode.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE QHiveFrame)

# Link Qt5 explicitly (required for standalone builds)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
target_link_libraries(${TARGET} PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

# Set output directory alongside the library
set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
