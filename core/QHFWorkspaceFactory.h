#ifndef QHFWORKSPACEFACTORY_H
#define QHFWORKSPACEFACTORY_H

#include "QHFWorkspaceBase.h"

namespace QHF {

    /**
     * @brief Workspace (see description)
     * @details (see source)
     */
    struct WorkspaceInfo {
        QString        name;
        QHFWorkspaceType type;
        int            order;
    };

    namespace { // non-exported namespace

        template <typename TWorkspace> struct CoreWorkspaceInitializer {
            void operator()(TWorkspace* workspace) const { workspace->initialize(); }
        };

        template <typename TWorkspace> class WorkspaceFactory {
            static_assert(std::is_base_of_v<QHFWorkspaceBase, TWorkspace>,
                          "TWorkspace must derive from QHFWorkspaceBase");

          public:
            /**
             * @brief  (see description)
             * @param parent Parent object
             * @return workspaceobject*/
            static QHFWorkspaceBase* create(QWidget* parent, bool isInitialize) {
                auto workspace = new TWorkspace(parent, isInitialize);
                CoreWorkspaceInitializer<TWorkspace>{}(workspace); // 自动调用 initialize
                return workspace;
            }
        };

    } // namespace

    class QHFWorkspaceRegistry {

      public:
        static constexpr int DEFAULT_ORDER_BASE = 1000;                     ///< Default顺序基数
        static constexpr int ORDER_STEP         = 100;                      ///< 顺序步长
        using CreatorFunc= std::function<QHFWorkspaceBase*(QWidget*, bool)>; ///< Create function类型

      private:
        QMap<QString, int>         m_registeredOrders; ///< Name到order映射
        QMap<QString, CreatorFunc> m_creators;         ///< Create function映射
        QList<WorkspaceInfo>       m_workspaces;       ///< workspace信息list

      public:/**
         * @brief  (see description)
         * @return Register 类实例(reference)
         */
        static QHFWorkspaceRegistry& instance();

        /**
         * @brief  (see description)
         * @tparam WorkspaceClass workspace类
         * @param type workspacetype* @param order workspaceorder(default-1, Represents auto分配)
         * @details (see source) WorkspaceFactory::create
         * createworkspace对象
         */
        template <typename WorkspaceClass>
        static void registerWorkspace(const QHFWorkspaceType& type, int order = -1) {
            auto& registry = instance(); // 在 lambda 外调用 instance()
            registry.registerWorkspaceCore(
                type, order,
                [](QWidget* parent = nullptr, bool isInitialize = true) -> QHFWorkspaceBase* {
                    return WorkspaceFactory<WorkspaceClass>::create(parent, isInitialize);
                });
        }

        /**
         * @brief  (see description)
         * @param type workspacetype* @param order workspaceorder(default-1, Represents auto分配)
         * @param creator Create function*/
        void registerWorkspaceCore(const QHFWorkspaceType& type, int order, CreatorFunc creator);

        /**
         * @brief  (see description)
         * @param name workspacename* @param parent Parent object (QObject*)
         * @param isInitialize is 否需要InitializesLayout(defaulttrue)
         * @return workspaceobject*/
        [[nodiscard]] QHFWorkspaceBase* createWorkspace(const QHFWorkspaceType& type, QWidget* parent,
                                                      bool isInitialize = true) const;

        /**
         * @brief  (see description)
         * @return workspaceinfolist*/
        [[nodiscard]] QList<WorkspaceInfo> workspaces() const;

        /**
         * @brief  (see description)
         * @param workspaceName workspacename* @return workspaceorder*/
        [[nodiscard]] int getOrder(const QString& workspaceName) const;

      private:
        QHFWorkspaceRegistry() = default; ///< 禁止实例化
/**
 * @brief (see description)
         * @param workspaceName workspacename* @param suggestedOrder suggest order* @return 分配 order*/
        int allocateOrder(const QString& workspaceName, int suggestedOrder);

        /**
         * @brief  (see description)
         * @param info workspaceinfo*/
        void insertWorkspaceInOrder(const WorkspaceInfo& info);
    };

#define QHF_REGISTER_WORKSPACE(WorkSType, WorkspaceClass, Order)                                       \
    static bool _registered_##WorkspaceClass = []() {                                              \
        QHFWorkspaceRegistry::registerWorkspace<WorkspaceClass>(WorkSType, Order);                   \
        return true;                                                                               \
    }();
} // namespace QHF

#endif // HWORKSPACEFACTORY_H
