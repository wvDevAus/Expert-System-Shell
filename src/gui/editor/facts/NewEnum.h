#pragma once

#include <optional>

#include <QDialog>

#include "knowledge/facts/Facts.hpp"

#include "ui_NewEnum.h"

    /**
     * @brief Expert System Shell Enum creation GUI.
     * Implemented through the Member Variable method.
     */
class NewEnum : public QDialog {
Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit NewEnum(QWidget *parent = nullptr,
                     std::optional<std::reference_wrapper<expert_system::knowledge::facts::EnumFact>> target_fact = std::nullopt);

private slots:
        /// Creates a new Enum using the data it is currently storing.
    void CreateNewEnum();

private:
        /// Stores the name of the currently selected Fact.
    std::optional<std::reference_wrapper<expert_system::knowledge::facts::EnumFact>> targetFact;

        /// Pointer member to auto-generated UI element.
    Ui::NewEnum ui;
};
