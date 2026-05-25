// QHFModeFactory.h
#ifndef QHFMODEFACTORY_H
#define QHFMODEFACTORY_H

#include "QHFUIIEditorContext.h"
#include "QHFUIIModeContext.h"
#include "QHFUIUtils.h"

#include <QDebug>
#include <QMap>
#include <QStringList>
#include <QVariant>
#include <functional>
#include <QHash>
#include <type_traits>

namespace QHF {

    using ModeKey = std::size_t; ///< mode键type

    using EditorName= StrongType<struct EditorNameTag, QString>; ///< Edit器名称类型
    using ModeName   = StrongType<struct ModeNameTag, QString>;   ///< modenametype
/**
 * @brief (see description)
     * @param editorName editorname* @param modeName modename*/
    inline ModeKey makeModeKey(const QString& editorName, const QString& modeName) {
        return qHash(QString("%1::%2").arg(editorName, modeName));
    }

    namespace { // non-exported namespace

        /**
         * @brief  (see description)
         * @tparam TMode mode type
         * @details (see source) QHFModeBase::initialize
         */
        template <typename TMode> struct CoreModeInitializer {
            void operator()(TMode* mode) const { mode->initialize(); }
        };

        /**
         * @brief Mode (see description)
         * @tparam TMode mode type
         */
        template <typename TMode> class ModeFactory {
            static_assert(std::is_base_of_v<IModeContext, TMode>,
                          "TMode must derive from QHFModeBase");

          public:
            static IModeContext* create(IEditorContext* context, QWidget* parent) {
                auto mode = new TMode(context, parent);
                CoreModeInitializer<TMode>{}(mode); // 自动调用 initialize
                return mode;
            }
        };

    } // namespace

    class QHFModeRegistry {
      public:
        using CreatorFunc =
            std::function<IModeContext*(IEditorContext*, QWidget*)>; // 更新 CreatorFunc
      private:
        QMap<ModeKey, CreatorFunc> m_creators;      ///< Create function映射
        QMap<QString, QStringList> m_editorToModes; ///< Edit器到模式列表

      public:
        /**
         * @brief  (see description)
         * @return modeRegister 实例*/
        static QHFModeRegistry& instance();

        /**
         * @brief  (see description)
         * @tparam ModeClass mode类
         * @param editorName editortype* @param modeName modename*/
        template <typename ModeClass>
        static void registerMode(const EditorName& editorName, QHFModeType modeType) {
            instance().registerModeCore(
                editorName, modeType,
                [](IEditorContext* context, QWidget* parent) -> IModeContext* {
                    return ModeFactory<ModeClass>::create(context, parent);
                });
        }

        /**
         * @brief  (see description)
         * @param editorName editortype* @param modeType modetype* @param creator Create function*/
        void registerModeCore(const EditorName& editorName, QHFModeType modeType,
                              CreatorFunc creator);

        /**
         * @brief  (see description)
         * @param editorName editortype* @param modeName modename* @param context editor实例* @param parent Parent widget
         * @return mode实例*/
        [[nodiscard]] IModeContext* createMode(const EditorName& editorName,
                                               const ModeName& modeName, IEditorContext* context,
                                               QWidget* parent = nullptr) const;

        /**
         * @brief  (see description)
         * @param editorName editortype* @return modelist*/
        [[nodiscard]] QStringList modeNames(const QString& editorName) const;

      private:
        QHFModeRegistry() = default; ///< Constructor
    };

// 公共register宏
#define QHF_REGISTER_MODE(EditorTypeStr, ModeClass, ModeType)                                     \
    static bool _registered_##ModeClass = []() {                                                   \
        static_assert(std::is_same_v<decltype(EditorTypeStr), QString>,                            \
                      "EditorTypeStr must be QString");                                            \
        static_assert(std::is_same_v<decltype(ModeType), QHFModeType>,                               \
                      "ModeType must be QHFModeType");                                               \
        QHFModeRegistry::registerMode<ModeClass>(EditorName{EditorTypeStr}, ModeType);               \
        return true;                                                                               \
    }();

} // namespace QHF

#endif // HMODEFACTORY_H
