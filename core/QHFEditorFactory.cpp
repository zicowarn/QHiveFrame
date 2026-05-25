#include "QHFEditorFactory.h"

namespace QHF {
    /// --- QHFEditorFactory implementation ---
    QHFEditorRegistry& QHFEditorRegistry::instance() {
        static QHFEditorRegistry inst;
        return inst;
    }

    void QHFEditorRegistry::registerEditor(EditorType type, CreatorFunc creator) {
        QString typeName     = EditorTypeUtils::toString(type);
        m_creators[typeName] = std::move(creator);
        m_editorNames.append(typeName);
    }

    QWidget* QHFEditorRegistry::create(EditorType type, QWidget* parent) const {
        QString typeName = EditorTypeUtils::toString(type);
        auto    iter     = m_creators.find(typeName);
        return (iter != m_creators.end()) ? iter.value()(parent) : nullptr;
    }

    QStringList QHFEditorRegistry::editorNames() const
    {
        return m_editorNames;
    }

} // namespace QHF
