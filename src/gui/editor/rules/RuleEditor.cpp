#include "RuleEditor.h"

#include <QMessageBox>

#include "gui/editor/rules/NewAssignment.h"
#include "gui/editor/rules/NewCondition.h"
#include "gui/editor/rules/NewRule.h"
#include "knowledge/rules/Antecedent.hpp"
#include "knowledge/rules/RuleDatabase.hpp"
#include "utility/Singleton.hpp"

RuleEditor::RuleEditor(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);
}

void RuleEditor::RuleSelected() {
    // Track the name of the Rule that was selected
    auto selection = ui.RuleList->currentItem();
    if (selection == nullptr) {
        // Indicate no Fact is currently stored and stop
        current_rule_name_ = std::nullopt;
        ClearRuleEditor();
        return;
    }

    // Gather the name of the Fact and update the editors
    current_rule_name_ = selection->text().toStdString();
    UpdateAntecedentEditor();
    UpdateConsequentEditor();

    // Ensure that the user can actually interact with the Rule
    ui.RuleFrame->setEnabled(true);
}

void RuleEditor::NewRule() {
    // Open the NewRule dialog
    class NewRule modal_dialog(this);
    modal_dialog.exec();

    // Update the whole Rule Editor
    UpdateRuleEditor();
}

void RuleEditor::DeleteRule() {
    // Find the selected Rule string
    if (current_rule_name_ == std::nullopt) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot delete an unselected Rule!");
        error_indication.exec();
        return;
    }

    // Get the selected Rule's name and delete it
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    database.managed_rules_.erase(current_rule_name_.value());
    UpdateRuleEditor();
}

void RuleEditor::ConditionSelected() {
    // Store the index of the selected condition and update the editor
    current_condition_index_ = ui.ConditionList->currentRow();
    UpdateAntecedentEditor();
}

void RuleEditor::NewCondition() {
    // Get the database and check the current rule is valid
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get().managed_rules_;
    auto current_rule = database.find(current_rule_name_.value());
    if (current_rule == database.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot operate an invalid Rule!");
        error_indication.exec();
        return;
    }

    // Pass the Condition creation over to a new dialog
    class NewCondition condition_dialog(this, current_rule->second);
    condition_dialog.exec();

    // Refresh only this editor
    UpdateAntecedentEditor();
}

void RuleEditor::DeleteCondition() {
    // Get the database and check the current rule is valid
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get().managed_rules_;
    if (database.find(current_rule_name_.value()) == database.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot operate an invalid Rule!");
        error_indication.exec();
        return;
    }

    // Check that the user has selected a Condition
    if (current_condition_index_ == std::nullopt) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot delete an unselected Condition!");
        error_indication.exec();
        return;
    }

    // Catch the selected Condition being the root
    if (current_condition_index_.value() == 0) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: the root Condition cannot be deleted!");
        error_indication.exec();
        return;
    }

    // Delete the selected condition
    auto& conditions = database.at(current_rule_name_.value()).trigger_.condition_chain_;
    auto condition_iterator = conditions.begin();
    std::advance(condition_iterator, current_condition_index_.value() - 1);
    conditions.erase(condition_iterator);

    // Remove the user's selection and update the editor
    current_condition_index_ = std::nullopt;
    UpdateAntecedentEditor();
}

void RuleEditor::SaveCondition() {
    //TODO: this
}

void RuleEditor::AssignmentSelected() {
    // Store the index of the selected condition and update the editor
    current_response_index_ = ui.AssignmentList->currentRow();
    UpdateConsequentEditor();
}

void RuleEditor::NewAssignment() {
    // Get the database and check the current rule is valid
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get().managed_rules_;
    auto current_rule = database.find(current_rule_name_.value());
    if (database.find(current_rule_name_.value()) == database.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot operate on invalid Rule!");
        error_indication.exec();
        return;
    }

    // Pass the Condition creation over to a new dialog
    class NewAssignment assignment_dialog(this, current_rule->second);
    assignment_dialog.exec();

    // Refresh only this editor
    UpdateConsequentEditor();
}

