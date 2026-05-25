#ifndef QHFPREFERENCESMANAGER_H
#define QHFPREFERENCESMANAGER_H

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QStringList>
#include <functional>

#include "QHFUIIPreferencePanel.h"

namespace QHF {

    /**
     * @brief  (see description)
     * @details (see source)Version
     */
    class SemanticVersion {
      private:
        int m_major = 0;
        int m_minor = 0;
        int m_patch = 0;

      public:
        /**
         * @brief Constructor.
         * @details Constructor.
         */
        SemanticVersion(const QString& version = "0.0.0");
        ~SemanticVersion() = default; ///< Destructor。
/**
 * @brief (see description)
         * @details (see source)Version.
         * @param other otherVersion* @return 如果current Version大于otherVersion, Returns  true；否则Returns  false
         */
        bool operator>(const SemanticVersion& other) const;

        /**
         * @brief  (see description)
         * @details (see source)Version.
         * @param other otherVersion* @return 如果current Version小于otherVersion, Returns  true；否则Returns  false
         */
        bool operator<(const SemanticVersion& other) const;

        /**
         * @brief  (see description)
         * @details (see source)Version.
         * @param other otherVersion* @return 如果current Version大于otherVersion, Returns  true；否则Returns  false
         */
        bool operator<=(const SemanticVersion& other) const;

        /**
         * @brief  (see description)
         * @details (see source)Version.
         * @param other otherVersion* @return 如果current Version等于otherVersion, Returns  true；否则Returns  false
         */
        bool operator==(const SemanticVersion& other) const;

        /**
         * @brief Converts to string.
         * @details Converts to string.
         * @return string*/
        [[nodiscard]] QString toString() const;
    };

    /**
     * @biref preferenceset 迁移规则
     * @details (see source)
     */
    struct MigrationRule {
        SemanticVersion targetVersion; ///< 目标Version

        virtual~MigrationRule() = default; ///< Destructor.
/**
 * @brief Apply migration (see description)
         * @details (see source).
         * @param settings preferenceSet   JSON object* @return Apply migration规则after preferenceset  JSON 对象
         * @note 该函数为虚函数, subclass必须implementation
         */
        virtual void apply(QJsonObject& settings) const = 0;
    };

    /**
     * @brief  (see description)
     * @details (see source)
     */
    struct RenameKeyRule : MigrationRule {
        QString oldKey; ///< 旧键名
        QString newKey;///< 新键名
/**
 * @brief Constructor.
         * @details Constructor.
         * @param version 目标Version* @param oldK 旧键名* @param newK 新键名*/
        /// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        RenameKeyRule(const SemanticVersion& version, QString oldK, QString newK)
            : oldKey(std::move(oldK)), newKey(std::move(newK)) {
            targetVersion = version;
        }

        /**
         * @brief Apply migration (see description)
         * @details (see source).
         * @param settings preferenceSet   JSON object*/
        void apply(QJsonObject& settings) const override {
            QStringList oldParts = oldKey.split('.');
            QStringList newParts = newKey.split('.');
            if (oldParts.size() == 2 && newParts.size() == 2) {
                const auto& oldCategory = oldParts[0];
                const auto& oldSetting  = oldParts[1];
                const auto& newCategory = newParts[0];
                const auto& newSetting  = newParts[1];
                if (settings.contains(oldCategory) &&
                    settings[oldCategory].toObject().contains(oldSetting)) {
                    QJsonObject category = settings[oldCategory].toObject();
                    QJsonValue  value    = category[oldSetting];
                    category.remove(oldSetting);
                    settings[oldCategory] = category;
                    if (!settings.contains(newCategory)) {
                        settings[newCategory] = QJsonObject();
                    }
                    QJsonObject newCat    = settings[newCategory].toObject();
                    newCat[newSetting]    = value;
                    settings[newCategory] = newCat;
                }
            }
        }
    };

    /**
     * @brief  (see description)
     * @details (see source)
     */
    struct ConvertValueRule : MigrationRule {
        QString                                      key;       ///< 键名
        std::function<QJsonValue(const QJsonValue&)> converter; ///< 转换function
/**
 * @brief Constructor.
         * @details Constructor.
         * @param version 目标Version* @param k 键名* @param c 转换function*/
        ConvertValueRule(const SemanticVersion& version, QString skey,
                         std::function<QJsonValue(const QJsonValue&)> cfunc)
            : key(std::move(skey)), converter(std::move(cfunc)) {
            targetVersion = version;
        }

        /**
         * @brief Apply migration (see description)
         * @details (see source).
         * @param settings preferenceSet   JSON object*/
        void apply(QJsonObject& settings) const override {
            QStringList parts = key.split('.');
            if (parts.size() == 2 && settings.contains(parts[0]) &&
                settings[parts[0]].toObject().contains(parts[1])) {
                QJsonObject category = settings[parts[0]].toObject();
                category[parts[1]]   = converter(category[parts[1]]);
                settings[parts[0]]   = category;
            }
        }
    };

