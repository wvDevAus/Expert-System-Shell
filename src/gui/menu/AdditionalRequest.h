#pragma once

#include <optional>

#include <QDialog>

#include "engines/explanation/Log.hpp"

#include "ui_AdditionalRequest.h"

    /**
     * @brief Expert System Shell consultation initial data request.
     * Implemented through the Member Variable method.
     */
class AdditionalRequest : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit AdditionalRequest(
            const std::set<std::string>& requested_facts,
            std::vector<std::vector<expert_system::engines::explanation::Log>>& existing_log,
            QWidget *parent = nullptr);

private slots:
        /// Attempts to manage a selection of a Fact.
    void FactSelected();

        /// Attempts to save the current value provided for a Fact's session.
    void SaveAndContinue();

private:
        /// Updates the contents of the editor and enables its interacivity.
    void UpdateEditor();

        /// Stores a reference to an existing log to output the events of this dialog into
    std::reference_wrapper<std::vector<std::vector<expert_system::engines::explanation::Log>>> existing_log_;

        /// Keeps track of the name of the selected Fact.
    std::string selected_fact_;

        /// Pointer member to auto-generated UI element.
    Ui::AdditionalRequest ui;
};
