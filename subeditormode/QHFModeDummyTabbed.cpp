#include "QHFModeDummyTabbed.h"

#include "QHFModeFactory.h"

#include <QDebug>
#include <QLabel>
#include <QTabWidget>

namespace QHF {
    /// --- QHFModeDummyTabbed implementation ---
    QHF_REGISTER_MODE(EditorTypeUtils::toString(EditorType::DUMMY), QHFModeDummyTabbed,
                       QHFModeType::DUMMY_TABBED);
    QHFModeDummyTabbed::QHFModeDummyTabbed(IEditorContext* context, QWidget* parent)
        : QHFModeBase(context, parent) {
        auto* tabWidget = new QTabWidget(this);
        tabWidget->addTab(new QLabel("Tab 1"), "Tab 1");
        tabWidget->addTab(new QLabel("Tab 2"), "Tab 2");
    }
    QString QHFModeDummyTabbed::modeName() const {
        return QHFModeTypeUtils::transName(QHFModeType::DUMMY_TABBED);
    }

    void QHFModeDummyTabbed::handleNotifyImpl(const QHFNotifier& notifier) {}

    /*
    QWidget* QHFModeDummyTabbed::createContent() {
        auto* tabWidget = new QTabWidget;
        tabWidget->addTab(new QLabel("Tab 1"), "Tab 1");
        tabWidget->addTab(new QLabel("Tab 2"), "Tab 2");
        // connect(tabWidget, &QTabWidget::currentChanged, this, &QHFModeBase::update);
        return tabWidget;
    }
        */
} // namespace QHF
