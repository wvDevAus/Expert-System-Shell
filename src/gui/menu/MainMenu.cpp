#include "MainMenu.h"

#include <fstream>
#include <iomanip>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "nlohmann/json.hpp"

#include "engines/inference/Forward.hpp"
#include "gui/editor/facts/FactEditor.h"
#include "gui/editor/rules/RuleEditor.h"
#include "gui/menu/ConsultationBegin.h"
#include "gui/menu/AdditionalRequest.h"
#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/rules/RuleDatabase.hpp"
#include "utility/Singleton.hpp"
#include "utility/SymbolsJSON.hpp"

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);
}

void MainMenu::SaveExpertSystem() {
    // Present the dialog and get a file name
    QString file_name = QFileDialog::getSaveFileName(
            this,
            "Save Expert System",
            "./",
            "Expert System JSON (*.json);;All files (*)");
    if (file_name.isEmpty()) {
        // Catch the invalid file name and stop
        return;
    }

    // Attempt to open the requested file
    std::ofstream file_stream(file_name.toStdString(),
                              std::ofstream::out | std::ofstream::ate);
    if (!file_stream.is_open()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Unable to open file \"" + file_name + "\"");
        error_indication.exec();
        return;
    }

    // Parse the current state of the databases into JSON format
    nlohmann::json json_store;
    json_store[expert_system::utility::JSON_ID_DATABASE_FACTS]
            = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    json_store[expert_system::utility::JSON_ID_DATABASE_RULES]
            = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();

    // Store the JSON object into the file stream and finish
    file_stream << std::setw(4) << json_store << std::endl;
    file_stream.close();
}

void MainMenu::LoadExpertSystem() {
    // Present the dialog and get a file name
    QString file_name = QFileDialog::getOpenFileName(
            this,
            "Load Expert System",
            "./",
            "Expert System JSON (*.json);;All files (*)");
    if (file_name.isEmpty()) {
        // Catch the invalid file name and stop
        return;
    }

    // Attempt to open the requested file
    std::ifstream file_stream(file_name.toStdString(), std::ifstream::in);
    if (!file_stream.is_open()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Unable to open file \"" + file_name + "\"");
        error_indication.exec();
        return;
    }

    // Parse the file data into a JSON object
    nlohmann::json json_store;
    file_stream >> json_store;
    file_stream.close();

    // Catch not all data being loaded
    if (json_store.find(expert_system::utility::JSON_ID_DATABASE_FACTS) == json_store.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Could not load the Fact Database from \"" + file_name + "\"");
        error_indication.exec();
        return;
    }
    if (json_store.find(expert_system::utility::JSON_ID_DATABASE_RULES) == json_store.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Could not load the Rule Database from \"" + file_name + "\"");
        error_indication.exec();
        return;
    }

    // Parse and store the databases
    expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get()
        = json_store.at(expert_system::utility::JSON_ID_DATABASE_FACTS)
                    .get<expert_system::knowledge::facts::FactDatabase>();
    expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get()
        = json_store.at(expert_system::utility::JSON_ID_DATABASE_RULES)
                    .get<expert_system::knowledge::rules::RuleDatabase>();
}

void MainMenu::RulesDomainKnowledge() {
    // Prevent this window from being opened if no Facts have been created.
    auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    if (fact_database.Count() == 0) {
        // Report the error
        QMessageBox error_indication;
        error_indication.setText("Error: Rules cannot be edited or created while no Facts exist!");
        error_indication.exec();
        return;
    }

    // Open the RuleEditor dialog
    RuleEditor modal_dialog(this);
    modal_dialog.exec();
}

void MainMenu::FactsDomainKnowledge() {
    // Open the FactEditor dialog
    FactEditor modal_dialog(this);
    modal_dialog.exec();
}

void MainMenu::ForwardInferrencingConsult() {
    // Keep track of the two databases
    auto& fact_database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto& rule_database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();

    // Keep track of the events that occur while inferring
    std::vector<std::vector<expert_system::engines::explanation::Log>> logged_events;

    // Begin the consultation by gathering a set of existing values from the user
    ConsultationBegin initial_dialog(this);
    initial_dialog.exec();
    std::vector<expert_system::engines::explanation::Log> initial_user_input_log;
    initial_user_input_log.push_back(expert_system::engines::inference::forward::LogExisting(fact_database));
    logged_events.push_back(initial_user_input_log);

    // Continue to loop through the inference process
    while (true) {
        // Catch if any facts do not have a session value
        if (fact_database.List(expert_system::knowledge::facts::FactFilter::kHasNoValue).empty()) {
            // Stop the consultation and show the results
            break;
        }

        // Catch if any rules have not yet been triggered
        if (rule_database.ListRules(expert_system::knowledge::rules::RuleFilter::kHasNotRunConsequent).empty()) {
            // Stop the consultation and show the results
            break;
        }

        // Find the triggered Rules
        auto trigger_list = expert_system::engines::inference::forward::FindTriggered(
                fact_database,
                rule_database);

        // Catch if any rules were actually triggered
        if (!trigger_list.empty()) {
            // Run the triggered Rule's Consequents and log the outcomes
            logged_events.push_back(expert_system::engines::inference::forward::RunTriggered(
                    trigger_list,
                    fact_database));
        }
        else {
            // Identify the Fact session values needed to trigger the remaining rules
            auto needed_fact_values =
                    expert_system::engines::inference::forward::IdentifyFactRequests(
                            fact_database,
                            rule_database);

            // Generate a dialog to request one of these missing fact session values
            AdditionalRequest request_dialog(needed_fact_values,
                                             logged_events,
                                             this);

            // Catch if the user wanted to finish the consultation
            if (request_dialog.exec() == QDialog::Rejected) {
                // Stop the consultation and show the results
                break;
            }
        }
    }

    // Display the consultation results


    // Make sure to clean up before finishing
    fact_database.Reset();
    rule_database.ResetRules();
}

void MainMenu::BackwardInferrencingConsult() {
    // Indicate this is currently not supported
    QMessageBox error_indication;
    error_indication.setText("Backwards Inferencing is currently not supported.");
    error_indication.exec();
}
