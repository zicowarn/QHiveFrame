#ifndef QHFAPPMAINWINDOW_H
#define QHFAPPMAINWINDOW_H

#include <QObject>

#include "QHFAppHeader.h"
#include "QHFAppStatusbar.h"
#include "QHFEditorBase.h"
#include "QHFUIUtils.h"
#include "QHFWorkspaceBase.h"
#include "QHFWorkspaceFactory.h"

#include <QList>
#include <QMainWindow>
#include <QMenuBar>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

namespace QHF {

    /**
     * @brief  (see description)
     * @details (see source)
     * QMainWindow, implementation了window 基本功能, 包括menu管理器、status bar、workspace管理器、workspace switch等.
     */
    class QHFAppMainWindow : public QMainWindow, public QHFThemedCRTP<QHFAppMainWindow> {
        Q_OBJECT

      public:
        /**
         * @brief  (see description)
         */
        struct FileInfo {
            QString fileName; ///< File name
            QString filePath; ///< File path
        };

      private:
        const bool m_isFirstCreation;         /// 是否为第一个窗口
        QVBoxLayout* m_mainLayout;            /// 主Layout
        QHFAppHeader* m_header;                 /// Menu manager
        QHFAppStatusBar* m_statusBar;           /// 状态栏
        QStackedWidget* m_workspaceContainer; ///< workspaceContainer
        QList<QHFWorkspaceBase*> m_workspaces;  ///< Workspace list
        QHFWorkspaceBase* m_activeWorkspace;    ///< Active workspace

      public:
        /**
         * @brief Constructor
         * @param parent Parent window
         * @param firstCreation is 否 as newwindow, 第一个window
         */
        explicit QHFAppMainWindow(QWidget* parent = nullptr, bool firstCreation = true);
        ~QHFAppMainWindow() override = default;

      Q_SIGNALS:
        /**
         * @brief Emitted (see description)
         */
        void fileNewRequested();

        /**
         * @brief Emitted (see description)
         */
        void fileOpened(const QString& filePath);

        /**
         * @brief Emitted (see description)
         */
        void fileClosed();

      protected:
        /**
         * @brief State (see description)
         */
        void changeEvent(QEvent* event) override;

        /**
         * @brief  (see description)
         */
        void closeEvent(QCloseEvent* event) override;

        /**
         * @brief  (see description)
         */
        void moveEvent(QMoveEvent* event) override;

      private:
        /**
         * @brief Initializes
         */
        void initialize();

        /**
         * @brief  (see description)
         */
        void updateTitle();

        /**
         * @brief  (see description)
         * @return is 否success*/
        bool newFile();

        /**
         * @brief  (see description)
         * @param openedFile openfileinfo* @return is 否success*/
        bool openFile(FileInfo& openedFile);

        /**
         * @brief  (see description)
         * @param savedFile Save fileinfo* @return is 否success*/
        bool saveFile(FileInfo& savedFile);

        /**
         * @brief  (see description)
         * @return is 否success*/
        bool saveAsFile();

        /**
         * @brief  (see description)
         * @return is 否success*/
        bool importFile();

        /**
         * @brief  (see description)
         * @return is 否success*/
        bool exportFile();

        /**
         * @brief  (see description)
         * @return is 否success*/
        bool closeFile();

        /// --- notification处理 ---
        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNotify(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNewFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleOpenFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleSaveFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleSaveAsFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleCloseFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source)
         * @param notifier notification*/
        void handleFileStateChanged(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification(FILE_STATE_MODIFIED)
         */
        void handleFileStateModified(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification(FILE_STATE_CLOSE, ...OPEN, 等)
         */
        void handleFileStateOtherCases(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleImportFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleExportFile(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleSetWorkspaceIndex(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleToggleFullScreen(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleSystemExit(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleToggleStatusBar(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNewSimpleWindow(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNewMainWindow(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleReportActivation(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handlePreferencesSettings(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleGetHelp(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleAboutUs(const QHFNotifier& notifier);
    };

} // namespace QHF

#endif // HAPPMAINWINDOW_H
