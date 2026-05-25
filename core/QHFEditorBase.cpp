#include "QHFEditorBase.h"

#include "QHFEditorFactory.h"
#include "QHFGuiNotifierCenter.h"
#include "QHFModeBase.h"
#include "QHFModeFactory.h"
#include "QHFThemeManager.h"

#include <QDebug>
#include <QGridLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

namespace QHF {

    constexpr int defaultMargin  = 5;
    constexpr int defaultSpacing = 5;

    /// --- QHFEditorBase implementation ---
    QHFEditorBase::QHFEditorBase(QWidget* parent) : QWidget(parent) {
        // InitializesMember variable
        m_parent          = parent;
        m_headerLayout    = nullptr;
        m_header          = nullptr;
        m_editorCombo     = nullptr;
        m_modeCombo       = nullptr;
        m_headerComboArea = nullptr;
        m_headerContent   = nullptr;
        m_buttonArea      = nullptr;
        m_contentLayout   = nullptr;
        m_contentScroll   = nullptr;
        m_currentMode     = nullptr;
        m_hasModeCombo    = false;
        m_modeStates.clear();

        // setstyle, 圆角border
        setObjectName("QHFEditorBase");
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
    }

    QHFEditorBase::~QHFEditorBase() {
        if (m_currentMode != nullptr) {
            m_currentMode->cleanup();
        }
    }

    void QHFEditorBase::requestUpdate() { updateContent(); }

    QGridLayout* QHFEditorBase::headerLayout() const { return m_headerLayout; }

    QHBoxLayout* QHFEditorBase::contentLayout() const { return m_contentLayout; }

    QString QHFEditorBase::currentModeName() const { return m_currentMode->modeName(); }

    void QHFEditorBase::setEditorPreSelectMode(const QString& modeName) {
        if (!modeName.isEmpty() && m_modeCombo != nullptr) {
            qDebug() << "Setting pre-select mode:" << modeName;
            // checkcurrentm_modeCombo 是否和 modeName 一致
            if (m_modeCombo->currentText() == modeName) {
                return; // 一致, 无需切换
            }
            auto modeNames = collectModeNames(); // 子类实现Returns mode name列表
            for (const QPair<QString, QString>& mode : modeNames) {
                if (mode.second == modeName) {
                    qDebug() << "Setting pre-select mode:" << mode.first;
                    int index = m_modeCombo->findText(mode.first);
                    if (index != -1) {
                        m_modeCombo->setCurrentIndex(index);
                        return;
                    }
                }
            }
        }
        // Not foundmode, 切换到defaultmode
        switchMode(defaultModeName());
    }

    void QHFEditorBase::setModeState(const QString& modeName, const QVariant& state) {
        m_modeStates[modeName] = state;
    }

    QVariant QHFEditorBase::getModeState(const QString& modeName) const {
        return m_modeStates.value(modeName);
    }

    void QHFEditorBase::initialize() {
        qDebug() << "Initializing editor:" << editorName();
        auto modeNames = collectModeNames();   // 子类实现Returns mode name列表
        setHasModeCombo(modeNames.size() > 1); // 模式下拉框
        setupLayout();                         // InitializesLayout
        applyTheme();                          // Applies theme
        initializeEditorCombo(editorName());   // Initializes编辑器下拉框
        initializeImpl(modeNames);             // 子类完成Initializes模式下拉框

        // connectionsignal
        if (m_editorCombo != nullptr) {
            connect(m_editorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                    [=](int index) {
                        const auto& displayName = m_editorCombo->itemText(index);
                        const auto& editorType  = m_editorCombo->itemData(index).toString();
                        qDebug() << "Switching editor type to:" << editorType
                                 << "with display name:" << displayName;
                        QHFNotifier noti(QHFNotifierType::SWITCH_EDITOR_TYPE);
                        noti.set("editor", this);
                        noti.set("newType", editorType);
                        QHFNotifierCenter::instance().publish(noti);
                    });
        }
        if (m_modeCombo != nullptr) {
            connect(m_modeCombo, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
                    this, [=]() {
                        // Publishes notification
                        publishEditorActive("mode switch", true);
                        //
                        handleModeSwitch(m_modeCombo->currentText());
                    });
        }

        // event处理
        connect(&QHFNotifierCenter::instance(), &QHFNotifierCenter::notify, this,
                &QHFEditorBase::handleNotify);
    }

