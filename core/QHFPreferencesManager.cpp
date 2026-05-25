#include "QHFPreferencesManager.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QStringList>
#include <functional>
#include <vector> // 添加 vector 支持

#include "QHFPreferencePanelFactory.h"

namespace QHF {

    /// --- SemanticVersion implementation ---
    SemanticVersion::SemanticVersion(const QString& version) {
        QStringList parts = version.split('-').first().split('.');
        if (parts.isEmpty()) {
            m_major = 0;
            m_minor = 0;
            m_patch = 0;
        }
        if (!parts.isEmpty()) {
            m_major = parts[0].toInt();
        }
        if (parts.size() >= 2) {
            m_minor = parts[1].toInt();
        }
        if (parts.size() >= 3) {
            m_patch = parts[2].toInt();
        }
    }

    bool SemanticVersion::operator>(const SemanticVersion& other) const {
        if (m_major != other.m_major) {
            return m_major > other.m_major;
        }
        if (m_minor != other.m_minor) {
            return m_minor > other.m_minor;
        }
        return m_patch > other.m_patch;
    }

    bool SemanticVersion::operator<(const SemanticVersion& other) const {
        if (m_major != other.m_major) {
            return m_major < other.m_major;
        }
        if (m_minor != other.m_minor) {
            return m_minor < other.m_minor;
        }
        return m_patch < other.m_patch;
    }

    bool SemanticVersion::operator<=(const SemanticVersion& other) const {
        if (m_major != other.m_major) {
            return m_major < other.m_major;
        }
        if (m_minor != other.m_minor) {
            return m_minor < other.m_minor;
        }
        return m_patch <= other.m_patch;
    }

    bool SemanticVersion::operator==(const SemanticVersion& other) const {
        return m_major == other.m_major && m_minor == other.m_minor && m_patch == other.m_patch;
    }

    QString SemanticVersion::toString() const {
        return QString("%1.%2.%3").arg(m_major).arg(m_minor).arg(m_patch);
    }

    /// --- QHFPreferencesManager implementation ---
    const QString QHFPreferencesManager::kCurrentVersion = "2.0.0";

    QHFPreferencesManager& QHFPreferencesManager::instance() {
        static QHFPreferencesManager instance;
        return instance;
    }

    QList<Setting> QHFPreferencesManager::loadDefaultSettings() const {
        QList<Setting> settings;
        for (const auto& meta : m_registeredSettings) {
            QStringList parts = meta.key.split('.');
            if (parts.size() == 2) {
                settings.append(
                    Setting{meta.key, meta.defaultValue, meta.defaultValue, meta.requiresRestart});
            }
        }
        return settings;
    }

    QList<Setting> QHFPreferencesManager::loadSettings() const {
        QList<Setting> settings;
        QJsonObject    jsonSettings = loadPreferences(m_registeredSettings);
        for (const auto& meta : m_registeredSettings) {
            QStringList parts = meta.key.split('.');
            if (parts.size() == 2) {
                QJsonValue value = jsonSettings.value(parts[0]).toObject().value(parts[1]);
                settings.append(Setting{meta.key,
                                        value.isUndefined() ? meta.defaultValue : value.toVariant(),
                                        meta.defaultValue, meta.requiresRestart});
            }
        }
        return settings;
    }

    void QHFPreferencesManager::saveSettings(const QList<Setting>& settings) {
        QJsonObject jsonSettings = m_currentSettings;
        for (const auto& setting : settings) {
            QStringList parts = setting.key.split('.');
            if (parts.size() == 2) {
                QJsonObject category   = jsonSettings.value(parts[0]).toObject();
                category[parts[1]]     = QJsonValue::fromVariant(setting.value);
                jsonSettings[parts[0]] = category;
            }
        }
        jsonSettings["version"] = kCurrentVersion;
        saveUserPreferences(jsonSettings);
    }

    QVariant QHFPreferencesManager::getSetting(const QString&  key,
                                             const QVariant& defaultValue) const {
        QJsonObject settings = m_currentSettings;
        QStringList parts    = key.split('.');
        QJsonValue  value    = settings;
        for (const auto& part : parts) {
            value = value.toObject().value(part);
            if (value.isUndefined()) {
                return defaultValue;
            }
        }
        return value.toVariant();
    }

