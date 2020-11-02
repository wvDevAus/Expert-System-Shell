#include "NewRule.h"

#include <utility>
#include <string>

#include <QMessageBox>

#include "knowledge/rules/Rule.hpp"
#include "knowledge/rules/RuleDatabase.hpp"
#include "utility/Singleton.hpp"

NewRule::NewRule(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);
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

    // Generate the Rule and end the dialog
    database.managed_rules_.emplace(name_string,
                                    expert_system::knowledge::rules::Rule());
    this->accept();
}