    void QHFEditorBase::initializeEditorCombo(const QString& preSelectEditorName) {
        if (m_editorCombo == nullptr) {
            qWarning() << "m_editorCombo not initialized; call setupLayout first";
            return;
        }
        m_editorCombo->clear();
        QStringList editors = QHFEditorRegistry::instance().editorNames();
        for (const QString& editor : editors) {
            EditorType type        = EditorTypeUtils::fromString(editor);
            QString    displayName = EditorTypeUtils::toTransName(type);
            qDebug() << "Registering editor:" << editor << "with display name:" << displayName;
            m_editorCombo->addItem(displayName, editor);
        }
        if (!preSelectEditorName.isEmpty()) {
            // check defaultEditor 是否valid
            for (const QString& editor : editors) {
                if (editor == preSelectEditorName) {
                    EditorType currentType = EditorTypeUtils::fromString(preSelectEditorName);
                    QString    displayName = EditorTypeUtils::toTransName(currentType);
                    m_editorCombo->setCurrentText(displayName);
                }
            }
        }
    }

    void QHFEditorBase::initializeModeCombo(const QList<QPair<QString, QString>>& modes,
                                          const QString&                        defaultMode) {

        if (m_modeCombo != nullptr && hasModeCombo()) {
            qDebug() << "m_modeCombo already initialized";
            m_modeCombo->clear();
            //
            for (const QPair<QString, QString>& mode : modes) {
                QString displayName = mode.first;  // 显示名称
                QString modeName    = mode.second; // 模式名称
                qDebug() << "Registering mode:" << modeName << "with display name:" << displayName;
                m_modeCombo->addItem(displayName, modeName);
            }
            // setdefaultmode
            if (!defaultMode.isEmpty()) {
                // check defaultMode 是否valid
                for (const QPair<QString, QString>& mode : modes) {
                    if (mode.second == defaultMode) {
                        int index = m_modeCombo->findText(mode.first);
                        if (index != -1) {
                            m_modeCombo->setCurrentIndex(index); // 此时信号尚未绑定
                            handleModeSwitch(m_modeCombo->currentText());
                            break;
                        }
                    }
                }
            } else {
                m_modeCombo->setCurrentIndex(-1); // 默认模式为第一个
            }
        } else {
            qWarning() << "m_modeCombo not initialized or solon mode case";
            switchMode(defaultMode);
        }
    }

    IModeContext* QHFEditorBase::createMode(const QString& modeName) {
        QString       editorNameStr = editorName(); // Returns editor type (如"DUMMY")
        IModeContext* mode = QHFModeRegistry::instance().createMode(EditorName{editorNameStr},
                                                                  ModeName{modeName}, this, this);
        if (mode == nullptr) {
            qWarning() << "Failed to create mode:" << modeName << "for editor:" << editorNameStr;
        }
        return mode;
    }

