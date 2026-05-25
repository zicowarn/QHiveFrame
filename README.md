# QHiveFrame

<p align="center">
  🇬🇧 <a href="README.md"><b>English</b></a> ·
  🇨🇳 <a href="README.zh-CN.md">中文</a> ·
  🇩🇪 <a href="README.de-DE.md">Deutsch</a>
</p>

---

A modular Qt/C++ GUI application framework inspired by Blender's multi-workspace / multi-editor system.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Qt5](https://img.shields.io/badge/Qt-5.15-green)

[GitHub: zicowarn/QHiveFrame](https://github.com/zicowarn/QHiveFrame)

---

## Overview

QHiveFrame provides a **three-tier hierarchical architecture** for building complex, multi-pane desktop applications with Qt:

```
Workspace (area) → Editor (panel) → Mode (context)
```

This architecture is heavily inspired by Blender's area/editor system, where a window can be freely split into multiple panels, each running independent content. Unlike Blender's Python/C hybrid design, QHiveFrame is written entirely in modern C++ (C++17) with a clean **registration-based plugin system**, allowing you to extend every tier without modifying framework internals.

### Framework vs Application

QHiveFrame is the **framework layer only** — it provides the UI infrastructure without bundling any domain-specific editors, tools, or rendering engines. This makes it suitable as a foundation for:

- 3D/CAD/CAE editors
- Game level editors
- Media production tools
- Data analysis workstations
- Any application needing a flexible multi-viewport layout

---

## Key Features

### 1. Three-Tier Architecture (Workspace → Editor → Mode)

| Tier | Base Class | Responsibility |
|------|-----------|---------------|
| **Workspace** | `QHFWorkspaceBase` | Top-level container with split-pane layout, manages Editor lifecycle |
| **Editor** | `QHFEditorBase` | Mid-level panel with header + scrollable content area, manages Mode switching |
| **Mode** | `QHFModeBase` | Bottom-level content provider with lifecycle hooks (initialize/cleanup/update) |

The granularity of this abstraction is deliberate: **Workspace manages layout, Editor manages panel switching, Mode manages content**. This strikes a balance between flexibility and comprehensibility — expressive enough for complex multi-panel arrangements, constrained enough to remain approachable.

Compared to Qt's `QDockWidget` (cumbersome for dynamic layouts) or `QMdiArea` (limited to MDI windows), this design reads like a **C++ incarnation of Blender's Area system**.

### 2. Flexible Split-Pane Layout

- Arbitrary nesting of horizontal/vertical splitters
- Dynamic add/remove/split of editor panes
- Active node tracking per workspace
- Full rebuild or incremental refresh

### 3. Dual Event System

| System | When to Use |
|--------|------------|
| **`QHFGuiEventBus`** | Lightweight global events (fire-and-forget, no data payload) |
| **`QHFNotifierCenter`** | Rich notifications with typed events and key-value data payloads |

This separation avoids the common pitfall of a single event system being either underpowered or overengineered for any given use case. Both use Qt Signal/Slot under the hood for thread-safe dispatch.

### 4. Registration-Based Plugin System

A true **Open-Closed Principle** implementation — the framework is open for extension, closed for modification:

```cpp
QHF_REGISTER_EDITOR(MyEditor, EditorType::MY_TYPE);
QHF_REGISTER_WORKSPACE(QHFWorkspaceType::MY_WORKSPACE, MyWorkspace, 1000);
QHF_REGISTER_MENU("Tools", QHFToolsMenu, 500);
QHF_REGISTER_PANEL("Advanced", QHFAdvancedPanel, 300);
```

No wiring code, no framework modification — all components are discovered automatically at compile time via the macro + registry pattern.

### 5. Preference Version Migration

Settings schema changes are inevitable in long-lived projects. `QHFPreferencesManager` handles this with declarative migration rules:

- **`RenameKeyRule`** — Rename or relocate a preference key, even across categories
- **`ConvertValueRule`** — Transform old values to new format
- **`RemoveKeyRule`** — Drop deprecated keys during migration
- **`CustomRule`** — Arbitrary migration logic via lambda

```cpp
QHFPreferencesManager::instance().addMigrationRule(
    SemanticVersion(1, 0, 0),
    std::make_unique<RenameKeyRule>("display/lastDir", "file/directory")
);
```

### 6. Built-In Theming

| Theme | Description |
|-------|------------|
| `LIGHT` | Bright, traditional desktop look |
| `DARK` | Dark mode for eye comfort |
| `OCEAN` | Blue-tinted light theme |
| `SUNNY` | Yellow-tinted warm theme |

Each theme defines: background, text, border, panel, button, combo box, scroll area, tab bar and menu colors + fonts + border radius.
The `QHFThemedCRTP<T>` CRTP mixin automatically applies the current theme to any widget that inherits from it.

### 7. State Persistence

- **`QHFStateManager`**: Window geometry, file state, language, favorite directories
- **`QHFPreferencesManager`**: JSON-based hierarchical settings with version migration
- Semantic versioning for migration triggers

### 8. i18n / Localization

- Qt `.ts`/`.qm` translation files included
- Chinese (zh_CN) and English (en_US) built-in
- `QHF_DECLARE_NAMESPACE_TR(context)` macro for namespace-scoped translations

### 9. Custom UI Widgets

- `QHFCollapsibleSection` — Collapsible group box
- `QHFCustomDragDropTreeView` — Tree view with internal drag/drop support
- `QHFCustomUnitSpinBox` — Unit-aware spin box
- `QHFCustomActionWidgetBase` — Base for custom menu/toolbar action widgets
- `QHFFileDialog` — Reusable file open/save dialogs

---

## Dependencies

| Dependency | Version | Required |
|-----------|---------|----------|
| Qt | 5.15+ | Yes |
| C++ Compiler | C++17 | Yes |
| Doxygen | any | Optional (for docs) |

---

## Build

```bash
# Clone
git clone https://github.com/zicowarn/QHiveFrame.git
cd QHiveFrame

# Configure and build (static library)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Or as shared library
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build

# With documentation
cmake -B build -DBUILD_DOCS=ON
cmake --build --preset docs  # then open build/docs/html/index.html
```

### CMake Presets

**Shared presets** (`CMakePresets.json` — version controlled):

```bash
# List all presets
cmake --list-presets

# Debug build
cmake --preset debug
cmake --build --preset debug

# Release build
cmake --preset release
cmake --build --preset release

# Test build (with unit tests)
cmake --preset test
cmake --build --preset test
cd build/test && ctest -V
```

**User-local presets** (`CMakeUserPresets.json` — gitignored):

Third-party dependency paths (Qt5_DIR, compiler, etc.) vary per machine.
Create a local preset from the template:

```bash
cp CMakeUserPresets.json.example CMakeUserPresets.json
# Edit CMakeUserPresets.json to match your environment, then:
cmake --preset <your-preset-name>
```

For macOS with Homebrew-installed Qt5 5.15:

```bash
cp CMakeUserPresets.json.example CMakeUserPresets.json
# The mac-brew preset is pre-configured; verify Qt5_DIR path
cmake --preset mac-brew
cmake --build --preset mac-brew
```

### Integration into Your Project

```cmake
# In your CMakeLists.txt
find_package(QHiveFrame REQUIRED)
target_link_libraries(your_target PRIVATE QHiveFrame::QHiveFrame)
```

---

## Example

An **ImageViewer** demo is included under [`examples/imageviewer/`](examples/imageviewer/):

```bash
# Build with examples
cmake --preset debug -DBUILD_EXAMPLES=ON
cmake --build --preset debug
./build/debug/examples/imageviewer/imageviewer
```

The example demonstrates:
- Creating a custom `QHFWorkspaceBase` with split-pane layout
- Implementing a custom `QHFEditorBase` with multiple modes
- Writing `QHFModeBase` subclasses (viewer and info panel)
- Full registration wiring using the framework's macros

To build it standalone (after installing QHiveFrame):

```bash
cd examples/imageviewer
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/installed/QHiveFrame
cmake --build build
```

![ImageViewer Demo](screenshots/imageviewer_demo.png) <!-- Add screenshot here when available -->

---

## Screenshots

Screenshots are stored in the [`screenshots/`](screenshots/) directory. Capture your own or contribute by:

1. Building and running an example
2. Taking a PNG or animated GIF screenshot
3. Saving it to `screenshots/<name>.png`
4. Referencing it in the README: `![description](screenshots/<name>.png)`

Recommended tools for capture:
- **macOS**: `Cmd+Shift+4` (PNG), GIPHY Capture (GIF)
- **Linux**: `ffmpeg -f x11grab` (GIF with `palettegen`)
- **Windows**: ShareX, ScreenToGif

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────┐
│                 QHFAppMainWindow                       │
│  ┌───────────────────────────────────────────────┐  │
│  │  QHFAppHeader (global menu bar + header)        │  │
│  ├───────────────────────────────────────────────┤  │
│  │  HWorkspaceContainer (QStackedWidget)         │  │
│  │  ┌─────────────────────────────────────────┐  │  │
│  │  │  QHFWorkspaceBase (split layout)          │  │  │
│  │  │  ┌──────────────┬──────────────────┐    │  │  │
│  │  │  │  QHFEditorBase  │  QHFEditorBase      │    │  │  │
│  │  │  │  ┌────────┐   │  ┌────────┐      │    │  │  │
│  │  │  │  │ Header │   │  │ Header │      │    │  │  │
│  │  │  │  ├────────┤   │  ├────────┤      │    │  │  │
│  │  │  │  │  Mode  │   │  │  Mode  │      │    │  │  │
│  │  │  │  └────────┘   │  └────────┘      │    │  │  │
│  │  │  └──────────────┴──────────────────┘    │  │  │
│  │  └─────────────────────────────────────────┘  │  │
│  ├───────────────────────────────────────────────┤  │
│  │  QHFAppStatusBar                                │  │
│  └───────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘
```

### Event Flow

```
User Action → QHFNotifierCenter::publish()
                │
                ├─→ QHFWorkspaceBase::handleNotify()  (layout management)
                ├─→ QHFEditorBase::handleNotify()     (mode switching)
                ├─→ QHFModeBase::handleNotify()       (content updates)
                └─→ QHFAppMainWindow::handleNotify()  (file operations, fullscreen, etc.)
```

---

## Getting Started

The framework ships with simple dummy implementations to demonstrate how to wire everything together:

- `QHFDummyEditor` — Minimal editor with placeholder modes
- `QHFModeDummyDefault` / `QHFModeDummyTabbed` — Example mode implementations
- `QHFDummyWorkspace` — Example workspace with split-pane layout
- `QHFFileMenu` / `QHFEditMenu` / `QHFViewMenu` / `QHFHelpMenu` — Pre-built menu templates

To build a real application, subclass the base classes and register them via the provided macros.

---

## Project Structure

```
QHiveFrame/
├── core/              # Framework core (58 files)
│   ├── QHFWorkspaceBase.h      # Workspace base class + LayoutNode
│   ├── QHFEditorBase.h         # Editor base class
│   ├── QHFModeBase.h           # Mode base class
│   ├── QHFGuiEventBus.h        # Lightweight event bus
│   ├── QHFGuiNotifierCenter.h  # Rich notification system
│   ├── QHFThemeManager.h       # Theme management (4 themes)
│   ├── QHFStateManager.h       # State persistence
│   ├── QHFPreferencesManager.h # Preferences with version migration
│   ├── QHFMenuFactory.h        # Menu registration system
│   ├── QHFEditorFactory.h      # Editor registration
│   ├── QHFModeFactory.h        # Mode registration
│   ├── QHFWorkspaceFactory.h   # Workspace registration
│   └── ...                   # Widgets, utilities, dialogs
├── dialog/            # Reusable dialog windows
├── menu/              # Pre-built menu implementations
├── workspace/         # Workspace examples
├── subeditor/         # Editor examples
├── subeditormode/     # Mode examples
├── preferencepanel/   # Preferences panel framework
├── customwidget/      # Custom Qt widget collection
├── icons/             # SVG icon assets
├── locals/            # i18n translation files (zh_CN, en_US)
├── CMakeLists.txt     # Build configuration
├── CMakePresets.json  # CMake presets
├── LICENSE            # MIT License
└── README.md          # This file
```

---

## Contributing

Contributions are welcome! Please follow the project's code style defined in [`.clang-format`](.clang-format):

```bash
# Install clang-format (if not already)
# macOS: brew install clang-format
# Linux: apt install clang-format
# Windows: part of LLVM, or via VS extension

# Format all source files before committing
find . \( -name '*.cpp' -o -name '*.h' \) -exec clang-format -i -style=file {} +
```

Key style rules:
- **Allman braces** — braces on their own line
- **4-space indent** — no tabs
- **100 char column limit**
- **Qt-style pointer/reference** — `int* ptr`, `int& ref`
- **Sorted includes** — own headers → Qt headers → system headers

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.
