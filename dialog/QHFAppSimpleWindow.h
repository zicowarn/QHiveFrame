#ifndef QHFAPPSIMPLEWINDOW_H
#define QHFAPPSIMPLEWINDOW_H

#include "QHFGuiNotifier.h"
#include "QHFUIUtils.h"
#include "QHFWorkspaceBase.h"

#include <QMainWindow>
#include <QVBoxLayout>

namespace QHF {

    /**
     * @brief  (see description)
     * @details (see source)
     * @tparam Derived subclasstype
     * @tparam SignalType signaltype
     */
    class QHFAppSimpleWindow : public QMainWindow, public QHFThemedCRTP<QHFAppSimpleWindow> {
        Q_OBJECT

      private:
        QHFWorkspaceType m_workspaceType;
        QVBoxLayout* m_layout;

      public:
        /**
         * @brief Constructor
         * @param workspaceType workspacetype* @param parent Parent window
         */
        explicit QHFAppSimpleWindow(QHFWorkspaceType workspaceType, QWidget* parent = nullptr);
        ~QHFAppSimpleWindow() override = default; ///< Destructor

      private:
        /**
         * @brief Initializes
         * @details (see source)Layout
         */
        void initialize();

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void updateTitle();

        /**
         * @brief Interface (see description)
         * @details (see source)
         */
        void applyTheme();

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void handleNotify(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void createWorkspace();

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
    };

} // namespace QHF

#endif // HAPPSIMPLEWINDOW_H
