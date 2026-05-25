#include "SplitDemoWorkspace.h"
#include "KanbanDemoWorkspace.h"
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
    app.setApplicationName("QHiveFrame Uni Demo");
    app.setOrganizationName("QHiveFrame");

    // Register workspaces — QHFAppMainWindow discovers them via QHFWorkspaceRegistry
    QHF::QHFWorkspaceRegistry::registerWorkspace<SplitDemoWorkspace>(
        QHF::QHFWorkspaceType::SPLIT_DEMO, 5000);
    QHF::QHFWorkspaceRegistry::registerWorkspace<KanbanDemoWorkspace>(
        QHF::QHFWorkspaceType::KANBAN_DEMO, 5100);

    // Create the main window — it will initialize all registered workspaces
    QHF::QHFAppMainWindow window;
    window.setWindowTitle(QStringLiteral("QHiveFrame — Uni Demo"));
    window.resize(1200, 800);
    window.show();

    qDebug() << "QHiveFrame Uni Demo started.";
    qDebug() << "Features demonstrated:";
    qDebug() << "  - Multiple workspaces (Split Demo / Kanban Demo)";
    qDebug() << "  - Split editor layout (project tree | task detail)";
    qDebug() << "  - Multiple modes per editor (tree/list, view/edit)";
    qDebug() << "  - QHFCollapsibleSection, QSpinBox, Form widgets";
    qDebug() << "  - Kanban board with 3-column layout";

    return app.exec();
}
