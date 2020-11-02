#include "NewEnum.h"

#include <utility>
#include <string>

#include <QMessageBox>

#include "knowledge/facts/FactDatabase.hpp"
#include "utility/Singleton.hpp"

NewEnum::NewEnum(QWidget *parent,
                 std::optional<std::reference_wrapper<expert_system::knowledge::facts::EnumFact>> target_fact)
        : QDialog(parent), targetFact(target_fact) {
    // Ensure that the UI is configured
    ui.setupUi(this);
}

void NewEnum::CreateNewEnum() {
    // Catch an empty fact
    if (!targetFact.has_value()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: cannot edit invalid Fact!");
        error_indication.exec();
        this->reject();
    }

    // Catch an empty enum name
    if (ui.NameEdit->text().isEmpty()) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: provided name is empty!");
        error_indication.exec();
        return;
    }

    // Catch a known enum name
    auto name_string = ui.NameEdit->text().toStdString();
    if (targetFact.value().get().enum_.Has(name_string)) {
        // Catch the error and indicate failure
        QMessageBox error_indication;
        error_indication.setText("Error: provided name is already used!");
        error_indication.exec();
        return;
    }

    // Generate the enum and end the dialog
    targetFact.value().get().enum_.PushBack(name_string);
    this->accept();
}
