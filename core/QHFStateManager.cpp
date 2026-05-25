#include "QHFStateManager.h"

#include "QHFGuiNotifier.h"
#include "QHFGuiNotifierCenter.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardPaths>
#include <QWidget>

namespace QHF {

    /// --- QHFStateManager implementation ---
    QHFStateManager& QHFStateManager::instance() {
        static QHFStateManager instance;
        return instance;
    }

    void QHFStateManager::saveState() {
        // create一个json 对象
        QJsonObject saveStateData;
        // save fileopenlist, name recentFiles 从 QStringList 而来data
        QJsonArray recentFilesArray;
        for (const QString& file : m_fileState.recentFiles) {
            recentFilesArray.append(file);
        }
        saveStateData["recentFiles"] = recentFilesArray;
        // saveFileDialog   FavoritePlaces
        QJsonArray favoritePlacesArray;
        for (const QString& place : m_favoritePlaces) {
            favoritePlacesArray.append(place);
        }
        saveStateData["favoritePlaces"] = favoritePlacesArray;

        // saveWindow state
        saveAppState(defaultAppStatePath(), saveStateData);
    }
    void QHFStateManager::loadState() {
        // 清除现有 state
        cleanup();
        // loadFile state
        auto appState = loadAppState(defaultAppStatePath());
        // loadfileopenlist
        if (appState.contains("recentFiles")) {
            auto recentFilesArray = appState["recentFiles"].toArray();
            for (const QJsonValue& file : recentFilesArray) {
                m_fileState.recentFiles.append(file.toString());
            }
        }
        // loadFileDialog   FavoritePlaces
        if (appState.contains("favoritePlaces")) {
            auto favoritePlacesArray = appState["favoritePlaces"].toArray();
            for (const QJsonValue& place : favoritePlacesArray) {
                m_favoritePlaces.append(place.toString());
            }
        }
    }

    void QHFStateManager::cleanup() {
        m_stateData.clear();
        m_windowStates.clear();
        m_fileState.path = "";
        m_fileState.name = "Untitled";
        m_fileState.isDirty = false;
        m_fileState.recentFiles.clear();
        m_fileState.isLoaded = false;
        m_currentLanguage = "zh_CN";
        m_favoritePlaces.clear();
    }

    bool QHFStateManager::isBlockingWindowActive() const
    {
        return m_isBlockingWindowActive;
    }

    void QHFStateManager::setBlockingWindowActive(bool isBlocking) {
        m_isBlockingWindowActive = isBlocking;
    }

    QWidget* QHFStateManager::activeWindow() const
    {
        return m_activeWindow;
    }

    void QHFStateManager::setActiveWindow(QWidget* window) { m_activeWindow = window; }

    int QHFStateManager::countWindows() const
    {
        return m_windowStates.size();
    }

    bool QHFStateManager::existWindow(QWidget* window) const {
        return m_windowStates.contains(window);
    }

    void QHFStateManager::addWindow(QWidget* window) {
        // check是否已exist
        if (m_windowStates.contains(window) || window == nullptr) {
            return;
        }
        WindowState state;
        if (m_windowStates.empty()) {
            // 第一个window, 从 m_stateData 中loadstate
            if (m_stateData.contains("AppMainWindow")) {
                // 从 m_stateData 中loadstate
            }
        } else {
            // 非第一个window, 使用从m_activeWindow中loadstate
            auto activeWinState = m_windowStates.value(m_activeWindow);
            // check state (复制)
            state = activeWinState;
        }
        // Save state
        m_windowStates.insert(window, state);
    }

    void QHFStateManager::removeWindow(QWidget* window) {
        // 保留最after一个window
        if (m_windowStates.size() <= 1) {
            if (m_activeWindow == window) {
                m_activeWindow = nullptr;
            }
            return;
        }
        m_windowStates.remove(window);
    }

    WindowState QHFStateManager::getWindowState(QWidget* window) const {
        if (!m_windowStates.contains(window) || window == nullptr) {
            return {};
        }
        // Get state态
        return m_windowStates[window];
    }

    void QHFStateManager::setWindowState(QWidget* window, const WindowState& state) {
        if (!m_windowStates.contains(window) || window == nullptr) {
            return;
        }
        // Save state
        m_windowStates[window] = state;
    }

    int QHFStateManager::getMainWinWorkspaceIndex(QWidget* window) const {
        if (!m_windowStates.contains(window) || window == nullptr) {
            return -1;
        }
        // Get state态
        return m_windowStates[window].currentWorkspaceIndex;
    }

