#include "NewAssignment.h"

#include <utility>
#include <string>

#include <QMessageBox>

#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/rules/Conditions.hpp"
#include "utility/Singleton.hpp"

NewAssignment::NewAssignment(QWidget *parent,
                           std::optional<std::reference_wrapper<expert_system::knowledge::rules::Rule>> current_rule)
        : QDialog(parent), current_rule_(current_rule) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Fill the Fact Selector
    auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    int index_counter = 0;
    for (const auto& current_fact: fact_database.List()) {
        // Add the Fact to the combo box
        ui.FactSelector->insertItem(index_counter, current_fact.c_str());
        index_counter++;
    }
}

void NewAssignment::CreateNewAssignment() {
    // Get the selected Fact
    auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto& selected_fact = fact_database.Find(ui.FactSelector->currentText().toStdString()).value().get();

    // Split logic based on the select Fact's type
    switch (selected_fact.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Create the Assignment
            current_rule_.value().get().response_.assignments_.emplace_back(
                    expert_system::knowledge::rules::VariantAssignment(expert_system::utility::ExpertSystemTypes::kBool));
            auto new_assignment = current_rule_.value().get().response_.assignments_.rbegin();
            auto& raw_condition = std::get<expert_system::knowledge::rules::BoolAssignment>(new_assignment->assignment_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.value_ = false;

            // End the dialog
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Create the Assignment
            current_rule_.value().get().response_.assignments_.emplace_back(
                    expert_system::knowledge::rules::VariantAssignment(expert_system::utility::ExpertSystemTypes::kInt));
            auto new_assignment = current_rule_.value().get().response_.assignments_.rbegin();
            auto& raw_condition = std::get<expert_system::knowledge::rules::IntAssignment>(new_assignment->assignment_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.value_ = 0;

            // End the dialog
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Create the Assignment
            current_rule_.value().get().response_.assignments_.emplace_back(
                    expert_system::knowledge::rules::VariantAssignment(expert_system::utility::ExpertSystemTypes::kFloat));
            auto new_assignment = current_rule_.value().get().response_.assignments_.rbegin();
            auto& raw_condition = std::get<expert_system::knowledge::rules::FloatAssignment>(new_assignment->assignment_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.value_ = 0.0f;

            // End the dialog
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Create the Assignment
            current_rule_.value().get().response_.assignments_.emplace_back(
                    expert_system::knowledge::rules::VariantAssignment(expert_system::utility::ExpertSystemTypes::kEnum));
            auto new_assignment = current_rule_.value().get().response_.assignments_.rbegin();
            auto& raw_condition = std::get<expert_system::knowledge::rules::EnumAssignment>(new_assignment->assignment_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.value_ = "";

            // End the dialog
            break;
        }
        default: {
            // Catch the error and indicate failure
            QMessageBox error_indication;
            error_indication.setText("Error: selected Fact has an unsupported type!");
            error_indication.exec();

            // Don't finish the dialog
            return;
        }
    }

    // Finished
    this->accept();
}
