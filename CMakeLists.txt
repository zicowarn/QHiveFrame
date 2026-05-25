cmake_minimum_required(VERSION 3.18)
project(QHiveFrame
    VERSION 1.0.0
    DESCRIPTION "A modular Qt/C++ GUI application framework inspired by Blender's editor system"
    LANGUAGES CXX
)

# ============================================
# 构建环境配置
# ============================================
# 本项目使用 CMakePresets.json 管理共享构建预设（debug / release / test / docs），
# 开箱即用：cmake --preset <name> && cmake --build --preset <name>
#
# 若 Qt5 等第三方依赖安装在非标准路径，请创建本地用户预设：
#   cp CMakeUserPresets.json.example CMakeUserPresets.json
#   # 编辑 CMakeUserPresets.json，设置 Qt5_DIR 等环境相关变量
#   cmake --preset <your-local-preset>
#
# CMakeUserPresets.json 已被 .gitignore 忽略，不会提交到版本库。
# ============================================

# ============================================
# 编译选项
# ============================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# MSVC 特定选项
if(MSVC)
    add_compile_options(/utf-8)
    add_compile_options(/MP)  # 多核编译
    add_compile_options(/bigobj)
endif()

# ============================================
# 构建类型
# ============================================
option(BUILD_SHARED_LIBS "Build QHiveFrame as shared library" OFF)
option(BUILD_EXAMPLES "Build example applications" OFF)
option(BUILD_DOCS "Build Doxygen documentation" OFF)

# ============================================
# Qt 依赖
# ============================================
# Prefer Homebrew Qt5 over system Qt6 to avoid include path conflicts
set(CMAKE_FIND_FRAMEWORK LAST)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)



# ============================================
# Source files (explicit list — no GLOB_RECURSE)
# ============================================
set(QHF_CORE_SOURCES
    core/QHFAppHeader.h          core/QHFAppHeader.cpp
    core/QHFAppStatusbar.h       core/QHFAppStatusbar.cpp
    core/QHFCollapsibleSection.h core/QHFCollapsibleSection.cpp
    core/QHFCustomActionWidgetBase.h   core/QHFCustomActionWidgetBase.cpp
    core/QHFCustomActionWidgetFactory.h core/QHFCustomActionWidgetFactory.cpp
    core/QHFCustomDragDropTreeView.h   core/QHFCustomDragDropTreeView.cpp
    core/QHFCustomUnitSpinBox.h        core/QHFCustomUnitSpinBox.cpp
    core/QHFEditorBase.h         core/QHFEditorBase.cpp
    core/QHFEditorFactory.h      core/QHFEditorFactory.cpp
    core/QHFFileDialog.h         core/QHFFileDialog.cpp
    core/QHFGuiAction.h          core/QHFGuiAction.tpp
    core/QHFGuiEvent.h
    core/QHFGuiEventBus.h        core/QHFGuiEventBus.cpp
    core/QHFGuiNotifier.h
    core/QHFGuiNotifierCenter.h  core/QHFGuiNotifierCenter.cpp
    core/QHFMenuBase.h           core/QHFMenuBase.cpp
    core/QHFMenuFactory.h        core/QHFMenuFactory.cpp
    core/QHFMessageBox.h         core/QHFMessageBox.cpp
    core/QHFModeBase.h           core/QHFModeBase.cpp
    core/QHFModeFactory.h        core/QHFModeFactory.cpp
    core/QHFPreferencePanelBase.h      core/QHFPreferencePanelBase.cpp
    core/QHFPreferencePanelFactory.h   core/QHFPreferencePanelFactory.cpp
    core/QHFPreferencesManager.h       core/QHFPreferencesManager.cpp
    core/QHFStateManager.h       core/QHFStateManager.cpp
    core/QHFThemeManager.h       core/QHFThemeManager.cpp
    core/QHFUIIEditorContext.h
    core/QHFUIIMenuProvider.h
    core/QHFUIIModeContext.h
    core/QHFUIIPreferencePanel.h
    core/QHFUIUtils.h
    core/QHFWorkspaceBase.h      core/QHFWorkspaceBase.cpp
    core/QHFWorkspaceFactory.h   core/QHFWorkspaceFactory.cpp
)

set(QHF_DIALOG_SOURCES
    dialog/QHFAboutUsDialog.h    dialog/QHFAboutUsDialog.cpp
    dialog/QHFAppMainWindow.h    dialog/QHFAppMainWindow.cpp
    dialog/QHFAppSimpleWindow.h  dialog/QHFAppSimpleWindow.cpp
    dialog/QHFContactUsDialog.h  dialog/QHFContactUsDialog.cpp
    dialog/QHFPreferenceDialog.h dialog/QHFPreferenceDialog.cpp
)

