#include "ConsultationResult.h"

#include <QTreeWidget>
#include <QListWidget>

#include "knowledge/facts/FactDatabase.hpp"
#include "utility/Singleton.hpp"

ConsultationResult::ConsultationResult(
        std::vector<std::vector<expert_system::engines::explanation::Log>> &logs,
        QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Fill the Fact list
    auto fact_names = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get().List();
    for (auto& current_name: fact_names) {
        // Add the Fact name to the list
        new QListWidgetItem(current_name.c_str(), ui.FactList);
    }

    // Iterate through each step in the process tree
    int step_counter = 1;
    for (auto& current_step: logs) {
        // Catch if the current step is empty
        if (current_step.empty()) {
            // Move onto the next step
            continue;
        }

        // Create a root tree item
        auto *item_root = new QTreeWidgetItem(ui.ProcessTree);
        item_root->setText(0, QString("Step #") + QString::number(step_counter));

        // Iterate through the logs for this step
        for (auto& current_log: current_step) {
            // Create a new tree item
            auto *item_source = new QTreeWidgetItem(item_root);

            // Catch if a rule was the source of the assignment
            if (current_log.rule_.has_value()) {
                // Indicate the source
                item_source->setText(0, current_log.rule_.value()->first.c_str());
                item_source->setText(1, "(Rule)");
            } else {
                // Indicate the source
                item_source->setText(1, "(User Input)");

                // Catch if this was the initial input
                if (step_counter == 1) {
                    // Indicate why it was provided by the user
                    item_source->setText(0, "Initial Value");
                }
                else {
                    // Indicate why it was provided by the user
                    item_source->setText(0, "Requested Data");
                }
            }

            // Iterate through the log's contents
            for (auto& current_assignment: current_log.assignments_) {
                // Keep track of the name of the Fact that was assigned
                auto fact_name = current_assignment.first;

                // Process the raw log assignment
                switch (current_assignment.second.type_) {
                    case expert_system::utility::ExpertSystemTypes::kBool: {
                        // Get the raw assigned value data
                        auto& raw_data = std::get<expert_system::knowledge::facts::BoolValue>(current_assignment.second.value_);

                        // Create and configure the new tree item
                        auto *item_value = new QTreeWidgetItem(item_source);
                        item_value->setText(0, fact_name.c_str());
                        if (raw_data.value_) {
                            // Post the data as a string
                            item_value->setText(1, "True");
                        }
                        else {
                            // Post the data as a string
                            item_value->setText(1, "False");
                        }
                        item_value->setText(2, QString::number(raw_data.confidence_factor_.Get()));

                        // Done
                        break;
                    }
                    case expert_system::utility::ExpertSystemTypes::kInt: {
                        // Get the raw assigned value data
                        auto& raw_data = std::get<expert_system::knowledge::facts::IntValue>(current_assignment.second.value_);

                        // Create and configure the new tree item
                        auto *item_value = new QTreeWidgetItem(item_source);
                        item_value->setText(0, fact_name.c_str());
                        item_value->setText(1, QString::number(raw_data.value_));
                        item_value->setText(2, QString::number(raw_data.confidence_factor_.Get()));

                        // Done
                        break;
                    }
                    case expert_system::utility::ExpertSystemTypes::kFloat: {
                        // Get the raw assigned value data
                        auto& raw_data = std::get<expert_system::knowledge::facts::FloatValue>(current_assignment.second.value_);

                        // Create and configure the new tree item
                        auto *item_value = new QTreeWidgetItem(item_source);
                        item_value->setText(0, fact_name.c_str());
                        item_value->setText(1, QString::number(raw_data.value_));
                        item_value->setText(2, QString::number(raw_data.confidence_factor_.Get()));

                        // Done
                        break;
                    }
                    case expert_system::utility::ExpertSystemTypes::kEnum: {
                        // Get the raw assigned value data
                        auto& raw_data = std::get<expert_system::knowledge::facts::EnumValue>(current_assignment.second.value_);

                        // Create and configure the new tree item
                        auto *item_value = new QTreeWidgetItem(item_source);
                        item_value->setText(0, fact_name.c_str());
                        item_value->setText(1, raw_data.enum_.At(raw_data.value_.value_)->c_str());
                        item_value->setText(2, QString::number(raw_data.value_.confidence_factor_.Get()));

                        // Done
                        break;
                    }
                    default: {
                        // Spit out an error
                        auto *item_value = new QTreeWidgetItem(item_source);
                        item_value->setText(0, "Error");
                        item_value->setText(1, "Error");
                        item_value->setText(2, "Error");
                    }
                }
            }
        }

        // Increment the step counter
        step_counter++;
    }
}

void ConsultationResult::FactSelected() {
    // Attempt to gather the selected Fact
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto search_result = database.Find(ui.FactList->currentItem()->text().toStdString());
    if (search_result == std::nullopt) {
        // Just indicate the error
        ui.FactValue->setText("Error");
        ui.Confidence->setText("Error");
    }

    // Process the Fact in its raw type
    auto& fact_reference = search_result.value().get();
    switch (fact_reference.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::BoolFact>(fact_reference.fact_);

            // Gather and present the Fact's session value data if it has one
            auto fact_value = fact_raw.GetValue();
            if (fact_value.has_value()) {
                // Present the raw value
                if (fact_value.value().value_) {
                    ui.FactValue->setText("True");
                } else {
                    ui.FactValue->setText("False");
                }

                // Present the confidence factor
                ui.Confidence->setText(QString::number(fact_value.value().confidence_factor_.Get()));
            }
            else {
                // Just indicate the error
                ui.FactValue->clear();
                ui.Confidence->clear();
            }
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::IntFact>(fact_reference.fact_);

            // Gather and present the Fact's session value data if it has one
            auto fact_value = fact_raw.GetValue();
            if (fact_value.has_value()) {
                // Present the raw value
                ui.FactValue->setText(QString::number(fact_value.value().value_));

                // Present the confidence factor
                ui.Confidence->setText(QString::number(fact_value.value().confidence_factor_.Get()));
            }
            else {
                // Just indicate the error
                ui.FactValue->clear();
                ui.Confidence->clear();
            }
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::FloatFact>(fact_reference.fact_);

            // Gather and present the Fact's session value data if it has one
            auto fact_value = fact_raw.GetValue();
            if (fact_value.has_value()) {
                // Present the raw value
                ui.FactValue->setText(QString::number(fact_value.value().value_));

                // Present the confidence factor
                ui.Confidence->setText(QString::number(fact_value.value().confidence_factor_.Get()));
            }
            else {
                // Just indicate the error
                ui.FactValue->clear();
                ui.Confidence->clear();
            }
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::EnumFact>(fact_reference.fact_);

            // Gather and present the Fact's session value data if it has one
            auto fact_value = fact_raw.fact_.GetValue();
            if (fact_value.has_value()) {
                // Present the raw value
                ui.FactValue->setText(fact_raw.enum_.At(fact_value.value().value_).value().c_str());

                // Present the confidence factor
                ui.Confidence->setText(QString::number(fact_value.value().confidence_factor_.Get()));
            }
            else {
                // Just indicate the error
                ui.FactValue->clear();
                ui.Confidence->clear();
            }
            break;
        }
        default: {
            // Just indicate the error
            ui.FactValue->setText("Error");
            ui.Confidence->setText("Error");
            break;
        }
    }
}
