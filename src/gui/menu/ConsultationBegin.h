#pragma once

#include <optional>

#include <QMainWindow>

#include "ui_ConsultationBegin.h"

    /**
     * @brief Expert System Shell consultation initial data request.
     * Implemented through the Member Variable method.
     */
class ConsultationBegin : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit ConsultationBegin(QWidget *parent = nullptr);

private slots:
        /// Attempts to manage a selection of a Fact.
    void FactSelected();

        /// Attempts to save the current value provided for a Fact's session.
    void SaveFactValue();

private:
        /// Clears the contents of the editor and disables its interactivity.
    void ClearEditor();

        /// Updates the contents of the editor and enables its interacivity.
    void UpdateEditor();

        /// Keeps track of the name of the selected Fact.
    std::optional<std::string> selected_fact_;

        /// Pointer member to auto-generated UI element.
    Ui::ConsultationBegin ui;
};