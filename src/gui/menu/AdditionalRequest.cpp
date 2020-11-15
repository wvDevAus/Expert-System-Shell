#include "AdditionalRequest.h"

#include <QMessageBox>

#include "engines/inference/Forward.hpp"
#include "utility/Singleton.hpp"

AdditionalRequest::AdditionalRequest(
        const std::set<std::string>& requested_facts,
        std::vector<std::vector<expert_system::engines::explanation::Log>>& existing_log,
        QWidget *parent) : QDialog(parent), existing_log_(existing_log) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Iterate through the requested Facts
    for (auto& current_fact: requested_facts) {
        // Add the current Fact name to the selector
        ui.FactSelector->addItem(current_fact.c_str());
    }

    // Make sure the initial target fact identifies the listed fact at index 0
    selected_fact_ = ui.FactSelector->currentText().toStdString();

    // Update the editor
    UpdateEditor();
}

void AdditionalRequest::FactSelected() {
    // Store the name of the Fact selected
    selected_fact_ = ui.FactSelector->currentText().toStdString();

    // Update the UI
    UpdateEditor();
}

void AdditionalRequest::SaveAndContinue() {
    // Get the selected Fact
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto search_result = database.Find(selected_fact_);

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
                error_indication.setText("Error: the provided value is outside of the Fact's range!");
                error_indication.exec();
                return;
            }

            // Log the user's assignment of the Fact's value
            std::vector<expert_system::engines::explanation::Log> this_assignment_log;
            this_assignment_log.push_back(expert_system::engines::inference::forward::LogSpecific(selected_fact_, database).value());
            existing_log_.get().push_back(this_assignment_log);
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
                error_indication.setText("Error: the provided value is outside of the Fact's range!");
                error_indication.exec();
                return;
            }

            // Log the user's assignment of the Fact's value
            std::vector<expert_system::engines::explanation::Log> this_assignment_log;
            this_assignment_log.push_back(expert_system::engines::inference::forward::LogSpecific(selected_fact_, database).value());
            existing_log_.get().push_back(this_assignment_log);
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
                error_indication.setText("Error: the provided value is outside of the Fact's range!");
                error_indication.exec();
                return;
            }

            // Log the user's assignment of the Fact's value
            std::vector<expert_system::engines::explanation::Log> this_assignment_log;
            this_assignment_log.push_back(expert_system::engines::inference::forward::LogSpecific(selected_fact_, database).value());
            existing_log_.get().push_back(this_assignment_log);
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
                error_indication.setText("Error: the provided value is not a valid enumeration from the Fact!");
                error_indication.exec();
                return;
            }

            // Create a confidence factor object from the user-provided value
            expert_system::utility::Confidence gathered_confidence((float) ui.ConfidenceEditor->value());

            // Attempt to store the data provided in the editor
            if (!raw_fact.fact_.SetValue(gathered_value.value(), gathered_confidence)) {
                // Report the error to the user and stop attempting to save the data
                QMessageBox error_indication;
                error_indication.setText("Error: the provided value is outside of the Fact's range!");
                error_indication.exec();
                return;
            }

            // Log the user's assignment of the Fact's value
            std::vector<expert_system::engines::explanation::Log> this_assignment_log;
            this_assignment_log.push_back(expert_system::engines::inference::forward::LogSpecific(selected_fact_, database).value());
            existing_log_.get().push_back(this_assignment_log);
            break;
        }
        default: {
            // Don't attempt to log or store anything
            QMessageBox error_indication;
            error_indication.setText("Error: cannot save to the invalid Fact!");
            error_indication.exec();
            return;
        }
    }

    // Indicate the dialog has finished
    this->accept();
}

void AdditionalRequest::UpdateEditor() {
    // Remove the existing data and validator from the editor
    ui.ValueEditor->setValidator(nullptr);
    ui.ValueEditor->clear();

    // Attempt to gather the Fact
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto search_result = database.Find(selected_fact_);
    if (search_result == std::nullopt) {
        // Lock the editor and stop the rest of the update
        ui.ButtonSaveAndContinue->setEnabled(false);
        ui.ValueEditor->setEnabled(false);
        ui.ValueEditor->setText("Error");
        return;
    }

    // Process the Fact in its raw type
    auto& fact_reference = search_result.value().get();
    switch (fact_reference.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Configure the Value editor's validator
            static QRegExp boolExpr("True|False");
            ui.ValueEditor->setValidator(new QRegExpValidator(boolExpr, this));

            // Guarantee the Value editor can be used
            ui.ButtonSaveAndContinue->setEnabled(true);
            ui.ValueEditor->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Configure the Value editor's validator
            ui.ValueEditor->setValidator(new QIntValidator(std::numeric_limits<int>::min(),
                                                           std::numeric_limits<int>::max(),
                                                           this));

            // Guarantee the Value editor can be used
            ui.ButtonSaveAndContinue->setEnabled(true);
            ui.ValueEditor->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Configure the Value editor's validator
            ui.ValueEditor->setValidator(new QDoubleValidator((double) std::numeric_limits<float>::min(),
                                                              (double) std::numeric_limits<float>::max(),
                                                              std::numeric_limits<float>::digits, this));

            // Guarantee the Value editor can be used
            ui.ButtonSaveAndContinue->setEnabled(true);
            ui.ValueEditor->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::EnumFact>(fact_reference.fact_);

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

            // Guarantee the Value editor can be used
            ui.ButtonSaveAndContinue->setEnabled(true);
            ui.ValueEditor->setEnabled(true);
            break;
        }
        default: {
            // Lock the editor
            ui.ButtonSaveAndContinue->setEnabled(false);
            ui.ValueEditor->setEnabled(false);
            ui.ValueEditor->setText("Error");
            break;
        }
    }
}
