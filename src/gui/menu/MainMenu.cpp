#include "MainMenu.h"

#include <fstream>
#include <iomanip>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "nlohmann/json.hpp"

#include "gui/editor/facts/FactEditor.h"
#include "gui/editor/rules/RuleEditor.h"
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
    // Prevent this window from being opened if Rules have been created.
    auto& rule_database = expert_system::utility::Singleton<expert_system::knowledge::rules::RuleDatabase>::Get();
    if ((int) rule_database.managed_rules_.size() != 0) {
        // Report the error
        QMessageBox error_indication;
        error_indication.setText("Error: Facts cannot be edited while Rules exist!");
        error_indication.exec();
        return;
    }

    // Open the FactEditor dialog
    FactEditor modal_dialog(this);
    modal_dialog.exec();
}

void MainMenu::ForwardInferrencingConsult() {
    // Open the ConsultForwardsChain dialog
    // TODO: Create the ConsultForwardsChain dialog
}

void MainMenu::BackwardInferrencingConsult() {
    // Open the ConsultBackwardsChain dialog
    // TODO: Create the ConsultBackwardsChain dialog
}
