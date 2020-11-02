#pragma once

#include <QDialog>

#include "ui_RuleEditor.h"

    /**
     * @brief Expert System Shell Rule editor GUI.
     * Implemented through the Member Variable method.
     */
class RuleEditor : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit RuleEditor(QWidget *parent = nullptr);

private slots:
        /// Processes a selected Rule.
    void RuleSelected();

        /// Opens a new dialog for the user to create a new Rule.
    void NewRule();

        /// Attempts to delete the currently selected Rule.
    void DeleteRule();

        /// Processes a selected Rule Condition.
    void ConditionSelected();

        /// Attempts to generate a new Antecedent Condition.
    void NewCondition();

        /// Attempts to generate the selected Antecedent Condition.
    void DeleteCondition();

        /// Attempts to save the selected Antecedent Condition.
    void SaveCondition();

        /// Reacts to the user changing the Condition's target Fact.
    void ConditionFactSelected();

        /// Processes a selected Rule Condition.
    void AssignmentSelected();

        /// Attempts to generate a new Antecedent Condition.
    void NewAssignment();

        /// Attempts to generate the selected Antecedent Condition.
    void DeleteAssignment();

        /// Attempts to save the selected Antecedent Condition.
    void SaveAssignment();

        /// Reacts to the user changing the Condition's target Fact.
    void AssignmentFactSelected();

private:
        /// Pointer member to auto-generated UI element.
    Ui::RuleEditor ui;
};
