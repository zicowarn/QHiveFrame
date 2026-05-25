#ifndef QHFMODEDUMMYTABBED_H
#define QHFMODEDUMMYTABBED_H

#include "QHFModeBase.h"
#include "QHFUIIEditorContext.h"
#include "QHFUIIModeContext.h"

namespace QHF {
    class QHFModeDummyTabbed : public QHFModeBase {
      public:
        explicit QHFModeDummyTabbed(IEditorContext* context, QWidget* parent = nullptr);
        ~QHFModeDummyTabbed() override = default; ///< Virtual destructor
        [[nodiscard]] QString modeName() const override;

      private:
        void handleNotifyImpl(const QHFNotifier& notifier) override;
    };
} // namespace QHF

#endif // HMODEDUMMYTABBED_H
