#include "QHFModeFactory.h"

#include <QDebug>

namespace QHF {
    QHFModeRegistry& QHFModeRegistry::instance() {
        static QHFModeRegistry instance;
        return instance;
    }

    void QHFModeRegistry::registerModeCore(const EditorName& editorName, QHFModeType modeType,
                                         CreatorFunc creator) {
        QString modeName = QHFModeTypeUtils::toString(modeType);
        qDebug() << "Registering mode:" << modeName << "for editor:" << editorName;
        m_creators[makeModeKey(editorName, modeName)] = std::move(creator);
        m_editorToModes[editorName].append(modeName);   // Adds to 映射
        m_editorToModes[editorName].sort();             // 保持排序
        m_editorToModes[editorName].removeDuplicates(); // 防止重复
    }

    IModeContext* QHFModeRegistry::createMode(const EditorName& editorName, const ModeName& modeName,
                                            IEditorContext* context, QWidget* parent) const {
        auto key  = makeModeKey(editorName, modeName);
        auto iter = m_creators.find(key);
        if (iter != m_creators.end()) {
            return iter.value()(context, parent);
        }
        qWarning() << "Mode not found:" << modeName << "for context:" << editorName;
        return nullptr;
    }

    QStringList QHFModeRegistry::modeNames(const QString& editorName) const {
        return m_editorToModes.value(editorName, {});
    }
} // namespace QHF
