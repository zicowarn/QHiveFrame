#ifndef PROJECTTREEEDITOR_H
#define PROJECTTREEEDITOR_H

#include "QHFEditorBase.h"

class ProjectTreeEditor : public QHF::QHFEditorBase {
    Q_OBJECT
public:
    explicit ProjectTreeEditor(QWidget* parent = nullptr);
    ~ProjectTreeEditor() override = default;

    [[nodiscard]] QString            editorName() const override;
    [[nodiscard]] QHF::EditorType    editorType() const override;
    [[nodiscard]] QHF::IModeContext* createMode(const QString& modeName) override;
    [[nodiscard]] QString            defaultModeName() const override;

private:
    [[nodiscard]] QList<QPair<QString, QString>> collectModeNames() const override;
    void initializeImpl(const QList<QPair<QString, QString>>& modeNames) override;
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
};

#endif // PROJECTTREEEDITOR_H