set(QHF_MENU_SOURCES
    menu/QHFEditMenu.h           menu/QHFEditMenu.cpp
    menu/QHFFileMenu.h           menu/QHFFileMenu.cpp
    menu/QHFHelpMenu.h           menu/QHFHelpMenu.cpp
    menu/QHFViewMenu.h           menu/QHFViewMenu.cpp
)

set(QHF_WORKSPACE_SOURCES
    workspace/QHFDummyWorkspace.h workspace/QHFDummyWorkspace.cpp
)

set(QHF_SUBEDITOR_SOURCES
    subeditor/QHFDummyEditor.h   subeditor/QHFDummyEditor.cpp
)

set(QHF_SUBMODE_SOURCES
    subeditormode/QHFModeDummyDefault.h subeditormode/QHFModeDummyDefault.cpp
    subeditormode/QHFModeDummyTabbed.h  subeditormode/QHFModeDummyTabbed.cpp
)

set(QHF_PREFERENCE_SOURCES
    preferencepanel/QHFGeneralPreferencePanel.h preferencepanel/QHFGeneralPreferencePanel.cpp
)

set(QHF_CUSTOMWIDGET_SOURCES
    customwidget/QHFCustomActionWidgetDummy.h customwidget/QHFCustomActionWidgetDummy.cpp
)

# Resource files
set(QHF_RESOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/QHFGuiIcons.qrc"
    "${CMAKE_CURRENT_SOURCE_DIR}/QHFGuiLocals.qrc"
)

# ============================================
# 头文件目录
# ============================================
set(QHF_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog
    ${CMAKE_CURRENT_SOURCE_DIR}/menu
    ${CMAKE_CURRENT_SOURCE_DIR}/workspace
    ${CMAKE_CURRENT_SOURCE_DIR}/subeditor
    ${CMAKE_CURRENT_SOURCE_DIR}/subeditormode
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencepanel
    ${CMAKE_CURRENT_SOURCE_DIR}/customwidget
)

# ============================================
# 目标库
# ============================================
set(QHF_ALL_SOURCES
    ${QHF_CORE_SOURCES}
    ${QHF_DIALOG_SOURCES}
    ${QHF_MENU_SOURCES}
    ${QHF_WORKSPACE_SOURCES}
    ${QHF_SUBEDITOR_SOURCES}
    ${QHF_SUBMODE_SOURCES}
    ${QHF_PREFERENCE_SOURCES}
    ${QHF_CUSTOMWIDGET_SOURCES}
)

qt5_add_resources(QHF_RESOURCES ${QHF_RESOURCE_FILES})

if(BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED
        ${QHF_ALL_SOURCES}
        ${QHF_RESOURCES}
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE QHF_BUILD_SHARED)
    target_compile_definitions(${PROJECT_NAME} INTERFACE QHF_SHARED)
else()
    add_library(${PROJECT_NAME} STATIC
        ${QHF_ALL_SOURCES}
        ${QHF_RESOURCES}
    )
endif()

# ============================================
# 目标属性
# ============================================
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dialog>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/menu>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/workspace>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/subeditor>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/subeditormode>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/preferencepanel>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/customwidget>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
)

# Qt5/Qt6 header conflict workaround (macOS brew dual-install)
# Qt5 internally uses #include <QtCore/qglobal.h> style, but the framework Headers
# paths exposed by CMake (Qt5Core.framework/Headers) lack a QtCore/ subdirectory,
# so the compiler falls through to /usr/local/include/QtCore (Qt6 symlink).
# We derive the Qt5 prefix and add its include/ root as a system include path.
if(Qt5_FOUND)
    # Derive Qt5 prefix from one of the known framework paths
    get_target_property(__qt5_core_incs Qt5::Core INTERFACE_INCLUDE_DIRECTORIES)
    list(GET __qt5_core_incs 0 __qt5_prefix_dir)
    # __qt5_prefix_dir is like ".../QtCore.framework" — go up 2 dirs to Cellar prefix
    get_filename_component(__qt5_prefix "${__qt5_prefix_dir}" DIRECTORY)
    get_filename_component(__qt5_prefix "${__qt5_prefix}" DIRECTORY)
    set(__qt5_include_root "${__qt5_prefix}/include")
    if(IS_DIRECTORY "${__qt5_include_root}")
        message(STATUS "Qt5 include root: ${__qt5_include_root}")
        # Use regular BEFORE (not SYSTEM/Isystem) because /usr/local/include is a
        # compiler built-in path in Apple Clang and takes priority over -isystem.
        # The Qt5 include root has QtCore/ subdirectory needed for <QtCore/...> includes.
        # This is only needed when both Qt5 and Qt6 are installed on the same machine.
        # Use global include_directories so all targets (library + tests + examples) get it.
        include_directories(BEFORE ${__qt5_include_root})
    endif()
    unset(__qt5_core_incs)
    unset(__qt5_prefix_dir)
    unset(__qt5_prefix)
    unset(__qt5_include_root)
