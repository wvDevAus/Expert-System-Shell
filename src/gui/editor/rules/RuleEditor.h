#pragma once

#include <optional>
#include <string>

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

        /// Processes a selected Rule Condition.
    void AssignmentSelected();

        /// Attempts to generate a new Antecedent Condition.
    void NewAssignment();

        /// Attempts to generate the selected Antecedent Condition.
    void DeleteAssignment();

        /// Attempts to save the selected Antecedent Condition.
    void SaveAssignment();

private:
        /// Tracks the user's currently selected Rule.
    std::optional<std::string> current_rule_name_;

        /// Tracks the user's currently selected Antecedent Condition.
    std::optional<int> current_condition_index_;

        /// Tracks the user's currently selected Consequent Assignment.
    std::optional<int> current_response_index_;

        /// Updates the contents of the Rule list.
    void UpdateRuleEditor();

        /// Updates the contents of the Rule editors.
    void UpdateAntecedentEditor();

        /// Updates the contents of the Rule editors.
    void UpdateConsequentEditor();

        /// Resets the state and contents of the Rule editor.
    void ClearRuleEditor();

        /// Resets the state and contents of the Antecedent editor.
    void ClearAntecedentEditor();

        /// Resets the state and contents of the Consequent editor.
    void ClearConsequentEditor();

        /// Pointer member to auto-generated UI element.
    Ui::RuleEditor ui;
};
