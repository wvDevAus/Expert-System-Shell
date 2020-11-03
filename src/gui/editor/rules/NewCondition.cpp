#include "NewCondition.h"

#include <utility>
#include <string>

#include <QMessageBox>

#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/rules/Conditions.hpp"
#include "utility/Singleton.hpp"

NewCondition::NewCondition(QWidget *parent,
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

void NewCondition::CreateNewCondition() {
    // Get the selected Fact
    auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto& selected_fact = fact_database.Find(ui.FactSelector->currentText().toStdString()).value().get();

    // Split logic based on the select Fact's type
    switch (selected_fact.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Create the Condition
            auto new_condition = current_rule_.value().get().trigger_.condition_chain_.emplace_back(
                    expert_system::knowledge::rules::ConnectorType::kAnd,
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kBool));
            auto& raw_condition = std::get<expert_system::knowledge::rules::BoolCondition>(new_condition.second.condition_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
            raw_condition.invert_ = false;
            raw_condition.target_ = false;
            raw_condition.confidence_factor_.Set(1.0f);

            // End the dialog
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Create the Condition
            auto new_condition = current_rule_.value().get().trigger_.condition_chain_.emplace_back(
                    expert_system::knowledge::rules::ConnectorType::kAnd,
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kInt));
            auto& raw_condition = std::get<expert_system::knowledge::rules::IntCondition>(new_condition.second.condition_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
            raw_condition.invert_ = false;
            raw_condition.target_ = 0;
            raw_condition.confidence_factor_.Set(1.0f);

            // End the dialog
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Create the Condition
            auto new_condition = current_rule_.value().get().trigger_.condition_chain_.emplace_back(
                    expert_system::knowledge::rules::ConnectorType::kAnd,
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kFloat));
            auto& raw_condition = std::get<expert_system::knowledge::rules::FloatCondition>(new_condition.second.condition_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
            raw_condition.invert_ = false;
            raw_condition.target_ = 0.0;
            raw_condition.confidence_factor_.Set(1.0f);

            // End the dialog
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Create the Condition
            auto new_condition = current_rule_.value().get().trigger_.condition_chain_.emplace_back(
                    expert_system::knowledge::rules::ConnectorType::kAnd,
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kEnum));
            auto& raw_condition = std::get<expert_system::knowledge::rules::EnumCondition>(new_condition.second.condition_);

            // Assign the property data
            raw_condition.fact_ = ui.FactSelector->currentText().toStdString();
            raw_condition.condition_ = expert_system::knowledge::rules::ConditionType::kEqualTo;
            raw_condition.invert_ = false;
            raw_condition.target_ = "";
            raw_condition.confidence_factor_.Set(1.0f);

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
