#include "ConsultationBegin.h"

#include <QListWidget>
#include <QMessageBox>

#include "utility/Singleton.hpp"
#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/rules/RuleDatabase.hpp"
#include "utility/Confidence.hpp"

ConsultationBegin::ConsultationBegin(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Ensure no Fact is selected
    selected_fact_ = std::nullopt;

    // Fill the Fact list
    auto fact_names = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get().List();
    for (auto& current_name: fact_names) {
        // Add the Fact name to the list
        new QListWidgetItem(current_name.c_str(), ui.FactList);
    }
}

void ConsultationBegin::FactSelected() {
    // Attempt to store the name of the currently selected Fact
    selected_fact_ = ui.FactList->currentItem()->text().toStdString();

    // Update the editor
    UpdateEditor();
}

void ConsultationBegin::SaveFactValue() {
    // Get the selected Fact
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto search_result = database.Find(selected_fact_.value());

    // Catch if the gathered fact is invalid
    if (search_result == std::nullopt) {
        // Catch the error but don't bother the user
        return;
    }

    // Split the logic for the type of fact
    switch (search_result.value().get().type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Get a reference to the raw Fact
            auto& raw_fact = std::get<expert_system::knowledge::facts::BoolFact>(search_result.value().get().fact_);

            // Get the user-provided value
            auto gathered_value = (ui.ValueEditor->text().compare("True") == 0);

            // Create a confidence factor object from the user-provided value
            expert_system::utility::Confidence gathered_confidence((float) ui.ConfidenceEditor->value());

            // Attempt to store the data provided in the editor
            if (!raw_fact.SetValue(gathered_value, gathered_confidence)) {
                // Report the error to the user and stop attempting to save the data
                QMessageBox error_indication;
                error_indication.setText("Error: cannot save the invalid value provided!");
                error_indication.exec();
            }

            // Finished
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Get a reference to the raw Fact
            auto& raw_fact = std::get<expert_system::knowledge::facts::IntFact>(search_result.value().get().fact_);

            // Get the user-provided value
            auto gathered_value = ui.ValueEditor->text().toInt();

            // Create a confidence factor object from the user-provided value
            expert_system::utility::Confidence gathered_confidence((float) ui.ConfidenceEditor->value());

            // Attempt to store the data provided in the editor
            if (!raw_fact.SetValue(gathered_value, gathered_confidence)) {
                // Report the error to the user and stop attempting to save the data
                QMessageBox error_indication;
                error_indication.setText("Error: cannot save the invalid value provided!");
                error_indication.exec();
            }

            // Finished
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Get a reference to the raw Fact
            auto& raw_fact = std::get<expert_system::knowledge::facts::FloatFact>(search_result.value().get().fact_);

            // Get the user-provided value
            auto gathered_value = ui.ValueEditor->text().toFloat();

            // Create a confidence factor object from the user-provided value
            expert_system::utility::Confidence gathered_confidence((float) ui.ConfidenceEditor->value());

            // Attempt to store the data provided in the editor
            if (!raw_fact.SetValue(gathered_value, gathered_confidence)) {
                // Report the error to the user and stop attempting to save the data
                QMessageBox error_indication;
                error_indication.setText("Error: cannot save the invalid value provided!");
                error_indication.exec();
            }

            // Finished
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Get a reference to the raw Fact
            auto& raw_fact = std::get<expert_system::knowledge::facts::EnumFact>(search_result.value().get().fact_);

            // Get the user-provided value
            auto gathered_value = raw_fact.enum_.At(ui.ValueEditor->text().toStdString());
            if (!gathered_value.has_value()) {
                // Report the error to the user and stop attempting to save the data
                QMessageBox error_indication;
                error_indication.setText("Error: cannot save the invalid value provided!");
                error_indication.exec();
                break;
            }

            // Create a confidence factor object from the user-provided value
            expert_system::utility::Confidence gathered_confidence((float) ui.ConfidenceEditor->value());

            // Attempt to store the data provided in the editor
            if (!raw_fact.fact_.SetValue(gathered_value.value(), gathered_confidence)) {
                // Report the error to the user and stop attempting to save the data
                QMessageBox error_indication;
                error_indication.setText("Error: cannot save the invalid value provided!");
                error_indication.exec();
            }

            // Finished
            break;
        }
        default: {
            // Clear the current selection and reset the editors
            selected_fact_ = std::nullopt;
            UpdateEditor();
            break;
        }
    }
}