    void QHFEditorBase::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    QWidget* QHFEditorBase::createHeaderComboArea() {
        m_headerComboArea = new QWidget(m_header);
        auto* comboLayout = new QHBoxLayout(m_headerComboArea);
        comboLayout->setContentsMargins(0, 0, 0, 0);
        comboLayout->setSpacing(defaultSpacing);

        // editor switch按钮
        m_editorCombo = new QComboBox(m_header);
        m_editorCombo->setEditable(false);
        m_editorCombo->setMinimumWidth(QHFEditorBase::ComboEditorMinWidth);
        // set尺寸策略, 高度撑满, 宽度according tocontent调整
        m_editorCombo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        m_editorCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        m_editorCombo->setObjectName("HeaderCombo");
        m_editorCombo->setToolTip(tr("Switch editor type"));
        comboLayout->addWidget(m_editorCombo);

        // mode选择bottom拉框
        if (hasModeCombo()) {
            if (m_modeCombo == nullptr) {
                m_modeCombo = new QComboBox(m_header);
                m_modeCombo->setEditable(false);
                m_modeCombo->setMinimumWidth(QHFEditorBase::ComboModeMinWidth);
                // set尺寸策略, 高度撑满, 宽度according tocontent调整
                m_modeCombo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                m_modeCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
                m_modeCombo->setToolTip(tr("Switch editor mode"));
                m_modeCombo->setObjectName("HeaderCombo");
                comboLayout->addWidget(m_modeCombo);
            }
            // 已有 m_modeCombo 时, initializeModeCombo 会清empty并重新填充
        } else {
            if (m_modeCombo != nullptr) {
                m_modeCombo->disconnect(); // 断开信号
                m_modeCombo->deleteLater();
                m_modeCombo = nullptr;
            }
        }
        return m_headerComboArea;
    }

    QWidget* QHFEditorBase::createHeaderButtonArea() {
        // 按钮Container
        m_buttonArea = new QWidget(m_header);
        // 按钮Layout
        auto* buttonLayout = new QHBoxLayout(m_buttonArea);
        // Layout margin 内边距
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        // Layout 间隙
        buttonLayout->setSpacing(defaultSpacing);

        // split按钮
        auto* splitVBtn = new QToolButton(m_buttonArea);
        splitVBtn->setIcon(QIcon(":/gui/icons/SplitV.svg"));
        splitVBtn->setIconSize(QHFEditorBase::HeaderIconSize);
        splitVBtn->setObjectName("QHFEditorHeaderToolButton");
        splitVBtn->setToolTip("Split Vertically into top and bottom halves");

        // split按钮
        auto* splitHBtn = new QToolButton(m_buttonArea);
        splitHBtn->setIcon(QIcon(":/gui/icons/SplitH.svg"));
        splitHBtn->setIconSize(QHFEditorBase::HeaderIconSize);
        splitHBtn->setObjectName("QHFEditorHeaderToolButton");
        splitHBtn->setToolTip("Split Horizontally into left and right halves");

        // 移除按钮
        auto* removeBtn = new QToolButton(m_buttonArea);
        removeBtn->setIcon(QIcon(":/gui/icons/Close.svg"));
        removeBtn->setIconSize(QHFEditorBase::HeaderIconSize);
        removeBtn->setObjectName("QHFEditorHeaderToolButton");
        removeBtn->setToolTip("Remove this editor");

        // 加入Layout
        buttonLayout->addWidget(splitVBtn);
        buttonLayout->addWidget(splitHBtn);
        buttonLayout->addWidget(removeBtn);

        // connectionsignal
        connect(splitVBtn, &QPushButton::clicked, this, [=]() {
            publishEditorActive("split vertically", false);
            QHFNotifier noti(QHFNotifierType::SPLIT_EDITOR_VERTICAL);
            noti.set("editor", this);
            QHFNotifierCenter::instance().publish(noti);
        });
        connect(splitHBtn, &QPushButton::clicked, this, [=]() {
            publishEditorActive("split horizontally", false);
            QHFNotifier noti(QHFNotifierType::SPLIT_EDITOR_HORIZONTAL);
            noti.set("editor", this);
            QHFNotifierCenter::instance().publish(noti);
        });
        connect(removeBtn, &QPushButton::clicked, this, [=]() {
            publishEditorActive("remove", false);
            QHFNotifier noti(QHFNotifierType::REMOVE_EDITOR);
            noti.set("editor", this);
            QHFNotifierCenter::instance().publish(noti);
        });

        return m_buttonArea;
    }

    bool QHFEditorBase::hasModeCombo() const { return m_hasModeCombo; }