    void QHFStateManager::setMainWinWorkspaceIndex(QWidget* window, int index) {
        if (!m_windowStates.contains(window) || window == nullptr) {
            return;
        }
        // Save state
        m_windowStates[window].currentWorkspaceIndex = index;
    }

    bool QHFStateManager::getStatusBarVisible(QWidget* window) const {
        if (!m_windowStates.contains(window) || window == nullptr) {
            return false;
        }
        // Get state态
        return m_windowStates[window].isShowStatusBar;
    }

    void QHFStateManager::setStatusBarVisible(QWidget* window, bool visible) {
        if (!m_windowStates.contains(window) || window == nullptr) {
            return;
        }
        // Save state
        m_windowStates[window].isShowStatusBar = visible;
    }

    void QHFStateManager::resetFileState() {
        m_fileState.name = "Untitled";
        m_fileState.isDirty = false;
        // m_fileState.recentFiles.clear();
        m_fileState.isLoaded = false;
    }

    void QHFStateManager::updateRecentFiles() {
        // 将currentfile加入到list 最before面
        auto currentPath = m_fileState.path;
        auto currentFile = m_fileState.name;
        if (currentFile.isEmpty()) {
            return;
        }
        // check是否已exist, 并移除
        if (m_fileState.recentFiles.contains(currentPath)) {
            m_fileState.recentFiles.removeAll(currentPath);
        }
        // 加入到list 最before面
        m_fileState.recentFiles.prepend(currentPath);
    }

    const FileState& QHFStateManager::getFileState() {
        if (m_fileState.isLoaded) {
            return m_fileState;
        }
        // 从file中loadstate
        m_fileState.name = "Untitled";
        // set为未修改
        m_fileState.isDirty = false;
        // loadset
        if (m_stateData.contains("AppFileState")) {
            // 从file中loadstate
        }
        // loadsuccess
        m_fileState.isLoaded = true;
        return m_fileState;
    }

    void QHFStateManager::setFileState(const FileState& state) { m_fileState = state; }

    void QHFStateManager::setCurrentLanguage(const QString& lang) { m_currentLanguage = lang; }

    QString QHFStateManager::getCurrentLanguage() const { return m_currentLanguage; };

    QStringList QHFStateManager::getFavoritePlaces() const
    {
        return m_favoritePlaces;
    }

    void QHFStateManager::setFavoritePlaces(const QStringList& places) { m_favoritePlaces = places; }

    void QHFStateManager::onFocusChanged(QWidget* old, QWidget* now) {
        qDebug() << "Focus changed: old=" << old << "now=" << now
                 << "activeWindow=" << QApplication::activeWindow();
        if (m_isBlockingWindowActive) {
            qDebug() << "Skipping focus change due to window switching";
            return;
        }

        QWidget* targetWindow = findTargetWindow(now);
        if (targetWindow != m_activeWindow) {
            m_activeWindow = targetWindow;
            QHFNotifier noti(QHFNotifierType::APP_WINDOW_ACTIVATED);
            noti.set("window", targetWindow);
            publishEvent(targetWindow, noti, 150);
            qDebug() << "window focused:" << targetWindow;
        }
    }

    QHFStateManager::QHFStateManager() : QObject(nullptr) {
        // Initializesstate
        cleanup();
        // connectionsignal
        connect(qApp, &QApplication::focusChanged, this, &QHFStateManager::onFocusChanged);
        qDebug() << "QHFStateManager initialized with focusChanged connection";
    }

    QWidget* QHFStateManager::findTargetWindow(QWidget* widget) const {
        if (widget == nullptr) {
            return nullptr;
        }
        // iterate m_windowStates
        for (auto it = m_windowStates.begin(); it != m_windowStates.end(); ++it) {
            if (it.key() == widget) {
                return it.key();
            }
        }
        return nullptr;
    }

    QString QHFStateManager::defaultAppStatePath() const {
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
        return dir.filePath("app_state.json");
    }

    QJsonObject QHFStateManager::loadAppState(const QString& path) const {
        qDebug() << "Loading app state from" << path;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open app state file:" << path;
            return {};
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isNull()) {
            qWarning() << "Invalid app state file:" << path;
            return {};
        }
        return doc.object();
    }

    void QHFStateManager::saveAppState(const QString& path, const QJsonObject& data) const {
        qDebug() << "Saving app state to" << path;
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Cannot open app state file for writing:" << path;
            return;
        }
        QJsonDocument doc(data);
        file.write(doc.toJson());
    }

} // namespace QHF
