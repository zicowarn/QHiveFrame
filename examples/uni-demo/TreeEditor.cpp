#include "TreeEditor.h"
#include "TreeMode.h"
#include "ListMode.h"

#include <QDebug>

TreeEditor::TreeEditor(QWidget* parent)
    : QHF::QHFEditorBase(parent)
{
}

QString TreeEditor::editorName() const
{
    return QStringLiteral("Tree");
}

QHF::EditorType TreeEditor::editorType() const
{
    return QHF::EditorType::TREE;
}

QHF::IModeContext* TreeEditor::createMode(const QString& modeName)
{
    if (modeName == QStringLiteral("ProjectTree")) {
        return new TreeMode(this, this);
    }
    if (modeName == QStringLiteral("ProjectList")) {
        return new ListMode(this, this);
    }
    qWarning() << "[UniDemo] Unknown mode for TreeEditor:" << modeName;
    return nullptr;
}

QString TreeEditor::defaultModeName() const
{
    return QStringLiteral("ProjectTree");
}

QList<QPair<QString, QString>> TreeEditor::collectModeNames() const
{
    return {
        {tr("Tree View"), QStringLiteral("ProjectTree")},
        {tr("List View"), QStringLiteral("ProjectList")},
    };
}

void TreeEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames)
{
    Q_UNUSED(modeNames);
    // Remove DUMMY/NONE from editor type combo (core placeholder editors)
    for (int i = m_editorCombo->count() - 1; i >= 0; --i) {
        const QString typeName = m_editorCombo->itemData(i).toString();
        if (typeName == QStringLiteral("DUMMY") || typeName == QStringLiteral("NONE")) {
            m_editorCombo->removeItem(i);
        }
    }
    setEditorTitle(tr("Items"));
    initializeModeCombo(
        {{tr("Tree View"), QStringLiteral("ProjectTree")},
         {tr("List View"), QStringLiteral("ProjectList")}},
        QStringLiteral("ProjectTree"));
}

void TreeEditor::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
