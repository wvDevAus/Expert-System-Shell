#include "NewRule.h"

#include <utility>
#include <string>

#include <QMessageBox>

#include "gui/editor/rules/NewCondition.h"
#include "knowledge/rules/Rule.hpp"
#include "knowledge/rules/RuleDatabase.hpp"
#include "utility/Singleton.hpp"

NewRule::NewRule(QWidget *parent) : QDialog(parent) {
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

void NewRule::CreateNewRule() {
    // Catch an empty Rule name
    if (ui.NameEdit->text().isEmpty()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: provided name is empty!");
        error_indication.exec();
        return;
    }

    // Catch a known Rule name
    auto name_string = ui.NameEdit->text().toStdString();
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    if (database.managed_rules_.find(name_string) != database.managed_rules_.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: provided name is already used!");
        error_indication.exec();
        return;
    }

    // Generate the rule
    database.managed_rules_.emplace(name_string, expert_system::knowledge::rules::Rule());
    auto& generated_rule = database.managed_rules_.at(name_string);

    // Split logic based on the select Fact's type
    auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto& selected_fact = fact_database.Find(ui.FactSelector->currentText().toStdString()).value().get();
    switch (selected_fact.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Create the Condition
            generated_rule.trigger_.root_condition_ =
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kBool);
            auto& raw_condition =
                    std::get<expert_system::knowledge::rules::BoolCondition>(generated_rule.trigger_.root_condition_.condition_);

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
            generated_rule.trigger_.root_condition_ =
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kInt);
            auto& raw_condition =
                    std::get<expert_system::knowledge::rules::IntCondition>(generated_rule.trigger_.root_condition_.condition_);

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
            generated_rule.trigger_.root_condition_ =
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kFloat);
            auto& raw_condition =
                    std::get<expert_system::knowledge::rules::FloatCondition>(generated_rule.trigger_.root_condition_.condition_);

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
            generated_rule.trigger_.root_condition_ =
                    expert_system::knowledge::rules::VariantCondition(expert_system::utility::ExpertSystemTypes::kEnum);
            auto& raw_condition =
                    std::get<expert_system::knowledge::rules::EnumCondition>(generated_rule.trigger_.root_condition_.condition_);

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

            // Delete the generated rule and allow the user to retry the dialog
            database.managed_rules_.erase(name_string);
            return;
        }
    }

    // End the dialog
    this->accept();
}