    /**
     * @brief  (see description)
     * @details (see source)
     */
    struct RemoveKeyRule : MigrationRule {
        QString key; ///< 键名
/**
 * @brief Constructor.
         * @details Constructor.
         * @param version 目标Version* @param k 键名* @note 该Constructor为私有, 只能via MigrationRule pointercreate对象
         */
        RemoveKeyRule(const SemanticVersion& version, QString skey) : key(std::move(skey)) {
            targetVersion = version;
        }

        /**
         * @brief Apply migration (see description)
         * @details (see source).
         * @param settings preferenceSet   JSON object*/
        void apply(QJsonObject& settings) const override {
            QStringList parts = key.split('.');
            if (parts.size() == 2 && settings.contains(parts[0]) &&
                settings[parts[0]].toObject().contains(parts[1])) {
                QJsonObject category = settings[parts[0]].toObject();
                category.remove(parts[1]);
                settings[parts[0]] = category;
            }
        }
    };

    /**
     * @brief Custom (see description)
     * @details (see source)
     */
    struct CustomRule : MigrationRule {
        std::function<void(QJsonObject&)> migration; ///< 迁移function
/**
 * @brief Constructor.
         * @details Constructor.
         * @param version 目标Version* @param m 迁移function*/
        CustomRule(const SemanticVersion& version, std::function<void(QJsonObject&)> mfunc)
            : migration(std::move(mfunc)) {
            targetVersion = version;
        }

        /**
         * @brief Apply migration (see description)
         * @details (see source).
         * @param settings preferenceSet   JSON object*/
        void apply(QJsonObject& settings) const override { migration(settings); }
    };

    /**
     * @brief Preferences (see description)
     * @details (see source).
     */
    class QHFPreferencesManager {

      public:
        static const QString kCurrentVersion; // 当前版本

      private:
        QJsonObject        m_currentSettings;    ///< 缓存current Set 
        QList<SettingMeta> m_registeredSettings; ///< 已Register  Set list

      public:/**
         * @brief Returns preferences manager instance.
         * @details Returns preferences manager instance.
         * @return preference管理器实例*/
        static QHFPreferencesManager& instance();

        /**
         * @brief  (see description)
         * @details (see source).
         * @return for交互 default Set list*/
        [[nodiscard]] QList<Setting> loadDefaultSettings() const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @return for交互 Set list*/
        [[nodiscard]] QList<Setting> loadSettings() const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param settings for交互 Set list*/
        void saveSettings(const QList<Setting>& settings);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param key 层级键名, 如 general.language
         * @param defaultValue default value* @return Set 项 value*/
        [[nodiscard]] QVariant getSetting(const QString&  key,
                                          const QVariant& defaultValue = {}) const;

      private:
        /**
         * @brief  (see description)
         * @details (see source).
         * @param registeredSettings 已Register  Set list* @return preferenceSet   JSON object*/
        [[nodiscard]] QJsonObject
        loadPreferences(const QList<SettingMeta>& registeredSettings) const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param prefs preferenceSet   JSON object*/
        void saveUserPreferences(const QJsonObject& prefs);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param defaults default preferenceSet   JSON object*/
        static void saveDefaultPreferences(const QJsonObject& defaults);

        /**
         * @brief Constructor.
         * @details Constructor.
         * @note 该Constructor为私有, 只能via instance() 获得实例。
         */
        QHFPreferencesManager();

        /**
         * @brief  (see description)
         * @details (see source).
         * @return 用户preferencefile path*/
        [[nodiscard]] static QString userPrefsPath();

        /**
         * @brief  (see description)
         * @details (see source).
         * @return default preferencefile path*/
        [[nodiscard]] static QString defaultPrefsPath();

        /**
         * @brief  (see description)
         * @details (see source).
         * @param path filepath* @return JSON 对象
         */
        static QJsonObject loadJson(const QString& path);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param defaults default Set * @param user 用户Set * @param registered 已Register  Set list* @return 合并after preferenceSet   JSON object*/
        [[nodiscard]] QJsonObject mergePreferences(const QJsonObject&        defaults,
                                                   const QJsonObject&        user,
                                                   const QList<SettingMeta>& registered) const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param settings preferenceSet   JSON object* @param userVersion 用户Version*/
        void migrateSettings(QJsonObject& settings, const SemanticVersion& userVersion) const;

        /**
         * @brief  (see description)
         * @details (see source)).
         */
        static void migrateFromV200(QJsonObject& settings);

        /**
         * @brief  (see description)
         * @details (see source).
         * @return 已Register  Set list*/
        static QList<SettingMeta> collectRegisteredSettings();
    };

} // namespace QHF
#endif
