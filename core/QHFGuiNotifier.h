#ifndef QHFGUINOTIFIER_H
#define QHFGUINOTIFIER_H

#include <QHash>
#include <QVariant>
#include <QVariantMap>
#include <optional> // 用于增强 get 方法

namespace QHF {
    enum class QHFNotifierType : std::uint8_t {
        NONE,
        // 调试type
        TEST_EVENT1,
        TEST_EVENT2,
        DEBUG_EVENT,
        // filemenurelated
        NEW_FILE,
        OPEN_FILE,
        OPEN_RECENTLY,
        SAVE_FILE,
        SAVE_AS,
        SAVE_COPY,
        CLOSE_FILE,
        IMPORT_FILE,
        IMPORT_FILE_1,
        IMPORT_FILE_2,
        EXPORT_FILE,
        EXPORT_FILE_1,
        EXPORT_FILE_2,
        SYSTEM_EXIT,
        // 编辑menurelated
        UNDO,
        REDO,
        REDO_HISOTRY,
        REPEAT,
        REPEAT_HISOTRY,
        PREFERENCES_SETTINGS,
        // 视图menurelated
        NEW_SIMPLE_WINDOW,
        NEW_MAIN_WINDOW,
        TOGGLE_FULL_SCREEN,
        NEXT_WORKSPACE,
        PREV_WORKSPACE,
        TOGGLE_STATUS_BAR,
        SAVE_SCREENSHOT,
        SAVE_SCREENSHOT_EDITOR,
        TOGGLE_DEBUG_WINDOW,
        // 帮助
        GET_HELP,
        MEMUAL_DOC,
        ABOUT_US,
        //
        APP_WINDOW_ACTIVATED,
        APP_WINDOW_ACTIVATED_WORKSPACE,
        APP_WINDOW_ACTIVATED_EDITOR,
        APP_WINDOW_MOVED,
        APP_WINDOW_MOVED_WORKSPACE,
        APP_WINDOW_MOVED_EDITOR,
        // 新增notificationtype
        REPORT_ACTIVATION,
        // workspacerelated
        SET_WORKSPACE_INDXE,
        // Editor related
        SWITCH_EDITOR_TYPE,
        SPLIT_EDITOR_HORIZONTAL,
        SPLIT_EDITOR_VERTICAL,
        REMOVE_EDITOR,
        // Mode related
        SWITCH_MODE_TYPE,
        // File staterelated
        FILE_STATE_MODIFIED, ///< File修改了
        FILE_STATE_CLOSED,   ///< File关闭
        FILE_STATE_OPENED,   ///< File打开
        FILE_STATE_SAVED,    ///< File保存
        FILE_STATE_IMPORTED, ///< File导入
        FILE_STATE_EXPORTED, ///< File导出
        // ... extend with more notification types here
        // 新window扩展info
        NEW_APP_WINDOW_INFO_WINDOW,
        NEW_APP_WINDOW_INFO_WORKSPACE,
        NEW_APP_WINDOW_INFO_EDITOR,
    };

    /**
     * @brief  (see description)
     */
    namespace QHFNotifierNameUtils {

