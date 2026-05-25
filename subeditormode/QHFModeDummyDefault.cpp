#include "QHFModeDummyDefault.h"

#include "QHFModeFactory.h"

#include <QDebug>
#include <QLabel>

namespace QHF {

    /// --- QHFModeDummyDefault implementation ---
    QHF_REGISTER_MODE(EditorTypeUtils::toString(EditorType::DUMMY), QHFModeDummyDefault,
                       QHFModeType::DUMMY_DEFAULT);
    QHFModeDummyDefault::QHFModeDummyDefault(IEditorContext* context, QWidget* parent)
        : QHFModeBase(context, parent) {}

    QString QHFModeDummyDefault::modeName() const {
        return QHFModeTypeUtils::transName(QHFModeType::DUMMY_DEFAULT);
    }

    void QHFModeDummyDefault::cleanup() { m_context->setModeState(modeName(), QVariant()); }

    void QHFModeDummyDefault::handleNotifyImpl(const QHFNotifier& notifier) {}
} // namespace QHF
