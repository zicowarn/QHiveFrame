# QHiveFrame

<p align="center">
  🇬🇧 <a href="README.md">English</a> ·
  🇨🇳 <a href="README.zh-CN.md">中文</a> ·
  🇩🇪 <a href="README.de-DE.md"><b>Deutsch</b></a>
</p>

---

Ein modulares Qt/C++ GUI-Framework, inspiriert von Blenders Multi-Workspace-/Multi-Editor-System.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Qt5](https://img.shields.io/badge/Qt-5.15-green)

[GitHub: zicowarn/QHiveFrame](https://github.com/zicowarn/QHiveFrame)

---

## Überblick

QHiveFrame bietet eine **dreistufige hierarchische Architektur** zum Bau komplexer, mehrteiliger Desktop-Anwendungen mit Qt:

```
Workspace (Bereich) → Editor (Panel) → Mode (Kontext)
```

Diese Architektur ist stark von Blenders Area/Editor-System inspiriert, bei dem ein Fenster frei in mehrere Panels aufgeteilt werden kann, die jeweils unabhängige Inhalte ausführen. Im Gegensatz zu Blenders Python/C-Hybrid-Design ist QHiveFrame vollständig in modernem C++ (C++17) geschrieben und verwendet ein **registrierungsbasiertes Plugin-System**, das es ermöglicht, jede Ebene zu erweitern, ohne die Framework-Interna zu ändern.

### Framework vs. Anwendung

QHiveFrame ist **reine Framework-Schicht** — es stellt die UI-Infrastruktur bereit, ohne domänenspezifische Editoren, Werkzeuge oder Rendering-Engines mitzuliefern. Damit eignet es sich als Grundlage für:

- 3D-/CAD-/CAE-Editoren
- Game-Level-Editoren
- Medienproduktionswerkzeuge
- Datenanalyse-Workstations
- Jede Anwendung, die ein flexibles Multi-Viewport-Layout benötigt

---

## Kernfunktionen

### 1. Dreistufige Architektur (Workspace → Editor → Mode)

| Ebene | Basisklasse | Zuständigkeit |
|-------|-------------|---------------|
| **Workspace** | `QHFWorkspaceBase` | Oberster Container mit Split-Layout, verwaltet Editor-Lebenszyklus |
| **Editor** | `QHFEditorBase` | Mittlere Panel-Ebene mit Header + scrollbarem Inhaltsbereich, verwaltet Mode-Umschaltung |
| **Mode** | `QHFModeBase` | Unterster Inhaltsanbieter mit Lebenszyklus-Hooks (initialize/cleanup/update) |

Die Granularität dieser Abstraktion ist bewusst gewählt: **Der Workspace verwaltet das Layout, der Editor die Panel-Umschaltung, der Mode den Inhalt**. Dies schafft eine Balance zwischen Flexibilität und Verständlichkeit — ausdrucksstark genug für komplexe Multi-Panel-Anordnungen, aber eingeschränkt genug, um überschaubar zu bleiben.

Im Vergleich zu Qts `QDockWidget` (umständlich für dynamische Layouts) oder `QMdiArea` (auf MDI-Fenster beschränkt) wirkt dieses Design wie eine **C++-Implementierung von Blenders Area-System**.

### 2. Flexibles Split-Pane-Layout

- Beliebige Verschachtelung horizontaler/vertikaler Splitter
- Dynamisches Hinzufügen/Entfernen/Teilen von Editor-Panels
- Aktive-Knoten-Verfolgung pro Workspace
- Vollständiger Neuaufbau oder inkrementelle Aktualisierung

### 3. Duales Ereignissystem

| System | Verwendung |
|--------|------------|
| **`QHFGuiEventBus`** | Leichte globale Ereignisse (feuern-und-vergessen, ohne Datennutzlast) |
| **`QHFNotifierCenter`** | Umfangreiche Benachrichtigungen mit typisierten Ereignissen und Schlüssel-Wert-Datennutzlast |

Diese Trennung vermeidet die übliche Falle eines einzelnen Ereignissystems, das entweder zu schwach oder überdimensioniert für den jeweiligen Anwendungsfall ist. Beide nutzen unter der Haube Qt Signal/Slot für thread-sicheren Versand.

### 4. Registrierungsbasiertes Plugin-System

Eine echte **Open-Closed-Principle**-Implementierung — das Framework ist offen für Erweiterung, geschlossen für Änderung:

```cpp
QHF_REGISTER_EDITOR(MyEditor, EditorType::MY_TYPE);
QHF_REGISTER_WORKSPACE(QHFWorkspaceType::MY_WORKSPACE, MyWorkspace, 1000);
QHF_REGISTER_MENU("Tools", QHFToolsMenu, 500);
QHF_REGISTER_PANEL("Advanced", QHFAdvancedPanel, 300);
```

Kein Verkabelungscode, keine Framework-Änderung — alle Komponenten werden zur Compile-Zeit automatisch über das Makro-und-Registry-Muster entdeckt.

### 5. Präferenz-Versionsmigration

Schemaänderungen in den Einstellungen sind in langlebigen Projekten unvermeidlich. `QHFPreferencesManager` behandelt dies mit deklarativen Migrationsregeln:

- **`RenameKeyRule`** — Umbenennen oder Verschieben eines Präferenzschlüssels, auch kategorieübergreifend
- **`ConvertValueRule`** — Alte Werte in ein neues Format umwandeln
- **`RemoveKeyRule`** — Veraltete Schlüssel während der Migration entfernen
- **`CustomRule`** — Beliebige Migrationslogik per Lambda

```cpp
QHFPreferencesManager::instance().addMigrationRule(
    SemanticVersion(1, 0, 0),
    std::make_unique<RenameKeyRule>("display/lastDir", "file/directory")
);
```

### 6. Integriertes Theming

| Thema | Beschreibung |
|-------|-------------|
| `LIGHT` | Helle, traditionelle Desktop-Optik |
| `DARK` | Dunkelmodus für augenschonendes Arbeiten |
| `OCEAN` | Blau getöntes helles Thema |
| `SUNNY` | Warmes gelb getöntes Thema |

Jedes Thema definiert: Hintergrund-, Text-, Rahmen-, Panel-, Button-, Combo-Box-, Scrollbereich-, Tab-Leisten- und Menüfarben + Schriftarten + Rahmenradius.
Die `QHFThemedCRTP<T>` CRTP-Mixin-Klasse wendet das aktuelle Thema automatisch auf jedes Widget an, das von ihr erbt.

### 7. Zustandspersistenz

- **`QHFStateManager`**: Fenstergeometrie, Dateizustand, Sprache, Lieblingsverzeichnisse
- **`QHFPreferencesManager`**: JSON-basierte hierarchische Einstellungen mit Versionsmigration
- Semantische Versionierung für Migrationsauslöser

### 8. i18n / Lokalisierung

- Qt `.ts`/`.qm` Übersetzungsdateien enthalten
- Chinesisch (zh_CN) und Englisch (en_US) integriert
- `QHF_DECLARE_NAMESPACE_TR(context)`-Makro für namensraumbezogene Übersetzungen

### 9. Benutzerdefinierte UI-Widgets

- `QHFCollapsibleSection` — Zusammenklappbare Gruppenbox
- `QHFCustomDragDropTreeView` — Baumansicht mit Drag-und-Drop-Unterstützung
- `QHFCustomUnitSpinBox` — Einheitenbewusster Zahlenauswahldrehfeld
- `QHFCustomActionWidgetBase` — Basis für benutzerdefinierte Menü-/Toolbar-Aktionswidgets
- `QHFFileDialog` — Wiederverwendbare Datei-Öffnen-/Speichern-Dialoge

---

## Abhängigkeiten

| Abhängigkeit | Version | Erforderlich |
|-------------|---------|:------------:|
| Qt | 5.15+ | Ja |
| C++-Compiler | C++17 | Ja |
| Doxygen | beliebig | Optional (für Doku) |

---

## Bauen (Build)

```bash
# Klonen
git clone https://github.com/zicowarn/QHiveFrame.git
cd QHiveFrame

# Konfigurieren und bauen (statische Bibliothek)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Oder als dynamische Bibliothek
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build build

# Mit Dokumentation
cmake -B build -DBUILD_DOCS=ON
cmake --build --preset docs  # dann build/docs/html/index.html öffnen
```

### CMake-Presets

**Geteilte Presets** (`CMakePresets.json` — versioniert):

```bash
# Alle Presets auflisten
cmake --list-presets

# Debug-Build
cmake --preset debug
cmake --build --preset debug

# Release-Build
cmake --preset release
cmake --build --preset release

# Test-Build (mit Unit-Tests)
cmake --preset test
cmake --build --preset test
cd build/test && ctest -V
```

**Benutzerlokale Presets** (`CMakeUserPresets.json` — git-ignoriert):

Drittanbieter-Abhängigkeitspfade (Qt5_DIR, Compiler, etc.) variieren pro Maschine.
Erstellen Sie ein lokales Preset aus der Vorlage:

```bash
cp CMakeUserPresets.json.example CMakeUserPresets.json
# Bearbeiten Sie CMakeUserPresets.json für Ihre Umgebung, dann:
cmake --preset <ihr-preset-name>
```

Für macOS mit Homebrew-installiertem Qt5 5.15:

```bash
cp CMakeUserPresets.json.example CMakeUserPresets.json
# Das mac-brew-Preset ist vorkonfiguriert; Qt5_DIR-Pfad prüfen
cmake --preset mac-brew
cmake --build --preset mac-brew
```

### Integration in Ihr Projekt

```cmake
# In Ihrer CMakeLists.txt
find_package(QHiveFrame REQUIRED)
target_link_libraries(ihr_target PRIVATE QHiveFrame::QHiveFrame)
```

---

## Beispiel

Eine **ImageViewer**-Demo befindet sich unter [`examples/imageviewer/`](examples/imageviewer/):

```bash
# Mit Beispielen bauen
cmake --preset debug -DBUILD_EXAMPLES=ON
cmake --build --preset debug
./build/debug/examples/imageviewer/imageviewer
```

Das Beispiel demonstriert:
- Erstellen eines benutzerdefinierten `QHFWorkspaceBase` mit Split-Pane-Layout
- Implementieren eines benutzerdefinierten `QHFEditorBase` mit mehreren Modi
- Schreiben von `QHFModeBase`-Unterklassen (Betrachter und Infopanel)
- Vollständige Registrierungsverdrahtung mit den Framework-Makros

Zum eigenständigen Bauen (nach Installation von QHiveFrame):

```bash
cd examples/imageviewer
cmake -B build -DCMAKE_PREFIX_PATH=/pfad/zu/installiertem/QHiveFrame
cmake --build build
```

![ImageViewer-Demo](screenshots/imageviewer_demo.png) <!-- Screenshot hier einfügen, wenn verfügbar -->

---

## Screenshots

Screenshots werden im [`screenshots/`](screenshots/)-Verzeichnis gespeichert. Erstellen Sie Ihre eigenen oder tragen Sie bei:

1. Bauen und Ausführen eines Beispiels
2. Aufnehmen eines PNG- oder animierten GIF-Screenshots
3. Speichern in `screenshots/<name>.png`
4. Referenzieren im README: `![description](screenshots/<name>.png)`

Empfohlene Werkzeuge:
- **macOS**: `Cmd+Umschalt+4` (PNG), GIPHY Capture (GIF)
- **Linux**: `ffmpeg -f x11grab` (GIF mit `palettegen`)
- **Windows**: ShareX, ScreenToGif

---

## Architekturdiagramm

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

### Ereignisfluss

```
Benutzeraktion → QHFNotifierCenter::publish()
                      │
                      ├─→ QHFWorkspaceBase::handleNotify()  (Layout-Verwaltung)
                      ├─→ QHFEditorBase::handleNotify()     (Modus-Umschaltung)
                      ├─→ QHFModeBase::handleNotify()       (Inhaltsaktualisierung)
                      └─→ QHFAppMainWindow::handleNotify()  (Dateioperationen, Vollbild, etc.)
```

---

## Erste Schritte

Das Framework enthält einfache Dummy-Implementierungen, die zeigen, wie alles zusammengeschaltet wird:

- `QHFDummyEditor` — Minimaler Editor mit Platzhalter-Modi
- `QHFModeDummyDefault` / `QHFModeDummyTabbed` — Beispiel-Mode-Implementierungen
- `QHFDummyWorkspace` — Beispiel-Workspace mit Split-Pane-Layout
- `QHFFileMenu` / `QHFEditMenu` / `QHFViewMenu` / `QHFHelpMenu` — Vorgefertigte Menüvorlagen

Um eine echte Anwendung zu bauen, leiten Sie von den Basisklassen ab und registrieren Sie diese über die bereitgestellten Makros.

---

## Projektstruktur

```
QHiveFrame/
├── core/              # Framework-Kern (58 Dateien)
│   ├── QHFWorkspaceBase.h      # Workspace-Basisklasse + LayoutNode
│   ├── QHFEditorBase.h         # Editor-Basisklasse
│   ├── QHFModeBase.h           # Mode-Basisklasse
│   ├── QHFGuiEventBus.h        # Leichter Ereignisbus
│   ├── QHFGuiNotifierCenter.h  # Umfangreiches Benachrichtigungssystem
│   ├── QHFThemeManager.h       # Themenverwaltung (4 Themes)
│   ├── QHFStateManager.h       # Zustandspersistenz
│   ├── QHFPreferencesManager.h # Einstellungen mit Versionsmigration
│   ├── QHFMenuFactory.h        # Menü-Registrierungssystem
│   ├── QHFEditorFactory.h      # Editor-Registrierung
│   ├── QHFModeFactory.h        # Mode-Registrierung
│   ├── QHFWorkspaceFactory.h   # Workspace-Registrierung
│   └── ...                   # Widgets, Hilfsprogramme, Dialoge
├── dialog/            # Wiederverwendbare Dialogfenster
├── menu/              # Vorgefertigte Menüimplementierungen
├── workspace/         # Workspace-Beispiele
├── subeditor/         # Editor-Beispiele
├── subeditormode/     # Mode-Beispiele
├── preferencepanel/   # Einstellungs-Panel-Framework
├── customwidget/      # Benutzerdefinierte Qt-Widget-Sammlung
├── icons/             # SVG-Symbolressourcen
├── locals/            # i18n-Übersetzungsdateien (zh_CN, en_US)
├── CMakeLists.txt     # Build-Konfiguration
├── CMakePresets.json  # CMake-Presets
├── LICENSE            # MIT-Lizenz
└── README.md          # Diese Datei (Englisch)
```

---

## Mitwirken

Beiträge sind willkommen! Bitte folgen Sie dem Code-Stil des Projekts, definiert in [`.clang-format`](.clang-format):

```bash
# clang-format installieren (falls nicht vorhanden)
# macOS: brew install clang-format
# Linux: apt install clang-format
# Windows: Teil von LLVM oder über VS-Erweiterung

# Alle Quelldateien vor dem Commit formatieren
find . \( -name '*.cpp' -o -name '*.h' \) -exec clang-format -i -style=file {} +
```

Wichtige Stilregeln:
- **Allman-Klammern** — Klammern in eigener Zeile
- **4 Leerzeichen Einzug** — keine Tabulatoren
- **100 Zeichen Zeilenbreite**
- **Qt-Stil Zeiger/Referenzen** — `int* ptr`, `int& ref`
- **Sortierte Includes** — eigene Header → Qt-Header → System-Header

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert — siehe [LICENSE](LICENSE) für Details.
