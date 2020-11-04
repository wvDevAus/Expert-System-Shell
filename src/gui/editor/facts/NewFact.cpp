#include "NewFact.h"

#include <string>

#include <QMessageBox>

#include "knowledge/facts/FactDatabase.hpp"
#include "utility/Singleton.hpp"

NewFact::NewFact(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Fill the Type selector's options
    ui.TypeSelection->insertItem(0, "Boolean");
    ui.TypeSelection->insertItem(1, "Integer");
    ui.TypeSelection->insertItem(2, "Float");
    ui.TypeSelection->insertItem(3, "Enumeration");
}

void NewFact::CreateNewFact() {
    // Catch an empty name
    if (ui.NameEdit->text().isEmpty()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: provided name is empty!");
        error_indication.exec();
        return;
    }

    // Catch a known name
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    if (database.Find(ui.NameEdit->text().toStdString()) != std::nullopt) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: provided name is already used by another Fact!");
        error_indication.exec();
        return;
    }

    // Generate the Fact
    switch (ui.TypeSelection->currentIndex()) {
        case 0: {
            // Create the Fact
            database.Create(ui.NameEdit->text().toStdString(),
                            expert_system::utility::ExpertSystemTypes::kBool);
            break;
        }
        case 1: {
            // Create the Fact
            database.Create(ui.NameEdit->text().toStdString(),
                            expert_system::utility::ExpertSystemTypes::kInt);
            break;
        }
        case 2: {
            // Create the Fact
            database.Create(ui.NameEdit->text().toStdString(),
                            expert_system::utility::ExpertSystemTypes::kFloat);
            break;
        }
        case 3: {
            // Create the Fact
            database.Create(ui.NameEdit->text().toStdString(),
                            expert_system::utility::ExpertSystemTypes::kEnum);
            break;
        }
        default:{
            // Catch the error and indicate failure
            QMessageBox error_indication;
            error_indication.setText("Error: unexpected Fact type provided!");
            error_indication.exec();
            return;
        }
    }

    // End the Dialog
    this->accept();
}
