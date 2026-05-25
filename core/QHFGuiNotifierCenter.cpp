#include "QHFGuiNotifierCenter.h"

#include <QDebug>
#include <QTimer>

namespace QHF {

    /// -------- QHFNotifierCenter implementation --------
    QHFNotifierCenter& QHFNotifierCenter::instance() {
        static QHFNotifierCenter inst;
        return inst;
    }

    void QHFNotifierCenter::publish(const QHFNotifier& notifier, int delayMs) {
        if (delayMs <= 0) {
            emit notify(notifier);
            qDebug() << "Published notifier:" << QHFNotifierNameUtils::toString(notifier.type);
        } else {
            QTimer::singleShot(delayMs, this, [this, notifier]() {
                emit notify(notifier);
                qDebug() << "Published delayed notifier:"
                         << QHFNotifierNameUtils::toString(notifier.type);
            });
        }
    }
} // namespace QHF