    QJsonObject
    QHFPreferencesManager::loadPreferences(const QList<SettingMeta>& registeredSettings) const {
        QJsonObject defaults  = loadJson(defaultPrefsPath());
        QJsonObject userPrefs = loadJson(userPrefsPath());
        return mergePreferences(defaults, userPrefs, registeredSettings);
    }

    void QHFPreferencesManager::saveUserPreferences(const QJsonObject& prefs) {
        const QString path = userPrefsPath();
        if (path.isEmpty()) {
            return;
        }

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qWarning() << "Failed to open" << path << ":" << file.errorString();
            return;
        }

        QJsonObject saveObj = prefs;
        saveObj["version"]  = kCurrentVersion;
        file.write(QJsonDocument(saveObj).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Saved user preferences to" << path;
        m_currentSettings = saveObj;
    }

    void QHFPreferencesManager::saveDefaultPreferences(const QJsonObject& defaults) {
        const QString path = defaultPrefsPath();
        if (path.isEmpty()) {
            return;
        }

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qWarning() << "Failed to open" << path << ":" << file.errorString();
            return;
        }

        QJsonObject saveObj = defaults;
        saveObj["version"]  = kCurrentVersion;
        file.write(QJsonDocument(saveObj).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Saved default preferences to" << path;
    }

    QHFPreferencesManager::QHFPreferencesManager() {
        m_registeredSettings = QHFPreferencePanelFactory::instance().getRegisteredSettings();
        m_currentSettings    = loadPreferences(m_registeredSettings);
    }