        static const QHash<QString, QHFNotifierType> notifierMap = {
            {"None", QHFNotifierType::NONE},
            {"TestEvent1", QHFNotifierType::TEST_EVENT1},
            {"TestEvent2", QHFNotifierType::TEST_EVENT2},
            {"DebugEvent", QHFNotifierType::DEBUG_EVENT},
            {"NewFile", QHFNotifierType::NEW_FILE},
            {"OpenFile", QHFNotifierType::OPEN_FILE},
            {"OpenRecently", QHFNotifierType::OPEN_RECENTLY},
            {"SaveFile", QHFNotifierType::SAVE_FILE},
            {"SaveAs", QHFNotifierType::SAVE_AS},
            {"SaveCopy", QHFNotifierType::SAVE_COPY},
            {"CloseFile", QHFNotifierType::CLOSE_FILE},
            {"ImportFile", QHFNotifierType::IMPORT_FILE},
            {"ImportFile1", QHFNotifierType::IMPORT_FILE_1},
            {"ImportFile2", QHFNotifierType::IMPORT_FILE_2},
            {"ExportFile", QHFNotifierType::EXPORT_FILE},
            {"ExportFile1", QHFNotifierType::EXPORT_FILE_1},
            {"ExportFile2", QHFNotifierType::EXPORT_FILE_2},
            {"SystemExit", QHFNotifierType::SYSTEM_EXIT},
            {"Undo", QHFNotifierType::UNDO},
            {"Redo", QHFNotifierType::REDO},
            {"RedoHistory", QHFNotifierType::REDO_HISOTRY},
            {"Repeat", QHFNotifierType::REPEAT},
            {"RepeatHistory", QHFNotifierType::REPEAT_HISOTRY},
            {"PreferencesSettings", QHFNotifierType::PREFERENCES_SETTINGS},
            {"NewSimpleWindow", QHFNotifierType::NEW_SIMPLE_WINDOW},
            {"NewMainWindow", QHFNotifierType::NEW_MAIN_WINDOW},
            {"ToggleFullScreen", QHFNotifierType::TOGGLE_FULL_SCREEN},
            {"NextWorkspace", QHFNotifierType::NEXT_WORKSPACE},
            {"PrevWorkspace", QHFNotifierType::PREV_WORKSPACE},
            {"ToggleStatusBar", QHFNotifierType::TOGGLE_STATUS_BAR},
            {"SaveScreenshot", QHFNotifierType::SAVE_SCREENSHOT},
            {"SaveScreenshotEditor", QHFNotifierType::SAVE_SCREENSHOT_EDITOR},
            {"ToggleDebugWindow", QHFNotifierType::TOGGLE_DEBUG_WINDOW},
            {"GetHelp", QHFNotifierType::GET_HELP},
            {"MemualDoc", QHFNotifierType::MEMUAL_DOC},
            {"AboutUs", QHFNotifierType::ABOUT_US},
            {"AppWindowActivated", QHFNotifierType::APP_WINDOW_ACTIVATED},
            {"AppWindowActivatedWorkspace", QHFNotifierType::APP_WINDOW_ACTIVATED_WORKSPACE},
            {"AppWindowActivatedEditor", QHFNotifierType::APP_WINDOW_ACTIVATED_EDITOR},
            {"AppWindowMoved", QHFNotifierType::APP_WINDOW_MOVED},
            {"AppWindowMovedWorkspace", QHFNotifierType::APP_WINDOW_MOVED_WORKSPACE},
            {"AppWindowMovedEditor", QHFNotifierType::APP_WINDOW_MOVED_EDITOR},
            {"ReportActivation", QHFNotifierType::REPORT_ACTIVATION},
            {"SetWorkspaceIndex", QHFNotifierType::SET_WORKSPACE_INDXE},
            {"SwitchEditorType", QHFNotifierType::SWITCH_EDITOR_TYPE},
            {"SplitEditorHorizontal", QHFNotifierType::SPLIT_EDITOR_HORIZONTAL},
            {"SplitEditorVertical", QHFNotifierType::SPLIT_EDITOR_VERTICAL},
            {"RemoveEditor", QHFNotifierType::REMOVE_EDITOR},
            {"SwitchModeType", QHFNotifierType::SWITCH_MODE_TYPE},
            {"FileStateChanged", QHFNotifierType::FILE_STATE_MODIFIED},
            {"FileStateClosed", QHFNotifierType::FILE_STATE_CLOSED},
            {"FileStateOpened", QHFNotifierType::FILE_STATE_OPENED},
            {"FileStateSaved", QHFNotifierType::FILE_STATE_SAVED},
            {"FileStateImported", QHFNotifierType::FILE_STATE_IMPORTED},
            {"FileStateExported", QHFNotifierType::FILE_STATE_EXPORTED},
            {"NewAppWindowInfoWindow", QHFNotifierType::NEW_APP_WINDOW_INFO_WINDOW},
            {"NewAppWindowInfoWorkspace", QHFNotifierType::NEW_APP_WINDOW_INFO_WORKSPACE},
            {"NewAppWindowInfoEditor", QHFNotifierType::NEW_APP_WINDOW_INFO_EDITOR},
        };

