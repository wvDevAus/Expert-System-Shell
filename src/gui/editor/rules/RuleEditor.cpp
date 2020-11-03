#include "RuleEditor.h"

#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

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

    // Get the selected condition
    std::optional<expert_system::knowledge::rules::ConnectorType> condition_connector = std::nullopt;
    std::optional<std::reference_wrapper<expert_system::knowledge::rules::VariantCondition>> selected_condition = std::nullopt;
    if (current_condition_index_ == std::nullopt) {
        // Just stop now, no need to render anything else
        return;
    } else if (current_condition_index_.value() != 0) {
        // Get the reference to the selected condition
        auto selection_iterator = selected_rule.trigger_.condition_chain_.begin();
        std::advance(selection_iterator, current_condition_index_.value() - 1);
        selected_condition = selection_iterator->second;
        condition_connector = selection_iterator->first;
    } else if (current_condition_index_.value() == 0) {
        // Get the reference to the root condition
        selected_condition = selected_rule.trigger_.root_condition_;
    } else {
        // Just stop now, no need to render anything else
        return;
    }

    // Split logic based on the condition's type
    switch (selected_condition.value().get().type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Get the raw Condition
            auto& raw_condition = std::get<expert_system::knowledge::rules::BoolCondition>(selected_condition.value().get().condition_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorCondition->setText("Boolean");
            ui.FactIndicatorCondition->setText(raw_condition.fact_.c_str());

            // Update the Condition editor's operation
            ui.OperationLabelInversion->setChecked(raw_condition.invert_);
            ui.OperationSelection->insertItem(0, "Equal to (==)");
            ui.OperationSelection->insertItem(1, "Less than (>)");
            ui.OperationSelection->insertItem(2, "More than (<)");
            switch (raw_condition.condition_) {
                case expert_system::knowledge::rules::ConditionType::kEqualTo:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
                case expert_system::knowledge::rules::ConditionType::kLessThan:
                    ui.OperationSelection->setCurrentIndex(1);
                    break;
                case expert_system::knowledge::rules::ConditionType::kGreaterThan:
                    ui.OperationSelection->setCurrentIndex(2);
                    break;
                default:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
            }

            // Update the target value and confidence factor
            static QRegExp boolExpr("True|False");
            ui.TargetEditor->setValidator(new QRegExpValidator(boolExpr, this));
            if (raw_condition.target_) {
                ui.TargetEditor->setText("True");
            } else {
                ui.TargetEditor->setText("False");
            }
            ui.ConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

            // Update the Condition connector if its isn't the root Condition
            if (condition_connector.has_value()) {
                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (condition_connector.value()) {
                    case expert_system::knowledge::rules::ConnectorType::kAnd:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kOr:
                        ui.OperationSelection->setCurrentIndex(1);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kXor:
                        ui.OperationSelection->setCurrentIndex(2);
                        break;
                    default:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                }
                ui.ConnectorSelection->setEnabled(true);
            } else {
                // Prevent use
                ui.ConnectorSelection->insertItem(0, "ROOT");
                ui.ConnectorSelection->setEnabled(false);
            }

            // Finished inserting data
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Get the raw Condition
            auto& raw_condition = std::get<expert_system::knowledge::rules::IntCondition>(selected_condition.value().get().condition_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorCondition->setText("Integer");
            ui.FactIndicatorCondition->setText(raw_condition.fact_.c_str());

            // Update the Condition editor's operation
            ui.OperationLabelInversion->setChecked(raw_condition.invert_);
            ui.OperationSelection->insertItem(0, "Equal to (==)");
            ui.OperationSelection->insertItem(1, "Less than (>)");
            ui.OperationSelection->insertItem(2, "More than (<)");
            switch (raw_condition.condition_) {
                case expert_system::knowledge::rules::ConditionType::kEqualTo:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
                case expert_system::knowledge::rules::ConditionType::kLessThan:
                    ui.OperationSelection->setCurrentIndex(1);
                    break;
                case expert_system::knowledge::rules::ConditionType::kGreaterThan:
                    ui.OperationSelection->setCurrentIndex(2);
                    break;
                default:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
            }

            // Update the target value and confidence factor
            ui.TargetEditor->setValidator(new QIntValidator(std::numeric_limits<int>::min(),
                                                            std::numeric_limits<int>::max(),
                                                            this));
            ui.TargetEditor->setText(QString::number(raw_condition.target_));
            ui.ConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

            // Update the Condition connector if its isn't the root Condition
            if (condition_connector.has_value()) {
                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (condition_connector.value()) {
                    case expert_system::knowledge::rules::ConnectorType::kAnd:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kOr:
                        ui.OperationSelection->setCurrentIndex(1);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kXor:
                        ui.OperationSelection->setCurrentIndex(2);
                        break;
                    default:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                }
                ui.ConnectorSelection->setEnabled(true);
            } else {
                // Prevent use
                ui.ConnectorSelection->insertItem(0, "ROOT");
                ui.ConnectorSelection->setEnabled(false);
            }

            // Finished inserting data
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Get the raw Condition
            auto& raw_condition = std::get<expert_system::knowledge::rules::FloatCondition>(selected_condition.value().get().condition_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorCondition->setText("Float");
            ui.FactIndicatorCondition->setText(raw_condition.fact_.c_str());

            // Update the Condition editor's operation
            ui.OperationLabelInversion->setChecked(raw_condition.invert_);
            ui.OperationSelection->insertItem(0, "Equal to (==)");
            ui.OperationSelection->insertItem(1, "Less than (>)");
            ui.OperationSelection->insertItem(2, "More than (<)");
            switch (raw_condition.condition_) {
                case expert_system::knowledge::rules::ConditionType::kEqualTo:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
                case expert_system::knowledge::rules::ConditionType::kLessThan:
                    ui.OperationSelection->setCurrentIndex(1);
                    break;
                case expert_system::knowledge::rules::ConditionType::kGreaterThan:
                    ui.OperationSelection->setCurrentIndex(2);
                    break;
                default:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
            }

            // Update the target value and confidence factor
            ui.TargetEditor->setValidator(new QDoubleValidator((double) std::numeric_limits<float>::min(),
                                                               (double) std::numeric_limits<float>::max(),
                                                               std::numeric_limits<float>::digits, this));
            ui.TargetEditor->setText(QString::number(raw_condition.target_));
            ui.ConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

            // Update the Condition connector if its isn't the root Condition
            if (condition_connector.has_value()) {
                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (condition_connector.value()) {
                    case expert_system::knowledge::rules::ConnectorType::kAnd:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kOr:
                        ui.OperationSelection->setCurrentIndex(1);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kXor:
                        ui.OperationSelection->setCurrentIndex(2);
                        break;
                    default:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                }
                ui.ConnectorSelection->setEnabled(true);
            } else {
                // Prevent use
                ui.ConnectorSelection->insertItem(0, "ROOT");
                ui.ConnectorSelection->setEnabled(false);
            }

            // Finished inserting data
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Get the raw Condition
            auto& raw_condition = std::get<expert_system::knowledge::rules::EnumCondition>(selected_condition.value().get().condition_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorCondition->setText("Enumeration");
            ui.FactIndicatorCondition->setText(raw_condition.fact_.c_str());

            // Update the Condition editor's operation
            ui.OperationLabelInversion->setChecked(raw_condition.invert_);
            ui.OperationSelection->insertItem(0, "Equal to (==)");
            ui.OperationSelection->insertItem(1, "Less than (>)");
            ui.OperationSelection->insertItem(2, "More than (<)");
            switch (raw_condition.condition_) {
                case expert_system::knowledge::rules::ConditionType::kEqualTo:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
                case expert_system::knowledge::rules::ConditionType::kLessThan:
                    ui.OperationSelection->setCurrentIndex(1);
                    break;
                case expert_system::knowledge::rules::ConditionType::kGreaterThan:
                    ui.OperationSelection->setCurrentIndex(2);
                    break;
                default:
                    ui.OperationSelection->setCurrentIndex(0);
                    break;
            }

            // Update the target value and confidence factor
            auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
            auto& target_fact = fact_database.Find(raw_condition.target_).value().get();
            auto& raw_fact = std::get<expert_system::knowledge::facts::EnumFact>(target_fact.fact_);
            int element_counter = 0;
            QString enumExprStr;
            auto enum_list = raw_fact.enum_.List();
            for (auto current_enum: enum_list) {
                // Append the current enum value to the string
                enumExprStr += current_enum.c_str();

                // Prevent attaching the separator to the very end of the string
                if (element_counter != (int) enum_list.size() - 1) {
                    // Append the regex separator
                    enumExprStr += "|";
                }
                element_counter++;
            }
            QRegExp enumExpr(enumExprStr);
            ui.TargetEditor->setValidator(new QRegExpValidator(enumExpr, this));
            ui.TargetEditor->setText(raw_condition.target_.c_str());
            ui.ConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

            // Update the Condition connector if its isn't the root Condition
            if (condition_connector.has_value()) {
                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (condition_connector.value()) {
                    case expert_system::knowledge::rules::ConnectorType::kAnd:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kOr:
                        ui.OperationSelection->setCurrentIndex(1);
                        break;
                    case expert_system::knowledge::rules::ConnectorType::kXor:
                        ui.OperationSelection->setCurrentIndex(2);
                        break;
                    default:
                        ui.OperationSelection->setCurrentIndex(0);
                        break;
                }
                ui.ConnectorSelection->setEnabled(true);
            } else {
                // Prevent use
                ui.ConnectorSelection->insertItem(0, "ROOT");
                ui.ConnectorSelection->setEnabled(false);
            }

            // Finished inserting data
            break;
        }
    }

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