endif()

# 版本宏
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        PRODUCT_VERSION="${PROJECT_VERSION}"
)

# ============================================
# 安装规则
# ============================================
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY core/ dialog/ menu/ workspace/ subeditor/ subeditormode/ preferencepanel/ customwidget/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.tpp"
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# ============================================
# Tests
# ============================================
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    find_package(Qt5 REQUIRED COMPONENTS Test)

    # Test: PreferencesManager migration rules
    add_executable(test_preferences
        tests/test_preferences.cpp
    )
    target_link_libraries(test_preferences PRIVATE ${PROJECT_NAME} Qt5::Test Qt5::Core)
    add_test(NAME Preferences COMMAND test_preferences)

    # Test: StateManager serialization
    add_executable(test_statemgr
        tests/test_statemgr.cpp
    )
    target_link_libraries(test_statemgr PRIVATE ${PROJECT_NAME} Qt5::Test Qt5::Core Qt5::Widgets)
    add_test(NAME StateManager COMMAND test_statemgr)

    # Test: Workspace LayoutNode operations
    add_executable(test_workspace
        tests/test_workspace.cpp
    )
    target_link_libraries(test_workspace PRIVATE ${PROJECT_NAME} Qt5::Test Qt5::Core Qt5::Widgets)
    add_test(NAME Workspace COMMAND test_workspace)
endif()

# ============================================
# Doxygen 文档（可选）
# ============================================
if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_PROJECT_VERSION ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRIPTION})
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET "")
    set(DOXYGEN_MACRO_EXPANSION YES)
    set(DOXYGEN_EXPAND_ONLY_PREDEF YES)

    # 使用自定义 Doxyfile.in 作为模板
    set(DOXYGEN_DOT_FOUND TRUE)
    if(DOXYGEN_DOT_FOUND)
        set(DOXYGEN_HAVE_DOT YES)
    else()
        set(DOXYGEN_HAVE_DOT NO)
    endif()

    doxygen_add_docs(docs
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/dialog
        ${CMAKE_CURRENT_SOURCE_DIR}/menu
        ${CMAKE_CURRENT_SOURCE_DIR}/workspace
        ${CMAKE_CURRENT_SOURCE_DIR}/subeditor
        ${CMAKE_CURRENT_SOURCE_DIR}/subeditormode
        ${CMAKE_CURRENT_SOURCE_DIR}/preferencepanel
        ${CMAKE_CURRENT_SOURCE_DIR}/customwidget
        COMMENT "Generate Doxygen documentation"
    )
endif()

# ============================================
# 示例程序（可选）
# ============================================
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ============================================
# clang-format 代码格式化
# ============================================
# 用法：
#   cmake --build <build-dir> --target format       # 格式化所有源文件
#   cmake --build <build-dir> --target check-format  # 仅检查，不修改
#
# 或通过 preset:
#   cmake --build --preset test --target format
# ============================================

# 收集所有需格式化的源文件（不含 build 目录和第三方）
set(QHF_FORMAT_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog
    ${CMAKE_CURRENT_SOURCE_DIR}/menu
    ${CMAKE_CURRENT_SOURCE_DIR}/workspace
    ${CMAKE_CURRENT_SOURCE_DIR}/subeditor
    ${CMAKE_CURRENT_SOURCE_DIR}/subeditormode
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencepanel
    ${CMAKE_CURRENT_SOURCE_DIR}/customwidget
)

file(GLOB_RECURSE QHF_FORMAT_SOURCES
    ${QHF_FORMAT_DIRS}
    *.h
    *.cpp
    *.tpp
)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    # 格式化（就地修改）
    add_custom_target(format
        COMMAND ${CLANG_FORMAT}
            -i -style=file -fallback-style=none
            ${QHF_FORMAT_SOURCES}
        COMMENT "Formatting sources with clang-format..."
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # 格式检查（CI 用，只报告差异，不修改）
    add_custom_target(check-format
        COMMAND ${CLANG_FORMAT}
            -style=file --dry-run -Werror
            ${QHF_FORMAT_SOURCES}
        COMMENT "Checking code style compliance..."
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    # 友好提示
    add_custom_target(format
        COMMAND ${CMAKE_COMMAND} -E echo
            "[QHiveFrame] clang-format not found. Install it: brew install clang-format"
    )
    add_custom_target(check-format
        COMMAND ${CMAKE_COMMAND} -E echo
            "[QHiveFrame] clang-format not found. Skipping format check."
    )
endif()


# ============================================
# Examples (standalone subdirectory)
# ============================================
# The examples/ directory has its own CMakeLists.txt and can also be
# built standalone after installing QHiveFrame:
#
#   cd examples/imageviewer
#   cmake -B build -DCMAKE_PREFIX_PATH=/path/to/installed/QHiveFrame
#   cmake --build build