        /**
         * @brief  (see description)
         * @param type notificationtype* @return string*/
        inline QString toString(QHFNotifierType type)
        {
            return notifierMap.key(type);
        }

        /**
         * @brief  (see description)
         * @param str string* @return notificationtype*/
        inline QHFNotifierType fromString(const QString& str) {
            return notifierMap.value(str, QHFNotifierType::NONE);
        }
    } // namespace QHFNotifierNameUtils

    /**
     * @brief  (see description)
     */
    struct QHFNotifier {
        QHFNotifierType type; ///< Event type
        QVariantMap   data; ///< 附加数据

        QHFNotifier() : type(QHFNotifierType::NONE){}
        explicit QHFNotifier(QHFNotifierType typ) : type(typ) {}
        // 支持dynamicevent
        explicit QHFNotifier(const QString& typ) : type(QHFNotifierNameUtils::fromString(typ)) {}

        template <typename T> void set(const QString& key, const T& value) {
            data.insert(key, QVariant::fromValue(value));
        }

        void set(const QString& key, const char* value) { data.insert(key, QString(value)); }

        // 修改 get 方法, Returns  std::optional
        template <typename T> [[nodiscard]] std::optional<T> get(const QString& key) const {
            if (data.contains(key)) {
                return data.value(key).template value<T>();
            }
            return std::nullopt;
        }
    };

    /**
     * @brief Notification (see description)
     */
    struct QHFActivationEvent {
        /**
         * @brief Notification (see description)
         */
        enum class Type : std::uint8_t {
            NONE,
            MODE_ACTIVE,
            EDITOR_ACTIVE,
            WORKSPACE_ACTIVE,
        };

        Type        type      = Type::NONE; ///< Event type
        QObject*    source    = nullptr; ///< Event source (模式、编辑器或工作区实例)
        QString     reason    = "";      ///< Trigger reason (如 "mouse press"、"focus in")
        bool        immediate = false;   ///< is 否立即发送
        QVariantMap context;///< Additional context (如模式名称、编辑器指针)

        // defaultConstructor
        QHFActivationEvent() = default;

        QHFActivationEvent(Type typ, QObject* src, QString rea, bool imm = false)
            : type(typ), source(src), reason(std::move(rea)), immediate(imm) {}
    };

    namespace QHFActivationTypeUtils {
        inline QString toString(QHFActivationEvent::Type type) {
            switch (type) {
            case QHFActivationEvent::Type::MODE_ACTIVE:
                return "MODE_ACTIVE";
            case QHFActivationEvent::Type::EDITOR_ACTIVE:
                return "EDITOR_ACTIVE";
            case QHFActivationEvent::Type::WORKSPACE_ACTIVE:
                return "WORKSPACE_ACTIVE";
            default:
                return "NONE";
            }
        }

        inline QHFActivationEvent::Type fromString(const QString& str) {
            if (str == "MODE_ACTIVE") {
                return QHFActivationEvent::Type::MODE_ACTIVE;
            }
            if (str == "EDITOR_ACTIVE") {
                return QHFActivationEvent::Type::EDITOR_ACTIVE;
            }
            if (str == "WORKSPACE_ACTIVE") {
                return QHFActivationEvent::Type::WORKSPACE_ACTIVE;
            }
            return QHFActivationEvent::Type::NONE;
        }

    } // namespace QHFActivationTypeUtils

    // 为 QHFNotifierType 定义 qHash
    inline uint qHash(QHFNotifierType type, uint seed = 0) {
        return ::qHash(static_cast<int>(type), seed);
    }

    // 为 QPair<T*, QHFNotifierType> 定义 qHash
    template <typename T> inline uint qHash(const QPair<T*, QHFNotifierType>& key, uint seed = 0) {
        QtPrivate::QHashCombine combine;
        uint                    h1 = ::qHash(key.first, seed);
        uint                    h2 = ::qHash(key.second, seed);
        return combine(seed, h1) ^ combine(seed, h2);
    }

} // namespace QHF

// Note: 册 QHFActivationEvent 为 QVariant 兼容type, ensure全局作用域
Q_DECLARE_METATYPE(QHF::QHFActivationEvent)

#endif // HGUINOTIFIER_H
