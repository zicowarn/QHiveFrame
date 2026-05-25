#ifndef QHFEDITORFACTORY_H
#define QHFEDITORFACTORY_H

#include <QMap>
#include <QWidget>
#include <functional>
#include <memory>
#include <type_traits>

#include "QHFUIIEditorContext.h"

namespace QHF {

    namespace { // non-exported namespace

        /**
         * @brief  (see description)
         * @tparam TEditor editortype
         * @details (see source)CoreEditorInitializer
         */
        template <typename TEditor> struct CoreEditorInitializer {
            void operator()(TEditor* editor) const { editor->initialize(); }
        };

        /**
         * @brief  (see description)
         * @tparam TEditor editortype
         * @details (see source)DefaultEditorInitializer
         */
        template <typename TEditor> struct DefaultEditorInitializer {
            void operator()(TEditor* editor) const {}
        };

        /**
         * @brief Editor (see description)
         * @tparam TEditor editortype
         * @tparam Initializer editorInitializes策略
         */
        template <typename TEditor, typename Initializer = DefaultEditorInitializer<TEditor>>
        class EditorFactory {
            static_assert(std::is_base_of_v<IEditorContext, TEditor>,
                          "TEditor must derive from IEditorContext");

          public:
            static QWidget* Create(QWidget* parent) {
                auto editor = new TEditor(parent);
                CoreEditorInitializer<TEditor>{}(
                    static_cast<TEditor*>(editor)); // 默认调用initialize
                Initializer{}(static_cast<TEditor*>(
                    editor)); // 额外 Initializes, 默认使用DefaultEditorInitializer
                return editor;
            }
        };
    } // namespace

    /**
     * @brief Editor (see description)
     * @details (see source)
     */
    class QHFEditorRegistry {

      public:
        using CreatorFunc = std::function<QWidget*(QWidget*)>; ///< Create function类型

      private:
        QStringList                m_editorNames; ///< Edit器名称列表
        QMap<QString, CreatorFunc> m_creators;    ///< Register 表

      public:/**
         * @brief  (see description)
         * @return editorRegister 器实例*/
        static QHFEditorRegistry& instance();

        /**
         * @brief  (see description)
         * @param type editortype* @param creator editorCreate function*/
        void registerEditor(EditorType type, CreatorFunc creator);

        /**
         * @brief  (see description)
         * @param type editortype* @param parent Parent object
         * @return new editor*/
        QWidget* create(EditorType type, QWidget* parent) const;

        /**
         * @brief  (see description)
         * @return editornamelist*/
        [[nodiscard]] QStringList editorNames() const;
    };

// Note: 册宏
#define QHF_REGISTER_EDITOR(EditorClass, EditorEnum, InitializerType)                                  \
    static bool _registered_##EditorClass = []() {                                                 \
        QHFEditorRegistry::instance().registerEditor(EditorEnum, [](QWidget* parent) -> QWidget* {   \
            return EditorFactory<EditorClass, InitializerType>::Create(parent);                    \
        });                                                                                        \
        return true;                                                                               \
    }();

// Note: 册宏 (default无Initializes器)
#define QHF_REGISTER_EDITOR_DEFAULT(EditorClass, EditorEnum)                                           \
    QHF_REGISTER_EDITOR(EditorClass, EditorEnum, DefaultEditorInitializer<EditorClass>)

} // namespace QHF

#endif // HEDITORFACTORY_H
