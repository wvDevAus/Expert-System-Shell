#include "FactEditor.h"

#include <QListWidget>

#include "gui/editor/facts/NewFact.h"
#include "knowledge/facts/FactDatabase.hpp"
#include "utility/Singleton.hpp"

FactEditor::FactEditor(QWidget *parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Update the Fact list
    UpdateFactList();
}

void FactEditor::NewFact() {
    // Open the NewFact dialog
    class NewFact modal_dialog(this);
    modal_dialog.exec();
}

void FactEditor::DeleteFact() {

}

void FactEditor::UpdateFactEditors() {

}

void FactEditor::UpdateFactList() {
    // Clear the FactList's contents
    ui.FactList->clear();

    // Gather the List of Fact names and iterate through it
    auto fact_names = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get().List();
    for (auto& current_name: fact_names) {
        // Add the Fact name to the list
        new QListWidgetItem(current_name.c_str(), ui.FactList);
    }
}
