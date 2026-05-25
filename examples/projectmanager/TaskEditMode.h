#ifndef TASKEDITMODE_H
#define TASKEDITMODE_H

#include "QHFModeBase.h"
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>

/**
 * @brief Task edit mode — editable form for task properties.
 * Demonstrates QHFCustomUnitSpinBox via standard QSpinBox (substitution).
 */
class TaskEditMode : public QHF::QHFModeBase {
    Q_OBJECT
public:
    explicit TaskEditMode(QHF::IEditorContext* context, QWidget* parent = nullptr);
    ~TaskEditMode() override = default;

    [[nodiscard]] QString modeName() const override;

private:
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
    void buildForm();

    QLineEdit*   m_titleEdit    = nullptr;
    QTextEdit*   m_descEdit     = nullptr;
    QComboBox*   m_priorityCbo  = nullptr;
    QComboBox*   m_statusCbo    = nullptr;
    QSpinBox*    m_progressSpin = nullptr;
    QDateEdit*   m_deadlineEdit = nullptr;
};

#endif // TASKEDITMODE_H
