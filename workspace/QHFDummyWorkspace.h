#ifndef QHFDUMMYWORKSPACE_H
#define QHFDUMMYWORKSPACE_H

#include "QHFWorkspaceBase.h"

namespace QHF {
    /**
     * @brief  (see description)
     * @details (see source).
     */
    class QHFDummyWorkspace : public QHFWorkspaceBase {
        Q_OBJECT

      public:
        static constexpr int SUGGESTED_ORDER = 8000;

        explicit QHFDummyWorkspace(QWidget* parent = nullptr, bool isInitialize = true);
        ~QHFDummyWorkspace() override = default;
        [[nodiscard]] QString        workspaceName() const override;
        [[nodiscard]] QHFWorkspaceType workspaceType() const override;

      private:
        void initializeImpl() override;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleNotifyImpl(const QHFNotifier& notifier) override;
    };
} // namespace QHF

#endif // HDUMMYWORKSPACE_H
