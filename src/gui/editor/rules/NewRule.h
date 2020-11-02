#pragma once

#include <QDialog>

#include "ui_NewRule.h"

    /**
     * @brief Expert System Shell Rule creation GUI.
     * Implemented through the Member Variable method.
     */
class NewRule : public QDialog {
Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit NewRule(QWidget *parent = nullptr);

private slots:
        /// Attempts to create a new Rule using the data provided by the user.
    void CreateNewRule();

private:
        /// Pointer member to auto-generated UI element.
    Ui::NewRule ui;
};
