#include "ProjectTreeEditor.h"
#include "ProjectTreeMode.h"
#include "ProjectListMode.h"

#include <QDebug>

ProjectTreeEditor::ProjectTreeEditor(QWidget* parent)
    : QHF::QHFEditorBase(parent)
{
}

QString ProjectTreeEditor::editorName() const
{
    return QStringLiteral("项目树");
}

QHF::EditorType ProjectTreeEditor::editorType() const
{
    return QHF::EditorType::PM_PROJECT_TREE;
}

QHF::IModeContext* ProjectTreeEditor::createMode(const QString& modeName)
{
    if (modeName == QStringLiteral("ProjectTree")) {
        return new ProjectTreeMode(this, this);
    }
    if (modeName == QStringLiteral("ProjectList")) {
        return new ProjectListMode(this, this);
    }
    qWarning() << "[PM] Unknown mode for ProjectTreeEditor:" << modeName;
    return nullptr;
}

QString ProjectTreeEditor::defaultModeName() const
{
    return QStringLiteral("ProjectTree");
}

QList<QPair<QString, QString>> ProjectTreeEditor::collectModeNames() const
{
    return {
        {tr("树形视图"), QStringLiteral("ProjectTree")},
        {tr("列表视图"), QStringLiteral("ProjectList")},
    };
}

void ProjectTreeEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames)
{
    Q_UNUSED(modeNames);
    // Remove DUMMY/NONE from editor type combo (core placeholder editors)
    for (int i = m_editorCombo->count() - 1; i >= 0; --i) {
        const QString typeName = m_editorCombo->itemData(i).toString();
        if (typeName == QStringLiteral("DUMMY") || typeName == QStringLiteral("NONE")) {
            m_editorCombo->removeItem(i);
        }
    }
    setEditorTitle(tr("项目"));
    initializeModeCombo(
        {{tr("树形视图"), QStringLiteral("ProjectTree")},
         {tr("列表视图"), QStringLiteral("ProjectList")}},
        QStringLiteral("ProjectTree"));
}

void ProjectTreeEditor::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