    void QHFEditorBase::setHasModeCombo(bool hasModeCombo) { m_hasModeCombo = hasModeCombo; }

    void QHFEditorBase::applyTheme() {
        // Subclass may override this function以seteditorstyle表
        const auto& theme            = QHFThemeManager::instance().theme();
        int         borderRadius     = theme.borderRadius;
        QColor      borderColor      = theme.borderColor;
        QColor      normalPanelColor = theme.normalPanelColor;
        QString     editorStyle =
            QString(
                "#%1 {border-radius: %2px; border: 1px solid %3; background-color: %4; color: %5;}")
                .arg(objectName())
                .arg(borderRadius)
                .arg(borderColor.name())
                .arg(normalPanelColor.name())
                // .arg("red") // 测试color
                .arg(theme.textColor.name());

        QColor  comboBgColor = theme.backgroundColor;
        QString comboStyle   = QString("QComboBox#HeaderCombo {"
                                         "  background-color: %1;"
                                         "  border: 1px solid %2;"
                                         "  border-radius: 1px;"
                                         "  color: %3;"
                                         "  padding: 1px;"
                                         "}"
                                         "QComboBox#HeaderCombo::indicator {"
                                         "  height: 0px;"
                                         "  width: 0px;"
                                         "}"
                                         "QComboBox#HeaderCombo QAbstractItemView  {"
                                         "  background-color: %4;"
                                         "  border: 1px solid %5;"
                                         "  border-radius: 1px;"
                                         "  color: %3;"
                                         "}"
                                         "QComboBox#HeaderCombo QAbstractItemView::item  {"
                                         "  padding-left: 8px;"
                                         "  image: none;"
                                         "}"

                                         "QComboBox#HeaderCombo::item:selected {"
                                         "  background-color: %6;"
                                         "}"
                                         "QComboBox#HeaderCombo::item:hover {"
                                         "  background-color: %7;"
                                         "}")
                                 .arg(comboBgColor.name())
                                 .arg(borderColor.name())
                                 .arg(theme.textColor.name())
                                 .arg(normalPanelColor.name())
                                 .arg(borderColor.name())
                                 .arg(theme.comboBoxSelecedColor.name())
                                 .arg(theme.comboBoxHoverBgColor.name());

        // 为按钮set统一style
        QColor  iconBtnBgColor      = theme.iconBtnBgColor;
        QColor  iconBtnHoverColor   = theme.iconBtnHoverBgColor;
        QColor  iconBtnPressedColor = theme.iconBtnPressedBgColor;
        QString buttonStyle         = QString("QToolButton#QHFEditorHeaderToolButton {"
                                                      "  border: none;"
                                                      "  border-radius: 2px;"
                                                      "  background-color: transparent;"
                                                      "  color: %1;"
                                                      "  padding: 0;"
                                                      "}"
                                                      "QToolButton#QHFEditorHeaderToolButton:hover {"
                                                      "  background-color: %2;"
                                                      "}"
                                                      "QToolButton#QHFEditorHeaderToolButton:pressed {"
                                                      "  background-color: %3;"
                                                      "}")
                                  .arg(theme.textColor.name())
                                  .arg(iconBtnHoverColor.name())
                                  .arg(iconBtnPressedColor.name());

        // ScrollArea style
        QString scrollAreaStyle = QString("QScrollArea {"
                                          "  border: none;"
                                          "}");

        // style合并
        editorStyle.append(comboStyle);
        editorStyle.append(buttonStyle);
        editorStyle.append(scrollAreaStyle);

        // 应用style
        setStyleSheet(editorStyle);
    }

    void QHFEditorBase::setupLayout() {
        auto* mainLayout = new QVBoxLayout(this);
        // LayoutParameter
        mainLayout->setContentsMargins(0, 0, 0, 0);
        // 主Layout间隙
        mainLayout->setSpacing(defaultSpacing);
        // Header Layout
        setupHeaderLayout(mainLayout);
        // 主体content Layout
        setupContentLayout(mainLayout);
        // 应用Layout
        setLayout(mainLayout);
    }

