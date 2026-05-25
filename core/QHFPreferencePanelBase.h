#ifndef QHFPREFERENCEPANELBASE_H
#define QHFPREFERENCEPANELBASE_H

#include <QPaintEvent>
#include <QVBoxLayout>
#include <QWidget>

#include "QHFThemeManager.h"
#include "QHFUIIPreferencePanel.h"
#include "QHFUIUtils.h"

namespace QHF {

    /**
     * @brief Preferences (see description)
     * @details (see source) IPreferencePanel
     * interface, 提供基本 panel功能, 如panelname、翻译name、register set元data、load/saveset等功能.
     */
    class QHFPreferencePanelBase : public QWidget,
                                 public QHFThemedCRTP<QHFPreferencePanelBase>,
                                 public IPreferencePanel {
        Q_OBJECT

      protected:
        QVBoxLayout* m_layout;

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
        explicit QHFPreferencePanelBase(QWidget* parent = nullptr);
        ~QHFPreferencePanelBase() override = default; ///< Virtual Destructor// --- 接口实现 ---
        /**
         * @brief Initializes (see description)
         * @details (see source).
         */
        void initialize() override;

      protected:
        /**
         * @brief  (see description)
         * @param event eventobject*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source).
         */
        virtual void applyTheme();

      private:
        /**
         * @brief  (see description)
         * @details (see source).
         * @param modeNames modenamelist*/
        virtual void initializeImpl() = 0;
    };

} // namespace QHF
#endif // HPREFERENCEPANELBASE_H
