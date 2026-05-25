#ifndef QHFFILEDIALOG_H
#define QHFFILEDIALOG_H

#include "QHFUIUtils.h"

#include <QFileDialog>
#include <QString>
#include <QStringList> // 用于Returns 多个文件路径

namespace QHF {

    // 别名在类外部, 命名empty间内部
    using QHFFDOption = QFileDialog::Option;
    using QHFFDOptions = QFileDialog::Options; // This is a typedef for QFlags<QFileDialog::Option>edef

    /**
     * @brief  (see description)
     * @details (see source)
     */
    class QHFFileDialog : public QFileDialog, public QHFThemedCRTP<QHFFileDialog> {
        Q_OBJECT

      public:
        /**
         * @brief Constructor
         * @param parent 父级*/
        explicit QHFFileDialog(QWidget* parent = nullptr);
        ~QHFFileDialog() override = default; ///< Virtual Destructor// --- 静态接口 (模仿 QFileDialog  静态方法) ---
        /**
         * @brief  (see description)
         * @param parent 父级* @param caption title* @param dir 初始directory* @param filter file过滤器* @param selectedFilter 选中 file过滤器* @param options option*/
        static QString getOpenFileName(QWidget* parent = nullptr,
                                       const QString& caption = QString(),
                                       const QString& dir = QString(),
                                       const QString& filter = QString(),
                                       QString* selectedFilter = nullptr, Options options = {});

        /**
         * @brief  (see description)
         * @param parent 父级* @param caption title* @param dir 初始directory* @param filter file过滤器* @param selectedFilter 选中 file过滤器* @param options option*/
        static QStringList
        getOpenFileNames(QWidget* parent = nullptr, const QString& caption = QString(),
                         const QString& dir = QString(), const QString& filter = QString(),
                         QString* selectedFilter = nullptr, Options options = {});

        /**
         * @brief  (see description)
         * @param parent 父级* @param caption title* @param dir 初始directory* @param filter file过滤器* @param selectedFilter 选中 file过滤器* @param options option*/
        static QString getSaveFileName(QWidget* parent = nullptr,
                                       const QString& caption = QString(),
                                       const QString& dir = QString(),
                                       const QString& filter = QString(),
                                       QString* selectedFilter = nullptr, Options options = {});

        /**
         * @brief  (see description)
         * @param parent 父级* @param caption title* @param dir 初始directory* @param options option*/
        static QString getExistingDirectory(QWidget* parent = nullptr,
                                            const QString& caption = QString(),
                                            const QString& dir = QString(),
                                            Options options = ShowDirsOnly);

      private:
        /**
         * @brief Applies theme
         */
        void applyTheme();

        /**
         * @brief Initializes
         * @details (see source)
         */
        void initialize();

        /**
         * @brief  (see description)
         * @param caption title* @param dir 初始directory* @param filter file过滤器* @param options option*/
        void setupQHFFileDialog(const QString& caption, const QString& dir, const QString& filter,
                              Options options);
    };

} // namespace QHF

#endif // HFILEDIALOG_H