    void QHFEditorBase::setupHeaderLayout(QBoxLayout* mainLayout) {
        if (m_header != nullptr) {
            qWarning() << "setupHeaderLayout called multiple times for editor:" << editorName();
            return;
        }
        // Header
        m_header = new QWidget(this);
        // 顶部Layout (共一行,  3列)
        m_headerLayout = new QGridLayout(m_header);
        // left top right bottom
        m_headerLayout->setContentsMargins(defaultMargin, defaultMargin, defaultMargin, 0);
        // 顶部间隙
        m_headerLayout->setSpacing(defaultSpacing);
        // NOLINTNEXTLINE(readability-magic-numbers)
        m_headerLayout->setColumnStretch(0, 0); //
        // NOLINTNEXTLINE(readability-magic-numbers)
        m_headerLayout->setColumnStretch(1, 1); // 编辑器下拉框
        // NOLINTNEXTLINE(readability-magic-numbers)
        m_headerLayout->setColumnStretch(2, 0); // 按钮区域

        // Header left侧content (editorbottom拉框)
        m_headerLayout->addWidget(createHeaderComboArea(), 0, 0);

        // Header 中间content (由 Mode 提供)
        m_headerContent = nullptr;

        // 添加不可变 按钮area
        m_headerLayout->addWidget(createHeaderButtonArea(), 0, 2);
        mainLayout->addWidget(m_header);
    }

    void QHFEditorBase::setupContentLayout(QBoxLayout* mainLayout) {
        // ThemeLayout
        m_contentLayout = new QHBoxLayout();
        // left top right bottom
        m_contentLayout->setContentsMargins(defaultMargin, 0, defaultMargin, defaultMargin);
        // Theme滚动Container
        m_contentScroll = new QScrollArea(this);
        // Container缩放set
        m_contentScroll->setWidgetResizable(true);
        // 将滚动Container加入Layout
        m_contentLayout->addWidget(m_contentScroll);
        // 主contentLayout
        mainLayout->addLayout(m_contentLayout);
    }

    void QHFEditorBase::updateContent() {
        if (m_currentMode == nullptr) {
            return;
        }

        // update Header content
        if (m_headerContent != nullptr) {
            m_headerLayout->removeWidget(m_headerContent);
            m_headerContent->setParent(nullptr);
            m_headerContent->deleteLater();
            m_headerContent = nullptr;
        }
        m_headerContent = m_currentMode->createHeaderContent();
        if (m_headerContent != nullptr) {
            m_headerLayout->setParent(m_header);
            m_headerLayout->addWidget(m_headerContent, 0, 1); // 插入到模式下拉框之后
        }

        // Updates content区
        QWidget* content = m_currentMode->createContent();
        if (content != nullptr) {
            m_contentScroll->setWidget(content);
        } else {
            qDebug() << "Warning: createContent returned nullptr for mode"
                     << m_currentMode->modeName();
        }
    }

    void QHFEditorBase::handleModeSwitch(const QString& displayName) {
        // Subclass may override this function以处理mode切换event
        QString modeName = m_modeCombo->currentData().toString(); // 获取 userData 中  modeName
        qDebug() << "Switching to mode:" << modeName << "for editor:" << editorName();
        // Switch mode
        switchMode(modeName);
        //
        QHFNotifier noti(QHFNotifierType::SWITCH_MODE_TYPE);
        noti.set("editor", this);
        noti.set("newType", modeName);
        QHFNotifierCenter::instance().publish(noti);
    }