void RuleEditor::DeleteAssignment() {
    // Get the database and check the current rule is valid
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get().managed_rules_;
    if (database.find(current_rule_name_.value()) == database.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot operate an invalid Rule!");
        error_indication.exec();
        return;
    }

    // Check that the user has selected an Assignment
    if (current_response_index_ == std::nullopt) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot delete an unselected Assignment!");
        error_indication.exec();
        return;
    }

    // Delete the selected Assignment
    auto& assignments = database.at(current_rule_name_.value()).response_.assignments_;
    auto assignment_iterator = assignments.begin();
    std::advance(assignment_iterator, current_response_index_.value());
    assignments.erase(assignment_iterator);

    // Remove the user's selection and update the editor
    current_response_index_ = std::nullopt;
    UpdateConsequentEditor();
}

void RuleEditor::SaveAssignment() {
    //TODO: this
}

void RuleEditor::UpdateRuleEditor() {
    // Clear the editor to prevent older cruft from getting through
    ClearRuleEditor();

    // Iterate through the stored Rules
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    for (const auto& current_rule: database.managed_rules_) {
        // Add the Rule to the Editor's list
        ui.RuleList->addItem(current_rule.first.c_str());
    }

    // Update the other editors, it should only fill in their lists
    UpdateAntecedentEditor();
    UpdateConsequentEditor();
}

void RuleEditor::UpdateAntecedentEditor() {
    // Clear the editor to prevent older cruft from getting through
    ClearAntecedentEditor();

    // Catch no Rule being selected
    if (current_rule_name_ == std::nullopt) {
        // Simply stop
        return;
    }

    // Add all of the stored Conditions to the list
    auto& rule_database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    auto& selected_rule = rule_database.managed_rules_.at(current_rule_name_.value());
    ui.ConditionList->addItem("Root Condition");
    int current_position = 1;
    for (auto chained_condition: selected_rule.trigger_.condition_chain_) {
        // Just indicate which part of the chain the condition is in
        ui.ConditionList->addItem("Chained Condition #" + QString::number(current_position));
        current_position++;
    }

    // Catch if the user has not selected a Condition
    if (current_condition_index_ == std::nullopt) {
        // Just stop now, no need to render anything else
        return;
    }

    // Fill in the rest of the editor
    //TODO: THIS

    // Finish by enabling user interactivity with the editor
    ui.ConditionFrame->setEnabled(true);
}

void RuleEditor::UpdateConsequentEditor() {
    // Clear the editor to prevent older cruft from getting through
    ClearConsequentEditor();

    // Catch no Rule being selected
    if (current_rule_name_ == std::nullopt) {
        // Just stop
        return;
    }

    // Iterate through the stored Assignments
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    int current_position = 0;
    auto& selected_rule = database.managed_rules_.at(current_rule_name_.value());
    for (auto chained_condition: selected_rule.response_.assignments_) {
        // Just indicate which part of the list the Assignment is in
        ui.AssignmentList->addItem("Assignment #" + QString::number(current_position + 1));
        current_position++;
    }

    // Catch if the user has not selected an Assignment
    if (current_response_index_ == std::nullopt) {
        // Just stop now, no need to render anything else
        return;
    }

    // Fill in the rest of the editor
    //TODO: THIS

    // Finish by enabling user interactivity with the editor
    ui.AssignmentFrame->setEnabled(true);
}

void RuleEditor::ClearRuleEditor() {
    // Ensure that the user still hasn't 'selected' content that should be cleared
    current_rule_name_ = std::nullopt;
    current_condition_index_ = std::nullopt;
    current_response_index_ = std::nullopt;

    // Clear and disable the Rule component editors
    ClearAntecedentEditor();
    ClearConsequentEditor();

    // Clear the Lists/Description and lock the editors
    ui.RuleList->clear();
    ui.DescriptionEditor->clear();
    ui.RuleFrame->setEnabled(false);
}

void RuleEditor::ClearAntecedentEditor() {
    // Clear and lock the content in the Condition editor
    ui.TypeIndicatorCondition->clear();
    ui.OperationSelection->clear();
    ui.TargetEditor->clear();
    ui.TargetEditor->setValidator(nullptr);
    ui.ConfidenceEditor->clear();
    ui.ConnectorSelection->clear();
    ui.ConditionList->clear();
    ui.ConditionFrame->setEnabled(false);
}

void RuleEditor::ClearConsequentEditor() {
    // Clear and lock the content in the Assignment editor
    ui.TypeIndicatorAssignment->clear();
    ui.ValueEditor->clear();
    ui.AssignmentList->clear();
    ui.AssignmentFrame->setEnabled(false);
}