void ConsultationBegin::ClearEditor() {
    // Clear the contents of everything except the Fact list
    ui.TypeIndicator->clear();
    ui.ValueEditor->clear();
    ui.ConfidenceEditor->clear();

    // Disable interacting with the editor
    ui.EditorFrame->setEnabled(false);
}

void ConsultationBegin::UpdateEditor() {
    // Ensure the editor is cleared out to avoid bringing cruft
    ClearEditor();

    // Catch if the editors should be locked
    if (!selected_fact_.has_value()) {
        // Simply stop
        return;
    }

    // Attempt to gather the Fact
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto search_result = database.Find(selected_fact_.value());
    if (search_result == std::nullopt) {
        // Simply stop
        return;
    }

    // Process the Fact in its raw type
    auto& fact_reference = search_result.value().get();
    switch (fact_reference.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::BoolFact>(fact_reference.fact_);
            ui.TypeIndicator->setText("Boolean");

            // Configure the Value editor's validator
            static QRegExp boolExpr("True|False");
            ui.ValueEditor->setValidator(new QRegExpValidator(boolExpr, this));

            // Gather and present the Fact's session value data if it has one
            auto existing_fact_value = fact_raw.GetValue();
            if (existing_fact_value.has_value()) {
                // Present the raw value
                if (existing_fact_value.value().value_) {
                    ui.ValueEditor->setText("True");
                } else {
                    ui.ValueEditor->setText("False");
                }

                // Present the confidence factor
                ui.ConfidenceEditor->setValue((double) existing_fact_value.value().confidence_factor_.Get());
            }

            // Enable the editor
            ui.EditorFrame->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Track the raw Fact and indicate its type
            auto& fact_raw = std::get<expert_system::knowledge::facts::IntFact>(fact_reference.fact_);
            ui.TypeIndicator->setText("Integer");

            // Configure the Value editor's validator
            ui.ValueEditor->setValidator(new QIntValidator(std::numeric_limits<int>::min(),
                                                           std::numeric_limits<int>::max(),
                                                           this));

            // Gather and present the Fact's session value data if it has one
            auto existing_fact_value = fact_raw.GetValue();
            if (existing_fact_value.has_value()) {
                // Present the raw value
                ui.ValueEditor->setText(QString::number(existing_fact_value.value().value_));

                // Present the confidence factor
                ui.ConfidenceEditor->setValue((double) existing_fact_value.value().confidence_factor_.Get());
            }

            // Enable the editor
            ui.EditorFrame->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Track the raw Fact and indicate its type
            auto& fact_raw = std::get<expert_system::knowledge::facts::FloatFact>(fact_reference.fact_);
            ui.TypeIndicator->setText("Float");

            // Configure the Value editor's validator
            ui.ValueEditor->setValidator(new QDoubleValidator((double) std::numeric_limits<float>::min(),
                                                              (double) std::numeric_limits<float>::max(),
                                                              std::numeric_limits<float>::digits, this));

            // Gather and present the Fact's session value data if it has one
            auto existing_fact_value = fact_raw.GetValue();
            if (existing_fact_value.has_value()) {
                // Present the raw value
                ui.ValueEditor->setText(QString::number(existing_fact_value.value().value_));

                // Present the confidence factor
                ui.ConfidenceEditor->setValue((double) existing_fact_value.value().confidence_factor_.Get());
            }

            // Enable the editor
            ui.EditorFrame->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Track the raw Fact and indicate its type
            auto& fact_raw = std::get<expert_system::knowledge::facts::EnumFact>(fact_reference.fact_);
            ui.TypeIndicator->setText("Enumeration");

            // Configure the Value editor's validator
            QString enumExprStr;
            auto enum_list = fact_raw.enum_.List();
            int element_counter = 0;
            for (auto& current_enum: enum_list) {
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

            // Gather and present the Fact's session value data if it has one
            auto existing_fact_value = fact_raw.fact_.GetValue();
            if (existing_fact_value.has_value()) {
                // Present the raw value
                ui.ValueEditor->setText(fact_raw.enum_.At(existing_fact_value.value().value_).value().c_str());

                // Present the confidence factor
                ui.ConfidenceEditor->setValue((double) existing_fact_value.value().confidence_factor_.Get());
            }

            // Enable the editor
            ui.EditorFrame->setEnabled(true);
            break;
        }
        default: {
            // Simply stop
            break;
        }
    }
}
