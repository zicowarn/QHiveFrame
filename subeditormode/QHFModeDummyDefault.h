#ifndef QHFMODEDUMMYDEFAULT_H
#define QHFMODEDUMMYDEFAULT_H

#include "QHFModeBase.h"
#include "QHFUIIEditorContext.h"
#include "QHFUIIModeContext.h"

namespace QHF {
    class QHFModeDummyDefault : public QHFModeBase {
      public:
        explicit QHFModeDummyDefault(IEditorContext* context, QWidget* parent = nullptr);
        ~QHFModeDummyDefault() override = default; ///< Virtual destructor
        [[nodiscard]] QString modeName() const override;
        void                  cleanup() override;

      private:
        void handleNotifyImpl(const QHFNotifier& notifier) override;
    };
} // namespace QHF

#endif // HMODEDUMMYDEFAULT_H
