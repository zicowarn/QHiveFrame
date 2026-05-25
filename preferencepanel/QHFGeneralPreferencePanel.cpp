#include "QHFGeneralPreferencePanel.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>

#include "QHFPreferencePanelFactory.h"

namespace QHF {

    /// --- QHFGeneralPreferencePanel implementation ---
    QHF_REGISTER_PANEL("General", QHFGeneralPreferencePanel, QHFGeneralPreferencePanel::SUGGESTED_ORDER)
    QList<SettingMeta> QHFGeneralPreferencePanel::registeredSettings() {
        return {
            {"general.language", "zh_CN", "2.0.0", true}, // Language需要重启
            {"general.theme", "Dark", "2.0.0", false}     // Theme无需重启
        };
    }

    QHFGeneralPreferencePanel::QHFGeneralPreferencePanel(QWidget* parent)
        : QHFPreferencePanelBase(parent) {}

    QString QHFGeneralPreferencePanel::panelName() const
    {
        return "General";
    }

    QString QHFGeneralPreferencePanel::transName() const
    {
        return tr("General");
    }

    void QHFGeneralPreferencePanel::loadSettings(const QList<Setting>& settings) {
        for (const auto& setting : settings) {
            if (setting.key == "general.language") {
                m_languageComboBox->blockSignals(true);
                qDebug() << "load language: " << setting.value.toString();
                QString setLanguage = setting.value.toString();
                // iterate languageComboBox
                for (int i = 0; i < m_languageComboBox->count(); i++) {
                    if (m_languageComboBox->itemData(i) == setLanguage) {
                        m_languageComboBox->setCurrentIndex(i);
                        break;
                    }
                }
                m_languageComboBox->blockSignals(false);
            } else if (setting.key == "general.theme") {
                m_themeComboBox->blockSignals(true);
                qDebug() << "load theme: " << setting.value.toString();
                QString setTheme = setting.value.toString();
                // iterate themeComboBox
                for (int i = 0; i < m_themeComboBox->count(); i++) {
                    if (m_themeComboBox->itemData(i) == setTheme) {
                        m_themeComboBox->setCurrentIndex(i);
                        break;
                    }
                }
                m_themeComboBox->blockSignals(false);
            }
        }
    }

    QList<Setting> QHFGeneralPreferencePanel::saveSettings() const {
        return {{"general.language", m_languageComboBox->currentData(), "zh_CN", true},
                {"general.theme", m_themeComboBox->currentData(), "Dark", false}};
    }

    void QHFGeneralPreferencePanel::applyTheme() {
        //
        const auto& theme = QHFThemeManager::instance().theme();
        // subclassstyle
        QString comboBoxStyle = QString("QComboBox#HPrefGeneralComboBox {"
                                        "   background-color: %1;"
                                        "}")
                                    .arg(theme.comboBoxBgColor.name());

        setStyleSheet(comboBoxStyle);
        // 调用base classstyle
        QHFPreferencePanelBase::applyTheme();
    }

    void QHFGeneralPreferencePanel::initializeImpl() {
        //
        auto* langugaelayout = new QHBoxLayout();
        langugaelayout->setContentsMargins(0, 0, 0, 0);
        langugaelayout->setSpacing(0);
        m_layout->addLayout(langugaelayout);
        // Label 语言
        auto* languageLabel = new QLabel(tr("Language"), this);
        languageLabel->setObjectName("HPrefGeneralLabel");
        langugaelayout->addWidget(languageLabel);
        // ComboBox 语言
        m_languageComboBox = new QComboBox(this);
        m_languageComboBox->setObjectName("HPrefGeneralComboBox");
        m_languageComboBox->setEditable(false);
        m_languageComboBox->addItem(tr("English"), "en_US");
        m_languageComboBox->addItem(tr("Chinese"), "zh_CN");
        langugaelayout->addWidget(m_languageComboBox);

        //
        auto* themelayout = new QHBoxLayout();
        themelayout->setContentsMargins(0, 0, 0, 0);
        themelayout->setSpacing(0);
        m_layout->addLayout(themelayout);
        // Label theme
        auto* themeLabel = new QLabel(tr("Theme"), this);
        themeLabel->setObjectName("HPrefGeneralLabel");
        themelayout->addWidget(themeLabel);
        // ComboBox theme
        m_themeComboBox = new QComboBox(this);
        m_themeComboBox->setObjectName("HPrefGeneralComboBox");
        m_themeComboBox->setEditable(false);
        m_themeComboBox->addItem(tr("Dark"), "Dark");
        themelayout->addWidget(m_themeComboBox);

        m_layout->addStretch(1);
    }

} // namespace QHF
