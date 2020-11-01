#include "FactEditor.h"

#include <limits>
#include <string>

#include <QDoubleValidator>
#include <QIntValidator>
#include <QListWidget>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>

#include "gui/editor/facts/NewFact.h"
#include "knowledge/facts/Facts.hpp"
#include "knowledge/facts/FactDatabase.hpp"
#include "utility/Singleton.hpp"

FactEditor::FactEditor(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);

    // Update the Fact list
    UpdateFactList();
}

void FactEditor::NewFact() {
    // Open the NewFact dialog
    class NewFact modal_dialog(this);
    modal_dialog.exec();

    // Update the Fact list
    UpdateFactList();
}

void FactEditor::DeleteFact() {
    // Find the selected list Fact
    if (selectedFact == std::nullopt) {
        // Catch no Fact being selected, and stop
        return;
    }

    // Get the selected Fact's name and delete it
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    database.Remove(selectedFact.value());
    UpdateFactList();

    // Clear the fact editors to prevent accessing invalid Facts
    selectedFact = std::nullopt;
    ClearFactEditors();
}

void FactEditor::SaveFact() {
    //TODO: THIS
}

void FactEditor::FactSelected() {
    // Track the selected Fact's name
    auto selection = ui.FactList->currentItem();
    if (selection == nullptr) {
        // Indicate no Fact is currently stored and stop
        selectedFact = std::nullopt;
        ClearFactEditors();
        return;
    }

    // Gather the name of the Fact
    selectedFact = selection->text().toStdString();
    UpdateFactEditors();
}

void FactEditor::ClearFactEditors() {
    // Clear and lock type editor
    ui.TypeIndicator->setText("");
    ui.Type->setEnabled(false);

    // Clear and lock the description editor
    ui.DescriptionEditor->setPlainText("");
    ui.DescriptionFrame->setEnabled(false);

    // Clear and lock the range editor
    ui.RangeCheckbox->setChecked(false);
    ui.MinimumEditor->setText("");
    ui.MinimumEditor->setValidator(nullptr);
    ui.MaximumEditor->setText("");
    ui.MaximumEditor->setValidator(nullptr);
    ui.InclusiveCheckbox->setEnabled(false);
    ui.RangeFrame->setEnabled(false);

    // Clear and lock the enumeration editor
    ui.EnumerationEditor->clear();
    ui.EnumerationFrame->setEnabled(false);
}

