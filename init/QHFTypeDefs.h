#ifndef QHFTYPEDEFS_H
#define QHFTYPEDEFS_H

/**
 * @file QHFTypeDefs.h
 * @brief Unified type definitions for QHiveFrame framework.
 *
 * ======================================================================
 * DESIGN RATIONALE — Why enums instead of QString-based types
 * ======================================================================
 *
 * Originally considered moving to a fully string-based type system
 * (e.g. QHFEditorRegistry::registerEditor("TREE", ...)),
 * but this approach introduces several risks:
 *
 * 1. MAGIC STRING PROLIFERATION ("magic strings spread everywhere")
 *    String IDs like "TREE" get copy-pasted across files.
 *    Renaming a type requires a global grep, and the compiler gives
 *    zero help catching stale references.
 *
 * 2. SILENT RUNTIME FAILURES ("silent failures at runtime")
 *    A typo — "TRE" — compiles fine. It only fails at
 *    runtime when the lookup returns a null/zero/NONE, and the UI
 *    silently shows nothing or a fallback. No compile-time safety net.
 *
 * 3. NO EXHAUSTIVENESS CHECKING
 *    C++ switch on enum gives -Wswitch warnings for unhandled cases.
 *    String-based "if-else chains" only check paths the author
 *    remembered to write — no compiler help for missing branches.
 *
 * 4. ZERO-COST vs DYNAMIC LOOKUP
 *    Enum comparison is a single uint8_t comparison (one CPU insn).
 *    QString comparison involves heap-allocated data and non-trivial
 *    character comparison.
 *
 * TRADE-OFF ACCEPTED:
 *   Adding a new type requires editing THIS FILE (add enum value +
 *   insert three switch-case branches in toString/fromString/transName).
 *   This is ONE file to edit instead of three (the original design
 *   scattered enums across QHFUIIEditorContext.h, QHFUIIModeContext.h,
 *   and QHFWorkspaceBase.h).
 *
 * For external/plugin extensions that cannot touch this header,
 * a separate dynamic string-based registry could be layered on top
 * without replacing this system.
 * ======================================================================
 */

#include "QHFUIUtils.h"

#include <cstdint>

namespace QHF {

// ======================================================================
// EditorType — identifies types of editors in the framework
// ======================================================================
/**
 * @note Add new values BEFORE the comment, keep the trailing marker.
 */
enum class EditorType : std::uint8_t {
    NONE,            ///< Empty type
    DUMMY,           ///< Placeholder type
    SPECIAL,         ///< Special type (development example)
    TREE, ///< Tree — project tree editor
    DETAIL,  ///< Detail — task detail editor
    KANBAN,       ///< Kanban — kanban editor
    // === EXTEND: add new EditorType values above this line ===
};

/**
 * @brief Utility functions for EditorType.
 * @details When adding a new EditorType value, you must add a case
 *          to ALL three functions below (toString, fromString, transName).
 */
namespace EditorTypeUtils {
    QHF_DECLARE_NAMESPACE_TR(QHF::EditorTypeUtils);

    inline QString toString(EditorType mode) {
        switch (mode) {
        case EditorType::DUMMY:
            return "DUMMY";
        case EditorType::SPECIAL:
            return "SPECIAL";
        case EditorType::TREE:
            return "TREE";
        case EditorType::DETAIL:
            return "DETAIL";
        case EditorType::KANBAN:
            return "KANBAN";
        // === EXTEND: add toString case above this line ===
        default:
            return "NONE";
        }
    }

    inline QString toTransName(EditorType mode) {
        switch (mode) {
        case EditorType::DUMMY:
            return tr("DUMMY");
        case EditorType::SPECIAL:
            return tr("SPECIAL");
        case EditorType::TREE:
            return tr("Tree");
        case EditorType::DETAIL:
            return tr("Detail");
        case EditorType::KANBAN:
            return tr("Kanban");
        // === EXTEND: add toTransName case above this line ===
        default:
            return tr("NONE");
        }
    }

    inline EditorType fromString(const QString& str) {
        if (str == "DUMMY") {
            return EditorType::DUMMY;
        }
        if (str == "SPECIAL") {
            return EditorType::SPECIAL;
        }
        if (str == "TREE") {
            return EditorType::TREE;
        }
        if (str == "DETAIL") {
            return EditorType::DETAIL;
        }
        if (str == "KANBAN") {
            return EditorType::KANBAN;
        }
        // === EXTEND: add fromString if-branch above this line ===
        return EditorType::NONE;
    }
} // namespace EditorTypeUtils


// ======================================================================
// QHFModeType — identifies types of modes in editors
// ======================================================================
/**
 * @note Add new values BEFORE the comment, keep the trailing marker.
 */
enum class QHFModeType : std::uint8_t {
    NONE,
    // Placeholder editor-related modes
    DUMMY_DEFAULT, ///< Default placeholder editor
    DUMMY_TABBED,  ///< Tabbable placeholder
    // Special editor-related modes
    SPECIAL_DEFAULT, ///< Default special editor
    // Project Manager modes
    TREE,   ///< Tree view mode
    LIST,   ///< List view mode
    DETAIL_VIEW,      ///< Detail view mode
    DETAIL_EDIT,      ///< Detail edit mode
    KANBAN,   ///< Kanban board mode
    // === EXTEND: add new QHFModeType values above this line ===
};

/**
 * @brief Utility functions for QHFModeType.
 * @details When adding a new QHFModeType value, you must add a case
 *          to ALL three functions below.
 */
namespace QHFModeTypeUtils {
    QHF_DECLARE_NAMESPACE_TR(QHF::QHFModeTypeUtils);

