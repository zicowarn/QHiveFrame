#include "QHFFileDialog.h"

#include "QHFStateManager.h"
#include "QHFThemeManager.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QHeaderView>
#include <QList>
#include <QMessageBox> // 可以在这里添加一些调试或错误提示
#include <QToolButton>
#include <QTreeView>
#include <QUrl>

namespace QHF {

    namespace {
        QList<QUrl> loadSiderUrlList() {
            QList<QUrl> urls;
            auto favoritePlaces = QHFStateManager::instance().getFavoritePlaces();
            if (favoritePlaces.isEmpty()) {
                return urls;
            }
            for (const auto& place : favoritePlaces) {
                qDebug() << "Loading sidebar URL: " << place;
                urls << QUrl::fromLocalFile(place);
            }
            return urls;
        }

        void saveSiderUrlList(const QList<QUrl>& urls) {
            if (urls.isEmpty()) {
                return;
            }
            QStringList favoritePlaces;
            for (const auto& url : urls) {
                qDebug() << "Saving sidebar URL: " << url.toLocalFile();
                favoritePlaces << url.toLocalFile();
            }
            QHFStateManager::instance().setFavoritePlaces(favoritePlaces);
            QHFStateManager::instance().saveState();
        }
    } // namespace

    QHFFileDialog::QHFFileDialog(QWidget* parent) : QFileDialog(parent) {
        // Set object name
        setObjectName("QHFFileDialog");
        // setwindowstyle
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
    }

    // --- staticinterfaceimplementation ---
    QString QHFFileDialog::getOpenFileName(QWidget* parent, const QString& caption,
                                         const QString& dir, const QString& filter,
                                         QString* selectedFilter, Options options) {
        QHFFileDialog dialog(parent);
        dialog.setFileMode(QFileDialog::ExistingFile); // 只能选择一个文件
        dialog.setupQHFFileDialog(caption, dir, filter, options);
        auto urls = loadSiderUrlList();
        if (!urls.isEmpty()) {
            dialog.setSidebarUrls(urls);
        }

        if (dialog.exec() == QDialog::Accepted) {
            auto selectedUrls = dialog.sidebarUrls();
            if (!selectedUrls.isEmpty()) {
                saveSiderUrlList(selectedUrls);
            }
            if (selectedFilter != nullptr) {
                *selectedFilter = dialog.selectedNameFilter();
            }
            return dialog.selectedFiles().first();
        }
        return {}; // 用户取消, Returns 空字符串
    }

    QStringList QHFFileDialog::getOpenFileNames(QWidget* parent, const QString& caption,
                                              const QString& dir, const QString& filter,
                                              QString* selectedFilter, Options options) {
        QHFFileDialog dialog(parent);
        dialog.setFileMode(QFileDialog::ExistingFiles); // 可以选择多个文件
        dialog.setupQHFFileDialog(caption, dir, filter, options);
        auto urls = loadSiderUrlList();
        if (!urls.isEmpty()) {
            dialog.setSidebarUrls(urls);
        }

        if (dialog.exec() == QDialog::Accepted) {
            auto selectedUrls = dialog.sidebarUrls();
            if (!selectedUrls.isEmpty()) {
                saveSiderUrlList(selectedUrls);
            }
            if (selectedFilter != nullptr) {
                *selectedFilter = dialog.selectedNameFilter();
            }
            return dialog.selectedFiles();
        }
        return {}; // 用户取消, Returns 空列表
    }

    QString QHFFileDialog::getSaveFileName(QWidget* parent, const QString& caption,
                                         const QString& dir, const QString& filter,
                                         QString* selectedFilter, Options options) {
        QHFFileDialog dialog(parent);
        dialog.setFileMode(QFileDialog::AnyFile); // 可以是任何文件, 包括不存在  (Saves )
        // 对于save对话框, 通常会default开启 DontConfirmOverwrite
        dialog.setupQHFFileDialog(caption, dir, filter, options | QFileDialog::DontConfirmOverwrite);
        auto urls = loadSiderUrlList();
        if (!urls.isEmpty()) {
            dialog.setSidebarUrls(urls);
        }

        if (dialog.exec() == QDialog::Accepted) {
            auto selectedUrls = dialog.sidebarUrls();
            if (!selectedUrls.isEmpty()) {
                saveSiderUrlList(selectedUrls);
            }
            if (selectedFilter != nullptr) {
                *selectedFilter = dialog.selectedNameFilter();
            }
            // 对于save, 通常只选择一个file
            QString filePath = dialog.selectedFiles().first();
            // 如果用户没有输入file扩展名, according to选择 过滤器auto添加
            if (!filePath.contains('.')) {
                QString selectedFilterString = dialog.selectedNameFilter();
                // Tip: 取括号内 扩展名, 例如 "Blender场景file (*.blend)" -> ".blend"
                int start = selectedFilterString.indexOf("(*.");
                int end = selectedFilterString.indexOf(")", start);
                if (start != -1 && end != -1) {
                    QString ext = selectedFilterString.mid(start + 2, end - (start + 2));
                    if (!ext.isEmpty()) {
                        // 如果有多个扩展名, 只取第一个
                        if (ext.contains(" ")) {
                            ext = ext.split(" ").first();
                        }
                        filePath += "." + ext;
                    }
                }
            }
            return filePath;
        }
        return {}; // 用户取消, Returns 空字符串
    }

