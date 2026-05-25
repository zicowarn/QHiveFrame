#include "QHFCustomActionWidgetDummy.h"

#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

#include "QHFCustomActionWidgetFactory.h"
#include "QHFThemeManager.h"

namespace QHF {

    /// --- QHFCustomActionWidgetDummy Implementation ---
    QHF_REGISTER_CUSTOM_WIDGET(QHFCustomActionWidgetType::DUMMY, QHFCustomActionWidgetDummy)
    QHFCustomActionWidgetDummy::QHFCustomActionWidgetDummy(ActionType type, QWidget* parent)
        : QHFCustomActionWidgetBase(type, parent) {
        //
        constexpr int defaultMargin  = 5;
        constexpr int defaultSpacing = 0;
        // Set object name
        setObjectName("QHFCustomActionWidgetDummy");
        // Set main layout
        m_layout = new QVBoxLayout();
        // Set layout properties
        m_layout->setContentsMargins(defaultMargin, defaultMargin, defaultMargin, defaultMargin);
        //
        m_layout->setSpacing(defaultSpacing);
        // Set layout
        setLayout(m_layout);
    }

    void QHFCustomActionWidgetDummy::updateContext() {}

    QVariantMap QHFCustomActionWidgetDummy::getData() const
    {
        return {};
    }

    ActionType QHFCustomActionWidgetDummy::getActionType() const
    {
        return m_type;
    }

    void QHFCustomActionWidgetDummy::applyTheme() {
        // Applies theme
        const auto& theme     = QHFThemeManager::instance().theme();
        QString     commStyle = QString("QWidget#QHFCustomActionWidgetDummy {"
                                            " border-radius: %1px;"
                                            " padding: %2;"
                                            "}")
                                .arg(theme.borderRadius)
                                .arg(theme.cusMenuPadding);

        QString menuStyle = QString("QMenu#HCustActiMenu {"
                                    " border-radius: %1px;"
                                    " padding: %2;"
                                    "}"
                                    "QMenu#HCustActiMenu::item {"
                                    " border-radius: %1px;"
                                    " padding: %3;"
                                    "}"
                                    "QMenu#HCustActiMenu::item:selected {"
                                    " background-color: %4;"
                                    "}")
                                .arg(theme.borderRadius)
                                .arg(theme.menuPadding)
                                .arg(theme.menuItemPadding)
                                .arg(theme.menuItemSelectedColor.name());
        setStyleSheet(commStyle + menuStyle);
    }

    void QHFCustomActionWidgetDummy::initializeImpl() {
        // Add a QMenu
        auto* menu = new QMenu();
        // Set object name
        menu->setObjectName("HCustActiMenu");
        // Add menu items
        menu->addAction("Action 1", [this]() { emit finished(); });
        menu->addAction("Action 2", [this]() { emit finished(); });
        menu->addAction("Action 3");
        // Add to layout
        m_layout->addWidget(menu);

        /*
        // Add a QLabel
        auto* button = new QPushButton("Create the required widget!!!", this);
        // label->setAlignment(Qt::AlignCenter);
        button->setStyleSheet("QPushButton {background-color: #fff; color: #333;}");
        m_layout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this]() {
            dispatchActionType(m_type, [this](auto& dispatcher, auto type) {
                if constexpr (std::is_same_v<std::decay_t<decltype(type)>, QHFNotifierType>) {
                    QHFNotifier note(type);
                    note.set("source", QString("PushButtonWidget"));
                    note.set("text", getData()["text"].toString());
                    QHFNotifierCenter::instance().publish(note);
                } else {
                    QHFGuiEventBus::instance().emitEvent(type);
                }
            });
            emit finished();
        });
        */
    }
} // namespace QHF
