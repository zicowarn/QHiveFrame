#include "ImageViewerWorkspace.h"
#include "QHFAppMainWindow.h"
#include "QHFWorkspaceFactory.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("QHiveFrame ImageViewer");
    app.setOrganizationName("QHiveFrame");

    // Register the ImageViewerWorkspace so QHFAppMainWindow can discover it
    QHF::QHFWorkspaceRegistry::registerWorkspace<ImageViewerWorkspace>(
        QHF::QHFWorkspaceType::DUMMY, 5000);

    // Create the main window — it will find and initialize our workspace
    QHF::QHFAppMainWindow window;
    window.resize(1024, 768);
    window.show();

    qDebug() << "QHiveFrame ImageViewer example started.";
    return app.exec();
}
