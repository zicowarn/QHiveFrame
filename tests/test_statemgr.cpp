#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTest>
#include <QTemporaryFile>
#include "../core/QHFStateManager.h"

using namespace QHF;

class TestStateManager : public QObject {
    Q_OBJECT

  private slots:

    void testFileStateReset()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        mgr.resetFileState();
        const FileState& fs = mgr.getFileState();
        QCOMPARE(fs.name, QString("Untitled"));
        // resetFileState clears dirty flag per design
        QVERIFY(!fs.isDirty);
        // Note: getFileState() always sets isLoaded=true internally
    }

    void testSetFileState()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        FileState fs;
        fs.path    = "/tmp/test.qhm";
        fs.name    = "test.qhm";
        fs.isDirty = false;
        fs.isLoaded = true;
        mgr.setFileState(fs);

        const FileState& retrieved = mgr.getFileState();
        QCOMPARE(retrieved.path, QString("/tmp/test.qhm"));
        QCOMPARE(retrieved.name, QString("test.qhm"));
        QVERIFY(!retrieved.isDirty);
        QVERIFY(retrieved.isLoaded);
    }

    void testLanguageSetting()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        mgr.setCurrentLanguage("zh_CN");
        QCOMPARE(mgr.getCurrentLanguage(), QString("zh_CN"));

        mgr.setCurrentLanguage("en_US");
        QCOMPARE(mgr.getCurrentLanguage(), QString("en_US"));
    }

    void testFavoritePlaces()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        QStringList places = {"/home", "/usr/local", "/tmp"};
        mgr.setFavoritePlaces(places);
        QCOMPARE(mgr.getFavoritePlaces(), places);
    }

    void testWindowLifecycle()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        QWidget w;
        w.setObjectName("TestWindow");

        QVERIFY(!mgr.existWindow(&w));
        QCOMPARE(mgr.countWindows(), 0);

        mgr.addWindow(&w);
        QVERIFY(mgr.existWindow(&w));
        QCOMPARE(mgr.countWindows(), 1);

        // removeWindow intentionally keeps at least one window in state
        mgr.removeWindow(&w);
        QVERIFY(mgr.existWindow(&w));
        QCOMPARE(mgr.countWindows(), 1);
    }

    void testActiveWindow()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        QWidget w;
        mgr.setActiveWindow(&w);
        QCOMPARE(mgr.activeWindow(), &w);
    }

    void testBlockingWindowActive()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        QVERIFY(!mgr.isBlockingWindowActive());

        mgr.setBlockingWindowActive(true);
        QVERIFY(mgr.isBlockingWindowActive());

        mgr.setBlockingWindowActive(false);
        QVERIFY(!mgr.isBlockingWindowActive());
    }

    void testWindowState()
    {
        QHFStateManager& mgr = QHFStateManager::instance();
        QWidget w;

        WindowState state;
        state.geometry = QRect(100, 100, 800, 600);
        state.currentWorkspaceIndex = 1;
        state.isShowStatusBar = false;
        state.isFullScreen = false;

        mgr.addWindow(&w);
        mgr.setWindowState(&w, state);

        WindowState retrieved = mgr.getWindowState(&w);
        QCOMPARE(retrieved.geometry, QRect(100, 100, 800, 600));
        QCOMPARE(retrieved.currentWorkspaceIndex, 1);
        QVERIFY(!retrieved.isShowStatusBar);
        QVERIFY(!retrieved.isFullScreen);
    }
};

QTEST_MAIN(TestStateManager)
#include "test_statemgr.moc"