    QString QHFFileDialog::getExistingDirectory(QWidget* parent, const QString& caption,
                                              const QString& dir, Options options) {
        QHFFileDialog dialog(parent);
        dialog.setFileMode(QFileDialog::Directory); // 只能选择目录
        // 强制显示directory而不是file, 并ensuredirectory只读
        dialog.setupQHFFileDialog(caption, dir, QString(),
                                options | QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly);

        auto urls = loadSiderUrlList();
        if (!urls.isEmpty()) {
            dialog.setSidebarUrls(urls);
        }

        if (dialog.exec() == QDialog::Accepted) {
            auto selectedUrls = dialog.sidebarUrls();
            if (!selectedUrls.isEmpty()) {
                saveSiderUrlList(selectedUrls);
            }
            return dialog.selectedFiles().first();
        }
        return {}; // 用户取消, Returns 空字符串
    }

    void QHFFileDialog::applyTheme() {
        // Applies theme
        const auto& theme = QHFThemeManager::instance().theme();
        // Returns current style
        auto currentStyle = styleSheet();

        // 主要style
        QString basisStyle = QString(R"(
                #QHFFileDialog {
                    background-color: %1;
                    color: %2;
                }
            )")
                                 .arg(theme.innerPanelColor.name(), theme.textColor.name());

        QString listViewStyle = QString(R"(
                #QHFFileDialog QListView {
                    background-color: %1;
                    color: %2;
                }
        )")
                                    .arg(theme.contentPanelColor.name(), theme.textColor.name());

        QString treeViewStyle = QString(R"(
                #QHFFileDialog QTreeView {
                    background-color: %1;
                    color: %2;
                }
        )")
                                    .arg(theme.contentPanelColor.name(), theme.textColor.name());

        QString treeViewHeaderStyle =
            QString(R"(
                #QHFFileDialog QHeaderView {
                    background-color: %1;
                    color: %2;
                }

                #QHFFileDialog QHeaderView::section {
                    background-color: %1;
                    color: %2;
                }
        )")
                .arg(theme.innerPanelColor.name(), theme.textColor.name());

        QString comoboStyle = QString(R"(
                #QHFFileDialog QComboBox {
                    background-color: %1;
                    color: %2;
                }
        )")
                                  .arg(theme.comboBoxBgColor.name(), theme.textColor.name());

        QString toolBtnStyle = QString(R"(
                #QHFFileDialog QToolButton, #QHFFileDialog QPushButton {
                    background-color: %1;
                    color: %2;
                }
        )")
                                   .arg(theme.btnBgColor.name(), theme.btnFgColor.name());

        // 扩展style
        currentStyle.append(basisStyle + listViewStyle + treeViewStyle + treeViewHeaderStyle +
                            comoboStyle + toolBtnStyle);

        // setstyle
        setStyleSheet(currentStyle);
    }

    void QHFFileDialog::initialize() {
        // Applies theme
        applyTheme();
    }

    void QHFFileDialog::setupQHFFileDialog(const QString& caption, const QString& dir,
                                       const QString& filter, Options options) {
        setOption(QFileDialog::DontUseNativeDialog, true); // 强制使用Qt风格

        // Set window title
        if (!caption.isEmpty()) {
            setWindowTitle(caption);
        } else {
            // Tip: 供一个defaulttitle, 如果外部没有指定
            setWindowTitle("文件选择器 - QHF");
        }

        // set初始directory
        if (!dir.isEmpty() && QDir(dir).exists()) {
            setDirectory(dir);
        } else {
            setDirectory(QDir::currentPath()); // 默认当前工作目录
        }

        // setfile过滤器
        if (!filter.isEmpty()) {
            setNameFilter(filter);
            // 可以尝试智能选择default过滤器, 例如如果过滤器contain".blend"
            if (filter.contains("*.blend", Qt::CaseInsensitive)) {
                selectNameFilter("Blender场景文件 (*.blend)");
            } else {
                // 否则选择第一个过滤器
                QString defaultFilter = filter.split(";;").value(0);
                if (!defaultFilter.isEmpty()) {
                    selectNameFilter(defaultFilter);
                }
            }
        } else {
            // 如果没有指定过滤器, 提供一个default 
            setNameFilter("所有文件 (*.*)");
        }

        // set侧边栏 (类似Blender 收藏夹)
        QList<QUrl> sidebarUrls;
        sidebarUrls << QUrl::fromLocalFile(QDir::homePath()); // 用户主目录
        sidebarUrls << QUrl::fromLocalFile(QDir::rootPath()); // 系统根目录
        // 您可以在这里添加项目特有 常用directory
        // sidebarUrls << QUrl::fromLocalFile("/path/to/your/project/assets");
        setSidebarUrls(sidebarUrls);

        // default视图mode为详细info
        setViewMode(QFileDialog::Detail);

        // 应用传递进来 QFileDialog::Options
        setOptions(options);

        // 对于save对话框, 如果file名不exist, default会提示覆盖, Blender可能不希望如此
        // QFileDialog::DontConfirmOverwrite 是一个常见option
    }

} // namespace QHF
