#include "RuleEditor.h"

#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

#include "gui/editor/rules/NewAssignment.h"
#include "gui/editor/rules/NewCondition.h"
#include "gui/editor/rules/NewRule.h"
#include "knowledge/rules/RuleDatabase.hpp"
#include "utility/Singleton.hpp"

RuleEditor::RuleEditor(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Ensure that the Rules can actually be seen
    UpdateRuleEditor();
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

    // Gather the name of the Rule and update the editors
    current_rule_name_ = selection->text().toStdString();
    current_condition_index_ = std::nullopt;
    current_response_index_ = std::nullopt;
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

    // Remove the user's selection and update the editor
    current_condition_index_ = std::nullopt;
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
    // Get the selected Rule
    auto& rule_database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    auto& selected_rule = rule_database.managed_rules_.at(current_rule_name_.value());

    // Split the logic depending on the source
    if (current_condition_index_ == std::nullopt) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot save an unselected Condition!");
        error_indication.exec();
        return;
    } else if (current_condition_index_.value() != 0) {
        // Get the reference to the selected condition
        auto chained_condition = selected_rule.trigger_.condition_chain_.begin();
        std::advance(chained_condition, current_condition_index_.value() - 1);

        // Split the logic relative to the condition type
        switch (chained_condition->second.type_) {
            case expert_system::utility::ExpertSystemTypes::kBool: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::BoolCondition>(chained_condition->second.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    } case 1: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kOr;
                        break;
                    } case 2: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kXor;
                        break;
                    } default: {
                        // Treat this as if it were index 0
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = (QString("True").compare(ui.TargetEditor->text()) == 0);

                // Finished
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kInt: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::IntCondition>(chained_condition->second.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    } case 1: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kOr;
                        break;
                    } case 2: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kXor;
                        break;
                    } default: {
                        // Treat this as if it were index 0
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = ui.TargetEditor->text().toInt();

                // Finished
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kFloat: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::FloatCondition>(chained_condition->second.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    } case 1: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kOr;
                        break;
                    } case 2: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kXor;
                        break;
                    } default: {
                        // Treat this as if it were index 0
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = ui.TargetEditor->text().toFloat();

                // Finished
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kEnum: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::EnumCondition>(chained_condition->second.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    } case 1: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kOr;
                        break;
                    } case 2: {
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kXor;
                        break;
                    } default: {
                        // Treat this as if it were index 0
                        chained_condition->first = expert_system::knowledge::rules::ConnectorType::kAnd;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = ui.TargetEditor->text().toStdString();

                // Finished
                break;
            }
            default: {
                // Do nothing
                return;
            }
        }
    } else if (current_condition_index_.value() == 0) {
        // Get the reference to the root condition
        auto& root_condition = selected_rule.trigger_.root_condition_;

        // Split the logic relative to the condition type
        switch (root_condition.type_) {
            case expert_system::utility::ExpertSystemTypes::kBool: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::BoolCondition>(root_condition.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = (QString("True").compare(ui.TargetEditor->text()) == 0);

                // Finished
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kInt: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::IntCondition>(root_condition.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = ui.TargetEditor->text().toInt();

                // Finished
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kFloat: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::FloatCondition>(root_condition.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = ui.TargetEditor->text().toFloat();

                // Finished
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kEnum: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::EnumCondition>(root_condition.condition_);

                // Store the values in the data fields
                raw_condition.invert_ = ui.OperationLabelInversion->isChecked();
                switch (ui.OperationSelection->currentIndex()) {
                    case 0: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    } case 1: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kLessThan;
                        break;
                    } case 2: {
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kGreaterThan;
                        break;
                    } default: {
                        // Treat this as if it was index 0
                        raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
                        break;
                    }
                }
                raw_condition.confidence_factor_.Set((float) ui.MinimumConfidenceEditor->value());

                // Get the target value
                raw_condition.target_ = ui.TargetEditor->text().toStdString();

                // Finished
                break;
            }
            default: {
                // Do nothing
                return;
            }
        }
    } else {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot save an invalid Condition!");
        error_indication.exec();
        return;
    }
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

    // Remove the user's selection and update the editor
    current_response_index_ = std::nullopt;
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
    // Get the selected Rule
    auto& rule_database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    auto& selected_rule = rule_database.managed_rules_.at(current_rule_name_.value());

    // Get the reference to the selected Assignment
    auto selected_assignment = selected_rule.response_.assignments_.begin();
    std::advance(selected_assignment, current_response_index_.value());

    // Split logic depending on the Assignment's type
    switch (selected_assignment->type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::BoolAssignment>(selected_assignment->assignment_);

            // Store the values
            raw_assignment.value_ = (QString("True").compare(ui.ValueEditor->text()) == 0);
            raw_assignment.confidence_factor_.Set((float) ui.OutcomeConfidenceEditor->value());
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::IntAssignment>(selected_assignment->assignment_);

            // Store the values
            raw_assignment.value_ = ui.ValueEditor->text().toInt();
            raw_assignment.confidence_factor_.Set((float) ui.OutcomeConfidenceEditor->value());
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::FloatAssignment>(selected_assignment->assignment_);

            // Store the values
            raw_assignment.value_ = ui.ValueEditor->text().toFloat();
            raw_assignment.confidence_factor_.Set((float) ui.OutcomeConfidenceEditor->value());
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::EnumAssignment>(selected_assignment->assignment_);

            // Store the values
            raw_assignment.value_ = ui.ValueEditor->text().toStdString();
            raw_assignment.confidence_factor_.Set((float) ui.OutcomeConfidenceEditor->value());
            break;
        }
        default: {
            // Do nothing
            break;
        }
    }
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

    // Split the logic depending on the source
    if (current_condition_index_ == std::nullopt) {
        // Just stop now, no need to render anything else
        return;
    } else if (current_condition_index_.value() != 0) {
        // Get the reference to the selected condition
        auto chained_condition = selected_rule.trigger_.condition_chain_.begin();
        std::advance(chained_condition, current_condition_index_.value() - 1);

        // Split logic based on the Condition's type
        switch (chained_condition->second.type_) {
            case expert_system::utility::ExpertSystemTypes::kBool: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::BoolCondition>(chained_condition->second.condition_);

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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (chained_condition->first) {
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

                // Finished inserting data
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kInt: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::IntCondition>(chained_condition->second.condition_);

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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (chained_condition->first) {
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

                // Finished inserting data
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kFloat: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::FloatCondition>(chained_condition->second.condition_);

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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (chained_condition->first) {
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

                // Update the Condition connector index
                ui.ConnectorSelection->setEnabled(true);

                // Finished inserting data
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kEnum: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::EnumCondition>(chained_condition->second.condition_);

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
                auto& target_fact = fact_database.Find(raw_condition.fact_).value().get();
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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Update the Condition's connector list
                ui.ConnectorSelection->insertItem(0, "AND");
                ui.ConnectorSelection->insertItem(1, "OR");
                ui.ConnectorSelection->insertItem(2, "XOR");
                switch (chained_condition->first) {
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

                // Update The Condition's connector index
                ui.ConnectorSelection->setEnabled(true);

                // Finished inserting data
                break;
            }
            default: {
                // Don't enable user-interaction with the system
                return;
            }
        }
    } else if (current_condition_index_.value() == 0) {
        // Get the reference to the root condition
        auto& root_condition = selected_rule.trigger_.root_condition_;

        // Split logic based on the Condition's type
        switch (root_condition.type_) {
            case expert_system::utility::ExpertSystemTypes::kBool: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::BoolCondition>(root_condition.condition_);

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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Finished inserting data
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kInt: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::IntCondition>(root_condition.condition_);

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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Finished inserting data
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kFloat: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::FloatCondition>(root_condition.condition_);

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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Finished inserting data
                break;
            }
            case expert_system::utility::ExpertSystemTypes::kEnum: {
                // Get the raw Condition
                auto& raw_condition = std::get<expert_system::knowledge::rules::EnumCondition>(root_condition.condition_);

                // Populate the Condition editor's target Fact data
                ui.TypeIndicatorCondition->setText("Enumeration");
                ui.FactIndicatorCondition->setText(raw_condition.fact_.c_str());

                // Update the Condition editor's operation
                ui.OperationLabelInversion->setChecked(raw_condition.invert_);
                ui.OperationSelection->insertItem(0, "Equal to (==)");
                ui.OperationSelection->insertItem(1, "Less than (<)");
                ui.OperationSelection->insertItem(2, "More than (>)");
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
                auto& target_fact = fact_database.Find(raw_condition.fact_).value().get();
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
                ui.MinimumConfidenceEditor->setValue((double) raw_condition.confidence_factor_.Get());

                // Finished inserting data
                break;
            }
            default: {
                // Don't enable user-interaction with the system
                return;
            }
        }

        // Visibly indicate this is the root condition
        ui.ConnectorSelection->insertItem(0, "ROOT");
        ui.ConnectorSelection->setEnabled(false);
    } else {
        // Just stop now, no need to render anything else
        return;
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

    // Get the selected Assignment
    auto selected_assignment = selected_rule.response_.assignments_.begin();
    if (current_response_index_ == std::nullopt) {
        // Just stop now, no need to render anything else
        return;
    }
    std::advance(selected_assignment, current_response_index_.value());

    // Split logic based on the Assignment's type
    switch (selected_assignment->type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::BoolAssignment>(selected_assignment->assignment_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorAssignment->setText("Boolean");
            ui.FactIndicatorAssignment->setText(raw_assignment.fact_.c_str());

            // Update the target value
            static QRegExp boolExpr("True|False");
            ui.ValueEditor->setValidator(new QRegExpValidator(boolExpr, this));
            if (raw_assignment.value_) {
                ui.ValueEditor->setText("True");
            } else {
                ui.ValueEditor->setText("False");
            }

            // Update the outcome confidence value
            ui.OutcomeConfidenceEditor->setValue((double) raw_assignment.confidence_factor_.Get());

            // Finished inserting data
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::IntAssignment>(selected_assignment->assignment_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorAssignment->setText("Integer");
            ui.FactIndicatorAssignment->setText(raw_assignment.fact_.c_str());

            // Update the target value and confidence factor
            ui.ValueEditor->setValidator(new QIntValidator(std::numeric_limits<int>::min(),
                                                            std::numeric_limits<int>::max(),
                                                            this));
            ui.ValueEditor->setText(QString::number(raw_assignment.value_));

            // Update the outcome confidence value
            ui.OutcomeConfidenceEditor->setValue((double) raw_assignment.confidence_factor_.Get());

            // Finished inserting data
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::FloatAssignment>(selected_assignment->assignment_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorAssignment->setText("Float");
            ui.FactIndicatorAssignment->setText(raw_assignment.fact_.c_str());

            // Update the target value
            ui.ValueEditor->setValidator(new QDoubleValidator((double) std::numeric_limits<float>::min(),
                                                               (double) std::numeric_limits<float>::max(),
                                                               std::numeric_limits<float>::digits, this));
            ui.ValueEditor->setText(QString::number(raw_assignment.value_));

            // Update the outcome confidence value
            ui.OutcomeConfidenceEditor->setValue((double) raw_assignment.confidence_factor_.Get());

            // Finished inserting data
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Get the raw Assignment
            auto& raw_assignment = std::get<expert_system::knowledge::rules::EnumAssignment>(selected_assignment->assignment_);

            // Populate the Condition editor's target Fact data
            ui.TypeIndicatorAssignment->setText("Enumeration");
            ui.FactIndicatorAssignment->setText(raw_assignment.fact_.c_str());

            // Update the Assignment value
            auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
            auto& target_fact = fact_database.Find(raw_assignment.fact_).value().get();
            auto& raw_fact = std::get<expert_system::knowledge::facts::EnumFact>(target_fact.fact_);
            int element_counter = 0;
            QString enumExprStr;
            auto enum_list = raw_fact.enum_.List();
            for (const auto& current_enum: enum_list) {
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
            ui.ValueEditor->setValidator(new QRegExpValidator(enumExpr, this));
            ui.ValueEditor->setText(raw_assignment.value_.c_str());

            // Update the outcome confidence value
            ui.OutcomeConfidenceEditor->setValue((double) raw_assignment.confidence_factor_.Get());

            // Finished inserting data
            break;
        }
        default: {
            // Don't enable user-interaction with the editor
            return;
        }
    }

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
    ui.FactIndicatorCondition->clear();
    ui.TargetEditor->clear();
    ui.TargetEditor->setValidator(nullptr);
    ui.MinimumConfidenceEditor->clear();
    ui.ConnectorSelection->clear();
    ui.ConditionList->clear();
    ui.ConditionFrame->setEnabled(false);
}

void RuleEditor::ClearConsequentEditor() {
    // Clear and lock the content in the Assignment editor
    ui.TypeIndicatorAssignment->clear();
    ui.FactIndicatorAssignment->clear();
    ui.ValueEditor->clear();
    ui.ValueEditor->setValidator(nullptr);
    ui.OutcomeConfidenceEditor->clear();
    ui.AssignmentList->clear();
    ui.AssignmentFrame->setEnabled(false);
}
