#ifndef QHFSTATEMANAGER_H
#define QHFSTATEMANAGER_H

/**
 * State manager
 * 1. File state, 同步File state到allwindow, title和status bar, 最新 filelist给Menus
 *   - currentfile
 *   - file历史
 *   - file元data (如file大小、create时间、修改时间等)
 *  struct FileState { QString path; QString name; bool isDirty; QStringList recentFiles; }
 * 2. workspacestate, 支持workspacecreate, 切换, save/回复
 *   - workspaceID
 *   - workspaceLayout？
 *   - currentworkspace
 * 3. 用户set, 听过个性化set
 *   - font大小, 语言, 快捷键
 *   - StatusBar, toolbar 可见性
 *  使用 QSettings 或 QMap<QString, QVariant> 存储set
 * 4. editorstate
 *   - currenteditor
 *   - editortopbottom文, 文本editorcontent, 3D视图角度
 *   - editortype
 * 5. 运行是state, 支持dynamicupdateUI
 *   - 运行mode, 运行configuration, 调试mode？
 *   - 操作历史, UNDO / REDO 栈
 *   - notificationstate, currentnotification或error消息
 */

#include <QObject>
#include <QPointer>
#include <QRect>
#include <QVariant>

namespace QHF {

    /**
     * @breif Window statetype
     */
    struct WindowState {
        QRect geometry;
        int currentWorkspaceIndex = 0; // 类型为 QHFAppMainWindow 时有效
        bool isShowStatusBar = true;   // 类型为 QHFAppMainWindow 时有效
        bool isFullScreen = false;
    };

    struct FileState {
        QString path;
        QString name;
        bool isDirty;
        QStringList recentFiles;
        bool isLoaded = false; // 是否已加载
    };

    class QHFStateManager : public QObject {
        Q_OBJECT

      private:
        QWidget* m_activeWindow;                    ///< Current激活窗口
        QMap<QString, QVariant> m_stateData;        ///< 用户Set 
        QMap<QWidget*, WindowState> m_windowStates; ///< Window state
        FileState m_fileState;                      ///< File state (暂时支持一个文件)
        QString m_currentLanguage;                  ///< Current语言
        bool m_isBlockingWindowActive;              ///< 阻塞window激活state
        QStringList m_favoritePlaces;///< File夹收藏夹

      public:
        static QHFStateManager& instance();

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void saveState();

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void loadState();

        /**
         * @brief  (see description)
         */
        void cleanup();

        /// ---- Window state管理 ----
        /**
         * @brief  (see description)
         * @return 阻塞windowactivatestate*/
        [[nodiscard]] bool isBlockingWindowActive() const;

        /**
         * @brief  (see description)
         * @details (see source) WindowState
         * @param isBlocking is 否阻塞*/
        void setBlockingWindowActive(bool isBlocking);

        /**
         * @brief Returns current  (see description)
         * @return current activatewindow*/
        [[nodiscard]] QWidget* activeWindow() const;

        /**
         * @brief Sets current  (see description)
         * @param window window*/
        void setActiveWindow(QWidget* window);

        /**
         * @brief  (see description)
         * @return window数量*/
        [[nodiscard]] int countWindows() const;

        /**
         * @brief  (see description)
         * @param window window* @return windowis 否存at */
        bool existWindow(QWidget* window) const;

        /**
         * @brief  (see description)
         */
        void addWindow(QWidget* window);

        /**
         * @brief  (see description)
         */
        void removeWindow(QWidget* window);

        /**
         * @brief  (see description)
         * @details (see source) WindowState
         */
        [[nodiscard]] WindowState getWindowState(QWidget* window) const;

        /**
         * @brief  (see description)
         * @details (see source) WindowState
         */
        void setWindowState(QWidget* window, const WindowState& state);

        /**
         * @brief  (see description)
         * @details (see source)Returns 
         */
        [[nodiscard]] int getMainWinWorkspaceIndex(QWidget* window) const;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void setMainWinWorkspaceIndex(QWidget* window, int index);

        /**
         * @brief Get state (see description)
         * @details (see source)Returns false
         */
        [[nodiscard]] bool getStatusBarVisible(QWidget* window) const;

        /**
         * @brief  (see description)
         * @details (see source) WindowState
         */
        void setStatusBarVisible(QWidget* window, bool visible);

        /// ---- File state管理 ----
        /**
         * @brief  (see description)
         * @details
         * resetFile state, 如果File state不exist, filename为"Untitled", 是否为脏file为true, update最近openfilelist
         * @return File state (reference)
         */
        void resetFileState();

        /**
         * @brief  (see description)
         * @details (see source) FileState
         */
        void updateRecentFiles();

        /**
         * @brief  (see description)
         * @details (see source) FileState
         * @return File state (reference)
         */
        [[nodiscard]] const FileState& getFileState();

        /**
         * @brief  (see description)
         * @details (see source) FileState
         */
        void setFileState(const FileState& state);

        // --- 现实语言set ---
        /**
         * @brief Sets current  (see description)
         * @param lang current 系统语言*/
        void setCurrentLanguage(const QString& lang);

        /**
         * @brief Returns current  (see description)
         * @return current 系统语言*/
        [[nodiscard]] QString getCurrentLanguage() const;

        // --- file夹收藏夹 ---
        /**
         * @brief  (see description)
         * @return file收藏夹*/
        [[nodiscard]] QStringList getFavoritePlaces() const;

        /**
         * @brief  (see description)
         * @param places file收藏夹*/
        void setFavoritePlaces(const QStringList& places);

      protected slots:
        /**
         * @brief  (see description)
         * @details (see source)
         */
        void onFocusChanged(QWidget* old, QWidget* now);

      private:
        /**
         * @brief Constructor
         * @details (see source), External creation disabled
         */
        QHFStateManager();

        /**
         * @brief Destructor
         */
        ~QHFStateManager() override = default;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        [[nodiscard]] QWidget* findTargetWindow(QWidget* now) const;

        /**
         * @brief State (see description)
         * @details (see source) QStandardPaths::AppConfigLocation
         * @return statefilepath*/
        QString defaultAppStatePath() const;

        /**
         * @brief  (see description)
         */
        QJsonObject loadAppState(const QString& path) const;

        /**
         * @brief  (see description)
         */
        void saveAppState(const QString& path, const QJsonObject& data) const;
    };
} // namespace QHF

#endif // HSTATEMANAGER_H
