#ifndef QHFGENERALPREFERENCEPANEL_H
#define QHFGENERALPREFERENCEPANEL_H

#include <QComboBox>

#include "QHFPreferencePanelBase.h"

namespace QHF {

    class QHFGeneralPreferencePanel : public QHFPreferencePanelBase {
        Q_OBJECT

      public:
        static constexpr int SUGGESTED_ORDER = 1000;

      private:
        QComboBox* m_languageComboBox;
        QComboBox* m_themeComboBox;

      public:
        /**
         * @brief Registered setting metadata
         * @return List of registered setting metadata
         * @note Subclasses must override this static function
         */
        static QList<SettingMeta> registeredSettings();

        /**
         * @brief Constructor
         * @param parent Parent object
         */
        explicit QHFGeneralPreferencePanel(QWidget* parent = nullptr);
        ~QHFGeneralPreferencePanel() override = default; ///< Virtual Destructor
/**
 * @brief Panel name
         * @return Panel name
         */
        [[nodiscard]] QString panelName() const override;

        /**
         * @brief Translated name
         * @return Translated name
         */
        [[nodiscard]] QString transName() const override;

        /**
         * @brief  (see description)
         * @param settings 要Load  Set */
        void loadSettings(const QList<Setting>& settings) override;

        /**
         * @brief  (see description)
         * @return Save  Set */
        [[nodiscard]] QList<Setting> saveSettings() const override;

      private:
        /**
         * @brief  (see description)
         * @details (see source).
         */
        void applyTheme() override;

        /**
         * @brief  (see description)
         * @details (see source).
         */
        void initializeImpl() override;
    };

} // namespace QHF

#endif // HGENERALPREFERENCEPANEL_H
