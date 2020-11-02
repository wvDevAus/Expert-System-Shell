#pragma once

#include <optional>
#include <string>

#include <QDialog>

#include "ui_FactEditor.h"

    /**
     * @brief Expert System Shell Fact editor GUI.
     * Implemented through the Member Variable method.
     */
class FactEditor : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit FactEditor(QWidget *parent = nullptr);

private slots:
        /// Opens a new dialog for the user to create a new Fact.
    void NewFact();

        /// Attempts to delete the currently selected Fact.
    void DeleteFact();

        /// Attempts to save the currently selected Fact from the current editor values.
    void SaveFact();

        /// Processes a selected Fact.
    void FactSelected();

        /// Updates the Range editor UI to be locked/unlocked.
    void UpdateRangeLock();

private:
        /// Disables and removes the data from all editors.
    void ClearFactEditors();

        /// Updates the editor elements to reflect the selected Fact.
    void UpdateFactEditors();

        /// Updates the list of Stored facts.
    void UpdateFactList();

        /// Stores the name of the currently selected Fact.
    std::optional<std::string> selectedFact;

        /// Pointer member to auto-generated UI element.
    Ui::FactEditor ui;
};