void FactEditor::UpdateFactEditors() {
    // Clear the Fact editors to ensure that no data may be left over
    ClearFactEditors();

    // Catch if the editors should be locked
    if (selectedFact == std::nullopt) {
        // Simply stop
        return;
    }

    // Attempt to gather the Fact
    auto& database = expert_system::utility::Singleton<expert_system::knowledge::facts::FactDatabase>::Get();
    auto search_result = database.Find(selectedFact.value());
    if (search_result == std::nullopt) {
        // Simply stop
        return;
    }

    // Process each Fact by its type
    auto& fact_reference = search_result.value().get();
    switch (fact_reference.type_) {
        case expert_system::utility::ExpertSystemTypes::kBool: {
            // Display the Fact's type
            ui.TypeIndicator->setText("Boolean");
            ui.Type->setEnabled(true);

            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::BoolFact>(fact_reference.fact_);

            // Display the Fact's description
            ui.DescriptionEditor->setPlainText(fact_raw.GetDescription().c_str());
            ui.DescriptionFrame->setEnabled(true);

            // Check if the Fact has a Range
            if (fact_raw.GetRange() != std::nullopt) {
                // Keep track of the Range
                auto fact_range = fact_raw.GetRange();

                // Assign the Range editor check
                ui.RangeCheckbox->setChecked(true);

                // Assign the minimum range value and validator
                static QRegExp boolExpr("True|False");
                ui.MinimumEditor->setValidator(new QRegExpValidator(boolExpr, this));
                if (fact_range->min_) {
                    ui.MinimumEditor->setText("True");
                } else {
                    ui.MinimumEditor->setText("False");
                }

                // Assign the maximum range value and validator
                ui.MaximumEditor->setValidator(new QRegExpValidator(boolExpr, this));
                if (fact_range->max_) {
                    ui.MaximumEditor->setText("True");
                } else {
                    ui.MaximumEditor->setText("False");
                }

                // Assign the inclusive flag
                if (fact_range->bounds_inclusive_) {
                    ui.InclusiveCheckbox->setChecked(true);
                }

                // Enable the range contents
                ui.RangeContents->setEnabled(true);
            }
            ui.RangeFrame->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kInt: {
            // Display the Fact's type
            ui.TypeIndicator->setText("Integer");
            ui.Type->setEnabled(true);

            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::IntFact>(fact_reference.fact_);

            // Display the Fact's description
            ui.DescriptionEditor->setPlainText(fact_raw.GetDescription().c_str());
            ui.DescriptionFrame->setEnabled(true);

            // Check if the Fact has a Range
            if (fact_raw.GetRange() != std::nullopt) {
                // Keep track of the Range
                auto fact_range = fact_raw.GetRange();

                // Assign the Range editor check
                ui.RangeCheckbox->setChecked(true);

                // Assign the minimum range value and validator
                ui.MinimumEditor->setValidator(new QIntValidator(std::numeric_limits<int>::min(),
                                                                 std::numeric_limits<int>::max(),
                                                                 this));
                ui.MinimumEditor->setText(QString::number(fact_range->min_));

                // Assign the minimum range value and validator
                ui.MinimumEditor->setValidator(new QIntValidator(std::numeric_limits<int>::min(),
                                                                 std::numeric_limits<int>::max(),
                                                                 this));
                ui.MinimumEditor->setText(QString::number(fact_range->max_));

                // Assign the inclusive flag
                if (fact_range->bounds_inclusive_) {
                    ui.InclusiveCheckbox->setChecked(true);
                }

                // Enable the range contents
                ui.RangeContents->setEnabled(true);
            }
            ui.RangeFrame->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kFloat: {
            // Display the Fact's type
            ui.TypeIndicator->setText("Float");
            ui.Type->setEnabled(true);

            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::FloatFact>(fact_reference.fact_);

            // Display the Fact's description
            ui.DescriptionEditor->setPlainText(fact_raw.GetDescription().c_str());
            ui.DescriptionFrame->setEnabled(true);

            // Check if the Fact has a Range
            if (fact_raw.GetRange() != std::nullopt) {
                // Keep track of the Range
                auto fact_range = fact_raw.GetRange();

                // Assign the Range editor check
                ui.RangeCheckbox->setChecked(true);

                // Assign the minimum range value and validator
                ui.MinimumEditor->setValidator(new QDoubleValidator((double) std::numeric_limits<float>::min(),
                                                                    (double) std::numeric_limits<float>::max(),
                                                                    std::numeric_limits<float>::digits, this));
                ui.MinimumEditor->setText(QString::number(fact_range->min_));

                // Assign the minimum range value and validator
                ui.MinimumEditor->setValidator(new QDoubleValidator((double) std::numeric_limits<float>::min(),
                                                                    (double) std::numeric_limits<float>::max(),
                                                                    std::numeric_limits<float>::digits, this));
                ui.MinimumEditor->setText(QString::number(fact_range->max_));

                // Assign the inclusive flag
                if (fact_range->bounds_inclusive_) {
                    ui.InclusiveCheckbox->setChecked(true);
                }

                // Enable the range contents
                ui.RangeContents->setEnabled(true);
            }
            ui.RangeFrame->setEnabled(true);
            break;
        }
        case expert_system::utility::ExpertSystemTypes::kEnum: {
            // Display the Fact's type
            ui.TypeIndicator->setText("Enumeration");
            ui.Type->setEnabled(true);

            // Track the raw Fact
            auto& fact_raw = std::get<expert_system::knowledge::facts::EnumFact>(fact_reference.fact_);

            // Display the Fact's description
            ui.DescriptionEditor->setPlainText(fact_raw.fact_.GetDescription().c_str());
            ui.DescriptionFrame->setEnabled(true);

            // Gather the Fact's enumeration and create it's regular expression string
            QString enumExprStr;
            auto enum_list = fact_raw.enum_.List();
            int element_counter = 0;
            for (auto& current_enum: enum_list) {
                // Append the current enum value to the string
                enumExprStr += current_enum.c_str();

                // Add the current enum value to the UI
                ui.EnumerationEditor->addItem(current_enum.c_str());

                // Prevent attaching the separator to the very end of the string
                if (element_counter != (int) enum_list.size() - 1) {
                    // Append the regex separator
                    enumExprStr += "|";
                }
                element_counter++;
            }

            // Check if the Fact has a Range
            if (fact_raw.fact_.GetRange() != std::nullopt) {
                // Keep track of the Range
                auto fact_range = fact_raw.fact_.GetRange();

                // Assign the Range editor check
                ui.RangeCheckbox->setChecked(true);

                // Create the regular expression for the Fact's enumeration
                QRegExp enumExpr(enumExprStr);

                // Assign the minimum range value and validator
                ui.MinimumEditor->setValidator(new QRegExpValidator(enumExpr, this));
                ui.MinimumEditor->setText(fact_raw.enum_.At(fact_range.value().min_).value().c_str());

                // Assign the minimum range value and validator
                ui.MinimumEditor->setValidator(new QRegExpValidator(enumExpr, this));
                ui.MinimumEditor->setText(fact_raw.enum_.At(fact_range.value().max_).value().c_str());

                // Assign the inclusive flag
                if (fact_range->bounds_inclusive_) {
                    ui.InclusiveCheckbox->setChecked(true);
                }

                // Enable the range contents
                ui.RangeContents->setEnabled(true);
            }
            ui.RangeFrame->setEnabled(true);
            break;
        }
        default: {
            // Simply stop
            break;
        }
    }
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

    // Reset the Fact editors
    selectedFact = std::nullopt;
    UpdateFactEditors();
}
