#include "MainMenu.h"

#include <fstream>
#include <iomanip>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "nlohmann/json.hpp"

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
    QString file_name = QFileDialog::getOpenFileName(
            this,
            tr("Save Expert System File"),
            "/home",
            tr("Expert System JSON (*.json);;All files (*)"));
    if (file_name.isEmpty()) {
        // Catch the invalid file name and stop
        return;
    }

    // Attempt to open the requested file
    std::ofstream file_stream(file_name.toUtf8().constData(),
                              std::ofstream::out | std::ofstream::ate);
    if (!file_stream.is_open()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText(tr("Unable to open file \"") + file_name + tr("\""));
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
            tr("Save Expert System File"),
            "/home",
            tr("Expert System JSON (*.json);;All files (*)"));
    if (file_name.isEmpty()) {
        // Catch the invalid file name and stop
        return;
    }

    // Attempt to open the requested file
    std::ifstream file_stream(file_name.toUtf8().constData(), std::ifstream::in);
    if (!file_stream.is_open()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText(tr("Unable to open file \"") + file_name + tr("\""));
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
        error_indication.setText(tr("Could not load the Fact Database from \"") + file_name + tr("\""));
        error_indication.exec();
        return;
    }
    if (json_store.find(expert_system::utility::JSON_ID_DATABASE_RULES) == json_store.end()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText(tr("Could not load the Rule Database from \"") + file_name + tr("\""));
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

}

void MainMenu::FactsDomainKnowledge() {

}

void MainMenu::ForwardInferrencingConsult() {

}

void MainMenu::BackwardInferrencingConsult() {

}
