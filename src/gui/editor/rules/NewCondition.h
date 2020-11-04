#pragma once

#include <optional>

#include <QDialog>

#include "knowledge/rules/Rule.hpp"

#include "ui_NewCondition.h"

    /**
     * @brief Expert System Shell Condition creation GUI.
     * Implemented through the Member Variable method.
     */
class NewCondition : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit NewCondition(QWidget *parent = nullptr,
                          std::optional<std::reference_wrapper<expert_system::knowledge::rules::Rule>> current_rule = std::nullopt);

private slots:
        /// Attempts to create a new Condition using the data provided by the user.
    void CreateNewCondition();

private:
        /// Stores a reference to the Rule currently being edited.
    std::optional<std::reference_wrapper<expert_system::knowledge::rules::Rule>> current_rule_;

        /// Pointer member to auto-generated UI element.
    Ui::NewCondition ui;
};