    QString QHFPreferencesManager::userPrefsPath() {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        if (configPath.isEmpty()) {
            qWarning() << "AppConfigLocation is empty!";
            return {};
        }

        QDir dir(configPath);
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qWarning() << "Cannot create config directory:" << configPath;
                return {};
            }
        }
        return dir.filePath("user_preferences.json");
    }

    QString QHFPreferencesManager::defaultPrefsPath() {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        if (configPath.isEmpty()) {
            qWarning() << "AppConfigLocation is empty!";
            return {};
        }

        QDir dir(configPath);
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qWarning() << "Cannot create config directory:" << configPath;
                return {};
            }
        }
        return dir.filePath("default_preferences.json");
    }

    QJsonObject QHFPreferencesManager::loadJson(const QString& path) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            return QJsonDocument::fromJson(file.readAll()).object();
        }
        if (path == ":/resources/default_preferences.json") {
            QFile fallback(defaultPrefsPath());
            if (fallback.open(QIODevice::ReadOnly)) {
                qDebug() << "Loaded default preferences from" << defaultPrefsPath();
                return QJsonDocument::fromJson(fallback.readAll()).object();
            }
        }
        return {};
    }

    /// NOLINTNEXTLINE(readability-function-cognitive-complexity)
    QJsonObject QHFPreferencesManager::mergePreferences(const QJsonObject&        defaults,
                                                      const QJsonObject&        user,
                                                      const QList<SettingMeta>& registered) const {
        QJsonObject result;
        bool        defaultsGenerated = false;

        // 1. Initializes结果: 优先defaultset
        if (!defaults.isEmpty()) {
            result = defaults;
            SemanticVersion defaultVersion(defaults.value("version").toString("0.0.0"));
            if (defaultVersion < SemanticVersion(kCurrentVersion)) {
                migrateSettings(result, defaultVersion);
            }
        } else {
            qWarning() << "Default preferences missing, generating from SettingMeta";
            for (const auto& meta : registered) {
                QStringList parts = meta.key.split('.');
                if (parts.size() == 2) {
                    const auto& category = parts[0];
                    const auto& setting  = parts[1];
                    if (!result.contains(category)) {
                        result[category] = QJsonObject();
                    }
                    QJsonObject categoryObj = result[category].toObject();
                    categoryObj[setting]    = QJsonValue::fromVariant(meta.defaultValue);
                    result[category]        = categoryObj;
                }
            }
            defaultsGenerated = true;
        }

        // 2. 补齐新版本 键
        SemanticVersion userVersion(user.value("version").toString("0.0.0"));
        SemanticVersion currentVersion(kCurrentVersion);
        for (const auto& meta : registered) {
            QStringList parts = meta.key.split('.');
            if (parts.size() == 2) {
                const auto&     category = parts[0];
                const auto&     setting  = parts[1];
                SemanticVersion metaVersion(meta.version);
                if (userVersion < metaVersion || user.isEmpty()) {
                    if (!result.contains(category)) {
                        result[category] = QJsonObject();
                    }
                    QJsonObject categoryObj = result[category].toObject();
                    if (!categoryObj.contains(setting)) {
                        categoryObj[setting] = QJsonValue::fromVariant(meta.defaultValue);
                        result[category]     = categoryObj;
                    }
                }
            }
        }

        // 3. 合并用户set, 处理版本回退
        if (!user.isEmpty()) {
            if (userVersion > currentVersion) {
                qWarning() << "User settings version" << userVersion.toString()
                           << "is higher than system version" << kCurrentVersion
                           << ", ignoring unknown keys";
            }
            for (auto it = user.constBegin(); it != user.constEnd(); ++it) {
                QString category = it.key();
                if (std::any_of(registered.begin(), registered.end(),
                                [&category](const SettingMeta& meta) {
                                    return meta.key.split('.').first() == category;
                                })) {
                    QJsonObject userCategory   = it.value().toObject();
                    QJsonObject resultCategory = result.value(category).toObject();
                    for (auto userIt = userCategory.constBegin(); userIt != userCategory.constEnd();
                         ++userIt) {
                        QString setting = userIt.key();
                        QString key     = QString("%1.%2").arg(category, setting);
                        auto    metaIt  = std::find_if(
                            registered.begin(), registered.end(),
                            [&key](const SettingMeta& meta) { return meta.key == key; });
                        if (metaIt != registered.end()) {
                            if (metaIt->defaultValue.type() == userIt.value().toVariant().type()) {
                                resultCategory[setting] = userIt.value();
                            } else {
                                qWarning() << "Invalid type for" << key << ", using default";
                            }
                        }
                    }
                    result[category] = resultCategory;
                }
            }
        }

        // 4. 版本迁移和Versionset
        if (!user.isEmpty() && userVersion <= currentVersion) {
            migrateSettings(result, userVersion);
        } else {
            result["version"] = kCurrentVersion;
        }

        // 5. savedefaultconfiguration (若生成)
        if (defaultsGenerated) {
            saveDefaultPreferences(result);
        }
        return result;
    }

    void QHFPreferencesManager::migrateSettings(QJsonObject&           settings,
                                              const SemanticVersion& version) const {
        // 使用 std::vector 替代 QList, 支持 std::unique_ptr  移动语义
        // 使用 emplace_back 构造 std::unique_ptr, 避免拷贝
        static const std::vector<std::unique_ptr<MigrationRule>> migrations = [this]() {
            std::vector<std::unique_ptr<MigrationRule>> migrations;
            migrations.emplace_back(std::make_unique<RenameKeyRule>(
                SemanticVersion("1.0.1"), "general.old_language", "general.language"));
            migrations.emplace_back(std::make_unique<ConvertValueRule>(
                SemanticVersion("1.0.1"), "general.theme",
                [](const QJsonValue& value) { return value.toString().toLower(); }));
            migrations.emplace_back(std::make_unique<RemoveKeyRule>(SemanticVersion("1.1.0"),
                                                                    "general.deprecated_key"));
            migrations.emplace_back(
                std::make_unique<CustomRule>(SemanticVersion(kCurrentVersion),
                                             [this](QJsonObject& sett) { migrateFromV200(sett); }));
            return migrations;
        }();

        bool            migrationApplied = false;
        SemanticVersion currentVersion(kCurrentVersion);
        for (const auto& rule : migrations) {
            if (version < rule->targetVersion && rule->targetVersion <= currentVersion) {
                rule->apply(settings); // 修复: 传递 settings Parameter
                migrationApplied = true;
                qDebug() << "Applied migration to" << rule->targetVersion.toString();
            }
        }

        if (migrationApplied || version < currentVersion) {
            settings["version"] = kCurrentVersion;
        }
    }

    void QHFPreferencesManager::migrateFromV200(QJsonObject& settings) {
        // 修复逻辑: 重命名 general.old_language 为 general.language
        if (settings.contains("general") &&
            settings["general"].toObject().contains("old_language")) {
            QJsonObject general = settings["general"].toObject();
            general["language"] = general["old_language"]; // 修复: 正确访问 old_language
            general.remove("old_language");
            settings["general"] = general;
        }
    }

    QList<SettingMeta> QHFPreferencesManager::collectRegisteredSettings() {
        return QHFPreferencePanelFactory::instance().getRegisteredSettings();
    }

} // namespace QHF
