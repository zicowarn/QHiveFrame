#include "ProjectManageWorkspace.h"
#include "TaskBoardWorkspace.h"
#include "QHFAppMainWindow.h"
#include "QHFWorkspaceFactory.h"

#include <QApplication>
#include <QDebug>
#include <QtCore/QResource>

int main(int argc, char* argv[])
{
    // Import Qt resources from QHiveFrame static library
    // QHFGuiIcons.qrc → provides :/gui/icons/SplitV.svg, Close.svg, etc.
    Q_INIT_RESOURCE(QHFGuiIcons);

    QApplication app(argc, argv);
    app.setApplicationName("QHiveFrame Project Manager");
    app.setOrganizationName("QHiveFrame");

    // Register workspaces — QHFAppMainWindow discovers them via QHFWorkspaceRegistry
    QHF::QHFWorkspaceRegistry::registerWorkspace<ProjectManageWorkspace>(
        QHF::QHFWorkspaceType::PM_PROJECT, 5000);
    QHF::QHFWorkspaceRegistry::registerWorkspace<TaskBoardWorkspace>(
        QHF::QHFWorkspaceType::PM_TASKBOARD, 5100);

    // Create the main window — it will initialize all registered workspaces
    QHF::QHFAppMainWindow window;
    window.setWindowTitle(QStringLiteral("QHiveFrame — 项目管理 Demo"));
    window.resize(1200, 800);
    window.show();

    qDebug() << "QHiveFrame Project Manager demo started.";
    qDebug() << "Features demonstrated:";
    qDebug() << "  - Multiple workspaces (项目管理 / 任务看板)";
    qDebug() << "  - Split editor layout (project tree | task detail)";
    qDebug() << "  - Multiple modes per editor (tree/list, view/edit)";
    qDebug() << "  - QHFCollapsibleSection, QSpinBox, Form widgets";
    qDebug() << "  - Kanban board with 3-column layout";

    return app.exec();
}
