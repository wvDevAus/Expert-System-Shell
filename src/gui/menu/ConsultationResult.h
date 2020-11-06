#pragma once

#include <vector>

#include <QDialog>

#include "engines/explanation/Log.hpp"

#include "ui_ConsultationResult.h"

    /**
     * @brief Expert System Shell consultation result display UI.
     * Implemented through the Member Variable method.
     */
class ConsultationResult : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit ConsultationResult(std::vector<std::vector<expert_system::engines::explanation::Log>>& logs,
                                QWidget *parent = nullptr);

private slots:
        /// Attempts to manage a selection of a Fact.
    void FactSelected();

private:
        /// Pointer member to auto-generated UI element.
    Ui::ConsultationResult ui;
};
