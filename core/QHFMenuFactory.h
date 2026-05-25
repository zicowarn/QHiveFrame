#ifndef QHFMENUFACTORY_H
#define QHFMENUFACTORY_H

#include <QMap>
#include <QString>
#include <functional>
#include <qHash>
#include <type_traits>

#include "QHFUIIMenuProvider.h"
#include "QHFUIUtils.h"

namespace QHF {

    using MenuName = StrongType<struct MenuNameTag, QString>; ///< menu name type
/**
 * @brief Menu (see description)
     * @details (see source)
     */
    struct MenuInfo {
        QString name;  ///< e.g."File"
        int     order; ///< e.g.1000
    };

    namespace { // non-exported namespace
        /**
         * @brief  (see description)
         * @tparam TMenu mode type
         * @details (see source) IMenuProvider::initialize
         */
        template <typename TMenu> struct CoreMenuInitializer {
            void operator()(TMenu* menu) const { menu->initialize(); }
        };

        /**
         * @brief Mode (see description)
         * @tparam TMenu mode type
         */
        template <typename TMenu> class MenuFactory {
            static_assert(std::is_base_of_v<IMenuProvider, TMenu>,
                          "TMenu must derive from IMenuProvider");

          public:
            /**
             * @brief  (see description)
             * @param parent Parent object
             * @return menuobject*/
            static IMenuProvider* create(QObject* parent) {
                auto menu = new TMenu(parent);
                CoreMenuInitializer<TMenu>{}(menu); // 自动调用 initialize
                return menu;
            }
        };

    } // namespace

    /**
     * @brief Menu (see description)
     * @details (see source)
     */
    class QHFMenuRegistry {

      public:
        static constexpr int DEFAULT_ORDER_BASE = 1000;              ///< Default顺序基数
        static constexpr int ORDER_STEP         = 100;               ///< 顺序步长
        static constexpr int SUB_ORDER_STEP= 10;                ///< 子menu顺序步长
        using CreatorFunc= std::function<IMenuProvider*(QObject*)>; ///< Create function类型

      private:
        QMap<QString, int>         m_registeredOrders; ///< Name到order映射
        QMap<QString, CreatorFunc> m_creators;         ///< Create function映射
        QList<MenuInfo>            m_menus;            ///< menu信息list

      public:/**
         * @brief  (see description)
         * @return Register 类实例(reference)
         */
        static QHFMenuRegistry& instance();

        /**
         * @brief Registers menu
         * @tparam MenuClass menu类
         * @param name menuname* @param trKey menu翻译键* @param order menuorder(default-1, Represents auto分配)
         * @details (see source)
         */
        template <typename MenuClass>
        static void registerMenu(const MenuName& name, int order = -1) {
            auto& registry = instance(); // 在 lambda 外调用 instance()
            registry.registerMenuCore(name, order, [](QObject* parent = nullptr) -> IMenuProvider* {
                return MenuFactory<MenuClass>::create(parent);
            });
        }

        /**
         * @brief Registers menu
         * @param name menuname* @param order menuorder(default-1, Represents auto分配)
         * @param creator Create function*/
        void registerMenuCore(const MenuName& name, int order, CreatorFunc creator);

        /**
         * @brief  (see description)
         * @param name menuname* @param parent Parent object (QObject*)
         * @return menuobject*/
        [[nodiscard]] IMenuProvider* createMenu(const QString& name, QObject* parent) const;

        /**
         * @brief  (see description)
         * @return menuinfolist*/
        [[nodiscard]] QList<MenuInfo> menus() const;

        /**
         * @brief  (see description)
         * @param menuName menuname* @return menuorder*/
        [[nodiscard]] int getOrder(const QString& menuName) const;

        /**
         * @brief  (see description)
         * @param menuName menu name
         * @param index sub-menu index
         * @return next sub-menu order
         */
        [[nodiscard]] int getSubOrder(const QString& menuName, int index) const; // 新增

      private:
        QHFMenuRegistry() = default; ///< prevent instantiation
/**
 * @brief (see description)
         * @param menuName menu name
         * @param suggestedOrder suggested order
         * @return allocated order
         */
        int allocateOrder(const QString& menuName, int suggestedOrder);

        /**
         * @brief  (see description)
         * @param info menu item
         */
        void insertMenuInOrder(const MenuInfo& info);
    };

#define QHF_REGISTER_MENU(NameStr, MenuClass, Order)                                                   \
    static bool _registered_##MenuClass = []() {                                                   \
        QHFMenuRegistry::registerMenu<MenuClass>(MenuName{NameStr}, Order);                          \
        return true;                                                                               \
    }();

} // namespace QHF

#endif // HMENUFACTORY_H
