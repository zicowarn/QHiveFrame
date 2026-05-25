#ifndef IPREFERENCEPANEL_H
#define IPREFERENCEPANEL_H
#include <QJsonObject>
#include <QList>
#include <QString>

namespace QHF {

    /**
     * @brief Preferences (see description)
     * @details (see source).
     */
    struct SettingMeta {
        QString  key;             ///< e.g. "general.language"
        QVariant defaultValue;    ///< e.g. "en_US"
        QString  version;         ///< e.g. "2.0.0"
        bool     requiresRestart; ///< whether restart is required
    };
    /**
     * @brief Setting meta data
     * @details (see source).
     */
    struct Setting {
        QString  key;             ///< e.g. "general.language"
        QVariant value;           ///< e.g. "en_US"
        QVariant defaultValue;    ///< e.g. "en_US", 回退或Initializes
        bool     requiresRestart; ///< whether restart is required
    };
    /**
     * @brief Preferences panel interface
     * @details (see source).
     */
    class IPreferencePanel {

      public:
        virtual ~IPreferencePanel() = default; ///< Virtual Destructor
/**
 * @brief Panel name ( (see description)
         * @return Panel name
         */
        [[nodiscard]] virtual QString panelName() const = 0;

        /**
         * @brief Translated name ( (see description)
         * @return Translated name
         */
        [[nodiscard]] virtual QString transName() const = 0;

        /**
         * @brief  (see description)
         * @param settings 要Load  Set */
        virtual void loadSettings(const QList<Setting>& settings) = 0;

        /**
         * @brief  (see description)
         * @return Save  Set */
        [[nodiscard]] virtual QList<Setting> saveSettings() const = 0;

        /**
         * Initializespanel
         */
        virtual void initialize() = 0;
    };

} // namespace QHF
#endif
