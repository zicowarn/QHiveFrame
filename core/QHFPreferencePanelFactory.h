#ifndef QHFPREFERENCEPANELFACTORY_H
#define QHFPREFERENCEPANELFACTORY_H

#include <QMap>
#include <QString>
#include <functional>
#include <QHash>
#include <type_traits>

#include "QHFUIIPreferencePanel.h"
#include "QHFUIUtils.h"

namespace QHF {

    using PanelName = StrongType<struct PanelNameTag, QString>; ///< panelnametype
/**
 * @brief Preferences (see description)
     */
    struct PreferencePanelInfo {
        QString name;  ///< e.g."General"
        int     order; ///< e.g.1000
    };

    namespace { // non-exported namespace
        /**
         * @brief  (see description)
         * @tparam TPanel paneltype
         * @details (see source) IPreferencePanel::initialize
         */
        template <typename TPanel> struct CorePanelInitializer {
            void operator()(TPanel* panel) const { panel->initialize(); }
        };

        /**
         * @brief Panel (see description)
         * @tparam TPanel paneltype
         */
        template <typename TPanel> class PanelFactory {
            static_assert(std::is_base_of_v<IPreferencePanel, TPanel>,
                          "TPanel must derive from IPreferencePanel");

          public:
            /**
             * @brief  (see description)
             * @param parent Parent object
             * @return panelobject*/
            static QWidget* create(QWidget* parent) {
                auto panel = new TPanel(parent);
                CorePanelInitializer<TPanel>{}(panel); // 自动调用 initialize
                return panel;
            }
        };

    } // namespace

    /**
     * @brief Preferences (see description)
     * @details (see source)
     */
    class QHFPreferencePanelFactory {

      public:
        static constexpr int DEFAULT_ORDER_BASE = 1000;             ///< Default顺序基数
        static constexpr int ORDER_STEP         = 100;              ///< 顺序步长
        using PanelCreatorFunc= std::function<QWidget*(QWidget*)>; ///< panelCreate functiontype
        using SettingsFunc=
            std::function<QList<SettingMeta>()>; ///< Set function类型;           ///< Set function类型
        using PanelInfo = PreferencePanelInfo; ///< panel信息结构体type

      private:
        QMap<QString, int>              m_registeredOrders; ///< Name到order映射
        QMap<QString, PanelCreatorFunc> m_creators;         ///< panelCreate function映射
        QMap<QString, SettingsFunc>     m_settingsFuncs;    ///< Set function映射
        QList<PanelInfo>                m_panels;           ///< panel信息list

      public:/**
         * @brief  (see description)
         * @return Register 类实例(reference)
         */
        static QHFPreferencePanelFactory& instance();

        /**
         * @brief  (see description)
         * @tparam TPanel paneltype
         * @param name panelname* @param order panelorder(default-1, Represents auto分配)
         * @details (see source)
         */
        template <typename TPanel>
        static void registerPanel(const PanelName& name, int order = -1) {
            auto& factory = instance(); // 在 lambda 外调用 instance()
            factory.registerPanelCore(
                name, order,
                [](QWidget* parent = nullptr) -> QWidget* {
                    return PanelFactory<TPanel>::create(parent);
                },
                &TPanel::registeredSettings);
        }

        /* @brief  (see description)
         * @param name panelname* @param order panelorder(default-1, Represents auto分配)
         * @param creator Create function*/
        void registerPanelCore(const PanelName& name, int order, PanelCreatorFunc creator,
                               SettingsFunc settingsFunc);

        /**
         * @brief  (see description)
         * @param name panelname* @param parent Parent object (QObject*)
         * @return panelobject*/
        [[nodiscard]] QWidget* createPanel(const QString& name, QWidget* parent) const;

        /**
         * @brief  (see description)
         * @return panelinfolist*/
        [[nodiscard]] QList<PreferencePanelInfo> panels() const;

        /**
         * @brief  (see description)
         * @return List of registered setting metadata
         */
        [[nodiscard]] QList<SettingMeta> getRegisteredSettings() const;

        /**
         * @brief  (see description)
         * @param panelName panelname* @return panelorder*/
        [[nodiscard]] int getOrder(const QString& panelName) const;

      private:
        /**
         * @brief  (see description)
         * @param panelName panelname* @param suggestedOrder suggest order* @return 分配 order*/
        int allocateOrder(const QString& panelName, int suggestedOrder);

        /**
         * @brief  (see description)
         * @param info panel项*/
        void insertPanelInOrder(const PreferencePanelInfo& info);
    };

#define QHF_REGISTER_PANEL(NameStr, PanelClass, Order)                                                 \
    static bool _registered_##PanelClass = []() {                                                  \
        QHFPreferencePanelFactory::registerPanel<PanelClass>(PanelName{NameStr}, Order);             \
        return true;                                                                               \
    }();

} // namespace QHF

#endif // HPREFERENCEPANELFACTORY_H
