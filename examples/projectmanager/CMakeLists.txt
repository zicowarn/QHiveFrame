cmake_minimum_required(VERSION 3.18)

# Project Manager Demo — standalone example for QHiveFrame framework
# Demonstrates:
#   - Multiple workspaces with workspace tab switching
#   - Split editor layout (horizontal split with project tree + task detail)
#   - Multiple modes per editor (tree/list, view/edit, kanban)
#   - QHFCollapsibleSection, form widgets, kanban board
#
# Build:
#   cmake --preset mac-brew -DBUILD_EXAMPLES=ON
#   cmake --build --preset mac-brew --target projectmanager

set(TARGET projectmanager)

set(SOURCES
    main.cpp
    ProjectManageWorkspace.h    ProjectManageWorkspace.cpp
    TaskBoardWorkspace.h        TaskBoardWorkspace.cpp
    ProjectTreeEditor.h         ProjectTreeEditor.cpp
    TaskEditor.h                TaskEditor.cpp
    KanbanEditor.h              KanbanEditor.cpp
    ProjectTreeMode.h           ProjectTreeMode.cpp
    ProjectListMode.h           ProjectListMode.cpp
    TaskViewMode.h              TaskViewMode.cpp
    TaskEditMode.h              TaskEditMode.cpp
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