    void QHFEditorBase::switchMode(const QString& modeName) {
        // create新mode
        if (modeName.isEmpty()) {
            qWarning() << "Empty mode name for editor:" << editorName();
            return;
        }
        if (m_currentMode != nullptr && m_currentMode->modeName() == modeName) {
            qDebug() << "Mode already active:" << modeName;
            return;
        }

        // cleanupcurrentmode
        if (m_currentMode != nullptr) {
            m_currentMode->disconnectSignals();
            m_currentMode->cleanup();
            delete m_currentMode;
        }

        // create新mode
        auto* newMode = createMode(modeName);
        // check
        if (newMode == nullptr) {
            qDebug() << "Mode not found:" << modeName << "for editor type" << editorName();
            return;
        }

        // create新mode
        m_currentMode = newMode;
        m_currentMode->connectSignals();
        updateContent();
    }

    void QHFEditorBase::handleNotify(const QHFNotifier& notifier) {
        switch (notifier.type) {
        case QHFNotifierType::NEW_APP_WINDOW_INFO_WORKSPACE:
            handleNewAppWindowInfoWorkspace(notifier);
            break;
        case QHFNotifierType::APP_WINDOW_ACTIVATED_WORKSPACE:
            handleAppWindowActivatedWorkspace(notifier);
            break;
        case QHFNotifierType::APP_WINDOW_MOVED_WORKSPACE:
            handleAppWindowMovedWorkspace(notifier);
            break;
        case QHFNotifierType::REPORT_ACTIVATION:
            handleReportActivation(notifier);
            break;
        default:
            break;
        }
        // Subclass may override this function以处理notificationevent
        handleNotifyImpl(notifier);
    }

    void QHFEditorBase::handleNewAppWindowInfoWorkspace(const QHFNotifier& notifier) {
        auto workspaceOpt = notifier.get<QWidget*>("workspace");
        if (workspaceOpt) {
            auto* workspace = *workspaceOpt; // 新建 窗口为Editor 祖父
            if (workspace != nullptr && m_parent != nullptr && workspace == m_parent) {
                QHFNotifier noti(QHFNotifierType::NEW_APP_WINDOW_INFO_EDITOR);
                noti.set("editor", this);
                QHFNotifierCenter::instance().publish(noti);
            }
        }
    }

    void QHFEditorBase::handleAppWindowActivatedWorkspace(const QHFNotifier& notifier) {
        auto workspaceOpt = notifier.get<QWidget*>("workspace");
        if (workspaceOpt) {
            auto* workspace = *workspaceOpt;
            if (workspace != nullptr && m_parent != nullptr && workspace == m_parent) {
                QHFNotifier noti(QHFNotifierType::APP_WINDOW_ACTIVATED_EDITOR);
                noti.set("editor", this);
                publishEvent(this, noti, 450);
            }
        }
    }

    void QHFEditorBase::handleAppWindowMovedWorkspace(const QHFNotifier& notifier) {
        auto workspaceOpt = notifier.get<QWidget*>("workspace");
        if (workspaceOpt) {
            auto* workspace = *workspaceOpt;
            if (workspace != nullptr && m_parent != nullptr && workspace == m_parent) {
                QHFNotifier noti(QHFNotifierType::APP_WINDOW_MOVED_EDITOR);
                noti.set("editor", this);
                QHFNotifierCenter::instance().publish(noti);
            }
        }
    }

    void QHFEditorBase::handleReportActivation(const QHFNotifier& notifier) {
        auto eventOpt = notifier.get<QHFActivationEvent>("event");
        if (eventOpt) {
            const auto& event = *eventOpt;
            if (event.type == QHFActivationEvent::Type::MODE_ACTIVE) {
                QString modeName = event.context.value("modeName").toString();
                auto*   source   = qobject_cast<QHFModeBase*>(event.source);
                if (source != nullptr && source == m_currentMode &&
                    modeName == m_currentMode->modeName()) {
                    publishEditorActive("mode reported", event.immediate);
                }
            }
        }
    }

    void QHFEditorBase::publishEditorActive(const QString& reason, bool immediate) {
        QHFActivationEvent event(QHFActivationEvent::Type::EDITOR_ACTIVE, this, reason, immediate);
        event.context["editor"] = QVariant::fromValue(this);
        QHFEditorBase* editor     = this;
        publishActivation(editor, event);
    }

} // namespace QHF
