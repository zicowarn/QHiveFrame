#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTest>
#include "../core/QHFPreferencesManager.h"

using namespace QHF;

class TestPreferences : public QObject {
    Q_OBJECT

  private slots:

    // ── SemanticVersion ──
    void testVersionParsing()
    {
        SemanticVersion v1("1.2.3");
        QCOMPARE(v1.toString(), QString("1.2.3"));

        SemanticVersion v2("0.0.0");
        QCOMPARE(v2.toString(), QString("0.0.0"));
    }

    void testVersionComparison()
    {
        SemanticVersion v1("2.0.0");
        SemanticVersion v2("1.0.0");
        SemanticVersion v3("2.0.0");

        QVERIFY(v1 > v2);
        QVERIFY(v2 < v1);
        QVERIFY(v1 == v3);
        QVERIFY(v1 <= v3);
        QVERIFY(v2 <= v1);
    }

    void testVersionEquality()
    {
        SemanticVersion a("1.0.0");
        SemanticVersion b("1.0.0");
        SemanticVersion c("1.0.1");

        QVERIFY(a == b);
        QVERIFY(!(a == c));
    }

    void testVersionLessThan()
    {
        SemanticVersion v1("1.0.0");
        SemanticVersion v2("2.0.0");
        SemanticVersion v3("1.5.0");

        QVERIFY(v1 < v2);
        QVERIFY(v1 < v3);
        QVERIFY(v3 < v2);
    }

    // ── RenameKeyRule ──
    void testRenameKeyRule()
    {
        QJsonObject settings;
        QJsonObject general;
        general["oldSetting"] = QJsonValue(42);
        settings["general"]   = general;

        RenameKeyRule rule(SemanticVersion("2.0.0"), "general.oldSetting", "general.newSetting");
        rule.apply(settings);

        QVERIFY(!settings["general"].toObject().contains("oldSetting"));
        QVERIFY(settings["general"].toObject().contains("newSetting"));
        QCOMPARE(settings["general"].toObject()["newSetting"].toInt(), 42);
    }

    void testRenameKeyCrossCategory()
    {
        QJsonObject settings;
        QJsonObject oldCat;
        oldCat["key"] = QJsonValue("value");
        settings["oldCat"] = oldCat;

        RenameKeyRule rule(SemanticVersion("2.0.0"), "oldCat.key", "newCat.key");
        rule.apply(settings);

        QVERIFY(!settings.contains("oldCat") || !settings["oldCat"].toObject().contains("key"));
        QVERIFY(settings["newCat"].toObject().contains("key"));
        QCOMPARE(settings["newCat"].toObject()["key"].toString(), QString("value"));
    }

    // ── ConvertValueRule ──
    void testConvertValueRule()
    {
        QJsonObject settings;
        QJsonObject general;
        general["count"] = QJsonValue(5);
        settings["general"] = general;

        ConvertValueRule rule(SemanticVersion("2.0.0"), "general.count",
                              [](const QJsonValue& v) { return QJsonValue(v.toInt() * 2); });
        rule.apply(settings);

        QCOMPARE(settings["general"].toObject()["count"].toInt(), 10);
    }

    // ── RemoveKeyRule ──
    void testRemoveKeyRule()
    {
        QJsonObject settings;
        QJsonObject general;
        general["deprecated"] = QJsonValue("old");
        general["keep"]       = QJsonValue("new");
        settings["general"]   = general;

        RemoveKeyRule rule(SemanticVersion("2.0.0"), "general.deprecated");
        rule.apply(settings);

        QVERIFY(!settings["general"].toObject().contains("deprecated"));
        QVERIFY(settings["general"].toObject().contains("keep"));
    }

    // ── CustomRule ──
    void testCustomRule()
    {
        QJsonObject settings;
        QJsonObject general;
        general["value"] = QJsonValue(0);
        settings["general"] = general;

        CustomRule rule(SemanticVersion("3.0.0"),
                        [](QJsonObject& s) {
                            QJsonObject g = s["general"].toObject();
                            g["value"]    = QJsonValue(100);
                            s["general"]  = g;
                        });
        rule.apply(settings);

        QCOMPARE(settings["general"].toObject()["value"].toInt(), 100);
    }

    // ── Edge cases ──
    void testRuleOnEmptySettings()
    {
        QJsonObject empty;
        RenameKeyRule rule(SemanticVersion("2.0.0"), "a.b", "x.y");
        // Should not crash or throw
        rule.apply(empty);
        QVERIFY(empty.isEmpty());
    }

    void testRuleOnNonExistentKey()
    {
        QJsonObject settings;
        QJsonObject cat;
        cat["a"] = QJsonValue(1);
        settings["cat"] = cat;

        RemoveKeyRule rule(SemanticVersion("2.0.0"), "cat.nonexistent");
        rule.apply(settings);
        QCOMPARE(settings["cat"].toObject().size(), 1);
    }
};

QTEST_MAIN(TestPreferences)
#include "test_preferences.moc"