    inline QString toString(QHFModeType modeType) {
        switch (modeType) {
        case QHFModeType::DUMMY_DEFAULT:
            return "DUMMY::DEFAULT";
        case QHFModeType::DUMMY_TABBED:
            return "DUMMY::TABBED";
        case QHFModeType::SPECIAL_DEFAULT:
            return "SPECIAL::DEFAULT";
        case QHFModeType::TREE:
            return "TREE";
        case QHFModeType::LIST:
            return "LIST";
        case QHFModeType::DETAIL_VIEW:
            return "DETAIL_VIEW";
        case QHFModeType::DETAIL_EDIT:
            return "DETAIL_EDIT";
        case QHFModeType::KANBAN:
            return "KANBAN";
        // === EXTEND: add toString case above this line ===
        default:
            return "NONE";
        }
    }

    inline QHFModeType fromString(const QString& str) {
        if (str == "DUMMY::DEFAULT") {
            return QHFModeType::DUMMY_DEFAULT;
        }
        if (str == "DUMMY::TABBED") {
            return QHFModeType::DUMMY_TABBED;
        }
        if (str == "SPECIAL::DEFAULT") {
            return QHFModeType::SPECIAL_DEFAULT;
        }
        if (str == "TREE") {
            return QHFModeType::TREE;
        }
        if (str == "LIST") {
            return QHFModeType::LIST;
        }
        if (str == "DETAIL_VIEW") {
            return QHFModeType::DETAIL_VIEW;
        }
        if (str == "DETAIL_EDIT") {
            return QHFModeType::DETAIL_EDIT;
        }
        if (str == "KANBAN") {
            return QHFModeType::KANBAN;
        }
        // === EXTEND: add fromString if-branch above this line ===
        return QHFModeType::NONE;
    }

    inline QString transName(QHFModeType modeType) {
        switch (modeType) {
        case QHFModeType::DUMMY_DEFAULT:
            return tr("DUMMY::DEFAULT");
        case QHFModeType::DUMMY_TABBED:
            return tr("DUMMY::TABBED");
        case QHFModeType::SPECIAL_DEFAULT:
            return tr("SPECIAL::DEFAULT");
        case QHFModeType::TREE:
            return tr("Tree View");
        case QHFModeType::LIST:
            return tr("List View");
        case QHFModeType::DETAIL_VIEW:
            return tr("View");
        case QHFModeType::DETAIL_EDIT:
            return tr("Edit");
        case QHFModeType::KANBAN:
            return tr("Kanban");
        // === EXTEND: add transName case above this line ===
        default:
            return tr("NONE");
        }
    }
} // namespace QHFModeTypeUtils


// ======================================================================
// QHFWorkspaceType — identifies types of workspaces
// ======================================================================
/**
 * @note Add new values BEFORE the comment, keep the trailing marker.
 */
enum class QHFWorkspaceType : std::uint8_t {
    NONE,        ///< Empty
    DUMMY,       ///< Test use
    TEST,        ///< Test 2 use
    SPLIT_DEMO,  ///< Split demo workspace
    KANBAN_DEMO,///< Kanban demo workspace
    // === EXTEND: add new QHFWorkspaceType values above this line ===
};

/**
 * @brief Utility functions for QHFWorkspaceType.
 * @details When adding a new QHFWorkspaceType value, you must add a
 *          case to ALL three functions below.
 */
namespace QHFWorkspaceTypeUtils {
    QHF_DECLARE_NAMESPACE_TR(QHF::QHFWorkspaceTypeUtils);

    inline QString toString(QHFWorkspaceType type) {
        switch (type) {
        case QHFWorkspaceType::DUMMY:
            return "DUMMY";
        case QHFWorkspaceType::TEST:
            return "TEST";
        case QHFWorkspaceType::SPLIT_DEMO:
            return "SPLIT_DEMO";
        case QHFWorkspaceType::KANBAN_DEMO:
            return "KANBAN_DEMO";
        // === EXTEND: add toString case above this line ===
        default:
            return "NONE";
        }
    }

    inline QString toTransName(QHFWorkspaceType type) {
        switch (type) {
        case QHFWorkspaceType::DUMMY:
            return tr("DUMMY");
        case QHFWorkspaceType::TEST:
            return tr("TEST");
        case QHFWorkspaceType::SPLIT_DEMO:
            return tr("Split Demo");
        case QHFWorkspaceType::KANBAN_DEMO:
            return tr("Kanban Demo");
        // === EXTEND: add toTransName case above this line ===
        default:
            return tr("NONE");
        }
    }

    inline QHFWorkspaceType fromString(const QString& str) {
        if (str == "DUMMY") {
            return QHFWorkspaceType::DUMMY;
        }
        if (str == "TEST") {
            return QHFWorkspaceType::TEST;
        }
        if (str == "SPLIT_DEMO") {
            return QHFWorkspaceType::SPLIT_DEMO;
        }
        if (str == "KANBAN_DEMO") {
            return QHFWorkspaceType::KANBAN_DEMO;
        }
        // === EXTEND: add fromString if-branch above this line ===
        return QHFWorkspaceType::NONE;
    }
} // namespace QHFWorkspaceTypeUtils

} // namespace QHF

#endif // QHFTYPEDEFS_H
